void PrintResults(const char* ChopOnBase, const char* ChopOffBase)
{
  std::cout << "Setting Chop On File: " << ChopOnBase << std::endl;
  std::cout << "Setting Chop Off File: " << ChopOffBase << std::endl << std::endl;
  std::string ChopOnSChop = ChopOnBase;
  std::string ChopOffSChop = ChopOffBase;
  std::string ChopOnSCher = ChopOnBase;
  std::string ChopOffSCher = ChopOffBase;
  std::string ChopOnS = ChopOnBase;
  std::string ChopOffS = ChopOffBase;
  ChopOnSChop = ChopOnSChop + "_WeightedHistogramOn.root";
  ChopOffSChop = ChopOffSChop + "_WeightedHistogramOff.root"
  ChopOnSCher = ChopOnSCher + "_CherenkovMergedOn.root";
  ChopOffSCher = ChopOffSCher + "_CherenkovMergedOff.root";
  ChopOnS = ChopOnS + ".root";
  ChopOffS = ChopOffS + ".root";
  const char* ChopOnChop = ChopOnSChop.c_str();
  const char* ChopOffChop = ChopOffSChop.c_str();
  const char* ChopOnCherenkov = ChopOnSCher.c_str();
  const char* ChopOffCherenkov = ChopOffSCher.c_str();
  const char* ChopOn = ChopOnS.c_str();
  const char* ChopOff = ChopOffS.c_str();
  
  TFile *chopOnChop = new TFile(ChopOnChop);
  TFile *chopOffChop = new TFile(ChopOffChop);
  TFile *chopOnCherenkov = new TFile(ChopOnCherenkov);
  TFile *chopOffCherenkov = new TFile(ChopOffCherenkov);
  TFile *chopOn = new TFile(ChopOn);
  TFile *chopOff = new TFile(ChopOff);
  
  
  // TTrees 
  TTree *ChopInOn, *ChopOutOn, *ChopInOff, *ChopOutOff, *NRFMatDataOn, *NRFMatDataOff, *CherenkovOn, *CherenkovOff, *DetDataOn, *DetDataOff;
  
  // Histograms 
  TH1D *IntObjInOn, *IntObjOutOn, *IntObjInOff, *IntObjOutOff, *IntNRFInOn, *IntNRFOutOn, *IntNRFInOff, *IntNRFOutOff, *WaterInOn, *WaterInOff, *WaterNRFOn, *WaterNRFOff, *DetOn, *DetOff;
  
  // ints, doubles, strings 
  
  int entries
  double weighted_sum
  
  // Quick Chopper Analysis 
    
  // Chopper On Analysis 
  chopOnChop->cd();
  chopOnChop->GetObject("wChopIn",ChopInOn);
  entries = ChopInOn->GetEntries();
  weighted_sum = ChopInOn->Integral();
  std::cout << "Incident ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  chopOnChop->GetObject("wChopOut",ChopOutOn);
  entries = ChopOutOn->GetEntries();
  weighted_sum = ChopOutOn->Integral();
  std::cout << "Emission ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  // Chopper Off Analysis
  chopOffChop->cd();
  chopOffChop->GetObject("wChopIn",ChopInOff);
  entries = ChopInOff->GetEntries();
  weighted_sum = ChopInOff->Integral();
  std::cout << "Incident OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  chopOffChop->GetObject("wChopOut",ChopOutOff);
  entries = ChopOutOff->GetEntries();
  weighted_sum = ChopOutOff->Integral();
  std::cout << "Emission OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  // Quick Interrogation Object Analysis 
  
  // Chopper On Analysis 
  chopOn->cd();
  chopOn->GetObject("IntObjIn",IntObjInOn);
  chopOn->GetObject("IntObjOut",IntObjOutOn);
  chopOn->GetObject("NRFIntObjIn",IntNRFInOn);
  chopOn->GetObject("NRFIntObjOut",IntNRFOutOn);
  entries = IntObjInOn->GetEntries();
  weighted_sum = IntObjInOn->Integral();
  std::cout << "Incident ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = IntObjOutOn->GetEntries();
  weighted_sum = IntObjOutOn->Integral();
  std::cout << "Emission ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = IntNRFInOn->GetEntries();
  weighted_sum = IntNRFInOn->Integral();
  std::cout << "NRF Incident ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = IntNRFOutOn->GetEntries();
  weighted_sum = IntNRFOutOn->Integral();
  std::cout << "NRF Emission ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  // Chopper Off Analysis 
  chopOff->cd();
  chopOff->GetObject("IntObjIn",IntObjInOff);
  chopOff->GetObject("IntObjOut",IntObjOutOff);
  chopOff->GetObject("NRFIntObjIn",IntNRFInOff);
  chopOff->GetObject("NRFIntObjOut",IntNRFOutOff);
  entries = IntObjInOff->GetEntries();
  weighted_sum = IntObjInOff->Integral();
  std::cout << "Incident OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = IntObjOutOff->GetEntries();
  weighted_sum = IntObjOutOff->Integral();
  std::cout << "Emission OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = IntNRFInOff->GetEntries();
  weighted_sum = IntNRFInOff->Integral();
  std::cout << "NRF Incident OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = IntNRFOutOff->GetEntries();
  weighted_sum = IntNRFOutOff->Integral();
  std::cout << "NRF Emission OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  // Quick Incident Water Tank Analysis 
  
  // Chopper On Analysis 
  chopOn->cd();
  chopOn->GetObject("WaterIn",WaterInOn);
  chopOn->GetObject("NRFWaterIn",WaterNRFOn);
  entries = WaterInOn->GetEntries();
  weighted_sum = WaterInOn->Integral();
  std::cout << "Incident ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = WaterNRFOn->GetEntries();
  weighted_sum = WaterNRFOn->Integral();
  std::cout << "NRF ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  // Chopper Off Analysis 
  chopOff->cd();
  chopOff->GetObject("WaterIn",WaterInOff);
  chopOff->GetObject("NRFWaterIn",WaterNRFOff);
  entries = WaterInOff->GetEntries();
  weighted_sum = WaterInOff->Integral();
  std::cout << "Incident OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = WaterNRFOff->GetEntries();
  weighted_sum = WaterNRFOff->Integral();
  std::cout << "NRF OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  // Quick Cherenkov Analysis 
  
  // Chopper On Analysis
  chopOnCherenkov->cd();
  chopOnCherenkov->GetObject("wCher",CherenkovOn);
  entries = CherenkovOn->GetEntries();
  weighted_sum = CherenkovOn->Integral();
  std::cout << "ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
    
  // Chopper Off Analysis 
  chopOffCherenkov->cd();
  chopOffCherenkov->GetObject("wCher",CherenkovOff);
  entries = CherenkovOff->GetEntries();
  weighted_sum = CherenkovOff->Integral();
  std::cout << "OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  // Quick Detected Analysis 
  
  // Chopper On Analysis 
  chopOn->cd();
  chopOn->GetObject("Detected",DetOn);
  entries = DetOn->GetEntries();
  weighted_sum = DetOn->Integral();
  std::cout << "ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  // Chopper Off Analysis 
  chopOff->cd();
  chopOff->GetObject("Detected",DetOff);
  entries = DetOff->GetEntries();
  weighted_sum = DetOff->Integral();
  std::cout << "OFF Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  
  
}
