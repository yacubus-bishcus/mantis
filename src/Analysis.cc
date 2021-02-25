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

#include "Analysis.hh"
#include "G4MPImanager.hh"
#include "RootDataMessenger.hh"
#include <sstream>
#include "G4AutoLock.hh"
#include "TFile.h"
#include "TTree.h"

extern G4bool bremTest;
extern G4bool debug;

G4ThreadLocal Analysis* the_analysis = 0;

G4Mutex rootm = G4MUTEX_INITIALIZER;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Analysis* Analysis::GetAnalysis()
{
   if ( ! the_analysis ) the_analysis = new Analysis;
   return the_analysis;
}


Analysis::Analysis()
: ROOTObjectsExist(false),MPI_Rank(0), MPI_Size(1)
{
  G4AutoLock l(&rootm);
  G4MPImanager *theMPIManager = G4MPImanager::GetManager();
  MPI_Rank = theMPIManager->GetRank();
  MPI_Size = theMPIManager->GetTotalSize();

  GenerateFileNames();
  ChopInTree = new TTree("ChopIn","Chopper Wheel Incident Data");
  ChopInTree->Branch("Energy",&ChopInEnergy);
  ChopInTree->Branch("Weight",&ChopInWeight);
  ChopInTree->Branch("EventID",&ChopInEventID);

  ChopOutTree = new TTree("ChopOut","Chopper Wheel Exiting Radiation Data");
  ChopOutTree->Branch("Energy",&ChopOutEnergy);
  ChopOutTree->Branch("Weight",&ChopOutWeight);
  ChopOutTree->Branch("EventID",&ChopOutEventID);
  ChopOutTree->Branch("isNRF",&ChopOutisNRF);

  if(!bremTest)
  {
    NRFTree = new TTree("NRF","NRF Data");
    NRFTree->Branch("EventID",&NRFEventID);
    NRFTree->Branch("Energy",&NRFEnergy);
    NRFTree->Branch("Weight",&NRFWeight);
    NRFTree->Branch("Material",&Material);
    NRFTree->Branch("ZPos",&ZPos);

    IntObjInTree = new TTree("IntObjIn","Incident Interrogation Object Data");
    IntObjInTree->Branch("Energy",&IntObjInEnergy);
    IntObjInTree->Branch("Weight",&IntObjInWeight);
    IntObjInTree->Branch("CreatorProcess",&IntObjInCreatorProcess);

    IntObjOutTree = new TTree("IntObjOut","Interrogation Object Emission Data");
    IntObjOutTree->Branch("Energy",&IntObjOutEnergy);
    IntObjOutTree->Branch("Weight",&IntObjOutWeight);
    IntObjOutTree->Branch("CreatorProcess",&IntObjOutCreatorProcess);

    WaterInTree = new TTree("Water","Incident Water Tank Data");
    WaterInTree->Branch("Energy",&WaterEnergy);
    WaterInTree->Branch("Weight",&WaterWeight);
    WaterInTree->Branch("CreatorProcess",&WaterCreatorProcess);

    CherenkovTree = new TTree("Cherenkov","Cherenkov in Water Data");
    CherenkovTree->Branch("Energy",&CherenkovEnergy);
    CherenkovTree->Branch("Weight",&CherenkovWeight);
    CherenkovTree->Branch("EventID",&CherenkovEventID);
    CherenkovTree->Branch("NumSecondaries",&CherenkovNumSecondaries);
    CherenkovTree->Branch("Time",&CherenkovTime);

    DetTree = new TTree("DetInfo","Detected Information");
    DetTree->Branch("EventID",&DetEventID);
    DetTree->Branch("Energy",&DetEnergy);
    DetTree->Branch("Weight",&DetWeight);
    DetTree->Branch("CreatorProcess",&DetCreatorProcess);
    DetTree->Branch("Time",&DetTime);

    DetInTree = new TTree("IncDetInfo","Incident Detector Process Information");
    DetInTree->Branch("EventID",&InDetEventID);
    DetInTree->Branch("Energy",&InDetEnergy);
    DetInTree->Branch("Weight",&InDetWeight);
    DetInTree->Branch("DetProcess",&InDetProcess);

  }
  ROOTObjectsExist = true;
  if(debug)
    std::cout << "Analysis::Book() --> Complete!" << std::endl;
}

