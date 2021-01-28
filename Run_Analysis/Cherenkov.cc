void Cherenkov(const char *InputFilename, const char *OutputFilenameBase, double Emax, bool ChopState)
{
    TFile *f = TFile::Open(InputFilename);
    TTree *Cherenkov;
    
    TTree *newCherenkov = new TTree("Merged","Cherenkov Events Merged");
    double tEnergy, Weight, tSec;
    int EventID;
    newCherenkov->Branch("EventID",&EventID);
    newCherenkov->Branch("Energy",&tEnergy);
    newCherenkov->Branch("Weight",&Weight);
    newCherenkov->Branch("Secondaries", &tSec);
    
    TH1D *wCher = new TH1D("wCher","Cherenkov Weighted Energy",10000,0.,Emax);
    
    f->GetObject("Cherenkov",Cherenkov);
    Int_t numEntries = Cherenkov->Draw("EventID","","goff");
    Double_t *eventID = Cherenkov->GetVal(0);
    std::string theCutString;
    const char* theCut;
    double secSum;
    std::vector<double> energyv, weightv, secv, energiesv, weightsv;
    std::vector<int> events;
    Int_t n = 0;
    Int_t nSum = 0;
    TCut c1;
    
    for(int i=0;i<numEntries;i++)
    {
        if(nSum < numEntries)
        {
            energiesv.clear();
            weightsv.clear();
            secSum = 0;
            theCutString = "EventID == ";
            theCutString = theCutString + to_string((int)eventID[nSum]);
            theCut = theCutString.c_str();
            events.push_back((int)eventID[nSum]);
            c1.SetTitle(theCut);
            n = Cherenkov->Draw("Energy:Weight:NumSecondaries",c1,"goff");
            nSum += n;
            Double_t *energy = Cherenkov->GetVal(0);
            Double_t *weight = Cherenkov->GetVal(1);
            Double_t *sec = Cherenkov->GetVal(2);
            
            for(int j=0;j<n;j++)
            {
                secSum += sec[j];
                energiesv.push_back(energy[j]);
                weightsv.push_back(weight[j]);
            }
            double maxE = *std::max_element(energiesv.begin(),energiesv.end());
            energyv.push_back(maxE);
            auto it = std::find(energiesv.begin(), energiesv.end(),maxE);
            int index = it - energiesv.begin();
            weightv.push_back(weightsv[index]);
            secv.push_back(secSum);
        }
        else
        {
            break;
        }
    } // end of for loop
    
    // Fill new TTree
    for(int i=0;i<events.size();i++)
    {
        EventID = events[i];
        tEnergy = energyv[i];
        Weight = weightv[i];
        tSec = secv[i];
        newCherenkov->Fill();
        wCher->Fill(tEnergy, Weight);
        
    }
    
    if(events.size() > 0)
    {
        std::string FinalOutFilename = OutputFilenameBase;
        FinalOutFilename = FinalOutFilename + "_CherenkovMerged";
        if(ChopState)
        {
            FinalOutFilename = FinalOutFilename + "On.root";
        }
        else
        {
            FinalOutFilename = FinalOutFilename + "Off.root";
        }
        const char* OutputFilename = FinalOutFilename.c_str();
        TFile *fout = new TFile(OutputFilename,"recreate");
        fout->cd();
        newCherenkov->Write();
        wCher->Write();
        std::cout << "Cherenkov Merged Event Data saved to: " << OutputFilename << std::endl;
    }
}
