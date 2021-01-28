void WeightHisto(const char *InputFilename, const char *OutputFilenameBase, double Emax, bool chopState)
{
    TFile *f = TFile::Open(InputFilename);
    TTree *ChopIn, *ChopOut, *NRFMatData, *Cherenkov, *DetInfo;
    Int_t nbins = 100000;
    TH1D *wChopIn = new TH1D("wChopIn","Weighted Chopper Incident",nbins,0.,Emax);
    TH1D *wChopOut = new TH1D("wChopOut","Weighted Chopper Emission",nbins,0.,Emax);
    TH1D *wNRF = new TH1D("wNRF","Weighted NRF",nbins,0.,Emax);
    TH1D *wCherenkov = new TH1D("wCherenkov","Weighted Cherenkov",nbins,0.,Emax);
    TH1D *wDet = new TH1D("wDet","Weighted Detector",nbins,0.,Emax);
    
    // Grab file objects
    f->GetObject("ChopIn",ChopIn);
    f->GetObject("ChopOut",ChopOut);
    f->GetObject("NRFMatData",NRFMatData);
    f->GetObject("Cherenkov",Cherenkov);
    f->GetObject("DetInfo",DetInfo);

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
