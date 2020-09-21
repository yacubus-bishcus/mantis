#ifndef RunAction_h
#define RunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include <vector>
#include "G4Run.hh"
#include "HistoManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Timer.hh"

class G4Timer;

class RunAction : public G4UserRunAction
{
  public:
    RunAction(HistoManager* histoAnalysis);
    virtual ~RunAction();

  public:

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddCerenkovEnergy(G4double en) {fCerenkovEnergy += en;}
    void AddScintillationEnergy(G4double en) {fScintEnergy += en;}
    void AddCerenkov(void) {fCerenkovCount++;} // changed from +=
    void AddScintillation(void) {fScintCount++;}
    void AddRayleigh(void) {fRayleighCount++;}
    void AddOpAbsorption(void) {fOpAbsorption++;}
    void AddTotalSurface(void) {fTotalSurface += 1;}
    void AddNRF(void){fNRF++;}
    void AddStatusKilled(void){fStatusKilled++;}

  private:
    G4Timer* fTimer;
    HistoManager* fHistoManager;
    G4double fCerenkovEnergy;
    G4double fScintEnergy;
    G4double fCerenkovCount;
    G4int fScintCount;
    G4int fRayleighCount;
    G4int fOpAbsorption;
    G4int fTotalSurface;
    G4int fNRF;
    G4int fStatusKilled;
};


#endif
