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

#include "RootDataManager.hh"

extern G4String gOutName;

RootDataManager *RootDataManager::theRootDataManager = 0;
RootDataManager *RootDataManager::GetInstance()
{return theRootDataManager;}

RootDataManager::RootDataManager()
:ROOTFileName(""), ROOTOutFile(new TFile),
ROOTChopperIn(new TGraph), ROOTIntObj(new TGraph),
ROOTObjectsExist(false)
{
  if(theRootDataManager)
  {
    G4cerr << "RootDataManager::RootDataManager() -> FATAL ERROR: The Root Data Manager was constructed twice!" << G4endl;
    return;
  }
  else
  {
    theRootDataManager = this;
  }

}

RootDataManager::~RootDataManager()
{
  delete ROOTOutFile;
}

// Method that creates ROOT File at the beginning of a G4Run
void RootDataManager::Book()
{
  if(ROOTObjectsExist)
  {
    G4cerr << "RootDataManager::Book() -> FATAL ERROR: ROOT Objects presently exist and writeable!" << G4endl;
    return;
  }
  if(ROOTOutFile) delete ROOTOutFile;

  ROOTFileName = gOutName + "_TGraphData.root";
  // Open New File Construct TGraph
  ROOTOutFile = new TFile(ROOTFileName.c_str(),"recreate");
  ROOTChopperIn = new TGraph();
  ROOTIntObj = new TGraph();
  ROOTChopperIn->SetNameTitle("ChopIn","Incident Chopper Wheel Weighted Energy Spectrum");
  ROOTIntObj->SetNameTitle("IntObjIn","Incident Interrogation Object Weighted Energy Spectrum");

  ROOTObjectsExist = true;
}

// Method that fills TGraph with energy, and weight vectors
// Requires index
void RootDataManager::ChopperAddPoint(G4int i, G4double x, G4double y)
{
  G4bool confirm = ROOTOutFile->cd();
  if(!confirm)
  {
    G4cerr << "RootDataManager::WriteToGraph() -> FATAL ERROR Unable to change into ROOTOutFile." << G4endl;
    return;
  }

  G4int index = i;
  G4double Energy = x;
  G4double Weight = y;
  ROOTChopperIn->SetPoint(index, Energy, Weight);
}
void RootDataManager::IntObjAddPoint(G4int i, G4double x, G4double y)
{
  G4bool confirm = ROOTOutFile->cd();
  if(!confirm)
  {
    G4cerr << "RootDataManager::WriteToGraph() -> FATAL ERROR Unable to change into ROOTOutFile." << G4endl;
    return;
  }

  G4int index = i;
  G4double Energy = x;
  G4double Weight = y;
  ROOTIntObj->SetPoint(index, Energy, Weight);
}

// Method to Write TGraphs to OutFile
void RootDataManager::WriteToFile()
{
  RootDataManager* manager = RootDataManager::GetInstance();
  if(!ROOTObjectsExist)
  {
    G4cerr << "RootDataManager::WriteToFile() -> FATAL ERROR ROOT Object does NOT Exist!" << G4endl;
    return;
  }
  G4bool confirm = ROOTOutFile->cd();
  if(!confirm)
  {
    G4cerr << "RootDataManager::WriteToFile() -> FATAL ERROR ROOTFile Not Found!" << G4endl;
    return;
  }

  std::cout << "RootDataManager::WriteToFile() -> Writing to disk." << std::endl;
  G4cout << "RootDataManager::WriteToFile() -> Writing to disk." << G4endl;
  ROOTChopperIn->Write();
  ROOTIntObj->Write();

  std::cout << "RootDataManager::WriteToFile() -> Wrote TGraph Data to " << ROOTFileName << std::endl;
  G4cout << "RootDataManager::WriteToFile() -> Wrote TGraph Data to " << ROOTFileName << G4endl;
  ROOTObjectsExist = false;
  ROOTOutFile->Close();
  delete manager;
  std::cout << "Output Files Closed." << std::endl;
  G4cout << "Output Files Closed." << G4endl;
}
