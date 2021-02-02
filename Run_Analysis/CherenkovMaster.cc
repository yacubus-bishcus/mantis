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
    TTree *Cherenkov_tree;
    f->GetObject("Cherenkov",Cherenkov_tree);
    Int_t n = Cherenkov_tree->GetEntries();
    int maxEntriesPerFile = 1e7;
    double numFiles = n/maxEntriesPerFile;
    numFiles = ceil(numFiles);
    Int_t startEntry = 0;
    bool lastFile = false;
    
    for(int i=0;i<numFiles;i++)
    {
        time_start = time(&timer);
        if(i == numFiles)
            lastFile = true;
        else
            lastFile = false;
        
        startEntry = Cherenkov(InputFilenameBase, Emax, maxEntriesPerFile, startEntry, i, lastFile);
        time_end = time(&timer);
        std::cout << "File " << i << " of " << numFiles << " complete!" << std::endl;
        std::cout << "Approximate time remaining: " << difftime(time_end, time_start)*(numFiles - i) << " seconds" << std::endl << std::endl;
    }
}
