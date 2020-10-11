#include "HistoManager.hh"

extern G4String gOutName;
extern G4bool output;
HistoManager::HistoManager(): fFactoryOn(false)
{}

HistoManager::~HistoManager()
{}

void HistoManager::Book()
{
  if(output)
  {

    G4AnalysisManager* manager = G4AnalysisManager::Instance();
    manager->SetVerboseLevel(0);
    manager->SetNtupleMerging(true);

    // Create Directories
    //manager->SetNtupleDirectoryName("CherenkovData");

    // open output file
    G4bool fileOpen = manager->OpenFile(gOutName);

    if(! fileOpen){
      G4cerr << "HistoManager::Book(): Cannot Open " <<manager->GetFileName()<<G4endl;
      return;
    }
    else
    {
      std::cout << "HistoManager::Book() Opened!" << std::endl;
    }

    // Create 0 Ntuple for Chopper Data 
      manager->CreateNtuple("ChopData","Chopper Data");
      manager->CreateNtupleDColumn("E_incident");
      manager->FinishNtuple();
    
    // Create 1 Ntuple for Interogation Object
      manager->CreateNtuple("IncObj","Incident Interrogation Obj");
      manager->CreateNtupleDColumn("E_incident");
      manager->CreateNtupleIColumn("isNRF");
      manager->FinishNtuple();
    
    // Create 2 Ntuple for incident water data
      manager->CreateNtuple("IncWater","Incident Water");
      manager->CreateNtupleDColumn("E_incident");
      manager->CreateNtupleIColumn("isNRF");
      manager->FinishNtuple();
    
    // Create 3 Ntuple for water data
      manager->CreateNtuple("Cherenkov","Cherenkov Data");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("x");
      manager->CreateNtupleDColumn("y");
      manager->CreateNtupleDColumn("z");
      manager->CreateNtupleDColumn("Theta");
      manager->CreateNtupleDColumn("Time");
      manager->FinishNtuple();

      // Create 4 Ntuple for Incident Detector Data
      manager->CreateNtuple("IncDet","Incident Photocathode");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("x");
      manager->CreateNtupleDColumn("y");
      manager->CreateNtupleDColumn("z");
      manager->FinishNtuple();

      // Create 5 Nutple for Energy if detected
      manager->CreateNtuple("Det","Detected Spectrum");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("x");
      manager->CreateNtupleDColumn("y");
      manager->CreateNtupleDColumn("z");
      manager->FinishNtuple();
    
      // Create 6 Ntuple for Reactions within detector
      manager->CreateNtuple("DetPro","Detector Processes");
      manager->CreateNtupleSColumn("Process");
      manager->FinishNtuple();

      // Create 7 Ntuple for Number of Photons Emitted in Water
      manager->CreateNtuple("NumPhotons","Number of Photons Produced Per Primary Particle Energy");
      manager->CreateNtupleDColumn("E_Beam");
      manager->CreateNtupleDColumn("NumPhotons");
      manager->FinishNtuple();

      fFactoryOn = true;
      G4cout << "Data Book Created." << G4endl;
      G4cout << "Output file is open in " << manager->GetFileName()<<"."
            << manager->GetFileType() << G4endl;

  }
  else
  {
    std::cout << "Visualization...no histogramming." << std::endl;
  }

}

void HistoManager::finish()
{
  if(output)
  {


    if(! fFactoryOn){
        G4cout << "ERROR HistoManager::finish: Failed to write to file" << G4endl;
        return;
    }
    G4AnalysisManager* manager = G4AnalysisManager::Instance();
    manager->Write();
    manager->CloseFile();
    G4cout << "Ntuples are saved. " << G4endl;

    delete manager;
    fFactoryOn = false;
  }
}
