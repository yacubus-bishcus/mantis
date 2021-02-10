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

extern G4String root_output_name;
extern G4String gOutName;

WeightHisto::WeightHisto(G4double Emax)
{
        time_start = std::time(&timer);
        std::string cher_to_nrf_infile = gOutName + "_NRF_to_Cher.root";
        std::string to_det_infile = gOutName + "_NRF_to_Cher_to_Det.root";
        if(gSystem->AccessPathName(cher_to_nrf_infile.c_str()))
        {
                G4cerr << "File: " << cher_to_nrf_infile << " NOT FOUND!" << G4endl;
                return;
        }
        else
        {
                f = new TFile(cher_to_nrf_infile.c_str());
                G4cout << "File: " << cher_to_nrf_infile << " exists!" << G4endl;
        }

        if(gSystem->AccessPathName(to_det_infile.c_str()))
        {
                G4cerr << "File: " << to_det_infile << " NOT FOUND!" << G4endl;
                return;
        }
        else
        {
                f1 = new TFile(to_det_infile.c_str());
                G4cout << "File: " << to_det_infile << " exists!" << G4endl;
        }


        // Set up Output Histograms
        wNRF_NRF_to_Cher = new TH1D("wNRF_NRF_to_Cher","Weighted NRF Spectrum that Lead to Cherenkov",100000, 0., Emax);
        wCher_NRF_to_Cher = new TH1D("wCher_NRF_to_Cher","Weighted Cherenkov Spectrum caused by NRF",100000,0.,Emax);
        wNRF_to_Det = new TH1D("wNRF_to_Det","Weighted NRF Energy Spectrum that Lead to Cherenkov that Lead to Detection",100000,0.,Emax);
        wCher_to_Det = new TH1D("wCher_to_Det","Weighted Cherenkov Energy Spectrum Caused by NRF that Lead to Detection", 100000, 0., Emax);

        bool confirm = f->cd();
        if(confirm)
        {
                f->GetObject("nrf_to_cher_tree",nrf_to_cher_tree);
        }
        else
                return;

        confirm = f1->cd();

        if(confirm)
        {
                f1->GetObject("nrf_to_cher_to_det_tree",nrf_to_cher_to_det_tree);
        }
        else
                return;
        G4cout << "WeightHisto::WeightHisto --> Objects Obtained." << G4endl;

}

WeightHisto::~WeightHisto()
{
}

// ******************************************************************************************************************************** //
// Fill NRF that Lead to Cherenkov Weighted Histogram for NRF Energies and Cherenkov Energies
// ******************************************************************************************************************************** //

void WeightHisto::Fill_NRF_to_Cherenkov()
{
        if(nrf_to_cher_tree == NULL)
        {
                G4cout << "NRF to Cherenkov Tree Empty. Exiting..." << G4endl;
                return;
        }
        G4int n_entries = nrf_to_cher_tree->Draw("NRF_Energy:NRF_Weight","","goff");
        G4cout << "NRF to Cherenkov Tree Entries: " << n_entries << G4endl;

        G4double *nrfcherNRFEnergy = nrf_to_cher_tree->GetVal(0);
        G4double *nrfcherNRFWeight = nrf_to_cher_tree->GetVal(1);

        for(int i=0; i<n_entries; ++i)
        {
                wNRF_NRF_to_Cher->Fill(nrfcherNRFEnergy[i], nrfcherNRFWeight[i]);
        }

        n_entries = nrf_to_cher_tree->Draw("Cher_Energy:Cher_Weight","","goff");
        G4cout << "NRF to Cherenkov to Detected Entries: " << n_entries << G4endl;

        G4double *nrfcherCherEnergy = nrf_to_cher_tree->GetVal(0);
        G4double *nrfcherCherWeight = nrf_to_cher_tree->GetVal(1);

        for(int i=0; i<n_entries; ++i)
        {
                wCher_NRF_to_Cher->Fill(nrfcherCherEnergy[i], nrfcherCherWeight[i]);
        }

        G4cout << "WeightHisto::Fill_NRF_to_Cherenkov --> Complete." << G4endl;
}

// ******************************************************************************************************************************** //
// Fill NRF that Lead to Cherenkov that Lead to Detection Weighted Histogram for NRF Energies and Cherenkov Energies
// ******************************************************************************************************************************** //

void WeightHisto::Fill_to_Det()
{
        if(nrf_to_cher_to_det_tree == NULL)
        {
                G4cout << "To Detection Tree Empty. Exiting..." << G4endl;
                return;
        }
        G4int n_entries = nrf_to_cher_to_det_tree->Draw("EnergyNRF:WeightNRF","","goff");
        G4double *nrfcherdetNRFEnergy = nrf_to_cher_to_det_tree->GetVal(0);
        G4double *nrfcherdetNRFWeight = nrf_to_cher_to_det_tree->GetVal(1);

        for(int i=0; i<n_entries; ++i)
        {
                wNRF_to_Det->Fill(nrfcherdetNRFEnergy[i], nrfcherdetNRFWeight[i]);
        }

        n_entries = nrf_to_cher_to_det_tree->Draw("EnergyCher:WeightCher","","goff");
        G4double *nrfcherdetCherEnergy = nrf_to_cher_to_det_tree->GetVal(0);
        G4double *nrfcherdetCherWeight = nrf_to_cher_to_det_tree->GetVal(1);

        for(int i=0; i<n_entries; ++i)
        {
                wCher_to_Det->Fill(nrfcherdetCherEnergy[i], nrfcherdetCherWeight[i]);
        }

        G4cout << "WeightHisto::Fill_to_Det --> Complete!" << G4endl;
}

// ******************************************************************************************************************************** //
// Write Weighted Histograms to File
// ******************************************************************************************************************************** //

void WeightHisto::Write()
{
        if(nrf_to_cher_tree == NULL)
        {
                return;
        }
        std::string fileOut = gOutName + "_WeightedHisto.root";
        fout = new TFile(fileOut.c_str(), "recreate");
        bool confirm = fout->cd();
        if(confirm)
        {
                if(nrf_to_cher_tree != NULL)
                {
                        wNRF_NRF_to_Cher->Write();
                        wCher_NRF_to_Cher->Write();
                }
                if(nrf_to_cher_to_det_tree != NULL)
                {
                        wNRF_to_Det->Write();
                        wCher_to_Det->Write();
                }
        }
        else
                return;

        fout->Close();
        G4cout << "Weighted Histograms written to: " << fileOut << G4endl;
        time_end = std::time(&timer2);
        G4cout << "Weighting Histos took: " << std::difftime(time_end,time_start) << " seconds!" << G4endl;
}
