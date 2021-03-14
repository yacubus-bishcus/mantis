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
// This File Scans the Output file and finds events that underwent NRF that lead
// to cherenkov that lead to detection.
//
// The script create a new root file with the Following Structure:
// TFile**        test_EventCheck.root
// TFile*        test_EventCheck.root

#include "EventCheck.hh"
#include "TSystem.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TString.h"
#include "TMath.h"

extern G4String root_output_name;
extern G4String gOutName;
extern G4String inFile;

EventCheck::EventCheck()
{
  time_start = std::time(&timer);

  if(gSystem->AccessPathName(root_output_name.c_str()))
  {
          std::cerr << "EventCheck::EventCheck -> File: " << root_output_name << " does not exist." << std::endl;
          G4cerr << "EventCheck::EventCheck -> File: " << root_output_name << " does not exist." << G4endl;
          return;
  }
  else
  {
          std::cout << "EventCheck::EventCheck -> File: " << root_output_name << " exists." << std::endl;
          G4cout << "EventCheck::EventCheck -> File: " << root_output_name << " exists." << G4endl;
  }

  TFile *f = new TFile(root_output_name.c_str(),"read");

  bool confirm = f->cd();
  if(!confirm)
  {
    G4cerr << "EventCheck::EventCheck -> Could not confirm Output File" << G4endl;
    exit(1);
  }

  TTree *Cherenkov_in, *NRF_in, *DetInfo_in;

  f->GetObject("Cherenkov",Cherenkov_in);
  f->GetObject("NRF",NRF_in);
  f->GetObject("DetInfo",DetInfo_in);

  Cherenkov_in->SetEstimate(-1);
  NRF_in->SetEstimate(-1);
  DetInfo_in->SetEstimate(-1);
  G4cout << "EventCheck::EventCheck -> Objects Grabbed!" << G4endl;

  // Grab NRF Events
  G4int nrf_entries = NRF_in->Draw("EventID","","goff");
  G4cout << "EventCheck::EventCheck -> NRF Entries: " << nrf_entries << G4endl;
  G4double *nrfEvent = NRF_in->GetVal(0);

  std::vector<int> nrfEventv;
  // Write NRF Events to Vector
  for(int i=0; i<nrf_entries; ++i)
  {
    nrfEventv.push_back((int)nrfEvent[i]);
  }

  // Grab Cherenkov Events
  G4int cher_entries = Cherenkov_in->Draw("EventID","","goff");
  G4cout << "EventCheck::EventCheck -> Cherenkov Entries: " << cher_entries << G4endl;
  G4double *cherEvent = Cherenkov_in->GetVal(0);

  std::vector<int> cherEventv;
  // Write Cherenkov Events to vector
  for(int i=0; i<cher_entries; ++i)
  {
    cherEventv.push_back((int)cherEvent[i]);
  }

  // Grab DetInfo Events
  G4int det_entries =0;

  if(!inFile.compare(0,24,"brems_distributions.root"))
    det_entries = DetInfo_in->Draw("EventID:Energy:Time:Weight","CreatorProcess == \"Scintillation\" || CreatorProcess == \"Cerenkov\"","goff");
  else
    det_entries = DetInfo_in->Draw("EventID:Energy:Time","CreatorProcess == \"Scintillation\" || CreatorProcess == \"Cerenkov\"","goff");

  G4cout << "EventCheck::EventCheck -> Total Number of Detected Optical Photon entries: "
          << det_entries << G4endl << G4endl;
  G4double *detEvent = DetInfo_in->GetVal(0);
  G4double *detEnergy = DetInfo_in->GetVal(1);
  G4double *detTime = DetInfo_in->GetVal(2);
  // Dynamically Allocate array for weights
  G4double *detWeight = new G4double[(int)det_entries];

  if(!inFile.compare(0,24,"brems_distributions.root"))
    detWeight = DetInfo_in->GetVal(3);

  std::vector<int> detEventv;
  for(int i=0; i<det_entries; ++i)
  {
    detEventv.push_back((int)detEvent[i]);
  }

  std::vector<int> nrf_to_cherEvents;
  if(nrf_entries != 0 && cher_entries != 0 && det_entries != 0)
  {
    G4cout << "EventCheck::EventCheck -> Finding NRF Events that Caused Optical Photons..." << G4endl;

    for(int i=0; i<nrf_entries; ++i)
    {
      // Grab nrf Event ID
      x = nrfEventv[i];
      // Check if NRF EventID is found in Cherenkov Vector
      auto exists = std::find(cherEventv.begin(),cherEventv.end(), x);

      if(exists != cherEventv.end())
        // if the eventID is found in cherenkov write to new vector
        nrf_to_cherEvents.push_back(x);
    }
    G4cout << "EventCheck::EventCheck -> NRF to Optical Photon Number of Events Found: "
            << nrf_to_cherEvents.size() << G4endl;

    for(unsigned int i=0; i<nrf_to_cherEvents.size(); ++i)
    {
      // Check if nrf->cherenkov event ID is in detected
      x = nrf_to_cherEvents[i];
      auto exists = std::find(detEventv.begin(), detEventv.end(), x);
      if(exists != detEventv.end())
      {
        // if the eventID is found write to new vector
        nrf_to_cher_to_det_event.push_back(x);
        // find the index of the detInfo array
        G4int index = exists - detEventv.begin();
        // write the rest of the detected info to new vectors
        nrf_to_cher_to_det_energy.push_back(detEnergy[index]);

        if(!inFile.compare(0,24,"brems_distributions.root"))
          nrf_to_cher_to_det_weight.push_back(detWeight[index]);

        nrf_to_cher_to_det_time.push_back(detTime[index]);
      }
    }
    G4cout << "EventCheck::EventCheck -> NRF to Optical Photons Detected Number of Events: "
            << nrf_to_cher_to_det_event.size() << G4endl;
  } // end if nrf_entries != 0 && cher_entries != 0

  // deallocate memory
  delete []detWeight;
  f->Close("R");
}

