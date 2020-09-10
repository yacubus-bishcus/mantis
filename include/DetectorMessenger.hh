#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"

class DetectorConstruction;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIdirectory;

class DetectorMessenger: public G4UImessenger
{
public:
  DetectorMessenger(DetectorConstruction*);
  ~DetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String); // must always be a string input
private:
  DetectorConstruction* DetectorA;
  G4UIcmdWithADouble* Cmd;
  G4UIcmdWithADouble* CmdX;
  G4UIcmdWithADouble* CmdY;
  G4UIcmdWithADouble* CmdZ;
  G4UIcmdWithADouble* CmdtX;
  G4UIcmdWithADouble* CmdtY;
  G4UIcmdWithADouble* CmdtZ;
  G4UIcmdWithADouble* CmdtXpos;
  G4UIcmdWithADouble* CmdtYpos;
  G4UIcmdWithADouble* CmdtZpos;
  G4UIcmdWithADouble* Cmdtrad;
  G4UIcmdWithAString* Cmdtsel;
  G4UIdirectory *myDir;
  G4UIdirectory * myDir2;
};

#endif
