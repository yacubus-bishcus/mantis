// User histogram manager action class for the Cargo Container NRF simulations
// Jacob E Bickus, MIT, 2020
// jbickus@mit.edu

#include "HistoManager.hh"

extern G4String gOutName;
HistoManager::HistoManager(): fFactoryOn(false), chosen_energy(-1)
{
}

HistoManager::~HistoManager()
{
}

void HistoManager::Book(G4bool bremTest)
{
  G4AnalysisManager* manager = G4AnalysisManager::Instance();
  G4int nbins = 100000;
  manager->SetVerboseLevel(0);
  manager->SetNtupleMerging(true);
  xmax = chosen_energy;
  #if defined (G4ANALYSIS_USE_ROOT)
      if(!bremTest && chosen_energy < 0)
      {
          TFile *fin = TFile::Open("brems_distributions.root");
          hBrems  = (TH1D*) fin->Get("hBrems");
          if (hBrems)
          {
              G4cout << "Imported brems from " << fin->GetName() << G4endl << G4endl;
              xmax = hBrems->GetXaxis()->GetXmax();
              std::cout << "Found Input Max Energy: " << xmax << " MeV" << std::endl;
              G4cout << "Found Input Max Energy: " << xmax << " MeV" << G4endl;
              fin->Close();
          }
          else
          {
                  std::cerr << "Error reading from file " << fin->GetName() << std::endl;
                  exit(1);
          }
      }

  #endif

  // open output file
  G4bool fileOpen = manager->OpenFile(gOutName);

  if(! fileOpen){
    G4cerr << "HistoManager::Book(): Cannot Open " <<manager->GetFileName()<<G4endl;
    return;
  }
  else
  {
    G4cout << "HistoManager::Book() Opened!" << G4endl;
  }
  
  // ************************ Histograms and Ntuples **************************** // 
  // The user will have the option to output 4 histograms and 5 ntuples 
  
  if(!bremTest)
  {
    // Histograms 
    // ************************************************************************* //
      // Create ID 0 1DHistogram for Interogation Object
      manager->CreateH1("IncObj","Interrogation Object Incident Weighted Energy Spectrum", nbins, 0., xmax, "MeV");
    
      // Create ID 1 1D Histogram for NRF Incident Interrogation Object 
      manager->CreateH1("NRFIncObject", "Interrogation Object NRF Photons Incident Weighted Energy Spectrum", nbins, 0., xmax, "MeV");
      // Create ID 2 1D Histogram for NRF Incident Water Tank 
      manager->CreateH1("NRFIncWater","Water Tank NRF Photons Incident Weighted Energy Spectrum", nbins, 0., xmax, "MeV");
    
    // Ntuples (TTrees)
    // ************************************************************************* //
      // Create ID 0 Ntuple for Brem Test Chop Data 
      manager->CreateNtuple("ChopperData", "Chopper Ntuple Data");
      manager->CreateNtupleDColumn("ChopperData");
      manager->FinishNtuple();
      // Create ID 1 Ntuple for cherenkov in water 
      manager->CreateNtuple("Cherenkov","Cherenkov Data");
      manager->CreateNtupleDColumn("BeamEnergy");
      manager->CreateNtupleIColumn("NumPhotons");
      manager->FinishNtuple();
    
      // Create ID 2 Ntuple for Incident Water Tank Data
      manager->CreateNtuple("IncWater", "Incident Water Tank Weighted Energy Spectrum");
      manager->CreateNtupleDColumn("IncidentEnergy");
      manager->FinishNtuple();
    
      // Create ID 3 Ntuple for Incident Photocathode Data
      manager->CreateNtuple("IncPC", "Incident Photocathode Weighted Energy Spectrum");
      manager->CreateNtupleDColumn("IncidentEnergy");
      manager->FinishNtuple();
    
      // Create ID 4 Ntuple for Photons Detected Data
      manager->CreateNtuple("Detected", "Photons Detected on Photocathode Weighted Energy Spectrum");
      manager->CreateNtupleDColumn("Energy");
  
      // Create ID 5 Ntuple for Reactions within Detector
      manager->CreateNtuple("DetPro","Detector Processes");
      manager->CreateNtupleSColumn("Process");
      manager->FinishNtuple();
  }
  else
  {
      // Create 0 Ntuple for Brem Test Chop Data 
      manager->CreateNtuple("ChopperData", "Chopper Ntuple Data");
      manager->CreateNtupleDColumn("ChopperData");
      manager->FinishNtuple();
  }

  fFactoryOn = true;

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
