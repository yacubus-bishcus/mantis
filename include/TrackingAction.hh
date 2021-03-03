#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "TrackingMessenger.hh"
#include "globals.hh"

class TrackingMessenger;

class TrackingAction : public G4UserTrackingAction {

  public:

    TrackingAction();
    virtual ~TrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

    void SetPhotonsOnly(){photonsOnly = true;}
    void SetOpticalPhotonsOnly(){opticalOnly = true;}
    void SetDetectedOnly(){detectedOnly = true;}
    void SetChopperOnly(){chopperOnly = true;}

  private:
    G4bool photonsOnly, opticalOnly, detectedOnly, chopperOnly;
    TrackingMessenger* trackM;

};

#endif
