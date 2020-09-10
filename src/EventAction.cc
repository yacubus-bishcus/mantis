#include "EventAction.hh"


EventAction::EventAction(G4ParticleGun* particle_gun, SteppingAction* stepA,
  const DetectorConstruction* det, HistoManager* histo, const RunAction* run)
: fDetector(det),fPMTCollID(-1),fPMTThreshold(1),fForcedrawphotons(false),
fForcenophotons(false), drawNumPhotonsFlag(0),eventM(NULL)
{
    eventM = new EventMessenger(this);
    particle_gun_local = particle_gun;
    local_histo = histo;
    stepA_local = stepA;
    local_run = run;
    fHitCount = 0;
    fPMTsAboveThreshold = 0;
}

EventAction::~EventAction()
{delete eventM;}


void EventAction::BeginOfEventAction(const G4Event* anEvent)
{

    anEvent->GetEventID();
    ResetEverything(); //reset all the class variables

    fHitCount = 0;
    fPMTsAboveThreshold = 0;

    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    if(fPMTCollID<0)
      fPMTCollID=SDman->GetCollectionID("pmtHitCollection");

}


void EventAction::EndOfEventAction(const G4Event* anEvent)
{

    G4AnalysisManager* manager = G4AnalysisManager::Instance();
    if(drawNumPhotonsFlag)
    {
      G4double E_beam=particle_gun_local->GetParticleEnergy();
      manager->FillNtupleDColumn(4,0,E_beam);
      manager->FillNtupleDColumn(4,1,stepA_local->Ev.size());
      manager->AddNtupleRow(4);
    }



    // End of Event PMT analysis

    PMTHitsCollection* pmtHC = nullptr;
    G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();

    // Get the hit collections
    if(hitsCE){
      if(fPMTCollID>=0) {
        pmtHC = (PMTHitsCollection*)(hitsCE->GetHC(fPMTCollID));
      }
    }

    if(pmtHC){
        G4ThreeVector reconPos(0.,0.,0.);
        G4int pmts=pmtHC->entries();
    //Gather info from all PMTs
        for(G4int i=0;i<pmts;i++){
            fHitCount += (*pmtHC)[i]->GetPhotonCount();
            reconPos+=(*pmtHC)[i]->GetPMTPos()*(*pmtHC)[i]->GetPhotonCount();
            if((*pmtHC)[i]->GetPhotonCount()>=fPMTThreshold){
                fPMTsAboveThreshold++;
            }
            else{//wasnt above the threshold, turn it back off
                (*pmtHC)[i]->SetDrawit(false);
            }
        }
        if(fHitCount > 0) {//dont bother unless there were hits
            reconPos/=fHitCount;
            fReconPos = reconPos;
          }
          pmtHC->DrawAllHits();
        }


    // Update Run root file
    time_t startTime = local_run->getStartTime();
    G4bool printSaveCheck = local_histo->OnceAWhileSave(startTime);
    if(printSaveCheck)
    {
      G4cout << anEvent->GetEventID() << G4endl;
    }
    // show event number for user

    std::cout << "\r\tEvent and tracks:\t " << anEvent->GetEventID() << "\t" << stepA_local->Ev.size() <<std::flush;
}


void EventAction::ResetEverything(){

    stepA_local->Ev.clear();


}
