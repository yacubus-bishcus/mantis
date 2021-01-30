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
// jbickus@mit.edu
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

#include "PrimaryGenActionMessenger.hh"


PrimaryGenActionMessenger::PrimaryGenActionMessenger(PrimaryGeneratorAction* genAction)
:genA(genAction)
{
  myDir = new G4UIdirectory("/input/");
  myDir->SetGuidance("Input Commands");
  Cmd = new G4UIcmdWithADouble("/input/energy",this);
  Cmd->SetGuidance("Choose Desired Energy Input");
  Cmd->SetParameterName("energy",false);
  Cmd->SetDefaultValue(-1);

}

PrimaryGenActionMessenger::~PrimaryGenActionMessenger()
{
  delete Cmd;
}


void PrimaryGenActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue

  if(command == Cmd)
  {
    G4double theCommand = Cmd->GetNewDoubleValue(newValue);
    std::cout << "PrimaryGenActionMessenger::Input Energy set to: " << std::setprecision(10) << theCommand << " MeV" << std::endl;
    genA->SetEnergyValue(theCommand);

  }
  else
  {
    G4cerr << "ERROR PrimaryGenActionMessenger :: SetStepInputValue command != Cmd" << G4endl;
  }
}
