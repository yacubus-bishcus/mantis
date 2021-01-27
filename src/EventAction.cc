#include "EventAction.hh"

EventAction::EventAction()
{
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
  tEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  if(anEvent->GetEventID() != 0 && anEvent->GetEventID() % 10000 == 0)
  {
    std::cout << "\r\tEvent & % Complete " << "\t" << anEvent->GetEventID() 
              << "\t" << 100.*(anEvent->GetEventID()/(double)tEvents) << " %" << std::flush;
  }
}
