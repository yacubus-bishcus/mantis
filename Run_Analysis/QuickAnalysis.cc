// To Run in Root: root -b -q 'QuickAnalysis.cc("ChopOn.root",ChopOff.root")'
// Analysis Prints Data for Chop On/Off Interrogation Object NRF/Full Spectrum Difference, 
// Water Tank NRF/Full Low/High Energy Spectrum Difference, 
// Incident Detector Low/High Energy Spectrum Difference, 
// and Detected Difference 
void QuickAnalysis(const char *ChopOn, const char *ChopOff)
{
    std::cout << "Setting Chop On File: " << ChopOn << std::endl;
    std::cout << "Setting Chop Off File: " << ChopOff << std::endl;
    string response = "Processing an older file structure...";
    
    TFile *chopOn = new TFile(ChopOn);
    TFile *chopOff = new TFile(ChopOff);
    bool confirmation = chopOn->cd();
    
    // Declare Variables 
    
    // TTrees 
    TTree *Cherenkov;
    TTree *CherenkovOff;
    
    // Histograms 
    // Interrogation Object Analysis
    TH1D *IncObj;
    TH1D *IncObjOff;
    TH1D *NRFIncObj;
    TH1D *NRFIncObjOff;
    
    // Water Tank Analysis
    TH1D *IncWaterLow;
    TH1D *IncWaterLowOff;
    TH1D *IncWaterHigh;
    TH1D *IncWaterHighOff;
    TH1D *NRFIncWater;
    TH1D *NRFIncWaterOff;
    
    // Photocathode Analysis 
    TH1D *IncDetLow;
    TH1D *IncDetLowOff;
    TH1D *IncDetHigh;
    TH1D *IncDetHighOff;
    TH1D *Detected;
    TH1D *DetectedOff;
    
    // Doubles 
    
    // Cherenkov Analysis
    double cherenkov, cherenkovOff;
    
    // Interrogation Object Analysis
    double intObj_entries, intObj_sum, intObj_entries_off, intObj_sum_off;
    double nrf_intObj_entries, nrf_intObj_sum, nrf_intObj_entries_off, nrf_intObj_sum_off;
    
    // Water Tank Analysis 
    double waterl_entries, waterl_sum, waterl_entries_off, waterl_sum_off;
    double waterh_entries, waterh_sum, waterh_entries_off, waterh_sum_off;
    double nrf_water_entries, nrf_water_sum, nrf_water_entries_off, nrf_water_sum_off;
    
    // Photocathode Analysis
    double pcl_entries, pcl_sum, pcl_entries_off, pcl_sum_off;
    double pch_entries, pch_sum, pch_entries_off, pch_sum_off;
    double det_entries, det_sum, det_entries_off, det_sum_off;
    
    // z-score
    // Cherenkov Analysis
    double cherenkov_z;
    // Interrogation Object Analysis
    double intObj_z;
    double nrfIntObj_z;
    // Water Tank Analysis
    double waterl_z;
    double waterh_z;
    double nrf_water_z;
    // Photocathode Analysis
    double pcl_z;
    double pch_z;
    double det_z;
    
    // Variables Set, Complete Calculations 
    
    if(confirmation)
    {
        // Get Objects from ON File 
        // Cherenkov
        chopOn->GetObject("Cherenkov", Cherenkov);
        // Interrogation Object
        chopOn->GetObject("IncObj", IncObj);
        chopOn->GetObject("NRFIncObj",NRFIncObj);
        // Water Tank 
        chopOn->GetObject("IncWaterLow", IncWaterLow);
        chopOn->GetObject("IncWaterHigh", IncWaterHigh);
        chopOn->GetObject("NRFIncWater", NRFIncWater);
        // Photocathode 
        chopOn->GetObject("IncDetLow",IncDetLow);
        chopOn->GetObject("IncDetHigh", IncDetHigh);
        chopOn->GetObject("Detected",Detected);
        
        // Get Entries and Sums 
        // Cherenkov 
        cherenkov = Cherenkov->GetEntries();
        // Interrogation Object
        intObj_entries = IncObj->GetEntries();
        intObj_sum = IncObj->Integral();
        nrf_intObj_entries = NRFIncObj->GetEntries();
        nrf_intObj_sum = NRFIncObj->Integral();
        // Water Tank 
        waterl_entries = IncWaterLow->GetEntries();
        waterl_sum = IncWaterLow->Integral();
        waterh_entries = IncWaterHigh->GetEntries();
        waterh_sum = IncWaterHigh->Integral();
        nrf_water_entries = NRFIncWater->GetEntries();
        nrf_water_sum = NRFIncWater->Integral();
        // Photocathode
        pcl_entries = IncDetLow->GetEntries();
        pcl_sum = IncDetLow->Integral();
        pch_entries = IncDetHigh->GetEntries();
        pch_sum = IncDetHigh->Integral();
        det_entries = Detected->GetEntries();
        det_sum = Detected->Integral();
    }
    else
    {
        std::cerr << "ERROR: Chopper On File with name: " << ChopOn << " not opened!!!"<< std::endl;
        return;
    }
    
    bool confirmation2 = chopOff->cd();
    if(confirmation2)
    {
        // Get Objects from OFF File 
        // Cherenkov
        chopOff->GetObject("Cherenkov", CherenkovOff);
        // Interrogation Object
        chopOff->GetObject("IncObj", IncObjOff);
        chopOff->GetObject("NRFIncObj",NRFIncObjOff);
        // Water Tank 
        chopOff->GetObject("IncWaterLow", IncWaterLowOff);
        chopOff->GetObject("IncWaterHigh", IncWaterHighOff);
        chopOff->GetObject("NRFIncWater", NRFIncWaterOff);
        // Photocathode 
        chopOff->GetObject("IncDetLow",IncDetLowOff);
        chopOff->GetObject("IncDetHigh", IncDetHighOff);
        chopOff->GetObject("Detected",DetectedOff);
        
        // Get Entries and Sums 
        // Cherenkov 
        cherenkovOff = CherenkovOff->GetEntries();
        // Interrogation Object
        intObj_entries_off = IncObjOff->GetEntries();
        intObj_sum_off = IncObjOff->Integral();
        nrf_intObj_entries_off = NRFIncObjOff->GetEntries();
        nrf_intObj_sum_off = NRFIncObjOff->Integral();
        // Water Tank 
        waterl_entries_off = IncWaterLowOff->GetEntries();
        waterl_sum_off = IncWaterLowOff->Integral();
        waterh_entries_off = IncWaterHighOff->GetEntries();
        waterh_sum_off = IncWaterHighOff->Integral();
        nrf_water_entries_off = NRFIncWaterOff->GetEntries();
        nrf_water_sum_off = NRFIncWaterOff->Integral();
        // Photocathode
        pcl_entries_off = IncDetLowOff->GetEntries();
        pcl_sum_off = IncDetLowOff->Integral();
        pch_entries_off = IncDetHighOff->GetEntries();
        pch_sum_off = IncDetHighOff->Integral();
        det_entries_off = DetectedOff->GetEntries();
        det_sum_off = DetectedOff->Integral();    
    }
    else
    {
        std::cerr << "ERROR: Chopper Off File with name: " << ChopOff << " not opened!!!"<<std::endl;
        return;
    }
    
    // Calculate z-score
   
    cherenkov_z = abs(cherenkov - cherenkovOff)/sqrt(pow(sqrt(cherenkov),2) + pow(sqrt(cherenkovOff),2)));
    intObj_z = abs(intObj_sum - intObj_sum_off)/sqrt(pow(sqrt(intObj_sum),2) + pow(sqrt(intObj_sum_off),2)));
    nrfIntObj_z = abs(nrf_intObj_sum - nrf_intObj_sum_off)/sqrt(pow(sqrt(nrf_intObj_sum),2) + pow(sqrt(nrf_intObj_sum_off),2)));
    waterl_z = abs(waterl_sum - waterl_sum_off)/sqrt(pow(sqrt(waterl_sum),2) + pow(sqrt(waterl_sum_off),2)));
    waterh_z = abs(waterh_sum - waterh_sum_off)/sqrt(pow(sqrt(waterh_sum),2) + pow(sqrt(waterh_sum_off),2)));
    nrf_water_z = abs(nrf_water_sum - nrf_water_sum_off)/sqrt(pow(sqrt(nrf_water_sum),2) + pow(sqrt(nrf_water_sum_off),2)));
    pcl_z = abs(pcl_sum - pcl_sum_off)/sqrt(pow(sqrt(pcl_sum),2) + pow(sqrt(pcl_sum_off),2)));
    pch_z = abs(pch_sum - pch_sum_off)/sqrt(pow(sqrt(pch_sum),2) + pow(sqrt(pch_sum_off),2)));
    det_z = abs(det_sum - det_sum_off)/sqrt(pow(sqrt(det_sum),2) + pow(sqrt(det_sum_off),2)));
    
    
    // Print Results 
    
    // Cherenkov Analysis
    std::cout << "Cherenkov Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    std::cout << "Chopper On Cherenkov Entries: " << cherenkov << std::endl;
    std::cout << "Chopper Off Cherenkov Entries: " << cherenkovOff << std::endl;
    std::cout << "Cherenkov Z-Score: " << cherenkov_z << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    
    // Interrogation Object Analysis 
    std::cout << "Interrogation Object Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    std::cout << "Chopper On Interrogation Object Entries: " << intObj_entries << " On Sum: " << intObj_sum << std::endl;
    std::cout << "Chopper Off Interrogation Object Entries: " << intObj_entries_off << " Off Sum: " << intObj_sum_off << std::endl;
    std::cout << "Interrogation Object Z-test result: " << intObj_z << std::endl;
    std::cout << "Chopper On Interrogation Object NRF Entries: " << nrf_intObj_entries << " On Sum: " << nrf_intObj_sum << std::endl;
    std::cout << "Chopper Off Interrogation Object NRF Entries: " << nrf_intObj_entries_off << " Off Sum: " << nrf_intObj_sum_off << std::endl;
    std::cout << "Interrogation Object NRF Z-test result: " << nrfIntObj_z << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    
    // Water Tank Analysis
    std::cout << "Water Tank Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    std::cout << "Chopper On Water Tank Low Energy Entries: " << waterl_entries << " On Sum: " << waterl_sum << std::endl;
    std::cout << "Chopper Off Water Tank Low Energy Entries: " << waterl_entries_off << " Off Sum: " << waterl_sum_off << std::endl;
    std::cout << "Water Tank Low Energy Z-test result: " << waterl_z << std::endl;
    std::cout << "Chopper On Water Tank High Energy Entries: " << waterh_entries << " On Sum: " << waterh_sum << std::endl;
    std::cout << "Chopper Off Water Tank High Energy Entries: " << waterh_entries_off << " Off Sum: " << waterh_sum_off << std::endl;
    std::cout << "Water Tank High Energy Z-test result: " << waterh_z << std::endl;
    std::cout << "Chopper On Water Tank NRF Entries: " << nrf_water_entries << " On Sum: " << nrf_water_sum << std::endl;
    std::cout << "Chopper Off Water Tank NRF Entries: " << nrf_water_entries_off << " Off Sum: " << nrf_water_sum_off << std::endl;
    std::cout << "Water Tank NRF Z-test result: " << nrf_water_z << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    
    // Photocathode Analysis 
    std::cout << "Photocathode Analysis..." << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    std::cout << "Chopper On Incident Photocathode Low Energy Entries: " << pcl_entries << " On Sum: " << pcl_sum << std::endl;
    std::cout << "Chopper Off Incident Photocathode Low Energy Entries: " << pcl_entries_off << " Off Sum: " << pcl_sum_off  << std::endl;
    std::cout << "Incident Photocathode Low Energy Z-test result: " << pcl_z  << std::endl;
    std::cout << "Chopper On Incident Photocathode High Energy Entries: " << pch_entries << " On Sum: " << pch_sum << std::endl;
    std::cout << "Chopper Off Incident Photocathode High Energy Entries: " << pch_entries_off << " Off Sum: " << pch_sum_off << std::endl;
    std::cout << "Incident Photocathode High Energy Z-test result: " << pch_z << std::endl;
    std::cout << "Chopper On Detected Entries: " << det_entries << " On Sum: " << det_sum << std::endl;
    std::cout << "Chopper Off Detected Entries: " << det_entries_off << " Off Sum: " << det_sum_off << std::endl;
    std::cout << "Detected Z-test result: " << det_z << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;
    
}
