// to Run in root: root -b -q 'Sampling.cc("brem.root", Emax)'
void Sampling(const char *bremInputFilename, double Emax, string sample_element)
{
	cout << "Emax set to: " << Emax << endl; // spectrum max energy in MeV

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

	double deltaE = 5.0e-6; // width of each important sampling region in MeV

	Int_t nbins = (Emax)/(deltaE);

	TH1D *hSample = new TH1D("hSample", "hSample", nbins, 0., Emax);

	// create the sampling distribution
	// user can adjust relative scales in SetBinContent
	for (int i = 1; i <= nbins; ++i) {
		double e = hSample->GetBinCenter(i);

		for (int j = 0; j < Evec_above_threshold.size(); ++j) {
			if (e < 1.5) {
				hSample->SetBinContent(i, 0.0001);
			}
            		else if (e > Evec_above_threshold[j] - deltaE && e < Evec_above_threshold[j] + deltaE)
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
	// Convert Sample histogram to TGraph
	TGraph *sampleGraph = new TGraph(hSample);

	// Convert Input Bremsstrahlung Spectrum Histogram to TGraph
	if(gSystem->AccessPathName(bremInputFilename) != 0)
	{
		std::cerr << "ERROR Reading: " << bremInputFilename << std::endl;
		exit(1);
	}

	TFile *f = TFile::Open(bremInputFilename);
	TTree *ChopperData;
	f->GetObject("Chopin", ChopperData);
	ChopperData->Print();

	nbins = sqrt(ChopperData->GetEntries()); // set number of bins to sqrt(entries)
	TH1D *hBrems = new TH1D("hBrems","Brem Histo",nbins,0.,Emax);
	// Convert TTree to Histo
	ChopperData->Draw("Energy>>hBrems","","goff");
	// normalize hBrems so that its integral is 1
	hBrems->Scale(1.0/(hBrems->Integral()));
	// Convert Histo to TGraph
	TGraph *bremsGraph = new TGraph(hBrems);
	bremsGraph->SetTitle("Bremsstrahlung Distribution");


	TCanvas *c0 = new TCanvas();
	c0->cd();
	gPad->SetTicks(1,1);
	gPad->SetLogy();

	sampleGraph->Draw();
	bremsGraph->SetLineColor(kRed);
	bremsGraph->Draw("same");
	sampleGraph->GetYaxis()->SetRangeUser(1e-9, 1e-1);
	sampleGraph->SetTitle("NRF importance sampling distribution");
	sampleGraph->GetXaxis()->SetTitle("energy #it{E} [MeV]");
	sampleGraph->GetYaxis()->SetTitle("probability per 5 eV");
	//sampleGraph->SetStats(0);
	c0->SaveAs("brems_distributions.png");
	std::cout << "brems_distributions.png created!" << std::endl;

	// save everything to file
	TFile *fout = new TFile("brems_distributions.root","recreate");
	fout->cd();
	bremsGraph->Write();
	sampleGraph->Write();
	std::cout << "File Complete. Saved to brems_distributions.root" << std::endl;
}
