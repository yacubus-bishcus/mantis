// Always include
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "DetectorConstruction.hh"
#include "physicsList.hh"
#include "ActionInitialization.hh"
// Typcially include
#include "time.h"
#include "Randomize.hh"
//#include "vector"
//#include <iostream>
#include "G4Types.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#include "G4TrajectoryDrawByParticleID.hh"
#endif

#include "G4UIExecutive.hh"

// For G4cout and G4cerr handling
#include "MySession.hh"
#include "G4ios.hh"
#include "G4UIsession.hh"

// declare global variables
G4long seed;
G4String macro, root_output_name, gOutName, bremTest, standalone_in, verbose_in, addNRF_in, checkEvents_in, weight_histo_in; 

namespace
{
  void PrintUsage()
  {
    G4cerr << "Usage: " << G4endl;
    G4cerr << "mantis [-m macro=mantis.in] [-s seed=1] [-o output_name] [-t bremTest=false] [-p standalone=false] [-v NRF_Verbose=false] [-n addNRF=true] [-e checkEvents=false] [-w weightHisto=false]" 
              << G4endl;
  }
}

int main(int argc,char **argv)
{
  // Defaults
  G4int start_time = time(0);
  G4bool use_xsec_tables = true;
  G4bool use_xsec_integration = true;
  G4bool force_isotropic = false;
  G4bool standalone = false;
  G4bool NRF_Verbose = false;
  G4bool addNRF = true;
  G4bool checkEvents = false;
  G4bool weightHisto = false;
  macro = "mantis.in";
  seed = 1;
  G4bool output = false;
  bremTest = "false";
  G4bool brem = false;

  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;

  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Evaluate Arguments
  if ( argc > 19 )
  {
    PrintUsage();
    return 1;
  }

  for (G4int i=1; i<argc; i=i+2) 
  {
    if (G4String(argv[i]) == "-m") macro = argv[i+1];
    else if (G4String(argv[i]) == "-s") seed = atoi(argv[i+1]);
    else if (G4String(argv[i]) == "-o") root_output_name = argv[i+1];
    else if (G4String(argv[i]) == "-t") bremTest = argv[i+1];
    else if (G4String(argv[i]) == "-p") standalone_in = argv[i+1];
    else if (G4String(argv[i]) == "-v") verbose_in = argv[i+1];
    else if (G4String(argv[i]) == "-n") addNRF_in = argv[i+1];
    else if (G4String(argv[i]) == "-e") checkEvents_in = argv[i+1];
    else if (G4String(argv[i]) == "-w") weight_histo_in = argv[i+1];
    else
    {
      PrintUsage();
      return 1;
    }
  }

  std::string RootOutputFile = (std::string)root_output_name;
  if(RootOutputFile.find(".root")<RootOutputFile.length()) {
          gOutName=(std::string)RootOutputFile.substr(0, RootOutputFile.find(".root"));
  }
  else gOutName=(std::string)root_output_name;

  G4UImanager* UI = G4UImanager::GetUIpointer();
  MySession* LoggedSession = new MySession;
  
  if(! ui && macro != "vis_save.mac")
  {
    output = true;
    UI->SetCoutDestination(LoggedSession);
  }
  
  // Write option selection to Outfile 
  if(standalone_in == "True" || standalone_in == "true")
  {
    G4cout << "Standalone File Requested." << G4endl;
    standalone = true;
  }
  if(verbose_in == "True" || verbose_in == "true")
  {
    G4cout << "NRF Verbose set to: " << verbose_in << G4endl;
    NRF_Verbose = true;
  }
  if(checkEvents_in == "True" || checkEvents_in == "true")
  {
    G4cout << "Check Events set to: " << checkEvents_in << G4endl;
    checkEvents = true;
  }
  if(weight_histo_in == "True" || weight_histo_in == "true")
  {
    G4cout << "Weight Histograms set to: " << weight_histo_in << G4endl;
    weightHisto = true;
  }
  if(addNRF_in == "False" || addNRF_in == "false")
  {
    //std::cout << "NRF Physics turned OFF!" << std::endl;
    G4cout << "NRF Physics turned OFF!" << G4endl;
    addNRF = false;
  }
  if(bremTest == "True" || bremTest == "true")
  {
     G4cout << "Conducting Brem Test!" << G4endl;
     brem = true;
  }
  G4cout << "Seed set to: " << seed << G4endl;
  
  // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
  CLHEP::HepRandom::setTheSeed(seed);

  // construct the default run manager
  G4RunManager* runManager = new G4RunManager;
  
  // set mandatory initialization classes
  DetectorConstruction* det = new DetectorConstruction(brem);
  runManager->SetUserInitialization(det);

  // Set up Physics List
  physicsList *thePL = new physicsList(addNRF, use_xsec_tables, use_xsec_integration, force_isotropic, standalone, NRF_Verbose);
  runManager->SetUserInitialization(thePL);
  runManager->SetUserInitialization(new ActionInitialization(det, brem, output, checkEvents, weightHisto));

  // Run manager initialized in macros
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();
#endif

  if(! ui)
  {
    G4String command = "/control/execute ";
    UI->ApplyCommand(command+macro);
  }
  else
  {
    // interactive mode
    UI->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  if(ui || macro == "vis_save.mac")
  {
    delete visManager;
  }

  G4int stop_time = time(0);
  G4cout << "The MC took:\t\t" << stop_time - start_time << "s" <<G4endl;

  delete LoggedSession;
  delete runManager;

  return 0;
}
