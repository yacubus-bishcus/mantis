void PrintResults(const char* ChopOnBase, const char* ChopOffBase, bool check)
{

  std::string OriginalOn = ChopOnBase;
  std::string OriginalOff = ChopOffBase;
  std::string addedOn = ChopOnBase;
  std::string addedOff = ChopOffBase;
  addedOn = addedOn + "_WeightedHistogramOn.root";
  addedOff = addedOff + "_WeightedHistogramOff.root";
  OriginalOn = OriginalOn + ".root";
  OriginalOff = OriginalOff + ".root";
  const char* OGOn = OriginalOn.c_str();
  const char* OGOff = OriginalOff.c_str();
  const char* adOn = addedOn.c_str();
  const char* adOff = addedOff.c_str();
  
  TFile *OGOnFile = new TFile(OGOn);
  TFile *OGOffFile = new TFile(OGOff);
  TFile *AdOnFile = new TFile(adOn);
  TFile *AdOffFile = new TFile(adOff);
  
  // Histograms from OG
  TH1D *IntObjInOn, *IntObjOutOn, *IntObjInOff, *IntObjOutOff, *IntNRFInOn, *IntNRFOutOn, *IntNRFInOff, *IntNRFOutOff;
  TH1D *WaterInOn, *WaterInOff, *WaterNRFOn, *WaterNRFOff;
  TH1D *DetOn, *DetOff;
    
  // Histograms from Added Weighted Histograms File
  TH1D *ChopInOn, *ChopInOff, *ChopOutOn, *ChopOutOff;
  TH1D *NRFMat, *NRFMatOff;
  TH1D *CherenkovOn, *CherenkovOff;
  TH1D *DetData, *DetDataOff;
  TH1D *wNRF_NRF_to_Cher, *wNRF_NRF_to_CherOff, *wCher_NRF_to_Cher, *wCher_NRF_to_CherOff;
  TH1D *wNRF_NRF_to_Cher_to_Det, *wNRF_NRF_to_Cher_to_DetOff, *wCher_NRF_to_Cher_to_Det, *wCher_NRF_to_Cher_to_DetOff;
  
  // ints, doubles, strings
  
  int entries;
  double weighted_sum, weighted_sum2, weighted_sum3, weighted_sum4, weighted_sum5, weighted_sum6, weighted_sum7, weighted_sum8;
  double cherenkov_in_z, cherenkov_out_z;
  double IntObj_in_z, IntObj_out_z, IntObj_nrf_in_z, IntObj_nrf_out_z;
  double water_in_z, water_nrf_z, cher_z, det_z;

  // ******************************************************************************************************************************** //
  // Variables Declared Objects Set up
  // ******************************************************************************************************************************** //
    
  // ******************************************************************************************************************************** //
  // Quick Chopper Analysis from Added Histograms File
  // ******************************************************************************************************************************** //
 
  std::cout << "Chopper Wheel Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
  // Chopper On Analysis
  AdOnFile->cd();
  AdOnFile->GetObject("wChopIn",ChopInOn);
  entries = ChopInOn->GetEntries();
  weighted_sum = ChopInOn->Integral();
  std::cout << "Incident ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  AdOnFile->GetObject("wChopOut",ChopOutOn);
  entries = ChopOutOn->GetEntries();
  weighted_sum2 = ChopOutOn->Integral();
  std::cout << "Emission ON Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;
  
  // Chopper Off Analysis
  AdOffFile->cd();
  AdOffFile->GetObject("wChopIn",ChopInOff);
  entries = ChopInOff->GetEntries();
  weighted_sum3 = ChopInOff->Integral();
  std::cout << "Incident OFF Entries: " << entries << " and sum: " << weighted_sum3 << std::endl;
  AdOffFile->GetObject("wChopOut",ChopOutOff);
  entries = ChopOutOff->GetEntries();
  weighted_sum4 = ChopOutOff->Integral();
  std::cout << "Emission OFF Entries: " << entries << " and sum: " << weighted_sum4 << std::endl;
  cherenkov_in_z = abs(weighted_sum - weighted_sum3)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum3),2)));
  cherenkov_out_z = abs(weighted_sum2 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum4),2)));
  
  // ******************************************************************************************************************************** //
  // Quick Interrogation Object Analysis from Original File
  // ******************************************************************************************************************************** //

  std::cout << std::endl << "Interrogation Object Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
  // Chopper On Analysis
  OGOnFile->cd();
  OGOnFile->GetObject("IntObjIn",IntObjInOn);
  OGOnFile->GetObject("IntObjOut",IntObjOutOn);
  OGOnFile->GetObject("NRFIntObjIn",IntNRFInOn);
  OGOnFile->GetObject("NRFIntObjOut",IntNRFOutOn);
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
  OGOffFile->cd();
  OGOffFile->GetObject("IntObjIn",IntObjInOff);
  OGOffFile->GetObject("IntObjOut",IntObjOutOff);
  OGOffFile->GetObject("NRFIntObjIn",IntNRFInOff);
  OGOffFile->GetObject("NRFIntObjOut",IntNRFOutOff);
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

  // ******************************************************************************************************************************** //
  // Quick Incident Water Tank Analysis from Original File
  // ******************************************************************************************************************************** //
  
  std::cout << std::endl << "Water Tank Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
  // Chopper On Analysis
  OGOnFile->cd();
  OGOnFile->GetObject("WaterIn",WaterInOn);
  OGOnFile->GetObject("NRFWaterIn",WaterNRFOn);
  entries = WaterInOn->GetEntries();
  weighted_sum = WaterInOn->Integral();
  std::cout << "Incident ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  entries = WaterNRFOn->GetEntries();
  weighted_sum2 = WaterNRFOn->Integral();
  std::cout << "NRF ON Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;
  
  // Chopper Off Analysis
  OGOffFile->cd();
  OGOffFile->GetObject("WaterIn",WaterInOff);
  OGOffFile->GetObject("NRFWaterIn",WaterNRFOff);
  entries = WaterInOff->GetEntries();
  weighted_sum3 = WaterInOff->Integral();
  std::cout << "Incident OFF Entries: " << entries << " and sum: " << weighted_sum3 << std::endl;
  entries = WaterNRFOff->GetEntries();
  weighted_sum4 = WaterNRFOff->Integral();
  std::cout << "NRF OFF Entries: " << entries << " and sum: " << weighted_sum4 << std::endl;
  water_in_z = abs(weighted_sum - weighted_sum3)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum3),2)));
  water_nrf_z = abs(weighted_sum2 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum4),2)));

  // ******************************************************************************************************************************** //
  // Quick Cherenkov Analysis from Added Histograms File
  // ******************************************************************************************************************************** //

  std::cout << std::endl << "Cherenkov Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
  // Chopper On Analysis
  AdOnFile->cd();
  AdOnFile->GetObject("wCher",CherenkovOn);
  entries = CherenkovOn->GetEntries();
  weighted_sum = CherenkovOn->Integral();
  std::cout << "ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
    
  // Chopper Off Analysis
  AdOffFile->cd();
  AdOffFile->GetObject("wCher",CherenkovOff);
  entries = CherenkovOff->GetEntries();
  weighted_sum2 = CherenkovOff->Integral();
  std::cout << "OFF Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;
  cher_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
    
  // ******************************************************************************************************************************** //
  // Quick Incident Photocathode Analysis from Added Histogram File
  // ******************************************************************************************************************************** //

  // Chopper On Analysis
  AdOnFile->cd();
  AdOnFile->GetObject("wDet",DetData);
  entries = DetData->GetEntries();
  weighted_sum = DetData->Integral();
  std::cout << "ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;

  // Chopper Off Analysis
  AdOffFile->cd();
  AdOffFile->GetObject("wDet",DetDataOff);
  entries = DetDataOff->GetEntries();
  weighted_sum2 = DetDataOff->Integral();
  std::cout << "OFF Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;

  double inc_det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));

  // ******************************************************************************************************************************** //
  // Quick Detected Analysis from Original File
  // ******************************************************************************************************************************** //
  
  std::cout << std::endl << "Photocathode Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
  // Chopper On Analysis
  OGOnFile->cd();
  OGOnFile->GetObject("Detected",DetOn);
  entries = DetOn->GetEntries();
  weighted_sum = DetOn->Integral();
  std::cout << "ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;
  
  // Chopper Off Analysis
  OGOffFile->cd();
  OGOffFile->GetObject("Detected",DetOff);
  entries = DetOff->GetEntries();
  weighted_sum2 = DetOff->Integral();
  std::cout << "OFF Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;
  det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
    
  // ******************************************************************************************************************************** //
  // Quick NRF to Cherenkov Analysis from Added Histograms File
  // ******************************************************************************************************************************** //
  if(check)
  {
    // NRF On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wNRF_NRF_to_Cher",wNRF_NRF_to_Cher);
    entries = wNRF_NRF_to_Cher->GetEntries();
    weighted_sum = wNRF_NRF_to_Cher->Integral();
    std::cout << "NRF ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;

    // NRF Off
    AdOffFile->cd();
    AdOffFile->GetObject("wNRF_NRF_to_Cher",wNRF_NRF_to_CherOff);
    entries = wNRF_NRF_to_CherOff->GetEntries();
    weighted_sum2 = wNRF_NRF_to_CherOff->Integral();
    std::cout << "NRF OFF Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;

    // Cher On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wCher_NRF_to_Cher",wCher_NRF_to_Cher);
    entries = wCher_NRF_to_Cher->GetEntries();
    weighted_sum3 = wCher_NRF_to_Cher->Integral();
    std::cout << "Cherenkov ON Entries: " << entries << " and sum: " << weighted_sum3 << std::endl;
    // Cher Off Analysis
    AdOffFile->cd();
    AdOffFile->GetObject("wCher_NRF_to_Cher",wCher_NRF_to_CherOff);
    entries = wCher_NRF_to_CherOff->GetEntries();
    weighted_sum4 = wCher_NRF_to_CherOff->Integral();
    std::cout << "Cherenkov OFF Entries: " << entries << " and sum: " << weighted_sum4 << std::endl;
    double wNRFCher_to_NRF_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
    double wCherCher_to_NRF_z = abs(weighted_sum3 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum3),2) + pow(sqrt(weighted_sum4),2)));

  // ******************************************************************************************************************************** //
  // Quick NRF to Cherenkov to Detected Analysis from Added Histograms File
  // ******************************************************************************************************************************** //

    // NRF On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wNRF_NRF_to_Cher_to_Det",wNRF_NRF_to_Cher_to_Det);
    entries = wNRF_NRF_to_Cher_to_Det->GetEntries();
    weighted_sum = wNRF_NRF_to_Cher_to_Det->Integral();
    std::cout << "NRF ON Entries: " << entries << " and sum: " << weighted_sum << std::endl;

    // NRF Off
    AdOffFile->cd();
    AdOffFile->GetObject("wNRF_NRF_to_Cher_to_Det",wNRF_NRF_to_Cher_to_DetOff);
    entries = wNRF_NRF_to_Cher_to_DetOff->GetEntries();
    weighted_sum2 = wNRF_NRF_to_Cher_to_DetOff->Integral();
    std::cout << "NRF OFF Entries: " << entries << " and sum: " << weighted_sum2 << std::endl;

    // Cher On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wCher_NRF_to_Cher_to_Det",wCher_NRF_to_Cher_to_Det);
    entries = wCher_NRF_to_Cher_to_Det->GetEntries();
    weighted_sum3 = wCher_NRF_to_Cher_to_Det->Integral();
    std::cout << "Cherenkov ON Entries: " << entries << " and sum: " << weighted_sum3 << std::endl;
    // Cher Off Analysis
    AdOffFile->cd();
    AdOffFile->GetObject("wCher_NRF_to_Cher",wCher_NRF_to_CherOff);
    entries = wCher_NRF_to_CherOff->GetEntries();
    weighted_sum4 = wCher_NRF_to_CherOff->Integral();
    std::cout << "Cherenkov OFF Entries: " << entries << " and sum: " << weighted_sum4 << std::endl;
    double wNRFCher_to_NRF_to_det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
    double wCherCher_to_NRF_to_det_z = abs(weighted_sum3 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum3),2) + pow(sqrt(weighted_sum4),2)));
  }
  
  // Z Test Results
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
  if(check)
  {
    std::cout << "NRF to Cherenkov NRF Spectrum Z-Score: " << wNRFCher_to_NRF_z << std::endl;
    std::cout << "NRF to Cherenkov Cherenkov Spectrum Z-Score: " << wCherCher_to_NRF_z << std::endl;
    std::cout << "NRF to Cherenkov to Detector NRF Spectrum Z-Score: " << wNRFCher_to_NRF_to_det_z << std::endl;
    std::cout << "NRF to Cherenkov to Detector Cherenkov Spectrum Z-Score: " << wCherCher_to_NRF_to_det_z << std::endl << std::endl;
  }
}
