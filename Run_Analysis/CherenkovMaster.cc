//
// Breaks up Cherenkov Merging for Large Runs

#include "Cherenkov.cc"

void CherenkovMaster(const char *InputFilenameBase, double Emax)
{
    time_t timer, timer2, time_start, time_end;
    std::string InFile = InputFilenameBase;
    InFile = InFile + ".root";
    const char* InputFilename = InFile.c_str();
    TFile *f = TFile::Open(InputFilename);
    TTree *Cherenkov;
    f->GetObject("Cherenkov",Cherenkov);
    Int_t n = Cherenkov->GetEntries();
    int maxEntriesPerFile = 1e7;
    double numFiles = n/maxEntriesPerFile;
    numFiles = ceil(numFiles);
    Int_t startEntry = 0;
    
    for(int i=0;i<numFiles;i++)
    {
        time_start = time(&timer);
        Cherenkov(InputFilenameBase, Emax, maxEntriesPerFile, startEntry, i);
        time_end = time(&timer);
        std::cout << "File " << i << " of " << numFiles << " complete!" << std::endl;
        std::cout << "Approximate time remaining: " << difftime(time_end, time_start)*(numFiles - i) << " seconds" << std::endl << std::endl;
        startEntry = (i*maxEntriesPerFile) + 1;
    }
}
