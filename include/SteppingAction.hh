#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include "G4OpBoundaryProcess.hh"
#include "RunAction.hh"

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(G4ParticleGun*, RunAction*);
    virtual ~SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

public: // for now set to public until I can figure out how to work with protected

        std::vector<double> Ev;


private:
    bool TrackMustDie(const G4Step*);
    int EventGeneratorParticle;
    float LowEnergyCutoff;
    G4ThreeVector p;
    G4ThreeVector X;
    G4ThreeVector Xdet;
    G4ThreeVector incX;
    G4ParticleGun* particle_gun_local;
    RunAction* run;
    G4OpBoundaryProcessStatus fExpectedNextStatus;
    G4String procCount;

};

#endif
