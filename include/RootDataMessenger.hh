#ifndef RootDataMessenger_hh
#define RootDataMessenger_hh 1

#include "G4UImessenger.hh"
#include "RootDataManager.hh"

class RootDataManager;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

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
