void WeightHisto(const char *InputFilenameBase, const char *OutputFilenameBase, double Emax, bool chopState)
{
    std::string InFile = InputFilenameBase;
    std::string InFileEvent = InputFilenameBase;
    if(chopState)
    {
      InFileEvent = InFileEvent + "_EventCheckOn.root";  
    }
    else
    {
      InFileEvent = InFileEvent + "_EventCheckOff.root";
    }
    InFile = InFile + ".root";
    const char* InputFilename = InFile.c_str();
    const char* InputFilenameEvent = InFileEvent.c_str();
    
    TFile *f = TFile::Open(InputFilename);
    TFile *f1 = TFile::Open(InputFilenameEvent);
    
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
    f->GetObject("Cherenkov",Cherenkov);
    f->GetObject("DetInfo",DetInfo);
    f1->cd();
    f1->GetObject("nrf_to_cher_tree",nrf_to_cher_tree);
    f1->GetObject("nrf_to_cher_to_det_tree",nrf_to_cher_to_det_tree);

    ChopIn->SetEstimate(-1);
    ChopOut->SetEstimate(-1);
    NRFMatData->SetEstimate(-1);
    Cherenkov->SetEstimate(-1);
    DetInfo->SetEstimate(-1);
    
    Int_t n = ChopIn->Draw("Energy:Weight","","goff");
    std::cout << "The chopper's incident array size is: " << n << std::endl;
    Double_t *energyIn = ChopIn->GetVal(0);
    Double_t *weightIn = ChopIn->GetVal(1);
    for(int i=0;i<n;i++)
    {
        wChopIn->Fill(energyIn[i],weightIn[i]);
    }

    Int_t n1 = ChopOut->Draw("Energy:Weight","","goff");
    std::cout << "The chopper's emission array size is: " << n1 << std::endl;
    Double_t *energyOut = ChopOut->GetVal(0);
    Double_t *weightOut = ChopOut->GetVal(1);
    for(int i=0;i<n1;i++)
    {
        wChopOut->Fill(energyOut[i],weightOut[i]);
    }
    
    Int_t n2 = NRFMatData->Draw("Energy:Weight","","goff");
    std::cout << "The NRF array size is: " << n2 << std::endl;
    Double_t *nrfEnergy = NRFMatData->GetVal(0);
    Double_t *nrfWeight = NRFMatData->GetVal(1);
    for(int i=0;i<n2;i++)
    {
        wNRF->Fill(nrfEnergy[i],nrfWeight[i]);
    }

    Int_t n3 = Cherenkov->Draw("Energy:Weight","","goff");
    std::cout << "The Cherenkov array size is: " << n3 << std::endl;
    Double_t *cherEnergy = Cherenkov->GetVal(0);
    Double_t *cherWeight = Cherenkov->GetVal(1);
    for(int i=0;i<n3;i++)
    {
        wCherenkov->Fill(cherEnergy[i],cherWeight[i]);
    }
    
    Int_t n4 = DetInfo->Draw("Energy:Weight","","goff");
    std::cout << "The Det array size is: " << n4 << std::endl;
    Double_t *detEnergy = DetInfo->GetVal(0);
    Double_t *detWeight = DetInfo->GetVal(1);
    for(int i=0;i<n4;i++)
    {
        wDet->Fill(detEnergy[i],detWeight[i]);
    }
    
    // Write Weighted Histograms to file
    std::string FinalOutName = OutputFilenameBase;
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
    wCherenkov->Write();
    wDet->Write();
    std::cout << "Weighted Histograms saved to: " << OutputFilename << std::endl;
    
}
