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

#ifndef PrimaryGenActionMessenger_h
#define PrimaryGenActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithADouble.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"

class PrimaryGeneratorAction;
class G4UIcmdWithADouble;
class G4UIdirectory;

class PrimaryGenActionMessenger: public G4UImessenger
{
public:
  PrimaryGenActionMessenger(PrimaryGeneratorAction*);
  ~PrimaryGenActionMessenger();

  void SetNewValue(G4UIcommand*, G4String);
private:
  PrimaryGeneratorAction* genA;
  G4UIcmdWithADouble* Cmd;
  G4UIdirectory *myDir;
};

#endif
