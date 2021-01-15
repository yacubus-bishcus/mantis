#include "EventAction.hh"

EventAction::EventAction(): drawCherenkovDataFlag(0)
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
        if(drawCherenkovDataFlag)
        {
          G4AnalysisManager* manager = G4AnalysisManager::Instance();
          eventInformation *anInfo = new eventInformation(anEvent);
          G4double beam_energy = anInfo->GetBeamEnergy();
          manager->FillH2(0, beam_energy/(MeV), cherenkov_counter);
        }
        
        if(anEvent->GetEventID() != 0 && anEvent->GetEventID() % 1000000 == 0)
        {
          std::cout << "\r\tEvent & % Complete " << "\t" << anEvent->GetEventID() << "\t" << 100.*(anEvent->GetEventID()/(double)tEvents) << " %" << std::flush;
        }
}
