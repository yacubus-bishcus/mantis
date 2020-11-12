// To Run in Root: root -b -q 'PostAnalysis.cc("ChopOn.root",ChopOff.root")'
void PostAnalysis(const char *ChopOn, const char *ChopOff)
{
    std::cout << "Setting Chop On File: " << ChopOn << std::endl;
    std::cout << "Setting Chop Off File: " << ChopOff << std::endl;
    
    TFile *chopOn = new TFile(ChopOn);
    TFile *chopOff = new TFile(ChopOff);
    bool confirmation = chopOn->cd();
    double chopOn_entries;
    double chopOn_sum;
    double chopOn_Energy_Sum_Guess;
    double chopOnBinValue;
    double chopOn_Energy_Counter = 0;
    double chopOn_inc_sum;
    
    double chopOff_entries;
    double chopOff_sum;
    double chopOff_Energy_Sum_Guess;
    double chopOffBinValue;
    double chopOff_Energy_Counter = 0;
    double chopOff_inc_sum;
    Int_t nbins;
    
    // Variables Set, Complete Calculations 
    
    if(confirmation)
    {
        TH1D *Detected;
        TH1D *IncWater;
        chopOn->GetObject("Detected",Detected);
        chopOn->GetObject("IncWater",IncWater);
        
        chopOn_entries = Detected->GetEntries();
        nbins = Detected->GetSize() - 2;
        chopOn_sum = Detected->Integral();
        chopOn_Energy_Sum_Guess = chopOn_entries*Detected->GetMean();
        chopOn_inc_sum = IncWater->Integral();
        for(i=0;i<nbins;i++)
        {
          chopOnBinValue = Detected->GetBinContent(i);
          chopOn_Energy_Counter = chopOn_Energy_Counter + chopOnBinValue;
        }
    }
    else
    {
        std::cout << "ERROR: Chopper On File with name: " << ChopOn << " not opened!!!"<< std::endl;
        return;
    }
    
    bool confirmation2 = chopOff->cd();
    if(confirmation2)
    {
        TH1D *Detected2;
        TH1D *IncWater2;
        chopOff->GetObject("Detected",Detected2);
        chopOff->GetObject("IncWater",IncWater2);
        
        nbins = Detected->GetSize() - 2;
        chopOff_entries = Detected2->GetEntries();
        chopOff_sum = Detected2->Integral();
        chopOff_Energy_Sum_Guess = chopOff_entries*Detected->GetMean();
        chopOff_inc_sum = IncWater2->Integral();
        
        for(i=0;i<nbins;i++)
        {
          chopOffBinValue = Detected->GetBinContent(i);
          chopOff_Energy_Counter = chopOff_Energy_Counter + chopOffBinValue;
        }
    }
    else
    {
        std::cout << "ERROR: Chopper Off File with name: " << ChopOff << " not opened!!!"<<std::endl;
        return;
    }
    
    std::cout << "Files Read Printing Results..." << std::endl;
    
    double per_diff = ((chopOn_sum - chopOff_sum)/chopOn_sum)*100.;
    std::cout << "On Entries: " << chopOn_entries << " On Sum: " << chopOn_sum << " On NRF: " << chopOn_inc_sum << std::endl;
    std::cout << "Off Entries: " << chopOff_entries << " Off Sum: " << chopOff_sum << " Off NRF: " << chopOff_inc_sum << std::endl;
    std::cout << "Weighted Sum Percent Difference: " << per_diff << " %" << std::endl;
    std::cout << "Estimated Chopper On Total Energy Deposited: " << chopOn_Energy_Sum_Guess << std::endl;
    std::cout << "Estimated Chopper Off Total Energy Deposited: " << chopOff_Energy_Sum_Guess << std::endl;
    std::cout << "Summing Method Chopper On Total Energy Deposited: " << chopOn_Energy_Counter << std::endl;
    std::cout << "Summing Method Chopper Off Total Energy Deposited: " << chopOff_Energy_Counter << std::endl;
    
}
