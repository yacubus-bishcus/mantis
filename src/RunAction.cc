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
RunAction::RunAction(HistoManager* histoAnalysis, G4bool brem, G4bool output_in, G4bool checkEvents_in)
        : G4UserRunAction(), fHistoManager(histoAnalysis)
{
  bremTest = brem;
  output = output_in; 
  checkEvents = checkEvents_in;
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  if(output)
  {
    fHistoManager->Book(bremTest);
  }
  fTotalSurface = 0;
  fCerenkovCount = 0;
  fScintCount = 0;
  fRayleighCount = 0;
  fOpAbsorption = 0;
  fCerenkovEnergy = 0;
  fScintEnergy = 0;
  fNRF = 0;
  fStatusKilled = 0;
  std::cout << "Beginning Run..." << std::endl;
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int TotNbofEvents = aRun->GetNumberOfEvent();
  std::ios::fmtflags mode = G4cout.flags();
  G4int prec = G4cout.precision(2);
  G4cout << G4endl << "Run Summary" << G4endl;
  G4cout <<   "---------------------------------" << G4endl;
  G4cout << "Total Number of Events: " << TotNbofEvents << G4endl;
  G4cout << "Total number of Surface Events: " << fTotalSurface << G4endl;
  G4cout << "Total number of NRF Photons: " << fNRF << G4endl;
  G4cout << "Total number of Cherenkov Photons: " << fCerenkovCount << G4endl;
  G4cout << "Total number of Tracks Cut Based on Position: " << fStatusKilled << G4endl;
  G4cout << "Average total energy of Cerenkov photons created in the Water per event: " 
          << (fCerenkovEnergy/eV)/TotNbofEvents << " eV." << G4endl;
  G4cout << "Average number of Cerenkov photons created in the Water per event: " 
          << fCerenkovCount/TotNbofEvents << G4endl;

  if (fCerenkovCount > 0) 
  {
    G4cout << " Average Cherenkov Photon energy emitted in Water: " 
            << (fCerenkovEnergy/eV)/fCerenkovCount << " eV." << G4endl;
  }

  G4cout << "Average total energy of scintillation photons created per event in the Water: " 
          << (fScintEnergy/eV)/TotNbofEvents << " eV." << G4endl;
  G4cout << "Average number of scintillation photons created per event in the Water: " 
          << fScintCount/TotNbofEvents << G4endl;

  if (fScintCount > 0) 
  {
    G4cout << " Average Scintillation Photon energy emitted in the Water: " 
            << (fScintEnergy/eV)/fScintCount << " eV." << G4endl;
  }

  G4cout <<   "---------------------------------" << G4endl;

  G4cout.setf(mode, std::ios::floatfield);

  G4cout.precision(prec);
  // Also print to screen and not just to log file
  std::cout << std::endl << "Run Summary" << std::endl;
  std::cout <<   "---------------------------------" << std::endl;
  std::cout << "Total Number of Events: " << TotNbofEvents << std::endl;
  std::cout << "Total number of Surface Events: " << fTotalSurface << std::endl;
  std::cout << "Total number of NRF Photons: " << fNRF << std::endl;
  std::cout << "Total number of Cherenkov Photons: " << fCerenkovCount << std::endl;
  std::cout << "Total number of Tracks Cut Based on Position: " << fStatusKilled << std::endl;
  std::cout << "Average total energy of Cerenkov photons created in the Water per event: " 
              << (fCerenkovEnergy/eV)/TotNbofEvents << " eV." << std::endl;
  std::cout << "Average number of Cerenkov photons created in the Water per event: " 
              << fCerenkovCount/TotNbofEvents << std::endl;

  if (fCerenkovCount > 0) 
  {
    std::cout << " Average Cherenkov Photon energy emitted in Water: " 
                << (fCerenkovEnergy/eV)/fCerenkovCount << " eV." << std::endl;
  }

  std::cout << "Average total energy of scintillation photons created per event in the Water: " 
              << (fScintEnergy/eV)/TotNbofEvents << " eV." << std::endl;
  std::cout << "Average number of scintillation photons created per event in the Water: " 
              << fScintCount/TotNbofEvents << std::endl;

  if (fScintCount > 0) 
  {
    std::cout << " Average Scintillation Photon energy emitted in the Water: " << (fScintEnergy/eV)/fScintCount << " eV." << std::endl;
  }

  std::cout <<   "---------------------------------" << std::endl;

  if(output)
  {
    fHistoManager->finish();
  }
  if(checkEvents)
  {
    EventCheck *eCheck = new EventCheck();  
    eCheck->WriteEvents();
  }
  
}
