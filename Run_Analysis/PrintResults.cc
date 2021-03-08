//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File:
// root -b -q 'PrintResults.cc("testOn.root", "testOff.root","hChop")'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 3 inputs
// 1. Chopper On Filename
// 2. Chopper Off Filename
// 3. Histogram name
//
// This File Scans the Weighted Histogram File.
// This file prints to terminal all histogram
// entries, weighted sums, and Z-Scores.
//

void PrintResults(const char* ChopOn, const char* ChopOff, const char* histName)
{
    // Do input checking

    if(gSystem->AccessPathName(ChopOn) || gSystem->AccessPathName(ChopOff))
    {
      std::cerr << "File not found." << std::endl;
      exit(100);
    }

    TFile *onFile = new TFile(ChopOn);
    TFile *offFile = new TFile(ChopOff);
    onFile->cd();
    TH1D *onHist;
    onFile->GetObject(histName,onHist);

    if(onHist != 0 || tDetInfo != 0)
    {
      int nentries = onHist->GetEntries();

      if(nentries <=0)
        exit(1);
    }
    else
      exit(1);

    std::cout << std::endl << "Chopper On " << histName << "Data..." << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl << std::endl;

    onHist->Print();


    TH1D *offHist;
    double weighted_sum;

    if(histName.compare(0,4, "hDet"))
    {
      weighted_sum = onHist->Integral();
    }
    else
    {
      weighted_sum = onHist->Integral(0,1);
    }

    offFile->cd();
    offFile->GetObject(histName,offHist);

    std::cout << std::endl << "Chopper Off " << histName << "Data..." << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl << std::endl;

    if(offHist != 0 || offDetInfo != 0)
    {
      int nentries = offHist->GetEntries();

      if(nentries <=0)
        exit(1);
    }
    else
      exit(1);

    double weighted_sum_off;
    offHist->Print();

    if(histName.compare(0,4, "hDet"))
      weighted_sum_off = offHist->Integral();
    else
      weighted_sum_off = offHist->Integral(0,1);

    double z_score = abs(weighted_sum - weighted_sum_off)/(sqrt(pow(sqrt(weighted_sum),2) + pow(sqrt(weighted_sum_off),2)));
    std::cout << std::endl << histName << " Z-Score: " << z_score << std::endl << std::endl;
}
