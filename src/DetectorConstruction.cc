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

#include "DetectorConstruction.hh"

extern G4bool bremTest;

DetectorConstruction::DetectorConstruction()
        : G4VUserDetectorConstruction(), // chopper properties
        chopperDensity(19.1*g/cm3), chopper_thick(30*mm), chopper_z(2*cm), chopperOn(false), // interrogation object properties
        IntObj_rad(4.5*cm), intObjDensity(19.1*g/cm3), intObj_x_pos(0*cm), intObj_y_pos(0*cm), intObj_z_pos(0*cm), IntObj_Selection("Uranium"), // radio abundances
        chopper_radio_abundance(0), intObj_radio_abundance(0), // Attenuator Properties
        attenuatorState(false), attenuatorState2(false), attenThickness(0.1*mm), attenThickness2(0.1*mm), attenuatorMat("G4_AIR"), attenuatorMat2("G4_AIR"), // Water Tank properties
        theAngle(120.0), water_size_x(60*cm), water_size_y(2.5908*m), water_size_z(40*cm), // plexi/tape properties
        plexiThickness(0.18*mm), tapeThick(0.01*cm), // PMT Properties
        PMT_rmax(25.4*cm), nPMT(4), pc_mat("GaAsP"), // Output Properties
        DetectorViewOnly(false), material_verbose(false), checkOverlaps(true), // Messenger
        detectorM(NULL)
{
        detectorM = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{
        delete detectorM;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
// Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();

// ************************************* Set up Materials ***************************************** //

        G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
        G4Material *steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
        G4Material *poly = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

        // Materials For NIST Shielding 
        G4Material *tungsten = nist->FindOrBuildMaterial("G4_W");
        G4Material *lead = nist->FindOrBuildMaterial("G4_Pb");
        G4Material *gold = nist->FindOrBuildMaterial("G4_Au");
        G4Material *copper = nist->FindOrBuildMaterial("G4_Cu");
        G4Material *attenuator = nist->FindOrBuildMaterial(attenuatorMat);
        G4Material *low_z_attenuator = nist->FindOrBuildMaterial(attenuatorMat2);
        
        //G4Element *elPb = new G4Element("Lead", "Pb", 82, 207.2*g/mole);
        //G4Element *elN = new G4Element("Nitrogen", "N2", 7, 14.01*g/mole);
        G4Element *elO = new G4Element("Oxygen", "O", 8, 16.0*g/mole);
        //G4Element *elC = new G4Element("Carbon", "C", 6, 12.0*g/mole);
        G4Element *elH = new G4Element("Hydrogen", "H", 1, 1.00794*g/mole);
        // Need to have water built by user for raleigh scattering
        G4Material *Water = new G4Material("Water", 0.99802*g/cm3,2, kStateLiquid,273.15, 1); // Name, density, numComponents, state, Temperature, pressure
        Water->AddElement(elH, 1);
        Water->AddElement(elO, 2);
        G4Material *plexiglass = nist->FindOrBuildMaterial("G4_PLEXIGLASS");

        G4Material *myVacuum = new G4Material("Vacuum", 1.e-5*g/cm3, 1, kStateGas, 273.15, 2.e-2*bar);
        myVacuum->AddMaterial(air,1);
// technically PMT glass is a special borosilicate glass calle k-free glass
// but pyrex is close enough as a borosilicate glass
        G4Material* PMT_mat = nist->FindOrBuildMaterial("G4_Pyrex_Glass");

// Setting up weapons grade material Isotopes
        G4Isotope* Uranium235 = new G4Isotope("Uranium235", 92, 235, 235.04393*g/mole); // atomicnumber, number of nucleons, mass of mole
        G4Isotope* Uranium238 = new G4Isotope("Uranium238", 92, 238, 238.02891*g/mole);
        G4Isotope* Plutonium239 = new G4Isotope("Plutonium239",94, 239, 239.0521634*g/mole);
        G4Isotope* Plutonium240 = new G4Isotope("Plutonium240", 94, 240, 240.05381*g/mole);
// Setting up Chopper Isotopes 
        G4Isotope* Lead204 = new G4Isotope("Lead204", 82, 204, 203.973043*g/mole);
        G4Isotope* Lead206 = new G4Isotope("Lead206", 82, 206, 205.974465*g/mole);
        G4Isotope* Lead207 = new G4Isotope("Lead207", 82, 207, 206.975897*g/mole);
        G4Isotope* Lead208 = new G4Isotope("Lead208", 82, 208, 207.976652*g/mole);
        
        G4Isotope* Tungsten180 = new G4Isotope("Tung180", 74, 180, 179.9467*g/mole);
        G4Isotope* Tungsten182 = new G4Isotope("Tung182", 74, 182, 181.9482*g/mole);
        G4Isotope* Tungsten183 = new G4Isotope("Tung183", 74, 183, 182.9502*g/mole);
        G4Isotope* Tungsten184 = new G4Isotope("Tung182", 74, 184, 183.9509*g/mole);
        G4Isotope* Tungsten186 = new G4Isotope("Tung186", 74, 186, 185.9543*g/mole);
        
// Setting up Chopper Materials 
        G4Element* Uranium_chopper = new G4Element("Chopper_Uranium", "U", 2); // name, element symbol, #isotopes
        G4Element* Plutonium_chopper = new G4Element("Chopper_Plutonium","Pu",2);
        G4Element* Lead_chopper = new G4Element("Chopper_Lead","Pb",4);
        Lead_chopper->AddIsotope(Lead204, 1.4*perCent);
        Lead_chopper->AddIsotope(Lead206, 24.1*perCent);
        Lead_chopper->AddIsotope(Lead207, 22.1*perCent);
        Lead_chopper->AddIsotope(Lead208, 52.4*perCent);
        G4Element* Tungsten_chopper = new G4Element("Chopper_Tungsten","W",5);
        Tungsten_chopper->AddIsotope(Tungsten180, 0.000012*perCent);
        Tungsten_chopper->AddIsotope(Tungsten182, 26.50*perCent);
        Tungsten_chopper->AddIsotope(Tungsten183, 14.31*perCent);
        Tungsten_chopper->AddIsotope(Tungsten184, 30.759988*perCent);
        Tungsten_chopper->AddIsotope(Tungsten186, 28.43*perCent);
        
// Setting up Interrogation object materials 
        G4Element* Uranium_interrogation = new G4Element("Interrogation_Uranium","U",2);
        G4Element* Plutonium_interrogation = new G4Element("Interrogation_Plutonium","Pu",2);
        G4Material* natural_uranium = nist->FindOrBuildMaterial("G4_U");
        G4Material* natural_plutonium = nist->FindOrBuildMaterial("G4_Pu");

// Set up Photocathode materials
        G4Element* elGa = new G4Element("Gallium", "Ga", 31, 69.723*g/mole);
        G4Element* elAs = new G4Element("Arsenic", "As", 33, 74.9216*g/mole);
        G4Element* elP = new G4Element("Phosphorous", "P", 15, 30.973762*g/mole);
        G4Material* GaAsP = new G4Material("GaAsP", 4.138*g/cm3, 3, kStateSolid, 293*kelvin, 1*pow(10,-6)*pascal);
        GaAsP->AddElement(elGa, 1);
        GaAsP->AddElement(elAs, 1);
        GaAsP->AddElement(elP, 1);
        G4Element* elCs = new G4Element("Cesium", "Cs", 55, 132.90545*g/mole);
        G4Element* elK = new G4Element("Potassium", "K", 19, 39.0983*g/mole);
        G4Element* elSb = new G4Element("Antimony", "Sb", 51, 121.76*g/mole);
        G4Material * bialkali = new G4Material("Bialkali", 3.29*g/cm3, 3, kStateSolid, 293*kelvin, 1*pow(10,-6)*pascal);
        bialkali->AddElement(elCs,1);
        bialkali->AddElement(elK,2);
        bialkali->AddElement(elSb,1);

        G4double world_size_x = 5.*m;
        G4double world_size_z = 10.*m;

        G4Box* solidWorld = new G4Box("World", world_size_x, world_size_x, world_size_z);

        G4LogicalVolume* logicWorld =
                new G4LogicalVolume(solidWorld, //its solid
                                    air, //its material
                                    "World"); //its name

// Make Physical volume ** NEVER CHANGE THIS **
        G4VPhysicalVolume* physWorld =
                new G4PVPlacement(0, //no rotation
                                  G4ThreeVector(), //at (0,0,0)
                                  logicWorld, //its logical volume
                                  "World", //its name
                                  0, //its mother  volume
                                  false, //no boolean operation
                                  0, //copy number
                                  false); //overlaps checking

// ********************************************************** World and Materials Complete ************************************************************//
        // Parameters used later 
        G4double container_z_pos = 2.4384*m +water_size_x + 1.0*m;
        G4double container_edge_position = container_z_pos - 2.4384*m;
        G4double colimator_size = 50*cm;
        G4double col_position = 1.0*cm + container_z_pos - 2.4384*m - colimator_size; // should go 1cm past the container 
        G4double col_edge_position = col_position + colimator_size;
        G4double bremStartPos = 135*cm;
        G4double bremBacking_thickness = 100.0*mm;
        G4double beamStart = bremStartPos - bremBacking_thickness/2.0 - 0.1*cm;
        G4cout << "DetectorConstruction::Build -> Beam Should Start at " << beamStart/(cm) << " cm" << G4endl;
        
// *********************************************************** Set up Chopper Wheel ****************************************************************** //

        G4cout << G4endl << "Chopper Wheel Information" << G4endl;
        G4cout << "----------------------------------------------------------------------" << G4endl;
        G4double chopper_beginning_edge_position = (bremStartPos+ chopper_z + linac_size) - chopper_thick/2.;  
        G4double chopper_end_edge_position = (bremStartPos + chopper_z + linac_size) + chopper_thick/2.;
        setBeginChopper(chopper_beginning_edge_position);
        G4cout << "Chopper Beginning Edge Set to: " << chopper_beginning_edge_position/(cm) << " cm" << G4endl;
        G4cout << "Chopper End Edge Set to: " << chopper_end_edge_position/(cm) << " cm" << G4endl;
        setEndChop(chopper_end_edge_position);
        if(chopper_end_edge_position > container_edge_position)
        {
                G4cerr << "ERROR: Chopper wheel location should be behind cargo container, exiting." << G4endl;
                exit(100);
        }

        G4Tubs *solidChopper = new G4Tubs("Chop", 0*cm, 15*cm, chopper_thick/2, 0.*deg, 180.*deg);
        G4Material *chopperMat = new G4Material("chopperMaterial", chopperDensity, 1);
        G4cout << "The Chopper State was set to: " << chopperOn << G4endl;

        if(chopperDensity == 19.1*g/cm3)
        {
                if(chopper_radio_abundance <= 0.0)
                {
                        G4cerr << "Fatal Error: User Must input chopper isotope abundance as percentage > 0" << G4endl;
                        exit(100);
                }

                if(chopperOn)
                {
                        chopper_U235_abundance = chopper_radio_abundance;
                        chopper_U238_abundance = 100. - chopper_radio_abundance;
                }
                else
                {
                        chopper_U235_abundance = 100. - chopper_radio_abundance;
                        chopper_U238_abundance = chopper_radio_abundance;     
                }

                Uranium_chopper->AddIsotope(Uranium235, chopper_U235_abundance*perCent);
                Uranium_chopper->AddIsotope(Uranium238, chopper_U238_abundance*perCent);
                chopperMat->AddElement(Uranium_chopper,1);
                G4cout << "The Chopper material selected was: Uranium" << G4endl;
                G4cout << "The Chopper fission isotope abundance was set to: " << chopper_radio_abundance << " %" << G4endl;
        }
        else if(chopperDensity == 19.6*g/cm3)
        {
                if(chopper_radio_abundance <= 0.0)
                {
                        G4cerr << "Fatal Error: User Must input chopper isotope abundance as percentage > 0" << G4endl;
                        exit(100);
                }

                if(chopperOn)
                {
                        chopper_Pu239_abundance = chopper_radio_abundance;
                        chopper_Pu240_abundance = 100. - chopper_radio_abundance;      
                }
                else
                {
                        chopper_Pu239_abundance = 100. - chopper_radio_abundance;
                        chopper_Pu240_abundance = chopper_radio_abundance;       
                }

                Plutonium_chopper->AddIsotope(Plutonium239, chopper_Pu239_abundance*perCent);
                Plutonium_chopper->AddIsotope(Plutonium240, chopper_Pu240_abundance*perCent);
                chopperMat->AddElement(Plutonium_chopper, 1);
                G4cout << "The Chopper material selected was: Plutonium" << G4endl;
                G4cout << "The Chopper fission isotope abundance was set to: " << chopper_radio_abundance << " %" << G4endl;
        }
        else if(chopperDensity == 11.34*g/cm3)
        {
                chopperMat->AddElement(Lead_chopper,1);
                G4cout << "The Chopper material selected was: Lead" << G4endl;
        }
        else if(chopperDensity == 19.3*g/cm3)
        {
                chopperMat->AddElement(Tungsten_chopper,1);
                G4cout << "The Chopper material selected was: Tungsten" << G4endl;
        }
        else{G4cerr << "ERROR Chopper Density not found!" << G4endl; exit(100);}

        G4cout << "The Chopper material density selected was: " << chopperDensity/(g/cm3) << " g/cm3" << G4endl;


        G4cout << "The Chopper thickness was: " << chopper_thick/(mm) << " mm" << G4endl;
        G4cout << "The Chopper center distance from the source was set as: " << (linac_size + chopper_z)/(cm) << " cm" << G4endl;

        logicChopper = new G4LogicalVolume(solidChopper, chopperMat, "Chop");

        new G4PVPlacement(0, G4ThreeVector(0, -7*cm,bremStartPos + chopper_z + linac_size),
                        logicChopper, "Chop", logicWorld, false,
                        0, checkOverlaps);
        
// Set up Linac configuration if Brem Test 
        
        if(bremTest)
        {
                G4Tubs *solidLinac = new G4Tubs("Linac",0, 7*cm, linac_size, 0*deg, 360*deg);
                logicalLinac = new G4LogicalVolume(solidLinac, tungsten, "Linac");
                new G4PVPlacement(0, G4ThreeVector(0,0, bremStartPos), logicalLinac, "Linac", logicWorld, false, 0, checkOverlaps);
                G4Tubs *solidVacuum = new G4Tubs("Vacuum", 0, 6*cm, linac_size, 0*deg, 360*deg);
                logicalVacuum = new G4LogicalVolume(solidVacuum, myVacuum, "Vacuum");
                new G4PVPlacement(0, G4ThreeVector(0,0,0), logicalVacuum, "Vac", logicalLinac, false,0,checkOverlaps);
// Make Brem target
                
// Brem Backing
                G4Box *solidBremTargetBacking = new G4Box("BremBacking", 20.1*mm/2.0, bremBacking_thickness/2.0, bremBacking_thickness/2.0);
                logicBremTargetBacking = new G4LogicalVolume(solidBremTargetBacking, copper, "BremBacking");
                new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicBremTargetBacking, "BremBacking", logicalVacuum, false, 0, checkOverlaps);
                
// Brem Radiator 
                G4double bremTarget_thickness = 0.102*mm;
                G4double brem_target_position = -(bremBacking_thickness/2.0) + bremTarget_thickness/2.0;
                G4Tubs *solidBremTarget = new G4Tubs("Brem", 0.*mm, 5.0*mm, bremTarget_thickness/2.0,0.*degree,360.*degree);
                logicBremTarget = new G4LogicalVolume(solidBremTarget, gold, "Brem");
                new G4PVPlacement(0, G4ThreeVector(0, 0, brem_target_position),logicBremTarget,"Brem", logicBremTargetBacking, false, 0, checkOverlaps);
                
                G4double brem_target_edge_position = bremStartPos + brem_target_position + bremTarget_thickness/2.0;
                G4cout << "Brem Target Beginning Edge Position: " << brem_target_edge_position/(cm) << " cm" << G4endl << G4endl;
                if(brem_target_edge_position/(cm) < beamStart/(cm))
                {
                        G4cerr << "DetectorConstruction::Build::291 -> FATAL ERROR DURING BREM TEST: Beam Started downstream of Brem Radiator!" << G4endl << G4endl;
                        exit(1);
                }
        }
        //else
        //{

// ***************************************** End of Brem Test Materials ***************************************** //

        // Set up Collimator
                G4double rearCol_Z_pos = bremStartPos - linac_size - 1.0*cm;
                G4Box *solidCollimator = new G4Box("Collimator", 1*cm, water_size_y, colimator_size);
                G4Box *solidCollimatorRear = new G4Box("Collimator",0.6096*m - 2*cm, 2.5908*m, 1*cm);
                G4LogicalVolume *logicCollimator = new G4LogicalVolume(solidCollimator, lead, "Collimator");
                G4LogicalVolume *logicCollimatorRear = new G4LogicalVolume(solidCollimatorRear, lead, "Collimator");
                G4cout << G4endl << "Container and Collimator Information" << G4endl;
                G4cout << "----------------------------------------------------------------------" << G4endl;
                G4cout << "Edge of Colimator placement: " << col_edge_position/(cm) << " cm" << G4endl << G4endl;
                new G4PVPlacement(0, G4ThreeVector(-0.6096*m - 1*cm, 0, col_position),
                                  logicCollimator, "ColL-Pb", logicWorld,
                                  false, 0, checkOverlaps);
                new G4PVPlacement(0, G4ThreeVector(0.6096*m + 1*cm, 0, col_position),
                                  logicCollimator, "ColRi-Pb", logicWorld,
                                  false, 0, checkOverlaps);
                new G4PVPlacement(0, G4ThreeVector(0,0,rearCol_Z_pos),
                                  logicCollimatorRear, "ColRe-Pb", logicWorld,
                                  false, 0, checkOverlaps);
        // Set up shipping container environment (8ft wide and 8.5ft high)
                G4double c_thick = 0.1905*cm; // approx 0.075 inch thick
                G4Box *solidContainer = new G4Box("Container", 0.6096*m, 2.5908*m, 2.4384*m);
                G4cout << "Edge of Container Placement: " << container_edge_position/(cm) << " cm" << G4endl << G4endl;
                G4LogicalVolume *logicContainer = new G4LogicalVolume(solidContainer, steel, "Container");
                new G4PVPlacement(0,
                                  G4ThreeVector(0, 0, container_z_pos),
                                  logicContainer, "Con-Steel",logicWorld,
                                  false,0,checkOverlaps);

                if(col_edge_position < container_edge_position)
                {
                        G4cerr << "ERROR: Collimator does not cover to edge of Cargo Container!!!" << G4endl;
                        exit(1);
                }

        // make container hollow
                G4Box *hollowContainer = new G4Box("ContainerAir", 0.6096*m -c_thick, 2.5908*m -c_thick, 2.4384*m -c_thick);
                G4LogicalVolume *logicHollowC = new G4LogicalVolume(hollowContainer, air, "hollowContainer");
                new G4PVPlacement(0, G4ThreeVector(),
                                  logicHollowC, "Con-Air",logicContainer, false,0,checkOverlaps);

// *********************************************** End of Shipping Container and Collimator Geometries ************************************ //

// *********************************************** Set up Interrogation Object **************************************************** //

                G4Sphere* solidIntObj = new G4Sphere("InterogationObject", 0, IntObj_rad, 0, 2*pi, 0, pi);

                intObj_U235_abundance = intObj_radio_abundance;
                intObj_U238_abundance = 100. - intObj_radio_abundance;
                intObj_Pu239_abundance = intObj_radio_abundance;
                intObj_Pu240_abundance = 100. - intObj_radio_abundance;
                G4Material* intObjMat = new G4Material("IntObj", intObjDensity, 1);
                intObjMat->SetName(IntObj_Selection);
                G4cout << G4endl << "Interrogation Object Information" << G4endl;
                G4cout << "----------------------------------------------------------------------" << G4endl;
                if(IntObj_Selection == "Uranium")
                {
                        if(intObj_radio_abundance <= 0.0)
                        {
                                G4cerr << "Fatal Error: User must input interrogation object isotope abundance for Uranium 235" << G4endl;
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
                                G4cerr << "Fatal Error: User must input interrogation object isotope abundance for Plutonium 239" << G4endl;
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
                else{G4cerr << "ERROR: Interogation Material not found."<<G4endl;}

                G4cout << "The User's Interogation Object Material: "
                       << intObjMat->GetName() << G4endl;
                if(IntObj_Selection == "Uranium" || IntObj_Selection == "Plutonium")
                {
                        G4cout << "The User's Interrogation Object Abundance: " << intObj_radio_abundance << " %" << G4endl;
                }
                G4cout << "The User's Interrogation Object Radius: " << IntObj_rad/(cm) << " cm" << G4endl;
                G4cout << "The User's Interrogation Object Density: " << intObjDensity/(g/cm3) << " g/cm3" << G4endl;
                G4cout << "The User's Interrogation Object Location: (" << intObj_x_pos/(cm) << ", " << intObj_y_pos/(cm) << ", " << intObj_z_pos/(cm) << ")" << " cm" << G4endl;

                G4LogicalVolume* logicIntObj = new G4LogicalVolume(solidIntObj, intObjMat,"IntObj");
                G4cout << "Begin of Interrogation Object: " << container_z_pos/(cm) + intObj_z_pos/(cm) -  IntObj_rad/(cm) << " cm" << G4endl;
                setEndIntObj(container_z_pos, 2.4384*m);

                physIntObj = new G4PVPlacement(0,
                                               G4ThreeVector(intObj_x_pos, intObj_y_pos, intObj_z_pos),
                                               logicIntObj, "IntObj", logicHollowC, false,
                                               0, checkOverlaps);


// ******************************************************** Interrogation Object Setup Complete ******************************************************* //

// ******************************************************** Begin Detector Construction *************************************************************** //

        // First Attenuation Layer

                G4Box* solidAttenuator = new G4Box("Attenuator", water_size_x + attenThickness + attenThickness2, water_size_y + attenThickness + attenThickness2,
                                                   water_size_z + attenThickness + attenThickness2);
                G4LogicalVolume* logicAttenuator = new G4LogicalVolume(solidAttenuator, attenuator, "Attenuator");
                if(attenuatorState)
                {
                        G4cout << G4endl << "Attenuator Information" << G4endl;
                        G4cout << "----------------------------------------------------------------------" << G4endl;
                        G4cout << "Attenuator Thickness set to: " << attenThickness << " cm of " << attenuator->GetName() << G4endl;
                }
                else
                {
                        attenuatorState2 = false;
                        attenThickness2 = 0*cm;
                        G4cout<< "Second Attenuator Thickness automatically set to Off." << G4endl;
                }

                G4double water_z_pos = container_z_pos - 2.4384*m;
                G4double myangle = (180. - theAngle)*pi/180.;
                G4double water_x_pos = tan(myangle)*(container_z_pos + intObj_z_pos - water_z_pos);
                G4double detDistance = water_x_pos/sin(myangle) + water_size_z;
                G4RotationMatrix* waterRot = new G4RotationMatrix;
                waterRot->rotateY((180. - theAngle)*deg);
                G4RotationMatrix* waterRot2 = new G4RotationMatrix;
                waterRot2->rotateY((180. + theAngle)*deg);

                new G4PVPlacement(waterRot,
                                  G4ThreeVector(water_x_pos,0,water_z_pos), logicAttenuator,
                                  "1LayL", logicWorld, false, 0, checkOverlaps);
                new G4PVPlacement(waterRot2,
                                  G4ThreeVector(-1*water_x_pos,0,water_z_pos), logicAttenuator,
                                  "1LayR", logicWorld, false, 0, checkOverlaps);

        // Option to add second layer of low Z attenuation material

                G4Box* solidSecondAttenuator = new G4Box("LowZAttenuator", water_size_x + attenThickness2, water_size_y+attenThickness2,
                                                         water_size_z+attenThickness2);
                G4LogicalVolume* logicSecondAttenuator = new G4LogicalVolume(solidSecondAttenuator, low_z_attenuator, "LowZAttenuator");
                new G4PVPlacement(0,G4ThreeVector(0,0,0), logicSecondAttenuator, "2Lay", logicAttenuator, false, 0, checkOverlaps);
                if(attenuatorState2)
                {
                        G4cout << "Second Attenuator set to: " << attenThickness2 << " cm of " << low_z_attenuator->GetName() << G4endl;
                }

        // Make Water Casing (Plexiglass)

                G4Box* solidCasing = new G4Box("Plexiglass", water_size_x, water_size_y, water_size_z);
                G4LogicalVolume* logicCasing = new G4LogicalVolume(solidCasing, plexiglass, "Plexiglass");
                new G4PVPlacement(0,G4ThreeVector(0,0,0), logicCasing, "Pglass", logicSecondAttenuator, false, 0, checkOverlaps);
                G4cout << G4endl << "Water Tank Information" << G4endl;
                G4cout << "----------------------------------------------------------------------" << G4endl;
                if(plexiThickness != 0.18*mm)
                {
                        G4cout << "Plexiglass Thickness Changed to: " << plexiThickness << " mm" << G4endl;
                }
                else
                {
                        G4cout << G4endl << "Plexiglass Thickness set to default: " << plexiThickness << " mm" << G4endl;
                }
                if(tapeThick != 0.01*cm)
                {
                        G4cout << "Optical Tape Thickness Changed to: " << tapeThick << " cm" << G4endl;
                }
                else
                {
                        G4cout << "Optical Tape Wrap set to default: " << tapeThick << " cm" << G4endl;
                }

        // Make Teflon tape wrap

                G4VSolid* solidTape = new G4Box("Tape", water_size_x-plexiThickness, water_size_y-plexiThickness, water_size_z-plexiThickness);
                G4Material *teflonTape = nist->FindOrBuildMaterial("G4_TEFLON");
                G4LogicalVolume* logicTape = new G4LogicalVolume(solidTape, teflonTape, "Tape");
                physTape = new G4PVPlacement(0,G4ThreeVector(0,0,0), logicTape, "Tape", logicCasing, false, 0, checkOverlaps);

        // Tub of water

                G4cout << "The Water Tank X was set to: " << water_size_x/(cm)<< " cm" << G4endl;
                G4cout << "The Water Tank Y was set to: " << water_size_y/(cm)<< " cm" << G4endl;
                G4cout << "The Water Tank Z was set to: " << water_size_z/(cm) << " cm" << G4endl << G4endl;

                G4Box* solidWater = new G4Box("Water", water_size_x-plexiThickness-tapeThick,
                                              water_size_y-plexiThickness-tapeThick,
                                              water_size_z-plexiThickness-tapeThick);
                G4LogicalVolume* logicWater =
                        new G4LogicalVolume(solidWater, //its solid
                                            Water, //its material
                                            "Water"); //its name

                physWater = new G4PVPlacement(0, //no rotation
                                              G4ThreeVector(0,0,0),
                                              logicWater, //its logical volume
                                              "Water", //its name
                                              logicTape, //its mother logical volume
                                              false, //no boolean operation
                                              0, //copy number
                                              checkOverlaps); //overlaps checking

// **************************************************** End of Water Tank Construction Setup ********************************************************* //


// ************************************************************** Construct PMTs ********************************************************************* //

                G4cout << G4endl << "PC and PMT Information" << G4endl;
                G4cout << "----------------------------------------------------------------------" << G4endl;
                G4double PMT_rmin = 0*cm;
                G4cout << "The PC Radius was set to " << PMT_rmax/(cm) << " cm" << G4endl;
                G4double PMT_z = 7.62*cm; // 3 in PMT
                G4Tubs* solidPMT = new G4Tubs("PMT", PMT_rmin, PMT_rmax, PMT_z, 0*deg, 360*deg);
                logicPMT = new G4LogicalVolume(solidPMT, PMT_mat, "PMT");

                G4cout << "The Number of PMTs was set to: " << nPMT << G4endl;
                G4double PMT_y_pos;
                std::vector<G4double> PMT_y_posv;
                if(nPMT>1)
                {
                        if(PMT_rmax*2 > water_size_y/(nPMT+1))
                        {
                                G4cerr << "ERROR Too many PMTs to fit on Water Surface!" << G4endl;
                                exit(10);
                        }
                        for(G4int i=1; i<=nPMT; i++)
                        {
                                PMT_y_pos = 0. - water_size_y/2 + i*(water_size_y/(nPMT + 1));
                                PMT_y_posv.push_back(PMT_y_pos);
                                G4cout << "PMT Position " << i << " set to " << PMT_y_posv[i-1]/(cm)<< " cm" << G4endl;
                        }
                }
                else
                {
                        PMT_y_posv.push_back(0);
                }

                for(G4int k=1; k<=nPMT; k++)
                {
                        new G4PVPlacement(0,
                                          G4ThreeVector(0, PMT_y_posv[k-1], -water_size_x/2 + PMT_z - plexiThickness - tapeThick),
                                          logicPMT,
                                          "PMT",
                                          logicWater,
                                          false,
                                          k,
                                          checkOverlaps);
                }

        // **************************************************** Construct Photocathode ****************************************************** //

                G4double PC_z = 20*nm;
                G4cout << "The Photocathode material was set as: " << pc_mat << G4endl << G4endl;
                if(pc_mat == "GaAsP")
                {
                        PC_mat = GaAsP;
                }
                else if(pc_mat == "Bialkali")
                {
                        PC_mat = bialkali;
                }
                else exit(1);

                G4Tubs* solidPhotoCathode = new G4Tubs("PC", PMT_rmin, PMT_rmax, PC_z, 0*deg, 360.*deg);
                logicPC = new G4LogicalVolume(solidPhotoCathode, PC_mat, "PC");
                G4double PMT_window_thickness = 3*mm;
                physPC = new G4PVPlacement(0,
                                           G4ThreeVector(0,0,PMT_z-PMT_window_thickness),
                                           logicPC,
                                           "PC",
                                           logicPMT, // daughter of PMT logical
                                           false,
                                           0,
                                           checkOverlaps);

        // ****************************** Visualization Handler ******************************** //

                G4VisAttributes *yellow= new G4VisAttributes( G4Colour(255/255.,255/255.,51/255. ));
                G4VisAttributes *red= new G4VisAttributes( G4Colour(255/255., 0/255., 0/255. ));
                G4VisAttributes *blue= new G4VisAttributes( G4Colour(0/255., 0/255.,  255/255. ));
                G4VisAttributes *grayc= new G4VisAttributes( G4Colour(128/255., 128/255.,  128/255. ));
                G4VisAttributes *lightGray= new G4VisAttributes( G4Colour(178/255., 178/255.,  178/255. ));
                G4VisAttributes *green= new G4VisAttributes( G4Colour(0/255., 255/255.,  0/255. ));
                G4VisAttributes *black = new G4VisAttributes(G4Colour(0.,0.,0.));
                G4VisAttributes *magenta = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));

                yellow->SetForceWireframe(true);
                logicAttenuator->SetVisAttributes(yellow);
                magenta->SetForceWireframe(true);
                logicSecondAttenuator->SetVisAttributes(magenta);
                black->SetForceWireframe(true);
                logicTape->SetVisAttributes(black);
                lightGray->SetForceWireframe(true);
                logicCasing->SetVisAttributes(lightGray);
                blue->SetForceWireframe(true);
                logicWater->SetVisAttributes(blue);
                green->SetForceWireframe(true);
                logicPMT->SetVisAttributes(green);
                red->SetForceSolid(true);
                logicPC->SetVisAttributes(red);

        // Set Visual colors
                if(bremTest)
                {
                        logicalLinac->SetVisAttributes(lightGray);
                        grayc->SetForceWireframe(true);
                        logicalVacuum->SetVisAttributes(grayc);
                        logicBremTarget->SetVisAttributes(red);
                        logicBremTargetBacking->SetVisAttributes(blue);
                }
                if(DetectorViewOnly)
                {
                        logicIntObj->SetVisAttributes(G4VisAttributes::Invisible);
                        logicContainer->SetVisAttributes(G4VisAttributes::Invisible);
                        logicHollowC->SetVisAttributes(G4VisAttributes::Invisible);
                        logicCollimator->SetVisAttributes(G4VisAttributes::Invisible);
                        logicChopper->SetVisAttributes(G4VisAttributes::Invisible);
                }
                else
                {
                        logicIntObj->SetVisAttributes(red);
                        logicContainer->SetVisAttributes(black);
                        logicHollowC->SetVisAttributes(lightGray);
                        logicCollimator->SetVisAttributes(magenta);
                        logicChopper->SetVisAttributes(red);
                }


        //
        // ------------ Generate & Add Material Properties Table ------------
        //

        // ************************** WATER ********************************//

                G4double photonEnergyIOF[] =
                { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
                  2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
                  2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
                  2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
                  2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
                  3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
                  3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
                  3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV
                };
                const G4int nEntriesIOF = sizeof(photonEnergyIOF)/sizeof(G4double);

                G4double photonEnergyAbs[] =
                {1.72*eV, 1.746197183*eV, 1.771142857*eV, 1.796811594*eV,1.823235294*eV,
                 1.850447761*eV, 1.878484848*eV, 1.907384615*eV, 1.9371875*eV,
                 1.967936508*eV, 1.999677419*eV, 2.032459016*eV, 2.066333333*eV,
                 2.101355932*eV,2.137586207*eV, 2.175087719*eV, 2.213928571*eV, 2.254181818*eV,
                 2.295925926*eV, 2.339245283*eV, 2.384230769*eV, 2.430980392*eV, 2.4796*eV,
                 2.530204082*eV, 2.582916667*eV, 2.63787234*eV, 2.695217391*eV,
                 2.755111111*eV, 2.817727273*eV, 2.883255814*eV, 2.951904762*eV,
                 3.023902439*eV, 3.0995*eV, 3.178974359*eV, 3.262631579*eV,
                 3.542285714*eV, 4.132666667*eV, 4.508363636*eV, 4.9592*eV
                };
                const G4int nEntriesAbs = sizeof(photonEnergyAbs)/sizeof(G4double);

                // rayleigh scattering! -- added automatically if material name = Water

                G4double refractiveIndex1[] =
                { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
                  1.346,  1.3465, 1.347,  1.3475, 1.348,
                  1.3485, 1.3492, 1.35,   1.3505, 1.351,
                  1.3518, 1.3522, 1.3530, 1.3535, 1.354,
                  1.3545, 1.355,  1.3555, 1.356,  1.3568,
                  1.3572, 1.358,  1.3585, 1.359,  1.3595,
                  1.36,   1.3608
                };
                assert(sizeof(refractiveIndex1) == sizeof(photonEnergyIOF));

                G4double absorption[] =
                {0.812347685*m, 1.209189843*m, 1.602564103*m, 1.937984496*m, 2.150537634*m,
                 2.277904328*m, 2.43902439*m, 2.941176471*m, 3.217503218*m, 3.429355281*m,
                 3.629764065*m, 3.78214826*m, 4.496402878*m, 7.4019245*m, 11.16071429*m, 14.38848921*m,
                 16.15508885*m, 17.69911504*m, 21.09704641*m, 23.04147465*m, 24.44987775*m,
                 30.76923077*m, 49.01960784*m,66.66666667*m,78.74015748*m,94.33962264*m,
                 102.145046*m, 108.4598698*m, 157.480315*m, 202.020202*m, 220.2643172*m,
                 211.4164905*m, 150.8295626*m, 117.5088132*m, 87.95074758*m,
                 60.60606061*m,18.11594203*m,11.79245283*m,9.652509653*m
                };
                assert(sizeof(absorption) == sizeof(photonEnergyAbs));

                // wavelenths 650, 600, 550, 500, 450, 400, 360 (approx peak), 350, 340, 300, 250
                G4double photonEnergyScintillation[] =
                {
                        1.90744*eV, 2.0664*eV, 2.25425*eV, 2.4796*eV, 2.75520*eV,
                        3.0996*eV, 3.44400*eV, 3.542405*eV, 3.64659*eV, 4.132806*eV, 4.95936*eV
                };
                const G4int nEntriesScint = sizeof(photonEnergyScintillation)/sizeof(G4double);

                G4double scintilD[] =
                {
                        0.007, 0.016, 0.028, 0.045, 0.25, 0.5, 0.95,
                        0.5, 0.006, 0.001, 0.0001
                };

                assert(sizeof(scintilD) == sizeof(photonEnergyScintillation));

                G4MaterialPropertiesTable* waterMPT = new G4MaterialPropertiesTable();

                waterMPT->AddProperty("RINDEX", photonEnergyIOF, refractiveIndex1, nEntriesIOF)->SetSpline(true);
                waterMPT->AddProperty("ABSLENGTH",photonEnergyAbs, absorption, nEntriesAbs)->SetSpline(true);
                // Data from Onken et al "Time response of water-based liquid scintillator from X-ray excitation"
                // Assumption made that fast and slow components follow same distribution
                waterMPT->AddProperty("FASTCOMPONENT",photonEnergyScintillation, scintilD, nEntriesScint)->SetSpline(true); // Energy Distribution for prompt scintillation
                waterMPT->AddProperty("SLOWCOMPONENT",photonEnergyScintillation, scintilD, nEntriesScint)->SetSpline(true); // Energy distribution for decay scintillation

                // Data from Caravaca et al. "Characterization of water-based liquid scintillator for Cherenkov and scintillation separation"
                waterMPT->AddConstProperty("SCINTILLATIONYIELD",234.0/MeV); // Light Yield Constant Number of photons/MeV
                waterMPT->AddConstProperty("RESOLUTIONSCALE",1.0); // the normal distribution will follow a normal standard deviation width
                waterMPT->AddConstProperty("FASTTIMECONSTANT", 2.25*ns); // value of a 1% WbLS
                waterMPT->AddConstProperty("SLOWTIMECONSTANT",15.10*ns); // value of a 1% WbLS
                waterMPT->AddConstProperty("YIELDRATIO",0.96); // Ratio of fast/slow(delay) photons

                G4double energy_water[] = {
                        1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
                        1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
                        1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
                        1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
                        1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
                        2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
                        2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
                        2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
                        2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
                        2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
                        3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
                        3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
                        3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
                        4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
                        5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
                };

                const G4int numentries_water = sizeof(energy_water)/sizeof(G4double);

                G4double mie_water[] = {
                        167024.4*m, 158726.7*m, 150742  *m,
                        143062.5*m, 135680.2*m, 128587.4*m,
                        121776.3*m, 115239.5*m, 108969.5*m,
                        102958.8*m, 97200.35*m, 91686.86*m,
                        86411.33*m, 81366.79*m, 76546.42*m,
                        71943.46*m, 67551.29*m, 63363.36*m,
                        59373.25*m, 55574.61*m, 51961.24*m,
                        48527.00*m, 45265.87*m, 42171.94*m,
                        39239.39*m, 36462.50*m, 33835.68*m,
                        31353.41*m, 29010.30*m, 26801.03*m,
                        24720.42*m, 22763.36*m, 20924.88*m,
                        19200.07*m, 17584.16*m, 16072.45*m,
                        14660.38*m, 13343.46*m, 12117.33*m,
                        10977.70*m, 9920.416*m, 8941.407*m,
                        8036.711*m, 7202.470*m, 6434.927*m,
                        5730.429*m, 5085.425*m, 4496.467*m,
                        3960.210*m, 3473.413*m, 3032.937*m,
                        2635.746*m, 2278.907*m, 1959.588*m,
                        1675.064*m, 1422.710*m, 1200.004*m,
                        1004.528*m, 833.9666*m, 686.1063*m
                };

                assert(sizeof(mie_water) == sizeof(energy_water));

                G4double mie_water_const[3]={0.99,0.99,0.8};

                waterMPT->AddProperty("MIEHG",energy_water,mie_water,numentries_water)->SetSpline(true);
                waterMPT->AddConstProperty("MIEHG_FORWARD",mie_water_const[0]);
                waterMPT->AddConstProperty("MIEHG_BACKWARD",mie_water_const[1]);
                waterMPT->AddConstProperty("MIEHG_FORWARD_RATIO",mie_water_const[2]);

                Water->SetMaterialPropertiesTable(waterMPT);
        // Set the Birks Constant for the Water scintillator only works when using sensitive detector
        //Water->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

        // ********************** APPLY OPTICAL QUALITIES ********************** //

        /* ************************************************************************* */

        // Water Tank Properties

                G4MaterialPropertiesTable* casingMPT = new G4MaterialPropertiesTable();
                G4MaterialPropertiesTable* casingOPMPT = new G4MaterialPropertiesTable();
        // add all of the properties here for plexiglass
                G4double ephotonPlexi[] = {1.1808067*eV,1.2336786*eV,1.298269*eV,1.3699967*eV,1.4501135*eV,1.5401826*eV,1.6421815*eV,1.7586482*eV,1.8928962*eV,2.049334*eV,
                                           2.2339586*eV,2.4551426*eV, 2.7249385*eV,2.850223*eV,3.0*eV,4.0*eV};
                const G4int numPlexi = sizeof(ephotonPlexi)/sizeof(G4double);
                G4double plexiReflectivity[] = {0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02}; // transparent
                G4double plexiTrans[] = {.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98};
                G4double plexiIOF[] = {1.4813,1.482,1.4827,1.4832,1.4837,1.4842,1.485,1.4862,1.488,1.4904,1.4933,1.4962,1.4996,1.5027,1.5027,1.5027};
                G4double plexiEff[] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
                G4double plexiAbs[] = {1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,30.0*m,30.0*m,11.4*m,11.4*m,7.3*m,7.3*m, 5.0*m, 1.0*mm}; // pretty much transparent until UV energy
                assert(sizeof(plexiIOF) == sizeof(ephotonPlexi));
                assert(sizeof(plexiReflectivity) == sizeof(ephotonPlexi));
                assert(sizeof(plexiTrans) == sizeof(ephotonPlexi));
                assert(sizeof(plexiEff) == sizeof(ephotonPlexi));
                assert(sizeof(plexiAbs) == sizeof(ephotonPlexi));
                G4OpticalSurface *casing_opsurf = new G4OpticalSurface("casingSurface", glisur, polished, dielectric_dielectric);
                casingOPMPT->AddProperty("REFLECTIVITY", ephotonPlexi, plexiReflectivity, numPlexi);
                casingOPMPT->AddProperty("TRANSMITTANCE", ephotonPlexi, plexiTrans, numPlexi);
                casingMPT->AddProperty("RINDEX", ephotonPlexi, plexiIOF, numPlexi);
                casingOPMPT->AddProperty("RINDEX", ephotonPlexi, plexiIOF, numPlexi);
                casingOPMPT->AddProperty("EFFICIENCY", ephotonPlexi, plexiEff, numPlexi);
                casingMPT->AddProperty("ABSLENGTH", ephotonPlexi, plexiAbs, numPlexi);
                casingOPMPT->AddProperty("ABSLENGTH", ephotonPlexi, plexiAbs, numPlexi);
                plexiglass->SetMaterialPropertiesTable(casingMPT);
                casing_opsurf->SetMaterialPropertiesTable(casingOPMPT);
                new G4LogicalSkinSurface("casing_surf", logicCasing, casing_opsurf);

        // Add properties for Teflon Tape

                G4MaterialPropertiesTable* tapeMPT = new G4MaterialPropertiesTable();
                G4MaterialPropertiesTable* tapeOPMPT = new G4MaterialPropertiesTable();
                G4double ephotonTape[] = {1.1427161*eV,1.2915073*eV,1.4586435*eV,1.6984206*eV,2.0160114*eV,2.5303*eV,3.262755*eV, 4.0*eV};
                const G4int numTape = sizeof(ephotonTape)/sizeof(G4double);
                G4double tapeReflectivity[] = {.98,.98,.98,.98,.98,.98,.98,.98};
                G4double tapeTrans[] = {0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02};
                G4double tapeEff[] = {0.,0.,0.,0.,0.,0.,0.,0.};
                G4double tapeIOF[] = {1.32,1.32,1.32,1.32,1.32,1.32,1.33,1.33};
                G4double tapeAbs[] = {1000*m, 1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m}; // setting absorption length arbitrarily large, don't want teflon to kill opt photons
                assert(sizeof(tapeReflectivity) == sizeof(ephotonTape));
                assert(sizeof(tapeEff) == sizeof(ephotonTape));
                assert(sizeof(tapeIOF) == sizeof(ephotonTape));
                assert(sizeof(tapeAbs) == sizeof(ephotonTape));
                assert(sizeof(tapeTrans) == sizeof(ephotonTape));
                G4OpticalSurface *tape_opsurf = new G4OpticalSurface("tapeSurface",glisur, polishedfrontpainted, dielectric_dielectric);
                tapeOPMPT->AddProperty("REFLECTIVITY", ephotonTape, tapeReflectivity, numTape);
                tapeOPMPT->AddProperty("TRANSMITTANCE", ephotonTape, tapeTrans, numTape);
                tapeMPT->AddProperty("RINDEX", ephotonTape, tapeIOF, numTape);
                tapeOPMPT->AddProperty("EFFICIENCY", ephotonTape, tapeEff, numTape);
                tapeMPT->AddProperty("ABSLENGTH", ephotonTape, tapeAbs, numTape);
                tape_opsurf->SetMaterialPropertiesTable(tapeOPMPT);
                teflonTape->SetMaterialPropertiesTable(tapeMPT);
                new G4LogicalBorderSurface("tape_surf", physWater, physTape, tape_opsurf);

        // *************************** Detector (PMT and PC) ***************************//

        // PMT
        // wavelengths (nm) reversed order 280, 300, 400, 450, 500, 750, 1000, 1500, 2000, 2500
                G4double ephotonPMT[] = {0.4959388*eV, 0.6199*eV, 0.8265*eV, 1.239*eV, 1.653*eV, 2.480*eV,
                                         2.755*eV, 3.10*eV, 3.54*eV, 4.133*eV};
                const G4int num = sizeof(ephotonPMT)/sizeof(G4double);
        // Wavelengths (nm) reversed order 200, 250, 280, 300, 350, 400, 450, 500, 550, 600, 650, 700
                G4double ephotonPMT2[] = {1.7711*eV, 1.9074*eV, 2.0663*eV, 2.2542*eV, 2.4796*eV,
                                          2.7551*eV, 3.0995*eV, 3.5423*eV, 4.133*eV, 4.428*eV, 4.959*eV, 6.199*eV};
                const G4int num2 = sizeof(ephotonPMT2)/sizeof(G4double);
                G4double ephotonPMT3[] = {1.823*eV, 1.8644316*eV, 1.907457*eV, 1.952515*eV,
                                          2.0*eV, 2.049*eV, 2.101*eV, 2.156*eV, 2.2140125*eV, 2.2749*eV, 2.339*eV,
                                          2.407*eV, 2.480*eV,2.56*eV,2.6379*eV, 2.725*eV, 2.817*eV, 2.917*eV,
                                          3.024017*eV, 3.138853*eV, 3.262755*eV};
                const G4int num3 = sizeof(ephotonPMT3)/sizeof(G4double);


        // PMT Surface Properties
                G4double reflectivity[] = {0.03822, 0.0392,  0.040,  0.041,
                                           0.0415,  0.0428,  0.0433,  0.0440, 0.0451, 0.0469 };
                G4double transmittance[] = {0.962, 0.961, 0.96, 0.959, 0.959, 0.958, 0.957,0.956,0.955, 0.954};
                assert(sizeof(reflectivity) == sizeof(ephotonPMT));
                assert(sizeof(transmittance) == sizeof(ephotonPMT2));
                G4double efficiency[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
                assert(sizeof(efficiency) == sizeof(ephotonPMT));
                G4double pmt_iof[] = {1.486, 1.4945, 1.5013, 1.5075, 1.5118, 1.5214, 1.5253, 1.5308, 1.5392, 1.5528};
                assert(sizeof(pmt_iof) == sizeof(ephotonPMT));
                G4double abslength[] = {1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m, 1000*m}; // treating glass as transparent
                assert(sizeof(abslength) == sizeof(ephotonPMT));
                G4OpticalSurface* PMT_opsurf = new G4OpticalSurface("PMTSurface",unified,polished,dielectric_metal);
                G4MaterialPropertiesTable* PMTopt = new G4MaterialPropertiesTable();
                G4MaterialPropertiesTable* PMTphysabs = new G4MaterialPropertiesTable();
                PMTopt->AddProperty("REFLECTIVITY", ephotonPMT, reflectivity, num)->SetSpline(true);
                PMTopt->AddProperty("TRANSMITTANCE", ephotonPMT2, transmittance, num2);
                PMTopt->AddProperty("EFFICIENCY", ephotonPMT, efficiency, num);
                PMTopt->AddProperty("RINDEX", ephotonPMT, pmt_iof, num)->SetSpline(true);
                PMTphysabs->AddProperty("RINDEX", ephotonPMT, pmt_iof, num)->SetSpline(true);
                PMTphysabs->AddProperty("ABSLENGTH", ephotonPMT, abslength, num);
                PMT_opsurf->SetMaterialPropertiesTable(PMTopt);
                PMT_mat->SetMaterialPropertiesTable(PMTphysabs);

        // Photocathode surface properties
                G4double Ga_As_photocath_EFF[]={0.25*perCent,36.246*perCent,39.8*perCent,40.0*perCent,36.0*perCent,30.0*perCent,
                                                24.0*perCent, 15.0*perCent,4.8*perCent, 0.6*perCent, 0.25*perCent, 0.10*perCent}; //Enables 'detection' of photons
                assert(sizeof(Ga_As_photocath_EFF) == sizeof(ephotonPMT2));
                G4double Ga_As_photocath_ReR[]={3.346,3.3588,3.3877,3.4857,3.6209,4.0919,4.6678,4.6102,3.8200, 3.7322};
                assert(sizeof(Ga_As_photocath_ReR) == sizeof(ephotonPMT));
                G4double Ga_As_photocath_ImR[]={0.0056,0.0071,0.0096,0.01523, 0.106, 0.41247, 1.0094, 2.0488, 2.0051, 1.9908};
                assert(sizeof(Ga_As_photocath_ImR) == sizeof(ephotonPMT));
                G4double bialkali_photocath_EFF[] = {0.015*perCent, 0.2*perCent, 3.0*perCent, 7.0*perCent, 18.5*perCent, 25.0*perCent,
                                                     28.0*perCent, 26.0*perCent, 6.0*perCent, 0.19*perCent, 0.1*perCent, 0.01*perCent};
                assert(sizeof(bialkali_photocath_EFF) == sizeof(ephotonPMT2));
                G4double bialkali_photocath_ReR[] = {2.96,2.95,2.95,2.95,2.96, 2.98, 3.01, 3.06,
                                                     3.12, 3.20, 3.26, 3.09, 3.0,3.0,3.0,2.87,2.7,2.61,2.38,2.18,1.92};
                assert(sizeof(bialkali_photocath_ReR) == sizeof(ephotonPMT3));
                G4double bialkali_photocath_ImR[] = {0.33,0.34,0.34,0.35,0.37,0.38,0.42,0.46,
                                                     0.53,0.63,0.86,1.05,1.06,1.11,1.34,1.44,1.50,1.53,1.71,1.69,1.69};
                assert(sizeof(bialkali_photocath_ImR) == sizeof(ephotonPMT3));
                G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
                if(pc_mat == "GaAsP")
                {
                        photocath_mt->AddProperty("EFFICIENCY", ephotonPMT2,Ga_As_photocath_EFF, num2)->SetSpline(true);
                        photocath_mt->AddProperty("REALRINDEX", ephotonPMT,Ga_As_photocath_ReR,num)->SetSpline(true);
                        photocath_mt->AddProperty("IMAGINARYRINDEX", ephotonPMT, Ga_As_photocath_ImR, num)->SetSpline(true);
                }
                else if(pc_mat == "Bialkali")
                {
                        photocath_mt->AddProperty("EFFICIENCY", ephotonPMT2,bialkali_photocath_EFF, num)->SetSpline(true);
                        photocath_mt->AddProperty("REALRINDEX", ephotonPMT3,bialkali_photocath_ReR,num3)->SetSpline(true);
                        photocath_mt->AddProperty("IMAGINARYRINDEX", ephotonPMT3, bialkali_photocath_ImR, num3)->SetSpline(true);
                }

                G4OpticalSurface* photocath_opsurf= new G4OpticalSurface("photocath_opsurf");
                photocath_opsurf->SetType(dielectric_metal);
                photocath_opsurf->SetFinish(polished);
                photocath_opsurf->SetPolish(1.0);
                photocath_opsurf->SetModel(glisur);
                photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

                new G4LogicalSkinSurface("photocath_surf", logicPC, photocath_opsurf); // name, physical volume of surface, phsical volume of world?, G4optical surface
                new G4LogicalSkinSurface("PMT_surf", logicPMT, PMT_opsurf);
        // Air
                G4double refractiveIndex2[] =
                {
                        1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                        1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                        1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                        1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                        1.00, 1.00, 1.00, 1.00
                };

                G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
                airMPT->AddProperty("RINDEX", photonEnergyIOF, refractiveIndex2, nEntriesIOF);

                air->SetMaterialPropertiesTable(airMPT);

        // Material Verbosity to print materials properties tables
                if(material_verbose)
                {
                        G4cout << "Material Verbose set to True/On!" << G4endl;
                        G4cout << "Material Properties Table for: " << air->GetName() << G4endl;
                        airMPT->DumpTable();
                        G4cout << "Material Properties Table for: " << teflonTape->GetName() << G4endl;
                        tapeOPMPT->DumpTable();
                        G4cout << "Material Properties Table for: " << pc_mat << G4endl;
                        photocath_mt->DumpTable();
                        G4cout << "Material Properties Table for: " << Water->GetName() << G4endl;
                        waterMPT->DumpTable();
                        G4cout << "Material Properties Table for: " << plexiglass->GetName() << G4endl;
                        casingOPMPT->DumpTable();
                }
       // } // for if !bremTest

//always return the physical World!!!
        G4cout << "DetectorConstruction::Construct -> Constructed!" << G4endl << G4endl;
        return physWorld;
}
/* ************************************************************************************ */
