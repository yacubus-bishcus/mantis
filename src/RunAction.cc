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

#include "RunAction.hh"
extern G4bool output;
extern G4bool checkEvents;
extern G4double chosen_energy;

RunAction::RunAction(RootDataManager* analysis, PrimaryGeneratorAction* pga, G4bool build)
        : G4UserRunAction(), fmanager(analysis), fpga(pga), fbuild(!build), nodeRank(0)
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  #ifdef MANTIS_MPI_ENABLED
    MPIManager *theMPIManager = MPIManager::GetInstance();
    nodeRank = theMPIManager->GetRank();

    G4double masterEvents = theMPIManager->GetMasterEvents();
    G4double slaveEvents = theMPIManager->GetSlaveEvents();

    G4cout << "MANTIS ANNOUNCEMENT: # events in master = " << masterEvents
	   << " / # events in slave = "  << slaveEvents << "\n" << G4endl;

     theMPIManager->ForceBarrier("RunAction::BeginOfRunAction()");
   #endif
    if(output)
    {
      fmanager->Book();
    }

    fTotalSurface = 0;
    fCerenkovCount = 0;
    fScintCount = 0;
    fCerenkovEnergy = 0;
    fScintEnergy = 0;
    fNRF = 0;
    fStatusKilled = 0;
    G4cout << G4endl << "Beginning Run..." << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  #ifdef MANTIS_MPI_ENABLED
    MPIManager::GetInstance()->ForceBarrier("RunAction::EndOfRunAction()");
  #endif
  if(nodeRank == 0)
  {
    if(chosen_energy < 0)
    {
      fpga->CloseInputFile();
      G4cout << "RunAction::EndOfRunAction -> PrimaryGeneratorAction Input File Closed." << G4endl;
    }

    if(fbuild)
    {
      #ifdef MANTIS_MPI_ENABLED
        totalEvents = MPIManager::GetInstance()->GetInstance()->GetTotalEvents();
      #endif
    }
    else
    {
      G4int TotNbofEvents = aRun->GetNumberOfEvent();
      std::ios::fmtflags mode = G4cout.flags();
      G4int prec = G4cout.precision(2);
      G4cout << G4endl << "Run Summary" << G4endl;
      G4cout << "----------------------------------------------------------------------" << G4endl;
      G4cout << "Total Number of Events:                                " << TotNbofEvents << G4endl;
      G4cout << "Total number of Surface Events:                        " << fTotalSurface << G4endl;
      G4cout << "Total number of NRF Photons:                           " << fNRF << G4endl;
      G4cout << "Total number of Cherenkov Photons:                     " << fCerenkovCount << G4endl;
      G4cout << "Total number of Scintillation Photons:                 " << fScintCount << G4endl;
      G4cout << "Total number of Optical Photons:                       " << fCerenkovCount + fScintCount << G4endl;
      G4cout << "Total number of Tracks Cut Based on Position:          " << fStatusKilled << G4endl;

      if (fCerenkovCount > 0)
      {
              G4cout << " Average Cherenkov Photon energy emitted:            "
                     << (fCerenkovEnergy/eV)/fCerenkovCount << " eV." << G4endl;
      }

      if (fScintCount > 0)
      {
              G4cout << " Average Scintillation Photon energy emitted:        "
                     << (fScintEnergy/eV)/fScintCount << " eV." << G4endl;
      }

      G4cout << "----------------------------------------------------------------------" << G4endl;

      G4cout.setf(mode, std::ios::floatfield);

      G4cout.precision(prec);
    }
  }

  if(output)
  {
    fmanager->finish();
  }
  
  if(checkEvents)
  {
    EventCheck *eCheck = new EventCheck();
    eCheck->WriteEvents();
  }
}
