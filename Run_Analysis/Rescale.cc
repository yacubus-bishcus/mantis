void Rescale(const char* inObj, double Er)
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

void Rescale(const char* inObj)
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
          newbinValue = binValues[i]*pow(hOut[j]->GetBinContent(i)/binValues[i],user_thick[0]/user_thick[j]);
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

// end of file
