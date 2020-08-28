#ifndef StepMessenger_h
#define StepMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ApplicationState.hh"

class SteppingAction;
class G4UIcmdWithAString;


class StepMessenger: public G4UImessenger
{
public:
  StepMessenger(SteppingAction*);
  ~StepMessenger();

  void SetStepInputValue(G4UIcommand*, G4String);
private:
  SteppingAction* stepA;
  G4UIcmdWithAString* Cmd;
};

#endif
