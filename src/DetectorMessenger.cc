#include "DetectorMessenger.hh"


DetectorMessenger::DetectorMessenger(DetectorConstruction* DetectorAction)
:DetectorA(DetectorAction)
{
  myDir = new G4UIdirectory("/mydet/");
  myDir2 = new G4UIdirectory("/mytar/");
  myDir3 = new G4UIdirectory("/chopper/");
  myDir2->SetGuidance("Target Setup Commands");
  myDir->SetGuidance("Detector Setup Commands");
  myDir3->SetGuidance("Chopper Setup Commands");
  Cmd = new G4UIcmdWithADouble("/mydet/PCrad",this);
  CmdX = new G4UIcmdWithADouble("/mydet/WaterX",this);
  CmdY = new G4UIcmdWithADouble("/mydet/WaterY",this);
  CmdZ = new G4UIcmdWithADouble("/mydet/WaterZ",this);
  Cmdtr = new G4UIcmdWithADouble("/mytar/IntObjRad",this);
  Cmdtrad = new G4UIcmdWithADouble("/mytar/fAbundance",this);
  Cmdtsel = new G4UIcmdWithAString("/mytar/target",this);
  CmdtXpos = new G4UIcmdWithADouble("/mytar/IntObjXPos",this);
  CmdtYpos = new G4UIcmdWithADouble("/mytar/IntObjYPos",this);
  CmdtZpos = new G4UIcmdWithADouble("/mytar/IntObjZPos",this);
  Cmdpcmat = new G4UIcmdWithAString("/mydet/PCmat",this);
  CmdnPMT = new G4UIcmdWithAnInteger("/mydet/nPMT",this);
  CmdAngle = new G4UIcmdWithADouble("/mydet/Angle",this);
  CmdChopthick = new G4UIcmdWithADouble("/chopper/thickness", this);
  CmdChopZ = new G4UIcmdWithADouble("/chopper/distance", this);
  CmdChopperOn = new G4UIcmdWithAString("/chopper/state", this);
  CmdAttenOn = new G4UIcmdWithAString("/mydet/attenuator", this);
  CmdAttenOn2 = new G4UIcmdWithAString("/mydet/attenuator2",this);
  CmdAttenThick = new G4UIcmdWithADouble("/mydet/attenuatorThickness",this);
  CmdAttenThick2 = new G4UIcmdWithADouble("/mydet/attenuatorThickness2",this);
  CmdAttenMat = new G4UIcmdWithAString("/mydet/attenuatorMaterial",this);
  CmdAttenMat2 = new G4UIcmdWithAString("/mydet/attenuatorMaterial2",this);


  Cmd->SetGuidance("Choose Desired PhotoCathode Radius");
  CmdX->SetGuidance("Choose Desired X Size of Water Tank");
  CmdY->SetGuidance("Choose Desired Y Size of Water Tank");
  CmdZ->SetGuidance("Choose Desired Z Size of Water Tank");
  Cmdtr->SetGuidance("Choose Desired radius Size of Interogation Target");
  Cmdtrad->SetGuidance("Choose Desired fission isotope abundance(enrichment) of Interogation Target");
  Cmdtsel->SetGuidance("Choose Desired Weapons grade target");
  CmdtXpos->SetGuidance("Choose Desired X Position of Interogation Target");
  CmdtYpos->SetGuidance("Choose Desired Y Position of Interogation Target");
  CmdtZpos->SetGuidance("Choose Desired Z Position of Interogation Target");
  Cmdpcmat->SetGuidance("Choose desired photocathode material");
  CmdnPMT->SetGuidance("Choose desired number of PMTs");
  CmdChopthick->SetGuidance("Choose desired chopper thickness");
  CmdChopZ->SetGuidance("Choose desired chopper distance from brem beam");
  CmdChopperOn->SetGuidance("Choose desired chopper wheel state");
  CmdAngle->SetGuidance("Choose desired Detector BackScatter Angle in Degrees");
  CmdAttenOn->SetGuidance("Choose if Attenuator Present or not");
  CmdAttenThick->SetGuidance("Choose Desired attenuator thickness");
  CmdAttenMat->SetGuidance("Choose desired attenuator material from NIST materials");
  CmdAttenOn2->SetGuidance("Choose if Attenuator Second Layer Present or not");
  CmdAttenThick2->SetGuidance("Choose Desired attenuator thickness");
  CmdAttenMat2->SetGuidance("Choose desired attenuator material from NIST materials");

  Cmd->SetParameterName("radius",false);
  CmdX->SetParameterName("waterx",false);
  CmdY->SetParameterName("watery",false);
  CmdZ->SetParameterName("waterz",false);
  Cmdtr->SetParameterName("targetradius",false);
  Cmdtrad->SetParameterName("targetabundance",false);
  Cmdtsel->SetParameterName("targetsel",false);
  CmdtXpos->SetParameterName("targetxpos",false);
  CmdtYpos->SetParameterName("targetypos",false);
  CmdtZpos->SetParameterName("targetzpos",false);
  Cmdpcmat->SetParameterName("photocathodeMat", false);
  CmdnPMT->SetParameterName("numberPMT", false);
  CmdChopZ->SetParameterName("chopperZ", false);
  CmdChopthick->SetParameterName("chopperthickness",false);
  CmdChopperOn->SetParameterName("chopperOn",false);
  CmdAngle->SetParameterName("Angle",false);
  CmdAngle->SetRange("Angle > 90 && Angle < 135");
  CmdAttenOn->SetParameterName("attenuator",false);
  CmdAttenThick->SetParameterName("attenThickness",false);
  CmdAttenMat->SetParameterName("attenMaterial",false);
  CmdAttenOn2->SetParameterName("attenuator2",false);
  CmdAttenThick2->SetParameterName("attenThickness2",false);
  CmdAttenMat2->SetParameterName("attenMaterial2",false);

  Cmdtsel->SetCandidates("Uranium Plutonium Lead/Uranium Lead/Plutonium");
  Cmdpcmat->SetCandidates("GaAsP Bialkali");
  CmdChopperOn->SetCandidates("On on Off off");
  CmdAttenOn->SetCandidates("On on Off off");
  CmdAttenMat->SetCandidates("G4_Pb G4_Cu G4_Zn G4_Ag G4_Cd G4_Th G4_U G4_Au G4_W G4_Fe");
  CmdAttenMat2->SetCandidates("G4_POLYETHYLENE G4_POLYPROPYLENE G4_POLYSTYRENE G4_POLYVINYL_CHLORIDE G4_POLYCARBONATE");

}

