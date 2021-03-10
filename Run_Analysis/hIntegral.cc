void hIntegral(TH1D *h)
{
  int nentries = h->GetEntries();
  TAxis *xaxis = h->GetXaxis();

  double intSum = 0;
  for(unsigned int i=0;i<nentries;++i)
  {
    double xVal = xaxis->GetBinCenter(i);
    double binVal = h->GetBinCenter(i);
    double integralVal = xVal*binVal;
    intSum += integralVal;
  }
  std::cout << intSum << std::endl;
}

void hIntegral(const char* filename, const char* objName)
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

  TH1D *inObj;
  f->GetObject(objName,inObj);
  if(inObj == 0)
    exit(0);

  inObj->Print();
  hIntegral(inObj);
}
