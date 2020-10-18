#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"


ActionInitialization::ActionInitialization(const DetectorConstruction* det, G4bool bremTest)
        : G4VUserActionInitialization(), fDetector(det)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::Build() const
{
        HistoManager* histo = new HistoManager();
        SetUserAction(new PrimaryGeneratorAction(bremTest));
        RunAction* run = new RunAction(histo);
        SetUserAction(run);
        SetUserAction(new SteppingAction(fDetector, run));
        SetUserAction(new EventAction());
        SetUserAction(new StackingAction(fDetector, run));
}
