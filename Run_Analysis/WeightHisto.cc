void WeightHisto(const char *InputFilenameBase, double Emax, bool chopState)
{
    std::string InFile = InputFilenameBase;
    std::string InFileEvent = InputFilenameBase;
    std::string InFileCherMerged = InputFilenameBase;
    
    if(chopState)
    {
      InFileEvent = InFileEvent + "_EventCheckOn.root";
      InFileCherMerged = InFileCherMerged + "_CherenkovMergedOn.root";
    }
    else
    {
      InFileEvent = InFileEvent + "_EventCheckOff.root";
      InFileCherMerged = InFileCherMerged + "_CherenkovMergedOff.root";
    }
    InFile = InFile + ".root";
    const char* InputFilename = InFile.c_str();
    const char* InputFilenameEvent = InFileEvent.c_str();
    const char* InputFilenameCher = InFileCherMerged.c_str();
    
    TFile *f = TFile::Open(InputFilename);
    TFile *f1 = TFile::Open(InputFilenameEvent);
    TFile *f2 = TFile::Open(InputFilenameCher);
    
    TTree *ChopIn, *ChopOut, *NRFMatData, *Cherenkov, *DetInfo;
    TTree *nrf_to_cher_tree, *nrf_to_cher_to_det_tree;
    Int_t nbins = 100000;
    
    TH1D *wChopIn = new TH1D("wChopIn","Weighted Chopper Incident",nbins,0.,Emax);
    TH1D *wChopOut = new TH1D("wChopOut","Weighted Chopper Emission",nbins,0.,Emax);
    TH1D *wNRF = new TH1D("wNRF","Weighted NRF",nbins,0.,Emax);
    TH1D *wCherenkov = new TH1D("wCherenkov","Weighted Cherenkov",nbins,0.,Emax);
    TH1D *wDet = new TH1D("wDet","Weighted Detector",nbins,0.,Emax);
    TH1D *wNRF_NRF_to_Cher = new TH1D("wNRF_NRF_to_Cher","Weighted NRF Spectrum that Lead to Cherenkov",nbins, 0., Emax);
    TH1D *wCher_NRF_to_Cher = new TH1D("wCher_NRF_to_Cher","Weighted Cherenkov Spectrum caused by NRF",nbins,0.,Emax);
    TH1D *wNRF_NRF_to_Cher_to_Det = new TH1D("wNRF_NRF_to_Cher_to_Det","Weighted NRF Energy Spectrum that Lead to Cherenkov that Lead to Detection",nbins,0.,Emax);
    TH1D *wCher_NRF_to_Cher_to_Det = new TH1D("wCher_NRF_to_Cher_to_Det","Weighted Cherenkov Energy Spectrum Caused by NRF that Lead to Detection", nbins, 0., Emax);
    
    // Grab file objects
    f->cd();
    f->GetObject("ChopIn",ChopIn);
    f->GetObject("ChopOut",ChopOut);
    f->GetObject("NRFMatData",NRFMatData);
    f->GetObject("DetInfo",DetInfo);
    f1->cd();
    f1->GetObject("nrf_to_cher_tree",nrf_to_cher_tree);
    f1->GetObject("nrf_to_cher_to_det_tree",nrf_to_cher_to_det_tree);
    f2->cd();
    f2->GetObject("wCher",Cherenkov);

    ChopIn->SetEstimate(-1);
    ChopOut->SetEstimate(-1);
    NRFMatData->SetEstimate(-1);
    DetInfo->SetEstimate(-1);
    nrf_to_cher_tree->SetEstimate(-1);
    nrf_to_cher_to_det_tree->SetEstimate(-1);
    
    // ******************************************************************************************************************************** //
    // Variables Declared Objects Set up
    // ******************************************************************************************************************************** //
    
    // ******************************************************************************************************************************** //
    // Fill Chopper In Weighted Histogram
    // ******************************************************************************************************************************** //
    
    Int_t n = ChopIn->Draw("Energy:Weight","","goff");
    Double_t *energyIn = ChopIn->GetVal(0);
    Double_t *weightIn = ChopIn->GetVal(1);
    for(int i=0;i<n;i++)
    {
        wChopIn->Fill(energyIn[i],weightIn[i]);
    }

    // ******************************************************************************************************************************** //
    // Fill Chopper Out Weighted Histogram
    // ******************************************************************************************************************************** //
    
    Int_t n1 = ChopOut->Draw("Energy:Weight","","goff");
    Double_t *energyOut = ChopOut->GetVal(0);
    Double_t *weightOut = ChopOut->GetVal(1);
    for(int i=0;i<n1;i++)
    {
        wChopOut->Fill(energyOut[i],weightOut[i]);
    }
    
    // ******************************************************************************************************************************** //
    // Fill NRF Weighted Histogram
    // ******************************************************************************************************************************** //
    
    Int_t n2 = NRFMatData->Draw("Energy:Weight","","goff");
    Double_t *nrfEnergy = NRFMatData->GetVal(0);
    Double_t *nrfWeight = NRFMatData->GetVal(1);
    for(int i=0;i<n2;i++)
    {
        wNRF->Fill(nrfEnergy[i],nrfWeight[i]);
    }

    
    // ******************************************************************************************************************************** //
    // Fill DetInfo Weighted Histogram
    // ******************************************************************************************************************************** //
    
    Int_t n4 = DetInfo->Draw("Energy:Weight","","goff");
    Double_t *detEnergy = DetInfo->GetVal(0);
    Double_t *detWeight = DetInfo->GetVal(1);
    for(int i=0;i<n4;i++)
    {
        wDet->Fill(detEnergy[i],detWeight[i]);
    }
    
    // ******************************************************************************************************************************** //
    // Fill NRF that Lead to Cherenkov Weighted Histogram for NRF Energies and Cherenkov Energies
    // ******************************************************************************************************************************** //
    
    Int_t n5 = nrf_to_cher_tree->Draw("NRF_Energy:NRF_Weight","","goff");
    Double_t *nrfcherNRFEnergy = nrf_to_cher_tree->GetVal(0);
    Double_t *nrfcherNRFWeight = nrf_to_cher_tree->GetVal(1);
    for(int i=0;i<n5;i++)
    {
        wNRF_NRF_to_Cher->Fill(nrfcherNRFEnergy[i], nrfcherNRFWeight[i]);
    }
    
    Int_t n6 = nrf_to_cher_tree->Draw("Cher_Energy:Cher_Weight","","goff");
    Double_t *nrfcherCherEnergy = nrf_to_cher_tree->GetVal(0);
    Double_t *nrfcherCherWeight = nrf_to_cher_tree->GetVal(1);
    for(int i=0;i<n6;i++)
    {
        wCher_NRF_to_Cher->Fill(nrfcherCherEnergy[i], nrfcherCherWeight[i]);
    }
    
    // ******************************************************************************************************************************** //
    // Fill NRF that Lead to Cherenkov that Lead to Detection Weighted Histogram for NRF Energies and Cherenkov Energies
    // ******************************************************************************************************************************** //
    
    Int_t n7 = nrf_to_cher_tree->Draw("EnergyNRF:WeightNRF","","goff");
    Double_t *nrfcherdetNRFEnergy = nrf_to_cher_tree->GetVal(0);
    Double_t *nrfcherdetNRFWeight = nrf_to_cher_tree->GetVal(1);
    for(int i=0;i<n7;i++)
    {
        wNRF_NRF_to_Cher_to_Det->Fill(nrfcherdetNRFEnergy[i], nrfcherdetNRFWeight[i]);
    }
    
    Int_t n8 = nrf_to_cher_tree->Draw("EnergyCher:WeightCher","","goff");
    Double_t *nrfcherdetCherEnergy = nrf_to_cher_tree->GetVal(0);
    Double_t *nrfcherdetCherWeight = nrf_to_cher_tree->GetVal(1);
    for(int i=0;i<n8;i++)
    {
        wCher_NRF_to_Cher_to_Det->Fill(nrfcherdetCherEnergy[i], nrfcherdetCherWeight[i]);
    }
    
    // ******************************************************************************************************************************** //
    // Write Weighted Histograms to File
    // ******************************************************************************************************************************** //
    
    std::string FinalOutName = InputFilenameBase;
    FinalOutName = FinalOutName + "_WeightedHistogram";
    if(chopState)
    {
        FinalOutName = FinalOutName + "On.root";
    }
    else
    {
        FinalOutName = FinalOutName + "Off.root";
    }
    const char* OutputFilename = FinalOutName.c_str();
    TFile *fout = new TFile(OutputFilename,"recreate");
    fout->cd();
    
    wChopIn->Write();
    wChopOut->Write();
    wNRF->Write();
    Cherenkov->Write();
    wDet->Write();
    wNRF_NRF_to_Cher->Write();
    wCher_NRF_to_Cher->Write();
    wNRF_NRF_to_Cher_to_Det->Write();
    wCher_NRF_to_Cher_to_Det->Write();
    fout->Close();
    std::cout << "Weighted Histograms saved to: " << OutputFilename << std::endl;  
}
