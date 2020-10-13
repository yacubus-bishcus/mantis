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
    #if defined (G4ANALYSIS_USE_ROOT)

        TFile *fin = TFile::Open("brems_distributions.root");
        hBrems  = (TH1D*) fin->Get("hBrems");
        if (hBrems)
        {
            G4cout << "Imported brems from " << fin->GetName() << G4endl << G4endl;
            xmax = hBrems->GetXaxis()->GetXmax();
            G4cout << "Found X Max: " << xmax << G4endl;
            fin->Close();
        }
        else
        {
                std::cerr << "Error reading from file " << fin->GetName() << std::endl;
                exit(1);
        }
    #else
        XMax = 5; // this is really just a placeholder
    #endif
    G4int nbins = 10000;
    manager->SetVerboseLevel(0);
    manager->SetNtupleMerging(true);
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

    // Create 0 Histogram for Chopper Data
      manager->CreateH1("ChopData","Chopper Data",nbins,0., xmax, "MeV");

    // Create 1 Histogram for Interogation Object
      manager->CreateH1("IncObj","Incident Interrogation Obj", nbins, 0., xmax, "MeV");

    // Create 2 Histogram for incident water data
      manager->CreateH1("IncWater","NRF Incident Water", nbins, 0., xmax, "MeV");

      // Create 3 Histogram for Incident Detector Data
      manager->CreateH1("IncDet","Incident Photocathode", nbins, 0., xmax/2, "MeV");

      // Create 4 Histogram for Energy if detected
      manager->CreateH1("Det","Detected Spectrum", nbins, 0., 1E-5, "MeV");

      // Create 0 Ntuple for Reactions within detector
      manager->CreateNtuple("DetPro","Detector Processes");
      manager->CreateNtupleSColumn("Process");
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
