#include "SteppingAction.hh"

SteppingAction::SteppingAction(const DetectorConstruction* det, RunAction* localrun, EventAction* localEvent, G4bool brem)
        : G4UserSteppingAction(), drawChopperDataFlag(0), drawNRFDataFlag(0), drawIntObjDataFlag(0),
        drawIncFlag(0), drawDetFlag(0), drawWaterIncDataFlag(0), stepM(NULL)
{
        stepM = new StepMessenger(this);
        local_det = det;
        fExpectedNextStatus = Undefined;
        event = localEvent;
        run = localrun;
        bremTest = brem;
}

SteppingAction::~SteppingAction()
{
        delete stepM;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

        G4StepPoint* endPoint   = aStep->GetPostStepPoint();
        G4StepPoint* startPoint = aStep->GetPreStepPoint();
        // Run Logical Checks
        if(endPoint == NULL) {
                return; // at the end of the world
        }
        else if(endPoint->GetPhysicalVolume()==NULL) {
                return;
        }

        G4Track* theTrack = aStep->GetTrack();
        // kill photons past IntObj
        G4double EndIntObj = local_det->getEndIntObj();

        // Run Cuts
        if(theTrack->GetPosition().z()/(cm) > EndIntObj/(cm))
        {
                // kill photons that go beyond the interrogation object
                theTrack->SetTrackStatus(fStopAndKill);
                run->AddStatusKilled();
        }
        else if(startPoint->GetPhysicalVolume()->GetName().compare(0, 3, "Col") == 0)
        {
                // kill photons in collimator
                theTrack->SetTrackStatus(fStopAndKill);
                run->AddStatusKilled();
        }

// *********************** Checks and Cuts Complete ************************ //

        G4bool isNRF = false;
        // Grab Weights from PrimaryGenerator
        eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
        weight = info->GetWeight();
        G4AnalysisManager* manager = G4AnalysisManager::Instance();
        
        // Keep track of the materials where NRF originates 
        if(drawNRFDataFlag)
        {
          const G4VProcess* process = endPoint->GetProcessDefinedStep();
          if(process->GetProcessName() == "NRF")
          {
            run->AddNRF();
            manager->FillNtupleDColumn(2,0,theTrack->GetKineticEnergy()/(MeV));
            manager->FillNtupleSColumn(2,1,endPoint->GetPhysicalVolume()->GetName());
            manager->FillNtupleIColumn(2,2,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
            G4ThreeVector NRF_loc = theTrack->GetPosition();
            manager->FillNtupleDColumn(2,3, NRF_loc.z()/(cm));
            manager->AddNtupleRow(2);
          }
        }
        
        if(theTrack->GetCreatorProcess() !=0)
        {
                G4String CPName = theTrack->GetCreatorProcess()->GetProcessName();
                if(CPName == "NRF")
                {
                        isNRF = true;
                }
        }

        // Testing Brem Beam Analysis
        if(drawChopperDataFlag)
        {
                if(endPoint->GetPhysicalVolume()->GetName().compare(0, 4,"Chop") == 0
                   && startPoint->GetPhysicalVolume()->GetName().compare(0, 4, "Chop") != 0
                   && theTrack->GetParticleDefinition() == G4Gamma::Definition())
                {
                        manager->FillNtupleDColumn(0,0, theTrack->GetKineticEnergy()/(MeV)); // not weighting chopper
                        manager->AddNtupleRow(0);
                }
        }
        // Testing NRF Analysis
        // inside Interogation Object for first time
        if(drawIntObjDataFlag && !bremTest)
        {
                if(endPoint->GetPhysicalVolume()->GetName().compare(0, 6,"IntObj") == 0
                   && startPoint->GetPhysicalVolume()->GetName().compare(0, 6, "IntObj") != 0)
                {
                   manager->FillH1(0, theTrack->GetKineticEnergy()/(MeV), weight);
                   
                   if(isNRF && drawNRFDataFlag)
                   {
                     manager->FillH1(1, theTrack->GetKineticEnergy()/(MeV), weight);
                   }
                }
        }

        // Water Analysis
        // First time incident Water keep track of NRF hitting water, low energy and high energy photons hitting water 
        if(drawWaterIncDataFlag && !bremTest)
        {
                if(endPoint->GetPhysicalVolume()->GetName().compare(0, 5,"Water") == 0
                   && startPoint->GetPhysicalVolume()->GetName().compare(0, 5, "Water") != 0)
                {
                  if(isNRF && drawNRFDataFlag)
                  {
                    manager->FillH1(2, theTrack->GetKineticEnergy()/(MeV), weight);
                  }
                  if(theTrack->GetKineticEnergy()/(MeV) < 1E-5)
                  {
                    manager->FillH1(3, theTrack->GetKineticEnergy()/(MeV), weight);
                  }
                  else{manager->FillH1(4, theTrack->GetKineticEnergy()/(MeV), weight);}
                }
        }

        // While in water keep track of cherenkov and pass number of cherenkov to EventAction
        if(startPoint->GetPhysicalVolume()->GetName().compare(0,5,"Water")==0) {

                // only care about secondaries that occur in water volume
                const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();

                if(secondaries->size()>0) {
                        for(unsigned int i=0; i<secondaries->size(); ++i) {
                                if(secondaries->at(i)->GetParentID()>0) {
                                        if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
                                                if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Scintillation") {
                                                        run->AddScintillationEnergy(secondaries->at(i)->GetKineticEnergy());
                                                        run->AddScintillation();
                                                }
                                                if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Cerenkov") {
                                                        // for total run
                                                        run->AddCerenkovEnergy(secondaries->at(i)->GetKineticEnergy());
                                                        run->AddCerenkov();
                                                        event->AddCherenkov();
                                                        if(isNRF)
                                                        {
                                                            event->AddNRF();    
                                                        }
                                                }
                                        }
                                }
                        }
                } // end of optical photons if statement

        } // end of if loop while inside water

        // Photocathode Analysis

        if(endPoint->GetStepStatus() == fGeomBoundary) {

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
                // incident photocathode Keep track of low and high energy photons incident Photocathode volume 
                if(endPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")==0 && startPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")!=0) { // first time in photocathode
                        run->AddTotalSurface();
                        if(drawIncFlag && !bremTest)
                        {
                          if(theParticle->GetKineticEnergy()/(MeV) < 1E-5)
                          {
                            manager->FillH1(5, theParticle->GetKineticEnergy()/(MeV), weight);
                          }
                          else
                          {
                            manager->FillH1(6,theParticle->GetKineticEnergy()/(MeV),weight);
                          }
                        }

                        for (G4int i=0; i<MAXofPostStepLoops; ++i) {
                                G4VProcess* currentProcess = (*postStepDoItVector)[i];

                                G4OpBoundaryProcess* opProc = dynamic_cast<G4OpBoundaryProcess*>(currentProcess);

                                if(opProc && !bremTest) {
                                        theStatus = opProc->GetStatus();

                                        if(theStatus == Transmission) {
                                                //run->AddTransmission();
                                                procCount = "Trans";
                                                // G4cout << "transmission"<< G4endl;
                                        }
                                        else if(theStatus == FresnelRefraction) {
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
                                        // Keep track of detected photons 
                                        else if (theStatus == Detection) {
                                                //run->AddDetection();
                                                procCount = "Det";
                                                manager->FillH1(7, theParticle->GetKineticEnergy()/(MeV), weight);
                                        }
                                        else if (theStatus == NotAtBoundary) {
                                                procCount = "NotAtBoundary";
                                        }
                                        else if (theStatus == SameMaterial) {
                                                procCount = "SameMaterial";
                                        }
                                        else if (theStatus == StepTooSmall) {
                                                procCount = "SteptooSmall";
                                        }
                                        else if (theStatus == NoRINDEX) {
                                                procCount = "NoRINDEX";
                                        }
                                        else {
                                                G4cout << "theStatus: " << theStatus
                                                       << " was none of the above." << G4endl;
                                                procCount = "noStatus";
                                        }
                                } // for if opProc
                                // Keep track of Detector processes 
                                if(drawDetFlag && !bremTest)
                                {
                                        manager->FillNtupleSColumn(3,0,procCount);
                                        manager->AddNtupleRow(3);
                                }
                        } // for for loop
                } // for if statement if first time in photocathode
        } // for if at boundary
} // end of user steepping action function
