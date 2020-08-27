// Always include
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
// for physics List
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
// Typcially include
#include "time.h"
#include "Randomize.hh"
#include "vector"
#include <iostream>
#include "G4Types.hh"
// Recommended by Ethan
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

// For G4cout and G4cerr handling
#include "MySession.hh"
#include "G4ios.hh"
#include "G4UIsession.hh"

// declare global variables

G4String macro;
G4long seed;
G4String output_name;
G4int maxNumber;
G4String gOutName;

namespace
{
void PrintUsage()
{
        std::cerr << "Usage: " << std::endl;
        std::cerr << "mantis [-m macro] [-s seed] [-r output_name] [-c maxNumber] [-l logFile_name]" << std::endl;
}
}

int main(int argc,char **argv)
{

  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
        G4int start_time = time(NULL);
        // Default inputs
        macro = "mantis.in";
        seed = 1;
        output_name = "none";
        maxNumber = 500;
        gOutName = "MyLogFile.out";
        // Evaluate Arguments
        if ( argc > 11 )
        {
                PrintUsage();
                return 1;
        }

        for (G4int i=1; i<argc; i=i+2) {
                if (G4String(argv[i]) == "-m") macro = argv[i+1];
                else if (G4String(argv[i]) == "-s") seed = atoi(argv[i+1]);
                else if (G4String(argv[i]) == "-r") output_name = argv[i+1];
                else if (G4String(argv[i]) == "-c") maxNumber = atoi(argv[i+1]);
                else if (G4String(argv[i]) == "-l") gOutName = argv[i+1];
                else
                {
                        PrintUsage();
                        return 1;
                }
        }

        G4UImanager* UI = G4UImanager::GetUIpointer();
        MySession* LoggedSession = new MySession;
        if(! ui && macro != "vis_save.mac"){
          UI->SetCoutDestination(LoggedSession);
        }
        // choose the Random engine
        CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
        CLHEP::HepRandom::setTheSeed(seed);

        // construct the default run manager
        G4RunManager* runManager = new G4RunManager;

        // set mandatory initialization classes
        DetectorConstruction* det = new DetectorConstruction();
        runManager->SetUserInitialization(det);

        // Set up Physics List
        G4VModularPhysicsList* physicsList = new FTFP_BERT(0);
        physicsList->ReplacePhysics(new G4EmStandardPhysics_option4(0));

        G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics(0);
        opticalPhysics->SetWLSTimeProfile("delta");
        opticalPhysics->SetScintillationYieldFactor(1.0);
        opticalPhysics->SetScintillationExcitationRatio(0.0);
        opticalPhysics->SetMaxNumPhotonsPerStep(maxNumber);
        opticalPhysics->SetMaxBetaChangePerStep(10.0);
        opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true);
        opticalPhysics->SetTrackSecondariesFirst(kScintillation, true);
        physicsList->SetVerboseLevel(0);
        physicsList->RegisterPhysics(opticalPhysics);
        runManager->SetUserInitialization(physicsList);

        runManager->SetUserInitialization(new ActionInitialization(det));
        // initialize G4 kernel
        //runManager->Initialize();

        // Initialize visualization
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


  delete LoggedSession;
  delete runManager;




        //delete UI;

        G4int stop_time = time(NULL);

        std::cout<<" The MC took:\t\t" << stop_time - start_time <<"s"<< std::endl;
        std::cout << std::endl << " Run completed!"<< std::endl;

        return 0;
}
