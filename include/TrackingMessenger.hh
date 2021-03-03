#ifndef TrackingMessenger_h
#define TrackingMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithAString.hh"
#include "TrackingAction.hh"
#include "G4UIdirectory.hh"

class TrackingAction;
class G4UIcmdWithAString;
class G4UIdirectory;


class TrackingMessenger: public G4UImessenger
{
public:
  TrackingMessenger(TrackingAction*);
  ~TrackingMessenger();

  void SetNewValue(G4UIcommand*, G4String);
private:
  TrackingAction* trackA;
  G4UIcmdWithAString* Cmd;
  G4UIdirectory *myDir;
};

#endif
