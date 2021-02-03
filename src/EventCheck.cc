//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Author:
// Jacob E Bickus, 2021
// MIT, NSE
// jbickus@mit.edu
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////
//
// File Explanation:
//
// This File Scans the Cherenkov Merged File and determines:
// 1. Check if a NRF event causes cherenkov
// 2. Check if a NRF event was detected
// 3. Check if a Cherenkov event was detected
// 4. Check if a NRF event lead to Cherenkov which was then detected
//
// The script create a new root file with the Following Structure:
// TFile**        test_EventCheck.root
// TFile*        test_EventCheck.root

#include "EventCheck.hh"

extern G4String root_output_name;
extern G4String gOutName;
EventCheck::EventCheck()
{
    G4int cher_to_detSecSum;
    time_start = std::time(&timer);
    
    if(gSystem->AccessPathName(root_output_name.c_str()))
    {
        std::cerr << "File: " << root_output_name << " does not exist." << std::endl;
        return;
    }
    else
    {
        G4cout << "File: " << root_output_name << " exists." << std::endl;
    }
    f = new TFile(root_output_name.c_str());
    bool confirm = f->cd();
    if(confirm)
    {
        //Cherenkov->Print();
        f->GetObject("Cherenkov",Cherenkov);
        //Cherenkov->Print();
        f->GetObject("NRFMatData",NRF);
        //NRF->Print();
        f->GetObject("DetInfo",DetData);
        //DetData->Print();
    
        Cherenkov->SetEstimate(-1);
        NRF->SetEstimate(-1);
        DetData->SetEstimate(-1);
    }
    
    G4cout << "EventCheck::Objects Grabbed!" << G4endl;

    // Set up Output Trees
    nrf_to_cher_tree = new TTree("nrf_to_cher_tree","NRF Events that Lead to Cherenkov");
    nrf_to_cher_tree->Branch("EventID",&nrf_cher_EventID);
    nrf_to_cher_tree->Branch("NRF_Energy",&nrfE);
    nrf_to_cher_tree->Branch("NRF_Weight",&nrfW);
    nrf_to_cher_tree->Branch("Cher_Energy",&cherE);
    nrf_to_cher_tree->Branch("Cher_Weight",&cherW);
    
    nrf_to_cher_to_det_tree = new TTree("nrf_to_cher_to_det_tree","NRF Events that Lead to Cherenkov that were Detected");
    nrf_to_cher_to_det_tree->Branch("EventID",&a);
    nrf_to_cher_to_det_tree->Branch("EnergyNRF",&energyNRF);
    nrf_to_cher_to_det_tree->Branch("EnergyCher",&energyCher);
    nrf_to_cher_to_det_tree->Branch("WeightNRF",&weightNRF);
    nrf_to_cher_to_det_tree->Branch("WeightCher",&weightCher);
    nrf_to_cher_to_det_tree->Branch("TimeNRF",&timeNRF);
    nrf_to_cher_to_det_tree->Branch("TimeCher",&timeCher);
 
    // Grab Cherenkov Events
    num_entries = Cherenkov->Draw("EventID:Energy:Weight:NumSecondaries","","goff");
    G4cout << "Cherenkov Entries: " << num_entries << G4endl;
    G4double *cherEvent = Cherenkov->GetVal(0);
    G4double *cherEnergy = Cherenkov->GetVal(1);
    G4double *cherWeight = Cherenkov->GetVal(2);
    G4double *cherSecondaries = Cherenkov->GetVal(3);

    for(int i=0;i<num_entries;++i)
    {
      cherEventv.push_back((int)cherEvent[i]);
    }

    // Grab NRF Events
    num_entries1 = NRF->Draw("EventID:Energy:Weight","","goff");
    G4cout << "NRF Entries: " << num_entries1 << G4endl;
    G4double *nrfEvent = NRF->GetVal(0);
    G4double *nrfEnergy = NRF->GetVal(1);
    G4double *nrfWeight = NRF->GetVal(2);

    for(int i=0;i<num_entries1;++i)
    {
        nrfEventv.push_back((int)nrfEvent[i]);
    }
    
    // Grab DetInfo Events
    num_entries2 = DetData->Draw("EventID:Energy:Weight:Time","DetectionProcess == \"Det\" && (CreatorProcess == \"Scintillation\" || CreatorProcess == \"Cerenkov\")","goff");
    G4cout << "Total Number of Detected entries: " << num_entries2 << G4endl << G4endl;
    G4double *detEvent = DetData->GetVal(0);
    G4double *detEnergy = DetData->GetVal(1);
    G4double *detWeight = DetData->GetVal(2);
    G4double *time = DetData->GetVal(3);

    for(int i=0;i<num_entries2;++i)
    {
        detEventv.push_back((int)detEvent[i]);
    }
    
    if(num_entries != 0 && num_entries1 != 0)
    {
      G4cout << "Finding NRF Events that Caused Cherenkov..." << G4endl;
      // if Cherenkov has more entries search with nrf events
      if(num_entries1 < num_entries)
      {
          for(int i=0;i<num_entries1;++i)
          {
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
          for(int i=0;i<num_entries;++i)
          {
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
      G4cout << G4endl << "NRF to Cherenkov Number of Events Found: " << nrf_to_cherEvents.size() << G4endl;
    }
    
    if(num_entries2 > 0)
    {
      for(int i=0;i<num_entries2;++i)
      {
          // Check to see if Cherenkov Event ID matches Det Event ID
          x = detEventv[i];
          auto exists = std::find(cherEventv.begin(), cherEventv.end(),x);

          if(exists != cherEventv.end())
          {
              index = exists - cherEventv.begin();
              cher_to_detEvents.push_back(x);
              cher_to_detEnergies.push_back(detEnergy[index]);
              cher_to_detWeights.push_back(detWeight[index]);
              cher_to_detTimes.push_back(time[index]);
              cher_to_detSecSum += (int)cherSecondaries[index];
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

      G4cout << G4endl << "Cherenkov Events lead to Detection: " << cher_to_detEvents.size() << G4endl;
      G4cout << "Total Number of Cherenkov Photons that lead to Detection: " << cher_to_detSecSum << G4endl;
      G4cout << "NRF Events Lead to Detection: " << nrf_to_detEvents.size() << G4endl;
    
      // ******************************************************************************************************************************** //
      // Determine if NRF and Cherenkov lead to Detection Event
      // ******************************************************************************************************************************** //

      // Check to see if cherenkov detected ID and nrf detected ID match
      // Check if more cherenkov events of nrf events lead to detection if cherenkov > nrf then look at nrf->Detected events
      if(cher_to_detEvents.size() > nrf_to_detEvents.size())
      {
          G4cout << "More Cherenkov Events than nrf events lead to detection. Checking NRF Detected Events..." << G4endl;
          for(unsigned int i=0;i<nrf_to_detEvents.size();++i)
          {
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
          G4cout << "More NRF Events than Cherenkov events lead to detection. Checking Cherenkov Events..." << G4endl;
          for(unsigned int i=0;i<cher_to_detEvents.size();++i)
          {
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

      G4cout << G4endl << "NRF Events Leading to Cherenkov Leading to Detection: " << nrf_to_cherenkov_to_detEvents.size() << G4endl;
    }
}
EventCheck::~EventCheck()
{}

// ******************************************************************************************************************************** //
// Fill TTrees
// ******************************************************************************************************************************** //

void EventCheck::WriteEvents()
{
    if(num_entries > 0 && num_entries1 > 0)
    {
        G4cout << "Filling NRF to Cherenkov Tree..." << G4endl;
      // Fill nrf_to_cher_tree
      for(unsigned int i=0;i<nrf_to_cherEvents.size();i++)
      {
          nrf_cher_EventID = nrf_to_cherEvents[i];
          nrfE = nrfEnergyv[i];
          nrfW = nrfWeightv[i];
          cherE = cherEnergyv[i];
          cherW = cherWeightv[i];
          nrf_to_cher_tree->Fill();
      }
    }
    
    if(num_entries2 > 0)
    {
        G4cout << "Filling NRF to Cherenkov to Detected Tree..." << G4endl;
      // Fill nrf_to_cher_to_det_tree
      for(unsigned int i=0;i<nrf_to_cherenkov_to_detEvents.size();i++)
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
    std::string FinalOutName = gOutName;
    std::string FinalOutName2 = gOutName;
    FinalOutName = FinalOutName + "_NRF_to_Cher.root";
    FinalOutName2 = FinalOutName2 + "_NRF_to_Cher_to_Det.root";
    const char* OutFilename = FinalOutName.c_str();
    const char* OutFilename2 = FinalOutName2.c_str();
    
    fout = new TFile(OutFilename,"recreate");
    fout->cd();
    
    if(nrf_to_cherEvents.size() > 0)
    {
        nrf_to_cher_tree->Write();
        G4cout << "NRF to Cherenkov Events Written to file: " << OutFilename << G4endl;
    }
    else
    {
        G4cout << OutFilename << " not written to File!" << G4endl << G4endl;
    }
    fout->Close();
    
    fout2 = new TFile(OutFilename2,"recreate");
    fout2->cd();
    
    if(nrf_to_cherenkov_to_detEvents.size() > 0)
    {
        nrf_to_cher_to_det_tree->Write();
        G4cout << "NRF to Cherenkov to Detected Events Written to file: " << OutFilename2 << G4endl;
    }
    else
    {
        G4cout << OutFilename2 << " not written to File!" << G4endl << G4endl;
    }
        
    fout2->Close();
    time_end = std::time(&timer2);
    G4cout << "Event Check took: " << std::difftime(time_end, time_start) << " seconds!" << G4endl << G4endl;
}
