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

  inObj->Print();
  double Emax = inObj->GetMaximum("Energy");
  TH1D *e1 = new TH1D("e1","Histogram",100,0.,Emax);

  if(cut1 == "NA")
    inObj->Draw("Energy>>e1","","goff");
  else
    inObj->Draw("Energy>>e1",cut1,"goff");

  e1->Print();
  hIntegral(e1);
}
