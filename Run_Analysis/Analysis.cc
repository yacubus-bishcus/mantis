//
// ************************************************************************************************ // 
// ************************************************************************************************ //
// To Run:
// root -b -q 'Analysis.cc("testOn","testOff", 2.1)'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
// 
// Execution of this script in ROOT CERN executes 4 other scripts in ROOT CERN 
// 1. Cherenkov.cc
// 2. EventCheck.cc
// 3. WeightHisto.cc
// 4. PrintResults.cc
// 
// Analysis.cc takes 4 inputs:
// 1. Chopper On File root name (without .root extension)
// 2. Chopper Off File root name (without .root extension)
// 3. OutputFile root name (without .root extension)
// 4. Max Energy of Bremsstrahlung Interrogation Beam 
//
// ************************************************************************************************ //
// ************************************************************************************************ //

#include "WeightHisto.cc"
#include "EventCheck.cc"
#include "Cherenkov.cc"
#include "PrintResults.cc"

void Analysis(const char* InputFilenameBaseOn, 
              const char* InputFilenameBaseOff, 
              const char* OutputFilenameBase, double Emax)
{
    std::cout << "Merging Cherenkov, Checking Events, and Writing Weighted Histograms!" << std::endl;
    std::string inFileOn = InputFilenameBaseOn;
    std::string inFileOff = InputFilenameBaseOff;
    inFileOn = inFileOn + ".root";
    inFileOff = inFileOff + ".root";
    const char* InputFilenameOn = inFileOn.c_str();
    const char* InputFilenameOff = inFileOff.c_str();
    // Chopper On
    Cherenkov(InputFilenameOn, OutputFilenameBase, Emax, true);
    EventCheck(InputFilenameBaseOn, OutputFilenameBase, true);
    WeightHisto(InputFilenameBaseOn, OutputFilenameBase, Emax, true);
    // Chopper Off
    Cherenkov(InputFilenameOff, OutputFilenameBase, Emax, false);
    EventCheck(InputFilenameBaseOff, OutputFilenameBase, false);
    WeightHisto(InputFilenameBaseOff, OutputFilenameBase, Emax, false);
    
    PrintResults(InputFilenameBaseOn, InputFilenameBaseOff);
    std::cout << std::endl << "Analysis Complete" << std::endl;
}
