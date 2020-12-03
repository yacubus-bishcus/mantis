#include "StepMessenger.hh"


StepMessenger::StepMessenger(SteppingAction* stepAction)
        : stepA(stepAction)
{
        myDir = new G4UIdirectory("/output/");
        myDir->SetGuidance("Output Commands");
        Cmd = new G4UIcmdWithAString("/output/myoutput",this);
        Cmd->SetGuidance("Choose Desired Outputs");
        Cmd->SetGuidance("Choice: ChopperData, WaterIncidentData DetIncidentData, DetData, IntObjData, none (default)");
        Cmd->SetParameterName("choice",false);
        Cmd->SetDefaultValue("none");
        Cmd->SetCandidates("ChopperData WaterIncidentData DetIncidentData DetData IntObjData none");
}

StepMessenger::~StepMessenger()
{
        delete Cmd;
}


void StepMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
        if(command == Cmd)
        {
                G4String theCommand = newValue;
                if(theCommand == "ChopperData")
                {
                        stepA->SetChopperDataFlag(1);
                        std::cout << "User Selected Chopper Data." << std::endl;
                }
                else if(theCommand == "WaterIncidentData")
                {
                        stepA->SetIncWatDataFlag(1);
                        std::cout << "User Selected Incident Water Data." <<std::endl;
                }
                else if(theCommand == "DetIncidentData")
                {
                        stepA->SetIncidentDataFlag(1);
                        std::cout << "User Selected Incident Photocathode Data." <<std::endl;
                }
                else if(theCommand == "DetData")
                {
                        stepA->SetDetDataFlag(1);
                        std::cout << "User Selected Detected Data."<<std::endl;
                }
                else if(theCommand == "IntObjData")
                {
                        stepA->SetIntObjDataFlag(1);
                        std::cout << "User Selected Interrogation Object Incident Data." << std::endl;
                }
                else if(theCommand == "none")
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
