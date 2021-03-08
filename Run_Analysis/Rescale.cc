void Rescale(const char* inObj, double Er)
{
  std::cout << "Input the Scale Base Filename:" << std::endl;
  const char* filename;
  std::cin >> filename;
  std::vector<std::string> user_files;
  std::vector<float> user_thick;
  std::vector<TFile*> tfilesv;

  // check to make sure file exists
  if(gSystem->AccessPathName(filename))
  {
    std::cout << "ERROR Input Filename not found." << std::endl;
    exit(1);
  }

  // Handle the Base File
  user_files.push_back(filename);
  TFile *base = new TFile(filename);
  tfilesv.push_back(base);

  std::cout << "Input the Scale Base Thickness:" << std::endl;
  float b_thick;
  std::cin >> b_thick;
  user_thick.push_back(b_thick);

  std::string response = "Y";
  float thick;

  // Handle Files to be scaled
  while(!strcmp(response.c_str(),"Y"))
  {
    std::cout << "Input Filename to Scale: " << std::endl;
    std::cin >> filename;
    user_files.push_back(filename);
    TFile *data_file = new TFile(filename);
    tfilesv.push_back(data_file);
    std::cout << "Input Thickness: " << std::endl;
    std::cin >> thick;
    user_thick.push_back(thick);
  }

  base->cd();
  double lowE = Er - 0.0001;
  double highE = Er + 0.0001;

  TH1D *hBase = new TH1D("hBase","Scale Base Histogram",60,lowE,highE);
  TTree *base_tInObj;
  base->GetObject(inObj,base_tInObj);
  base_tInObj->SetEstimate(-1);
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
  for(int i=1;i<tfilesv.size();++i)
  {
    const char* hInName = "Scaled_";
    hInName = (std::string)hIn + std::to_string(i);
    TH1D *hIn = new TH1D(hInName,"Scaled Histogram",60,lowE,highE);
    TFile *scaleFile = tfilesv[i];
    scaleFile->cd();
    TTree *tInObj;
    scaleFile->GetObject(inObj,tInObj);
    const char* tCommand = "Energy>>";
    tCommand = (std::string)tCommand + (std::string)hInName;
    tInObj->Draw(tCommand,"Weight","goff");
    hOut.push_back(hIn);
    scaleFile->close();
  }

  std::cout << "Histograms created." << std::endl;
  for(int i=0;i<hOut.size();++i)
    hOut[i]->Print();

  std::cout << "Scaling Histograms." << std::endl;
  double newbinValue = 0;
  std::vector<TH1D*> outputHisto;
  for(int j=1;j<hOut.size();++j)
    for(int i=0;i<60;++i)
    {
      if(binValues[i] == 0)
        newbinValue = 0;
      else
        newbinValue = binValues[i]*pow(hOut[j]->GetBinContent(i)/binValues[i],1./user_thick[i]);

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

} // end of function
