#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include "g4root.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#if defined (G4ANALYSIS_USE_ROOT)
#include "TFile.h"
#include "TROOT.h"
#include "TH1D.h"
#endif


class HistoManager
{
public:
HistoManager();
~HistoManager();

void finish(G4bool);     // close root file
void Book(G4bool);
void SetChosenEnergy(G4double val)
{
  chosen_energy = val;
}

private:
G4bool fFactoryOn;
G4double chosen_energy;
G4double xmax;
#if defined (G4ANALYSIS_USE_ROOT)
TH1D *hBrems;
#endif

};

#endif
