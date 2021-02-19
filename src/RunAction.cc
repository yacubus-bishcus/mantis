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
extern G4bool weightHisto;

RunAction::RunAction(HistoManager* histoAnalysis, RootDataManager* RootAnalysis)
        : G4UserRunAction(), fHistoManager(histoAnalysis), fRootManager(RootAnalysis)
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run*)
{
        if(output)
        {
                fHistoManager->Book();
                //fRootManager->Book();
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
        G4cout << "Average total energy of Cherenkov photons per event:   "
               << (fCerenkovEnergy/eV)/TotNbofEvents << " eV." << G4endl;
        G4cout << "Average number of Cherenkov photons created per event: "
               << fCerenkovCount/TotNbofEvents << G4endl;

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

        if(output)
        {
                fHistoManager->finish();
                //fRootManager->WriteToFile();
        }
        if(checkEvents)
        {
                EventCheck *eCheck = new EventCheck();
                eCheck->WriteEvents();
        }
        if(weightHisto && checkEvents && output)
        {
                WeightHisto *wHisto = new WeightHisto(fHistoManager->GetEmax());
                wHisto->Fill_NRF_to_Cherenkov();
                wHisto->Fill_to_Det();
        }
}
