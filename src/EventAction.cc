//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Author:
// Jacob E Bickus, 2021
// MIT, NSE
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

#include "EventAction.hh"

EventAction::EventAction()
{
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  //std::cout << "EventAction::BeginOfEventAction -> Beginning" << std::endl;
  c_secondaries = 0;
  energyv.clear();
  timev.clear();
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
  //std::cout << "EventAction::EndOfEventAction -> Beginning" << std::endl;
  if(c_secondaries > 0)
  {
    // Grab Max Energy
    //G4double maxE = *std::max_element(energyv.begin(),energyv.end());
    G4double maxE = 1.0;
    // Find Max Energy's Weight
    eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
    weight = info->GetWeight();
    // Find the Average Time 
    if(timev.size() > 0)
    {
      c_time = calcAvg();
    }
    else
      c_time = 0;
    // Fill the TTree 
    G4AnalysisManager* manager = G4AnalysisManager::Instance();
    manager->FillNtupleDColumn(3,0,maxE);
    manager->FillNtupleDColumn(3,1, weight);
    manager->FillNtupleIColumn(3,2,anEvent->GetEventID());
    manager->FillNtupleIColumn(3,3,c_secondaries);
    manager->FillNtupleDColumn(3,4,c_time);
    manager->AddNtupleRow(3);
  }
  
  tEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  if(anEvent->GetEventID() != 0 && anEvent->GetEventID() % 10 == 0)
  {
    std::cout << "\r\tEvent " << "\t" << anEvent->GetEventID() << std::flush;
  }
}
