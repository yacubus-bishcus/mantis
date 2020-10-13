#include "PrimaryGeneratorAction.hh"
#include "PrimaryGenActionMessenger.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(),
        chosen_energy(-1), genM(NULL), fParticleGun(0)
{

        genM = new PrimaryGenActionMessenger(this);
        G4int n_particle = 1;
        fParticleGun = new G4ParticleGun(n_particle);

        // Default Kinematics
        fParticleGun->SetParticleDefinition(G4Gamma::Definition());

        fParticleGun->SetParticleTime(0.0*ns);


#if defined (G4ANALYSIS_USE_ROOT)

        // file contains the normalized brems distribution p(E), sampling distribution s(E),
        // and binary 0/1 for off/on resonance useful in weighting
        TFile *fin = TFile::Open("brems_distributions.root");
        hBrems  = (TH1D*) fin->Get("hBrems");
        hSample = (TH1D*) fin->Get("hSample");
        if (hBrems && hSample)
        {
                G4cout << "Imported brems and sampling distributions from " << fin->GetName() << G4endl << G4endl;
        }
        else
        {
                std::cerr << "Error reading from file " << fin->GetName() << std::endl;
                exit(1);
        }

#endif
        // Set Beam Position
        beam_offset_x = 0*cm;
        beam_offset_y = 0*cm;
        z0 = 0*cm;
        beam_size = 1.3*mm;
        source_width=0; //by default the width along Z is zero

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

        if(chosen_energy < 0)
        {
                energy = hSample->GetRandom()*MeV; // sample the resonances specified by hSample
        }
#else
        if(chosen_energy < 0)
        {
                std::cerr << "ERROR: G4ANALYSIS_USE_ROOT not defined in pre-compiler" << std::endl;
                std::cerr << "SYSTEM EXITING" << std::endl;
                exit(100);
        }

#endif

        if(chosen_energy > 0)
        {
                energy = chosen_energy;
        }

        fParticleGun->SetParticleEnergy(energy*MeV);


        const float pi=acos(-1);

        // Set beam position
        r = beam_size*acos(G4UniformRand())/pi*2.;
        ph = 360.*G4UniformRand()*CLHEP::deg;
        x_r = r*cos(ph);
        y_r = r*sin(ph);
        z_r = source_width*(G4UniformRand()-0.5);
        fParticleGun->SetParticlePosition(G4ThreeVector(x_r+beam_offset_x,y_r+beam_offset_y,z_r+z0));


        // Set beam momentum
        theta = 0.*CLHEP::deg;
        phi = 0.*CLHEP::deg;
        G4ThreeVector vDir;
        vDir = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
        fParticleGun->SetParticleMomentumDirection(vDir);

        fParticleGun->GeneratePrimaryVertex(anEvent);
        G4double w = 1.0;

#if defined (G4ANALYSIS_USE_ROOT)
        if(chosen_energy < 0)
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
