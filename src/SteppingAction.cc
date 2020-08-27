#include "SteppingAction.hh"
#include "EventAction.hh"
#include "PMTSD.hh"
#include "StackingAction.hh"
#include "HistoManager.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "vector"
#include "globals.hh"
#include "G4SystemOfUnits.hh"


SteppingAction::SteppingAction(G4ParticleGun* particle_gun, RunAction* localrun)
: G4UserSteppingAction()
{
    particle_gun_local = particle_gun;
    fExpectedNextStatus = Undefined;
    run = localrun;
}

SteppingAction::~SteppingAction()
{ ; }

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

    G4String particleName = aStep->GetTrack()->GetDynamicParticle()
    ->GetParticleDefinition()->GetParticleName();
    G4AnalysisManager* manager = G4AnalysisManager::Instance();
    // Run Logical Checks
    if(aStep->GetPostStepPoint()==NULL){
        return; // at the end of the world
    }
    else if(aStep->GetPostStepPoint()->GetPhysicalVolume()==NULL){
        return;
    }

    G4Track* theTrack = aStep->GetTrack();

    if (particleName == "opticalphoton") {
         const G4VProcess* pds = aStep->GetPostStepPoint()->
                                                      GetProcessDefinedStep();
         if (pds->GetProcessName() == "OpAbsorption") {
             run->AddOpAbsorption();

         }
         else if (pds->GetProcessName() == "OpRayleigh") {
             run->AddRayleigh();
         }
     }

    // Water Analysis


    // Here I am inside the water
    if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0,5,"Water")==0){

        // only care about secondaries that occur in water volume
        const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();

        if(secondaries->size()>0){
            for(unsigned int i=0; i<secondaries->size(); ++i){
                if(secondaries->at(i)->GetParentID()>0){
                    if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
                        if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Scintillation"){
                            G4double en = secondaries->at(i)->GetKineticEnergy();
                            run->AddScintillationEnergy(en);
                            run->AddScintillation();
                        }
                        if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Cerenkov"){
                            // for total run
                            G4double en = secondaries->at(i)->GetKineticEnergy();
                            run->AddCerenkovEnergy(en);
                            run->AddCerenkov();
                            //for step

                        }

                        // only care about cerernkov
                        // root file
                        X= secondaries->at(i)->GetPosition(); //take the position from the post step position
                        p = secondaries->at(i)->GetMomentum();

                        manager->FillNtupleDColumn(0,0, secondaries->at(i)->GetKineticEnergy()/(MeV));
                        manager->FillNtupleDColumn(0,1, X.x()/(cm));
                        manager->FillNtupleDColumn(0,2, X.y()/(cm));
                        manager->FillNtupleDColumn(0,3, X.z()/(cm));
                        manager->FillNtupleDColumn(0,4, asin(sqrt(pow(p.x(),2)+pow(p.y(),2))/p.mag()));
                        manager->FillNtupleDColumn(0,5, secondaries->at(i)->GetGlobalTime());
                        manager->AddNtupleRow(0);

                        Ev.push_back( secondaries->at(i)->GetKineticEnergy()/(MeV));// records for histogram


                    }
                }
            }
        } // end of optical photons if statement

    } // end of if loop while inside water

    // PMT Analysis

    G4StepPoint* endPoint   = aStep->GetPostStepPoint();
    G4StepPoint* startPoint = aStep->GetPreStepPoint();

    if(endPoint->GetStepStatus() == fGeomBoundary){

        const G4DynamicParticle* theParticle = theTrack->GetDynamicParticle();

        G4ThreeVector oldMomentumDir = theParticle->GetMomentumDirection();

        G4ThreeVector m0 = startPoint->GetMomentumDirection(); // don't use these yet?
        G4ThreeVector m1 = endPoint->GetMomentumDirection();

        G4OpBoundaryProcessStatus theStatus = Undefined;
        G4ProcessManager* OpManager =
          G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
        G4int MAXofPostStepLoops =
          OpManager->GetPostStepProcessVector()->entries();
        G4ProcessVector* postStepDoItVector =
          OpManager->GetPostStepProcessVector(typeDoIt);

        if(endPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")==0 && startPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")!=0){ // first time in photocathode
            run->AddTotalSurface();
            //incX = theParticle->GetPosition();
            manager->FillNtupleDColumn(1,0,theParticle->GetKineticEnergy()/(MeV));
            Xdet = endPoint->GetPosition();
            manager->FillNtupleDColumn(1,1,Xdet.x()/(cm));
            manager->FillNtupleDColumn(1,2,Xdet.y()/(cm));
            manager->FillNtupleDColumn(1,3,Xdet.z()/(cm));
            manager->AddNtupleRow(1);

            for (G4int i=0; i<MAXofPostStepLoops; ++i) {
                G4VProcess* currentProcess = (*postStepDoItVector)[i];

                G4OpBoundaryProcess* opProc = dynamic_cast<G4OpBoundaryProcess*>(currentProcess);

                if(opProc){
                    theStatus = opProc->GetStatus();

                    if(theStatus == Transmission){
                        //run->AddTransmission();
                        procCount = "Trans";
                        // G4cout << "transmission"<< G4endl;
                    }
                    else if(theStatus == FresnelRefraction){
                        //run->AddFresnelRefraction();
                        procCount = "Refr";
                        //G4cout << "fres refraction"<< G4endl;
                    }
                    else if (theStatus == TotalInternalReflection) {
                      //run->AddTotalInternalReflection();
                        procCount = "Int_Refl";
                        //G4cout << "totalinternal" << G4endl;
                    }
                    else if (theStatus == LambertianReflection) {
                      //run->AddLambertianReflection();
                        procCount = "Lamb";
                    }
                    else if (theStatus == LobeReflection) {
                      //run->AddLobeReflection();
                        procCount = "Lobe";
                    }
                    else if (theStatus == SpikeReflection) {
                      //run->AddSpikeReflection();
                        procCount = "Spike";
                    }
                    else if (theStatus == BackScattering) {
                      //run->AddBackScattering();
                        procCount = "BackS";
                    }
                    else if (theStatus == Absorption) {
                      //run->AddAbsorption();
                        procCount = "Abs";
                    }
                    else if (theStatus == Detection) {
                      //run->AddDetection();
                        procCount = "Det";
                        //G4cout << "Detection" << G4endl;
                    }
                    else if (theStatus == NotAtBoundary) {
                      //run->AddNotAtBoundary();
                        procCount = "NotAtBoundary";
                    }
                    else if (theStatus == SameMaterial) {
                      //run->AddSameMaterial();
                        procCount = "SameMaterial";
                    }
                    else if (theStatus == StepTooSmall) {
                      //run->AddStepTooSmall();
                        procCount = "SteptooSmall";
                    }
                    else if (theStatus == NoRINDEX) {
                      //run->AddNoRINDEX();
                        procCount = "NoRINDEX";
                        //G4cout << "No Rindex" << G4endl;
                    }
                    else {
                        G4cout << "theStatus: " << theStatus
                               << " was none of the above." << G4endl;
                        procCount = "noStatus";
                      }
                } // for if opProc
                manager->FillNtupleSColumn(2,0,procCount);
                manager->AddNtupleRow(2);
            } // for for loop


        } // for if statement if first time in photocathode

    } // for if at boundary
    //G4cout << "Stepping Fine" << G4endl;

} // end of user steepping action function


bool SteppingAction::TrackMustDie(const G4Step *aStep){

    //Energy cuts
    if(aStep->GetTrack()->GetDefinition()->GetPDGEncoding()==EventGeneratorParticle &&
       aStep->GetTrack()->GetKineticEnergy()/MeV < LowEnergyCutoff){ //for the main track, kill it off below some limit
      return true;
    }

    return false;
}
