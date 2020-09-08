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
  CmdtX = new G4UIcmdWithADouble("/mytar/IntObjX",this);
  CmdtY = new G4UIcmdWithADouble("/mytar/IntObjY",this);
  CmdtZ = new G4UIcmdWithADouble("/mytar/IntObjZ",this);
  Cmdtrad = new G4UIcmdWithADouble("/mytar/fAbundance",this);
  Cmdtsel = new G4UIcmdWithAString("/mytar/target",this);


  Cmd->SetGuidance("Choose Desired PhotoCathode Radius");
  CmdX->SetGuidance("Choose Desired X Size of Water Tank");
  CmdY->SetGuidance("Choose Desired Y Size of Water Tank");
  CmdZ->SetGuidance("Choose Desired Z Size of Water Tank");
  CmdtX->SetGuidance("Choose Desired X Size of Interogation Target");
  CmdtY->SetGuidance("Choose Desired Y Size of Interogation Target");
  CmdtZ->SetGuidance("Choose Desired Z Size of Interogation Target");
  Cmdtrad->SetGuidance("Choose Desired fission isotope abundance(enrichment) of Interogation Target");
  Cmdtsel->SetGuidance("Choose Desired Weapons grade target");
  Cmd->SetParameterName("radius",false);
  CmdX->SetParameterName("waterx",false);
  CmdY->SetParameterName("watery",false);
  CmdZ->SetParameterName("waterz",false);
  CmdtX->SetParameterName("targetx",false);
  CmdtY->SetParameterName("targety",false);
  CmdtZ->SetParameterName("targetz",false);
  Cmdtrad->SetParameterName("targetrad",false);
  Cmdtsel->SetParameterName("targetsel",false);
  Cmd->SetDefaultValue(-1);
  CmdX->SetDefaultValue(-1);
  CmdY->SetDefaultValue(-1);
  CmdZ->SetDefaultValue(-1);
  CmdtX->SetDefaultValue(-1);
  CmdtY->SetDefaultValue(-1);
  CmdtZ->SetDefaultValue(-1);
  Cmdtrad->SetDefaultValue(-1);
  Cmdtsel->SetDefaultValue("Uranium");
  Cmdtsel->SetCandidates("Uranium Plutonium");

  //Cmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

}

DetectorMessenger::~DetectorMessenger()
{
  delete Cmd;
  delete CmdX;
  delete CmdY;
  delete CmdZ;
  delete CmdtX;
  delete CmdtY;
  delete CmdtZ;
  delete Cmdtrad;
  delete Cmdtsel;
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
  else if(command == CmdtX)
  {
    G4double theCommandtX = CmdtX->GetNewDoubleValue(newValue);
    if(theCommandtX == -1)
    {
      std::cout << "Using Default Target X" <<std::endl;
    }
    else
    {
      DetectorA->SetIntObjX(theCommandtX);
    }
  }
  else if(command == CmdtY)
  {
    G4double theCommandtY = CmdtY->GetNewDoubleValue(newValue);
    if(theCommandtY == -1)
    {
      std::cout << "Using Default Target Y" <<std::endl;
    }
    else
    {
      DetectorA->SetIntObjY(theCommandtY);
    }
  }
  else if(command == CmdtZ)
  {
    G4double theCommandtZ = CmdtZ->GetNewDoubleValue(newValue);
    if(theCommandtZ == -1)
    {
      std::cout << "Using Default Target Z" <<std::endl;
    }
    else
    {
      DetectorA->SetIntObjZ(theCommandtZ);
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
    if(theCommandtsel == "Uranium")
    {
      std::cout << "Using a Uranium Target" <<std::endl;
      DetectorA->SetIntObj(theCommandtsel);
    }
    else if(theCommandtsel == "Plutonium")
    {
      std::cout << "Using a Plutonium Target" << std::endl;
      DetectorA->SetIntObj(theCommandtsel);
    }
    else{std::cerr << "ERROR: DetectorMessenger: Target not selected."<<std::endl;}
  }
  else
  {
    G4cerr << "ERROR DetectorMessenger :: SetDetectorInputValue command != Cmd" << G4endl;
  }
}
