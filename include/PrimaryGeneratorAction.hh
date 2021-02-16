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

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "globals.hh"
#include <vector>
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "eventInformation.hh"

#include "TFile.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TRandom1.h"
#include "TSystem.h"

class G4Event;
class PrimaryGenActionMessenger;
class HistoManager;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:
PrimaryGeneratorAction(G4bool, G4bool);
virtual ~PrimaryGeneratorAction();

public:
virtual void GeneratePrimaries(G4Event*);
G4ParticleGun* GetParticleGun()
{
  return fParticleGun;
};
void SetEnergyValue(G4double val)
{
  chosen_energy = val;
}

G4double SampleUResonances();

private:
G4bool bremTest, resonance_test, file_check;
G4double chosen_energy;
PrimaryGenActionMessenger* genM;
G4ParticleGun* fParticleGun;

TRandom1 Random;
TH1D *hBrems;
TH1D *hSample;

protected:
G4float energy;
};

#endif
