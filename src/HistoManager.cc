#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

extern G4String gOutName;
HistoManager::HistoManager():fFactoryOn(false)
{}

HistoManager::~HistoManager()
{}

void HistoManager::Book()
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

  // Create Ntuple for water data
    manager->CreateNtuple("WaterData","WaterData");

    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("x");
    manager->CreateNtupleDColumn("y");
    manager->CreateNtupleDColumn("z");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Time");
    manager->FinishNtuple();

    // Create Ntuple for Incident Detector Data

    manager->CreateNtuple("IncidentData","IncidentData");

    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("x");
    manager->CreateNtupleDColumn("y");
    manager->CreateNtupleDColumn("z");
    manager->FinishNtuple();

    // Create Ntuple for Reactions within detector

    manager->CreateNtuple("DetData","DetData");

    manager->CreateNtupleSColumn("Process");
    manager->CreateNtupleDColumn("E_beam");
    manager->FinishNtuple();

    // Create Nutple for Energy if detected
    manager->CreateNtuple("DetEnergy","DetEnergy");
    manager->CreateNtupleDColumn("det_energy");
    manager->CreateNtupleDColumn("x");
    manager->CreateNtupleDColumn("y");
    manager->FinishNtuple();

    // Create Ntuple for Number of Photons Emitted in Water
    manager->CreateNtuple("NumPhotons","NumPhotons");

    manager->CreateNtupleDColumn("E_Beam");
    manager->CreateNtupleDColumn("NumPhotons");
    manager->FinishNtuple();

    fFactoryOn = true;
    G4cout << "Data Book Created." << G4endl;
    G4cout << "Output file is open in " << manager->GetFileName()<<"."
          << manager->GetFileType() << G4endl;

}

void HistoManager::finish()
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
