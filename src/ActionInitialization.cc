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
#include "RootDataManager.hh"
#include "G4Types.hh"

extern G4bool debug;

ActionInitialization::ActionInitialization(const DetectorConstruction* det, G4bool build)
        : G4VUserActionInitialization(), fDetector(det), fbuild(build)
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::Build() const
{
    if(debug)
        std::cout << "ActionInitialization::Build() -> Begin!" << std::endl;

    RootDataManager *analysis = new RootDataManager(fbuild);
    PrimaryGeneratorAction* pga = new PrimaryGeneratorAction();
    SetUserAction(pga);
    RunAction* run = new RunAction(analysis,pga, fbuild);
    SetUserAction(run);
    EventAction* event = new EventAction();
    SetUserAction(event);
    SetUserAction(new SteppingAction(fDetector, run, event));
    SetUserAction(new StackingAction(fDetector, run));

    if(debug)
        std::cout << "ActionInitialization::Build() -> End!" << std::endl;
}
