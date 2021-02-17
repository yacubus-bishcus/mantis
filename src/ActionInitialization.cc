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
#include "G4Types.hh"


ActionInitialization::ActionInitialization(const DetectorConstruction* det, G4bool brem_check)
        : G4VUserActionInitialization(), fDetector(det), bremTest(brem_check)
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::Build() const
{
        //std::cout << "ActionInitialization::Build() -> Begin!" << std::endl;
        HistoManager* histo = new HistoManager();
        SetUserAction(new PrimaryGeneratorAction());
        RunAction* run = new RunAction(histo, bremTest);
        SetUserAction(run);
        EventAction* event = new EventAction(weightHisto);
        SetUserAction(event);
        SetUserAction(new SteppingAction(fDetector, run, event, bremTest));
        SetUserAction(new StackingAction(fDetector, run));
        //std::cout << "ActionInitialization::Build() -> End!" << std::endl;
}
