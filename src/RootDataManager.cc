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
#include "G4MPImanager.hh"
#include "RootDataMessenger.hh"
#include <sstream>

extern G4String gOutName;
extern G4bool bremTest;
extern G4bool debug;

RootDataManager *RootDataManager::theRootDataManager = 0;


RootDataManager *RootDataManager::GetInstance()
{ return theRootDataManager; }

RootDataManager::RootDataManager(G4bool arch): parallelArchitecture(!arch),
MPI_Rank(0), MPI_Size(1),
ROOTFileName(""), ROOTFile(new TFile), ChopInTree(new TTree), ChopOutTree(new TTree),
NRFTree(new TTree), IntObjInTree(new TTree), IntObjOutTree(new TTree),
WaterInTree(new TTree), CherenkovTree(new TTree), DetTree(new TTree), DetInTree(new TTree),
ROOTObjectsExist(false), ChopInEnergy(0.), ChopInWeight(0.), ChopInEventID(0),
ChopOutEnergy(0.), ChopOutWeight(0.), ChopOutEventID(0), ChopOutisNRF(0),
NRFEnergy(0.),NRFWeight(0.), NRFEventID(0), Material(""), ZPos(0.),
IntObjInEnergy(0.), IntObjInWeight(0.), IntObjInCreatorProcess(""),
IntObjOutEnergy(0.), IntObjOutWeight(0.), IntObjOutCreatorProcess(""),
WaterEnergy(0.),WaterWeight(0.), WaterCreatorProcess(""),
CherenkovEnergy(0.), CherenkovWeight(0.), CherenkovEventID(0), CherenkovNumSecondaries(0), CherenkovTime(0.),
DetEnergy(0.), DetWeight(0.), DetEventID(0), DetCreatorProcess(""), DetTime(0.),
InDetEnergy(0.), InDetWeight(0.), InDetEventID(0), InDetProcess("")

{
  if(theRootDataManager)
  G4Exception("RootDataManager::RootDataManager()", "RootException001",
  FatalException, "The RootDataManager was constructed twice!");
  else
    theRootDataManager = this;

  ROOTFileName = gOutName;
  theMessenger = new RootDataMessenger(this);
}

RootDataManager::~RootDataManager()
{
  delete ROOTFile;
  delete theMessenger;
}

void RootDataManager::Book()
{
  if(ROOTObjectsExist)
  {
    G4cerr << "RootDataManager::Book() -> FATAL ERROR: ROOT Objects already opened!" << G4endl;
    return;
  }
  #ifdef MANTIS_MPI_ENABLED
    MPIManager *theMPIManager = MPIManager::GetInstance();
    MPI_Rank = theMPIManager->GetRank();
    MPI_Size = theMPIManager->GetSize();
  #endif

  GenerateFileNames();
  if(ROOTFile) delete ROOTFile;
  ROOTFile = new TFile(ROOTFileName, "recreate");

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
    std::cout << "RootDataManager::Book() --> Complete!" << std::endl;
}


