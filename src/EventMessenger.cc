#include "EventMessenger.hh"


EventMessenger::EventMessenger(EventAction* eventAction)
        : eventA(eventAction)
{
        myDir = new G4UIdirectory("/user_event/");
        myDir->SetGuidance("Event Output Commands");
        Cmd = new G4UIcmdWithAString("/user_event/output",this);
        Cmd->SetGuidance("Choose Desired Outputs");
        Cmd->SetGuidance("Choice: CherenkovData, none (default)");
        Cmd->SetParameterName("choice",false);
        Cmd->SetDefaultValue("none");
        Cmd->SetCandidates("CherenkovData none");
}

EventMessenger::~EventMessenger()
{
        delete Cmd;
}


void EventMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
        if(command == Cmd)
        {
                G4String theCommand = newValue;
                if(theCommand == "CherenkovData")
                {
                        eventA->SetCherenkovDataFlag(1);
                        std::cout << "User Selected Cherenkov Data." << std::endl;
                }
               
                else
                {
                        G4cerr << "ERROR: EventMessenger::SetNewValue Command not found." << G4endl;
                }

        }
        else
        {
                G4cerr << "ERROR EventMessenger :: SetStepInputValue command != Cmd" << G4endl;
        }
}
