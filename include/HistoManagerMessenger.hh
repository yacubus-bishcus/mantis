#ifndef HistoManagerMessenger_h
#define HistoManagerMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithADouble.hh"
#include "HistoManager.hh"
#include "G4UIdirectory.hh"

class HistoManager;
class G4UIcmdWithADouble;
class G4UIdirectory;

class HistoManagerMessenger: public G4UImessenger
{
public:
  HistoManagerMessenger(HistoManager*);
  ~HistoManagerMessenger();

  void SetNewValue(G4UIcommand*, G4String);
private:
    HistoManager* histoManager;
    G4UIcmdWithADouble* Cmd;
    G4UIdirectory *myDir;
};

#endif
