#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4Types.hh"

class DetectorConstruction;


class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(const DetectorConstruction*, G4bool, G4bool);
    virtual ~ActionInitialization();

    virtual void Build() const;

private:
    const DetectorConstruction* fDetector;
    G4bool bremTest;
    G4bool output;
};

#endif
