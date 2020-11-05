#include "RunAction.hh"
extern G4bool output;
RunAction::RunAction(HistoManager* histoAnalysis, G4bool brem)
        : G4UserRunAction(),fTimer(nullptr), fHistoManager(histoAnalysis)
{

        fTimer = new G4Timer;
        bremTest = brem;
}

RunAction::~RunAction()
{
        delete fTimer;
}


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

        fTimer->Start();
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
        fTimer->Stop();

        G4int TotNbofEvents = aRun->GetNumberOfEvent();
        std::ios::fmtflags mode = G4cout.flags();
        G4int prec = G4cout.precision(2);
        G4cout << G4endl << "Run Summary" << G4endl;
        G4cout <<   "---------------------------------" << G4endl;
        G4cout << "Total Number of Events: " << TotNbofEvents << G4endl;
        G4cout << "Total number of Surface Events: " << fTotalSurface << G4endl;
        G4cout << "Total number of NRF Photons: " << fNRF << G4endl;
        G4cout << "Total number of Tracks Cut Based on Position: " << fStatusKilled << G4endl;
        G4cout <<
                "Average total energy of Cerenkov photons created in the Water per event: " << (fCerenkovEnergy/eV)/TotNbofEvents << " eV." << G4endl;
        G4cout << "Average number of Cerenkov photons created in the Water per event: " << fCerenkovCount/TotNbofEvents << G4endl;

        if (fCerenkovCount > 0) {
                G4cout << " Average Cherenkov Photon energy emitted in Water: " << (fCerenkovEnergy/eV)/fCerenkovCount << " eV." << G4endl;
        }

        G4cout <<
                "Average total energy of scintillation photons created per event in the Water: " << (fScintEnergy/eV)/TotNbofEvents << " eV." << G4endl;
        G4cout <<
                "Average number of scintillation photons created per event in the Water: " << fScintCount/TotNbofEvents << G4endl;

        if (fScintCount > 0) {
                G4cout << " Average Scintillation Photon energy emitted in the Water: " << (fScintEnergy/eV)/fScintCount << " eV." << G4endl;
        }

        G4cout << "Average number of OpRayleigh per event:   " << fRayleighCount/TotNbofEvents << G4endl;
        G4cout << "Average number of OpAbsorption per event: " << fOpAbsorption/TotNbofEvents << G4endl;

        //G4double bdry = G4double(fBoundaryAbsorptionCount)/TotNbofEvents;
        //G4cout << "Number of photons absorbed at boundary per event: " << bdry << G4endl;

        G4cout <<   "---------------------------------" << G4endl;

        G4cout.setf(mode, std::ios::floatfield);

        G4cout.precision(prec);
        // Also print to screen and not just to log file
        std::cout << std::endl << "Run Summary" << std::endl;
        std::cout <<   "---------------------------------" << std::endl;
        std::cout << "Total Number of Events: " << TotNbofEvents << std::endl;
        std::cout << "Total number of Surface Events: " << fTotalSurface << std::endl;
        std::cout << "Total number of NRF Photons: " << fNRF << std::endl;
        std::cout << "Total number of Tracks Cut Based on Position: " << fStatusKilled << std::endl;
        std::cout <<
                "Average total energy of Cerenkov photons created in the Water per event: " << (fCerenkovEnergy/eV)/TotNbofEvents << " eV." << std::endl;
        std::cout << "Average number of Cerenkov photons created in the Water per event: " << fCerenkovCount/TotNbofEvents << std::endl;

        if (fCerenkovCount > 0) {
                std::cout << " Average Cherenkov Photon energy emitted in Water: " << (fCerenkovEnergy/eV)/fCerenkovCount << " eV." << std::endl;
        }

        std::cout <<
                "Average total energy of scintillation photons created per event in the Water: " << (fScintEnergy/eV)/TotNbofEvents << " eV." << std::endl;
        std::cout <<
                "Average number of scintillation photons created per event in the Water: " << fScintCount/TotNbofEvents << std::endl;

        if (fScintCount > 0) {
                std::cout << " Average Scintillation Photon energy emitted in the Water: " << (fScintEnergy/eV)/fScintCount << " eV." << std::endl;
        }

        std::cout << "Average number of OpRayleigh per event:   " << fRayleighCount/TotNbofEvents << std::endl;
        std::cout << "Average number of OpAbsorption per event: " << fOpAbsorption/TotNbofEvents << std::endl;

        //G4double bdry = G4double(fBoundaryAbsorptionCount)/TotNbofEvents;
        //std::cout  << "Number of photons absorbed at boundary per event: " << bdry << std::endl;

        std::cout <<   "---------------------------------" << std::endl;

if(output)
{
  fHistoManager->finish();
}

}
