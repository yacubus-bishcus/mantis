//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File in ROOT CERN:
// root -b 'Rebin.cc("testOn.root", "ChopIn", "ChopIn_Weighted", 300, 2.1)'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 3 inputs
// 1. Verbosity. bool
// 2. Filename. const char*
// 3. TTree Object to Weight and Rebin. const char*
// Other Input Options
// 4. Weighted and Rebined TH1D Object Name. const char* Default = "hOut"
// 5. Number of Bins. int Default = 120,000
// 6. Min Energy of Histogram (MeV) for a constant bin width histogram or
//    the Max Energy of the non nrf region.  Default set as 0.0 MeV. double
// 7. Max Energy of Histogram (MeV) Default set as 2.1 MeV. double
// 8. TCut to place on TTree prior to Histogramming. TCut Default = "NA"
// 9. Variable Bin Width Bool. bool Default = false
// 10. NRF Area of Interest Bin Width. double Default = -1
// 11. Non-NRF Area of Interest Bin Width. double Default = -1
//
// This File Scans the File for the user's TTree Object Name. It then Draws
// that object to a new weighted histogram with the user's number of bins

void Rebin(bool verbose, const char* inFile, const char* ObjName, const char* OutObjName,
  int nbins, double Emin = 0.0, double Emax=2.1, TCut cut1="NA",
  bool VarArray=false, double nrf_bin_width = -1., double non_nrf_bin_width =-1.)
{

  // Check to make sure file exists
  if(gSystem->AccessPathName(inFile))
  {
    std::cerr << "ERROR Could not find " << inFile << "exiting..." << std::endl;
    exit(1);
  }

  // Complete User Input Checks
  if(VarArray && (nrf_bin_width < 0. || non_nrf_bin_width < 0.))
  {
    std::cerr << "USER ERROR. Variable bin widths selected without user selecting "
                << "the magnitude of the bin widths. Exiting..." << std::endl;
    exit(1);
  }

  TFile *f = new TFile(inFile);
  f->cd();
  TTree *inObj;
  // Grab the Users TTree
  inObj = (TTree*) f->Get(ObjName);
  inObj->SetEstimate(-1);
  // Grab TTree Values
  Int_t nentries=0;
  if(cut1=="NA")
    nentries = inObj->Draw("Energy:Weight","","goff");
  else
    nentries = inObj->Draw("Energy:Weight",cut1,"goff");

  if(nentries <= 0)
    exit(1);

  if(verbose)
    inObj->Print();

  Double_t *energies = inObj->GetVal(0);
  Double_t *weights = inObj->GetVal(1);
  TH1D *hObj;
  // If User wants fixed bin widths
  if(!VarArray)
  {
    if(verbose)
      std::cout << "User did not select variable bin widths." << std::endl;
    hObj = new TH1D(OutObjName, "Weighted Energy Spectrum", nbins, Emin, Emax);
  }
  // User wants variable bin widths
  else
  {
    if(verbose)
      std::cout << "User selected variable bin widths." <<std::endl;
    // Find total number of bins
    double edge_counter = 0.;
    int nbins1 = Emin/(non_nrf_bin_width);
    int nbins2 = (Emax - Emin)/nrf_bin_width;
    Int_t tbins = nbins1 + nbins2;
    // create edges (dynamically sized array)
    Double_t* edges = new Double_t[tbins+1];
    // fill the edges for the first region
    if(verbose)
      std::cout << "Creating Edges for the first region..." << std::endl;

    for(int i=0;i<nbins1+1;++i)
    {
      edges[i] = edge_counter;
      edge_counter += non_nrf_bin_width;
    }
    // fill the edges for the second region
    edge_counter = edge_counter - non_nrf_bin_width;
    if(verbose)
      std::cout << "Creating Edges for the second region..." << std::endl;

    for(int i=nbins1;i<tbins+2;++i)
    {
      edges[i] = edge_counter;
      edge_counter += nrf_bin_width;
    }

    // edges complete now create new histogram
    hObj = new TH1D(OutObjName,"Weighted Energy Spectrum w/ Variable Bins",tbins,edges);

  }// end !VarArray

  // Prior to filling histogram set structure for storing bin errors
  hObj->Sumw2();

  if(verbose)
    std::cout << "Filling Histogram..." << std::endl;
  // Fill the new Histogram
  for(unsigned int i=0;i<nentries;++i)
  {
    hObj->Fill(energies[i],weights[i]);
  }

  // Write to OutFile
  std::string OutFileName;
  if(!VarArray)
    OutFileName = "rebinned_" + to_string(nbins) + "_" + (std::string)inFile;
  else
    OutFileName = "rebinned_Variable_binWidth_" + to_string(nrf_bin_width) + "_" + (std::string)inFile;

  TFile *fout;
  if(!gSystem->AccessPathName(OutFileName.c_str()))
    fout = new TFile(OutFileName.c_str(),"update");
  else
    fout = new TFile(OutFileName.c_str(),"recreate");

  fout->cd();
  hObj->Write();
  std::cout << "Rebinned Histogram written to: " << OutFileName << std::endl;
  fout->Close();

}// end Rebin.cc

void Rebin(const char* inFile,const char* ObjName,const char* OutObjName)
{
  // Check to make sure file exists
  if(gSystem->AccessPathName(inFile))
  {
    std::cerr << "ERROR Could not find " << inFile << "exiting..." << std::endl;
    exit(1);
  }

  TFile *f = new TFile(inFile);
  f->cd();
  TTree *inObj;
  // Grab the Users TTree
  inObj = (TTree*) f->Get(ObjName);
  inObj->SetEstimate(-1);

  // Write TTree to histogram
  TH1D *hObj = new TH1D(OutObjName, "Weighted Energy Spectrum",100);
  std::string tCommand = "Energy>>" + (std::string)OutObjName
  inObj->Draw(tCommand.c_str(), "Weight","goff");
  hObj->Sumw2();
  hObj->Print();

  TFile *fout;
  std::string OutFileName = "converted_" + (std::string)inFile;

  if(!gSystem->AccessPathName(OutFileName.c_str()))
    fout = new TFile(OutFileName.c_str(),"update");
  else
    fout = new TFile(OutFileName.c_str(),"recreate");

  fout->cd();
  hObj->Write();
  fout->Close();
}
