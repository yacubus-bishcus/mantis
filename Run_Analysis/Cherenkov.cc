//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File:
// root -b -q 'Cherenkov.cc("test.root", 2.1, true)'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 3 inputs 
// 1. InputFilename
// 2. Max Energy of Bremsstrahlung Interrogation Beam
// 3. InputFile Chopper State 
// 
// This File takes the TTree from the input file and merges cherenkov events based on the event ID
// The Max energy for a given event is recorded along with that energy's weight
// The script create a new root file with the Following Structure:
// TFile**		test_CherenkovMergedOn.root	
// TFile*		test_CherenkovMergedOn.root	
//  KEY: TTree	Merged;1	Cherenkov Events Merged
//  KEY: TH1D	wCher;1	Cherenkov Weighted Energy


void Cherenkov(const char *InputFilenameBase, double Emax, bool ChopState)
{
    std::string InFile = InputFilenameBase;
    InFile = InFile + ".root";
    const char* InputFilename = InFile.c_str();
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
    Cherenkov->SetEstimate(-1);
    std::cout << "Cherenkov Object Grabbed." << std::endl;
    Int_t numEntries = Cherenkov->Draw("EventID:Energy:Weight:NumSecondaries","","goff");
    Double_t *eventID = Cherenkov->GetVal(0);
    Double_t *energy = Cherenkov->GetVal(1);
    Double_t *weight = Cherenkov->GetVal(2);
    Double_t *sec = Cherenkov->GetVal(3);
    std::vector<double> energyv, weightv, energiesv, weightsv;
    std::vector<int> eventIDv, secv;
    for(int i=0;i<numEntries;i++)
    {
        eventIDv.push_back((int)eventID[i]);
        energyv.push_back(energy[i]);
        weightv.push_back(weight[i]);
        secv.push_back((int)sec[i]);
    }
    std::string theCutString = "EventID == ";
    std::string theString;
    const char* theCut;
    double secSum;
    std::vector<int> events;
    Int_t n = 0;
    Int_t nSum = 0;
    TCut c1;
    
    // ******************************************************************************************************************************** //
    // Variables Declared Objects Set up
    // ******************************************************************************************************************************** //
    std::cout << "Merging Cherenkov with " << numEntries << " number of entries..." << std::endl;
    for(int i=0;i<numEntries;i++)
    {
        std::cout << "\r Event Complete:\t" << i << std::flush;
        if(nSum < numEntries)
        {
            energiesv.clear();
            weightsv.clear();
            secSum = 0;
            int x = eventIDv[i];
            auto it = std::find(eventIDv.begin(),eventIDv.end(),x);
            while(it != eventIDv.end())
            {
                results.emplace_back(std::distance(eventIDv.begin(), it));
                it = std::find(std::next(it), eventIDv.end(), x);
            }
            
            events.push_back((int)eventID[nSum]);
       
            nSum += n;
            
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
    
    // ******************************************************************************************************************************** //
    // Fill New Tree
    // ******************************************************************************************************************************** //
    std::cout << "Filling Cherenkov Merged File..." << std::endl;
    for(int i=0;i<events.size();i++)
    {
        EventID = events[i];
        tEnergy = energyv[i];
        Weight = weightv[i];
        tSec = secv[i];
        newCherenkov->Fill();
        wCher->Fill(tEnergy, Weight);
        
    }
    
    std::cout << "Cherenkov Number of Merged Events: " << events.size() << std::endl;
    
    // ******************************************************************************************************************************** //
    // Write Data to file 
    // ******************************************************************************************************************************** //
    std::cout << "Writing Cherenkov to File..." << std::endl;
    if(events.size() > 0)
    {
        std::string FinalOutFilename = InputFilenameBase;
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
        fout->Close();
    }
}
