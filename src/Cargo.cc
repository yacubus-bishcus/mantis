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

#include "Cargo.hh"

Cargo::Cargo()
:
RemoveContainer(false),
IntObj_rad(4.5*cm), intObjDensity(19.1*g/cm3), IntObj_Selection("Uranium"),
intObj_radio_abundance(0),
cargoM(NULL)
{
  cargoM = new CargoMessenger(this);
}

Cargo::~Cargo()
{
  delete cargoM;
}

void Cargo::Construct(G4LogicalVolume* logicWorld, double container_z_pos, double container_edge_position, double chopper_end_edge_position, bool checkOverlaps)
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material *steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material *poly = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material *lead = nist->FindOrBuildMaterial("G4_Pb");
  // Setting up weapons grade material Isotopes
  G4Isotope* Uranium235 = new G4Isotope("Uranium235", 92, 235, 235.04393*g/mole); // atomicnumber, number of nucleons, mass of mole
  G4Isotope* Uranium238 = new G4Isotope("Uranium238", 92, 238, 238.02891*g/mole);
  G4Isotope* Plutonium239 = new G4Isotope("Plutonium239",94, 239, 239.0521634*g/mole);
  G4Isotope* Plutonium240 = new G4Isotope("Plutonium240", 94, 240, 240.05381*g/mole);

  // Setting up Interrogation object materials
  G4Element* Uranium_interrogation = new G4Element("Interrogation_Uranium","U",2);
  G4Element* Plutonium_interrogation = new G4Element("Interrogation_Plutonium","Pu",2);
  G4Material* natural_uranium = nist->FindOrBuildMaterial("G4_U");
  G4Material* natural_plutonium = nist->FindOrBuildMaterial("G4_Pu");

  // Set up shipping container environment (8ft wide and 8.5ft high)
  G4double c_thick = 0.1905*cm; // approx 0.075 inch thick
  G4Box *solidContainer = new G4Box("Container", 0.3048*m, 1.2954*m, 1.2192*m);
  G4cout << "Cargo::Construct -> Edge of Container Placement: " << container_edge_position/(cm) << " cm" << G4endl << G4endl;
  G4LogicalVolume *logicContainer = new G4LogicalVolume(solidContainer, steel, "Container");

  if(!RemoveContainer)
  {
    G4cout << "Cargo::Construct -> Building Container!" << G4endl;
    new G4PVPlacement(0,
                    G4ThreeVector(0, 0, container_z_pos),
                    logicContainer, "Con-Steel",logicWorld,
                    false,0,checkOverlaps);
  }

  // make container hollow
  G4Box *hollowContainer = new G4Box("ContainerAir", 0.3048*m -c_thick, 1.2954*m -c_thick, 1.2192*m -c_thick);
  G4LogicalVolume *logicHollowC = new G4LogicalVolume(hollowContainer, air, "hollowContainer");

  if(!RemoveContainer)
  {
    G4cout << "Cargo::Construct -> Container Built!" << G4endl;
    new G4PVPlacement(0, G4ThreeVector(),
                      logicHollowC, "Con-Air",logicContainer, false,
                      0,checkOverlaps);
  }

  // Set Up the Interrogation Object
    G4Sphere* solidIntObj = new G4Sphere("InterogationObject", 0, IntObj_rad, 0, 2*pi, 0, pi);

    intObj_U235_abundance = intObj_radio_abundance;
    intObj_U238_abundance = 100. - intObj_radio_abundance;
    intObj_Pu239_abundance = intObj_radio_abundance;
    intObj_Pu240_abundance = 100. - intObj_radio_abundance;
    G4Material* intObjMat = new G4Material("IntObj", intObjDensity, 1);
    intObjMat->SetName(IntObj_Selection);
    G4cout << G4endl << "Cargo::Construct -> Interrogation Object Information" << G4endl;
    G4cout << "----------------------------------------------------------------------" << G4endl;
    if(IntObj_Selection == "Uranium")
    {
      if(intObj_radio_abundance <= 0.0)
      {
        G4cerr << "Fatal Error: Cargo::Construct -> User must input interrogation object isotope abundance for Uranium 235" << G4endl;
        exit(100);
      }
      Uranium_interrogation->AddIsotope(Uranium235, intObj_U235_abundance*perCent);
      Uranium_interrogation->AddIsotope(Uranium238, intObj_U238_abundance*perCent);
      intObjMat->AddElement(Uranium_interrogation,1);
    }
    else if(IntObj_Selection == "Plutonium")
    {
      if(intObj_radio_abundance <= 0.0)
      {
        G4cerr << "Fatal Error: Cargo::Construct -> User must input interrogation object isotope abundance for Plutonium 239" << G4endl;
        exit(100);
      }
      Plutonium_interrogation->AddIsotope(Plutonium239, intObj_Pu239_abundance*perCent);
      Plutonium_interrogation->AddIsotope(Plutonium240, intObj_Pu240_abundance*perCent);
      intObjMat->AddElement(Plutonium_interrogation,1);
    }
    else if(IntObj_Selection == "Natural Uranium")
    {
            intObjMat->AddMaterial(natural_uranium,1);
    }
    else if(IntObj_Selection == "Natural Plutonium")
    {
            intObjMat->AddMaterial(natural_plutonium,1);
    }
    else if(IntObj_Selection == "Lead")
    {
            intObjMat->AddMaterial(lead,1);
    }
    else if(IntObj_Selection == "Steel")
    {
            intObjMat->AddMaterial(steel,1);
    }
    else if(IntObj_Selection == "Plastic")
    {
            intObjMat->AddMaterial(poly,1);
    }
    else{G4cerr << "ERROR: Cargo::Construct -> Interogation Material not found."<<G4endl;}

    G4cout << "Cargo::Construct -> Interogation Object Material: "
           << intObjMat->GetName() << G4endl;
    if(IntObj_Selection == "Uranium" || IntObj_Selection == "Plutonium")
    {
            G4cout << "Cargo::Construct -> Interrogation Object Abundance: " << intObj_radio_abundance << " %" << G4endl;
    }
    G4cout << "Cargo::Construct -> Interrogation Object Radius: " << IntObj_rad/(cm) << " cm" << G4endl;
    G4cout << "Cargo::Construct -> Interrogation Object Density: " << intObjDensity/(g/cm3) << " g/cm3" << G4endl;

    G4LogicalVolume* logicIntObj = new G4LogicalVolume(solidIntObj, intObjMat,"IntObj");
    G4cout << "Cargo::Construct -> Begin of Interrogation Object: " << container_z_pos/(cm) + 0/(cm) -  IntObj_rad/(cm) << " cm" << G4endl;
    setEndIntObj(container_z_pos, 1.2192*m);

    if(!RemoveContainer)
    {
      new G4PVPlacement(0,
                         G4ThreeVector(0, 0, 0),
                         logicIntObj, "IntObj", logicHollowC, false,
                         0, checkOverlaps);
    }
    else
    {
      new G4PVPlacement(0,
                        G4ThreeVector(0,0,chopper_end_edge_position+1.0*m),
                        logicIntObj, "IntObj",logicWorld, false, 0, checkOverlaps);
    }
}

void Cargo::AddCargo()
{
  G4cout << "Cargo::AddCargo -> Adding Cargo." << G4endl;
}