//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File:
// root -b -q 'PrintResults.cc("testOn.root", "testOff.root", "WeightOn.root", WeightOff.root")'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 5 inputs
// 1. Chopper On Filename
// 2. Chopper Off Filename
// 3. Weight On Filename (Optional Default = NA)
// 4. Weight Off Filename (Optional Default = NA)
//
// This File Scans the Weighted Histogram File.
// This file prints to terminal all histogram
// entries, weighted sums, and Z-Scores.
//

void PrintResults(const char* ChopOn, const char* ChopOff, std::string WeightOn = "NA", std::string WeightOff = "NA")
{
    TFile *onFile, *offFile, *weightOnFile, *weightOffFile;
    
    // Do input checking
    
    if(!gSystem->AccessPathName(ChopOn))
    {
        std::cout << "On File Found." << std::endl;
        onFile = new TFile(ChopOn);
    }
    else
    {
        std::cerr << "On File not found." << std::endl;
        exit(100);
    }
    if(!gSystem->AccessPathName(ChopOff))
    {
        std::cout << "Off File Found." << std::endl;
        offFile = new TFile(ChopOff);
    }
    else
    {
        std::cerr << "Off File not found." << std::endl;
        exit(100);
    }
    
    if(!WeightOn.compare(0,2,"NA") || !WeightOff.compare(0,2,"NA"))
    {
        std::cout << "Not analyzing Weighted Histogram files." << std::endl;
    }
    else
    {
        std::cout << "Analyzing Weighted Histogram files." << std::endl;
        const char* WeightOnChar = WeightOn.c_str();
        const char* WeightOffChar = WeightOff.c_str();
        if(!gSystem->AccessPathName(WeightOnChar))
        {
            std::cout << "Weight Histogram File On Found." << std::endl;
            weightOnFile = new TFile(WeightOnChar);
        }
        else
        {
            std::cerr << "Weight Histogram File not found. " << std::endl;
            exit(100);
        }
        if(!gSystem->AccessPathName(WeightOffChar))
        {
            std::cout << "Weight Histogram File Off Found." << std::endl;
            weightOffFile = new TFile(WeightOffChar);
        }
        else
        {
            std::cerr << "Weight Histogram File not found. " << std::endl;
            exit(100);
        }
    }
  
    // Potential Histograms from OG Files
    TH1D *ChopInOn, *ChopInOff, *ChopOutOn, *ChopOutOff;
    TH1D *NRFMat, *NRFMatOff;
    TH1D *IntObjInOn, *IntObjOutOn, *IntObjInOff, *IntObjOutOff, *IntNRFInOn, *IntNRFOutOn, *IntNRFInOff, *IntNRFOutOff;
    TH1D *CherenkovOn, *CherenkovOff;
    TH1D *WaterInOn, *WaterInOff, *WaterNRFOn, *WaterNRFOff;
    TH1D *DetData, *DetDataOff;
    TH1D *DetOn, *DetOff;

    // Potential Histograms from Added Weighted Histograms File
    TH1D *wNRF_NRF_to_Cher, *wNRF_NRF_to_CherOff, *wCher_NRF_to_Cher, *wCher_NRF_to_CherOff;
    TH1D *wNRF_NRF_to_Cher_to_Det, *wNRF_NRF_to_Cher_to_DetOff, *wCher_NRF_to_Cher_to_Det, *wCher_NRF_to_Cher_to_DetOff;
  
    // ints, doubles, strings
    double weighted_sum, weighted_sum2, weighted_sum3, weighted_sum4, weighted_sum5, weighted_sum6, weighted_sum7, weighted_sum8;
    double wNRFCher_to_NRF_to_det_z, wCherCher_to_NRF_to_det_z, wNRFCher_to_NRF_z, wCherCher_to_NRF_z;

  // ******************************************************************************************************************************** //
  // Variables Declared Objects Set up
  // ******************************************************************************************************************************** //
    
    // ******************************************************************************************************************************** //
    // Quick Chopper Analysis from OG File
    // ******************************************************************************************************************************** //
    std::cout << "Chopper Wheel Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    // Chopper On Analysis
    onFile->cd();
    onFile->GetObject("ChopperIn_Weighted",ChopInOn);
    weighted_sum = ChopInOn->Integral();
    ChopInOn->Print();
    onFile->GetObject("ChopperOut_Weighted",ChopOutOn);
    ChopOutOn->Print();
    weighted_sum2 = ChopOutOn->Integral();

    // Chopper Off Analysis
    offFile->cd();
    offFile->GetObject("ChopperIn_Weighted",ChopInOff);
    weighted_sum3 = ChopInOff->Integral();
    ChopInOff->Print();
    offFile->GetObject("ChopperOut_Weighted",ChopOutOff);
    weighted_sum4 = ChopOutOff->Integral();
    ChopOutOff->Print();

    double chopper_in_z = abs(weighted_sum - weighted_sum3)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum3),2)));
    double chopper_out_z = abs(weighted_sum2 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum4),2)));

    // ******************************************************************************************************************************** //
    // Quick Interrogation Object Analysis from Original File
    // ******************************************************************************************************************************** //

    std::cout << std::endl << "Interrogation Object Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    // Chopper On Analysis
    onFile->cd();
    onFile->GetObject("IntObjIn",IntObjInOn);
    onFile->GetObject("IntObjOut",IntObjOutOn);
    onFile->GetObject("NRFIntObjIn",IntNRFInOn);
    onFile->GetObject("NRFIntObjOut",IntNRFOutOn);
    weighted_sum = IntObjInOn->Integral();
    IntObjInOn->Print();
    weighted_sum2 = IntObjOutOn->Integral();
    IntObjOutOn->Print();
    weighted_sum3 = IntNRFInOn->Integral();
    IntNRFInOn->Print();
    weighted_sum4 = IntNRFOutOn->Integral();
    IntNRFOutOn->Print();

    // Chopper Off Analysis
    offFile->cd();
    offFile->GetObject("IntObjIn",IntObjInOff);
    offFile->GetObject("IntObjOut",IntObjOutOff);
    offFile->GetObject("NRFIntObjIn",IntNRFInOff);
    offFile->GetObject("NRFIntObjOut",IntNRFOutOff);
    weighted_sum5 = IntObjInOff->Integral();
    IntObjInOff->Print();
    weighted_sum6 = IntObjOutOff->Integral();
    IntObjOutOff->Print();
    weighted_sum7 = IntNRFInOff->Integral();
    IntNRFInOff->Print();
    weighted_sum8 = IntNRFOutOff->Integral();
    IntNRFOutOff->Print();
    double IntObj_in_z = abs(weighted_sum - weighted_sum5)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum5),2)));
    double IntObj_out_z = abs(weighted_sum2 - weighted_sum6)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum6),2)));
    double IntObj_nrf_in_z = abs(weighted_sum3 - weighted_sum7)/(sqrt(pow(sqrt(weighted_sum3),2) + pow(sqrt(weighted_sum7),2)));
    double IntObj_nrf_out_z = abs(weighted_sum4 - weighted_sum8)/(sqrt(pow(sqrt(weighted_sum4),2) + pow(sqrt(weighted_sum8),2)));

    // ******************************************************************************************************************************** //
    // Quick Incident Water Tank Analysis from Original File
    // ******************************************************************************************************************************** //

    std::cout << std::endl << "Water Tank Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    // Chopper On Analysis
    onFile->cd();
    onFile->GetObject("WaterIn",WaterInOn);
    onFile->GetObject("NRFWaterIn",WaterNRFOn);
    weighted_sum = WaterInOn->Integral();
    WaterInOn->Print();
    weighted_sum2 = WaterNRFOn->Integral();
    WaterNRFOn->Print();

    // Chopper Off Analysis
    offFile->cd();
    offFile->GetObject("WaterIn",WaterInOff);
    offFile->GetObject("NRFWaterIn",WaterNRFOff);
    weighted_sum3 = WaterInOff->Integral();
    WaterInOff->Print();
    weighted_sum4 = WaterNRFOff->Integral();
    WaterNRFOff->Print();
    double water_in_z = abs(weighted_sum - weighted_sum3)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum3),2)));
    double water_nrf_z = abs(weighted_sum2 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum2),2) + pow(sqrt(weighted_sum4),2)));

    // ******************************************************************************************************************************** //
    // Quick Cherenkov Analysis from OG File
    // ******************************************************************************************************************************** //

    std::cout << std::endl << "Cherenkov Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    // Chopper On Analysis
    onFile->cd();
    onFile->GetObject("Cherenkov_Weighted",CherenkovOn);
    weighted_sum = CherenkovOn->Integral();
    CherenkovOn->Print();

    // Chopper Off Analysis
    offFile->cd();
    offFile->GetObject("Cherenkov_Weighted",CherenkovOff);
    weighted_sum2 = CherenkovOff->Integral();
    CherenkovOff->Print();
    double cher_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));

    // ******************************************************************************************************************************** //
    // Quick Incident Detector Analysis from OG Files
    // ******************************************************************************************************************************** //

    std::cout << std::endl << "Photocathode Incident Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    // Chopper On Analysis
    onFile->cd();
    onFile->GetObject("Inc_Det_Weighted",DetData);
    weighted_sum = DetData->Integral();
    DetData->Print();

    // Chopper Off Analysis
    offFile->cd();
    offFile->GetObject("Inc_Det_Weighted",DetDataOff);
    weighted_sum2 = DetDataOff->Integral();
    DetDataOff->Print();
    double inc_det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));


    // ******************************************************************************************************************************** //
    // Quick Detected Analysis from Original File
    // ******************************************************************************************************************************** //

    std::cout << std::endl << "Photocathode Detected Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    // Chopper On Analysis
    onFile->cd();
    onFile->GetObject("Detected_Weighted",DetOn);
    weighted_sum = DetOn->Integral();
    DetOn->Print();

    // Chopper Off Analysis
    offFile->cd();
    offFile->GetObject("Detected_Weighted",DetOff);
    weighted_sum2 = DetOff->Integral();
    DetOff->Print();
    double det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
    
  // ******************************************************************************************************************************** //
  // Quick NRF to Cherenkov Analysis from Added Histograms File
  // ******************************************************************************************************************************** //
  if(!WeightOn.compare(0,2,"NA") || !WeightOff.compare(0,2,"NA"))
  {
      std::cout << "Conducting Z-Score Tests..." << std::endl;
  }
  else
  {
      if(weightOnFile != NULL)
      {
        // NRF On Analysis
        weightOnFile->cd();
        weightOnFile->GetObject("wNRF_NRF_to_Cher",wNRF_NRF_to_Cher);
        weighted_sum = wNRF_NRF_to_Cher->Integral();
        wNRF_NRF_to_Cher->Print();

        // NRF Off Analysis
        weightOffFile->cd();
        weightOffFile->GetObject("wNRF_NRF_to_Cher",wNRF_NRF_to_CherOff);
        weighted_sum2 = wNRF_NRF_to_CherOff->Integral();
        wNRF_NRF_to_CherOff->Print();

        // Cher On Analysis
        weightOnFile->cd();
        weightOnFile->GetObject("wCher_NRF_to_Cher",wCher_NRF_to_Cher);
        weighted_sum3 = wCher_NRF_to_Cher->Integral();
        wCher_NRF_to_Cher->Print();
        // Cher Off Analysis
        weightOffFile->cd();
        weightOffFile->GetObject("wCher_NRF_to_Cher",wCher_NRF_to_CherOff);
        weighted_sum4 = wCher_NRF_to_CherOff->Integral();
        wCher_NRF_to_CherOff->Print();
        wNRFCher_to_NRF_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
        wCherCher_to_NRF_z = abs(weighted_sum3 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum3),2) + pow(sqrt(weighted_sum4),2)));

      // ******************************************************************************************************************************** //
      // Quick NRF to Cherenkov to Detected Analysis from Added Histograms File
      // ******************************************************************************************************************************** //

        // NRF On Analysis
        weightOnFile->cd();
        weightOnFile->GetObject("wNRF_NRF_to_Cher_to_Det",wNRF_NRF_to_Cher_to_Det);
        weighted_sum = wNRF_NRF_to_Cher_to_Det->Integral();
        wNRF_NRF_to_Cher_to_Det->Print();

        // NRF Off
        weightOffFile->cd();
        weightOffFile->GetObject("wNRF_NRF_to_Cher_to_Det",wNRF_NRF_to_Cher_to_DetOff);
        weighted_sum2 = wNRF_NRF_to_Cher_to_DetOff->Integral();
        wNRF_NRF_to_Cher_to_DetOff->Print();

        // Cher On Analysis
        weightOnFile->cd();
        weightOnFile->GetObject("wCher_NRF_to_Cher_to_Det",wCher_NRF_to_Cher_to_Det);
        weighted_sum3 = wCher_NRF_to_Cher_to_Det->Integral();
        wCher_NRF_to_Cher_to_Det->Print();
        // Cher Off Analysis
        weightOffFile->cd();
        weightOffFile->GetObject("wCher_NRF_to_Cher",wCher_NRF_to_CherOff);
        weighted_sum4 = wCher_NRF_to_CherOff->Integral();
        wCher_NRF_to_CherOff->Print();
        wNRFCher_to_NRF_to_det_z = abs(weighted_sum - weighted_sum2)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum2),2)));
        wCherCher_to_NRF_to_det_z = abs(weighted_sum3 - weighted_sum4)/(sqrt(pow(sqrt(weighted_sum3),2) + pow(sqrt(weighted_sum4),2)));
      }
  }

