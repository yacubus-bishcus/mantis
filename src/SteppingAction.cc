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
extern G4bool bremTest;
extern G4bool debug;

SteppingAction::SteppingAction(const DetectorConstruction* det, RunAction* run, EventAction* event)
        : G4UserSteppingAction(), kdet(det), krun(run), kevent(event),
        drawChopperIncDataFlag(0), drawChopperOutDataFlag(0), drawNRFDataFlag(0),
        drawIntObjDataFlag(0), drawWaterIncDataFlag(0), drawCherenkovDataFlag(0), drawDetDataFlag(0),
        stepM(NULL)
{
        stepM = new StepMessenger(this);
        fExpectedNextStatus = Undefined;
}

SteppingAction::~SteppingAction()
{
        delete stepM;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    if(debug)
        std::cout << "SteppingAction::UserSteppingAction --> Beginning" << std::endl;

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

    // Grab Relevant event information including the particle weight
    eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
    weight = info->GetWeight();
    G4double beamEnergy = info->GetBeamEnergy();

    // throw a warning if energy conservation appears to be broken for gammas
    // small deviations have been observed; attribute to a likely mismatch in database energies
    if (theTrack->GetKineticEnergy() > beamEnergy)
    {
      G4cerr << G4endl;
      G4cerr << "Warning: gammaEnergy " << theTrack->GetKineticEnergy() << " MeV > beamEnergy " << beamEnergy << " MeV!!" << G4endl;
      G4cerr << "  energy difference (keV) = " << (theTrack->GetKineticEnergy()-beamEnergy)/keV << G4endl;
      G4cerr << "  creator process = " << theTrack->GetCreatorProcess()->GetProcessName() << G4endl;
      G4cerr << "  material = " << theTrack->GetMaterial()->GetName() << G4endl;

      // if the discrepancy is > 1.0 keV, throw an actual error
      if (theTrack->GetKineticEnergy()-beamEnergy > 1.0*keV) {
        G4cerr << "  energy difference > 1.0 keV. Aborting..." << G4endl;
        exit(111);
      } else {
        G4cerr << "  energy difference < 1.0 keV likely due to NRF database mismatch. Carrying on..." << G4endl;
      }
    }
    G4String nextStep_VolumeName = endPoint->GetPhysicalVolume()->GetName();
    G4String previousStep_VolumeName = startPoint->GetPhysicalVolume()->GetName();
    // kill photons past IntObj
    G4double EndIntObj = kdet->getEndIntObj();

    // Run Cuts
    if(bremTest)
    {
        G4double EndChop = kdet->getEndChop();
        //G4ThreeVector pB = aStep->GetPreStepPoint()->GetMomentum();
        //G4double tB = asin(sqrt(pow(pB.x(),2)+pow(pB.y(),2))/pB.mag()); //the angle of the particle relative to the Z axis
        if(theTrack->GetPosition().z() > EndChop)
        {
          theTrack->SetTrackStatus(fStopAndKill);
          krun->AddStatusKilled();
        }
    }

    if(theTrack->GetPosition().z() > EndIntObj)
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
    G4AnalysisManager* manager = G4AnalysisManager::Instance();

    G4String CPName = "beam";
    // Check if Track is created by NRF
    if(theTrack->GetCreatorProcess() !=0)
    {
      CPName = theTrack->GetCreatorProcess()->GetProcessName();
      if(CPName == "NRF")
      {
        isNRF = 1;
      }
    }

// *********************************** Track Bremsstrahlung Beam for Brem Test ***************************************** //

    if(bremTest)
    {
      if(nextStep_VolumeName.compare(0,11,"BremBacking") != 0
          && previousStep_VolumeName.compare(0,11,"BremBacking") == 0
          && theTrack->GetParticleDefinition() == G4Gamma::Definition())
      {
        G4ThreeVector p = aStep->GetPreStepPoint()->GetMomentum();
        G4double angle = asin(sqrt(pow(p.x(),2)+pow(p.y(),2))/p.mag()); //the angle of the particle relative to the Z axis
        if(cos(angle) < 0.94 || CPName != "eBrem")
        {
          theTrack->SetTrackStatus(fStopAndKill);
          krun->AddStatusKilled();
        }
        else
        {
          manager->FillNtupleDColumn(0,0,theTrack->GetKineticEnergy()/(MeV));
          manager->FillNtupleDColumn(0,1, angle);
          manager->AddNtupleRow(0);
        }
      }
    }
// **************************************************** Track NRF Materials **************************************************** //

    const G4VProcess* process = endPoint->GetProcessDefinedStep();
    // Keep track of Any NRF Created
    if(drawNRFDataFlag)
    {
      if(process->GetProcessName() == "NRF")
      {
        krun->AddNRF();
        manager->FillNtupleIColumn(3,0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        manager->FillNtupleDColumn(3,1,theTrack->GetTotalEnergy()/(MeV));
        manager->FillNtupleDColumn(3,2,weight);
        manager->FillNtupleSColumn(3,3,endPoint->GetPhysicalVolume()->GetName());
        G4ThreeVector NRF_loc = theTrack->GetPosition();
        manager->FillNtupleDColumn(3,4, NRF_loc.z()/(cm));
        manager->AddNtupleRow(3);
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
        manager->FillNtupleDColumn(1,0, theTrack->GetKineticEnergy()/(MeV)); // not weighting chopper
        manager->FillNtupleDColumn(1,1, weight);
        manager->FillNtupleIColumn(1,2,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        manager->AddNtupleRow(1);
      }
    }
    if(drawChopperOutDataFlag)
    {
      // Gammas Exiting Chopper Wheel
      if(nextStep_VolumeName.compare(0,4,"Chop") != 0
         && previousStep_VolumeName.compare(0,4,"Chop") == 0)
      {
        manager->FillNtupleDColumn(2,0, theTrack->GetKineticEnergy()/(MeV));
        manager->FillNtupleDColumn(2,1, weight);
        manager->FillNtupleIColumn(2,2,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        manager->FillNtupleIColumn(2,3,isNRF);
        manager->AddNtupleRow(2);
      }
    }

// *********************************************** Track Interrogation Object Interactions **************************************************** //

        // Interrogation Object Analysis
        if(drawIntObjDataFlag && !bremTest)
        {
          G4ThreeVector p = aStep->GetPreStepPoint()->GetMomentum();
          G4double angle = asin(sqrt(pow(p.x(),2)+pow(p.y(),2))/p.mag()); //the angle of the particle relative to the Z axis
          // Incident Air Pocket Just Prior to Interrogation Object
          if(nextStep_VolumeName.compare(0,9,"AirPocket") == 0
            && previousStep_VolumeName.compare(0,9,"AirPocket") !=0)
          {
            manager->FillNtupleDColumn(4,0,theTrack->GetKineticEnergy()/(MeV));
            manager->FillNtupleDColumn(4,1,weight);
            manager->FillNtupleSColumn(4,2,CPName);
            manager->FillNtupleDColumn(4,3,angle);
            manager->FillNtupleDColumn(4,4,theTrack->GetGlobalTime());
            manager->FillNtupleIColumn(4,5, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
            manager->AddNtupleRow(4);
          }
                // Incident Interrogation Object
          if(nextStep_VolumeName.compare(0, 6,"IntObj") == 0
             && previousStep_VolumeName.compare(0, 6, "IntObj") != 0)
          {
              manager->FillNtupleDColumn(5,0, theTrack->GetKineticEnergy()/(MeV));
              manager->FillNtupleDColumn(5,1, weight);
              manager->FillNtupleSColumn(5,2, CPName);
              manager->FillNtupleDColumn(5,3,angle);
              manager->FillNtupleDColumn(5,4,theTrack->GetGlobalTime());
              manager->FillNtupleIColumn(5,5,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
              manager->AddNtupleRow(5);
          }
          // Exiting Interrogation Object
          if(nextStep_VolumeName.compare(0, 6,"IntObj") != 0
             && previousStep_VolumeName.compare(0,6,"IntObj") == 0)
          {
            manager->FillNtupleDColumn(6,0, theTrack->GetKineticEnergy()/(MeV));
            manager->FillNtupleDColumn(6,1, weight);
            manager->FillNtupleSColumn(6,2, CPName);
            manager->AddNtupleRow(6);
          }
        }

// *********************************************** Track Water Tank Interactions **************************************************** //

        // Water Analysis
        // First time incident Water keep track of NRF hitting water
        // Be careful here it will keep track of reflections
        if(drawWaterIncDataFlag && !bremTest)
        {
          if(nextStep_VolumeName.compare(0, 5,"Water") == 0
             && previousStep_VolumeName.compare(0, 5, "Water") != 0)
          {
            manager->FillNtupleDColumn(7,0, theTrack->GetKineticEnergy()/(MeV));
            manager->FillNtupleDColumn(7,1, weight);
            manager->FillNtupleSColumn(7,2, CPName);
            manager->FillNtupleIColumn(7,3, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
            manager->FillNtupleIColumn(7,4, theTrack->GetTrackID());
            manager->AddNtupleRow(7);
          }
        }

// *********************************************** Track Cherenkov Interactions **************************************************** //

        // While in water keep track of cherenkov and pass number of cherenkov to EventAction
        if(startPoint->GetPhysicalVolume()->GetName().compare(0,5,"Water")==0)
        {
                // only care about secondaries that occur in water volume
          if(bremTest)
          {
            theTrack->SetTrackStatus(fStopAndKill); // kill track only intersted in incident chopper Data
            krun->AddStatusKilled();
          }

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
                    manager->FillNtupleIColumn(9,0,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
                    manager->FillNtupleDColumn(9,1, theParticle->GetKineticEnergy()/(MeV));
                    manager->FillNtupleDColumn(9,2, weight);
                    G4String creatorProcess;

                    if(theTrack->GetCreatorProcess() !=0)
                        creatorProcess = theTrack->GetCreatorProcess()->GetProcessName();
                    else
                        creatorProcess = "Brem";

                    manager->FillNtupleSColumn(9,3, creatorProcess);
                    manager->FillNtupleDColumn(9,4, theTrack->GetGlobalTime()); // time units is nanoseconds
                    manager->AddNtupleRow(9);
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
                  manager->FillNtupleIColumn(10,0,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
                  manager->FillNtupleDColumn(10,1, theParticle->GetKineticEnergy()/(MeV));
                  manager->FillNtupleDColumn(10,2, weight);
                  manager->FillNtupleSColumn(10,3, procCount);
                  manager->AddNtupleRow(10);
                } // for if keeping track of detector process data

              } // for if opProc
            } // for for loop
          } // for if statement if first time in photocathode
        } // for if at boundary
      if(debug)
        std::cout << "SteppingAction::UserSteppingAction()-> Ending!" <<std::endl;
} // end of user stepping action function
