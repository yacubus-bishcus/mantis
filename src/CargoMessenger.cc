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

#include "CargoMessenger.hh"

CargoMessenger::CargoMessenger(Cargo* Cargo)
: cargo(Cargo)
{
  myDir = new G4UIdirectory("/IntObj/");
  myDir->SetGuidance("Interrogation Object Options");
  Cmd = new G4UIcmdWithADouble("/IntObj/Radius",this);
  Cmd1 = new G4UIcmdWithADouble("/IntObj/Abundance",this);
  Cmd2 = new G4UIcmdWithAString("/IntObj/Material",this);

  Cmd->SetGuidance("Choose Desired radius Size of Interrogation Target");
  Cmd1->SetGuidance("Choose Desired fission isotope abundance(enrichment) of Interrogation Target");
  Cmd2->SetGuidance("Choose Desired target");

  Cmd1->SetParameterName("abundance",false);
  Cmd1->SetRange("abundance > 0 && abundance < 100");
  Cmd2->SetCandidates("Uranium NaturalU Plutonium NaturalPu Lead Steel Plastic");

  myDir2 = new G4UIdirectory("/Container/");
  myDir2->SetGuidance("Container Options");
  CmdRemoveObjects = new G4UIcmdWithAString("Container/Remove",this);
  CmdRemoveObjects->SetGuidance("Choose to remove objects for simulation");
  CmdRemoveObjects->SetCandidates("True true False false");
}

CargoMessenger::~CargoMessenger()
{
  delete Cmd;
  delete Cmd1;
  delete Cmd2;
  delete CmdRemoveObjects;
}

void CargoMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == Cmd)
  {
    G4double theCommandtX = Cmd->GetNewDoubleValue(newValue);
    cargo->SetIntObj_radius(theCommandtX);
  }
  else if(command == Cmd1)
  {
    G4double theCommandtrad = Cmd1->GetNewDoubleValue(newValue);
    cargo->SetIntObjAbundance(theCommandtrad);
  }
  else if(command == Cmd2)
  {
    G4String theCommandtsel = newValue;
    cargo->SetIntObj(theCommandtsel);
  }
  else if(command == CmdRemoveObjects)
  {
    G4String theCommand = newValue;
    if(theCommand == "True" || theCommand == "true")
      cargo->SetRemoveContainer(true);
  }
}
