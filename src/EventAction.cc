#include "EventAction.hh"
#include "PMTHit.hh"
#include "DetectorConstruction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
#include "HistoManager.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"

#include <vector>
#include "G4ParticleGun.hh"
#include <iostream>


EventAction::EventAction(G4ParticleGun* particle_gun, SteppingAction* stepA,
  const DetectorConstruction* det)
: fDetector(det),fPMTCollID(-1),fPMTThreshold(1),fForcedrawphotons(false),
fForcenophotons(false)
{
    particle_gun_local = particle_gun;
    stepA_local = stepA;
    fHitCount = 0;
    fPMTsAboveThreshold = 0;
}

EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event* anEvent)
{

    G4int EventID = anEvent->GetEventID();
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
    G4double E_beam=particle_gun_local->GetParticleEnergy();
    manager->FillNtupleDColumn(3,0,E_beam);
    manager->FillNtupleDColumn(3,1,stepA_local->Ev.size());
    manager->AddNtupleRow(3);


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


    // Update Run Statistics


    // show event number for user

    std::cout << "\r\tEvent and tracks:\t " << anEvent->GetEventID() << "\t" << stepA_local->Ev.size() <<std::flush;
}


void EventAction::ResetEverything(){

    stepA_local->Ev.clear();

    stepA_local->detector_hit.clear();

}
