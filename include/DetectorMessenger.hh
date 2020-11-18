#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
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
  G4UIcmdWithADouble* Cmdtr;
  G4UIcmdWithADouble* CmdtXpos;
  G4UIcmdWithADouble* CmdtYpos;
  G4UIcmdWithADouble* CmdtZpos;
  G4UIcmdWithADouble* Cmdtrad;
  G4UIcmdWithAString* Cmdtsel;
  G4UIcmdWithAString* Cmdpcmat;
  G4UIcmdWithAnInteger* CmdnPMT;
  G4UIcmdWithADouble* CmdChopthick;
  G4UIcmdWithADouble* CmdChopZ;
  G4UIcmdWithAString* CmdChopperOn;
  G4UIcmdWithADouble* CmdAngle;
  G4UIcmdWithAString* CmdAttenOn;
  G4bool check_atten_on = false;
  G4UIcmdWithADouble* CmdAttenThick;
  G4UIcmdWithAString* CmdAttenMat;
  G4UIcmdWithAString* CmdAttenOn2;
  G4bool check_atten2_on = false;
  G4UIcmdWithADouble* CmdAttenThick2;
  G4UIcmdWithAString* CmdAttenMat2;
  G4UIcmdWithAString* CmdVis;
  G4UIdirectory *myDir;
  G4UIdirectory *myDir2;
  G4UIdirectory *myDir3;
  G4UIdirectory *myDir4;
};

#endif
