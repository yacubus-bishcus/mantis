#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "PMTSD.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
//private:
//    G4VPhysicalVolume *World;
  
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    //G4VPhysicalVolume* GetDet() const {return physWater;}
    
    virtual G4VPhysicalVolume* Construct();
        //return physWorld;
    
    virtual void ConstructSDandField();
    std::vector<G4ThreeVector> PmtPositions;

 
private:
    G4LogicalVolume* logicPC;
    G4LogicalVolume* logicPMT;
    G4VPhysicalVolume* physPC;
    G4VPhysicalVolume* physPMT;
    G4VPhysicalVolume* physWater;
};


#endif

