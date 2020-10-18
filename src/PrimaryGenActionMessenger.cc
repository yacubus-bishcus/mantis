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
  Cmd2 = new G4UicmdWithAString("/input/bremTest",this);
  Cmd2->SetGuidance("Testing Bremstrahlung");
  Cmd2->SetParameterName("brem",false);
  Cmd2->SetDefaultValue("false");
  Cmd2->SetCandidates("True true False false");

}

PrimaryGenActionMessenger::~PrimaryGenActionMessenger()
{
  delete Cmd;
  delete Cmd2;
}


void PrimaryGenActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue

  if(command == Cmd)
  {
    G4double theCommand = Cmd->GetNewDoubleValue(newValue);
    genA->SetEnergyValue(theCommand);

  }
  else if(command == Cmd2)
  {
    if(newValue == "True" || newValue == "true")
      genA->SetBremTest(true);
    else
      genA->SetBremTest(false);
    
  }
  else
  {
    G4cerr << "ERROR PrimaryGenActionMessenger :: SetStepInputValue command != Cmd" << G4endl;
  }
}
