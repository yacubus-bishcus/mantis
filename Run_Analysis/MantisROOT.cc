//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Author:
// Jacob E Bickus, 2021
// MIT, NSE
// jbickus@mit.edu
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////
bool debug;

class MantisROOT
{
public:
    MantisROOT(bool debug_in=false);
    ~MantisROOT();

  public:
    static MantisROOT *GetInstance();
    static void Destroy();
    void Help();
    void Show(string cmd="All", bool=false);
    TFile* OpenFile(const char*);
    void CombineFiles(std::vector<string>, std::vector<string>, const char*);
    void CopyTrees(const char*, std::vector<string>);
    void Sig2Noise(std::vector<string>, string, bool Weighted=false, bool cut=false,TCut cut1="NA");
    void ZScore(const char*, const char*, std::vector<string>);
    void Integral(TTree*);
    void Integral(std::vector<TTree*>);
    void Integral(std::vector<TTree*>,TCut);
    void PredictThickness(std::vector<string>);
    void PredictThickness(std::vector<string>, double);
    void RebinHisto(vector<string>, vector<string>, vector<string>, int, double, double);
    void RebinHisto(vector<string>, vector<string>, vector<string>, int, double, double, TCut);
    void VarRebin(vector<string>, vector<string>, vector<string>, int, double, double, TCut, double, double);
    void CheckEvents(const char*,bool);
    void Sampling(const char*, string sample_element="U", double deltaE=5.0e-6, bool checkZero=false, double non_nrf_energy_cut=1.5);
    void SimpleSampling(const char*, double deltaE=5.0e-6, double cut_energy=1.5, double weight=10000, bool checkZero=false);
    void CheckIntObj(const char*, const char*, double, bool Weighted=false);
    void CheckAngles(const char*, int estimate=-1);
  public:
    static MantisROOT *instance;

  private:
    void Compute(const char*, time_t, bool);
    void CopyATree(const char*, const char*);
    void CopyATreeNoWeight(const char*, const char*);
    void SNR_IntObj(const char*, bool);
    void SNR_Det(const char*, bool, bool, TCut cut1="NA");
    void hIntegral(TH1*);
    double hIntegral(TH1*, int);
    double hIntegral(TTree*, int, TCut, double);
    double hIntegral(TTree*, int);
    double hIntegralReturn(TTree*, bool, TCut cut1="NA");
    double hIntegralReturnWeightedCounts(TTree*, double cut_energy=5e-6);
    double hIntegralReturnWeightedEnergy(TTree*, double cut_energy=5e-6);
    void hIntegral(TTree*);
    void hIntegral(TTree*, TCut);
    void hIntegral(const char*, const char*);
    void hIntegral(const char*, const char*, TCut);
    void hIntegral(const char*);
    double ZTest(double, double);
    void ZTest(const char*, const char*, const char*);
    void Rebin(bool,const char*,const char*,const char*,int,double Emin=0.0,double Emax=2.1,TCut cut1="NA",bool VarArray=false,double nrf_bin_width=-1.,double non_nrf_bin_width=-1.);
    void Rebin(const char*, const char*, const char*);
    void Rescale(const char*, double);
    void Rescale(const char*);

    double ReturnBremMax(const char*);
    TH1D* BuildBrem(const char*, double, bool);
    TH1D* BuildSimpleSample(const char*, double, double, double);
    void WriteSampling(TH1D*, TH1D*, TGraph*, TGraph*, double);

    string EraseSubStr(string&, const string&);

    void Show_Help();
    void Show_Help_Description();
    void Show_Show();
    void Show_Show_Description();
    void Show_OpenFile();
    void Show_OpenFile_Description();
    void Show_CombineFiles();
    void Show_CombineFiles_Description();
    void Show_CopyTrees();
    void Show_CopyTrees_Description();
    void Show_Sig2Noise();
    void Show_Sig2Noise_Description();
    void Show_ZScore();
    void Show_ZScore_Description();
    void Show_Integral();
    void Show_Integral_Description();
    void Show_PredictThickness();
    void Show_PredictThickness_Description();
    void Show_RebinHisto();
    void Show_RebinHisto_Description();
    void Show_VarRebin();
    void Show_VarRebin_Description();
    void Show_CheckEvents();
    void Show_CheckEvents_Description();
    void Show_Sampling();
    void Show_Sampling_Description();
    void Show_GetInstance();
    void Show_SimpleSampling();
    void Show_SimpleSampling_Description();
    void Show_CheckIntObj();
    void Show_CheckIntObj_Description();
    void Show_CheckAngles();
    void Show_CheckAngles_Description();
};

MantisROOT *MantisROOT::instance = 0;


MantisROOT *MantisROOT::GetInstance()
{
  if(instance == 0)
  {
    instance = new MantisROOT();
  }
  std::cout << "Mantis ROOT Analysis Package for Mantis Output Analysis" << std::endl;
  std::cout <<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << std::endl;
  std::cout << "Author:" <<std::endl;
  std::cout << "Jacob E Bickus, 2021" << std::endl;
  std::cout << "MIT, NSE" << std::endl;
  std::cout << "jbickus@mit.edu" << std::endl;
  std::cout <<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << std::endl;
  return instance;
}

MantisROOT::MantisROOT(bool debug_in=false)
{
  debug = debug_in;
  if(debug)
    std::cout << "Set to Debugging Mode!" << std::endl;
  std::cout << "Run: Help() to see the list of available functions." << std::endl;
}

MantisROOT::~MantisROOT()
{}

void MantisROOT::Destroy()
{
  if(instance != 0)
  {
   delete instance;
   instance = 0;
  }
}

void MantisROOT::Help()
{
  std::cout << "Calls and Descriptions" << std::endl << std::endl;

  Show_Help();
  Show_Help_Description();
  std::cout << std::endl;
  Show_GetInstance();
  std::cout << std::endl << std::endl;
  Show_OpenFile();
  Show_OpenFile_Description();
  std::cout << std::endl;
  Show_CombineFiles();
  Show_CombineFiles_Description();
  std::cout << std::endl;
  Show_CopyTrees();
  Show_CopyTrees_Description();
  std::cout << std::endl;
  Show_Sig2Noise();
  Show_Sig2Noise_Description();
  std::cout << std::endl;
  Show_ZScore();
  Show_ZScore_Description();
  std::cout << std::endl;
  Show_Integral();
  Show_Integral_Description();
  std::cout << std::endl;
  Show_PredictThickness();
  Show_PredictThickness_Description();
  std::cout << std::endl;
  Show_RebinHisto();
  Show_RebinHisto_Description();
  std::cout << std::endl;
  Show_VarRebin();
  Show_VarRebin_Description();
  std::cout << std::endl;
  Show_CheckEvents();
  Show_CheckEvents_Description();
  std::cout << std::endl;
  Show_Sampling();
  Show_Sampling_Description();
  std::cout << std::endl;
  Show_SimpleSampling();
  Show_SimpleSampling_Description();
  std::cout << std::endl;
  Show_CheckIntObj();
  Show_CheckIntObj_Description();
  std::cout << std::endl;
  Show_CheckAngles();
  Show_CheckAngles_Description();
  std::cout << std::endl;
}

void MantisROOT::Rebin(bool verbose, const char* inFile, const char* ObjName, const char* OutObjName,
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

}

// *************************************************************************//
// *************************************************************************//
// *************************************************************************//

void MantisROOT::Rebin(const char* inFile,const char* ObjName,const char* OutObjName)
{
  // Check to make sure file exists
  if(gSystem->AccessPathName(inFile))
  {
    std::cerr << "ERROR Could not find " << inFile << "exiting..." << std::endl;
    exit(1);
  }

  std::string OutFileName = "converted_" + (std::string)inFile;
  if(!gSystem->AccessPathName(OutFileName.c_str()))
  {
    std::cout << "Rebinned File Already exists. Continue to rebin?" << std::endl;
    string response;
    std::cin >> response;

    if(!response.compare(0,1,"N"))
      exit(1);
    else
      std::cout << "Continuing rebinning..." << std::endl;

  }

  TFile *f = new TFile(inFile);
  f->cd();
  TTree *inObj;
  // Grab the Users TTree
  inObj = (TTree*) f->Get(ObjName);
  inObj->SetEstimate(-1);
  double Emax = inObj->GetMaximum("Energy");
  // Write TTree to histogram
  TH1D *hObj = new TH1D(OutObjName, "Weighted Energy Spectrum",100,0.,Emax);
  std::string tCommand = "Energy>>" + (std::string)OutObjName;
  inObj->Draw(tCommand.c_str(), "Weight","goff");

  TFile *fout;

  if(!gSystem->AccessPathName(OutFileName.c_str()))
    fout = new TFile(OutFileName.c_str(),"update");
  else
    fout = new TFile(OutFileName.c_str(),"recreate");

  fout->cd();
  hObj->Write();
  fout->Close();
} // end of Rebin Functions

void MantisROOT::hIntegral(TH1 *h)
{
  int nentries = h->GetEntries();
  TAxis *xaxis = h->GetXaxis();
  double hMean = h->GetMean();
  int nbins = h->GetNbinsX();

  double intSum = 0;
  for(unsigned int i=0;i<nbins;++i)
  {
    double xVal = xaxis->GetBinCenter(i);
    double binVal = h->GetBinContent(i);
    double integralVal = xVal*binVal;
    intSum += integralVal;
  }
  std::cout << h->GetTitle() << " BinCenter Method Sum: " << intSum << std::endl;
  std::cout << h->GetTitle() << " Mean Method Sum: " << nentries*hMean << std::endl;
  std::cout << h->GetTitle() << " Integral Method: " << h->Integral() << std::endl << std::endl;
}
double MantisROOT::hIntegral(TH1* h, int returnValue)
{
  int nentries = h->GetEntries();
  TAxis *xaxis = h->GetXaxis();
  double hMean = h->GetMean();
  int nbins = h->GetNbinsX();

  double intSum = 0;
  for(unsigned int i=0;i<nbins;++i)
  {
    double xVal = xaxis->GetBinCenter(i);
    double binVal = h->GetBinContent(i);
    double integralVal = xVal*binVal;
    intSum += integralVal;
  }

  if(returnValue == 1)
    return intSum;
  else if(returnValue == 2)
    return nentries*hMean;
  else if(returnValue == 3)
    return h->Integral();
  else
    return 0;

}

