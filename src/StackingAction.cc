#include "StackingAction.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

StackingAction::StackingAction(EventAction *ea)
        : fEventAction(ea)
{

}


StackingAction::~StackingAction()
{
}

void StackingAction::NewStage(){

        //G4cout << "Number of Scintillation photons produced in this event : " << fScintillationCounter << G4endl;
        //G4cout << "Number of Cerenkov photons produced in this event : " << fCerenkovCounter << G4endl;
}

void StackingAction::PrepareNewEvent(){
        //G4cout << "preparing new event" << G4endl;

}
