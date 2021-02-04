//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Author:
// Jacob E Bickus, 2021
// MIT, NSE
// jbickus@mit.edu
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGenActionMessenger.hh"
extern G4long seed;

PrimaryGeneratorAction::PrimaryGeneratorAction(G4bool brem_in, G4bool resonance_in) 
        : G4VUserPrimaryGeneratorAction(),
        bremTest(brem_in), resonance_test(resonance_in), chosen_energy(-1), 
        genM(NULL), fParticleGun(0)
{

  genM = new PrimaryGenActionMessenger(this);
  fParticleGun = new G4ParticleGun(1);

  if(bremTest)
  {
    fParticleGun->SetParticleDefinition(G4Electron::Definition());
    G4cout << "Particle Type set to Electron!" << G4endl << G4endl;
  }
  else
  {
    fParticleGun->SetParticleDefinition(G4Gamma::Definition());
    G4cout << "Particle Type set to Gamma!"<< G4endl << G4endl;
  }

  // Default Kinematics
  fParticleGun->SetParticleTime(0.0*ns);

#if defined (G4ANALYSIS_USE_ROOT)

  // file contains the normalized brems distribution p(E), sampling distribution s(E),
  // and binary 0/1 for off/on resonance useful in weighting

  if(!bremTest && !resonance_test && chosen_energy < 0)
  {
     gRandom->SetSeed(seed);
     TFile *fin = TFile::Open("brems_distributions.root");
     hBrems  = (TH1D*) fin->Get("hBrems");
     hSample = (TH1D*) fin->Get("hSample");
     if (hBrems && hSample)
     {
          G4cout << "Imported brems and sampling distributions from " << fin->GetName() << G4endl << G4endl;
     }
     else
     {
          G4cerr << "Error reading from file " << fin->GetName() << G4endl;
          exit(1);
     }
  }
  else if(!bremTest && !resonance_test && chosen_energy > 0)
  {
     G4cout << "PrimaryGeneratorAction::PrimaryGeneratorAction Chosen Energy set to: " << chosen_energy << " MeV" << G4endl;
     HistoManager* histo = new HistoManager;
     histo->SetChosenEnergy(chosen_energy);
  }
  else if(resonance_test)
  {
    G4cout << "Max Energy set to 2 MeV!" << G4endl;
    HistoManager *histo = new HistoManager;
    histo->SetChosenEnergy(2.0);
  }

#endif

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete genM;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

// Set Particle Energy (Must be in generate primaries)
#if defined (G4ANALYSIS_USE_ROOT)

  if(chosen_energy < 0 && !bremTest && !resonance_test)
  {
    energy = hSample->GetRandom()*MeV; // sample the resonances specified by hSample
  }
#else
  if(chosen_energy < 0)
  {
    G4cerr << "ERROR: G4ANALYSIS_USE_ROOT not defined in pre-compiler" << G4endl;
    G4cerr << "SYSTEM EXITING" << G4endl;
    exit(100);
  }

#endif

  if(chosen_energy > 0 && !resonance_test)
  {
    energy = chosen_energy*MeV;
  }
  else if(resonance_test)
  {
    energy = SampleUResonances();
  }

  fParticleGun->SetParticleEnergy(energy);


  const float pi=acos(-1);
  G4double beam_size = 1.3*mm;
  // Set beam position
  G4double x_r = beam_size*acos(G4UniformRand())/pi*2.*cos(360.*G4UniformRand()*CLHEP::deg);
  G4double y_r = beam_size*acos(G4UniformRand())/pi*2.*sin(360.*G4UniformRand()*CLHEP::deg);
  fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,100*cm));


  // Set beam momentum
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1)); // along z axis 

  fParticleGun->GeneratePrimaryVertex(anEvent);
  G4double w = 1.0;

#if defined (G4ANALYSIS_USE_ROOT)
  if(chosen_energy < 0 && !bremTest && !resonance_test)
  {
    G4double s = hSample->GetBinContent(hSample->GetXaxis()->FindBin(energy));
    G4double dNdE = hBrems->GetBinContent(hBrems->GetXaxis()->FindBin(energy));
    w = dNdE/s;
  }

#endif
// Pass the event information
  eventInformation *anInfo = new eventInformation(anEvent);
  anInfo->SetWeight(w);
  anInfo->SetBeamEnergy(energy);
  anEvent->SetUserInformation(anInfo);

}

G4double PrimaryGeneratorAction::SampleUResonances() {
  std::vector<double> er;
  er.push_back(1.6562*MeV);
  er.push_back(1.73354686425*MeV);
  er.push_back(1.8623*MeV);

  G4int idx = Random.Integer(er.size());
  G4double de = 25.0*eV;

  return Random.Uniform(er[idx]-de, er[idx]+de);
}
