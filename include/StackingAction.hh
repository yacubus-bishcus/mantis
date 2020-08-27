

#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"

class EventAction;

class StackingAction : public G4UserStackingAction
{
public:
StackingAction(EventAction*);
virtual ~StackingAction();

public:
//virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
void NewStage();
void PrepareNewEvent();

private:
EventAction *fEventAction;
};

#endif
