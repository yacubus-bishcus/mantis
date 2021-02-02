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
// jbickus@mit.edu
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"


ActionInitialization::ActionInitialization(const DetectorConstruction* det, G4bool brem_check, G4bool output_in, G4bool checkEvents_in)
        : G4VUserActionInitialization(), fDetector(det)
{
   bremTest = brem_check;
   output = output_in;
   checkEvents = checkEvents_in;
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::Build() const
{
    HistoManager* histo = new HistoManager();
    SetUserAction(new PrimaryGeneratorAction(bremTest));
    RunAction* run = new RunAction(histo, bremTest, output, checkEvents);
    SetUserAction(run);
    EventAction* event = new EventAction();
    SetUserAction(event);
    SetUserAction(new SteppingAction(fDetector, run, event, bremTest));
    SetUserAction(new StackingAction(fDetector, run));
}
