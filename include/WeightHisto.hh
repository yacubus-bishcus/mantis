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

#ifndef WeightHisto_h
#define WeightHisto_h 1

#include "globals.hh"
#include "G4Types.hh"
#include "G4ios.hh"

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
#include "TString.h"

class TH1D;
class TFile;
class TTree;


class WeightHisto
{
public:
    WeightHisto(G4double);
    ~WeightHisto();
    
public:
    void Fill_NRF_to_Cherenkov();
    void Fill_to_Det();
    
private:

    time_t timer, timer2, time_start, time_end;
    TFile *f, *f1, *fout;
    TTree *nrf_to_cher_tree, *nrf_to_cher_to_det_tree;
    TH1D *wNRF_NRF_to_Cher, *wCher_NRF_to_Cher, *wNRF_to_Det, *wCher_to_Det;
    G4double Emax;
};

#endif