EventCheck::~EventCheck()
{
}

// ******************************************************************************************************************************** //
// Fill TTrees
// ******************************************************************************************************************************** //

void EventCheck::WriteEvents()
{
  if(gSystem->AccessPathName(root_output_name.c_str()))
  {
    std::cerr << "EventCheck::WriteEvents -> File: " << root_output_name << " does not exist." << std::endl;
    G4cerr << "EventCheck::WriteEvents -> File: " << root_output_name << " does not exist." << G4endl;
    return;
  }


  TTree *Brem_in, *ChopIn_in, *ChopOut_in, *NRF_in, *Air_in, *IntObjIn_in, *IntObjOut_in, *Water_in, *Cherenkov_in, *DetInfo_in, *IncDetInfo_in;
  TFile *fout = new TFile(root_output_name.c_str(),"update");
  fout->cd();

  fout->GetObject("Brem",Brem_in);
  fout->GetObject("ChopIn",ChopIn_in);
  fout->GetObject("ChopOut",ChopOut_in);
  fout->GetObject("NRF",NRF_in);
  fout->GetObject("AirIn",Air_in);
  fout->GetObject("IntObjIn", IntObjIn_in);
  fout->GetObject("IntObjOut", IntObjOut_in);
  fout->GetObject("Water",Water_in);
  fout->GetObject("Cherenkov",Cherenkov_in);
  fout->GetObject("DetInfo",DetInfo_in);
  fout->GetObject("IncDetInfo",IncDetInfo_in);

  Brem_in->SetEstimate(-1);
  ChopIn_in->SetEstimate(-1);
  ChopOut_in->SetEstimate(-1);
  NRF_in->SetEstimate(-1);
  Air_in->SetEstimate(-1);
  IntObjIn_in->SetEstimate(-1);
  IntObjOut_in->SetEstimate(-1);
  Water_in->SetEstimate(-1);
  Cherenkov_in->SetEstimate(-1);
  DetInfo_in->SetEstimate(-1);
  IncDetInfo_in->SetEstimate(-1);

  // Set up NRF to Cher to Det Tree
  G4int event;
  G4double energy, weight, thetimes;

  TTree *nrf_to_cher_to_det_tree = new TTree("event_tree","NRF Events that Lead to Cherenkov that were Detected");
  nrf_to_cher_to_det_tree->Branch("EventID",&event);
  nrf_to_cher_to_det_tree->Branch("Energy",&energy);

  if(!inFile.compare(0,24,"brems_distributions.root"))
    nrf_to_cher_to_det_tree->Branch("Weight",&weight);

  nrf_to_cher_to_det_tree->Branch("Time",&thetimes);

  // Fill nrf_to_cher_to_det Tree
  if(nrf_to_cher_to_det_event.size() > 0)
  {
    // Fill tree
    for(unsigned int i=0; i<nrf_to_cher_to_det_event.size(); ++i)
    {
      event = nrf_to_cher_to_det_event[i];
      energy = nrf_to_cher_to_det_energy[i];

      if(!inFile.compare(0,24,"brems_distributions.root"))
        weight = nrf_to_cher_to_det_weight[i];

      thetimes = nrf_to_cher_to_det_time[i];
      nrf_to_cher_to_det_tree->Fill();
    }
  }

// ******************************************************************************************************************************** //
// Write TTrees to OutFile
// ******************************************************************************************************************************** //

  Brem_in->Write();
  ChopIn_in->Write();
  ChopOut_in->Write();
  NRF_in->Write();
  Air_in->Write();
  IntObjIn_in->Write();
  IntObjOut_in->Write();
  Water_in->Write();
  Cherenkov_in->Write();
  DetInfo_in->Write();
  IncDetInfo_in->Write();
  nrf_to_cher_to_det_tree->Write();


  std::cout << "EventCheck::WriteEvents -> TTrees Written to File: "
              << root_output_name << std::endl;
  G4cout << "EventCheck::WriteEvents -> TTrees Written to File: "
              << root_output_name << G4endl;
  fout->Close();
  time_end = std::time(&timer2);
  G4cout << "Event Check took: " << std::difftime(time_end, time_start)
          << " seconds!" << G4endl << G4endl;
}
