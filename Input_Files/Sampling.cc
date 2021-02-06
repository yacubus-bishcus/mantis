// to Run in root: root -b -q 'Sampling.cc("brem.root", Emax)'
void Sampling(const char *bremInputFilename, double Emax, string sample_element)
{
	const double pi = TMath::Pi();
	double Emin = 0.0; // spectrum min energy in MeV
	cout << "Emax set to: " << Emax << endl; // spectrum max energy in MeV

	// resonance energies in MeV as calculated by G4NRF
	vector<double> Evec;
	vector<double> Evec_above_threshold;
	
	if(sample_element == "U")
	{
		std::cout << "Sampling for Uranium!" << std::endl;
		// U-238
		Evec.push_back(1.78200716196); // 33
		Evec.push_back(1.79300725065); // 6.21
		Evec.push_back(1.84600768563); // 31
		Evec.push_back(2.17601067909); // 24e-3
		Evec.push_back(2.20901100546); // 18e-3
		Evec.push_back(2.24501136709); // 14.2e-3

		// U-235
		Evec.push_back(1.65624253132); // 2.79 1.65623626566 1.65624248786
		Evec.push_back(1.73355368089); //22.4 1.73354686425 1.73355368089
		Evec.push_back(1.8152575266); // 8.9 1.81525753275
		Evec.push_back(1.86232578887); // 5.88 1.86231792191 1.86232578887
		Evec.push_back(2.00336916735); // 6.7 2.00336916735
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
	
	double maxNRF = *std::max_element(Evec_above_threshold.begin(), Evec_above_threshold.end());
	double minNRF = *std::min_element(Evec_above_threshold.begin(), Evec_above_threshold.end());
	std::cout << "Minimum NRF: " << minNRF << std::endl;
	std::cout << "Maximum NRF Energy: " << maxNRF << std::endl;
	double deltaE = 10.0e-6; // width of each important sampling region in MeV

	Int_t nbins = (Emax-Emin)/(deltaE/2.0);

	TH1D *hSample = new TH1D("hSample", "hSample", nbins, Emin, Emax);

	// create the sampling distribution
	// user can adjust relative scales in SetBinContent
	for (int i = 1; i <= nbins; ++i) {
		double e = hSample->GetBinCenter(i);

		for (int j = 0; j < Evec_above_threshold.size(); ++j) {
			if (e < minNRF - 0.01) {
				hSample->SetBinContent(i, 0.0001);
			}
            		else if (e > Evec_above_threshold[j] - deltaE/2.0 && e < Evec_above_threshold[j] + deltaE/2.0)
            		{
				hSample->SetBinContent(i, 1);
				break;
            		}
            		else
            		{
				hSample->SetBinContent(i, 0.01);
			}
		}
	}

	// normalize hSample so that its integral is 1
	hSample->Scale(1.0/(hSample->Integral()));


	// also create a normalized brems spectrum for weighting
	TFile *f = TFile::Open(bremInputFilename);
	std::cout << "Root input File: " <<bremInputFilename << " being read..." << std::endl;
	TTree *ChopperData;
	f->GetObject("ChopperData", ChopperData);
	ChopperData->Print();
	int brems_nbin = 300;
	TH1D *ho = new TH1D("ho","ho", brems_nbin, Emin, Emax);
	if (f != NULL) {
		ChopperData->Draw("ChopperData>>ho", "", "goff");
	} else {
		cout << "Error! TFile not found.\nAborting..." << endl;
		exit(1);
	}

	ho->Smooth(1024);
	double xmin = ho->GetXaxis()->GetXmin();
	double xmax = ho->GetXaxis()->GetXmax();
	TH1D *hBrems = new TH1D("hBrems", "hBrems", nbins, xmin, xmax);
	std::cout << "Creating hBrems with " << nbins << " bins..." << std::endl;
    
	for (int i = 1; i <= nbins; ++i) 
	{
     double value = ho->GetBinContent(brems_nbin*(i-1)/nbins+1);
		hBrems->SetBinContent(i, value);

	}
	std::cout << "Finished creating hBrems!" << std::endl;
	hBrems->Scale(1.0/hBrems->Integral());


	TCanvas *c0 = new TCanvas();
	c0->cd();
	gPad->SetTicks(1,1);
	gPad->SetLogy();

	hSample->Draw("HIST");
	hBrems->SetLineColor(kRed);
	hBrems->SetTitle("bremsstrahlung distribution");
	hBrems->Draw("HIST, same");
	std::cout << "hBrems Drawn!" << std::endl;

	hSample->GetYaxis()->SetRangeUser(1e-9, 1e-1);
	hSample->SetTitle("NRF importance sampling distribution");
	hSample->GetXaxis()->SetTitle("energy #it{E} [MeV]");
	hSample->GetYaxis()->SetTitle("probability per 5 eV");
	hSample->SetStats(0);
	c0->SaveAs("brems_distributions.png");
	std::cout << "brems_distributions.png created!" << std::endl;

	// save everything to file
	TFile *fout = new TFile("brems_distributions.root","recreate");
	fout->cd();
	hBrems->Write();
	hSample->Write();
	std::cout << "File Complete. Saved to brems_distributions.root" << std::endl;
}
