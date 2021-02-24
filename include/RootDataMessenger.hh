#ifndef RootDataMessenger_h
#define RootDataMessenger_h 1

#include "G4UImessenger.hh"

class RootDataManager;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class RootDataMessenger : public G4UImessenger
{

public:
  RootDataMessenger(RootDataManager*);
  ~RootDataMessenger();

  void SetNewValue(G4UIcommand *, G4String);

private:
  RootDataManager *theManager;

  G4UIdirectory *rootdirectory;

  G4UIcmdWithAString *rootfilenamecmd;
  G4UIcmdWithoutParameter *root_book_cmd;
  G4UIcmdWithoutParameter *root_write_cmd;
};

#endif
