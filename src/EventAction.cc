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

  
  tEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  if(anEvent->GetEventID() != 0 && anEvent->GetEventID() % 10 == 0)
  {
    std::cout << "\r\tEvent " << "\t" << anEvent->GetEventID() << std::flush;
  }
}
