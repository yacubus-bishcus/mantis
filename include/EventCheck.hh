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
#include <vector>

#include "TROOT.h"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

class TFile;
class TTree;
class TBranch;


class EventCheck
{
public:
    EventCheck();
    ~EventCheck();

public:
    void Compute();
    void WriteEvents();
    void CopyATree(const char*, G4bool);
    void CopyEvents();
    void Cleanup(G4bool);
    void Finish();

private:
  time_t timer, timer2, time_start, time_end;
  std::vector<double> nrf_to_cher_to_det_event, nrf_to_cher_to_det_energy, nrf_to_cher_to_det_weight,nrf_to_cher_to_det_time;
  G4int x;
  G4String event_output_name;

};

#endif
