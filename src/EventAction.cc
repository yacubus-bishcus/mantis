#include "EventAction.hh"


EventAction::EventAction(SteppingAction* stepA)
        : drawNumPhotonsFlag(0), eventM(NULL)
{
        eventM = new EventMessenger(this);
        stepA_local = stepA;
}

EventAction::~EventAction()
{
        delete eventM;
}


void EventAction::BeginOfEventAction(const G4Event* anEvent)
{

        anEvent->GetEventID();
        ResetEverything(); //reset all the class variables

}


void EventAction::EndOfEventAction(const G4Event* anEvent)
{

        G4AnalysisManager* manager = G4AnalysisManager::Instance();
        if(drawNumPhotonsFlag)
        {
                if(stepA_local->Ev.size() > 0)
                {
                        eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
                        E_beam = info->GetBeamEnergy();
                        manager->FillNtupleDColumn(4,0,E_beam);
                        manager->FillNtupleDColumn(4,1,stepA_local->Ev.size());
                        manager->AddNtupleRow(4);
                }
        }


        std::cout << "\r\tEvent and tracks:\t " << anEvent->GetEventID() << "\t" << stepA_local->Ev.size() <<std::flush;
}


void EventAction::ResetEverything(){

        stepA_local->Ev.clear();
}
