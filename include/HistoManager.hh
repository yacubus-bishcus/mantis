#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include "g4root.hh"


class HistoManager
{
public:
HistoManager();
~HistoManager();

void finish();     // close root file
void Book();

private:
G4bool fFactoryOn;
G4double fE_beam, fEnergy, fx, fy, fz, ftheta, ftime;
G4double fincEnergy;
G4String fProcess;
G4double fE_Beam, fNumPhotons;

};


#endif
