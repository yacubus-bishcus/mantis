#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <iostream>
#include <fstream>
#include <vector>
#include "G4Types.hh"
#include "G4ParticleGun.hh"
#include "SteppingAction.hh"
#include "HistoManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
class G4Event;
class DetectorConstruction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(G4ParticleGun*,SteppingAction*, const DetectorConstruction*);
   ~EventAction();

  public:
    void BeginOfEventAction(const G4Event* anEvent);
    void EndOfEventAction(const G4Event* anEvent);

    void SetPMTThreshold(G4int t){fPMTThreshold=t;}

    //void IncBoundaryAbsorption(){fBoundaryAbsorptionCount++;}
    void IncHitCount(G4int i=1){fHitCount+=i;}
    void SetReconPos(const G4ThreeVector& p){fReconPos=p;}
    //void SetConvPos(const G4ThreeVector& p){fConvPos=p;fConvPosSet=true;}

    G4int GetHitCount()const {return fHitCount;}

    //G4ThreeVector GetEWeightPos(){return fEWeightPos;}
    G4ThreeVector GetReconPos(){return fReconPos;}

    void IncPMTSAboveThreshold(){fPMTsAboveThreshold++;}
    G4int GetPMTSAboveThreshold(){return fPMTsAboveThreshold;}

private:
    std::ofstream data_file;
    void ResetEverything();
    //void Weighting(const G4Event* anEvent);

    G4ParticleGun* particle_gun_local;
    SteppingAction* stepA_local;
    const DetectorConstruction* fDetector;

    // for PMT analysis
    G4int fPMTCollID;

    G4int fPMTThreshold;

    G4bool fForcedrawphotons;
    G4bool fForcenophotons;

    G4int fHitCount;

    G4ThreeVector fReconPos; //relies on hitCount>0

    G4int fPMTsAboveThreshold;

    // for Water analysis

    G4double E,E_beam;
    G4double x;
    G4double y;
    G4double z;
    G4double theta;
    G4float Time;
    G4int ID;
    G4int TrackID;
    G4int EventID;
    G4int ProcID;
    std::string ParticleName,CreatorProcessName;
    bool IsSurfaceHitTrack;
    bool SaveTrackInfo = true;
    bool briefoutput = false;

};

#endif
