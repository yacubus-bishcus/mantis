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

#include "SteppingAction.hh"

SteppingAction::SteppingAction(const DetectorConstruction* det, RunAction* run, EventAction* event, G4bool brem, G4bool weightHisto)
        : G4UserSteppingAction(), kdet(det), krun(run), kevent(event), bremTest(brem), WeightHisto(weightHisto),
        drawChopperIncDataFlag(0), drawChopperOutDataFlag(0), drawNRFDataFlag(0),
        drawIntObjDataFlag(0), drawWaterIncDataFlag(0), drawCherenkovDataFlag(0), drawDetDataFlag(0),
        stepM(NULL)
{
        //std::cout << "SteppingAction::SteppingAction" << std::endl;
        stepM = new StepMessenger(this);
        fExpectedNextStatus = Undefined;
}

SteppingAction::~SteppingAction()
{
        delete stepM;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
        //std::cout << "SteppingAction::UserSteppingAction --> Beginning" << std::endl;
        G4StepPoint* endPoint   = aStep->GetPostStepPoint();
        G4StepPoint* startPoint = aStep->GetPreStepPoint();
        G4Track* theTrack = aStep->GetTrack();

        // Run Logical Checks
        if(endPoint == NULL)
        {
                return; // at the end of the world
        }
        else if(endPoint->GetPhysicalVolume()==NULL)
        {
                return;
        }

        G4String nextStep_VolumeName = endPoint->GetPhysicalVolume()->GetName();
        G4String previousStep_VolumeName = startPoint->GetPhysicalVolume()->GetName();
        // kill photons past IntObj
        G4double EndIntObj = kdet->getEndIntObj();

        // Run Cuts
        if(theTrack->GetPosition().z()/(cm) > EndIntObj/(cm))
        {
                // kill photons that go beyond the interrogation object
                theTrack->SetTrackStatus(fStopAndKill);
                krun->AddStatusKilled();
        }
        else if(nextStep_VolumeName.compare(0, 3, "Col") == 0)
        {
                // kill photons in collimator
                theTrack->SetTrackStatus(fStopAndKill);
                krun->AddStatusKilled();
        }

// ************************************************* Checks and Cuts Complete ************************************************** //

        G4int isNRF = 0;
        // Grab Weights from PrimaryGenerator
        eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
        weight = info->GetWeight();
        G4AnalysisManager* manager = G4AnalysisManager::Instance();

// **************************************************** Track NRF Materials **************************************************** //

        const G4VProcess* process = endPoint->GetProcessDefinedStep();
        // Keep track of Any NRF Created
        if(drawNRFDataFlag)
        {
                if(process->GetProcessName() == "NRF")
                {
                        krun->AddNRF();
                        manager->FillNtupleIColumn(2,0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
                        manager->FillNtupleDColumn(2,1,theTrack->GetTotalEnergy()/(MeV));
                        manager->FillNtupleDColumn(2,2,weight);
                        manager->FillNtupleSColumn(2,3,endPoint->GetPhysicalVolume()->GetName());
                        G4ThreeVector NRF_loc = theTrack->GetPosition();
                        manager->FillNtupleDColumn(2,4, NRF_loc.z()/(cm));
                        manager->AddNtupleRow(2);
                        if(WeightHisto)
                        {
                                manager->FillH1(8, theTrack->GetKineticEnergy()/(MeV), weight);
                        }
                }
        }
        // Check if Track is created by NRF
        if(theTrack->GetCreatorProcess() !=0)
        {
                G4String CPName = theTrack->GetCreatorProcess()->GetProcessName();
                if(CPName == "NRF")
                {
                        isNRF = 1;
                }
        }

// *********************************************** Track Chopper Interactions **************************************************** //

        // Chopper Analysis
        if(drawChopperIncDataFlag)
        {
                // Gammas Incident Chopper Wheel
                if(nextStep_VolumeName.compare(0, 4,"Chop") == 0
                   && previousStep_VolumeName.compare(0, 4, "Chop") != 0
                   && theTrack->GetParticleDefinition() == G4Gamma::Definition())
                {
                        manager->FillNtupleDColumn(0,0, theTrack->GetKineticEnergy()/(MeV)); // not weighting chopper
                        manager->FillNtupleDColumn(0,1, weight);
                        manager->FillNtupleIColumn(0,2,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
                        manager->AddNtupleRow(0);
                        if(WeightHisto)
                        {
                                manager->FillH1(0, theTrack->GetKineticEnergy()/(MeV), weight);
                        }
                }
        }
        if(drawChopperOutDataFlag)
        {
                // Gammas Exiting Chopper Wheel
                if(nextStep_VolumeName.compare(0,4,"Chop") != 0
                   && previousStep_VolumeName.compare(0,4,"Chop") == 0)
                {
                        manager->FillNtupleDColumn(1,0, theTrack->GetKineticEnergy()/(MeV));
                        manager->FillNtupleDColumn(1,1, weight);
                        manager->FillNtupleIColumn(1,2,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
                        manager->FillNtupleIColumn(1,3,isNRF);
                        manager->AddNtupleRow(1);
                        if(WeightHisto)
                        {
                                manager->FillH1(1, theTrack->GetKineticEnergy()/(MeV), weight);
                        }
                }
        }
// *********************************************** Track Interrogation Object Interactions **************************************************** //

        // Interrogation Object Analysis
        if(drawIntObjDataFlag && !bremTest)
        {
                // Incident Interrogation Object
                if(nextStep_VolumeName.compare(0, 6,"IntObj") == 0
                   && previousStep_VolumeName.compare(0, 6, "IntObj") != 0)
                {
                        manager->FillH1(2, theTrack->GetKineticEnergy()/(MeV), weight);
                        // NRF Incident Interrogation Object
                        if(isNRF && drawNRFDataFlag)
                        {
                                manager->FillH1(3, theTrack->GetKineticEnergy()/(MeV), weight);
                        }
                }
                // Exiting Interrogation Object
                if(nextStep_VolumeName.compare(0, 6,"IntObj") != 0
                   && previousStep_VolumeName.compare(0,6,"IntObj") == 0)
                {
                        manager->FillH1(4, theTrack->GetKineticEnergy()/(MeV), weight);
                        // NRF Exiting Interrogation Object
                        if(isNRF && drawNRFDataFlag)
                        {
                                manager->FillH1(5, theTrack->GetKineticEnergy()/(MeV), weight);
                        }
                }
        }
// *********************************************** Track Water Tank Interactions **************************************************** //

        // Water Analysis
        // First time incident Water keep track of NRF hitting water
        if(drawWaterIncDataFlag && !bremTest)
        {
                if(nextStep_VolumeName.compare(0, 5,"Water") == 0
                   && previousStep_VolumeName.compare(0, 5, "Water") != 0)
                {
                        manager->FillH1(6, theTrack->GetKineticEnergy()/(MeV),weight);
                        // NRF Incident Water Tank
                        if(isNRF && drawNRFDataFlag)
                        {
                                manager->FillH1(7, theTrack->GetKineticEnergy()/(MeV), weight);
                        }
                }
        }
// *********************************************** Track Cherenkov Interactions **************************************************** //

        // While in water keep track of cherenkov and pass number of cherenkov to EventAction
        if(startPoint->GetPhysicalVolume()->GetName().compare(0,5,"Water")==0) {
                // only care about secondaries that occur in water volume
                const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();
                if(secondaries->size()>0)
                {
                        if(drawCherenkovDataFlag)
                        {
                                kevent->CherenkovEnergy(theTrack->GetKineticEnergy()/(MeV));
                                kevent->CherenkovSecondaries(secondaries->size());
                                kevent->CherenkovTime(theTrack->GetGlobalTime());
                        }

                        for(unsigned int i=0; i<secondaries->size(); ++i)
                        {
                                if(secondaries->at(i)->GetParentID()>0)
                                {
                                        if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
                                        {
                                                if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Scintillation")
                                                {
                                                        krun->AddScintillationEnergy(secondaries->at(i)->GetKineticEnergy());
                                                        krun->AddScintillation();
                                                }
                                                if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Cerenkov")
                                                {
                                                        // for total run
                                                        krun->AddCerenkovEnergy(secondaries->at(i)->GetKineticEnergy());
                                                        krun->AddCerenkov();
                                                }
                                        }
                                }
                        }
                } // end of optical photons if statement
        } // end of if loop while inside water

        // *********************************************** Track Photocathode Interactions **************************************************** //

        // Photocathode Analysis

        if(endPoint->GetStepStatus() == fGeomBoundary)
        {
                const G4DynamicParticle* theParticle = theTrack->GetDynamicParticle();
                G4OpBoundaryProcessStatus theStatus = Undefined;
                G4ProcessManager* OpManager =
                        G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
                G4int MAXofPostStepLoops =
                        OpManager->GetPostStepProcessVector()->entries();
                G4ProcessVector* postStepDoItVector =
                        OpManager->GetPostStepProcessVector(typeDoIt);
                // incident photocathode
                if(nextStep_VolumeName.compare(0,2,"PC")==0
                   && previousStep_VolumeName.compare(0,2,"PC")!=0)
                {
                        krun->AddTotalSurface();

                        for (G4int i=0; i<MAXofPostStepLoops; ++i)
                        {
                                G4VProcess* currentProcess = (*postStepDoItVector)[i];

                                G4OpBoundaryProcess* opProc = dynamic_cast<G4OpBoundaryProcess*>(currentProcess);

                                if(opProc && !bremTest)
                                {
                                        theStatus = opProc->GetStatus();

                                        if(theStatus == Transmission)
                                        {
                                                procCount = "Trans";
                                        }
                                        else if(theStatus == FresnelRefraction)
                                        {
                                                procCount = "Refr";
                                        }
                                        else if (theStatus == TotalInternalReflection)
                                        {
                                                procCount = "Int_Refl";
                                        }
                                        else if (theStatus == LambertianReflection)
                                        {
                                                procCount = "Lamb";
                                        }
                                        else if (theStatus == LobeReflection)
                                        {
                                                procCount = "Lobe";
                                        }
                                        else if (theStatus == SpikeReflection)
                                        {
                                                procCount = "Spike";
                                        }
                                        else if (theStatus == BackScattering)
                                        {
                                                procCount = "BackS";
                                        }
                                        else if (theStatus == Absorption)
                                        {
                                                procCount = "Abs";
                                        }
                                        // Keep track of detected photons
                                        else if (theStatus == Detection)
                                        {
                                                procCount = "Det";
                                                manager->FillNtupleIColumn(4,0,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
                                                manager->FillNtupleDColumn(4,1, theParticle->GetKineticEnergy()/(MeV));
                                                manager->FillNtupleDColumn(4,2, weight);
                                                G4String creatorProcess;
                                                if(theTrack->GetCreatorProcess() !=0)
                                                        creatorProcess = theTrack->GetCreatorProcess()->GetProcessName();
                                                else
                                                        creatorProcess = "Brem";
                                                manager->FillNtupleSColumn(4,3, creatorProcess);
                                                manager->FillNtupleDColumn(4,4, theTrack->GetGlobalTime()); // time units is nanoseconds
                                                manager->AddNtupleRow(4);
                                                manager->FillH1(11, theParticle->GetKineticEnergy()/(MeV), weight);
                                        }
                                        else if (theStatus == NotAtBoundary)
                                        {
                                                procCount = "NotAtBoundary";
                                        }
                                        else if (theStatus == SameMaterial)
                                        {
                                                procCount = "SameMaterial";
                                        }
                                        else if (theStatus == StepTooSmall)
                                        {
                                                procCount = "SteptooSmall";
                                        }
                                        else if (theStatus == NoRINDEX)
                                        {
                                                procCount = "NoRINDEX";
                                        }
                                        else
                                        {
                                                procCount = "noStatus";
                                        }
                                        // Keep track of Detector Process Data
                                        if(drawDetDataFlag && !bremTest)
                                        {
                                                manager->FillNtupleIColumn(5,0,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
                                                manager->FillNtupleDColumn(5,1, theParticle->GetKineticEnergy()/(MeV));
                                                manager->FillNtupleDColumn(5,2, weight);
                                                manager->FillNtupleSColumn(5,3, procCount);
                                                manager->AddNtupleRow(5);
                                                if(WeightHisto)
                                                {
                                                        manager->FillH1(10,theParticle->GetKineticEnergy()/(MeV), weight);
                                                } // for if WeightHisto
                                        } // for if keeping track of detector process data

                                } // for if opProc
                        } // for for loop
                } // for if statement if first time in photocathode
        } // for if at boundary
          //std::cout << "SteppingAction::UserSteppingAction()-> Ending!" <<std::endl;
} // end of user stepping action function
