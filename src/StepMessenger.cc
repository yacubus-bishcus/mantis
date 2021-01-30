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

#include "StepMessenger.hh"


StepMessenger::StepMessenger(SteppingAction* stepAction)
        : stepA(stepAction)
{
  myDir = new G4UIdirectory("/output/");
  myDir->SetGuidance("Output Commands");
  Cmd = new G4UIcmdWithAString("/output/myoutput",this);
  Cmd->SetGuidance("Choose Desired Outputs");
  Cmd->SetGuidance("Choice: ChopData, NRFData, IntObjData, WaterIncData, CherenkovData, DetData, none (default)");
  Cmd->SetParameterName("choice",false);
  Cmd->SetDefaultValue("none");
  Cmd->SetCandidates("ChopData NRFData IntObjData WaterIncData CherenkovData DetData none");
}

StepMessenger::~StepMessenger()
{
  delete Cmd;
}


void StepMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
  if(command == Cmd)
  {
    G4String theCommand = newValue;
    if(theCommand == "ChopData")
    {
      stepA->SetChopperDataFlag(1);
      std::cout << "User Selected Chopper Data." << std::endl;
    }
    else if(theCommand == "NRFData")
    {
      stepA->SetNRFDataFlag(1);
      std::cout << "User Selected NRF Data." << std::endl;
    }
    else if(theCommand == "IntObjData")
    {
      stepA->SetIntObjDataFlag(1);
      std::cout << "User Selected Interrogation Object Incident Data." << std::endl;
    }
    else if(theCommand == "WaterIncData")
    {
      stepA->SetWaterIncDataFlag(1);
      std::cout << "User Selected Incident Water Data." <<std::endl;
    }
    else if(theCommand == "CherenkovData")
    {
      stepA->SetCherenkovDataFlag(1);
      std::cout << "User Selected Cherenkov Data." <<std::endl;
    }
    else if(theCommand == "DetData")
    {
      stepA->SetDetDataFlag(1);
      std::cout << "User Selected Detected Data."<<std::endl;
    }
    else if(theCommand == "none")
    {
      G4cout << "No output requested." << G4endl;
    }
    else
    {
      G4cerr << "ERROR: StepMessenger::SetStepInputValue Command not found." << G4endl;
    }

  }
  else
  {
    G4cerr << "ERROR StepMessenger :: SetStepInputValue command != Cmd" << G4endl;
  }
}