DetectorMessenger::~DetectorMessenger()
{
  delete Cmd;
  delete CmdX;
  delete CmdY;
  delete CmdZ;
  delete Cmdtr;
  delete Cmdtrad;
  delete Cmdtsel;
  delete CmdtXpos;
  delete CmdtYpos;
  delete CmdtZpos;
  delete Cmdpcmat;
  delete CmdnPMT;
  delete CmdChopthick;
  delete CmdChopZ;
  delete CmdChopperOn;
  delete CmdAngle;
  delete CmdAttenOn;
  delete CmdAttenThick;
  delete CmdAttenMat;
  delete CmdAttenOn2;
  delete CmdAttenThick2;
  delete CmdAttenMat2;
}


void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
  if(command == Cmd)
  {
    G4double theCommand = Cmd->GetNewDoubleValue(newValue);
    if(theCommand == -1)
    {
      std::cout << "Using Default PC Radius."<<std::endl;
    }
    else
    {
      DetectorA->SetPC_radius(theCommand);
    }

  }
  else if(command == CmdX)
  {
    G4double theCommandX = CmdX->GetNewDoubleValue(newValue);
    if(theCommandX == -1)
    {
      std::cout << "Using Default X Water"<<std::endl;
    }
    else
    {
      DetectorA->SetWaterX(theCommandX);
    }
  }
  else if(command == CmdY)
  {
    G4double theCommandY = CmdY->GetNewDoubleValue(newValue);
    DetectorA->SetWaterY(theCommandY);
  }
  else if(command == CmdZ)
  {
    G4double theCommandZ = CmdZ->GetNewDoubleValue(newValue);
    DetectorA->SetWaterZ(theCommandZ);
  }
  else if(command == Cmdtr)
  {
    G4double theCommandtX = Cmdtr->GetNewDoubleValue(newValue);
    DetectorA->SetIntObj_radius(theCommandtX);
  }

  else if(command == Cmdtrad)
  {
    G4double theCommandtrad = Cmdtrad->GetNewDoubleValue(newValue);
    DetectorA->SetAbundance(theCommandtrad);
  }
  else if(command == Cmdtsel)
  {
    G4String theCommandtsel = newValue;
    DetectorA->SetIntObj(theCommandtsel);

  }
  else if(command == CmdtXpos)
  {
    G4double theCommandtXpos = CmdtXpos->GetNewDoubleValue(newValue);
    DetectorA->SetIntObjX_pos(theCommandtXpos);
  }
  else if(command == CmdtYpos)
  {
    G4double theCommandtYpos = CmdtYpos->GetNewDoubleValue(newValue);
    DetectorA->SetIntObjY_pos(theCommandtYpos);
  }
  else if(command == CmdtZpos)
  {
    G4double theCommandtZpos = CmdtZpos->GetNewDoubleValue(newValue);
    DetectorA->SetIntObjZ_pos(theCommandtZpos);
  }
  else if(command == Cmdpcmat)
  {
    G4String commandpcmat = newValue;
    std::cout << commandpcmat << " set as photocathode material." << std::endl;
    DetectorA->SetPC_material(commandpcmat);
  }
  else if(command == CmdnPMT)
  {
    G4int thecmdnPMT = CmdnPMT->GetNewIntValue(newValue);
    DetectorA->SetnPMT(thecmdnPMT);
  }
  else if(command == CmdChopZ)
  {
    G4double thecmdchopz = CmdChopZ->GetNewDoubleValue(newValue);
    DetectorA->SetChopper_z(thecmdchopz);
  }
  else if(command == CmdChopthick)
  {
    G4double thecmdchopthick = CmdChopthick->GetNewDoubleValue(newValue);
    DetectorA->SetChopperThick(thecmdchopthick);
  }
  else if(command == CmdChopperOn)
  {
    G4String thecmdchopperon = newValue;
    if(thecmdchopperon == "On" || thecmdchopperon == "on")
    {
      DetectorA->SetChopperOn(true);
    }
    else
    {
      DetectorA->SetChopperOn(false);
    }
  }
  else if(command == CmdAngle)
  {
    G4double thecmdAngle = CmdAngle->GetNewDoubleValue(newValue);
    DetectorA->SettheAngle(thecmdAngle);
  }
  else if(command == CmdAttenOn)
  {
    G4String theCmdAttenOn = newValue;
    if(theCmdAttenOn == "Off" || theCmdAttenOn == "off")
    {
      DetectorA->SetAttenuatorState(false);
      DetectorA->SetAttenuatorMaterial("G4_AIR");
      check_atten_on = false;
    }
    else
    {
      check_atten_on = true;
      DetectorA->SetAttenuatorState(true);
    }
  }
  else if(command == CmdAttenThick)
  {
    if(check_atten_on)
    {
      G4double theCmdAttendThickness = CmdAttenThick->GetNewDoubleValue(newValue);
      DetectorA->SetAttenuatorThickness(theCmdAttendThickness);
    }
  }
  else if(command == CmdAttenMat)
  {
    if(check_atten_on)
    {
      G4String theAttenMaterial = newValue;
      DetectorA->SetAttenuatorMaterial(theAttenMaterial);
    }
  }
    else if(command == CmdAttenOn2)
  {
    G4String theCmdAttenOn2 = newValue;
    if(theCmdAttenOn2 == "Off" || theCmdAttenOn2 == "off")
    {
      DetectorA->SetAttenuatorState2(false);
      DetectorA->SetAttenuatorMaterial2("G4_AIR");
      check_atten2_on = false;
    }
    else
    {
      check_atten2_on = true;
      DetectorA->SetAttenuatorState2(true);
    }
  }
  else if(command == CmdAttenThick2)
  {
    if(check_atten2_on)
    {
      G4double theCmdAttendThickness2 = CmdAttenThick2->GetNewDoubleValue(newValue);
      DetectorA->SetAttenuatorThickness2(theCmdAttendThickness2);
    }
  }
  else if(command == CmdAttenMat2)
  {
    if(check_atten2_on)
    {
      G4String theAttenMaterial2 = newValue;
      DetectorA->SetAttenuatorMaterial2(theAttenMaterial2);
    }
  }
  else
  {
    G4cerr << "ERROR DetectorMessenger :: SetDetectorInputValue command != Cmd" << G4endl;
  }
}
