//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File in ROOT CERN:
// root -b 'Rebin.cc("testOn.root", "ChopIn", "ChopIn_Weighted", 300, 2.1)'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 5 inputs
// 1. Filename
// 2. TTree Object to Weight and Rebin
// 3. Weighted and Rebined TH1D Object Name
// 4. Number of Bins
// 5. Max Energy of Histogram (MeV)
//
// This File Scans the File for the user's TTree Object Name. It then Draws
// that object to a new weighted histogram with the user's number of bins

void Rebin(const char* inFile, const char* ObjName, const char* OutObjName, int nbins, double Emax)
{

  // Check to make sure file exists
  if(gSystem->AccessPathName(inFile) != 0)
  {
    std::cerr << "ERROR Could not find " << inFile << "exiting..." << std::endl;
    exit(1);
  }
  TFile *f = new TFile(inFile);
  f->cd();
  TTree *inObj;
  // Grab the Users TTree
  inObj = (TTree*) f->Get(ObjName);
  inObj->Print();
  // Grab TTree Values
  Int_t nentries = inObj->Draw("Energy:Weight","","goff");
  Double_t *energies = inObj->GetVal(0);
  Double_t *weights = inObj->GetVal(1);

  // Fill Histogram 
  TH1D *hObj = new TH1D(OutObjName,"Weighted Energy Spectrum",nbins,0.,Emax);
  for(int i=0;i<nentries,++i)
  {
    hObj->Fill(energies[i],weights[i]);
  }

  hObj->Draw("HIST");
}
