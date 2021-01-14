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
