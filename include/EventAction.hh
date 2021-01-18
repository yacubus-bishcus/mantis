#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <iostream>
#include <fstream>
#include "G4Types.hh"
#include "G4EventManager.hh"
#include "EventMessenger.hh"
#include "eventInformation.hh"
#include "HistoManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"
#include <time.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
class G4Event;
class EventMessenger;

class EventAction : public G4UserEventAction
{
public:
EventAction();
~EventAction();

public:
void BeginOfEventAction(const G4Event*);
void EndOfEventAction(const G4Event*);
void AddCherenkov()
{
  cherenkov_counter++;
}
void SetCherenkovDataFlag(G4int val)
{
        drawCherenkovDataFlag = val;
};

private:
G4int tEvents;
G4int cherenkov_counter;
G4int drawCherenkovDataFlag;
EventMessenger* eventM;
};

#endif
