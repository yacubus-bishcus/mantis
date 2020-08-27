#include "DetectorConstruction.hh"
#include "PMTSD.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"

#include "G4Box.hh" //for box
//#include "G4Cons.hh" //for cone
//#include "G4Orb.hh" //for orb
//#include "G4Sphere.hh" //for sphere
#include "G4Tubs.hh" // for tube/cylinder

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
// for color attributes
#include "G4Colour.hh"
#include "G4VisAttributes.hh"



DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
    // Set World Dimensions
    G4double world_size_x = 150*cm;
    G4double world_size_y = 150*cm;
    G4double world_size_z = 175*cm;

    // Make Solid World

    G4Box* solidWorld = new G4Box("World", world_size_x, world_size_y, world_size_z);
    G4Material* air = nist->FindOrBuildMaterial("G4_AIR");

    // Make Logical Volume
    G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        air,           //its material
                        "World");            //its name

    // Make Physical volume ** NEVER CHANGE THIS **
    G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      false);                //overlaps checking

    // Tub of water

    G4double water_size_x = 100*cm;
    G4double water_size_y = 100*cm;
    G4double water_size_z = 50*cm;

    // Make Solid Water

    G4Box* solidWater = new G4Box("Water", water_size_x, water_size_y, water_size_z);
    G4Material* Water = nist->FindOrBuildMaterial("G4_WATER");

    // Make Logical Volume
    G4LogicalVolume* logicWater =
    new G4LogicalVolume(solidWater,          //its solid
                        Water,           //its material
                        "Water");            //its name

    // Make Physical volume
    G4double water_x_pos = 0*cm;
    G4double water_y_pos =0*cm;
    G4double water_z_pos =70*cm;

    physWater = new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(water_x_pos,water_y_pos,water_z_pos),
                      logicWater,            //its logical volume
                      "Water",               //its name
                      logicWorld,            //its mother logical volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

    // PMT Cylinder(Tube) face

    G4double PMT_rmin = 0*cm;
    G4double PMT_rmax = 10*2.54*cm; // made it 10 times larger for now
    G4double PMT_z = 6*2.54*cm;
    G4double PMT_start_phi = 0.*deg;
    G4double PMT_phi = 360.*deg;// may not work with pi

    // Make Solid PMT

    G4Tubs* solidPMT = new G4Tubs("PMT", PMT_rmin, PMT_rmax, PMT_z, PMT_start_phi, PMT_phi);
    G4Material* PMT_mat = nist->FindOrBuildMaterial("G4_Pyrex_Glass"); // technically PMT glass is a special borosilicate glass calle k-free glass but pyrex is close enough as a borosilicate glass

    // Make Logical PMT

    logicPMT = new G4LogicalVolume(solidPMT, PMT_mat, "PMT");
    // Set as Sensitive detector



    // Make Physical PMT

    G4double PMT_x_pos = 0*cm;
    G4double PMT_y_pos = 0*cm;
    G4double PMT_z_pos = (water_z_pos + water_size_z + PMT_z);
    PmtPositions.push_back(G4ThreeVector(PMT_x_pos,PMT_y_pos,PMT_z_pos));

   physPMT = new G4PVPlacement(0,
                                G4ThreeVector(PMT_x_pos,PMT_y_pos,PMT_z_pos),
                                logicPMT,
                                "PMT",
                                logicWorld,
                                false,
                                0,
                                checkOverlaps);

    // Make Solid PhotoCathode
    G4double PC_z = 1*cm;
    G4Tubs* solidPhotoCathode = new G4Tubs("PC", PMT_rmin, PMT_rmax, PC_z, PMT_start_phi, PMT_phi);
    G4Material* PC_mat = nist->FindOrBuildMaterial("G4_GALLIUM_ARSENIDE");

    //Make Logical Photocathode volume
    logicPC = new G4LogicalVolume(solidPhotoCathode, PC_mat, "PC");
    G4double PMT_window_thickness = 1; // cm
    // Make physical volume
    physPC = new G4PVPlacement(0,
                                G4ThreeVector(0,0,(-PMT_z +PMT_window_thickness*cm)), // now position is relation to mother volume(PMT)
                                logicPC,
                                "PC",
                                logicPMT, // daughter of PMT logical
                                false,
                                0,
                                checkOverlaps);



    // Visualization Attributes

    //logicWorld->SetVisAttributes (G4VisAttributes::GetInvisible()); // sets world invisible in visualization
    G4VisAttributes * yellow= new G4VisAttributes( G4Colour(255/255. ,255/255. ,51/255. ));
    G4VisAttributes * red= new G4VisAttributes( G4Colour(255/255. , 0/255. , 0/255. ));
    G4VisAttributes * blue= new G4VisAttributes( G4Colour(0/255. , 0/255. ,  255/255. ));
    G4VisAttributes * grayc= new G4VisAttributes( G4Colour(128/255. , 128/255. ,  128/255. ));
    G4VisAttributes * lightGray= new G4VisAttributes( G4Colour(178/255. , 178/255. ,  178/255. ));
    G4VisAttributes * green= new G4VisAttributes( G4Colour(0/255. , 255/255. ,  0/255. ));

    yellow->SetVisibility(true);
    yellow->SetForceSolid(true);
    red->SetVisibility(true);
    red->SetForceSolid(true);
    blue->SetVisibility(true);
    green->SetVisibility(true);
    green->SetForceSolid(true);
    grayc->SetVisibility(true);
    grayc->SetForceSolid(true);
    lightGray->SetVisibility(true);
    lightGray->SetForceSolid(true);

    // Set Visual colors

    logicWater->SetVisAttributes(yellow);
    logicPMT->SetVisAttributes(green);


    //
    // ------------ Generate & Add Material Properties Table ------------
    //
      G4double photonEnergy[] =
                { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
                  2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
                  2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
                  2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
                  2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
                  3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
                  3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
                  3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };

      const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

      G4double photonEnergy2[] =
      {
        1.00E-03*keV, 1.50E-03*keV, 2.00E-03*keV, 3.00E-03*keV, 4.00E-03*keV, 5.00E-03*keV,
6.00E-03*keV, 8.00E-03*keV, 1.00E-02*keV, 1.50E-02*keV, 2.00E-02*keV, 3.00E-02*keV, 4.00E-02*keV,
5.00E-02*keV, 6.00E-02*keV, 8.00E-02*keV, 1.00E-01*keV, 1.50E-01*keV, 2.00E-01*keV, 3.00E-01*keV, 4.00E-01*keV,
5.00E-01*keV, 6.00E-01*keV, 8.00E-01*keV, 1.00E+00*keV, 1.02E+00*keV, 1.25E+00*keV, 1.50E+00*keV,
2.00E+00*keV, 2.04E+00*keV, 3.00E+00*keV, 4.00E+00*keV
};


    //
    // Water need to add rayleigh scattering! -- added automatically if material name = Water
    //
      G4double refractiveIndex1[] =
                { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
                  1.346,  1.3465, 1.347,  1.3475, 1.348,
                  1.3485, 1.3492, 1.35,   1.3505, 1.351,
                  1.3518, 1.3522, 1.3530, 1.3535, 1.354,
                  1.3545, 1.355,  1.3555, 1.356,  1.3568,
                  1.3572, 1.358,  1.3585, 1.359,  1.3595,
                  1.36,   1.3608};

      assert(sizeof(refractiveIndex1) == sizeof(photonEnergy));

      G4double absorption[] =
               {2.45E-04*cm, 7.27E-04*cm, 1.62E-03*cm, 5.19E-03*cm, 1.21E-02*cm ,2.35E-02*cm,
4.06E-02*cm, 9.64E-02*cm, 1.88E-01*cm, 5.98E-01*cm, 1.23E+00*cm, 2.66E+00*cm, 3.73E+00*cm,
4.41E+00*cm, 4.86E+00*cm, 5.44E+00*cm, 5.86E+00*cm, 6.64E+00*cm, 7.30E+00*cm, 8.43E+00*cm,
9.43E+00*cm, 1.03E+01*cm, 1.12E+01*cm, 1.27E+01*cm, 1.41E+01*cm, 1.43E+01*cm, 1.58E+01*cm,
1.74E+01*cm, 2.02E+01*cm, 2.05E+01*cm, 2.52E+01*cm, 2.94E+01*cm };

      assert(sizeof(absorption) == sizeof(photonEnergy2));

      G4double scintilFast[] =
                { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                  1.00, 1.00, 1.00, 1.00 };

      assert(sizeof(scintilFast) == sizeof(photonEnergy));

      G4double scintilSlow[] =
                { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
                  7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
                  3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
                  4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
                  7.00, 6.00, 5.00, 4.00 };

      assert(sizeof(scintilSlow) == sizeof(photonEnergy));

      G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

      myMPT1->AddProperty("RINDEX", photonEnergy, refractiveIndex1,nEntries)->SetSpline(true);
      myMPT1->AddProperty("ABSLENGTH",photonEnergy2, absorption, nEntries)->SetSpline(true);
      myMPT1->AddProperty("FASTCOMPONENT",photonEnergy, scintilFast, nEntries)->SetSpline(true);
      myMPT1->AddProperty("SLOWCOMPONENT",photonEnergy, scintilSlow, nEntries)->SetSpline(true);

      myMPT1->AddConstProperty("SCINTILLATIONYIELD",50./MeV);
      myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
      myMPT1->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
      myMPT1->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
      myMPT1->AddConstProperty("YIELDRATIO",0.8);

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

    G4double energy_ray_water[] = {
        1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
        1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
        1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
        1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
        1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
        2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
        2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
        2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV
    }; // 32 entries

    const G4int numentries_water_ray = sizeof(energy_ray_water)/sizeof(G4double);

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

      // gforward, gbackward, forward backward ratio
      G4double mie_water_const[3]={0.99,0.99,0.8};

      myMPT1->AddProperty("MIEHG",energy_water,mie_water,numentries_water)->SetSpline(true);
      myMPT1->AddConstProperty("MIEHG_FORWARD",mie_water_const[0]);
      myMPT1->AddConstProperty("MIEHG_BACKWARD",mie_water_const[1]);
      myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",mie_water_const[2]);

    G4double rayleigh_water[] = {
        2335.5*m, 2210.5*m, 2090.8*m, 1976.4*m, 1866.8*m, 1762.1*m,
        1662.1*m, 1566.6*m, 1475.4*m, 1388.5*m, 1305.6*m, 1224.6*m,
        1146.8*m, 1075.6*m, 1007.8*m, 941.23*m, 880.95*m, 821.12*m,
        766.36*m, 714.50*m, 665.40*m, 618.97*m, 575.11*m, 533.69*m,
        493.46*m, 457.10*m, 421.51*m, 389.06*m, 358.57*m, 329.98*m,
        303.17*m, 277.43*m

    }; // 32 entries

    // Note to Self: Never have spline set to true if rayleigh water goes to zero. Creates seg fault

    assert(sizeof(rayleigh_water) == sizeof(energy_ray_water));

    myMPT1->AddProperty("RAYLEIGH",energy_ray_water,rayleigh_water,numentries_water_ray)->SetSpline(true);

    //G4cout << "Water G4MaterialPropertiesTable" << G4endl;
      //myMPT1->DumpTable();

      Water->SetMaterialPropertiesTable(myMPT1);

      // Set the Birks Constant for the Water scintillator

      Water->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

    // APPLY OPTICAL QUALITIES

    /* ****************************************************************************************************************************************************** */

    G4OpticalSurface* water_opsurf= new G4OpticalSurface("water_opsurf");

    water_opsurf->SetType(dielectric_metal);
    water_opsurf->SetFinish(polished);
    water_opsurf->SetPolish(1.0);
    water_opsurf->SetModel(glisur);
    new G4LogicalBorderSurface("water_surf", physWater, physPMT, water_opsurf); // name, physical volume1, phsical volume2, G4optical surface

    // PMT
    G4double ephotonPMT[] = {1.7711*eV, 1.9074*eV, 2.0663*eV, 2.2542*eV, 2.4796*eV, 2.7551*eV, 3.0995*eV, 3.5423*eV, 4.133*eV};
    const G4int num = sizeof(ephotonPMT)/sizeof(G4double);

    // PMT Surface Properties
    G4double reflectivity[] = {0.95831455,  0.95812048,  0.95790026,  0.95761486,  0.95722493, 0.95674285,  0.95601079,  0.95598459,  0.95598459};
    assert(sizeof(reflectivity) == sizeof(ephotonPMT));
    G4double efficiency[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    assert(sizeof(efficiency) == sizeof(ephotonPMT));

    G4OpticalSurface* PMT_opsurf = new G4OpticalSurface("PMTSurface",unified,polished,dielectric_metal);
    G4MaterialPropertiesTable* PMTopt = new G4MaterialPropertiesTable();
    PMTopt->AddProperty("REFLECTIVITY", ephotonPMT, reflectivity, num);
    PMTopt->AddProperty("EFFICIENCY", ephotonPMT, efficiency, num);
    PMT_opsurf->SetMaterialPropertiesTable(PMTopt);

    // Photocathode surface properties
    G4double photocath_EFF[]={0.13392893,0.72115423,0.9375,0.99431704,1.,0.86805444,0.703125,0.44642893,0.20833417}; //Enables 'detection' of photons
    assert(sizeof(photocath_EFF) == sizeof(ephotonPMT));
    G4double photocath_ReR[]={3.3817,3.3970,3.4392,3.5246,3.6880,4.0462,4.3354,3.6361,3.5688};
    assert(sizeof(photocath_ReR) == sizeof(ephotonPMT));
    G4double photocath_ImR[]={1.7862,1.7538,1.7171,0.8109, 0.565079, 0.41516, 0.3074, 0.252807,0.24902};
    assert(sizeof(photocath_ImR) == sizeof(ephotonPMT));

    G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
    photocath_mt->AddProperty("EFFICIENCY",ephotonPMT,photocath_EFF,num);
    photocath_mt->AddProperty("REALRINDEX",ephotonPMT,photocath_ReR,num);
    photocath_mt->AddProperty("IMAGINARYRINDEX",ephotonPMT,photocath_ImR,num);

    G4OpticalSurface* photocath_opsurf= new G4OpticalSurface("photocath_opsurf"); // name, G4OpticalSurfaceModel, G4OpticalSurfaceFinish, G4SurfaceType, G4double value = 1.0
    photocath_opsurf->SetType(dielectric_metal);
    photocath_opsurf->SetFinish(polished);
    photocath_opsurf->SetPolish(1.0);
    photocath_opsurf->SetModel(glisur);
    photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

    // Create Logical Skin surfaces (might need to change these to G4LogicalBorderSurfaces)
    new G4LogicalSkinSurface("photocath_surf", logicPC, photocath_opsurf); // name, physical volume of surface, phsical volume of world?, G4optical surface


    //
    // Air
    //
      G4double refractiveIndex2[] =
                { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                  1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                  1.00, 1.00, 1.00, 1.00 };

      G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
      myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndex2, nEntries);

      //G4cout << "Air G4MaterialPropertiesTable" << G4endl;
      //myMPT2->DumpTable();

      air->SetMaterialPropertiesTable(myMPT2);

    // Set the Surface Properties


  //
  //always return the physical World!!!
  //
  return physWorld;
}

/* ************************************************************************************ */

void DetectorConstruction::ConstructSDandField() {


// PMT SD

  //Created here so it exists as pmts are being placed
    G4cout << "Construction /Det/pmtSD" << G4endl;
    PMTSD* pmt_SD = new PMTSD("/Det/pmtSD");
    pmt_SD->InitPMTs(1); //let pmtSD know # of pmts
    pmt_SD->SetPmtPositions(PmtPositions);
    G4SDManager::GetSDMpointer()->AddNewDetector(pmt_SD);
    SetSensitiveDetector(logicPC, pmt_SD);
    //sensitive detector is not actually on the photocathode.
    //processHits gets done manually by the stepping action.
    //It is used to detect when photons hit and get absorbed&detected at the
    //boundary to the photocathode (which doesnt get done by attaching it to a
    //logical volume.
    //It does however need to be attached to something or else it doesnt get
    //reset at the begining of events

}