double MantisROOT::hIntegral(TTree *inObj, int returnValue)
{
  inObj->SetEstimate(-1);
  double Emax = inObj->GetMaximum("Energy");
  TH1D *e1 = new TH1D("e1",inObj->GetName(),100,0.,Emax);

  inObj->Draw("Energy>>e1","","goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }

  double theSum = hIntegral(e1, returnValue);
  delete e1;

  if(returnValue == 0)
    return intSum;
  else
    return theSum;

}
double MantisROOT::hIntegral(TTree *inObj, int returnValue, TCut cut1, double Emax)
{
  //std::cout << "MantisROOT::hIntegral -> Setting TCut to: " << cut1 << std::endl;
  inObj->SetEstimate(-1);
  TH1D *e1 = new TH1D("e1",inObj->GetName(),100,0.,Emax);

  if(cut1 == "NA")
    inObj->Draw("Energy>>e1","","goff");
  else
    inObj->Draw("Energy>>e1",cut1,"goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }

  double theSum = hIntegral(e1, returnValue);
  delete e1;

  if(returnValue == 0)
    return intSum;
  else
    return theSum;

}

void MantisROOT::hIntegral(TTree* inObj)
{
  inObj->SetEstimate(-1);
  double Emax = inObj->GetMaximum("Energy");
  TH1D *e1 = new TH1D("e1",inObj->GetName(),100,0.,Emax);

  inObj->Draw("Energy>>e1","","goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }

  std::cout << inObj->GetName() << " TTree Sum: " << intSum << std::endl;

  hIntegral(e1);
  if(debug)
    std::cout << "MantisROOT::hIntegral -> Deallocating Memory." << std::endl;
  delete e1; // avoids potential memory leak
}

double MantisROOT::hIntegralReturn(TTree* inObj, bool cut, TCut cut1="NA")
{
  inObj->SetEstimate(-1);

  if(cut)
  {
    if(debug)
      std::cout << "Cut " << cut1 << " placed!" << std::endl;
    inObj->Draw("Energy",cut1,"goff");
  }
  else
    inObj->Draw("Energy","","goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }
  return intSum;
}

double MantisROOT::hIntegralReturnWeightedCounts(TTree* inObj, double cut_energy=5e-6)
{
  inObj->SetEstimate(-1);
  TH1D* h = new TH1D("h","Det Integral",100,0.,cut_energy);

  if(debug)
    std::cout << "Cut " << cut_energy << " MeV placed!" << std::endl;
  inObj->Draw("Energy>>h","Weight","goff");
  double intSum = h->Integral();
  if(debug)
    std::cout << "MantisROOT::hIntegralReturnWeightedCounts -> Deallocating Memory..." << std::endl;
  //delete h;
  return intSum;

}

double MantisROOT::hIntegralReturnWeightedEnergy(TTree* inObj, double cut_energy=5e-6)
{
  inObj->SetEstimate(-1);
  if(debug)
    std::cout << "Cut " << cut_energy << " MeV placed!" << std::endl;
  int nentries = inObj->Draw("Energy:Weight","","goff");
  double *Energies = inObj->GetVal(0);
  double *Weights = inObj->GetVal(1);

  double intSum = 0;
  for(int i=0;i<nentries;++i)
  {
    intSum += Energies[i]*Weights[i];
  }
  if(debug)
    std::cout << "MantisROOT::hIntegralReturnWeightedEnergy -> Deallocating Memory..." << std::endl;
  //delete Energies;
  //delete Weights;

  return intSum;
}

void MantisROOT::hIntegral(TTree *inObj,TCut cut1)
{
  inObj->SetEstimate(-1);
  double Emax = inObj->GetMaximum("Energy");
  TH1D *e1 = new TH1D("e1",inObj->GetName(),100,0.,Emax);

  if(cut1 == "NA")
    inObj->Draw("Energy>>e1","","goff");
  else
    inObj->Draw("Energy>>e1",cut1,"goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }

  std::cout << inObj->GetName() << " TTree Sum: " << intSum << std::endl;

  hIntegral(e1);
  if(debug)
    std::cout << "MantisROOT::hIntegral -> Deallocating Memory..." << std::endl;
  delete e1; // avoids potential memory leak
}

void MantisROOT::hIntegral(const char* filename, const char* objName)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cerr << "File not found." << std::endl;
    exit(100);
  }
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  bool confirm = f->cd();
  if(!confirm)
    exit(0);

  TTree *inObj;
  f->GetObject(objName,inObj);
  if(inObj == 0)
    exit(0);

  hIntegral(inObj);
  if(debug)
    std::cout << "MantisROOT::hIntegral -> Deallocating Memory..." << std::endl;
  delete inObj; // avoids potential memory leak
}

void MantisROOT::hIntegral(const char* filename, const char* objName, TCut cut1)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cerr << "File not found." << std::endl;
    exit(100);
  }
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  bool confirm = f->cd();
  if(!confirm)
    exit(0);

  TTree *inObj;
  f->GetObject(objName,inObj);
  if(inObj == 0)
    exit(0);

  hIntegral(inObj, cut1);
  delete inObj; // avoids potential memory leak
}

void MantisROOT::hIntegral(const char* filename)
{
  // Doing the integral for chopOut, IntObjIn and DetInfo
  if(gSystem->AccessPathName(filename))
  {
    std::cerr << "File not found." << std::endl;
    exit(100);
  }
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  TTree *inChopOut, *inIntObjIn, *inDetInfo;
  f->GetObject("ChopOut",inChopOut);
  f->GetObject("IntObjIn",inIntObjIn);
  f->GetObject("DetInfo",inDetInfo);
  hIntegral(inChopOut);
  hIntegral(inIntObjIn);
  inDetInfo->SetEstimate(-1);
  TH1D *e1 = new TH1D("e1","DetInfo Histogram",100,0.,10e-6);
  inDetInfo->Draw("Energy>>e1","Energy<10e-6","goff");
  std::cout << "DetInfo Integral: " << e1->Integral() << std::endl;
  if(debug)
    std::cout << "MantisROOT::hIntegral -> Deallocating Memory..." << std::endl;
  delete e1;
  f->Close();
} // end of hIntegral Functions

void MantisROOT::CombineFiles(std::vector<string> filenames, std::vector<string> objects, const char* outfilename)
{
  for(int i=0;i<objects.size();++i)
  {
    TFile *f = new TFile(outfilename, "update");
    f->cd();
    TChain ch(objects[i].c_str());
    for(int i=0;i<filenames.size();++i)
      ch.Add(filenames[i].c_str());

    ch.Merge(f,1000);

    std::cout << objects[i] << " merged to " << outfilename << std::endl;
  }

}

void MantisROOT::CopyTrees(const char* filename, std::vector<string> noObjv)
{
  std::vector<bool> dataList = {0,0,0,0,0,0,0,0,0,0,0,0};
  std::vector<string> optList = {"Weight","Brem","ChopIn","ChopOut","NRF","AirIn",
                                  "IntObjIn","IntObjOut","Water","Cherenkov",
                                  "DetInfo","IncDetInfo"};
  // check which trees to copy
  for(int i=0;i<noObjv.size();++i)
  {
    for(int j=0;j<optList.size();++j)
    {
      int counter = 0;
      if(!noObjv[i].compare(optList[j]))
      {
        dataList[j] = true;
        continue;
      }
      else
        counter++;

      if(counter == optList.size())
        std::cout << noObjv[i] << " option not found!" << std::endl;

    }
  }

  std::cout << "Copying Tree Status: " << std::endl;
  for(int i=0;i<optList.size();++i)
    std::cout << "\t\t " << optList[i] << " = " << dataList[i] << std::endl;

  for(int i=1;i<optList.size();++i)
  {
    if(dataList[0])
    {
      if(dataList[i])
        CopyATree(filename, optList[i].c_str());
    }
    else
    {
      if(dataList[i])
        CopyATreeNoWeight(filename, optList[i].c_str());
    }
  }

  string outfilename = "converted_" + string(filename);
  std::cout << "All Trees Copied to " << outfilename << std::endl;
}

void MantisROOT::Sig2Noise(std::vector<string> filenames, string object, bool Weighted=false, bool cut=false, TCut cut1="NA")
{
  if(!object.compare("IntObj"))
  {
    for(int i=0;i<filenames.size();++i)
    {
      std::cout << std::endl << "Signal to Noise Calculation for " << filenames[i] << std::endl;
      SNR_IntObj(filenames[i].c_str(), Weighted);
    }
  }
  else if(!object.compare("Det"))
  {
    for(int i=0;i<filenames.size();++i)
    {
      std::cout << std::endl << "Signal to Noise Calculation for " << filenames[i] << std::endl;
      SNR_Det(filenames[i].c_str(), Weighted,cut,cut1);
    }
  }
  else if(!object.compare("Both"))
    for(int i=0;i<filenames.size();++i)
    {
      std::cout << std::endl << "Signal to Noise Calculation for " << filenames[i] << std::endl;
      std::cout << filenames[i] << " Interrogation Object Signal to Noise Calculating..." << std::endl;
      SNR_IntObj(filenames[i].c_str(), Weighted);
      std::cout << filenames[i] << " Detected Signal to Noise Calculating..." << std::endl;
      SNR_Det(filenames[i].c_str(), Weighted,cut,cut1);
    }
  else
  {
    std::cerr << "Error: Object Not Found Input Options: " << std::endl
    << "IntObj" << std::endl << "Det" << std::endl << "Both" << std::endl;
    return;
  }

  std::cout << "Signal to Noise Ratio Analysis Complete." << std::endl;
}

void MantisROOT::ZScore(const char* file1, const char* file2, std::vector<string> objects)
{
  for(int i=0;i<objects.size();++i)
  {
    std::cout << std::endl << objects[i] << " ZScore Results: " << std::endl;
    ZTest(file1, file2, objects[i].c_str());
  }

  std::cout << std::endl << "ZScore Analysis Complete." << std::endl;
}

void MantisROOT::Integral(TTree* tree)
{
  hIntegral(tree);
  std::cout << "Integral Analysis Complete." << std::endl;
}

void MantisROOT::Integral(std::vector<TTree*> trees)
{
  for(int i=0;i<trees.size();++i)
    hIntegral(trees[i]);

  std::cout << "Integral Analysis Complete." << std::endl;
}

void MantisROOT::Integral(std::vector<TTree*> trees, TCut cut1)
{
  for(int i=0;i<trees.size();++i)
    hIntegral(trees[i], cut1);

  std::cout << "Integral Analysis Complete." << std::endl;
}

void MantisROOT::PredictThickness(std::vector<string> obj)
{
  for(int i=0;i<obj.size();++i)
    Rescale(obj[i].c_str());

  std::cout << "Thickness Prediction Analysis Complete." << std::endl;
}

void MantisROOT::PredictThickness(std::vector<string> obj, double Er)
{
  for(int i=0;i<obj.size();++i)
    Rescale(obj[i].c_str(),Er);

  std::cout << "Thickness Prediction Analysis Complete." << std::endl;
}

void MantisROOT::RebinHisto(std::vector<string> inFile, std::vector<string> ObjName,
                 std::vector<string> OutObjName, int nbins, double Emin,
                 double Emax)
{
  if(OutObjName.size() != ObjName.size())
  {
    std::cout << "Error ObjName and OutObjName vector inputs must be same length! Check your inputs!" << std::endl;
    exit(1);
  }
  for(int i=0;i<inFile.size();++i)
  {
    for(int j=0;j<ObjName.size();++j)
    {
      Rebin(false, inFile[i].c_str(), ObjName[j].c_str(), OutObjName[j].c_str(),
            nbins, Emin, Emax);
    }
  }
}

void MantisROOT::RebinHisto(std::vector<string> inFile, std::vector<string> ObjName,
                 std::vector<string> OutObjName, int nbins, double Emin,
                 double Emax, TCut cut1)
{
  if(OutObjName.size() != ObjName.size())
  {
    std::cout << "Error ObjName and OutObjName vector inputs must be same length! Check your inputs!" << std::endl;
    exit(1);
  }
  for(int i=0;i<inFile.size();++i)
  {
    for(int j=0;j<ObjName.size();++j)
    {
      Rebin(false, inFile[i].c_str(), ObjName[j].c_str(), OutObjName[j].c_str(),
            nbins, Emin, Emax, cut1);
    }
  }
}

void MantisROOT::VarRebin(std::vector<string> inFile, std::vector<string> ObjName,
                           std::vector<string> OutObjName, int nbins,
                           double Ecut1, double Ecut2,
                           TCut cut1, double binwidth1, double binwidth2)
{
  if(OutObjName.size() != ObjName.size())
  {
    std::cout << "Error ObjName and OutObjName vector inputs must be same length! Check your inputs!" << std::endl;
    exit(1);
  }
  for(int i=0;i<inFile.size();++i)
  {
    for(int j=0;j<ObjName.size();++j)
    {
      Rebin(false, inFile[i].c_str(), ObjName[j].c_str(), OutObjName[j].c_str(),
            nbins, Ecut1, Ecut2, cut1, true, binwidth1, binwidth2);
    }
  }
}

void MantisROOT::Show(string name="All", bool description=false)
{
  if(!name.compare("All") && description)
  {
    Help();
  }
  else if(!name.compare("All") && !description)
  {
    Show_CheckAngles();
    Show_CheckEvents();
    Show_CheckIntObj();
    Show_CombineFiles();
    Show_CopyTrees();
    Show_Help();
    Show_Integral();
    Show_OpenFile();
    Show_PredictThickness();
    Show_RebinHisto();
    Show_Sampling();
    Show_Show();
    Show_Sig2Noise();
    Show_SimpleSampling();
    Show_VarRebin();
    Show_ZScore();
  }
  else if(!name.compare("Help"))
  {
    Show_Help();
    if(description)
      Show_Help_Description();
  }
  else if(!name.compare("Show"))
  {
    Show_Show();
    if(description)
      Show_Show_Description();
  }
  else if(!name.compare("OpenFile"))
  {
    Show_OpenFile();
    if(description)
      Show_OpenFile_Description();
  }
  else if(!name.compare("CopyTrees"))
  {
    Show_CopyTrees();
    if(description)
      Show_CopyTrees_Description();
  }
  else if(!name.compare("CombineFiles"))
  {
    Show_CombineFiles();
    if(description)
      Show_CombineFiles_Description();
  }
  else if(!name.compare("Sig2Noise"))
  {
    Show_Sig2Noise();
    if(description)
      Show_Sig2Noise_Description();
  }
  else if(!name.compare("ZScore"))
  {
    Show_ZScore();
    if(description)
      Show_ZScore_Description();
  }
  else if(!name.compare("Integral"))
  {
    Show_Integral();
    if(description)
      Show_Integral_Description();
  }
  else if(!name.compare("PredictThickness"))
  {
    Show_PredictThickness();
    if(description)
      Show_PredictThickness_Description();
  }
  else if(!name.compare("RebinHisto"))
  {
    Show_RebinHisto();
    if(description)
      Show_RebinHisto_Description();
  }
  else if(!name.compare("VarRebin"))
  {
    Show_VarRebin();
    if(description)
      Show_VarRebin_Description();
  }
  else if(!name.compare("CheckEvents"))
  {
    Show_CheckEvents();
    if(description)
      Show_CheckEvents_Description();
  }
  else if(!name.compare("Sampling"))
  {
    Show_Sampling();
    if(description)
      Show_Sampling_Description();
  }
  else if(!name.compare("SimpleSampling"))
  {
    Show_SimpleSampling();
    if(description)
      Show_SimpleSampling_Description();
  }
  else if(!name.compare("CheckIntObj"))
  {
    Show_CheckIntObj();
    if(description)
      Show_CheckIntObj_Description();
  }
  else if(!name.compare("CheckAngles"))
  {
    Show_CheckAngles();
    if(description)
      Show_CheckAngles_Description();
  }
  else
    std::cout << "Error Function Not Found." << std::endl;
}

TFile* MantisROOT::OpenFile(const char* filename)
{
  return (new TFile(filename));
} // end of OpenFile function

void MantisROOT::CheckEvents(const char* filename, bool Weighted=false)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cerr << "CheckEvents -> File: " << filename << " does not exist." << std::endl;
    return;
  }
  time_t timer;
  time_t time_start = std::time(&timer);
  Compute(filename,time_start,Weighted);
}

