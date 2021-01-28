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
  ChopOffSChop = ChopOffSChop + "_WeightedHistogramOff.root";
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
  
  // Histograms
  TH1D *IntObjInOn, *IntObjOutOn, *IntObjInOff, *IntObjOutOff, *IntNRFInOn, *IntNRFOutOn, *IntNRFInOff, *IntNRFOutOff, *WaterInOn, *WaterInOff, *WaterNRFOn, *WaterNRFOff, *DetOn, *DetOff;
  
  // ints, doubles, strings
  
  int entries;
  double weighted_sum, weighted_sum2, weighted_sum3, weighted_sum4, weighted_sum5, weighted_sum6, weighted_sum7, weighted_sum8;
  double cherenkov_in_z, cherenkov_out_z;
  double IntObj_in_z, IntObj_out_z, IntObj_nrf_in_z, IntObj_nrf_out_z;
  double water_in_z, water_nrf_z, cher_z, det_z;
  
  // Quick Chopper Analysis
  std::cout << "Chopper Wheel Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
  // Chopper On Analysis
  chopOnChop->cd();
  chopOnChop->GetObject("wChopIn",ChopInOn);
  entries = ChopInOn->GetEntries();
  weighted_sum = ChopInOn->Integral();
  std::cout << "Incident ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  chopOnChop->GetObject("wChopOut",ChopOutOn);
  entries = ChopOutOn->GetEntries();
  weighted_sum2 = ChopOutOn->Integral();
  std::cout << "Emission ON Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;
  
  // Chopper Off Analysis
  chopOffChop->cd();
  chopOffChop->GetObject("wChopIn",ChopInOff);
  entries = ChopInOff->GetEntries();
  weighted_sum3 = ChopInOff->Integral();
  std::cout << "Incident OFF Entries: " << entries << " and sum: " << weighted_sum3 << std::endl;
  chopOffChop->GetObject("wChopOut",ChopOutOff);
  entries = ChopOutOff->GetEntries();
  weighted_sum4 = ChopOutOff->Integral();
  std::cout << "Emission OFF Entries: " << entries << " and sum: " << weighted_sum4 << std::endl;
  cherenkov_in_z = abs(weighted_sum - weighted_sum3)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum3),2)));
  cherenkov_out_z = abs(weighted_sum2 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum4),2)));
  
  
  // Quick Interrogation Object Analysis
  std::cout << "Interrogation Object Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
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
  weighted_sum2 = IntObjOutOn->Integral();
  std::cout << "Emission ON Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;
  entries = IntNRFInOn->GetEntries();
  weighted_sum3 = IntNRFInOn->Integral();
  std::cout << "NRF Incident ON Entries: " << entries << " and sum: " << weighted_sum3 << std::endl;
  entries = IntNRFOutOn->GetEntries();
  weighted_sum4 = IntNRFOutOn->Integral();
  std::cout << "NRF Emission ON Entries: " << entries << " and sum: " << weighted_sum4 << std::endl;
  
  // Chopper Off Analysis
  chopOff->cd();
  chopOff->GetObject("IntObjIn",IntObjInOff);
  chopOff->GetObject("IntObjOut",IntObjOutOff);
  chopOff->GetObject("NRFIntObjIn",IntNRFInOff);
  chopOff->GetObject("NRFIntObjOut",IntNRFOutOff);
  entries = IntObjInOff->GetEntries();
  weighted_sum5 = IntObjInOff->Integral();
  std::cout << "Incident OFF Entries: " << entries << " and sum: " << weighted_sum5 << std::endl;
  entries = IntObjOutOff->GetEntries();
  weighted_sum6 = IntObjOutOff->Integral();
  std::cout << "Emission OFF Entries: " << entries << " and sum: " << weighted_sum6 << std::endl;
  entries = IntNRFInOff->GetEntries();
  weighted_sum7 = IntNRFInOff->Integral();
  std::cout << "NRF Incident OFF Entries: " << entries << " and sum: " << weighted_sum7 << std::endl;
  entries = IntNRFOutOff->GetEntries();
  weighted_sum8 = IntNRFOutOff->Integral();
  std::cout << "NRF Emission OFF Entries: " << entries << " and sum: " << weighted_sum8 << std::endl;
  IntObj_in_z = abs(weighted_sum - weighted_sum5)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum5),2)));
  IntObj_out_z = abs(weighted_sum2 - weighted_sum6)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum6),2)));
  IntObj_nrf_in_z = abs(weighted_sum3 - weighted_sum7)/(sqrt(pow(sqrt(weighted_sum3),2) + pow(sqrt(weighted_sum7),2)));
  IntObj_nrf_out_z = abs(weighted_sum4 - weighted_sum8)/(sqrt(pow(sqrt(weighted_sum4),2) + pow(sqrt(weighted_sum8),2)));
  
  // Quick Incident Water Tank Analysis
  std::cout << "Water Tank Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
  // Chopper On Analysis
  chopOn->cd();
  chopOn->GetObject("WaterIn",WaterInOn);
  chopOn->GetObject("NRFWaterIn",WaterNRFOn);
  entries = WaterInOn->GetEntries();
  weighted_sum = WaterInOn->Integral();
  std::cout << "Incident ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = WaterNRFOn->GetEntries();
  weighted_sum2 = WaterNRFOn->Integral();
  std::cout << "NRF ON Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;
  
  // Chopper Off Analysis
  chopOff->cd();
  chopOff->GetObject("WaterIn",WaterInOff);
  chopOff->GetObject("NRFWaterIn",WaterNRFOff);
  entries = WaterInOff->GetEntries();
  weighted_sum3 = WaterInOff->Integral();
  std::cout << "Incident OFF Entries: " << entries << " and sum: " << weighted_sum3 << std::endl;
  entries = WaterNRFOff->GetEntries();
  weighted_sum4 = WaterNRFOff->Integral();
  std::cout << "NRF OFF Entries: " << entries << " and sum: " << weighted_sum4 << std::endl;
  water_in_z = abs(weighted_sum - weighted_sum3)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum3),2)));
  water_nrf_z = abs(weighted_sum2 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum4),2)));
  
  // Quick Cherenkov Analysis
  std::cout << "Cherenkov Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
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
  weighted_sum2 = CherenkovOff->Integral();
  std::cout << "OFF Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;
  cher_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
  
  // Quick Detected Analysis
  // Photocathode Analysis
  std::cout << "Photocathode Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
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
  weighted_sum2 = DetOff->Integral();
  std::cout << "OFF Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;
  det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
  
  // Z Test Results
  cherenkov_in_z, cherenkov_out_z;
  IntObj_in_z, IntObj_out_z, IntObj_nrf_in_z, IntObj_nrf_out_z;
  water_in_z, water_nrf_z, cher_z, det_z;
  std::cout << "Cherenkov Incident Z-Score: " << cherenkov_in_z << std::endl;
  std::cout << "Cherenkov Emission Z-Score: " << cherenkov_out_z << std::endl;
  std::cout << "Interrogation Object Incident Z-Score: " << IntObj_in_z << std::endl;
  std::cout << "Interrogation Object Emission Z-Score: " << IntObj_out_z << std::endl;
  std::cout << "Interrogation Object NRF Incident Z-Score: " << IntObj_nrf_in_z << std::endl;
  std::cout << "Interrogation Object NRF Emission Z-Score: " << IntObj_nrf_out_z << std::endl;
  std::cout << "Water Tank Incident Z-Score: " << water_in_z << std::endl;
  std::cout << "Water Tank NRF incident Z-Score: " << water_nrf_z << std::endl;
  std::cout << "Created Cherenkov Energy Z-Score: " << cher_z << std::endl;
  std::cout << "Detected Z-Score: " << det_z << std::endl;
}
