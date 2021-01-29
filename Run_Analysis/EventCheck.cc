//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File:
// root -b -q 'EventCheck.cc("test", true)'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 2 inputs 
// 1. InputFilename
// 2. InputFile Chopper State 
// 
// This File Scans the Cherenkov Merged File and determines: 
// 1. Check if a NRF event causes cherenkov
// 2. Check if a NRF event was detected
// 3. Check if a Cherenkov event was detected 
// 4. Check if a NRF event lead to Cherenkov which was then detected 
// 
// The script create a new root file with the Following Structure:
// TFile**		test_EventCheckOn.root	
// TFile*		test_EventCheckOn.root

void EventCheck(const char *InputFilenameBase, bool chopState)
{
    time_t timer, timer2, time_start, time_end;
    time_start = std::time(&timer);
    std::string inFile = InputFilenameBase;
    inFile = inFile + ".root";
    const char* InputFilename = inFile.c_str();
    TFile *f = TFile::Open(InputFilename);
    std::string cherInput = InputFilenameBase;
    if(chopState)
    {
        cherInput = cherInput + "_CherenkovMergedOn.root";
    }
    else
    {
        cherInput = cherInput + "_CherenkovMergedOff.root";
    }
    const char* cherInputFile = cherInput.c_str();
    TFile *f1 = TFile::Open(cherInputFile);
    
    TTree *Cherenkov, *NRF, *DetData;
    std::vector<int> cherEventv, nrfEventv, detEventv, nrf_to_cherEvents, cher_to_detEvents, nrf_to_detEvents;
    std::vector<double> cher_to_detEnergies, cher_to_detWeights, cher_to_detTimes;
    std::vector<double> nrf_to_detEnergies, nrf_to_detWeights, nrf_to_detTimes;
    std::vector<double> nrfEnergyv, nrfWeightv, cherEnergyv, cherWeightv;
    std::vector<double> nrf_to_cherenkov_to_detEvents, nrf_to_cherenkov_to_detNRFEnergies, nrf_to_cherenkov_to_detCherEnergies;
    std::vector<double> nrf_to_cherenkov_to_detNRFWeights, nrf_to_cherenkov_to_detCherWeights;
    std::vector<double> nrf_to_cherenkov_to_detNRFTimes, nrf_to_cherenkov_to_detCherTimes;
    double nrfE,nrfW, cherE, cherW, a, energyNRF, energyCher, weightNRF, weightCher, timeNRF, timeCher;
    int x, nrf_cher_EventID, index;
    
    // Set up Output Trees
    TTree *nrf_to_cher_tree = new TTree("nrf_to_cher_tree","NRF Events that Lead to Cherenkov");
    nrf_to_cher_tree->Branch("EventID",&nrf_cher_EventID);
    nrf_to_cher_tree->Branch("NRF_Energy",&nrfE);
    nrf_to_cher_tree->Branch("NRF_Weight",&nrfW);
    nrf_to_cher_tree->Branch("Cher_Energy",&cherE);
    nrf_to_cher_tree->Branch("Cher_Weight",&cherW);
    
    TTree *nrf_to_cher_to_det_tree = new TTree("nrf_to_cher_to_det_tree","NRF Events that Lead to Cherenkov that were Detected");
    nrf_to_cher_to_det_tree->Branch("EventID",&a);
    nrf_to_cher_to_det_tree->Branch("EnergyNRF",&energyNRF);
    nrf_to_cher_to_det_tree->Branch("EnergyCher",&energyCher);
    nrf_to_cher_to_det_tree->Branch("WeightNRF",&weightNRF);
    nrf_to_cher_to_det_tree->Branch("WeightCher",&weightCher);
    nrf_to_cher_to_det_tree->Branch("TimeNRF",&timeNRF);
    nrf_to_cher_to_det_tree->Branch("TimeCher",&timeCher);
    
    // Grab objects
    f1->cd();
    f1->GetObject("Merged",Cherenkov);
    //Cherenkov->Print();
    f->cd();
    f->GetObject("NRFMatData",NRF);
    //NRF->Print();
    f->GetObject("DetInfo",DetData);
    //DetData->Print();
    
    Cherenkov->SetEstimate(-1);
    NRF->SetEstimate(-1);
    DetData->SetEstimate(-1);
    
    std::cout << "EventCheck::Objects Grabbed!" << std::endl;
    
    // ******************************************************************************************************************************** //
    // Variables Declared Objects Set up
    // ******************************************************************************************************************************** //
    
    // ******************************************************************************************************************************** //
    // Grab Events
    // ******************************************************************************************************************************** //
    
    // Grab Cherenkov Events
    Int_t n = Cherenkov->Draw("EventID:Energy:Weight","","goff");
    std::cout << "Cherenkov Entries: " << n << std::endl;
    Double_t *cherEvent = Cherenkov->GetVal(0);
    Double_t *cherEnergy = Cherenkov->GetVal(1);
    Double_t *cherWeight = Cherenkov->GetVal(2);

    for(int i=0;i<n;i++)
    {
      cherEventv.push_back((int)cherEvent[i]);
    }
      
    // Grab NRF Events
    Int_t n1 = NRF->Draw("EventID:Energy:Weight","","goff");
    std::cout << "NRF Entries: " << n1 << std::endl;
  
    Double_t *nrfEvent = NRF->GetVal(0);
    Double_t *nrfEnergy = NRF->GetVal(1);
    Double_t *nrfWeight = NRF->GetVal(2);

    for(int i=0;i<n1;i++)
    {
        nrfEventv.push_back((int)nrfEvent[i]);
    }
    
    
    // Grab DetInfo Events
    Int_t n2 = DetData->Draw("EventID:Energy:Weight:Time","DetectionProcess == \"Det\"","goff");
    std::cout << "Total Number of Detected entries: " << n2 << std::endl;
  
    Double_t *detEvent = DetData->GetVal(0);
    Double_t *detEnergy = DetData->GetVal(1);
    Double_t *detWeight = DetData->GetVal(2);
    Double_t *time = DetData->GetVal(3);

    for(int i=0;i<n2;i++)
    {
        detEventv.push_back((int)detEvent[i]);
    }
    
    std::cout << "All Events grabbed." << std::endl;
    
    // ******************************************************************************************************************************** //
    // Determine if NRF Lead to Cherenkov
    // ******************************************************************************************************************************** //
    
    if(n != 0 && n1 != 0)
    {
      std::cout << "Finding NRF Events that Caused Cherenkov..." << std::endl;
      // if Cherenkov has more entries search with nrf events
      if(n1 < n)
      {
          std::cout << "Cherenkov has more entries than NRF. Searching NRF Events..." << std::endl;
          for(int i=0;i<n1;i++)
          {
              std::cout << "\r Checking Event: \t" << i << std::flush;
              x = nrfEventv[i];
              auto exists = std::find(cherEventv.begin(),cherEventv.end(), x);

              if(exists != cherEventv.end())
              {
                  index = exists - cherEventv.begin();
                  nrf_to_cherEvents.push_back(x);
                  nrfEnergyv.push_back(nrfEnergy[i]);
                  nrfWeightv.push_back(nrfWeight[i]);
                  cherEnergyv.push_back(cherEnergy[index]);
                  cherWeightv.push_back(cherWeight[index]);
              }
          }
      }
      // Else NRF has more entries search with cherenkov events
      else
      {
          std::cout << "NRF has more entries than Cherenkov. Searching Cherenkov Events..." << std::endl;
          for(int i=0;i<n;i++)
          {
              std::cout << "\r Checking Event: \t" << i << std::flush;
              x = cherEventv[i];
              auto exists = std::find(nrfEventv.begin(),nrfEventv.end(), x);

              if(exists != nrfEventv.end())
              {
                  index = exists - nrfEventv.begin();
                  nrf_to_cherEvents.push_back(x);
                  cherEnergyv.push_back(cherEnergy[i]);
                  cherWeightv.push_back(cherWeight[i]);
                  nrfEnergyv.push_back(nrfEnergy[index]);
                  nrfWeightv.push_back(nrfWeight[index]);
              }
          }
      }

      std::cout << "NRF to Cherenkov Number of Events Found: " << nrf_to_cherEvents.size() << std::endl;
    }
    
    // ******************************************************************************************************************************** //
    // Determine if Cherenkov or NRF Lead to Detection Event
    // ******************************************************************************************************************************** //
    if(n2 > 0)
    {
      for(int i=0;i<n2;i++)
      {
          // Check to see if Cherenkov Event ID matches Det Event ID
          std::cout << "\r Checking Event: \t" << i << std::flush;
          x = detEventv[i];
          auto exists = std::find(cherEventv.begin(), cherEventv.end(),x);
          if(exists != cherEventv.end())
          {
              index = exists - cherEventv.begin();
              cher_to_detEvents.push_back(x);
              cher_to_detEnergies.push_back(detEnergy[index]);
              cher_to_detWeights.push_back(detWeight[index]);
              cher_to_detTimes.push_back(time[index]);
          }
          // Check to see if NRF Event ID matches Det Event ID
          exists = std::find(nrfEventv.begin(),nrfEventv.end(),x);
          if(exists != nrfEventv.end())
          {
              index = exists - nrfEventv.begin();
              nrf_to_detEvents.push_back(x);
              nrf_to_detEnergies.push_back(detEnergy[index]);
              nrf_to_detWeights.push_back(detWeight[index]);
              nrf_to_detTimes.push_back(time[index]);
          }
      }

      std::cout << "Cherenkov Events lead to Detection: " << cher_to_detEvents.size() << std::endl;
      std::cout << "NRF Events Lead to Detection: " << nrf_to_detEvents.size() << std::endl;
    
    
      // ******************************************************************************************************************************** //
      // Determine if NRF and Cherenkov lead to Detection Event
      // ******************************************************************************************************************************** //

      // Check to see if cherenkov detected ID and nrf detected ID match
      // Check if more cherenkov events of nrf events lead to detection if cherenkov > nrf then look at nrf->Detected events
      if(cher_to_detEvents.size() > nrf_to_detEvents.size())
      {
          std::cout << "More Cherenkov Events than nrf events lead to detection. Checking NRF Detected Events..." << std::endl;
          for(int i=0;i<nrf_to_detEvents.size();i++)
          {
              std::cout << "\r Checking Event: \t" << i << std::flush;
              x = nrf_to_detEvents[i];
              auto exists = std::find(cher_to_detEvents.begin(), cher_to_detEvents.end(),x);
              if(exists != cher_to_detEvents.end())
              {
                  index = exists - cher_to_detEvents.begin();
                  nrf_to_cherenkov_to_detEvents.push_back(x);
                  nrf_to_cherenkov_to_detNRFEnergies.push_back(nrf_to_detEnergies[i]);
                  nrf_to_cherenkov_to_detCherEnergies.push_back(cher_to_detEnergies[index]);
                  nrf_to_cherenkov_to_detNRFWeights.push_back(nrf_to_detWeights[i]);
                  nrf_to_cherenkov_to_detCherWeights.push_back(cher_to_detWeights[index]);
                  nrf_to_cherenkov_to_detNRFTimes.push_back(nrf_to_detTimes[i]);
                  nrf_to_cherenkov_to_detCherTimes.push_back(cher_to_detTimes[index]);
              }
          }
      }
      else if(nrf_to_detEvents.size() > cher_to_detEvents.size())
      {
          std::cout << "More NRF Events than Cherenkov events lead to detection. Checking Cherenkov Events..." << std::endl;
          for(int i=0;i<cher_to_detEvents.size();i++)
          {
              std::cout << "\r Checking Event: \t" << i << std::flush;
              x = cher_to_detEvents[i];
              auto exists = std::find(nrf_to_detEvents.begin(), nrf_to_detEvents.end(),x);
              if(exists != nrf_to_detEvents.end())
              {
                  index = exists - nrf_to_detEvents.begin();
                  nrf_to_cherenkov_to_detEvents.push_back(x);
                  nrf_to_cherenkov_to_detNRFEnergies.push_back(nrf_to_detEnergies[index]);
                  nrf_to_cherenkov_to_detCherEnergies.push_back(cher_to_detEnergies[i]);
                  nrf_to_cherenkov_to_detNRFWeights.push_back(nrf_to_detWeights[index]);
                  nrf_to_cherenkov_to_detCherWeights.push_back(cher_to_detWeights[i]);
                  nrf_to_cherenkov_to_detNRFTimes.push_back(nrf_to_detTimes[index]);
                  nrf_to_cherenkov_to_detCherTimes.push_back(cher_to_detTimes[i]);
              }
          }
      }

      std::cout << "NRF Events Leading to Cherenkov Leading to Detection: " << nrf_to_cherenkov_to_detEvents.size() << std::endl;
    }
    
    // ******************************************************************************************************************************** //
    // Fill TTrees
    // ******************************************************************************************************************************** //
    if(n > 0 && n1 > 0)
    {
        std::cout << "Filling NRF to Cherenkov Tree..." << std::endl;
      // Fill nrf_to_cher_tree
      for(int i=0;i<nrf_to_cherEvents.size();i++)
      {
          nrf_cher_EventID = nrf_to_cherEvents[i];
          nrfE = nrfEnergyv[i];
          nrfW = nrfWeightv[i];
          cherE = cherEnergyv[i];
          cherW = cherWeightv[i];
          nrf_to_cher_tree->Fill();
      }
    }
    
    if(n2 > 0)
    {
        std::cout << "Filling NRF to Cherenkov to Detected Tree..." << std::endl;
      // Fill nrf_to_cher_to_det_tree
      for(int i=0;i<nrf_to_cherenkov_to_detEvents.size();i++)
      {
          a = nrf_to_cherenkov_to_detEvents[i];
          energyNRF = nrf_to_cherenkov_to_detNRFEnergies[i];
          energyCher = nrf_to_cherenkov_to_detCherEnergies[i];
          weightNRF = nrf_to_cherenkov_to_detNRFWeights[i];
          weightCher = nrf_to_cherenkov_to_detCherWeights[i];
          timeNRF = nrf_to_cherenkov_to_detNRFTimes[i];
          timeCher = nrf_to_cherenkov_to_detCherTimes[i];
          nrf_to_cher_to_det_tree->Fill();
      }
    }
    
    // ******************************************************************************************************************************** //
    // Write TTrees to OutFile
    // ******************************************************************************************************************************** //
    
    // Write to file
    std::string FinalOutName = InputFilenameBase;
    FinalOutName = FinalOutName + "_NRF_to_Cher";
    std::string FinalOutName2 = FinalOutName + "_NRF_to_Cher_to_Det";
    if(chopState)
    {
        FinalOutName = FinalOutName + "On.root";
        FinalOutName2 = FinalOutName2 + "On.root";
    }
    else
    {
        FinalOutName = FinalOutName + "Off.root";
        FinalOutName2 = FinalOutName2 + "Off.root";
    }
    const char* OutFilename = FinalOutName.c_str();
    const char* OutFilename2 = FinalOutName2.c_str();
    TFile *fout = new TFile(OutFilename,"recreate");
    fout->cd();
    
    if(nrf_to_cherEvents.size() > 0)
    {
        nrf_to_cher_tree->Write();
        std::cout << "NRF to Cherenkov Events Written to file: " << OutFilename << std::endl;
    }
    else
    {
        std::cout << OutFilename << " not written to File!" << std::endl << std::endl;
    }
    fout->Close();
    TFile *fout2 = new TFile(OutFilename2,"recreate");
    fout2->cd();
    
    if(nrf_to_cherenkov_to_detEvents.size() > 0)
    {
        nrf_to_cher_to_det_tree->Write();
        std::cout << "NRF to Cherenkov to Detected Events Written to file: " << OutFilename << std::endl;
    }
    else
    {
        std::cout << OutFilename2 << " not written to File!" << std::endl << std::endl;
    }
        
    fout2->Close();
    time_end = std::time(&timer2);
    std::cout << "Event Check took: " << std::difftime(time_end, time_start) << " seconds!" << std::endl;

}
