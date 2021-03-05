//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File in ROOT CERN:
// root -b 'SNR.cc("testOn.root")'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 1 inputs
// 1. Filename. const char*
//
// This File Scans the File for the IntObjIn and IntObjOut TTrees and finds the SNR

void SNR(const char* inFile)
{
  // Check to make sure file exists
  if(gSystem->AccessPathName(inFile))
  {
    std::cerr << "ERROR Could not find " << inFile << "exiting..." << std::endl;
    exit(1);
  }
  TFile *f = new TFile(inFile);
  f->cd();
  TTree *aIntObjIn, *aIntObjOut;
  aIntObjIn = (TTree*) f->Get("IntObjIn");
  aIntObjOut = (TTree*) f->Get("IntObjOut");

  if(aIntObjIn !=0 && aIntObjOut !=0)
  {
    aIntObjIn->Print();
    aIntObjOut->Print();
    double inMax = aIntObjIn->GetMaximum("Energy");
    double outMax = aIntObjOut->GetMaximum("Energy");
    aIntObjIn->SetEstimate(-1);
    aIntObjOut->SetEstimate(-1);

    TH1D* e11 = new TH1D("e11","IntObjIn NRF Histogram",100,1.73350, 1.73360);
    TH1D* e10 = new TH1D("e10","IntObjIn NRF Histogram",100,1.65620,1.65630);
    TH1D* e12 = new TH1D("e12","IntObjIn NRF Histogram",100,1.81520,1.81530);
    TH1D* e13 = new TH1D("e13","IntObjIn NRF Histogram",100,1.86230,1.86240);
    TH1D* e14 = new TH1D("e14","IntObjIn NRF Histogram",100,2.00610,2.00620);

    TH1D* e21 = new TH1D("e21","IntObjOut NRF Histogram",100,1.73350,1.73360);
    TH1D* e20 = new TH1D("e20","IntObjOut NRF Histogram",100,1.65620,1.65630);
    TH1D* e22 = new TH1D("e22","IntObjOut NRF Histogram",100,1.81520,1.81530);
    TH1D* e23 = new TH1D("e23","IntObjOut NRF Histogram",100,1.86230,1.86240);
    TH1D* e24 = new TH1D("e24","IntObjOut NRF Histogram",100,2.00615,2.00625);

    TH1D* eT = new TH1D("eT","IntObjIn Histogram",100,0.0,inMax);
    TH1D* eT2 = new TH1D("eT2","IntObjOut Histogram",100,0.0,outMax);

    std::cout << "Drawing IntObjIn NRF Histograms..." << std::endl;
    aIntObjIn->Draw("Energy>>e10","Weight","goff");
    aIntObjIn->Draw("Energy>>e11","Weight","goff");
    aIntObjIn->Draw("Energy>>e12","Weight","goff");
    aIntObjIn->Draw("Energy>>e13","Weight","goff");
    aIntObjIn->Draw("Energy>>e14","Weight","goff");

    std::cout << "Drawing IntObjOut NRF Histograms..." << std::endl;
    aIntObjOut->Draw("Energy>>e20","Weight","goff");
    aIntObjOut->Draw("Energy>>e21","Weight","goff");
    aIntObjOut->Draw("Energy>>e22","Weight","goff");
    aIntObjOut->Draw("Energy>>e23","Weight","goff");
    aIntObjOut->Draw("Energy>>e24","Weight","goff");

    std::cout << "Drawing IntObjIn and IntObjOut Histograms..." << std::endl;
    aIntObjIn->Draw("Energy>>eT","Weight","goff");
    aIntObjOut->Draw("Energy>>eT2","Weight","goff");
    double eStart[] = {1.65623,1.73354,1.81525,1.86231,2.006197};
    double eEnd[5];

    for(int i=0;i<5;++i)
    {
      std::cout << "Bin Start Energy: " << std::setprecision(10) << eStart[i] << std::endl;
      eEnd[i] = eStart[i] + 0.00001;
      std::cout << "Bin End Energy: " << std::setprecision(10) << eEnd[i] << std::endl;
    }

    int binStart = e10->GetXaxis()->FindBin(eStart[0]);
    int binEnd = e10->GetXaxis()->FindBin(eEnd[0]);
    double inSignal[5];
    double outSignal[5];
    inSignal[0] = e10->Integral(binStart, binEnd);
    outSignal[0] = e20->Integral(binStart,binEnd);
    binStart = e11->GetXaxis()->FindBin(eStart[1]);
    binEnd = e11->GetXaxis()->FindBin(eEnd[1]);
    inSignal[1] = e11->Integral(binStart,binEnd);
    outSignal[1] = e21->Integral(binStart,binEnd);
    binStart = e12->GetXaxis()->FindBin(eStart[2]);
    binEnd = e12->GetXaxis()->FindBin(eEnd[2]);
    inSignal[2] = e12->Integral(binStart,binEnd);
    outSignal[2] = e22->Integral(binStart,binEnd);
    binStart = e13->GetXaxis()->FindBin(eStart[3]);
    binEnd = e13->GetXaxis()->FindBin(eEnd[3]);
    inSignal[3] = e13->Integral(binStart,binEnd);
    outSignal[3] = e23->Integral(binStart,binEnd);
    binStart = e14->GetXaxis()->FindBin(eStart[4]);
    std::cout << "Bin Start: " << binStart << std::endl;
    binEnd = e14->GetXaxis()->FindBin(eEnd[4]);
    std::cout << "Bin End: " << binEnd << std::endl;
    std::cout << "Integral: " << e14->Integral(binStart,binEnd) << std::endl;
    inSignal[4] = e14->Integral(binStart,binEnd);
    outSignal[4] = e24->Integral(binStart,binEnd);

    double tSignalin = 0;
    double tSignalout = 0;

    for(int i=0;i<5;++i)
    {
      std::cout << "IntObjIn " << eStart[i] << " MeV Signals: \t" << inSignal[i] << std::endl;
      tSignalin += inSignal[i];
      std::cout << "IntObjOut " << eStart[i] << " MeV Signals: \t" << outSignal[i] << std::endl;
      tSignalout += outSignal[i];
    }
    std::cout << "Total IntObjIn Signal: \t" << tSignalin << std::endl;
    std::cout << "Total IntObjOut Signal: \t" << tSignalout << std::endl;

    double inNoise = eT->Integral();
    std::cout << "IntObjIn Noise: \t" << inNoise << std::endl;
    double outNoise = eT2->Integral();
    std::cout << "IntObjOut Noise: \t" << outNoise << std::endl << std::endl;

    std::cout << "The NRF photons are removed from the beam in the chopper wheel stage."
              << std::endl
              << "A lower SNR is better because you want the signal from NRF to be minimized."
              << std::endl;
    std::cout << "IntObjIn SNR: \t" << abs(tSignalin - inNoise)/sqrt(inNoise) << std::endl;
    std::cout << "IntObjOut SNR: \t" << abs(tSignalout - outNoise)/sqrt(outNoise) << std::endl;
  }
}
