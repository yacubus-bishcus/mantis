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

#ifndef Cargo_h
#define Cargo_h 1

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
#include "G4Tubs.hh" // for tube/cylinder
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4NistManager.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "CargoMessenger.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class CargoMessenger;

class Cargo
{
public:
Cargo();
~Cargo();

void Construct(G4LogicalVolume*, double, double, double, bool);
void AddCargo();

void SetIntObj_radius(G4double val)
{
        IntObj_rad = val;
        IntObj_rad = IntObj_rad*cm;
}
void SetIntObjAbundance(G4double val)
{
        intObj_radio_abundance = val;
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
  else{G4cerr << "ERROR: Cargo -> IntObj not correctly chosen" << G4endl;}
}

void setEndIntObj(G4double z_pos_con, G4double con_z_size)
{
  EndIntObj = z_pos_con + con_z_size/2;
  G4cout << "Z-Cut set to: " << EndIntObj/(cm) << " cm" << G4endl << G4endl;
}

G4double getEndIntObj()const
{
  return EndIntObj;
}

void SetRemoveContainer(G4bool val)
{
  RemoveContainer=val;
  G4cout << "Cargo:: WARNING Container has been removed!" << G4endl;
}

private:
  G4bool RemoveContainer;
  G4double IntObj_rad, intObjDensity;
  G4String IntObj_Selection;
  G4double intObj_radio_abundance;
  G4double intObj_U235_abundance, intObj_U238_abundance, intObj_Pu239_abundance, intObj_Pu240_abundance;
  CargoMessenger* cargoM;
  G4double EndIntObj;

};

#endif
