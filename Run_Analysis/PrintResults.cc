//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File:
// root -b -q 'PrintResults.cc("test", "testOff", true, true)'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 5 inputs 
// 1. Chopper On Filename 
// 2. Chopper Off Filename
// 3. Check Bool if Weighted Histogram File Exists 
// 4. Check Bool if Cherenkov Weighted Histogram Exists in Weighted Histogram file 
// 5. Check Bool if Weighted Histogram contains EventCheck Data
// 
// This File Scans the Weighted Histogram File. 
// This file prints to terminal all histogram 
// entries, weighted sums, and Z-Scores.
//

void PrintResults(const char* ChopOnBase, const char* ChopOffBase, bool checkAdOnFile, bool checkCherenkov, bool checkEvents)
{
  TFile *AdOnFile, *AdOffFile;
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
  if(gSystem->AccessPathName(adOn) && gSystem->AccessPathName(adOff))
  {
    std::cout << "Added Files do not exist. Skipping..." << std::endl;
  }
  else
  {
    AdOnFile = TFile::Open(adOn);
    AdOffFile = TFile::Open(adOff);
  }
  
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
  double weighted_sum, weighted_sum2, weighted_sum3, weighted_sum4, weighted_sum5, weighted_sum6, weighted_sum7, weighted_sum8;
  double chopper_in_z, chopper_out_z;
  double IntObj_in_z, IntObj_out_z, IntObj_nrf_in_z, IntObj_nrf_out_z;
  double water_in_z, water_nrf_z, cher_z, det_z;
  double wNRFCher_to_NRF_to_det_z, wCherCher_to_NRF_to_det_z, wNRFCher_to_NRF_z, wCherCher_to_NRF_z, inc_det_z;

  // ******************************************************************************************************************************** //
  // Variables Declared Objects Set up
  // ******************************************************************************************************************************** //
    
  // ******************************************************************************************************************************** //
  // Quick Chopper Analysis from Added Histograms File
  // ******************************************************************************************************************************** //
  if(checkAdOnFile)
  {
    std::cout << "Chopper Wheel Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    // Chopper On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wChopIn",ChopInOn);
    weighted_sum = ChopInOn->Integral();
    ChopInOn->Print();
    AdOnFile->GetObject("wChopOut",ChopOutOn);
    ChopOutOn->Print();
    weighted_sum2 = ChopOutOn->Integral();

    // Chopper Off Analysis
    AdOffFile->cd();
    AdOffFile->GetObject("wChopIn",ChopInOff);
    weighted_sum3 = ChopInOff->Integral();
    ChopInOff->Print();
    AdOffFile->GetObject("wChopOut",ChopOutOff);
    weighted_sum4 = ChopOutOff->Integral();
    ChopOutOff->Print();
    chopper_in_z = abs(weighted_sum - weighted_sum3)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum3),2)));
    chopper_out_z = abs(weighted_sum2 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum4),2)));
  }
  
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
  weighted_sum = IntObjInOn->Integral();
  IntObjInOn->Print();
  weighted_sum2 = IntObjOutOn->Integral();
  IntObjOutOn->Print();
  weighted_sum3 = IntNRFInOn->Integral();
  IntNRFInOn->Print();
  weighted_sum4 = IntNRFOutOn->Integral();
  IntNRFOutOn->Print();
  
  // Chopper Off Analysis
  OGOffFile->cd();
  OGOffFile->GetObject("IntObjIn",IntObjInOff);
  OGOffFile->GetObject("IntObjOut",IntObjOutOff);
  OGOffFile->GetObject("NRFIntObjIn",IntNRFInOff);
  OGOffFile->GetObject("NRFIntObjOut",IntNRFOutOff);
  weighted_sum5 = IntObjInOff->Integral();
  IntObjInOff->Print();
  weighted_sum6 = IntObjOutOff->Integral();
  IntObjOutOff->Print();
  weighted_sum7 = IntNRFInOff->Integral();
  IntNRFInOff->Print();
  weighted_sum8 = IntNRFOutOff->Integral();
  IntNRFOutOff->Print();
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
  weighted_sum = WaterInOn->Integral();
  WaterInOn->Print();
  weighted_sum2 = WaterNRFOn->Integral();
  WaterNRFOn->Print();
  
  // Chopper Off Analysis
  OGOffFile->cd();
  OGOffFile->GetObject("WaterIn",WaterInOff);
  OGOffFile->GetObject("NRFWaterIn",WaterNRFOff);
  weighted_sum3 = WaterInOff->Integral();
  WaterInOff->Print();
  weighted_sum4 = WaterNRFOff->Integral();
  WaterNRFOff->Print();
  water_in_z = abs(weighted_sum - weighted_sum3)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum3),2)));
  water_nrf_z = abs(weighted_sum2 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum4),2)));

  // ******************************************************************************************************************************** //
  // Quick Cherenkov Analysis from Added Histograms File
  // ******************************************************************************************************************************** //

  if(checkAdOnFile && checkCherenkov)
  {
    std::cout << std::endl << "Cherenkov Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    // Chopper On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wCher",CherenkovOn);
    weighted_sum = CherenkovOn->Integral();
    CherenkovOn->Print();

    // Chopper Off Analysis
    AdOffFile->cd();
    AdOffFile->GetObject("wCher",CherenkovOff);
    weighted_sum2 = CherenkovOff->Integral();
    CherenkovOff->Print();
    cher_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
  }
  if(checkAOnFile)
  {
    // ******************************************************************************************************************************** //
    // Quick Incident Photocathode Analysis from Added Histogram File
    // ******************************************************************************************************************************** //
    std::cout << std::endl << "Photocathode Incident Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    // Chopper On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wDet",DetData);
    weighted_sum = DetData->Integral();
    DetData->Print();

    // Chopper Off Analysis
    AdOffFile->cd();
    AdOffFile->GetObject("wDet",DetDataOff);
    weighted_sum2 = DetDataOff->Integral();
    DetDataOff->Print();
    inc_det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
  }

  // ******************************************************************************************************************************** //
  // Quick Detected Analysis from Original File
  // ******************************************************************************************************************************** //
  
  std::cout << std::endl << "Photocathode Detected Analysis..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
  // Chopper On Analysis
  OGOnFile->cd();
  OGOnFile->GetObject("Detected",DetOn);
  weighted_sum = DetOn->Integral();
  DetOn->Print();
  
  // Chopper Off Analysis
  OGOffFile->cd();
  OGOffFile->GetObject("Detected",DetOff);
  weighted_sum2 = DetOff->Integral();
  DetOff->Print();
  det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
    
  // ******************************************************************************************************************************** //
  // Quick NRF to Cherenkov Analysis from Added Histograms File
  // ******************************************************************************************************************************** //
  if(checkEvents)
  {
    // NRF On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wNRF_NRF_to_Cher",wNRF_NRF_to_Cher);
    weighted_sum = wNRF_NRF_to_Cher->Integral();
    wNRF_NRF_to_Cher->Print();

    // NRF Off
    AdOffFile->cd();
    AdOffFile->GetObject("wNRF_NRF_to_Cher",wNRF_NRF_to_CherOff);
    weighted_sum2 = wNRF_NRF_to_CherOff->Integral();
    wNRF_NRF_to_CherOff->Print();

    // Cher On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wCher_NRF_to_Cher",wCher_NRF_to_Cher);
    weighted_sum3 = wCher_NRF_to_Cher->Integral();
    wCher_NRF_to_Cher->Print();
    // Cher Off Analysis
    AdOffFile->cd();
    AdOffFile->GetObject("wCher_NRF_to_Cher",wCher_NRF_to_CherOff);
    weighted_sum4 = wCher_NRF_to_CherOff->Integral();
    wCher_NRF_to_CherOff->Print();
    wNRFCher_to_NRF_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
    wCherCher_to_NRF_z = abs(weighted_sum3 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum3),2) + pow(sqrt(weighted_sum4),2)));

  // ******************************************************************************************************************************** //
  // Quick NRF to Cherenkov to Detected Analysis from Added Histograms File
  // ******************************************************************************************************************************** //

    // NRF On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wNRF_NRF_to_Cher_to_Det",wNRF_NRF_to_Cher_to_Det);
    weighted_sum = wNRF_NRF_to_Cher_to_Det->Integral();
    wNRF_NRF_to_Cher_to_Det->Print();

    // NRF Off
    AdOffFile->cd();
    AdOffFile->GetObject("wNRF_NRF_to_Cher_to_Det",wNRF_NRF_to_Cher_to_DetOff);
    weighted_sum2 = wNRF_NRF_to_Cher_to_DetOff->Integral();
    wNRF_NRF_to_Cher_to_DetOff->Print();

    // Cher On Analysis
    AdOnFile->cd();
    AdOnFile->GetObject("wCher_NRF_to_Cher_to_Det",wCher_NRF_to_Cher_to_Det);
    weighted_sum3 = wCher_NRF_to_Cher_to_Det->Integral();
    wCher_NRF_to_Cher_to_Det->Print();
    // Cher Off Analysis
    AdOffFile->cd();
    AdOffFile->GetObject("wCher_NRF_to_Cher",wCher_NRF_to_CherOff);
    weighted_sum4 = wCher_NRF_to_CherOff->Integral();
    wCher_NRF_to_CherOff->Print();
    wNRFCher_to_NRF_to_det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
    wCherCher_to_NRF_to_det_z = abs(weighted_sum3 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum3),2) + pow(sqrt(weighted_sum4),2)));
  }
  
  // Z Test Results
  std::cout << std::endl << "Z-Score Summary..." << std::endl;
  std::cout << "*************************************" << std::endl << std::endl;
  if(checkAdOnFile)
  {
    std::cout << "Chopper Incident Z-Score: " << chopper_in_z << std::endl;
    std::cout << "Chopper Emission Z-Score: " << chopper_out_z << std::endl;
  }
  std::cout << "Interrogation Object Incident Z-Score: " << IntObj_in_z << std::endl;
  std::cout << "Interrogation Object Emission Z-Score: " << IntObj_out_z << std::endl;
  std::cout << "Interrogation Object NRF Incident Z-Score: " << IntObj_nrf_in_z << std::endl;
  std::cout << "Interrogation Object NRF Emission Z-Score: " << IntObj_nrf_out_z << std::endl;
  std::cout << "Water Tank Incident Z-Score: " << water_in_z << std::endl;
  std::cout << "Water Tank NRF incident Z-Score: " << water_nrf_z << std::endl;
  if(checkAdOnFile && checkCherenkov)
  {
    std::cout << "Created Cherenkov Energy Z-Score: " << cher_z << std::endl;
  }
  if(checkAdOnFile)
  {
    std::cout << "Incident Photocathode Z-Score: " << inc_det_z << std::endl;
  }
  std::cout << "Detected Z-Score: " << det_z << std::endl;
  if(checkEvents)
  {
    std::cout << "NRF to Cherenkov NRF Spectrum Z-Score: " << wNRFCher_to_NRF_z << std::endl;
    std::cout << "NRF to Cherenkov Cherenkov Spectrum Z-Score: " << wCherCher_to_NRF_z << std::endl;
    std::cout << "NRF to Cherenkov to Detector NRF Spectrum Z-Score: " << wNRFCher_to_NRF_to_det_z << std::endl;
    std::cout << "NRF to Cherenkov to Detector Cherenkov Spectrum Z-Score: " << wCherCher_to_NRF_to_det_z << std::endl << std::endl;
  }
}
