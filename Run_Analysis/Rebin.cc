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
// 1. Filename. const char*
// 2. TTree Object to Weight and Rebin. const char*
// 3. Weighted and Rebined TH1D Object Name. const char*
// 4. Number of Bins int
// 5. Max Energy of Histogram (MeV). double
//
// Additional Optional inputs are Available to the User for variable bin drawing
// 1. Variable Bin Width. bool
// 2. The NRF Bin Width. double
// 3. Importance Sampling Energy Region split. double
// 4. Importance sampling region weighting. double
//
// This File Scans the File for the user's TTree Object Name. It then Draws
// that object to a new weighted histogram with the user's number of bins

void Rebin(const char* inFile, const char* ObjName, const char* OutObjName,
  int nbins, double Emax,
  bool VarArray=false, double nrf_bin_width = -1., double sample_energy=-1.,
  double sample_weight=-1.)
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
  TH1D *hObj;
  // If User wants fixed bin widths
  if(!VarArray)
  {
    hObj = new TH1D(OutObjName,"Weighted Energy Spectrum",nbins,0.,Emax);
  }
  // User wants variable bin widths
  else
  {
    if(sample_energy < 0 || sample_weight < 0 || nrf_bin_width < 0)
    {
      std::cerr << "Error User must input Energy and Weight of region split." << std::endl;
      exit(1);
    }

    // Find total number of bins
    double bin_increment = nrf_bin_width*sample_weight;
    double edge_counter = 0.;
    int nbins1 = sample_energy/(bin_increment);
    int nbins2 = (Emax - sample_energy)/nrf_bin_width;
    Int_t tbins = nbins1 + nbins2;
    // create edges (dynamically sized array)
    Double_t* edges = new Double_t[tbins+1];
    // fill the edges for the first region
    for(int i=0;i<nbins1+1;++i)
    {
      edges[i] = edge_counter;
      edge_counter += bin_increment;
    }
    // fill the edges for the second region
    edge_counter = edge_counter - bin_increment;
    for(int i=nbins1;i<tbins+2;++i)
    {
      edges[i] = edge_counter;
      edge_counter += nrf_bin_width;
    }

    // edges complete now create new histogram
    hObj = new TH1D(OutObjName,"Weighted Energy Spectrum w/ Variable Bins",tbins,edges);

  }// end !VarArray

  // Fill the new Histogram
  for(int i=0;i<nentries;++i)
  {
    hObj->Fill(energies[i],weights[i]);
  }

  // Write to OutFile
  std::string OutFileName = "rebinned_" + (std::string)inFile;
  TFile *fout = new TFile(OutFileName.c_str(),"recreate");
  fout->cd();
  hObj->Write();
  std::cout << "Rebinned Histogram written to: " << OutFileName << std::endl;
  fout->Close();
  //delete []edges;

}// end Rebin.cc