void RootDataManager::finish()
{
  if(!ROOTObjectsExist)
  {
    G4cerr << "RootDataManager::finish() -> FATAL ERROR: ROOT Objects do not exist to write!" << G4endl;
    return;
  }

  if(parallelArchitecture)
  {
    // Write all TTrees
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
    ROOTFile->Close();

    if(MPI_Rank == 0)
    {
      G4String FinalFileName = ROOTFileName.substr(0, ROOTFileName.find(".slave"));
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
    ROOTObjectsExist = false;
  } // for if parallelArchitecture
  else
  {
    // Write all TTrees
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
    ROOTObjectsExist = false;
  } // for else parallelArchitecture
}// for RootDataManager::finish

void RootDataManager::FillChopIn(G4double e, G4double w, G4int id)
{
  if (!ROOTObjectsExist) return;
  ChopInEnergy = e;
  ChopInWeight = w;
  ChopInEventID = id;
  ChopInTree->Fill();
}

void RootDataManager::FillChopOut(G4double e, G4double w, G4int id, G4int nrf)
{
  if (!ROOTObjectsExist) return;
  ChopOutEnergy = e;
  ChopOutWeight = w;
  ChopOutEventID = id;
  ChopOutisNRF = nrf;
  ChopOutTree->Fill();
}

void RootDataManager::FillNRF(G4double e, G4double w, G4int id, G4String Mat, G4double z)
{
  if (!ROOTObjectsExist) return;
  NRFEnergy = e;
  NRFWeight = w;
  NRFEventID = id;
  Material = Mat;
  ZPos = z;
  NRFTree->Fill();
}

void RootDataManager::FillIntObjIn(G4double e, G4double w, G4String c)
{
  if (!ROOTObjectsExist) return;
  IntObjInEnergy = e;
  IntObjInWeight = w;
  IntObjInCreatorProcess = c;
  IntObjInTree->Fill();
}

void RootDataManager::FillIntObjOut(G4double e, G4double w, G4String c)
{
  if (!ROOTObjectsExist) return;
  IntObjOutEnergy = e;
  IntObjOutWeight = w;
  IntObjOutCreatorProcess = c;
  IntObjOutTree->Fill();
}

void RootDataManager::FillWater(G4double e, G4double w, G4String c)
{
  if (!ROOTObjectsExist) return;
  WaterEnergy = e;
  WaterWeight = w;
  WaterCreatorProcess = c;
  WaterInTree->Fill();
}

void RootDataManager::FillCherenkov(G4double e, G4double w, G4int id, G4int secondaries, G4double times)
{
  if (!ROOTObjectsExist) return;
  CherenkovEnergy = e;
  CherenkovWeight = w;
  CherenkovEventID = id;
  CherenkovNumSecondaries = secondaries;
  CherenkovTime = times;
  CherenkovTree->Fill();
}

void RootDataManager::FillDet(G4double e, G4double w, G4int id, G4String c, G4double times)
{
  if (!ROOTObjectsExist) return;
  DetEnergy = e;
  DetWeight = w;
  DetEventID = id;
  DetCreatorProcess = c;
  DetTime = times;
  DetTree->Fill();
}

void RootDataManager::FillInDet(G4double e, G4double w, G4int id, G4String c)
{
  if (!ROOTObjectsExist) return;
  InDetEnergy = e;
  InDetWeight = w;
  InDetEventID = id;
  InDetProcess = c;
  DetInTree->Fill();
}

void RootDataManager::GenerateFileNames()
{
  if(ROOTObjectsExist)
  {
    G4cerr << "RootDataManager::GenerateFileNames -> ROOT Objects Already Exist before File!" << G4endl;
    return;
  }

  std::stringstream ss;
  if(parallelArchitecture)
  {
    ss << ROOTFileName << ".slave" << MPI_Rank;
    ROOTFileName = ss.str();
  }

  // Generate a class member vector list (on the master node only) of
  // all the slave file names. This list will be used for MPI
  // reduction before the ROOT writing process. Note that slave file
  // names are cleared at every time Book() is called in
  // order to enable multiple ROOT files to be written for many runs
  // within a single MANTIS session

  if(MPI_Rank == 0)
  {
    if(parallelArchitecture)
    {
      slaveFileNames.clear();

      for(int rank=0; rank<MPI_Size; rank++)
      {
      	size_t pos = ROOTFileName.find("slave");
      	if(pos != G4String::npos)
        {
      	  ss.str("");
      	  ss << ROOTFileName.substr(0,pos) << "slave" << rank;
      	  slaveFileNames.push_back((G4String)ss.str());
      	}
      	else
      	  G4Exception("RootDataManager::BeginRunCleanup()","RootException002",
      		      FatalException,"MANTIS could not find slaves files!");
      }// for for loop
    }// for if parallelArchitecture
  } // for if MPI_Rank == 0
} // for GenerateFileNames

void RootDataManager::ReduceSlaveValuesToMaster()
{
#ifdef MANTIS_MPI_ENABLED

  G4cout << "MANTIS ANNOUNCEMENT: Beginning the MPI reduction of data to the master!"
	 << G4endl;

  G4cout << "MANTIS ANNOUNCEMENT: Finished the MPI reduction of values to the master!\n"
	 << G4endl;
#endif
}
