#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "PMTSD.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4OpRayleigh.hh"

#include "G4Box.hh" //for box
//#include "G4Cons.hh" //for cone
//#include "G4Orb.hh" //for orb
//#include "G4Sphere.hh" //for sphere
#include "G4Tubs.hh" // for tube/cylinder

// for color attributes
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "DetectorMessenger.hh"

#include "PMTSD.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class DetectorMessenger;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{

public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    //G4VPhysicalVolume* GetDet() const {return physWater;}

    virtual G4VPhysicalVolume* Construct();
        //return physWorld;

    virtual void ConstructSDandField();
    std::vector<G4ThreeVector> PmtPositions;
    void SetWaterX(G4double val)
    {
      water_size_x = val;
      water_size_x = water_size_x*cm;
    }
    void SetWaterY(G4double val)
    {
      water_size_y = val;
      water_size_y = water_size_y*cm;
    }
    void SetWaterZ(G4double val)
    {
      water_size_z = val;
      water_size_z = water_size_z*cm;
    }
    void SetPC_radius(G4double val)
    {PMT_rmax = val;
      PMT_rmax = PMT_rmax*cm;
    };
    void SetIntObjX(G4double val)
    {
      IntObj_x = val;
      IntObj_x = IntObj_x*cm;
    }
    void SetIntObjY(G4double val)
    {
      IntObj_y = val;
      IntObj_y = IntObj_y*cm;
    }
    void SetIntObjZ(G4double val)
    {
      IntObj_z = val;
      IntObj_z = IntObj_z*cm;
    }
    void SetAbundance(G4double val)
    {
      radio_abundance = val;
      radio_abundance = radio_abundance*perCent;
    }

    void SetIntObj(G4String val)
    {
      if(val == "Uranium")
      {
        IntObj_Selection = val;
        intObjDensity = 19.1;
      }
      else if(val == "Plutonium")
      {
        IntObj_Selection = val;
        intObjDensity = 19.6;
      }
      else{std::cerr << "ERROR: IntObj not correctly chosen"<<std::endl;}
    }

    void setEndIntObj(G4double z_Size, G4double z_pos)
    {
      EndIntObj = z_Size/2 + z_pos;
    }

    G4double getEndIntObj()const
    {
      return EndIntObj;
    }

private:
    G4double intObj_z_pos;
    G4double EndIntObj;
    G4double IntObj_x, IntObj_y, IntObj_z;
    G4double radio_abundance;
    G4String IntObj_Selection;
    G4double intObjDensity;
    G4VPhysicalVolume* physIntObj;
    G4LogicalVolume* logicPC;
    G4LogicalVolume* logicPMT;
    G4VPhysicalVolume* physPC;
    G4VPhysicalVolume* physPMT;
    G4VPhysicalVolume* physWater;
    G4double water_size_x, water_size_y, water_size_z, PMT_rmax;
    DetectorMessenger* detectorM;
};


#endif