// ******************************************************************************************************************************** //
// Conduct Z Score Tests
// ******************************************************************************************************************************** //
    
    // OG File
    std::cout << std::endl << "Z-Score Summary..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    std::cout << "Chopper Incident Z-Score: " << chopper_in_z << std::endl;
    std::cout << "Chopper Emission Z-Score: " << chopper_out_z << std::endl;
    std::cout << "Interrogation Object Incident Z-Score: " << IntObj_in_z << std::endl;
    std::cout << "Interrogation Object Emission Z-Score: " << IntObj_out_z << std::endl;
    std::cout << "Interrogation Object NRF Incident Z-Score: " << IntObj_nrf_in_z << std::endl;
    std::cout << "Interrogation Object NRF Emission Z-Score: " << IntObj_nrf_out_z << std::endl;
    std::cout << "Water Tank Incident Z-Score: " << water_in_z << std::endl;
    std::cout << "Water Tank NRF incident Z-Score: " << water_nrf_z << std::endl;
    std::cout << "Created Cherenkov Energy Z-Score: " << cher_z << std::endl;
    std::cout << "Incident Photocathode Z-Score: " << inc_det_z << std::endl;
    std::cout << "Detected Z-Score: " << det_z << std::endl;
    
    // Weighted File
    if(WeightOn.compare(0,2,"NA") && WeightOff.compare(0,2,"NA"))
    {
        if(weightOnFile != NULL)
        {
            std::cout << "NRF to Cherenkov NRF Spectrum Z-Score: " << wNRFCher_to_NRF_z << std::endl;
            std::cout << "NRF to Cherenkov Cherenkov Spectrum Z-Score: " << wCherCher_to_NRF_z << std::endl;
            std::cout << "NRF to Cherenkov to Detector NRF Spectrum Z-Score: " << wNRFCher_to_NRF_to_det_z << std::endl;
            std::cout << "NRF to Cherenkov to Detector Cherenkov Spectrum Z-Score: " << wCherCher_to_NRF_to_det_z << std::endl << std::endl;
        }
    }
    
// End of Analysis
}
