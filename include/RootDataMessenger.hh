#ifndef RootDataMessenger_h
#define RootDataMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"
#include "RootDataManager.hh"

class RootDataManager;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIdirectory;

class RootDataMessenger : public G4UImessenger
{

public:
  RootDataMessenger(RootDataManager*);
  ~RootDataMessenger();

  void SetNewValue(G4UIcommand *, G4String);

private:
  RootDataManager *theManager;

  G4UIdirectory *root_directory;

  G4UIcmdWithAString *root_filename_cmd;
  G4UIcmdWithoutParameter *root_book_cmd;
  G4UIcmdWithoutParameter *root_write_cmd;
};

#endif
