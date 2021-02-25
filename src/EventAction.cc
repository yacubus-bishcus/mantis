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
#include "Analysis.hh"

extern G4bool debug;

EventAction::EventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event*)
{
    if(debug)
        std::cout << "EventAction::BeginOfEventAction -> Beginning" << std::endl;

        c_secondaries = 0;
        energyv.clear();
        timev.clear();

    if(debug)
        std::cout << "EventAction::BeginOfEventAction -> Ending" << std::endl;
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
    if(debug)
        std::cout << "EventAction::EndOfEventAction -> Beginning" << std::endl;

        if(c_secondaries > 0)
        {
                // Grab Max Energy
                G4double maxE = *std::max_element(energyv.begin(),energyv.end());
                // Find Max Energy's Weight
                eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
                G4double weight = info->GetWeight();
                // Find the Average Time
                G4double c_time;
                if(timev.size() > 0)
                {
                        c_time = calcAvg();
                }
                else
                        c_time = 0;
                // Fill the TTree
                Analysis *manager = Analysis::GetAnalysis();
                manager->FillCherenkov(maxE, weight, anEvent->GetEventID(), c_secondaries, c_time);
        }

    if(debug)
        std::cout << "EventAction::EndOfEventAction() --> Ending!" << std::endl;
}
