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
    time_t timer,timer2, t_start, t_end;
    t_start = time(&timer);
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
    
    // Grab all values and place in arrays 
    Int_t numEntries = Cherenkov->Draw("EventID:Energy:Weight:NumSecondaries","","goff");
    Double_t *eventID = Cherenkov->GetVal(0);
    Double_t *energy = Cherenkov->GetVal(1);
    Double_t *weight = Cherenkov->GetVal(2);
    Double_t *sec = Cherenkov->GetVal(3);
    
    // Final Vectors 
    std::vector<int> events, secv;
    std::vector<double> energyv, weightv;
    // Temp Holding Vectors 
    std::vector<int> eventIDv;
    std::vector<double> energiesv, weightsv;
    std::vector<size_t> results;
    
    for(int i=0;i<numEntries;i++)
    {
        eventIDv.push_back((int)eventID[i]);
    }

    double secSum;
    Int_t n = 0;
    Int_t nSum = 0;
    
    // ******************************************************************************************************************************** //
    // Variables Declared Objects Set up
    // ******************************************************************************************************************************** //
    
    std::cout << "Merging Cherenkov with " << numEntries << " number of entries..." << std::endl;
    for(int i=0;i<numEntries;i++)
    {
        if(nSum < numEntries)
        {
            std::cout << "\r Event: " << nSum << std::flush;
            // clear holding vector/variables 
            energiesv.clear();
            weightsv.clear();
            secSum = 0;
            // grab the next event to check 
            int x = eventIDv[nSum];
            // find where the indices are that match x 
            auto it = std::find(eventIDv.begin(),eventIDv.end(),x);
            while(it != eventIDv.end())
            {
                results.emplace_back(std::distance(eventIDv.begin(), it));
                it = std::find(std::next(it), eventIDv.end(), x);
            }
            // Determine how many indices match x 
            int n = results.size();
            // write the event that has matches into the final event vector 
            events.push_back((int)eventIDv[nSum]);
            // sum the events to skip repeats ... move on to next event ID value 
            nSum += n;
            // Fill the energies, weights and number of secondaries temp vectors 
            for(int j=0;j<n;j++)
            {
                // indices of matching events 
                int index = results[j];
                // sum the number of secondaries of matching events 
                secSum = secSum + sec[index];
                // put all of the energies and weights into temp vector
                energiesv.push_back(energy[index]);
                weightsv.push_back(weight[index]);
            }
            // find the max energy for the event 
            double maxE = *std::max_element(energiesv.begin(),energiesv.end());
            // add the max energy to the final energy vector
            energyv.push_back(maxE);
            // find index where the maxE is 
            auto it2 = std::find(energiesv.begin(), energiesv.end(),maxE);
            int maxEindex = it2 - energiesv.begin();
            // add the weight of the max energy to the final weight vector 
            weightv.push_back(weightsv[maxEindex]);
            // add the total number of secondaries into final secondary vector 
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
    
    std::cout << std::endl << "Filling Cherenkov Merged File..." << std::endl;
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
    t_end = time(&timer2);
    std::cout << "Time to Merge Cherenkov: " << std::difftime(t_end, t_start) << " seconds" << std::endl;
}
