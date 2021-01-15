#include "EventAction.hh"

EventAction::EventAction()
{}

EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event*)
{
  cherenkov_counter = 0;
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
        tEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
        eventInformation *anInfo = new eventInformation(anEvent);
        G4double beam_energy = anInfo->GetBeamEnergy();
        manager->FillH2(beam_energy, cherenkov_counter);
        
        if(anEvent->GetEventID() != 0 && anEvent->GetEventID() % 1000000 == 0)
        {
          std::cout << "\r\tEvent & % Complete " << "\t" << anEvent->GetEventID() << "\t" << 100.*(anEvent->GetEventID()/(double)tEvents) << " %" << std::flush;
        }
}
