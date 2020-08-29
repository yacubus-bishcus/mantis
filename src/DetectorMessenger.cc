#include "DetectorMessenger.hh"


DetectorMessenger::DetectorMessenger(DetectorConstruction* DetectorAction)
:DetectorA(DetectorAction)
{
  myDir = new G4UIdirectory("/mydet/");
  myDir->SetGuidance("Output Commands");
  Cmd = new G4UIcmdWithADouble("/mydet/PCrad",this);
  Cmd->SetGuidance("Choose Desired PhotoCathode Radius");
  Cmd->SetParameterName("radius",false);
  Cmd->SetDefaultValue(-1);
  Cmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

}

DetectorMessenger::~DetectorMessenger()
{
  delete Cmd;
}


void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
  if(command == Cmd)
  {
    G4double theCommand = Cmd->GetNewDoubleValue(newValue);
    if(theCommand == -1)
    {
      std::cout << "Using Default PC Radius."<<std::endl;
    }
    else
    {
      DetectorA->SetPC_radius(theCommand);
    }

  }
  else
  {
    G4cerr << "ERROR DetectorMessenger :: SetDetectorInputValue command != Cmd" << G4endl;
  }
}
