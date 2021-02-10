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

#include "PhysicsListOld.hh"

PhysicsListOld::PhysicsListOld(G4bool addNRF_in, G4bool use_xsec_tables_in,
                         G4bool use_xsec_integration_in, G4bool force_isotropic_in, 
                         G4bool standalone_in, G4bool verbose_in)
                        : addNRF(addNRF_in), use_xsec_tables(use_xsec_tables_in),
                          use_xsec_integration(use_xsec_integration_in),
                          force_isotropic(force_isotropic_in),
                          standalone(standalone_in),
                          NRF_Verbose(verbose_in)
{
  G4HadronicProcessStore::Instance()->SetVerbose(0);
  ConstructPhysics();
}


PhysicsListOld::~PhysicsListOld()
{
}

void PhysicsListOld::ConstructPhysics()
{
  G4DecayPhysics *theDecayPhysics = new G4DecayPhysics();
  theDecayPhysics->ConstructParticle();
  RegisterPhysics(theDecayPhysics);

  // Add OpticalPhysics to PhysicsListOld
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics(0);
  opticalPhysics->SetWLSTimeProfile("delta");
  opticalPhysics->SetScintillationYieldFactor(1.0); // this would change if the yield changed based on particle type --> not relevant here
  opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true);
  opticalPhysics->SetTrackSecondariesFirst(kScintillation, true);
  G4cout << "Geant4 10.5 Optical Physics Set." << G4endl;

  RegisterPhysics(opticalPhysics);

  // Add NRF to the PhysicsListOld
  if(addNRF)
  {
    RegisterPhysics(new G4NRFPhysics("NRF", use_xsec_tables, use_xsec_integration, force_isotropic, standalone, NRF_Verbose));
    G4cout << "\nAdded NRF to the physicsList.\n" << G4endl;
  }

  // Add the rest of the usual suspects
  RegisterPhysics(new G4EmStandardPhysics_option4(0));
  RegisterPhysics(new G4EmExtraPhysics(0));

  // Ion stopping-in-matter physics
  RegisterPhysics( new G4StoppingPhysics(0) );

  // Neutron tracking cuts for optimized simulation
  G4NeutronTrackingCut *theNeutronTrackingCut = new G4NeutronTrackingCut(0);
  theNeutronTrackingCut->SetTimeLimit(10*microsecond);
  theNeutronTrackingCut->SetKineticEnergyLimit(0.01*eV);
  RegisterPhysics( theNeutronTrackingCut );
}


void PhysicsListOld::ConstructParticle()
{
  G4Gamma::Definition();
  G4Electron::Definition();
  G4Positron::Definition();
}


void PhysicsListOld::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
}


void PhysicsListOld::SetCuts()
{
  SetCutValue(0.05*mm, "e-");
  SetCutValue(0.05*mm, "e+");
  SetCutValue(0.05*mm, "proton");
}
