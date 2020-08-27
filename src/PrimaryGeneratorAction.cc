#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(),fParticleGun(0){

        G4int n_particle = 1;
        fParticleGun = new G4ParticleGun(n_particle);

        // Default Kinematics

        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        particleNumber= 22; // gamma particle number
        G4ParticleDefinition *particle = particleTable->FindParticle(particleNumber);
        fParticleGun->SetParticleDefinition(particle);

        fParticleGun->SetParticleTime(0.0*ns);
        // Determine particle Beam Energy


        ReadInputSpectrumFile("input_spectrum.txt");

        // Set Beam Position
        beam_offset_x = 0*cm;
        beam_offset_y = 0*cm;
        z0 = 0*cm;
        beam_size = 5;
        source_width=0; //by default the width along Z is zero

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
        delete fParticleGun;

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

        // Set Particle Energy (Must be in generate primaries)

        random=G4UniformRand()*N[N.size()-1];

        // first order interpolation
        for(int i=0; i<N.size(); i++)
                if(N[i]>random && i>0) {
                        float f = (random - N[i-1]) / (N[i] - N[i-1]);
                        energy= f*e[i] + (1-f)*e[i-1];
                        break;
                }

        fParticleGun->SetParticleEnergy(energy);


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

}


void PrimaryGeneratorAction::ReadInputSpectrumFile(std::string filename){

        std::ifstream f(filename);
        if(f.is_open()) { //check that the file is open
                G4cout<<"oooooooooooooo Reading input file for beam energies oooooooooooo"<<G4endl;
                N.push_back(0);
                while(!f.eof()) {
                        float a,b;
                        f>>a>>b;
                        e.push_back(a);
                        dNde.push_back(b);
                        if(dNde.size()>1) N.push_back(N.at(N.size()-1)+dNde.at(dNde.size()-1)*(e.at(e.size()-1)-e.at(e.size()-2)));
                }
        }
        else {
                std::cout<<"Input file expected, however open failed, exiting."<<std::endl;
                exit(8);
        }

        f.close();
}
