#include "SteppingAction.hh"

SteppingAction::SteppingAction(const DetectorConstruction* det, RunAction* localrun, G4bool brem)
        : G4UserSteppingAction(), drawChopperDataFlag(0), drawIntObjDataFlag(0),
        drawIncFlag(0), drawDetFlag(0), drawWaterIncDataFlag(0), stepM(NULL)
{
        stepM = new StepMessenger(this);
        local_det = det;
        fExpectedNextStatus = Undefined;
        run = localrun;
        bremTest = brem;
}

SteppingAction::~SteppingAction()
{
        delete stepM;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

        // Run Logical Checks
        if(aStep->GetPostStepPoint()==NULL) {
                return; // at the end of the world
        }
        else if(aStep->GetPostStepPoint()->GetPhysicalVolume()==NULL) {
                return;
        }

        G4Track* theTrack = aStep->GetTrack();
        // kill photons past IntObj
        G4double EndIntObj = local_det->getEndIntObj();

        // Run Cuts
        if(theTrack->GetPosition().z()/(cm) > EndIntObj/(cm))
        //if(theTrack->GetPosition().z()/(cm) > 20.*cm) // for testing brem beam
        {
                // kill photons that go beyond the interrogation object
                theTrack->SetTrackStatus(fStopAndKill);
                run->AddStatusKilled();
        }
        else if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 10, "Collimator") == 0)
        {
                // kill photons in collimator
                theTrack->SetTrackStatus(fStopAndKill);
                run->AddStatusKilled();
        }
        else if(theTrack->GetPosition().z()/(cm) < -10.*cm)
        {
                // Kill photons that go in behind beam origin
                theTrack->SetTrackStatus(fStopAndKill);
                run->AddStatusKilled();
        }

// *********************** Checks and Cuts Complete ************************ //

        G4int isNRF = 0;
        eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
        weight = info->GetWeight();
        G4String particleName = aStep->GetTrack()->GetDynamicParticle()
                                ->GetParticleDefinition()->GetParticleName();
        G4AnalysisManager* manager = G4AnalysisManager::Instance();

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

        if(theTrack->GetCreatorProcess() !=0)
        {
                G4String CPName = theTrack->GetCreatorProcess()->GetProcessName();
                if(CPName == "NRF")
                {
                        run->AddNRF();
                        isNRF = 1;
                }
        }

        // Testing Brem Beam Analysis
        if(drawChopperDataFlag)
        {
                if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().compare(0, 7,"Chopper") == 0
                   && aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 7, "Chopper") != 0
                   && theTrack->GetParticleDefinition() == G4Gamma::Definition())
                {
                        G4double energy_chopper = theTrack->GetKineticEnergy()/(MeV);
                        manager->FillNtupleDColumn(0,0, energy_chopper); // not weighting chopper
                        manager->AddNtupleRow(0);
                }
        }
        // Testing NRF Analysis
        // inside Interogation Object for first time
        if(drawIntObjDataFlag && !bremTest)
        {
                if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().compare(0, 14,"IntObjPhysical") == 0
                   && aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 14, "IntObjPhysical") != 0)
                {
                        G4double energy_IntObj = theTrack->GetKineticEnergy()/(MeV);
                        manager->FillH1(0, energy_IntObj, weight);
                }
        }

        // Water Analysis
        // first time in detector determine incident water energies
        if(drawWaterIncDataFlag && !bremTest)
        {
                if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().compare(0, 5,"Water") == 0
                   && aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 5, "Water") != 0)
                {
                  if(isNRF)
                  {
                    manager->FillH1(1, theTrack->GetKineticEnergy()/(MeV), weight);
                  }
                  if(theTrack->GetKineticEnergy()/(MeV) < 1E-5)
                  {
                    manager->FillH1(2, theTrack->GetKineticEnergy()/(MeV), weight);
                  }
                  else{manager->FillH1(3, theTrack->GetKineticEnergy()/(MeV), weight);}
                }
        }

        // Here I am inside the water
        if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0,5,"Water")==0) {

                // only care about secondaries that occur in water volume
                const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();

                if(secondaries->size()>0) {
                        for(unsigned int i=0; i<secondaries->size(); ++i) {
                                if(secondaries->at(i)->GetParentID()>0) {
                                        if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
                                                if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Scintillation") {
                                                        G4double en = secondaries->at(i)->GetKineticEnergy();
                                                        run->AddScintillationEnergy(en);
                                                        run->AddScintillation();
                                                }
                                                if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Cerenkov") {
                                                        // for total run
                                                        G4double en = secondaries->at(i)->GetKineticEnergy();
                                                        run->AddCerenkovEnergy(en);
                                                        run->AddCerenkov();
                                                }
                                        }
                                }
                        }
                } // end of optical photons if statement

        } // end of if loop while inside water

        // PMT Analysis

        G4StepPoint* endPoint   = aStep->GetPostStepPoint();
        G4StepPoint* startPoint = aStep->GetPreStepPoint();

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
                // incident photocathode
                if(endPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")==0 && startPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")!=0) { // first time in photocathode
                        run->AddTotalSurface();
                        if(drawIncFlag && !bremTest)
                        {
                          if(theParticle->GetKineticEnergy()/(MeV) < 1E-5)
                          {
                            manager->FillH1(4, theParticle->GetKineticEnergy()/(MeV), weight);
                          }
                          else
                          {
                            manager->FillH1(5,theParticle->GetKineticEnergy()/(MeV),weight);
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
                                        else if (theStatus == Detection) {
                                                //run->AddDetection();
                                                procCount = "Det";
                                                det_energy = theParticle->GetKineticEnergy()/(MeV);
                                                manager->FillH1(6, det_energy, weight);
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
                                if(drawDetFlag && !bremTest)
                                {
                                        manager->FillNtupleSColumn(1,0,procCount);
                                        manager->AddNtupleRow(1);
                                }
                        } // for for loop
                } // for if statement if first time in photocathode
        } // for if at boundary
} // end of user steepping action function
