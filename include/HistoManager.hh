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

void finish();     // close root file
void Book(G4bool);
void SetChosenEnergy(G4double val)
{
  chosen_energy = val;
}

G4double GetEmax()const
{
  return xmax;
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
