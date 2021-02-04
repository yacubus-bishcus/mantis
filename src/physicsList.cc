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

#include "physicsList.hh"


physicsList::physicsList(G4bool addNRF_in, G4bool use_xsec_tables_in, G4bool use_xsec_integration_in, G4bool force_isotropic_in, G4bool standalone_in, G4bool verbose_in)
        : addNRF(addNRF_in),
        use_xsec_tables(use_xsec_tables_in),
        use_xsec_integration(use_xsec_integration_in),
        force_isotropic(force_isotropic_in),
        standalone(standalone_in),
        NRF_Verbose(verbose_in)
        {
        defaultCutValue = 0.05*mm;
        cutForElectron = defaultCutValue;
        cutForPositron = defaultCutValue;
        cutForProton = defaultCutValue;

        G4HadronicProcessStore::Instance()->SetVerbose(0);

        ConstructPhysics();
}


physicsList::~physicsList()
{
}

void physicsList::ConstructPhysics() {
        G4DecayPhysics *theDecayPhysics = new G4DecayPhysics();
        theDecayPhysics->ConstructParticle();
        RegisterPhysics(theDecayPhysics);

        // Add OpticalPhysics to physicsList
        G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics(0);
        opticalPhysics->SetWLSTimeProfile("delta");
        opticalPhysics->SetScintillationYieldFactor(1.0);
        opticalPhysics->SetScintillationExcitationRatio(0.0);
        G4int maxNumber = 500;
        opticalPhysics->GetCerenkovProcess()->SetMaxNumPhotonsPerStep(maxNumber);
        opticalPhysics->SetMaxBetaChangePerStep(10.0);
        opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true);
        opticalPhysics->SetTrackSecondariesFirst(kScintillation, true);
        RegisterPhysics(opticalPhysics);

        // Add NRF to the physicsList
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


void physicsList::ConstructParticle() {
        G4Gamma::Definition();
        G4Electron::Definition();
        G4Positron::Definition();
}


void physicsList::ConstructProcess() {
        G4VModularPhysicsList::ConstructProcess();
}


void physicsList::SetCuts() {
        //SetCutValue(cutForGamma, "gamma");
        SetCutValue(cutForElectron, "e-");
        SetCutValue(cutForPositron, "e+");
        SetCutValue(cutForProton, "proton");
}
