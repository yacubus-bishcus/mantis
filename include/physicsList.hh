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

#ifndef physicsList_hh
#define physicsList_hh 1

#include "G4VModularPhysicsList.hh"


class physicsList: public G4VModularPhysicsList {
 public:
  physicsList(G4bool, G4bool, G4bool, G4bool, G4bool, G4bool);
  ~physicsList();

  void ConstructParticle();
  void ConstructPhysics();
  void ConstructProcess();
  void SetCuts();

 private:
  G4double cutForGamma, cutForElectron, cutForPositron, cutForProton;
  G4bool addNRF, use_xsec_tables, use_xsec_integration, force_isotropic, standalone, NRF_Verbose;
};

#endif
