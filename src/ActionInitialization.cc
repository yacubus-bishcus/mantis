#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"


ActionInitialization::ActionInitialization(const DetectorConstruction* det, G4bool brem_check, G4bool output_in)
        : G4VUserActionInitialization(), fDetector(det)
{
   bremTest = brem_check;
   output = output_in;
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::Build() const
{
        HistoManager* histo = new HistoManager();
        SetUserAction(new PrimaryGeneratorAction(bremTest));
        RunAction* run = new RunAction(histo, bremTest, output);
        SetUserAction(run);
        EventAction* event = new EventAction();
        SetUserAction(new SteppingAction(fDetector, run, event, bremTest));
        SetUserAction(new StackingAction(fDetector, run));
}
