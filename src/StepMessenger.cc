#include "StepMessenger.hh"


StepMessenger::StepMessenger(SteppingAction* stepAction)
:stepA(stepAction)
{
  Cmd = new G4UIcmdWithAString("/stepping/output",this);
  Cmd->SetGuidance("Choose Desired Outputs");
  Cmd->SetGuidance("Choice: WaterData, IncidentData, DetData, none (default)");
  Cmd->SetParamterName("choice",true);
  Cmd->SetDefaultValue("none");
  Cmd->SetCandidates("WaterData IncidentData, DetData, none");
  Cmd->AvailableForStates(G4State_Idle);

}

StepMessenger::~StepMessenger()
{
  delete Cmd;
}


void StepMessenger::SetStepInputValue(G4UIcommand* command, G4String newValue)
{
  if(command == Cmd)
  {
    if(command== "WaterData")
    {
      stepA->SetWaterDataFlag(1);
    }
    else if(command == "IncidentData")
    {
      stepA->SetIncidentDataFlag(1);
    }
    else if(command == "DetData")
    {
      stepA->SetDetDataFlag(1);
    }
    else if(command == "none")
    {
      G4cout << "No output requested." << G4endl;
    }
    else
    {
      G4cerr << "ERROR: StepMessenger::SetStepInputValue Command not found." << G4endl;
    }

  }
  else
  {
    G4cerr << "ERROR StepMessenger :: SetStepInputValue command != Cmd" << G4endl;
  }
}
