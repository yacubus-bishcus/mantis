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

    // Create Ntuple for water data
      manager->CreateNtuple("WaterData","WaterData");

      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleIColumn("isNRF");
      manager->CreateNtupleDColumn("x");
      manager->CreateNtupleDColumn("y");
      manager->CreateNtupleDColumn("z");
      manager->CreateNtupleDColumn("Theta");
      manager->CreateNtupleDColumn("Time");
      manager->FinishNtuple();

      // Create Ntuple for Incident Detector Data

      manager->CreateNtuple("IncidentDet","IncidentDet");

      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleIColumn("isNRF");
      manager->CreateNtupleDColumn("x");
      manager->CreateNtupleDColumn("y");
      manager->CreateNtupleDColumn("z");
      manager->FinishNtuple();

      // Create Ntuple for Reactions within detector

      manager->CreateNtuple("DetProcess","DetProcess");

      manager->CreateNtupleSColumn("Process");
      manager->FinishNtuple();

      // Create Nutple for Energy if detected
      manager->CreateNtuple("Detected","Detected");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleIColumn("isNRF");
      manager->CreateNtupleDColumn("x");
      manager->CreateNtupleDColumn("y");
      manager->CreateNtupleDColumn("z");
      manager->FinishNtuple();

      // Create Ntuple for Number of Photons Emitted in Water
      manager->CreateNtuple("NumPhotons","NumPhotons");
      manager->CreateNtupleDColumn("E_Beam");
      manager->CreateNtupleDColumn("NumPhotons");
      manager->FinishNtuple();

      // Create Ntuple for Interogation Object
      manager->CreateNtuple("IntObj","IntObj");
      manager->CreateNtupleDColumn("E_incident");
      manager->CreateNtupleIColumn("isNRF");
      manager->FinishNtuple();

      // Create Ntuple for incident water data
      manager->CreateNtuple("IncidentWater","IncidentWater");
      manager->CreateNtupleDColumn("E_incident");
      manager->CreateNtupleIColumn("isNRF");
      manager->FinishNtuple();

      manager->CreateNtuple("ChopperData","ChopperData");
      manager->CreateNtupleDColumn("E_incident");
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
