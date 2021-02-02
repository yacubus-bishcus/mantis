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

#ifndef EventCheck_h
#define EventCheck_h 1

#include "globals.hh"
#include "G4Types.hh"
#include "G4ios.hh"

#if defined (G4ANALYSIS_USE_ROOT)
#include "TFile.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TTree.h"
#endif


class EventCheck
{
public:
    EventCheck();
    ~EventCheck();
    
public:
    void Determine_NRF_to_Cherenkov();
    void Determine_Detection_Events();
    void WriteEvents();
    
private:

time_t timer, timer2, time_start, time_end;
TTree *Cherenkov, *NRF, *DetData, nrf_to_cher_tree, nrf_to_cher_to_det_tree;
std::vector<int> cherEventv, nrfEventv, detEventv, nrf_to_cherEvents, cher_to_detEvents, nrf_to_detEvents;
std::vector<double> cher_to_detEnergies, cher_to_detWeights, cher_to_detTimes;
std::vector<double> nrf_to_detEnergies, nrf_to_detWeights, nrf_to_detTimes;
std::vector<double> nrfEnergyv, nrfWeightv, cherEnergyv, cherWeightv;
std::vector<double> nrf_to_cherenkov_to_detEvents, nrf_to_cherenkov_to_detNRFEnergies, nrf_to_cherenkov_to_detCherEnergies;
std::vector<double> nrf_to_cherenkov_to_detNRFWeights, nrf_to_cherenkov_to_detCherWeights;
std::vector<double> nrf_to_cherenkov_to_detNRFTimes, nrf_to_cherenkov_to_detCherTimes;
double nrfE,nrfW, cherE, cherW, a, energyNRF, energyCher, weightNRF, weightCher, timeNRF, timeCher;
int nrf_cher_EventID;
G4int n, n1, n2;

};

#endif
