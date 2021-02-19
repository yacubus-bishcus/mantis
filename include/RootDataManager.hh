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

#ifndef RootDataManager_hh
#define RootDataManager_hh 1

#include <vector>
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "TFile.h"
#include "TGraph.h"

class RootDataManager
{
public:
  RootDataManager();
  ~RootDataManager();

  static RootDataManager *GetInstance();

  void Book();
  void ChopperAddPoint(G4int, G4double, G4double);
  void IntObjAddPoint(G4int, G4double, G4double);
  void WriteToFile();

  void SetFileName(G4String FN){ROOTFileName = FN;}
  G4String GetFileName() {return ROOTFileName;}

  G4bool CheckForTGraphObject(){return ROOTObjectsExist;}

private:
  static RootDataManager *theRootDataManager;
  G4String ROOTFileName;
  TFile *ROOTOutFile;
  TGraph *ROOTChopperIn, *ROOTIntObj;
  G4bool ROOTObjectsExist;

};

#endif
