#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include <vector>
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

class G4Event;
class PrimaryGenActionMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:
      PrimaryGeneratorAction();
      virtual ~PrimaryGeneratorAction();

public:
    virtual void GeneratePrimaries(G4Event*);
    inline double GetBeamEnergy(){return energy;}
    G4ParticleGun* GetParticleGun(){return fParticleGun;};
    void SetEnergyValue(G4double val){chosen_energy = val;}

private:
  G4double chosen_energy;
  PrimaryGenActionMessenger* genM;
  G4ParticleGun* fParticleGun;

protected:

      int particleNumber;
      G4double r;
      G4double ph;
      G4double x_r,y_r,z_r;
      G4double z0;
      G4double phi;
      G4double theta;
      G4bool randomizePrimary;
      G4float beam_offset_x,beam_offset_y,beam_size,source_width;
      G4float energy;
      double random;
      std::vector<float> e,dNde,N; //the input from the file

      void ReadInputSpectrumFile(std::string);

};


#endif
