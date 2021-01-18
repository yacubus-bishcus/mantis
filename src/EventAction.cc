#include "EventAction.hh"

EventAction::EventAction(): drawCherenkovDataFlag(0), eventM(NULL)
{
  eventM = new EventMessenger(this);
}

EventAction::~EventAction()
{
  delete eventM;
}


void EventAction::BeginOfEventAction(const G4Event*)
{
  cherenkov_counter = 0;
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
        tEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
        if(drawCherenkovDataFlag)
        {
          G4AnalysisManager* manager = G4AnalysisManager::Instance();
          eventInformation *anInfo = new eventInformation(anEvent);
          G4double beam_energy = anInfo->GetBeamEnergy();
          manager->FillNtupleDColumn(1,0, beam_energy);
          manager->FillNtupleIColumn(1,1, cherenkov_counter);
          manager->AddNtupleRow(1);
        }
        
        if(anEvent->GetEventID() != 0 && anEvent->GetEventID() % 10000 == 0)
        {
          std::cout << "\r\tEvent & % Complete " << "\t" << anEvent->GetEventID() << "\t" << 100.*(anEvent->GetEventID()/(double)tEvents) << " %" << std::flush;
        }
}
