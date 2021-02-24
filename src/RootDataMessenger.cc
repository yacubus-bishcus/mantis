//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Author:
// Jacob E Bickus, 2021
// MIT, NSE
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

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
  delete root_write_cmd;
  delete root_book_cmd;
  delete root_filename_cmd;
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