void MantisROOT::Compute(const char* fname, time_t time_start, bool Weighted)
{
  int x =0;
  std::vector<int> nrf_to_cher_to_det_event;
  std::vector<double> nrf_to_cher_to_det_time, nrf_to_cher_to_det_energy, nrf_to_cher_to_det_weight;
  TFile *fMaincompute = new TFile(fname);
  fMaincompute->cd();
  string filename = fMaincompute->GetName();
  TTree *Cherenkov_in, *NRF_in, *DetInfo_in;
  fMaincompute->GetObject("Cherenkov",Cherenkov_in);
  fMaincompute->GetObject("NRF",NRF_in);
  fMaincompute->GetObject("DetInfo",DetInfo_in);
  Cherenkov_in->SetEstimate(-1);
  NRF_in->SetEstimate(-1);
  DetInfo_in->SetEstimate(-1);
  std::cout << "CheckEvents::Compute -> Objects Grabbed!" << std::endl;

  int nrf_entries = NRF_in->Draw("EventID","","goff");

  std::cout << "CheckEvents::Compute -> NRF Entries: " << nrf_entries << std::endl;

  double *nrfEvent = NRF_in->GetVal(0);
  std::vector<int> nrfEventv;
  // Write NRF Events to Vector
  for(int i=0; i<nrf_entries; ++i)
  {
    nrfEventv.push_back((int)nrfEvent[i]);
  }

  // Grab Cherenkov Events
  int cher_entries = Cherenkov_in->Draw("EventID","","goff");

  std::cout << "CheckEvents::Compute -> Cherenkov Entries: " << cher_entries << std::endl;

  double *cherEvent = Cherenkov_in->GetVal(0);

  std::vector<int> cherEventv;
  // Write Cherenkov Events to vector
  for(int i=0; i<cher_entries; ++i)
  {
    cherEventv.push_back((int)cherEvent[i]);
  }

  // Grab DetInfo Events
  int det_entries =0;

  if(Weighted)
    det_entries = DetInfo_in->Draw("EventID:Energy:Time:Weight","CreatorProcess == \"Scintillation\" || CreatorProcess == \"Cerenkov\"","goff");
  else
    det_entries = DetInfo_in->Draw("EventID:Energy:Time","CreatorProcess == \"Scintillation\" || CreatorProcess == \"Cerenkov\"","goff");

  std::cout << "CheckEvents::Compute -> Total Number of Detected Optical Photon entries: "
            << det_entries << std::endl << std::endl;

  double *detEvent = DetInfo_in->GetVal(0);
  double *detEnergy = DetInfo_in->GetVal(1);
  double *detTime = DetInfo_in->GetVal(2);

  double *detWeight = new double[(int)det_entries];

  if(Weighted)
    detWeight = DetInfo_in->GetVal(3);

  std::vector<int> detEventv;
  for(int i=0; i<det_entries; ++i)
  {
    detEventv.push_back((int)detEvent[i]);
  }

  std::vector<int> nrf_to_cherEvents;

  if(nrf_entries != 0 && cher_entries != 0 && det_entries != 0)
  {
    std::cout << "CheckEvents::Compute -> Finding NRF Events that Caused Optical Photons..." << std::endl;

    for(int i=0; i<nrf_entries; ++i)
    {
      // Grab nrf Event ID
      x = nrfEventv[i];
      // Check if NRF EventID is found in Cherenkov Vector
      auto exists = std::find(cherEventv.begin(),cherEventv.end(), x);

      if(exists != cherEventv.end())
        // if the eventID is found in cherenkov write to new vector
        nrf_to_cherEvents.push_back(x);
    }
    std::cout << "CheckEvents::Compute -> NRF to Optical Photon Number of Events Found: "
            << nrf_to_cherEvents.size() << std::endl;

    for(unsigned int i=0; i<nrf_to_cherEvents.size(); ++i)
    {
      // Check if nrf->cherenkov event ID is in detected
      x = nrf_to_cherEvents[i];
      auto exists = std::find(detEventv.begin(), detEventv.end(), x);
      if(exists != detEventv.end())
      {
        // if the eventID is found write to new vector
        nrf_to_cher_to_det_event.push_back(x);
        // find the index of the detInfo array
        int index = exists - detEventv.begin();
        // write the rest of the detected info to new vectors
        nrf_to_cher_to_det_energy.push_back(detEnergy[index]);

        if(Weighted)
          nrf_to_cher_to_det_weight.push_back(detWeight[index]);

        nrf_to_cher_to_det_time.push_back(detTime[index]);
      }
    }
    std::cout << "CheckEvents::Compute -> NRF to Optical Photons Detected Number of Events: "
            << nrf_to_cher_to_det_event.size() << std::endl;
  } // end if nrf_entries != 0 && cher_entries != 0

  if(debug)
    std::cout << "CheckEvents::Compute -> Deallocating memory..." << std::endl;
  // deallocate memory
  //delete []detWeight;
  if(debug)
    std::cout << "CheckEvents::Compute -> Closing File... " << &fMaincompute << std::endl;
  //f->cd();
  fMaincompute->Close();
  if(debug)
    std::cout << "CheckEvents::Compute -> File Closed." << std::endl;

  // Open New TFile to write to
  string event_output_name = "w_events_" + filename;
  TFile *fin = new TFile(event_output_name.c_str(),"recreate");
  fin->cd();

  // Set up NRF to Cher to Det Tree
  int event;
  double energy, weight, thetimes;

  TTree *nrf_to_cher_to_det_tree = new TTree("event_tree","NRF Events that Lead to Cherenkov that were Detected");
  nrf_to_cher_to_det_tree->Branch("EventID",&event);
  nrf_to_cher_to_det_tree->Branch("Energy",&energy);

  if(Weighted)
    nrf_to_cher_to_det_tree->Branch("Weight",&weight);

  nrf_to_cher_to_det_tree->Branch("Time",&thetimes);

  // Fill nrf_to_cher_to_det Tree
  if(nrf_to_cher_to_det_event.size() > 0)
  {
    // Fill tree
    for(unsigned int i=0; i<nrf_to_cher_to_det_event.size(); ++i)
    {
      event = nrf_to_cher_to_det_event[i];
      energy = nrf_to_cher_to_det_energy[i];

      if(Weighted)
        weight = nrf_to_cher_to_det_weight[i];

      thetimes = nrf_to_cher_to_det_time[i];
      nrf_to_cher_to_det_tree->Fill();
    }
  }

  nrf_to_cher_to_det_tree->Write();


  std::cout << "CheckEvents::Compute -> TTrees Written to File: "
              << event_output_name << std::endl;

  if(debug)
    std::cout << "CheckEvents::Compute -> Closing Event File..." << std::endl;
  fin->Close();
  if(debug)
    std::cout << "CheckEvents::Compute -> Event File Closed." << std::endl;

  time_t timer2;
  time_t time_end = std::time(&timer2);
  std::cout << "Event Check took: " << std::difftime(time_end, time_start)
          << " seconds!" << std::endl << std::endl;
}

