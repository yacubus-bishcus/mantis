#include "WeightHisto.cc"
#include "EventCheck.cc"
#include "Cherenkov.cc"
#include "PrintResults.cc"

void Analysis(const char* InputFilenameBaseOn, const char* InputFilenameBaseOff, const char* OutputFilenameBase, double Emax)
{
    std::cout << "Weighting histograms, Checking Events, and Merging Cherenkov!" << std::endl;
    std::string inFileOn = InputFilenameBaseOn;
    std::string inFileOff = InputFilenameBaseOff;
    inFileOn = inFileOn + ".root";
    inFileOff = inFileOff + ".root";
    const char* InputFilenameOn = inFileOn.c_str();
    const char* InputFilenameOff = inFileOff.c_str();
    // Chopper On
    WeightHisto(InputFilenameOn, OutputFilenameBase, Emax, true);
    EventCheck(InputFilenameOn, OutputFilenameBase, true);
    Cherenkov(InputFilenameOn, OutputFilenameBase, Emax, true);
    // Chopper Off
    WeightHisto(InputFilenameOff, OutputFilenameBase, Emax, false);
    EventCheck(InputFilenameOff, OutputFilenameBase, false);
    Cherenkov(InputFilenameOff, OutputFilenameBase, Emax, false);
    
    PrintResults(InputFilenameBaseOn, InputFilenameBaseOff);
    std::cout << std::endl << "Analysis Complete" << std::endl;
}