Analysis::~Analysis()
{
  delete ChopInTree;
  delete ChopOutTree;
  delete NRFTree;
  delete IntObjInTree;
  delete IntObjOutTree;
  delete WaterInTree;
  delete CherenkovTree;
  delete DetTree;
  delete DetInTree;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Analysis::Update()
{
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Analysis::Clear()
{
  ChopInTree->Reset();
  ChopOutTree->Reset();
  NRFTree->Reset();
  IntObjInTree->Reset();
  IntObjOutTree->Reset();
  WaterInTree->Reset();
  CherenkovTree->Reset();
  DetTree->Reset();
  DetInTree->Reset();

  return;
}

void Analysis::Save(const G4String& fname)
{
  G4AutoLock l(&rootm);
  TFile* file = new TFile(fname.c_str(), "RECREATE", "Geant4 ROOT analysis");

  ChopInTree->Write();
  ChopOutTree->Write();
  if(!bremTest)
  {
    NRFTree->Write();
    IntObjInTree->Write();
    IntObjOutTree->Write();
    WaterInTree->Write();
    CherenkovTree->Write();
    DetTree->Write();
    DetInTree->Write();
  }

  file->Close();

  delete file;

  return;
}


void Analysis::finish()
{
    if(MPI_Rank == 0)
    {
      G4String FinalFileName = ROOTFileName.substr(0, ROOTFileName.find(".rank"));
      TFile *FinalFile = new TFile(FinalFileName.c_str(), "update");
      FinalFile->Open(FinalFileName.c_str(), "recreate");

      std::vector<G4String>::iterator it;
      G4String targets = " ";
      for(it = slaveFileNames.begin(); it != slaveFileNames.end(); it++)
      {
        targets += (*it).c_str();
        targets += " ";
      }

      G4String haddCommand = "hadd -f " + FinalFileName + targets;
      system(haddCommand.c_str());

      for(it = slaveFileNames.begin(); it != slaveFileNames.end(); it++)
      {
        G4String RemoveSlaveFileCmd = "rm -f " + (*it);
        system(RemoveSlaveFileCmd.c_str());
      }

    } // for if MPI_RANK == 0
}// for Analysis::finish

void Analysis::FillChopIn(G4double e, G4double w, G4int id)
{
  if (!ROOTObjectsExist) return;
  ChopInEnergy = e;
  ChopInWeight = w;
  ChopInEventID = id;
  ChopInTree->Fill();
}

void Analysis::FillChopOut(G4double e, G4double w, G4int id, G4int nrf)
{
  if (!ROOTObjectsExist) return;
  ChopOutEnergy = e;
  ChopOutWeight = w;
  ChopOutEventID = id;
  ChopOutisNRF = nrf;
  ChopOutTree->Fill();
}

void Analysis::FillNRF(G4double e, G4double w, G4int id, G4String Mat, G4double z)
{
  if (!ROOTObjectsExist) return;
  NRFEnergy = e;
  NRFWeight = w;
  NRFEventID = id;
  Material = Mat;
  ZPos = z;
  NRFTree->Fill();
}

void Analysis::FillIntObjIn(G4double e, G4double w, G4String c)
{
  if (!ROOTObjectsExist) return;
  IntObjInEnergy = e;
  IntObjInWeight = w;
  IntObjInCreatorProcess = c;
  IntObjInTree->Fill();
}

void Analysis::FillIntObjOut(G4double e, G4double w, G4String c)
{
  if (!ROOTObjectsExist) return;
  IntObjOutEnergy = e;
  IntObjOutWeight = w;
  IntObjOutCreatorProcess = c;
  IntObjOutTree->Fill();
}

void Analysis::FillWater(G4double e, G4double w, G4String c)
{
  if (!ROOTObjectsExist) return;
  WaterEnergy = e;
  WaterWeight = w;
  WaterCreatorProcess = c;
  WaterInTree->Fill();
}

void Analysis::FillCherenkov(G4double e, G4double w, G4int id, G4int secondaries, G4double times)
{
  if (!ROOTObjectsExist) return;
  CherenkovEnergy = e;
  CherenkovWeight = w;
  CherenkovEventID = id;
  CherenkovNumSecondaries = secondaries;
  CherenkovTime = times;
  CherenkovTree->Fill();
}

void Analysis::FillDet(G4double e, G4double w, G4int id, G4String c, G4double times)
{
  if (!ROOTObjectsExist) return;
  DetEnergy = e;
  DetWeight = w;
  DetEventID = id;
  DetCreatorProcess = c;
  DetTime = times;
  DetTree->Fill();
}

void Analysis::FillInDet(G4double e, G4double w, G4int id, G4String c)
{
  if (!ROOTObjectsExist) return;
  InDetEnergy = e;
  InDetWeight = w;
  InDetEventID = id;
  InDetProcess = c;
  DetInTree->Fill();
}

void Analysis::GenerateFileNames()
{
  if(MPI_RANK == 0)
  {
    slaveFileNames.Clear();
    for(int rank=0; rank<MPI_Size; ++rank)
    {
      size_t pos = ROOTFileName.find(".rank");
      if(pos != G4String::npos)
      {
        ss.str("");
        ss << ROOTFileName.substr(0,pos) << ".rank" << rank;
        slaveFileNames.push_back((G4String)ss.str());
      }
    }
  }
}
