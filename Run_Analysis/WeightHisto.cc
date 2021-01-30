//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File:
// root -b -q 'WeightHisto.cc("test", 2.1, true)'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 3 inputs 
// 1. InputFilename
// 2. Max Energy of Bremsstrahlung Interrogation Object
// 3. InputFile Chopper State 
// 
// This File Scans the Cherenkov Merged File and the EventCheck File. 
// This file outputs all histograms that require weighting to 
// a new root file with the Following Structure:
// TFile**		test_WeightedHistogramOn.root	
// TFile*		test_WeightedHistogramOn.root	
//  KEY: TH1D	wChopIn;1	Weighted Chopper Incident
//  KEY: TH1D	wChopOut;1	Weighted Chopper Emission
//  KEY: TH1D	wNRF;1	Weighted NRF
//  KEY: TH1D	wCher;1	Cherenkov Weighted Energy
//  KEY: TH1D	wDet;1	Weighted Detector
//

bool WeightHisto(const char *InputFilenameBase, double Emax, bool chopState)
{
    TFile *f1, *f2, *f3;
    time_t timer, timer2, time_start, time_end;
    time_start = std::time(&timer);
    std::string InFile = InputFilenameBase;
    std::string InFileEvent, InFileEvent2, InFileCherMerged;
    bool checkNRF_to_Cher, checkNRF_to_Cher_to_Det, checkCherenkov;
    bool tcheck = false;
    
    if(chopState)
    {
      InFileEvent = InFile + "_NRF_to_CherOn.root";
      InFileEvent2 = InFile + "_NRF_to_Cher_to_DetOn.root";
      InFileCherMerged = InFile + "_CherenkovMergedOn.root";
    }
    else
    {
      InFileEvent = InFile + "_NRF_to_CherOff.root";
      InFileEvent2 = InFile + "_NRF_to_Cher_to_DetOff.root";
      InFileCherMerged = InFile + "_CherenkovMergedOff.root";
    }
    
    InFile = InFile + ".root";
    const char* InputFilename = InFile.c_str();
    const char* InputFilenameEvent = InFileEvent.c_str();
    const char* InputFilenameEvent2 = InFileEvent2.c_str();
    const char* InputFilenameCher = InFileCherMerged.c_str();
    
    TFile *f = TFile::Open(InputFilename);
    
    if(gSystem->AccessPathName(InputFilenameEvent))
    {
        std::cout << "NRF to Cherenkov Event File does not exist. Skipping..." << std::endl;
        checkNRF_to_Cher = false;
    }
    else
    {
        f1 = TFile::Open(InputFilenameEvent); 
        checkNRF_to_Cher = true;
    }
    if(gSystem->AccessPathName(InputFilenameEvent2))
    {
        std::cout << "NRF to Cherenkov to Detected File does not exist. Skipping..." << std::endl;
        checkNRF_to_Cher_to_Det = false;
    }
    else
    {
        f2 = TFile::Open(InputFilenameEvent2);  
        checkNRF_to_Cher_to_Det = true;
    }
    if(gSystem->AccessPathName(InputFilenameCher))
    {
        std::cout << " Merged Cherenkov File does not exist. Skipping..." << std::endl;
        checkCherenkov = false;
    }
    else
    {
        f3 = TFile::Open(InputFilenameCher);
        checkCherenkov = true;
    }
    
    TTree *ChopIn, *ChopOut, *NRFMatData, *DetInfo;
    TTree *nrf_to_cher_tree, *nrf_to_cher_to_det_tree;
    TH1D *Cherenkov;
    Int_t nbins = 100000;
    
    TH1D *wChopIn = new TH1D("wChopIn","Weighted Chopper Incident",nbins,0.,Emax);
    TH1D *wChopOut = new TH1D("wChopOut","Weighted Chopper Emission",nbins,0.,Emax);
    TH1D *wNRF = new TH1D("wNRF","Weighted NRF",nbins,0.,Emax);
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
    
    if(checkNRF_to_Cher)
    {    
      f1->cd();
      f1->GetObject("nrf_to_cher_tree",nrf_to_cher_tree);
      nrf_to_cher_tree->SetEstimate(-1);
    }
    if(checkNRF_to_Cher_to_Det)
    {
      f2->cd();
      f2->GetObject("nrf_to_cher_to_det_tree",nrf_to_cher_to_det_tree);
      nrf_to_cher_to_det_tree->SetEstimate(-1); 
    }
    if(checkCherenkov)
    {
      f3->cd();
      f3->GetObject("wCher",Cherenkov);
    }

    ChopIn->SetEstimate(-1);
    ChopOut->SetEstimate(-1);
    NRFMatData->SetEstimate(-1);
    DetInfo->SetEstimate(-1);

    std::cout << "WeightHisto::Objects Grabbed!" << std::endl;
    // ******************************************************************************************************************************** //
    // Variables Declared Objects Set up
    // ******************************************************************************************************************************** //
    
    // ******************************************************************************************************************************** //
    // Fill Chopper In Weighted Histogram
    // ******************************************************************************************************************************** //
    
    Int_t n = ChopIn->Draw("Energy:Weight","","goff",10000000);
    Double_t *energyIn = ChopIn->GetVal(0);
    Double_t *weightIn = ChopIn->GetVal(1);
    for(int i=0;i<n;i++)
    {
        wChopIn->Fill(energyIn[i],weightIn[i]);
    }
    std::cout << "Chopper Incident Filled." << std::endl;
    // ******************************************************************************************************************************** //
    // Fill Chopper Out Weighted Histogram
    // ******************************************************************************************************************************** //
    
    Int_t n1 = ChopOut->Draw("Energy:Weight","","goff",10000000);
    Double_t *energyOut = ChopOut->GetVal(0);
    Double_t *weightOut = ChopOut->GetVal(1);
    for(int i=0;i<n1;i++)
    {
        wChopOut->Fill(energyOut[i],weightOut[i]);
    }
    std::cout << "Chopper Emission Filled." << std::endl;
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
    std::cout << "NRF Filled." << std::endl;
    
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
    std::cout << "DetInfo Filled." << std::endl;
    // ******************************************************************************************************************************** //
    // Fill NRF that Lead to Cherenkov Weighted Histogram for NRF Energies and Cherenkov Energies
    // ******************************************************************************************************************************** //
    if(checkNRF_to_Cher)
    {
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
    }

        // ******************************************************************************************************************************** //
        // Fill NRF that Lead to Cherenkov that Lead to Detection Weighted Histogram for NRF Energies and Cherenkov Energies
        // ******************************************************************************************************************************** //
    if(checkNRF_to_Cher_to_Det)
    {
        Int_t n7 = nrf_to_cher_to_det_tree->Draw("EnergyNRF:WeightNRF","","goff");
        Double_t *nrfcherdetNRFEnergy = nrf_to_cher_to_det_tree->GetVal(0);
        Double_t *nrfcherdetNRFWeight = nrf_to_cher_to_det_tree->GetVal(1);
        for(int i=0;i<n7;i++)
        {
            wNRF_NRF_to_Cher_to_Det->Fill(nrfcherdetNRFEnergy[i], nrfcherdetNRFWeight[i]);
        }

        Int_t n8 = nrf_to_cher_to_det_tree->Draw("EnergyCher:WeightCher","","goff");
        Double_t *nrfcherdetCherEnergy = nrf_to_cher_to_det_tree->GetVal(0);
        Double_t *nrfcherdetCherWeight = nrf_to_cher_to_det_tree->GetVal(1);
        for(int i=0;i<n8;i++)
        {
            wCher_NRF_to_Cher_to_Det->Fill(nrfcherdetCherEnergy[i], nrfcherdetCherWeight[i]);
        }
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
    if(checkCherenkov)
    {
      Cherenkov->Write();
    }
    wDet->Write();
    
    if(checkNRF_to_Cher)
    {
        wNRF_NRF_to_Cher->Write();
        wCher_NRF_to_Cher->Write();
    }
    
    if(checkNRF_to_Cher_to_Det)
    {
        wNRF_NRF_to_Cher_to_Det->Write();
        wCher_NRF_to_Cher_to_Det->Write();
    }
    fout->Close();
    std::cout << "Weighted Histograms saved to: " << OutputFilename << std::endl; 
    if(checkNRF_to_Cher && checkNRF_to_Cher_to_Det)
    {
        tcheck = true;
    }
    time_end = std::time(&timer2);
    std::cout << "Weighting Histos took: " << std::difftime(time_end,time_start) << " seconds!" << std::endl;
    return tcheck;
}
