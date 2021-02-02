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
  c_secondaries = 0;
  energyv.clear();
  weightv.clear();
  timev.clear();
  mergeCherenkov = false;
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
  if(mergeCherenkov)
  {
    // Grab Max Energy
    G4double maxE = *std::max_element(energyv.begin(),energyv.end());
    // Find Max Energy's Weight
    auto it = std::find(energyv.begin(), energyv.end(),maxE);
    G4int maxEIndex = it - energyv.begin();
    G4double c_weight = weightv[maxEIndex];
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
    manager->FillNtupleDColumn(3,1,c_weight);
    manager->FillNtupleIColumn(3,2,anEvent->GetEventID());
    manager->FillNtupleIColumn(3,3,c_secondaries);
    manager->FillNtupleDColumn(3,4,c_time);
    manager->AddNtupleRow(3);
  }
  
  tEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  if(anEvent->GetEventID() != 0 && anEvent->GetEventID() % 10000 == 0)
  {
    std::cout << "\r\tEvent & % Complete " << "\t" << anEvent->GetEventID() 
              << "\t" << 100.*(anEvent->GetEventID()/(double)tEvents) << " %" << std::flush;
  }
}
