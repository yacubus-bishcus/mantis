#include "WeightHisto.cc"
#include "EventCheck.cc"
#include "Cherenkov.cc"
#include "PrintResults.cc"

void Analysis(const char* InputFilenameOn, const char* InputFilenameOff, const char* OutputFilenameBase, double Emax)
{
    std::cout << "Weighting histograms, Checking Events, and Merging Cherenkov!" << std::endl;
    // Chopper On
    WeightHisto(InputFilenameOn, OutputFilenameBase, Emax, true);
    EventCheck(InputFilenameOn, OutputFilenameBase, true);
    Cherenkov(InputFilenameOn, OutputFilenameBase, Emax, true);
    // Chopper Off
    WeightHisto(InputFilenameOff, OutputFilenameBase, Emax, false);
    EventCheck(InputFilenameOff, OutputFilenameBase, false);
    Cherenkov(InputFilenameOff, OutputFilenameBase, Emax, false);
    
    PrintResults(InputFilenameOn, InputFilenameOff);
    std::cout << "Analysis Complete" << std::endl;
}
