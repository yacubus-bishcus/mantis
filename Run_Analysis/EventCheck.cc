void EventCheck(const char *InputFilename, const char *OutFilenameBase, bool chopState)
{
    TFile *f = TFile::Open(InputFilename);
    TTree *Cherenkov, *NRF;
    std::vector<double> cherEventv, nrfEventv, Events, nrfEnergyv, nrfWeightv;
    double x,y,nrfE,nrfW;
    
    TTree *SameEvents = new TTree("SameEvents","NRF Events that Lead to Cherenkov");
    SameEvents->Branch("EventID",&y);
    SameEvents->Branch("NRF_Energy",&nrfE);
    SameEvents->Branch("NRF_Weight",&nrfW);
    
    f->GetObject("Cherenkov",Cherenkov);
    f->GetObject("NRFMatData",NRF);
    
    Int_t n = Cherenkov->Draw("EventID","","goff");
    Double_t *cherEvent = Cherenkov->GetVal(0);
    
    for(int i=0;i<n;i++)
    {
        cherEventv.push_back(cherEvent[i]);
    }
    
    Int_t n1 = NRF->Draw("EventID:Energy:Weight","","goff");
    Double_t *nrfEvent = NRF->GetVal(0);
    Double_t *nrfEnergy = NRF->GetVal(1);
    Double_t *nrfWeight = NRF->GetVal(2);

    for(int i=0;i<n1;i++)
    {
        nrfEventv.push_back(nrfEvent[i]);
    }

    if(n1 < n)
    {
        for(int i=0;i<n1;i++)
        {
            x = nrfEventv[i];
            auto exists = std::find(cherEventv.begin(),cherEventv.end(), x);
            
            if(exists != cherEventv.end())
            {
                int index = exists - cherEventv.begin();
                Events.push_back(x);
                nrfEnergyv.push_back(nrfEnergy[index]);
                nrfWeightv.push_back(nrfWeight[index]);
            }
        }
    }
    else
    {
        for(int i=0;i<n;i++)
        {
            x = cherEventv[i];
            auto exists = std::find(nrfEventv.begin(),nrfEventv.end(), x);
            
            if(exists != nrfEventv.end())
            {
                int index = exists - nrfEventv.begin();
                Events.push_back(x);
                nrfEnergyv.push_back(nrfEnergy[index]);
                nrfWeightv.push_back(nrfWeight[index]);
            }
        }
    }

    for(int i=0;i<Events.size();i++)
    {
        y = Events[i];
        nrfE = nrfEnergyv[i];
        nrfW = nrfWeightv[i];
        SameEvents->Fill();
    }
    std::cout << "Number of Events Found in Both: " << Events.size() << std::endl;
    
    // Write to file
    if(Events.size() > 0)
    {
        std::string FinalOutName = OutFilenameBase;
        FinalOutName = FinalOutName + "_EventCheck";
        if(chopState)
        {
            FinalOutName = FinalOutName + "On.root";
        }
        else
        {
            FinalOutName = FinalOutName + "Off.root";
        }
        const char* OutFilename = FinalOutName.c_str();
        TFile *fout = new TFile(OutFilename,"recreate");
        fout->cd();
        SameEvents->Write();
        std::cout << "Writing Event Data to: " << OutFilename << std::endl;
    }

}
