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

#include "TrackingMessenger.hh"


TrackingMessenger::TrackingMessenger(TrackingAction* TrackingAction)
        : trackA(TrackingAction)
{
  myDir = new G4UIdirectory("/mantistracking/");
  myDir->SetGuidance("Tracking Commands");
  Cmd = new G4UIcmdWithAString("/mantistracking/track",this);
  Cmd->SetGuidance("Choose Desired Vis Tracking Output");
  Cmd->SetGuidance("Choice: Photons, OpticalPhotons, Detected, Chopper, all (default)");
  Cmd->SetParameterName("choice",false);
  Cmd->SetDefaultValue("all");
  Cmd->SetCandidates("Photons OpticalPhotons Detected Chopper all");
}

TrackingMessenger::~TrackingMessenger()
{
  delete Cmd;
}


void TrackingMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
  if(command == Cmd)
  {
    G4String theCommand = newValue;
    if(theCommand == "Photons")
    {
      trackA->SetPhotonsOnly();
      G4cout << G4endl << "User Selected to Track Photons Only." << G4endl;
    }
    else if(theCommand == "OpticalPhotons")
    {
      trackA->SetOpticalPhotonsOnly();
      G4cout << G4endl << "User Selected to Track Optical Photons Only." << G4endl;
    }
    else if(theCommand == "Detected")
    {
      trackA->SetDetectedOnly();
      G4cout << G4endl << "User Selected to Track Detected Particles Only." << G4endl;
    }
    else if(theCommand == "Chopper")
    {
      trackA->SetChopperOnly();
      G4cout << G4endl << "User Selected to Track Particles Incident Chopper Only." << G4endl;
    }
    else
    {
      G4cerr << "TrackingMessenger::SetNewValue -> Command Not Found." << G4endl;
    }
  }
}
