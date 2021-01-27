#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "G4OpBoundaryProcess.hh"
#include "RunAction.hh"
#include "StackingAction.hh"
#include "HistoManager.hh"
#include "StepMessenger.hh"
#include "DetectorConstruction.hh"
#include "eventInformation.hh"

#include "G4SteppingManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"

class StepMessenger;

class SteppingAction : public G4UserSteppingAction
{
public:
SteppingAction(const DetectorConstruction*, RunAction*, G4bool);
virtual ~SteppingAction();

// method from the base class
virtual void UserSteppingAction(const G4Step*);

public: // for now set to public until I can figure out how to work with protected

void SetChopperDataFlag(G4int val)
{
  drawChopperDataFlag = val;
}
void SetNRFDataFlag(G4int val)
{
  drawNRFDataFlag = val;
}
void SetIntObjDataFlag(G4int val)
{
  drawIntObjDataFlag = val;
}
void SetWaterIncDataFlag(G4int val)
{
  drawWaterIncDataFlag = val;
}
void SetCherenkovDataFlag(G4int val)
{
  drawCherenkovDataFlag = val;
}
void SetDetDataFlag(G4int val)
{
  drawDetDataFlag = val;
}

private:
G4bool bremTest;
G4double weight;
const DetectorConstruction* local_det;
RunAction* run;
G4OpBoundaryProcessStatus fExpectedNextStatus;
G4String procCount;
G4int drawChopperDataFlag, drawNRFDataFlag, drawIntObjDataFlag, drawWaterIncDataFlag, drawCherenkovDataFlag, drawDetDataFlag;
StepMessenger* stepM;
};

#endif
