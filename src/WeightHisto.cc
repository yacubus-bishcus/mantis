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



#include "WeightHisto.hh"

#include "TROOT.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"

extern G4String root_output_name;
extern G4String gOutName;

WeightHisto::WeightHisto(G4double Emax)
{
    time_start = std::time(&timer);
    std::string cher_to_nrf_infile = gOutName + "_NRF_to_Cher.root";
    std::string to_det_infile = gOutName + "_NRF_to_Cher_to_Det.root";
    if(gSystem->AccessPathName(cher_to_nrf_infile.c_str()))
    {
        std::cerr << "File: " << cher_to_nrf_infile << " NOT FOUND!" << std::endl;
        return;
    }
    else
        f = new TFile(cher_to_nrf_infile.c_str());
    
    if(gSystem->AccessPathName(to_det_infile.c_str()))
    {
        std::cerr << "File: " << to_det_infile << " NOT FOUND!" << std::endl;
        return;
    }
    else
        f1 = new TFile(to_det_infile.c_str());
    
    // Set up Output Histograms
    wNRF_NRF_to_Cher = new TH1D("wNRF_NRF_to_Cher","Weighted NRF Spectrum that Lead to Cherenkov",100000, 0., Emax);
    wCher_NRF_to_Cher = new TH1D("wCher_NRF_to_Cher","Weighted Cherenkov Spectrum caused by NRF",100000,0.,Emax);
    wNRF_to_Det = new TH1D("wNRF_to_Det","Weighted NRF Energy Spectrum that Lead to Cherenkov that Lead to Detection",100000,0.,Emax);
    wCher_to_Det = new TH1D("wCher_to_Det","Weighted Cherenkov Energy Spectrum Caused by NRF that Lead to Detection", 100000, 0., Emax);
    bool confirm = f->cd();
    if(confirm)
    {
        f->GetObject("nrf_to_cher_tree",nrf_to_cher_tree);
        nrf_to_cher_tree->SetEstimate(-1);
    }
    else
        return;
    confirm = f1->cd();
    if(confirm)
    {
        f1->GetObject("nrf_to_cher_to_det_tree",nrf_to_cher_to_det_tree);
        nrf_to_cher_to_det_tree->SetEstimate(-1);
    }
    else
        return;
    std::cout << "WeightHisto::WeightHisto --> Objects Obtained." << std::endl;
    
}

WeightHisto::~WeightHisto()
{}

// ******************************************************************************************************************************** //
// Fill NRF that Lead to Cherenkov Weighted Histogram for NRF Energies and Cherenkov Energies
// ******************************************************************************************************************************** //

void WeightHisto::Fill_NRF_to_Cherenkov()
{
    G4int n_entries = nrf_to_cher_tree->Draw("NRF_Energy:NRF_Weight","","goff");
    G4double *nrfcherNRFEnergy = nrf_to_cher_tree->GetVal(0);
    G4double *nrfcherNRFWeight = nrf_to_cher_tree->GetVal(1);
    
    for(int i=0;i<n_entries;++i)
    {
        wNRF_NRF_to_Cher->Fill(nrfcherNRFEnergy[i], nrfcherNRFWeight[i]);
    }

    n_entries = nrf_to_cher_tree->Draw("Cher_Energy:Cher_Weight","","goff");
    G4double *nrfcherCherEnergy = nrf_to_cher_tree->GetVal(0);
    G4double *nrfcherCherWeight = nrf_to_cher_tree->GetVal(1);
    
    for(int i=0;i<n_entries;++i)
    {
        wCher_NRF_to_Cher->Fill(nrfcherCherEnergy[i], nrfcherCherWeight[i]);
    }
}

// ******************************************************************************************************************************** //
 // Fill NRF that Lead to Cherenkov that Lead to Detection Weighted Histogram for NRF Energies and Cherenkov Energies
 // ******************************************************************************************************************************** //

void WeightHisto::Fill_to_Det()
{
    G4int n_entries = nrf_to_cher_to_det_tree->Draw("EnergyNRF:WeightNRF","","goff");
    G4double *nrfcherdetNRFEnergy = nrf_to_cher_to_det_tree->GetVal(0);
    G4double *nrfcherdetNRFWeight = nrf_to_cher_to_det_tree->GetVal(1);
    
    for(int i=0;i<n_entries;++i)
    {
        wNRF_to_Det->Fill(nrfcherdetNRFEnergy[i], nrfcherdetNRFWeight[i]);
    }

    n_entries = nrf_to_cher_to_det_tree->Draw("EnergyCher:WeightCher","","goff");
    G4double *nrfcherdetCherEnergy = nrf_to_cher_to_det_tree->GetVal(0);
    G4double *nrfcherdetCherWeight = nrf_to_cher_to_det_tree->GetVal(1);
    
    for(int i=0;i<n_entries;++i)
    {
        wCher_to_Det->Fill(nrfcherdetCherEnergy[i], nrfcherdetCherWeight[i]);
    }
}

// ******************************************************************************************************************************** //
// Write Weighted Histograms to File
// ******************************************************************************************************************************** //

void WeightHisto::Write()
{
    std::string fileOut = gOutName + "_WeightedHisto.root";
    fout = new TFile(fileOut.c_str(), "recreate");
    bool confirm = fout->cd();
    if(confirm)
    {
        wNRF_NRF_to_Cher->Write();
        wCher_NRF_to_Cher->Write();
        wNRF_to_Det->Write();
        wCher_to_Det->Write();
    }
    else
        return;
    
    fout->Close();
    std::cout << "Weighted Histograms written to: " << fileOut << std::endl;
    time_end = std::time(&timer2);
    std::cout << "Weighting Histos took: " << std::difftime(time_end,time_start) << " seconds!" << std::endl;
}
