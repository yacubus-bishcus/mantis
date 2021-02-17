// Always include
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "DetectorConstruction.hh"
#include "PhysicsListNew.hh"
#include "ActionInitialization.hh"
// Typcially include
#include "time.h"
#include "Randomize.hh"
#include "G4Types.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#include "G4TrajectoryDrawByParticleID.hh"
G4VisManager* visManager;
#endif

#include "G4UIExecutive.hh"

// For G4cout and G4cerr handling
#include "MySession.hh"
#include "G4ios.hh"
#include "G4UIsession.hh"

// declare global variables
G4long seed;
G4String macro, root_output_name, gOutName, bremTest, resonance_in, standalone_in, verbose_in, addNRF_in, checkEvents_in, weight_histo_in, inFile;
G4double chosen_energy;

namespace
{
void PrintUsage()
{
        G4cerr << "Usage: " << G4endl;
        G4cerr << "mantis [-m macro=mantis.in] [-a chosen_energy=-1.] [-s seed=1] [-o output_name] [-t bremTest=false] " <<
                "[-r resonance_test=false] [-p standalone=false] [-v NRF_Verbose=false] [-n addNRF=true] " <<
                "[-e checkEvents=false] [-w weightHisto=false] [-i inFile]"
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
        chosen_energy = -1.;
        seed = 1;
        G4bool output = false;
        G4bool resonance_test = false;
        bremTest = "false";
        G4bool brem = false;
        inFile = "brems_distributions.root";

        // Detect interactive mode (if no arguments) and define UI session
        //
        G4UIExecutive* ui = 0;

        if ( argc == 1 ) {
                ui = new G4UIExecutive(argc, argv);
        }

        // Evaluate Arguments
        if ( argc > 21)
        {
                PrintUsage();
                return 1;
        }

        for (G4int i=1; i<argc; i=i+2)
        {
                if (G4String(argv[i]) == "-m") macro = argv[i+1];
                else if (G4String(argv[i]) == "-a") chosen_energy = std::stod(argv[i+1]);
                else if (G4String(argv[i]) == "-s") seed = atoi(argv[i+1]);
                else if (G4String(argv[i]) == "-o") root_output_name = argv[i+1];
                else if (G4String(argv[i]) == "-t") bremTest = argv[i+1];
                else if (G4String(argv[i]) == "-r") resonance_in = argv[i+1];
                else if (G4String(argv[i]) == "-p") standalone_in = argv[i+1];
                else if (G4String(argv[i]) == "-v") verbose_in = argv[i+1];
                else if (G4String(argv[i]) == "-n") addNRF_in = argv[i+1];
                else if (G4String(argv[i]) == "-e") checkEvents_in = argv[i+1];
                else if (G4String(argv[i]) == "-w") weight_histo_in = argv[i+1];
                else if (G4String(argv[i]) == "-i") inFile = argv[i+1];
                else
                {
                        PrintUsage();
                        return 1;
                }
        }
        
        std::cout << root_output_name << std::endl;
        std::string RootOutputFile = (std::string)root_output_name;
        if(RootOutputFile.find(".root")<RootOutputFile.length()) {
                gOutName=(std::string)RootOutputFile.substr(0, RootOutputFile.find(".root"));
        }
        else gOutName=(std::string)root_output_name;

        //std::cout << "G4UImanager::GetUIpointer()" << std::endl;
        G4UImanager* UI = G4UImanager::GetUIpointer();
        MySession* LoggedSession = new MySession;

        if(!ui && macro != "vis_save.mac")
        {
                output = true;
                UI->SetCoutDestination(LoggedSession);
        }
        //std::cout << "UI->SetCoutDestination" << std::endl;
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
        if(resonance_in == "True" || resonance_in == "true")
        {
                G4cout << "Conducting Resonance Test!" << G4endl;
                resonance_test = true;
        }

        G4cout << "Seed set to: " << seed << G4endl;
        std::cout << "Seed set to: " << seed << std::endl;

        // choose the Random engine
        CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
        CLHEP::HepRandom::setTheSeed(seed);

        // construct the default run manager
        G4RunManager* runManager = new G4RunManager;

        // set mandatory initialization classes

        DetectorConstruction* det = new DetectorConstruction(brem);
        runManager->SetUserInitialization(det);

        // Set up Physics List
        PhysicsListNew *thePLNew = new PhysicsListNew(addNRF, use_xsec_tables, use_xsec_integration, force_isotropic, standalone, NRF_Verbose);
        runManager->SetUserInitialization(thePLNew);

        runManager->SetUserInitialization(new ActionInitialization(det, brem, resonance_test, output, checkEvents, weightHisto));

#ifdef G4VIS_USE
        if(ui || macro == "vis_save.mac")
        {
                visManager = new G4VisExecutive();
                visManager->Initialize();
        }
#endif
        //std::cout << "visManager Initialized" << std::endl;
        if(!ui)
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
//std::cout << "Deleting visManager..." << std::endl;
#ifdef G4VIS_USE
        if(ui || macro == "vis_save.mac")
        {
                delete visManager;
        }
#endif

        G4int stop_time = time(0);
        G4cout << G4endl << "*-----------------------------------------------------------*" << G4endl;
        G4cout << G4endl << "*-----------------------------------------------------------*" << G4endl;
        G4cout << G4endl << "The MC took:\t\t" << stop_time - start_time << "s" << G4endl << G4endl;

        delete LoggedSession;
        delete runManager;

        return 0;
}