void MantisROOT::CopyATree(const char* filename, const char* tObj)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cerr << "File not found." << std::endl;
    exit(100);
  }
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  TTree *oldTree;
  f->GetObject(tObj, oldTree);
  oldTree->SetBranchStatus("*",1);
  string outfilename = "converted_" + string(filename);

  TFile *newfile;
  if(gSystem->AccessPathName(outfilename.c_str()))
    newfile = new TFile(outfilename.c_str(), "recreate");
  else
    newfile = new TFile(outfilename.c_str(),"update");

  auto newtree = oldTree->CloneTree();

  newtree->Print();
  // write to new file
  newfile->Write();
  newfile->Close();
}// end of function CopyATree

void MantisROOT::CopyATreeNoWeight(const char* filename, const char* tObj)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cerr << "File not found." << std::endl;
    exit(100);
  }
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  TTree *oldTree;
  f->GetObject(tObj, oldTree);

  oldTree->SetBranchStatus("*",0);

  if(!string(tObj).compare("ChopIn"))
  {
    for(auto activeBranchName : {"Energy","EventID"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("ChopOut"))
  {
    for(auto activeBranchName : {"Energy","EventID","isNRF","Angle"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("NRF"))
  {
    for(auto activeBranchName : {"EventID","Energy", "Material", "ZPos"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("AirIn") || !string(tObj).compare(0,6,"IntObj"))
  {
    for(auto activeBranchName : {"Energy", "CreatorProcess", "Angle", "Time", "EventID"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("Water"))
  {
    for(auto activeBranchName : {"Energy","CreatorProcess","EventID", "TrackID"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("Cherenkov"))
  {
    for(auto activeBranchName : {"Energy","EventID", "NumSecondaries", "Time"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("DetInfo"))
  {
    for(auto activeBranchName : {"EventID", "Energy", "CreatorProcess", "Time"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("IncDetInfo"))
  {
    for(auto activeBranchName : {"EventID", "Energy", "DetProcess"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else
  {
    std::cerr << "ERROR: Object Name not found." << std::endl;
    exit(1);
  }

  string outfilename = "converted_" + string(filename);

  TFile *newfile;
  if(gSystem->AccessPathName(outfilename.c_str()))
    newfile = new TFile(outfilename.c_str(), "recreate");
  else
    newfile = new TFile(outfilename.c_str(),"update");

  auto newtree = oldTree->CloneTree();

  newtree->Print();
  // write to new file
  newfile->Write();
  newfile->Close();
} // end of CopyTreeNoWeight

void MantisROOT::SNR_IntObj(const char* inFile, bool Weighted)
{
  // Check to make sure file exists
  if(gSystem->AccessPathName(inFile))
  {
    std::cerr << "ERROR Could not find " << inFile << "exiting..." << std::endl;
    exit(1);
  }

  TFile *f = new TFile(inFile);
  f->cd();
  TTree *aIntObjIn;
  f->GetObject("IntObjIn", aIntObjIn);

  if(aIntObjIn !=0)
  {
    aIntObjIn->Print();
    double inMax = aIntObjIn->GetMaximum("Energy");
    aIntObjIn->SetEstimate(-1);

    TH1D* e11 = new TH1D("e11","IntObjIn NRF Histogram",100,1.73350, 1.73360);
    TH1D* e10 = new TH1D("e10","IntObjIn NRF Histogram",100,1.65620,1.65630);
    TH1D* e12 = new TH1D("e12","IntObjIn NRF Histogram",100,1.81520,1.81530);
    TH1D* e13 = new TH1D("e13","IntObjIn NRF Histogram",100,1.86230,1.86240);
    TH1D* e14 = new TH1D("e14","IntObjIn NRF Histogram",100,2.00615,2.00625);

    TH1D* eT = new TH1D("eT","IntObjIn Histogram",100,0.0,inMax);

    std::cout << "Drawing IntObjIn NRF Histograms..." << std::endl;
    if(Weighted)
    {
      aIntObjIn->Draw("Energy>>e10","Weight","goff");
      aIntObjIn->Draw("Energy>>e11","Weight","goff");
      aIntObjIn->Draw("Energy>>e12","Weight","goff");
      aIntObjIn->Draw("Energy>>e13","Weight","goff");
      aIntObjIn->Draw("Energy>>e14","Weight","goff");
    }
    else
    {
      aIntObjIn->Draw("Energy>>e10","","goff");
      aIntObjIn->Draw("Energy>>e11","","goff");
      aIntObjIn->Draw("Energy>>e12","","goff");
      aIntObjIn->Draw("Energy>>e13","","goff");
      aIntObjIn->Draw("Energy>>e14","","goff");
    }

    std::cout << "Drawing IntObjIn and IntObjOut Histograms..." << std::endl;

    if(Weighted)
      aIntObjIn->Draw("Energy>>eT","Weight","goff");
    else
      aIntObjIn->Draw("Energy>>eT","","goff");

    // start integral 5eV prior to energy level peak
    double eStart[] = {1.6562312,1.7335419,1.8152525,1.8623129,2.0061941};
    double eEnd[5];

    for(int i=0;i<5;++i)
      eEnd[i] = eStart[i] + 0.00001; // add 10 eV or 5eV above energy level peak

    int binStart = e10->GetXaxis()->FindBin(eStart[0]);
    int binEnd = e10->GetXaxis()->FindBin(eEnd[0]);
    double inSignal[5];
    inSignal[0] = e10->Integral(binStart, binEnd);
    binStart = e11->GetXaxis()->FindBin(eStart[1]);
    binEnd = e11->GetXaxis()->FindBin(eEnd[1]);
    inSignal[1] = e11->Integral(binStart,binEnd);
    binStart = e12->GetXaxis()->FindBin(eStart[2]);
    binEnd = e12->GetXaxis()->FindBin(eEnd[2]);
    inSignal[2] = e12->Integral(binStart,binEnd);
    binStart = e13->GetXaxis()->FindBin(eStart[3]);
    binEnd = e13->GetXaxis()->FindBin(eEnd[3]);
    inSignal[3] = e13->Integral(binStart,binEnd);
    binStart = e14->GetXaxis()->FindBin(eStart[4]);
    binEnd = e14->GetXaxis()->FindBin(eEnd[4]);
    inSignal[4] = e14->Integral(binStart,binEnd);

    double tSignalin = 0;

    for(int i=0;i<5;++i)
    {
      std::cout << "IntObjIn " << eStart[i] << " MeV Signals: \t" << inSignal[i] << std::endl;
      tSignalin += inSignal[i];
    }

    std::cout << "Total IntObjIn Signal: \t" << tSignalin << std::endl;

    double inNoise = eT->Integral();
    std::cout << "IntObjIn Noise: \t" << inNoise << std::endl;

    std::cout << "The NRF photons are removed from the beam in the chopper wheel stage."
              << std::endl
              << "A lower SNR is better because you want the signal from NRF to be minimized."
              << std::endl;
    std::cout << "IntObjIn SNR: \t" << tSignalin/sqrt(inNoise) << std::endl;
  }
  else
    std::cerr << "ERROR IntObjIn Not Found in " << inFile << std::endl;
}// end of SNR function

void MantisROOT::SNR_Det(const char* inFile, bool Weighted, bool cut, TCut cut1="NA")
{
  // Check to make sure file exists
  if(gSystem->AccessPathName(inFile))
  {
    std::cerr << "ERROR Could not find " << inFile << "exiting..." << std::endl;
    exit(1);
  }
  string event_output_name = "w_events_" + string(inFile);
  // check if event file already exists if not make one
  if(gSystem->AccessPathName(event_output_name.c_str()))
  {
    CheckEvents(inFile,Weighted);
  }

  // open events file
  TFile *eventf = new TFile(event_output_name.c_str());
  eventf->cd();
  TTree *eventT;
  eventf->GetObject("event_tree",eventT);
  double eventCounts, eventEnergy;
  if(!Weighted)
  {
    if(debug)
      std::cout << "MantisROOT::SNR_Det -> !Weighted hIntegralReturn" << std::endl;
    eventCounts = eventT->GetEntries();
    eventEnergy = hIntegralReturn(eventT,false);
  }
  else
  {
    if(debug)
      std::cout << "MantisROOT::SNR_Det-> Weighted hIntegralReturnWeighted" << std::endl;
    eventCounts = hIntegralReturnWeightedCounts(eventT);
    eventEnergy = hIntegralReturnWeightedEnergy(eventT);
  }

  std::cout << "Detected Counts from NRF: " << eventCounts << std::endl;
  std::cout << "Detected Energy from NRF: " << eventEnergy << " MeV" << std::endl;
  if(debug)
    std::cout << "MantisROOT::SNR_Det -> Closing Event File..." << std::endl;
  eventf->Close();
  if(debug)
    std::cout << "MantisROOT::SNR_Det -> Event File Closed." << std::endl;
  // Open the file
  TFile *mainf = new TFile(inFile);
  mainf->cd();
  TTree* detT;
  mainf->GetObject("DetInfo",detT);
  double counts, energy;
  if(!Weighted)
  {
    counts = detT->GetEntries();
    energy = hIntegralReturn(detT,cut,cut1);
  }
  else
  {
    counts = hIntegralReturnWeightedCounts(detT);
    energy = hIntegralReturnWeightedEnergy(detT);
  }

  std::cout << "Total Detected Counts: " << counts << std::endl;
  std::cout << "Total Detected Energy: " << energy << " MeV" << std::endl;

  std::cout << "Counts SNR: " << eventCounts/sqrt(counts) << std::endl;
  //std::cout << "Energy SNR: " << eventEnergy/sqrt(energy) << std::endl;
  if(debug)
    std::cout << "MantisROOT::SNR_Det -> Closing Main File..." << std::endl;
  mainf->Close();
  if(debug)
    std::cout << "MantisROOT::SNR_Det -> Main File Closed." << std::endl;

}

double MantisROOT::ZTest(double c1, double c2)
{
  double zscore = abs(c1 - c2)/(sqrt(pow(sqrt(c1),2) + pow(sqrt(c2),2)));
  //std::cout << "The Z-Score: " << zscore << std::endl;
  return zscore;
}

void MantisROOT::ZTest(const char* file1, const char* file2, const char* inObj)
{
  if(gSystem->AccessPathName(file1))
  {
    std::cerr << "ERROR Could not find " << file1 << "exiting..." << std::endl;
    exit(1);
  }
  if(gSystem->AccessPathName(file2))
  {
    std::cerr << "ERROR Could not find " << file2 << "exiting..." << std::endl;
    exit(1);
  }

  TFile *f = new TFile(file1);
  if(f != 0)
    f->cd();
  else
    return;

  TTree *inTree;
  f->GetObject(inObj, inTree);
  double c1,c11,c12,c13;
  if(!string(inObj).compare("DetInfo"))
  {
    TCut detCut = "Energy<5e-6";
    std::cout << "MantisROOT::ZTest -> Setting TCut for DetInfo: " << detCut << std::endl;
    c1  = hIntegral(inTree,0,detCut,5e-6);
    c11 = hIntegral(inTree,1,detCut,5e-6);
    c12 = hIntegral(inTree,2,detCut,5e-6);
    c13 = hIntegral(inTree,3,detCut,5e-6);
  }
  else if(!string(inObj).compare("IntObjIn"))
  {
    TCut intCut = "cos(Angle)>.99";
    std::cout << "MantisROOT::ZTest -> Setting TCut for IntObjIn: " << intCut << std::endl;
    c1  = hIntegral(inTree,0,intCut,2.1);
    c11 = hIntegral(inTree,1,intCut,2.1);
    c12 = hIntegral(inTree,2,intCut,2.1);
    c13 = hIntegral(inTree,3,intCut,2.1);
  }
  else
  {
    c1  = hIntegral(inTree,0);
    c11 = hIntegral(inTree,1);
    c12 = hIntegral(inTree,2);
    c13 = hIntegral(inTree,3);
  }

  delete inTree;
  f->Close();

  TFile *f1 = new TFile(file2);
  if(f1 != 0)
    f1->cd();
  else
    return;

  TTree *inTree2;
  f1->GetObject(inObj,inTree2);
  double c2,c21,c22,c23;
  if(!string(inObj).compare("DetInfo"))
  {
    TCut detCut = "Energy<5e-6";
    std::cout << "MantisROOT::ZTest -> Setting TCut for DetInfo: " << detCut << std::endl;
    c2  = hIntegral(inTree2,0,detCut,5e-6);
    c21 = hIntegral(inTree2,1,detCut,5e-6);
    c22 = hIntegral(inTree2,2,detCut,5e-6);
    c23 = hIntegral(inTree2,3,detCut,5e-6);
  }
  else if(!string(inObj).compare("IntObjIn"))
  {
    TCut intCut = "cos(Angle)>.99";
    std::cout << "MantisROOT::ZTest -> Setting TCut for IntObjIn: " << intCut << std::endl;
    c2  = hIntegral(inTree2,0,intCut,2.1);
    c21 = hIntegral(inTree2,1,intCut,2.1);
    c22 = hIntegral(inTree2,2,intCut,2.1);
    c23 = hIntegral(inTree2,3,intCut,2.1);
  }
  else
  {
    c2  = hIntegral(inTree2,0);
    c21 = hIntegral(inTree2,1);
    c22 = hIntegral(inTree2,2);
    c23 = hIntegral(inTree2,3);
  }


  std::cout << "TTree Method Z-Score: " << ZTest(c1,c2) << std::endl;
  std::cout << "Bin Center Method Z-Score: " << ZTest(c11,c21) << std::endl;
  std::cout << "Mean Method Z-Score: " << ZTest(c12,c22) << std::endl;
  std::cout << "Integral Method Z-Score: " << ZTest(c13,c23) << std::endl;

  delete inTree2;
  f1->Close();


} // end of ZTest functions

void MantisROOT::Rescale(const char* inObj, double Er)
{
  std::cout << "Input the Scale Base Filename:" << std::endl;
  string filename;
  std::cin >> filename;
  std::vector<std::string> user_files;
  std::vector<float> user_thick;
  std::vector<TFile*> tfilesv;

  // check to make sure file exists
  if(gSystem->AccessPathName(filename.c_str()))
  {
    std::cout << "ERROR Input Filename not found." << std::endl;
    exit(1);
  }

  // Handle the Base File
  user_files.push_back(filename);
  TFile *base = new TFile(filename.c_str());
  tfilesv.push_back(base);

  std::cout << "Input the Scale Base Thickness:" << std::endl;
  float b_thick;
  std::cin >> b_thick;
  user_thick.push_back(b_thick);

  string response = "Y";
  float thick;

  // Handle Files to be scaled
  while(!strcmp(response.c_str(),"Y"))
  {
    std::cout << "Input Filename to Scale: " << std::endl;
    std::cin >> filename;
    user_files.push_back(filename);

    if(gSystem->AccessPathName(filename.c_str()))
    {
      std::cerr << "ERROR File not found." << std::endl;
      exit(1);
    }

    TFile *data_file = TFile::Open(filename.c_str());
    tfilesv.push_back(data_file);
    std::cout << "Input Thickness: " << std::endl;
    std::cin >> thick;
    user_thick.push_back(thick);
    std::cout << "Would you like to load another file?" << std::endl;
    std::cin >> response;
  }

  base->cd();
  double lowE = Er - 0.0001;
  double highE = Er + 0.0001;

  TH1D *hBase = new TH1D("hBase","Scale Base Histogram",60,lowE,highE);

  TTree *base_tInObj;
  base->GetObject(inObj,base_tInObj);
  base_tInObj->SetEstimate(-1);
  double Emax = base_tInObj->GetMaximum("Energy");
  TH1D *oghBase = new TH1D("theBase","Full Base Histogram",100,0.,Emax);
  base_tInObj->Draw("Energy>>theBase","Weight","goff");
  std::vector<TH1D*> ogOut;
  ogOut.push_back(oghBase);
  base_tInObj->Draw("Energy>>hBase","Weight","goff");
  std::vector<TH1D*> hOut;
  hOut.push_back(hBase);

  // based on 60 bins in histograms normalize to background
  std::vector<double> binValues;
  for(int i=0;i<60;++i)
  {
    binValues.push_back(hBase->GetBinContent(i));
  }

  std::cout << "Base set." << std::endl;

  std::cout << "Creating Histograms..." << std::endl;
  TTree *scale_tInObj;

  TH1D *hIn = new TH1D("hIn","Scaled Histogram",60,lowE,highE);
  for(int i=1;i<tfilesv.size();++i)
  {
    TFile *scaleFile = tfilesv[i];
    scaleFile->cd();
    //scaleFile->ls();
    scaleFile->GetObject(inObj,scale_tInObj);
    if(scale_tInObj !=0)
      std::cout << scaleFile->GetName() << " " << inObj << " grabbed."
                  << std::endl;
    else
    {
      std::cerr << "Error Object " << inObj << " not found in file: "
                  << scaleFile->GetName() << std::endl;
      exit(1);
    }

    TH1D *hIn = new TH1D("hIn","Scaled Histogram",60,lowE,highE);
    TH1D *oghIn = new TH1D("theH","Full Histogram to be Scaled",100,0.,Emax);
    scale_tInObj->SetEstimate(-1);
    scale_tInObj->Draw("Energy>>theH","Weight","goff");
    string fullhname = "full_" + std::to_string(i);
    ogOut.push_back(oghIn);
    ogOut[i]->SetName(fullhname.c_str());

    int nentries = scale_tInObj->Draw("Energy:Weight","","goff");
    Double_t* energies = scale_tInObj->GetVal(0);
    Double_t* weights = scale_tInObj->GetVal(1);

    for(unsigned int i=0;i<nentries;++i)
    {
      hIn->Fill(energies[i],weights[i]);
    }

    hOut.push_back(hIn);
    string hname = "Scaled_" + std::to_string(i);
    string htitle = "Scaled_" + user_files[i];
    hOut[i]->SetName(hname.c_str());
    hOut[i]->SetTitle(htitle.c_str());
    //scaleFile->Close();
  }

  std::cout << "Histograms created." << std::endl;
  for(int i=0;i<hOut.size();++i)
    hOut[i]->Print();

  std::cout << "Scaling Histograms." << std::endl;
  double newbinValue = 0;
  std::vector<TH1D*> outputHisto;
  for(int j=1;j<hOut.size();++j)
  {
    for(int i=0;i<60;++i)
    {
      if(binValues[i] == 0)
        newbinValue = 0;
      else
        newbinValue = binValues[i]*pow(hOut[j]->GetBinContent(i)/binValues[i],1./user_thick[j]);

      hOut[j]->SetBinContent(i, newbinValue);

    }
    std::cout << "Histogram Scaled: " <<std::endl;
    hOut[j]->Print();
  }

  // Draw Histograms to canvas
  TCanvas *c0 = new TCanvas();
  c0->cd();
  gPad->SetTicks(1,1);
  hOut[0]->SetLineColor(kBlack);
  std::cout << "Histogram from " << user_files[0]
              << " line color set to: Black" << std::endl;
  hOut[0]->Draw("h");

  std::vector<double> last_colors;
  last_colors.push_back(0);
  int histo_color = 0;
  string histo_color_label;

  for(int i=1;i<hOut.size();++i)
  {
    bool check = false;
    while(!check)
    {
        histo_color = rand() % 9 + 2;
        if(count(last_colors.begin(),last_colors.end(),histo_color) !=0)
        {
            check = false;
        }
        else
        {
            last_colors.push_back(histo_color);
            check = true;
        }
    }
    hOut[i]->SetLineColor(histo_color);
    switch(histo_color)
    {
        case 2:
            histo_color_label = "RED";
            break;
        case 3:
            histo_color_label = "LIGHT GREEN";
            break;
        case 4:
            histo_color_label = "DARK BLUE";
            break;
        case 5:
            histo_color_label = "YELLOW";
            break;
        case 6:
            histo_color_label = "PINK";
            break;
        case 7:
            histo_color_label = "LIGHT BLUE/CYAN";
            break;
        case 8:
            histo_color_label = "MEDIUM GREEN";
            break;
        case 9:
            histo_color_label = "PURPLE";
            break;
        default:
            histo_color_label = "BLACK";
    }
    cout << "Histogram from " << user_files[i] << " line color set to: " << histo_color_label << endl;

    hOut[i]->Draw("h,SAME");
  }

  std::cout << "All Histograms drawn." << std::endl;
  c0->SaveAs("Rescaled_Output.png");
  // Save to file
  TFile *fout = new TFile("thick_rescale.root","recreate");
  fout->cd();

  for(int i=0;i<hOut.size();++i)
  {
    ogOut[i]->Write();
    hOut[i]->Write();
  }

  std::cout << "Histograms saved." << std::endl;

} // end of function


// *************************************************************************** //
// *************************************************************************** //
// *************************************************************************** //

void MantisROOT::Rescale(const char* inObj)
{
  std::cout << "Input the Scale Base Filename:" << std::endl;
  string filename;
  std::cin >> filename;
  std::vector<std::string> user_files;
  std::vector<float> user_thick;
  std::vector<TFile*> tfilesv;

  // check to make sure file exists
  if(gSystem->AccessPathName(filename.c_str()))
  {
    std::cout << "ERROR Input Filename not found." << std::endl;
    exit(1);
  }

  // Handle the Base File
  user_files.push_back(filename);
  TFile *base = new TFile(filename.c_str());
  tfilesv.push_back(base);

  std::cout << "Input the Scale Base Thickness:" << std::endl;
  float b_thick;
  std::cin >> b_thick;
  user_thick.push_back(b_thick);

  string response = "Y";
  float thick;

  // Handle Files to be scaled
  while(!strcmp(response.c_str(),"Y"))
  {
    std::cout << "Input Filename to Scale: " << std::endl;
    std::cin >> filename;
    user_files.push_back(filename);

    if(gSystem->AccessPathName(filename.c_str()))
    {
      std::cerr << "ERROR File not found." << std::endl;
      exit(1);
    }

    TFile *data_file = TFile::Open(filename.c_str());
    tfilesv.push_back(data_file);
    std::cout << "Input Thickness: " << std::endl;
    std::cin >> thick;
    user_thick.push_back(thick);
    std::cout << "Would you like to load another file?" << std::endl;
    std::cin >> response;
  }

  bool same_rescale = false;

  if(!user_files[0].compare(user_files[1]))
    same_rescale = true;

  if(same_rescale)
    std::cout << "Rescaling the same histogram." << std::endl;

  base->cd();

  // Create the base histogram to scale everything else to
  TTree *base_tInObj;
  base->GetObject(inObj,base_tInObj);
  base_tInObj->SetEstimate(-1);
  double Emax = base_tInObj->GetMaximum("Energy");
  TH1D *hBase = new TH1D("hBase","Scale Base Histogram",100,0.,Emax);
  base_tInObj->Draw("Energy>>hBase","Weight","goff");
  std::vector<TH1D*> hOut;
  hOut.push_back(hBase);

  // based on 100 bins in histograms normalize to background
  std::vector<double> binValues;

  for(int i=0;i<100;++i)
  {
    binValues.push_back(hBase->GetBinContent(i));
  }

  std::cout << "Base set." << std::endl;

  std::cout << "Creating Histograms..." << std::endl;
  TTree *scale_tInObj;

  TH1D *hIn = new TH1D("hIn","Scaled Histogram",100,0.,Emax);
  for(int i=1;i<tfilesv.size();++i)
  {
    TFile *scaleFile = tfilesv[i];
    scaleFile->cd();
    //scaleFile->ls();
    scaleFile->GetObject(inObj,scale_tInObj);
    if(scale_tInObj !=0)
      std::cout << scaleFile->GetName() << " " << inObj << " grabbed."
                  << std::endl;
    else
    {
      std::cerr << "Error Object " << inObj << " not found in file: "
                  << scaleFile->GetName() << std::endl;
      exit(1);
    }

    TH1D *hIn = new TH1D("theH","Full Histogram to be Scaled",100,0.,Emax);
    scale_tInObj->SetEstimate(-1);

    int nentries = scale_tInObj->Draw("Energy:Weight","","goff");
    Double_t* energies = scale_tInObj->GetVal(0);
    Double_t* weights = scale_tInObj->GetVal(1);

    for(unsigned int i=0;i<nentries;++i)
    {
      hIn->Fill(energies[i],weights[i]);
    }

    hOut.push_back(hIn);
    string hname = "Scaled_" + std::to_string(i);
    string htitle = "Scaled_" + user_files[i];
    hOut[i]->SetName(hname.c_str());
    hOut[i]->SetTitle(htitle.c_str());
  }

  std::cout << "Histograms created." << std::endl;
  for(int i=0;i<hOut.size();++i)
    hOut[i]->Print();

  std::cout << "Scaling Histograms." << std::endl;
  double newbinValue = 0;
  std::vector<TH1D*> outputHisto;
  for(int j=1;j<hOut.size();++j)
  {
    for(int i=0;i<100;++i)
    {
      if(binValues[i] == 0)
        newbinValue = 0;
      else
      {
        if(same_rescale)
          newbinValue = binValues[i]*user_thick[0]/user_thick[j];
        else
          newbinValue = binValues[i]*pow(hOut[j]->GetBinContent(i)/binValues[i],1./user_thick[j]);
      }
      hOut[j]->SetBinContent(i, newbinValue);

    }
    std::cout << "Histogram Scaled: " <<std::endl;
    hOut[j]->Print();
  }

  // Draw Histograms to canvas
  TCanvas *c0 = new TCanvas();
  c0->cd();
  gPad->SetTicks(1,1);
  hOut[0]->SetLineColor(kBlack);
  std::cout << "Histogram from " << user_files[0]
              << " line color set to: Black" << std::endl;
  hOut[0]->Draw("h");

  std::vector<double> last_colors;
  last_colors.push_back(0);
  int histo_color = 0;
  string histo_color_label;

  for(int i=1;i<hOut.size();++i)
  {
    bool check = false;
    while(!check)
    {
        histo_color = rand() % 9 + 2;
        if(count(last_colors.begin(),last_colors.end(),histo_color) !=0)
        {
            check = false;
        }
        else
        {
            last_colors.push_back(histo_color);
            check = true;
        }
    }
    hOut[i]->SetLineColor(histo_color);
    switch(histo_color)
    {
        case 2:
            histo_color_label = "RED";
            break;
        case 3:
            histo_color_label = "LIGHT GREEN";
            break;
        case 4:
            histo_color_label = "DARK BLUE";
            break;
        case 5:
            histo_color_label = "YELLOW";
            break;
        case 6:
            histo_color_label = "PINK";
            break;
        case 7:
            histo_color_label = "LIGHT BLUE/CYAN";
            break;
        case 8:
            histo_color_label = "MEDIUM GREEN";
            break;
        case 9:
            histo_color_label = "PURPLE";
            break;
        default:
            histo_color_label = "BLACK";
    }
    cout << "Histogram from " << user_files[i] << " line color set to: " << histo_color_label << endl;

    hOut[i]->Draw("h,SAME");
  }

  std::cout << "All Histograms drawn." << std::endl;
  c0->SaveAs("Rescaled_Output.png");
  // Save to file
  TFile *fout = new TFile("thick_rescale.root","recreate");
  fout->cd();

  for(int i=0;i<hOut.size();++i)
    hOut[i]->Write();

  std::cout << "Histograms saved." << std::endl;

}// end of function
TH1D* MantisROOT::BuildBrem(const char* bremInputFilename, double deltaE, bool checkZero)
{
  std::cout << "MantisROOT::BuildBrem -> Building Brem Distribution..." << std::endl;
  TFile *f = TFile::Open(bremInputFilename);
  bool confirm = f->cd();
  if(!confirm)
    exit(10);
  TTree *ChopperData;
  f->GetObject("Brem", ChopperData);
  //ChopperData->Print();

  double Emax = ChopperData->GetMaximum("Energy");

  Int_t nbins = Emax/deltaE;

  TH1D *tmp_hBrems = new TH1D("tmp_hBrems","Temporary Bremsstrahlung Data",250, 0.,Emax);
  std::cout << "MantisROOT::BuildBrem -> Grabbing Brem Data..." << std::endl;
  ChopperData->Draw("Energy>>tmp_hBrems","","goff");
  //tmp_Brems->Smooth(4);
  std::cout << "MantisROOT::BuildBrem -> Data Grabbed." << std::endl;
  //std::cout << "MantisROOT::BuildBrem -> Rebinning Brem Data..." << std::endl;
  TH1D* hBrems = new TH1D("hBrems","Final Bremsstrahlung Data",nbins, 0.,Emax);

  for(int i=1;i<=nbins;++i)
  {
    hBrems->SetBinContent(i,tmp_hBrems->GetBinContent(tmp_hBrems->GetNbinsX()*(i-1)/nbins+1));
  }

  hBrems->Scale(1.0/hBrems->Integral());
  //std::cout << "MantisROOT::BuildBrem -> Rebin Complete." << std::endl;
  if(checkZero)
  {
    std::cout << "MantisROOT::BuildBrem -> Checking for zeros..." << std::endl;
    bool zero_present = false;
    for(unsigned int i=0;i<nbins;++i)
    {
      int count = 0;
      if(hBrems->GetBinContent(i) == 0)
      {
        zero_present = true;
        while(hBrems->GetBinContent(i-1-count) == 0)
          count++;
        hBrems->SetBinContent(i,hBrems->GetBinContent(i-1-count));
      }
    }
    if(zero_present)
      std::cout << "MantisROOT::BuildBrem -> Zeros Found and Corrected." << std::endl;
    else
      std::cout << "MantisROOT::BuildBrem -> No Zeros Found Correction not needed." << std::endl;
  }
  std::cout << "MantisROOT::BuildBrem -> Brem Distribution Created." << std::endl;
  return hBrems;
}

double MantisROOT::ReturnBremMax(const char* bremInputFilename)
{
  std::cout << "MantisROOT::ReturnBremMax -> Searching Max Energy..." << std::endl;
  TFile *f = TFile::Open(bremInputFilename);
  bool confirm = f->cd();
  if(!confirm)
    exit(10);
  TTree *ChopperData;
  f->GetObject("Brem", ChopperData);
  //ChopperData->Print();

  double Emax = ChopperData->GetMaximum("Energy");
  std::cout << "MantisROOT::ReturnBremMax -> Max Found: " << Emax << std::endl;
  return Emax;
}

TH1D* MantisROOT::BuildSimpleSample(const char* bremInputFilename, double deltaE, double cut_energy, double weight)
{
  double Emax = ReturnBremMax(bremInputFilename);

  int nbins = Emax/deltaE;
  TH1D *hSample = new TH1D("hSample", "hSample", nbins, 0., Emax);
  double theWeight = 1./weight;

  if(debug)
  {
    std::cout << "MantisROOT::BuildSimpleSample -> Weights below: " << cut_energy << " set to: "
    << theWeight << std::endl;
  }
	// create the sampling distribution
	// user can adjust relative scales in SetBinContent
	for (int i = 1; i <= nbins; ++i)
  {
		double e = hSample->GetBinCenter(i);

		if (e < cut_energy)
			hSample->SetBinContent(i, theWeight);
		else
			hSample->SetBinContent(i, 1);
	}


	// normalize hSample so that its integral is 1
	hSample->Scale(1.0/(hSample->Integral()));
  std::cout << "MantisROOT::BuildSimpleSample -> Sampling Distribution Created." << std::endl;
  return hSample;
}

void MantisROOT::WriteSampling(TH1D* hBrems, TH1D* hSample, TGraph* gBrems, TGraph* gSample, double bin_width)
{
  std::cout << "MantisROOT::WriteSampling -> Writing to file..." << std::endl;
  hSample->SetTitle("NRF importance sampling distribution");
  gSample->SetTitle("NRF importance sampling distribution");
  hSample->GetXaxis()->SetTitle("energy #it{E} [MeV]");
  string titleProb = "probability per " + std::to_string(bin_width*1e6) + " eV";
  hSample->GetYaxis()->SetTitle(titleProb.c_str());
  gSample->GetXaxis()->SetTitle("Energy #it{E} [MeV]");
  gSample->GetYaxis()->SetTitle(titleProb.c_str());

  // save everything to file
  TFile *fout = new TFile("brems_distributions.root","recreate");
  fout->cd();
  gBrems->Write();
  hBrems->Write();
  gSample->Write();
  hSample->Write();
  std::cout << "MantisROOT::WriteSampling -> File Complete. Saved to brems_distributions.root" << std::endl;
  fout->Close();
}

void MantisROOT::SimpleSampling(const char* bremInputFilename, double deltaE=5.0e-6, double cut_energy=1.5, double weight=10000, bool checkZero=false)
{
  if(gSystem->AccessPathName(bremInputFilename))
  {
    std::cerr << "ERROR Reading: " << bremInputFilename << std::endl;
    exit(1);
  }

  TH1D* hBrems = BuildBrem(bremInputFilename, deltaE, checkZero);
  hBrems->Print();
	TGraph *gBrems = new TGraph(hBrems);
  TH1D* hSample = BuildSimpleSample(bremInputFilename, deltaE, cut_energy, weight);
  TGraph *gSample = new TGraph(hSample);

  WriteSampling(hBrems,hSample,gBrems,gSample,deltaE);

  std::cout << "MantisROOT::SimpleSampling -> Complete." << std::endl;

}

void MantisROOT::Sampling(const char *bremInputFilename, string sample_element="U",
							double deltaE=5.0e-6, bool checkZero=false, double non_nrf_energy_cut=1.5)
{
	// Convert Input Bremsstrahlung Spectrum Histogram to TGraph
	if(gSystem->AccessPathName(bremInputFilename))
	{
		std::cerr << "ERROR Reading: " << bremInputFilename << std::endl;
		exit(1);
	}

  TH1D* hBrems = BuildBrem(bremInputFilename, deltaE, checkZero);
  TGraph* gBrems = new TGraph(hBrems);
  double Emax = ReturnBremMax(bremInputFilename);
  int nbins = Emax/deltaE;
	// resonance energies in MeV as calculated by G4NRF
	vector<double> Evec;
	vector<double> Evec_above_threshold;

	if(sample_element == "U")
	{
		std::cout << "Sampling for Uranium!" << std::endl;
		// U-238
		Evec.push_back(1.78200716196); // Vavrek
		Evec.push_back(1.84600768563); // Vavrek
		Evec.push_back(2.17601067909); // Vavrek
		Evec.push_back(2.20901100546); // Vavrek
		Evec.push_back(2.24501136709); // Vavrek

		// U-235
		Evec.push_back(1.65623622215); // Me
		Evec.push_back(1.73354686425); // Vavrek
		Evec.push_back(1.81525753275); // Vavrek
		Evec.push_back(1.86231786689); // Me
		//Evec.push_back(2.00336916735); // Vavrek
		Evec.push_back(2.00619912943); // Me
	}
	else if(sample_element == "Pu")
	{
		std::cout << "Sampling for Plutonium!" << std::endl;
		// Pu-239
		Evec.push_back(2.13501023737);
		Evec.push_back(2.14357031961);
		Evec.push_back(2.15101039142);
		Evec.push_back(2.43171328004);

		// Pu-240
		Evec.push_back(2.43321324158);
		Evec.push_back(2.57751485869);
		Evec.push_back(2.56641473101);
	}
	else
	{
		std::cout << "Sample element not found. Exiting..." << std::endl;
		exit(100);
	}

	for(int i=0;i<Evec.size();i++)
	{
	  if(Evec[i] <= Emax)
	  {
	    Evec_above_threshold.push_back(Evec[i]);
	  }
	}

	TH1D *hSample = new TH1D("hSample", "hSample", nbins, 0., Emax);

	// create the sampling distribution
	// user can adjust relative scales in SetBinContent
	for (int i = 1; i <= nbins; ++i) {
		double e = hSample->GetBinCenter(i);

		for (int j = 0; j < Evec_above_threshold.size(); ++j)
		{
			if (e < non_nrf_energy_cut)
			{
				hSample->SetBinContent(i, 0.0001);
			}
  		else if (e > Evec_above_threshold[j] - deltaE && e < Evec_above_threshold[j] + deltaE)
  		{
				hSample->SetBinContent(i, 1);
				break;
  		}
  		else
  		{
				hSample->SetBinContent(i, 0.1);
			}
		}
	}

	// normalize hSample so that its integral is 1
	hSample->Scale(1.0/(hSample->Integral()));
	TGraph *gSample = new TGraph(hSample);
	WriteSampling(hBrems,hSample,gBrems,gSample,deltaE);

}// End of sampling

void MantisROOT::CheckIntObj(const char* onFile, const char* offFile, double Er, bool Weighted=false)
{
  if(gSystem->AccessPathName(onFile) || gSystem->AccessPathName(offFile))
  {
    std::cout << "MantisROOT::CheckIntObj -> Files Not Found." << std::endl;
    exit(1);
  }

  // On Analysis
  TFile *f = new TFile(onFile);
  if(f ==0)
  {
    std::cout << "MantisROOT::CheckIntObj -> File " << onFile << " NULL!" << std::endl;
    exit(1);
  }
  bool confirmf = f->cd();
  if(!confirmf)
  {
    std::cout << "MantisROOT::CheckIntObj -> Cound Not Change into " << onFile << " directory." << std::endl;
    exit(1);
  }
  TTree* inTree;
  f->GetObject("IntObjIn", inTree);
  std::cout << "MantisROOT::CheckIntObj -> " << onFile << " IntObjIn grabbed." << std::endl;
  double emin = Er - 100e-6;
  double emax = Er + 100e-6;
  TH1D *e1 = new TH1D("e1","Incident Interrogation Object 2eV Binning",100, emin, emax);
  if(Weighted)
    inTree->Draw("Energy>>e1","Weight","goff");
  else
    inTree->Draw("Energy>>e1","","goff");

  e1->SetStats(0);
  e1->Sumw2();
  e1->Print();

  // Off Analysis
  TFile *f2 = new TFile(offFile);
  if(f2 ==0)
  {
    std::cout << "MantisROOT::CheckIntObj -> ERROR File " << offFile << " NULL!" << std::endl;
    exit(1);
  }
  bool confirmf2 = f2->cd();
  if(!confirmf2)
  {
    std::cout << "MantisROOT::CheckIntObj -> ERROR Could NOT Change into " << offFile << " directory." << std::endl;
    exit(1);
  }

  TTree *inTree2;
  f2->GetObject("IntObjIn", inTree2);
  std::cout << "MantisROOT::CheckIntObj -> " << offFile << " IntObjIn grabbed." << std::endl;

  TH1D *e2 = new TH1D("e2","Incident Interrogation Object 2eV Binning",100, emin, emax);

  if(Weighted)
    inTree2->Draw("Energy>>e2","Weight","goff");
  else
    inTree2->Draw("Energy>>e2","","goff");

  e2->SetStats(0);
  e2->Sumw2();
  e2->SetLineColor(kRed);
  e2->Print();

  TCanvas *c1 = new TCanvas();
  c1->cd();
  gPad->SetTicks(1,1);
  e2->Draw();
  f->cd();

  e1->Draw("SAME");

  e2->GetXaxis()->SetTitle("Incident Energy [MeV]");
  e2->GetYaxis()->SetTitle("Counts/2eV");

  auto legend = new TLegend();
  legend->SetHeader("Chopper State", "C");
  legend->AddEntry(e1, "Chopper On");
  legend->AddEntry(e2, "Chopper Off");
  legend->Draw();

  //f->Close();
  //f2->Close();
  std::cout << "MantisROOT::CheckIntObj -> Complete." << std::endl;

} // end of CheckIntObj

void MantisROOT::CheckAngles(const char* filename, int estimate=-1)
{
  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Checking Angles..." << std::endl;

  if(gSystem->AccessPathName(filename))
  {
    std::cout << "MantisROOT::CheckAngles -> File " << filename << " Not Found." << std::endl;
    exit(1);
  }

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Opening File." << std::endl;

  TFile *f = new TFile(filename);
  f->cd();

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> File opened." << std::endl;

  TTree *t1, *t2;
  f->GetObject("IntObjOut",t1);
  f->GetObject("DetInfo",t2);
  t1->SetEstimate(estimate);
  t2->SetEstimate(estimate);
  std::cout << "MantisROOT::CheckAngles -> Grabbing IntObjOut Angles and Events." << std::endl;
  int entries = t1->Draw("Theta:Phi:EventID","","goff");

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> IntObjOut Entries: " << entries << std::endl;

  Double_t* thetas = t1->GetVal(0);
  Double_t* phis = t1->GetVal(1);
  Double_t* eventID = t1->GetVal(2);

  std::vector<double> thetasv;
  std::vector<double> phisv;
  std::vector<int> eventsv;

  if(estimate == -1)
  {
    for(int i=0;i<entries;++i)
    {
      thetasv.push_back(thetas[i]);
      phisv.push_back(phis[i]);
      eventsv.push_back((int)eventID[i]);
    }
  }
  else
  {
    for(int i=0;i<estimate;++i)
    {
      thetasv.push_back(thetas[i]);
      phisv.push_back(phis[i]);
      eventsv.push_back((int)eventID[i]);
    }
  }

  std::cout << "MantisROOT::CheckAngles -> IntObjOut Angles and Events Grabbed." << std::endl;
  std::cout << "MantisROOT::CheckAngles -> Grabbing Detinfo Events..." << std::endl;

  int entries2 = t2->Draw("EventID","","goff");

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> DetInfo Entries: " << entries2 << std::endl;

  Double_t* eventID2 = t2->GetVal(0);
  std::vector<int> events2v;

  if(estimate == -1)
  {
    for(int i=0;i<entries2;++i)
      events2v.push_back((int)eventID2[i]);
  }
  else
  {
    for(int i=0;i<estimate;++i)
      events2v.push_back((int)eventID2[i]);
  }

  std::cout << "MantisROOT::CheckAngles -> DetInfo Events Grabbed." << std::endl;

  std::cout << "MantisROOT::CheckAngles -> Removing Duplicates and Sorting DetInfo..." << std::endl;
  std::sort(events2v.begin(),events2v.end());
  events2v.erase(unique(events2v.begin(),events2v.end()),events2v.end());
  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Duplicates Removed. New Vector Size: " << events2v.size() << std::endl;

  // Compare Event IDs
  std::vector<double> det_thetasv;
  std::vector<double> det_phisv;
  std::vector<int> det_eventsv;
  int x;

  if(estimate == -1)
  {
    for(int i=0;i<eventsv.size();++i)
    {
      if(i % 100 == 0)
        std::cout << "\r** Checking Entry: " << i << std::flush;
      // Grab IntObjOut EventID
      x = eventsv[i];
      // Check if DetInfo EventID matches IntObjOut EventID
      auto exists = std::find(events2v.begin(),events2v.end(), x);

      if(exists != events2v.end())
      {
        det_eventsv.push_back(x);
        det_thetasv.push_back(thetasv[i]);
        det_phisv.push_back(phisv[i]);
      }

    }
  }
  else
  {
    for(int i=0;i<estimate;++i)
    {
      if(i % 100 == 0)
        std::cout << "\r** Checking Entry: " << i << std::flush;
      // Grab IntObjOut EventID
      x = eventsv[i];
      // Check if DetInfo EventID matches IntObjOut EventID
      auto exists = std::find(events2v.begin(),events2v.end(), x);

      if(exists != events2v.end())
      {
        det_eventsv.push_back(x);
        det_thetasv.push_back(thetasv[i]);
        det_phisv.push_back(phisv[i]);
      }

    }
  }

  std::cout << std::endl << "MantisROOT::CheckAngles -> Search complete." << std::endl;
  string filename_string = string(filename);
  string filenamebase = EraseSubStr(filename_string, ".root");
  string outfile = "Check_Angles_" + filenamebase + ".root";
  TFile *fout = new TFile(outfile.c_str(),"RECREATE");
  fout->cd();
  TTree *tAngle = new TTree("tAngle","IntObjOut Emission Angles Leading to Detection");
  int theEvent;
  double theTheta, thePhi;

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Assigning Branches..." << std::endl;

  tAngle->Branch("EventID",&theEvent);
  tAngle->Branch("Theta", &theTheta);
  tAngle->Branch("Phi", &thePhi);

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Filling TTree..." << std::endl;

  for(int i=0;i<det_eventsv.size();++i)
  {
    theEvent = det_eventsv[i];
    theTheta = det_thetasv[i];
    thePhi = det_phisv[i];
    tAngle->Fill();
  }

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Writing..." << std::endl;

  tAngle->Write();
  std::cout << "MantisROOT::CheckAngles -> Events and Angles written to Check_Angles.root" << std::endl;
  fout->Close();
  f->cd();
  f->Close();


} // end of CheckAngles

//******************************************************************************//
//******************************************************************************//
//******************************************************************************//
// Begin Show Functions

void MantisROOT::Show_Help()
{
  std::cout << "void Help()" << std::endl;
}

void MantisROOT::Show_Help_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Help lists the available function calls and their descriptions."
  << std::endl << "To See just the Functions calls try Show()." << std::endl;
}

void MantisROOT::Show_Show()
{
  std::cout << "void Show(string function=All, bool description=false)" << std::endl;
}

void MantisROOT::Show_Show_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Show lists the available functions."
  << std::endl << "For a Specific Functions call pass the function name to Show()."
  << std::endl << "For the Description of the function in addition to the call inputs pass a bool true as the second input."
  << std::endl << "Example: mantis->Show(\"Show\",true)" << std::endl;
}

void MantisROOT::Show_OpenFile()
{
  std::cout << "TFile* OpenFile(const char* filename)" << std::endl;
}

void MantisROOT::Show_OpenFile_Description()
{
  std::cout <<"DESCRIPTION: " << std::endl << "Opens a TFile with the input filename. Returns TFile*" << std::endl;
}

void MantisROOT::Show_CombineFiles()
{
  std::cout << "void CombineFiles(std::vector<string> filenames, std::vector<string> ObjectsToCombine, const char* Outfilename)" << std::endl;
}

void MantisROOT::Show_CombineFiles_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Combines files through TChain Method."
  << std::endl << "Takes a vector of filenames with the TTree object names in the second input vector to merge into the third input (filename)."
  << std::endl << "Example: mantis->CombineFiles({\"file1.root\",\"file2.root\"},{\"IntObjIn\",\"DetInfo\"},\"Merged.root\")" << std::endl;
}

void MantisROOT::Show_CopyTrees()
{
  std::cout << "void CopyTrees(const char* filename, std::vector<string> TTreeNamesToCopy)" << std::endl;
}

void MantisROOT::Show_CopyTrees_Description()
{
  std::cout << "Clones TTrees from input 1 (filename) to a file named converted_(filename)."
  << std::endl << "The TTrees to be cloned are inputed as input two as a string vector."
  << std::endl << "Example: mantis->CopyTrees(\"test.root\",{\"IntObjIn\",\"DetInfo\"})" << std::endl;
}

void MantisROOT::Show_Sig2Noise()
{
  std::cout << "void Sig2Noise(std::vector<string> Filenames, string DataName, bool Weighted=false, bool cut=false, TCut cut1=\"NA\")" << std::endl;
}

void MantisROOT::Show_Sig2Noise_Description()
{
  std::cout << "Computes the Signal to Noise Ratio in the files inputed in the string vector."
  << std::endl << "The signal to noise ratio can be computed for the Incident Interrogation Object spectrum"
  << std::endl << ", the detected spectrum, or both with the second input options: IncObj, Det, Both."
  << std::endl << "IF the TTrees contain weights be sure to set the third input bool option to true."
  << std::endl << "IF the TTrees are to have a cut placed the fourth input should be set to true and "
  << std::endl << " the fifth input should contain the TCut in parenthesis."
  << std::endl << "Example: mantis->Sig2Noise({\"TestOn.root\",\"TestOff.root\"},\"Both\", true, true, \"Energy<5e-6\")" << std::endl;
}

void MantisROOT::Show_ZScore()
{
  std::cout << "void ZScore(const char* filename1, const char* filename2, std::vector<string> ObjectNames)" << std::endl;
}

void MantisROOT::Show_ZScore_Description()
{
  std::cout << "Computes ZTest on input 1 filename and input 2 filename for the TTree object names in input 3 string vector."
  << std::endl << "Example: mantis->ZScore(\"TestOn.root\", \"TestOff.root\", {\"IntObjIn\",\"DetInfo\"})" << std::endl;
}
void MantisROOT::Show_Integral()
{
  std::cout << "void Integral(TTree* Object)" << std::endl << "void Integral(std::vector<TTree*> Objects)"
  << std::endl << "void Integral(std::vector<TTree*> Object, TCut IntegralCut)" << std::endl;
}

void MantisROOT::Show_Integral_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Takes either a TTree object input, a vector of TTree objects, or a vector of TTree Objects with a TCut."
  << std::endl << "Computes the integral of the TTree with various methods."
  << std::endl << "If TCut is added the integral is determined after the TCut is applied."
  << std::endl << "Example: mantis->Integral(DetTree)" << std::endl;
}

void MantisROOT::Show_PredictThickness()
{
  std::cout << "void PredictThickness(std::vector<string> ObjectNames)" << std::endl
  << "void PredictThickness(std::vector<string> ObjectNames, double ResonanceEnergy)" << std::endl;
}

void MantisROOT::Show_PredictThickness_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Predicts the results of changing the thickness on the objects passed in the string vector."
  << std::endl << "If the second input is passed the thickness calculations will focus on the given resonance energy."
  << std::endl << "Example: mantis->PredictThickness({\"IntObjIn\",\"IntObjOut\"},1.73354)" << std::endl;
}

void MantisROOT::Show_RebinHisto()
{
  std::cout << "void RebinHisto(std::vector<string> inFile, std::vector<string> ObjName, std::vector<string> OutObjName, int nbins, double Emin, double Emax)"
  << std::endl << "void RebinHisto(std::vector<string> inFile, std::vector<string> ObjName,std::vector<string> OutObjName, int nbins, double Emin, double Emax, TCut cut1)"
  << std::endl;
}

void MantisROOT::Show_RebinHisto_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Rebins the TTree Objects from multiples files given histogram parameters."
  << std::endl << "Example: mantis->RebinHisto({\"file1.root\",\"file2.root\"},{\"IntObjIn\",\"IntObjOut\"},{\"Rebinned_IntObjIn\",\"Rebinned_IntObjOut\"},100,1.7334,1.7336)"
  << std::endl << "This would rebin the Incident and Emission Interrogation object spectra into TH1D objects Rebinned_IntObjIn and Rebinned_IntObjOut respectively."
  << std::endl << "The rebinned histograms would have 100 bins between 1.7334 and 1.7336 MeV." << std::endl;
}

void MantisROOT::Show_VarRebin()
{
  std::cout << "void VarRebin(vector<string>, vector<string>, vector<string>, int, double, double, TCut, double, double)" << std::endl;
}

void MantisROOT::Show_VarRebin_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "See RebinHisto. This Function allows variable binning." << std::endl;
}

void MantisROOT::Show_CheckEvents()
{
  std::cout << "void CheckEvents(const char* filename, bool Weighted)" << std::endl;
}

void MantisROOT::Show_CheckEvents_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Checks TTrees NRF, Cherenkov and DetInfo for EventIDs that match."
  << std::endl << "If the events match the event data in the detector is collected and written to w_events_(filename)."
  << std::endl << "This function is called in Sig2Noise." << std::endl
  << "Example: mantis->CheckEvents(\"test.root\",true) would Check WEIGHTED events in file test.root." << std::endl;
}

void MantisROOT::Show_Sampling()
{
  std::cout << "void Sampling(const char* bremInputFilename, string sample_element=U, double deltaE=5.0e-6, bool checkZero=false, double non_nrf_energy_cut=1.5)" << std::endl;
}

void MantisROOT::Show_Sampling_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Creates an importance sampling distribution and prepares mantis input file brems_distributions.root."
  << std::endl << "If the user would like a different bin width for hBrems than the user can supply the bin width with input 3."
  << std::endl << "Example: mantis->Sampling(\"brem.root\",\"U\", 5e-6,true,1.5)"
  << std::endl << "would create brems_distributions.root with 5e-6 bin widths where if any bin content = 0 that bin would be set to the prior bins content"
  << std::endl << "the importance sampling distribution energies below 1.5 MeV would have importances 1/1000 of the NRF resonance energies."
  << std::endl;
}

void MantisROOT::Show_SimpleSampling()
{
  std::cout << "void SimpleSampling(const char* bremInputFilename, double deltaE=5.0e-6, double cut_energy=1.5, double weight=10000, bool checkZero=false)" << std::endl;
}

void MantisROOT::Show_SimpleSampling_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Creates an importance sampling distribution and prepares mantis input file brems_distributions.root."
  << std::endl << "If the User would like a different bin width for hBrems than the user can supply the bin width with input 2."
  << std::endl << "Example: mantis->SimpleSampling(\"brem.root\", 5e-6,1.5, 10000, true)"
  << std::endl << "would create brems_distributions.root with 5e-6 bin widths where if any bin content = 0 that bin would be set to the prior bins content"
  << std::endl << "the importance sampling distribution energies below 1.5 MeV would have importances 1/10000 of all energies above 1.5 MeV."
  << std::endl;
}

void MantisROOT::Show_CheckIntObj()
{
  std::cout << "void CheckIntObj(const char* onFile, const char* offFile, double Er, bool Weighted=false)" << std::endl;
}

void MantisROOT::Show_CheckIntObj_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Draws a 200eV Wide Region centered on Er with 2eV bin widths for both Chopper States (On/Off)."
  << std::endl << "If the spectra contain weights be sure to add the boolean true to the fourth input." << std::endl;
}

void MantisROOT::Show_CheckAngles()
{
  std::cout << "void CheckAngles(const char* filename, int estimate)" << std::endl;
}

void MantisROOT::Show_CheckAngles_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Determines which Angles emitted from IntObjOut are Detected."
  << std::endl << "Estimate sets the limit on how many events to check. "
  << std::endl << "Usefull for determining emission angle cuts to place." << std::endl;
}

void MantisROOT::Show_GetInstance()
{
  std::cout << "Run MantisROOT* m = MantisROOT::GetInstance(bool debug=false) to get MantisROOT singleton." << std::endl;
}
//******************************************************************************//
//******************************************************************************//
//******************************************************************************//
// End Show Functions, Allow Help to be called outside of MantisROOT Class

string MantisROOT::EraseSubStr(string & mainStr, const string & toErase)
{
    // Search for the substring in string
    string newString = mainStr;
    size_t pos = newString.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        newString.erase(pos, toErase.length());
    }
    return newString;
}

void Help()
{
  MantisROOT* m = MantisROOT::GetInstance();
  m->Help();
}


//
// end of file
