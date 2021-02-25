// Always include
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "DetectorConstruction.hh"
#include "PhysicsListNew.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "RootDataManager.hh"
#include "runMetadata.hh"

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

// For FileSystem Handling
#include <sys/stat.h>
// For MPI
#ifdef MANTIS_MPI_ENABLED
#include "G4MPImanager.hh"
#endif

// C++ classes
#include <ctime>
#include <unistd.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string>

// declare global variables
G4long seed;
G4double chosen_energy;
// String global variables
G4String macro, root_output_name, gOutName, inFile;
// boolean global variables
G4bool bremTest, resonanceTest, checkEvents, debug;

void check_file_exists(const std::string &p)
{
  struct stat buffer;

  if(stat (p.c_str(), &buffer) == 0)
    std::cout << p << " exists." << std::endl;
  else
  {
    std::cout << p << " does not exist." << std::endl;
    std::cout << "User MUST Set the correct Path for the NRF Database as an environment variable." <<
    std::endl << "See README For Details!" << std::endl;
    exit(1);
  }
}

namespace
{
void PrintUsage()
{
        G4cerr << "Usage Example: " << G4endl;
        G4cerr << "./mantis mantis.in -o test.root -s 1" << G4endl;
        G4cerr << "Would run mantis with the macro named mantis.in. The Root data "
        << G4endl << "file would be outputed to test.root and the simulation would be ran "
        << G4endl << "with a seed of 1" << G4endl << G4endl;
        G4cerr << "mantis Options Menu: " << G4endl;
        G4cerr << "-h Print this Usage Menu" << G4endl;
        G4cerr << "-a Energy for a Monoenergetic Beam (MeV)" << G4endl;
        G4cerr << "-d Run in Debugging Mode (extra verbosity)" << G4endl;
        G4cerr << "-e Check Events in NRF, Cherenkov and Det TTrees to see coincidence in Event IDs" << G4endl;
        G4cerr << "-i Input ROOT File to be read as Input Spectrum instead of Sampling from brems_distributions.root" << G4endl;
        G4cerr << "-n Add NRF Physics default = true" << G4endl;
        G4cerr << "-o Output filename default = test.root" << G4endl;
        G4cerr << "-p Print Standalone Database of NRF Energies" << G4endl;
        G4cerr << "-r Set Resonance Test. The input spectrum will be from Uranium Resonance Energies" << G4endl;
        G4cerr << "-s Seed for the Run Manager" << G4endl;
        G4cerr << "-v Set NRF Verbosity. Prints verbosity from G4NRF" << G4endl;
        exit(1);
}
}

