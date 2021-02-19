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

extern G4long seed;
extern G4String inFile;
extern G4double chosen_energy;
extern G4bool resonanceTest;
extern G4bool bremTest;

PrimaryGeneratorAction::PrimaryGeneratorAction()
        : G4VUserPrimaryGeneratorAction(),
        fParticleGun(0)
{
        fParticleGun = new G4ParticleGun(1);
        if(chosen_energy > 0)
                G4cout << "PrimaryGeneratorAction::Beam Energy > 0" << G4endl;

        G4cout << "PrimaryGeneratorAction::Beam Position Set to: (0,0," << beamStart << ")cm" << G4endl;

        if(bremTest)
        {
                fParticleGun->SetParticleDefinition(G4Electron::Definition());
                G4cout << "Particle Type set to Electron!" << G4endl;
                file_check = false;
        }
        else
        {
                fParticleGun->SetParticleDefinition(G4Gamma::Definition());
                G4cout << "Particle Type set to Gamma!" << G4endl;
        }

        // Default Kinematics
        fParticleGun->SetParticleTime(0.0*ns);

        if(chosen_energy < 0)
        {
                gRandom->SetSeed(seed);
                r->SetSeed(seed); // setting the seed for TRandom1

                if(gSystem->AccessPathName(inFile.c_str()) != 0)
                {
                  G4cerr << "PrimaryGeneratorAction::PrimaryActionGenerator FATAL ERROR -> " << inFile << " NOT FOUND!" << G4endl;
                  exit(1);
                }
                TFile *fin = TFile::Open(inFile.c_str());
                G4String fileName = (G4String)fin->GetName();
                hBrems  = (TGraph*) fin->Get("sampleBrems");
                if(!hBrems)
                {
                  G4cerr << "PrimaryGeneratorAction::PrimaryActionGenerator FATAL ERROR -> hBrems Fail." << G4endl;
                  exit(1);
                }
                // Find Max Energy
                G4double *xvalues = hBrems->GetX();
                MaxE = TMath::MaxElement(hBrems->GetN(), xvalues);
                if(fileName.compare(0,24,"brems_distributions.root") == 0)
                {
                    file_check = false;
                    hSample = (TGraph*) fin->Get("sampleGraph");
                    if(!hSample)
                    {
                      G4cerr << "PrimaryGeneratorAction::PrimaryGeneratorAction() -> FATAL ERROR Failure to grab TGraphs from File: " << fileName << G4endl;
                      exit(1);
                    }
                }
                else
                  file_check = true;
        }
        else
        {
                file_check = false;
                G4cout << "PrimaryGeneratorAction::PrimaryGeneratorAction Chosen Energy set to: " << chosen_energy << " MeV" << G4endl;
        }

        G4cout << G4endl << "User Macro Inputs" << G4endl;
        G4cout << "----------------------------------------------------------------------" << G4endl;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
        delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // ******************** Energy Sampling Options *************************** //

        // This option reads from a input file of user's choice and samples
        // from bremsstrahlung TGraph without any importance sampling
        G4double e = 0.;
        G4double w = 1.;
        if(!resonanceTest && chosen_energy < 0)
        {
          e = MaxE*r->Rndm();
          G4double dNdE = hBrems->Eval(e)*MeV;
          // User is not importance sampling
          if(file_check)
            energy = hBrems->Eval(e)*MeV;
          // User IS using importance sampling
          else
            energy = hSample->Eval(e)*MeV;

          w = dNdE/energy;
        }
        // The user has selected a mono-energetic beam
        else if(chosen_energy > 0)
          energy = chosen_energy*MeV;
        // User wishes to sample from Uranium resonance energies
        else
          energy = SampleUResonances();

        // Set the energy
        fParticleGun->SetParticleEnergy(energy);

        const float pi=acos(-1);
        G4double beam_size = 100.0*mm;
        // Set beam position
        G4double x_r = beam_size*acos(G4UniformRand())/pi*2.*cos(360.*G4UniformRand()*CLHEP::deg);
        G4double y_r = beam_size*acos(G4UniformRand())/pi*2.*sin(360.*G4UniformRand()*CLHEP::deg);
        fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,beamStart*cm)); // set the electron beam far enough back behind brem radiator

        // Set beam momentum
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1)); // along z axis

        fParticleGun->GeneratePrimaryVertex(anEvent);

// Pass the event information
        eventInformation *anInfo = new eventInformation(anEvent);
        anInfo->SetWeight(w);
        anInfo->SetBeamEnergy(energy);
        anEvent->SetUserInformation(anInfo);
        //std::cout << "PrimaryActionGenerator::GeneratePrimaries() -> End!" << std::endl;
}

G4double PrimaryGeneratorAction::SampleUResonances() {
        std::vector<double> er;
        er.push_back(1.65624253132*MeV);
        er.push_back(1.7335537285*MeV);
        er.push_back(1.86232584382*MeV);

        G4int idx = Random.Integer(er.size());
        G4double de = 25.0*eV;

        return Random.Uniform(er[idx]-de, er[idx]+de);
}
