#include "DetectorMessenger.hh"


DetectorMessenger::DetectorMessenger(DetectorConstruction* DetectorAction)
:DetectorA(DetectorAction)
{
  myDir = new G4UIdirectory("/mydet/");
  myDir2 = new G4UIdirectory("/mytar/");
  myDir2->SetGuidance("Target Setup Commands");
  myDir->SetGuidance("Detector Setup Commands");
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
  Cmdpcmat = new G4UIcmdWithAString("/mydet/Pcmat",this);
  CmdnPMT = new G4UIcmdWithAnInteger("/mydet/nPMT",this);


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

  Cmd->SetDefaultValue(-1);
  CmdX->SetDefaultValue(-1);
  CmdY->SetDefaultValue(-1);
  CmdZ->SetDefaultValue(-1);
  Cmdtr->SetDefaultValue(-1);
  Cmdtrad->SetDefaultValue(-1);
  Cmdtsel->SetDefaultValue("Uranium");
  Cmdtsel->SetCandidates("Uranium Plutonium");
  CmdtXpos->SetDefaultValue(-1);
  CmdtYpos->SetDefaultValue(-1);
  CmdtZpos->SetDefaultValue(-1);
  Cmdpcmat->SetDefaultValue("GaAsP");
  Cmdpcmat->SetCandidates("GaAsP Bialkali");
  CmdnPMT->SetDefaultValue(1);

  //Cmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

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
    if(theCommandY == -1)
    {
      std::cout << "Using Default Y Water"<<std::endl;
    }
    else
    {
      DetectorA->SetWaterY(theCommandY);
    }
  }
  else if(command == CmdZ)
  {
    G4double theCommandZ = CmdZ->GetNewDoubleValue(newValue);
    if(theCommandZ == -1)
    {
      std::cout << "Using Default Z Water"<<std::endl;
    }
    else
    {
      DetectorA->SetWaterZ(theCommandZ);
    }
  }
  else if(command == Cmdtr)
  {
    G4double theCommandtX = Cmdtr->GetNewDoubleValue(newValue);
    if(theCommandtX == -1)
    {
      std::cout << "Using Default Target radius" <<std::endl;
    }
    else
    {
      DetectorA->SetIntObj_radius(theCommandtX);
    }
  }

  else if(command == Cmdtrad)
  {
    G4double theCommandtrad = Cmdtrad->GetNewDoubleValue(newValue);
    if(theCommandtrad == -1)
    {
      std::cout << "Using Default Target fission abundance" <<std::endl;
    }
    else
    {
      DetectorA->SetAbundance(theCommandtrad);
    }
  }
  else if(command == Cmdtsel)
  {
    G4String theCommandtsel = newValue;
    std::cout << "Using a " << theCommandtsel << " target." << std::endl;
    DetectorA->SetIntObj(theCommandtsel);

  }
  else if(command == CmdtXpos)
  {
    G4double theCommandtXpos = CmdtXpos->GetNewDoubleValue(newValue);
    if(theCommandtXpos == -1)
    {
      std::cout << "Using Default Target X position" <<std::endl;
    }
    else
    {
      DetectorA->SetIntObjX_pos(theCommandtXpos);
    }
  }
  else if(command == CmdtYpos)
  {
    G4double theCommandtYpos = CmdtYpos->GetNewDoubleValue(newValue);
    if(theCommandtYpos == -1)
    {
      std::cout << "Using Default Target Y position" <<std::endl;
    }
    else
    {
      DetectorA->SetIntObjY_pos(theCommandtYpos);
    }
  }
  else if(command == CmdtZpos)
  {
    G4double theCommandtZpos = CmdtZpos->GetNewDoubleValue(newValue);
    if(theCommandtZpos == -1)
    {
      std::cout << "Using Default Target Z position" <<std::endl;
    }
    else
    {
      DetectorA->SetIntObjZ_pos(theCommandtZpos);
    }
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
    if(thecmdnPMT == 1)
    {
      std::cout << "Using Default Number of PMTs: 1" << std::endl;
    }
    else
    {
      DetectorA->SetnPMT(thecmdnPMT);
    }
  }
  else
  {
    G4cerr << "ERROR DetectorMessenger :: SetDetectorInputValue command != Cmd" << G4endl;
  }
}