int main(int argc,char **argv)
{
  // Check to make sure user set up environment correctly
  if(getenv("G4NRFGAMMADATA") == NULL)
  {
    std::cout << "mantis.cc -> FATAL ERROR: User must set environmental variable G4NRFGAMMADATA!" << std::endl;
    exit(1);
  }

  if(getenv("MANTIS_TOPDIR") == NULL)
  {
    G4cout << "mantis.cc -> FATAL ERROR: User must set environmental variable MANTIS_TOPDIR"
    << G4endl << "Try running . ./setup.sh" << G4endl;
  }
  check_file_exists((std::string)getenv("G4NRFGAMMADATA"));
  G4String MANTIS_TOPDIR = (G4String)std::getenv("MANTIS_TOPDIR");

  // Defaults
  G4int start_time = time(0);
  // Physics List Defaults
  G4bool use_xsec_tables = true;
  G4bool use_xsec_integration = true;
  G4bool force_isotropic = false;
  G4String standalone_in = "false";
  G4String verbose_in = "false";
  G4String addNRF_in = "true";

  G4bool standalone = false;
  G4bool NRF_Verbose = false;
  G4bool addNRF = true;
  // Run Defaults
  macro = "mantis.in";
  seed = 1;
  inFile = "brems_distributions.root";
  G4String debug_in = "false";
  debug = false;
  // Primary Generator Defaults
  G4String resonance_in = "false";
  resonanceTest = false;
  chosen_energy = -1.;
  G4String bremTest_in = "false";
  bremTest = false;

  // Output Defaults
  root_output_name = "test.root";
  G4String checkEvents_in = "false";
  checkEvents = false;

  // Select whether mantis architecture is sequential or parallel based on
  // the binary name (sequential=="mantis"), parallel=="mantis_mpi")

  G4bool sequentialBuild = true;
  std::string arg0 = argv[0];
  macro = argv[1];
  if(arg0=="mantis_mpi")
    sequentialBuild = false;

  if(sequentialBuild)
  {
    std::cout << "Reading Inputs..." << std::endl;
    for (G4int i=1; i<argc; i=i+2)
    {
      if      (G4String(argv[i]) == "-h") PrintUsage();
      else if (G4String(argv[i]) == "-a") chosen_energy = std::stod(argv[i+1]);
      else if (G4String(argv[i]) == "-s") seed = atoi(argv[i+1]);
      else if (G4String(argv[i]) == "-o") root_output_name = argv[i+1];
      else if (G4String(argv[i]) == "-t") bremTest_in = argv[i+1];
      else if (G4String(argv[i]) == "-r") resonance_in = argv[i+1];
      else if (G4String(argv[i]) == "-p") standalone_in = argv[i+1];
      else if (G4String(argv[i]) == "-v") verbose_in = argv[i+1];
      else if (G4String(argv[i]) == "-n") addNRF_in = argv[i+1];
      else if (G4String(argv[i]) == "-e") checkEvents_in = argv[i+1];
      else if (G4String(argv[i]) == "-i") inFile = argv[i+1];
      else if (G4String(argv[i]) == "-d") debug_in = argv[i+1];
      else
      {
        PrintUsage();
      }
    }
  }

  // Handle Output File
  std::string RootOutputFile = (std::string)root_output_name;
  if(RootOutputFile.find(".root")<RootOutputFile.length()) {
          gOutName=(std::string)RootOutputFile.substr(0, RootOutputFile.find(".root"));
  }
  else gOutName=(std::string)root_output_name;

  // Handle Debugging
  if(debug_in == "True" || debug_in == "true")
  {
    std::cout << "Debugging mode set." << std::endl;
    debug = true;
  }

  // construct the default run manager
  G4RunManager* runManager = new G4RunManager;
  G4UImanager* UI = G4UImanager::GetUIpointer();

  #ifdef MANTIS_MPI_ENABLED
    const G4int argcMPI = 2;
    char *argvMPI[argcMPI];
    argvMPI[0] = argv[0]; // binary name
    argvMPI[1] = (char *)"/tmp/MANTISSlave"; // slave file base name
    if(debug)
      std::cout << "Instantiating MPIManager." << std::endl;
    if(debug)
    {
      std::cout << argvMPI[0] << std::endl;
      std::cout << argvMPI[1] << std::endl;
    }
    MPIManager *theMPIManager = new MPIManager(argcMPI,argvMPI);
    if(debug)
      std::cout << "MPIManager Instantiated." << std::endl;
  #endif

  MySession* LoggedSession = 0;
  if(sequentialBuild)
  {
    LoggedSession = new MySession;
    if(macro != "vis_save.mac")
    {
      UI->SetCoutDestination(LoggedSession);
    }
  }

  // Physics List Options
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
  if(addNRF_in == "False" || addNRF_in == "false")
  {
          //std::cout << "NRF Physics turned OFF!" << std::endl;
          G4cout << "NRF Physics turned OFF!" << G4endl;
          addNRF = false;
  }

  // Primary Generator Options
  if(bremTest_in == "True" || bremTest_in == "true")
  {
          G4cout << "Conducting Brem Test!" << G4endl;
          bremTest = true;
  }

  if(checkEvents_in == "True" || checkEvents_in == "true")
  {
          G4cout << "Checking Events!" << G4endl;
          checkEvents = true;
  }

  if(resonance_in == "True" || resonance_in == "true")
  {
          G4cout << "Completing Resonance Test!" << G4endl;
          resonanceTest = true;
  }

  // Some User Error Checking
  if(bremTest && resonanceTest)
  {
          G4cerr << "FATAL ERROR mantis.cc -> Cannot test bremsstrahlung and resonance during the same run!" << G4endl;
          exit(1);
  }
  if(bremTest && chosen_energy < 0)
  {
          G4cerr << "FATAL ERROR mantis.cc -> Cannot test bremsstrahlung without option -a input energy!" << G4endl;
          exit(1);
  }

  // ***************************** set mandatory initialization classes **************************** //

  DetectorConstruction* det = new DetectorConstruction();
  runManager->SetUserInitialization(det);

  // Set up Physics List
  PhysicsListNew *thePLNew = new PhysicsListNew(addNRF, use_xsec_tables,
                                                use_xsec_integration, force_isotropic,
                                                standalone, NRF_Verbose);
  runManager->SetUserInitialization(thePLNew);

  PrimaryGeneratorAction* pga = new PrimaryGeneratorAction();
  runManager->SetUserAction(pga);
  runManager->Initialize();

  // Set User action classes
  RunAction* run = new RunAction(pga,sequentialBuild);
  runManager->SetUserAction(run);
  EventAction* event = new EventAction();
  runManager->SetUserAction(event);
  runManager->SetUserAction(new SteppingAction(det, run, event));
  runManager->SetUserAction(new StackingAction(det, run));
  RootDataManager *manager = new RootDataManager(sequentialBuild);
  run->SetRootDataManager(manager);

// ******************************************************************************************************************** //
  if(sequentialBuild)
  {
    if(macro == "vis_save.mac")
    {
      #ifdef G4VIS_USE
        visManager = new G4VisExecutive();
        visManager->Initialize();
      #else
        std::cerr << "FATAL ERROR G4VIS_USE not defined in Precompiler!" << std::endl;
        return 1;
      #endif
    }


    std::cout << "Seed set to: " << seed << std::endl;

    // choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
    CLHEP::HepRandom::setTheSeed(seed);

    G4String command = "/control/execute ";
    UI->ApplyCommand(command+macro);

    #ifdef G4VIS_USE
        if(macro == "vis_save.mac")
        {
          delete visManager;
        }
    #endif
    delete LoggedSession;
  }// if sequentialBuild
  #ifdef MANTIS_MPI_ENABLED
    else // RUNNING MPI
    {
      // Internally assign niceness of 19 to parallel builds
      G4int priority = 19;
      setpriority(PRIO_PROCESS,getpid(),priority);
      G4int MPI_Rank = theMPIManager->GetRank();
      CLHEP::HepRandom::setTheSeed(seed+ 7*MPI_Rank);
      G4String command = "/control/execute ";
      UI->ApplyCommand(command+macro);
      delete theMPIManager;
    }
  #endif


  G4int stop_time = time(0);
  G4cout << G4endl << "----------------------------------------------------------------------" << G4endl;
  G4cout << G4endl << "----------------------------------------------------------------------" << G4endl;
  G4cout << G4endl << "The MC took:\t\t" << stop_time - start_time << "s" << G4endl << G4endl;

  delete manager;
  delete runManager;

  return 0;
}
