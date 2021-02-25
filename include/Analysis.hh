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

#ifndef ANALYSIS_MANAGER_H
#define ANALYSIS_MANAGER_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)



#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4String.hh"

#include "TFile.h"
#include "TTree.h"

#include <vector>

class Analysis
{
public:
~Analysis();
static Analysis *GetAnalysis();

void Update();
void Clear();
void Save(const G4String& fname);
void finish();

void FillChopIn(G4double, G4double, G4int);
void FillChopOut(G4double, G4double, G4int, G4int);
void FillNRF(G4double, G4double, G4int, G4String, G4double);
void FillIntObjIn(G4double, G4double, G4String);
void FillIntObjOut(G4double, G4double, G4String);
void FillWater(G4double, G4double, G4String);
void FillCherenkov(G4double, G4double, G4int, G4int, G4double);
void FillDet(G4double, G4double, G4int, G4String, G4double);
void FillInDet(G4double, G4double, G4int, G4String);


private:
  Analysis();
  DISALLOW_COPY_AND_ASSIGN(Analysis);
  G4bool ROOTObjectsExist;
  G4int MPI_RANK;
  G4String ROOTFileName;
  std::vector<G4String> slaveFileNames;

  TTree *ChopInTree,*ChopOutTree, *NRFTree, *IntObjInTree, *IntObjOutTree, *WaterInTree, *CherenkovTree, *DetTree, *DetInTree;
  // Chopper In
  G4double ChopInEnergy, ChopInWeight;
  G4int ChopInEventID;
  // Chopper Out
  G4double ChopOutEnergy, ChopOutWeight;
  G4int ChopOutEventID, ChopOutisNRF;
  // NRF
  G4double NRFEnergy, NRFWeight;
  G4int NRFEventID;
  G4String Material;
  G4double ZPos;
  // IntObjIn
  G4double IntObjInEnergy, IntObjInWeight;
  G4String IntObjInCreatorProcess;
  // IntObjOut
  G4double IntObjOutEnergy, IntObjOutWeight;
  G4String IntObjOutCreatorProcess;
  // Water
  G4double WaterEnergy, WaterWeight;
  G4String WaterCreatorProcess;
  // Cherenkov
  G4double CherenkovEnergy, CherenkovWeight;
  G4int CherenkovEventID, CherenkovNumSecondaries;
  G4double CherenkovTime;
  // Det
  G4double DetEnergy, DetWeight;
  G4int DetEventID;
  G4String DetCreatorProcess;
  G4double DetTime;
  // InDet
  G4double InDetEnergy, InDetWeight;
  G4int InDetEventID;
  G4String InDetProcess;

};

#endif
