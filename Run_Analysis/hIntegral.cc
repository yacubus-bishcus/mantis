void hIntegral(TH1 *h)
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
  std::cout << "BinCenter Method Sum: " << intSum << std::endl;
  std::cout << "Mean Method Sum: " << nentries*hMean << std::endl;
}

void hIntegral(TTree *inObj,TCut cut1="NA")
{
  //inObj->Print();
  inObj->SetEstimate(-1);
  double Emax = inObj->GetMaximum("Energy");
  TH1D *e1 = new TH1D("e1","Histogram",100,0.,Emax);

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

  std::cout << "TTree Sum: " << intSum << std::endl;

  hIntegral(e1);
  delete e1; // avoids potential memory leak
}

void hIntegral(const char* filename, const char* objName, TCut cut1="NA")
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
  delete inObj; // avoids potential memory leak
}
