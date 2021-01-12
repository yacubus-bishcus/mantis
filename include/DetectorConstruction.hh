#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
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
#include "G4Sphere.hh" //for sphere
#include "G4Tubs.hh" // for tube/cylinder

// for color attributes
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "DetectorMessenger.hh"
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
DetectorConstruction(G4bool);
virtual ~DetectorConstruction();


virtual G4VPhysicalVolume* Construct();

void SetAttenuatorState(G4bool val)
{
  attenuatorState = val;
}
void SetAttenuatorThickness(G4double val)
{
   attenThickness = val;
}
void SetAttenuatorMaterial(G4String val)
{
  attenuatorMat = val;
}
void SetAttenuatorState2(G4bool val)
{
  attenuatorState2 = val;
}
void SetAttenuatorThickness2(G4double val)
{
   attenThickness2 = val;
}
void SetAttenuatorMaterial2(G4String val)
{
  attenuatorMat2 = val;
}
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
{
        PMT_rmax = val;
        PMT_rmax = PMT_rmax*cm;
};
void SetIntObj_radius(G4double val)
{
        IntObj_rad = val;
        IntObj_rad = IntObj_rad*cm;
}
void SetIntObjX_pos(G4double val)
{
        intObj_x_pos = val;
        intObj_x_pos = intObj_x_pos*cm;
}
void SetIntObjY_pos(G4double val)
{
        intObj_y_pos = val;
        intObj_y_pos = intObj_y_pos*cm;
}
void SetIntObjZ_pos(G4double val)
{
        intObj_z_pos = val;
        intObj_z_pos = intObj_z_pos*cm;
}
void SetIntObjAbundance(G4double val)
{
        intObj_radio_abundance = val;
}
void SetChopperAbundance(G4double val)
{
  chopper_radio_abundance = val;
}
void SetIntObj(G4String val)
{
        if(val == "Uranium")
        {
                IntObj_Selection = val;
                intObjDensity = 19.1*g/cm3;
        }
        else if(val == "Plutonium")
        {
                IntObj_Selection = val;
                intObjDensity = 19.6*g/cm3;
        }
        else if(val == "NaturalU")
        {
          IntObj_Selection = "Natural Uranium";
          intObjDensity = 19.1*g/cm3;
        }
        else if(val == "NaturalPu")
        {
          IntObj_Selection = "Natural Plutonium";
          intObjDensity = 19.6*g/cm3;
        }
        else if(val == "Lead")
        {
          IntObj_Selection = "Lead";
          intObjDensity = 11.4*g/cm3;
        }
        else if(val == "Steel")
        {
          IntObj_Selection = "Steel";
          intObjDensity = 8.05*g/cm3;
        }
        else if(val == "Plastic")
        {
          IntObj_Selection = "Plastic";
          intObjDensity = 0.94*g/cm3;
        }
        else{std::cerr << "ERROR: IntObj not correctly chosen"<<std::endl;}
}

void SetChopperMaterial(G4String val)
{
  if(val == "Uranium")
  {
    chopperDensity = 19.1*g/cm3;
  }
  else if(val == "Plutonium")
  {
    chopperDensity = 19.6*g/cm3;
  }
}

void setEndIntObj(G4double z_pos_con, G4double con_z_size)
{
        EndIntObj = z_pos_con + con_z_size/2;
}

G4double getEndIntObj()const
{
        return EndIntObj;
}
void SetPC_material(G4String val)
{
        pc_mat = val;
}
void SetnPMT(G4int val)
{
        nPMT = val;
}
void SetChopperOn(G4bool val)
{
        chopperOn = val;
}
G4bool GetChopperState()const
{
        return chopperOn;
}
void SetChopperThick(G4double val)
{
        chopper_thick = val;
        chopper_thick = chopper_thick*mm;
}
void SetChopper_z(G4double val)
{
        chopper_z = val;
        chopper_z = chopper_z*cm;
}
void SettheAngle(G4double val)
{
        theAngle = val;
}
void SetPlexiThickness(G4double val)
{
  plexiThickness = val;
  plexiThickness = plexiThickness*mm;
}
void SetTapeThickness(G4double val)
{
  tapeThick = val;
  tapeThick = tapeThick*cm;
}
void SetDetectorViewOnly(G4bool val)
{
  DetectorViewOnly = val;
}
void SetMaterialVerbosity(G4bool val)
{
  material_verbose = val;
}

private:
G4bool bremTest;

// Chopper Properties 
G4double chopperDensity, chopper_thick, chopper_z;
G4bool chopperOn;
G4double chopper_U235_abundance, chopper_U238_abundance, chopper_Pu239_abundance, chopper_Pu240_abundance;

// Interrogation Object Properties 
G4double EndIntObj, IntObj_rad, intObjDensity, intObj_x_pos, intObj_y_pos, intObj_z_pos;
G4String IntObj_Selection;
G4VPhysicalVolume* physIntObj;
G4double intObj_U235_abundance, intObj_U238_abundance, intObj_Pu239_abundance, intObj_Pu240_abundance;

G4double chopper_radio_abundance, intObj_radio_abundance;

// Material, Logical and Physical Volumes 
G4Material* PC_mat;
G4LogicalVolume *logicalLinac;
G4LogicalVolume *logicalVacuum;
G4LogicalVolume *logicBremTarget;
G4LogicalVolume *logicBremTargetBacking;

G4LogicalVolume* logicPC;
G4LogicalVolume* logicPMT;
G4VPhysicalVolume* physPC;
G4LogicalVolume* logicChopper;
G4VPhysicalVolume* physWater;
G4VPhysicalVolume* physTape;

// Detector Properties 

// Attenuator Properties 
G4bool attenuatorState, attenuatorState2;
G4double attenThickness, attenThickness2;
G4String attenuatorMat, attenuatorMat2;

// Water Tank Properties 
G4double theAngle, water_size_x, water_size_y, water_size_z;

// Plexi/Tape Properties 
G4double plexiThickness, tapeThick;

// PMT Properties 
G4double PMT_rmax;
G4int nPMT;
G4String pc_mat;

// Output Properties 
G4bool DetectorViewOnly, material_verbose;

// Messenger 
DetectorMessenger* detectorM;

};


#endif
