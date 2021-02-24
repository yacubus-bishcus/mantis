#include "RootDataMessenger.hh"

RootDataMessenger::RootDataMessenger(RootDataManager *rdm)
: theManager(rdm)
{
  root_directory = new G4UIdirectory("/mantis/root/");
  root_directory->SetGuidance("ROOT COMMANDS");
  root_filename_cmd = new G4UIcmdWithAString("/mantis/root/setFileName",this);
  root_filename_cmd->SetGuidance("Sets ROOT Output Filename");
  root_filename_cmd->SetParameterName("choice",false);
  root_book_cmd = new G4UIcmdWithoutParameter("/mantis/root/book",this);
  root_book_cmd->SetGuidance("Sets ROOT Objects");
  root_book_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  root_write_cmd = new G4UIcmdWithoutParameter("/mantis/root/write",this);
  root_write_cmd->SetGuidance("Writes ROOT Objects to file");
  root_write_cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

}

RootDataMessenger::~RootDataMessenger()
{
  delete root_filename_cmd;
  delete root_book_cmd;
  delete root_write_cmd;
}

void RootDataMessenger::SetNewValue(G4UIcommand *cmd, G4String newValue)
{
  if(cmd == root_filename_cmd)
    theManager->SetFileName(newValue);

  if(cmd == root_book_cmd)
    theManager->Book();

  if(cmd == root_write_cmd)
    theManager->finish();
}
