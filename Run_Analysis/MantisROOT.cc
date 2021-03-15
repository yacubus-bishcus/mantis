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

#include "MantisCopyATree.cc"
#include "MantisCopyATreeNoWeight.cc"
#include "SNR.cc"
#include "ZTest.cc"
#include "Rebin.cc"
#include "Rescale.cc"

void Help()
{
  std::cout << "To Instantiate MantisROOT Call: MantisROOT* mantis = MantisROOT::GetInstance()" << std::endl;
  std::cout << "Calls and Descriptions" << std::endl << std::endl;

  std::cout << "Call: " << std::endl << "CombineFiles(string base_filename, string outfilename)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Stitches files together using the root of a filename (without .root extension) "
  << std::endl << "and writes the combined data to a new file with the outfilename. " << std::endl;

  std::cout << std::endl << "Call:" << std::endl << "CopyTrees(const char* filename, vector<string> CopyData)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Copies TTrees from filename with names provided by user in the second input option." << std::endl
  << "If the user wishes to copy trees with weights (importance sampling simulation data) " << std::endl
  << "simply include 'Weight' in the second input vector." << std::endl
  << "Available Options for the string vector include: " << std::endl
  << "Weight, Brem, Chop, NRF, IntObj, Water, Cherenkov, Det" << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "Sig2Noise(vector<string> filenames)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Completes signal to noise ratio calculations on the interrogation object "
  << "for each of the files provided." << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "ZScore(const char* file1, const char* file2, vector<string> DataName)"
  << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Performs ZScore Calculation for files 1 and 2 for the given data sets name." << std::endl
  << "DataName Options are: " << std::endl << "Brem, ChopIn, ChopOut, NRF, " << std::endl
  << "AirIn, IntObjIn, IntObjOut, " << std::endl << "Water, Cherenkov" << std::endl
  << "DetInfo, IncDetInfo" << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "Integral(std::vector<TTree*> TTreeNames)" << std::endl;
  std::cout << "Integral(std::vector<TTree*> TTreeNames, TCut cut)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Performs Integral Calculation on the TTrees provided." << std::endl;
  std::cout << "Added Option allows for a cut to be placed on TTree prior to integration." << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "PredictThickness(std::vector<string> PredictData)" << std::endl;
  std::cout << "PredictThickness(vector<string> PredictData, double Resonance_Energy)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Predicts spectra adjustment for a given chopper thickness based on the data from another chopper thickness." << std::endl
  << "Data Options: " << std::endl << "ChopOut, IntObjIn, IntObjOut" << std::endl
  << "If the additional option for the resonance energy is passed the Output histogram will be centered on the given resonance energy." << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "RebinHisto(vector<string> inFile, vector<string> ObjName, vector<string> OutObjName, int nbins, double Emin, double Emax)" << std::endl;
  std::cout << "RebinHisto(vector<string> inFile, vector<string> ObjName, vector<string> OutObjName, int nbins, double Emin, double Emax, TCut cut1)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Saves the histogram of ObjName to OutObjName with nbins [Emin, Emax]." << std::endl;
  std::cout << "Added Option allows for a TCut to be applied prior to histogramming." << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "VarRebin(vector<string> inFile, vector<string> ObjName, vector<string> OutObjName, int nbins, double Region1CutoffEnergy, "
  << std::endl << " \t \t double Region2CutOffEnergy, TCut cut1, double binwidth1, double binwidth2)" <<std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Saves the histogram of ObjName to OutObjName with nbins with a TCut applied " << std::endl
  << "pass 'NA' to not apply any cuts. The bin width will vary for the two regions defined by the region cut off energies." << std::endl << std::endl;
}

class MantisROOT
{
public:
    MantisROOT();
    ~MantisROOT();

  public:
    static MantisROOT *GetInstance();
    static void Destroy();
    void Help();
    void Show(string);
    TFile* OpenFile(const char*);
    void CombineFiles(string, string);
    void CopyTrees(const char*, std::vector<string>);
    void Sig2Noise(std::vector<string>);
    void ZScore(const char*, const char*, std::vector<string>);
    void Integral(std::vector<TTree*>);
    void Integral(std::vector<TTree*>,TCut);
    void PredictThickness(std::vector<string>);
    void PredictThickness(std::vector<string>, double);
    void RebinHisto(vector<string>, vector<string>, vector<string>, int, double, double);
    void RebinHisto(vector<string>, vector<string>, vector<string>, int, double, double, TCut);
    void VarRebin(vector<string>, vector<string>, vector<string>, int, double, double, TCut, double, double);
  public:
    static MantisROOT *instance;
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

MantisROOT::MantisROOT()
{
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

  std::cout << "Call: " << std::endl << "CombineFiles(string base_filename, string outfilename)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Stitches files together using the root of a filename (without .root extension) "
  << std::endl << "and writes the combined data to a new file with the outfilename. " << std::endl;

  std::cout << std::endl << "Call:" << std::endl << "CopyTrees(const char* filename, vector<string> CopyData)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Copies TTrees from filename with names provided by user in the second input option." << std::endl
  << "If the user wishes to copy trees with weights (importance sampling simulation data) " << std::endl
  << "simply include 'Weight' in the second input vector." << std::endl
  << "Available Options for the string vector include: " << std::endl
  << "Weight, Brem, Chop, NRF, IntObj, Water, Cherenkov, Det" << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "Sig2Noise(vector<string> filenames)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Completes signal to noise ratio calculations on the interrogation object "
  << "for each of the files provided." << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "ZScore(const char* file1, const char* file2, vector<string> DataName)"
  << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Performs ZScore Calculation for files 1 and 2 for the given data sets name." << std::endl
  << "DataName Options are: " << std::endl << "Brem, ChopIn, ChopOut, NRF, " << std::endl
  << "AirIn, IntObjIn, IntObjOut, " << std::endl << "Water, Cherenkov" << std::endl
  << "DetInfo, IncDetInfo" << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "Integral(std::vector<TTree*> TTreeNames)" << std::endl;
  std::cout << "Integral(std::vector<TTree*> TTreeNames, TCut cut)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Performs Integral Calculation on the TTrees provided." << std::endl;
  std::cout << "Added Option allows for a cut to be placed on TTree prior to integration." << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "PredictThickness(std::vector<string> PredictData)" << std::endl;
  std::cout << "PredictThickness(vector<string> PredictData, double Resonance_Energy)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Predicts spectra adjustment for a given chopper thickness based on the data from another chopper thickness." << std::endl
  << "Data Options: " << std::endl << "ChopOut, IntObjIn, IntObjOut" << std::endl
  << "If the additional option for the resonance energy is passed the Output histogram will be centered on the given resonance energy." << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "RebinHisto(vector<string> inFile, vector<string> ObjName, vector<string> OutObjName, int nbins, double Emin, double Emax)" << std::endl;
  std::cout << "RebinHisto(vector<string> inFile, vector<string> ObjName, vector<string> OutObjName, int nbins, double Emin, double Emax, TCut cut1)" << std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Saves the histogram of ObjName to OutObjName with nbins [Emin, Emax]." << std::endl;
  std::cout << "Added Option allows for a TCut to be applied prior to histogramming." << std::endl;

  std::cout << std::endl << "Call: " << std::endl << "VarRebin(vector<string> inFile, vector<string> ObjName, vector<string> OutObjName, int nbins, double Region1CutoffEnergy, "
  << std::endl << " \t \t double Region2CutOffEnergy, TCut cut1, double binwidth1, double binwidth2)" <<std::endl;

  std::cout << "DESCRIPTION: " << std::endl << "Saves the histogram of ObjName to OutObjName with nbins with a TCut applied " << std::endl
  << "pass 'NA' to not apply any cuts. The bin width will vary for the two regions defined by the region cut off energies." << std::endl << std::endl;

}

void MantisROOT::CombineFiles(string base_filename, string outfilename)
{
  string command = "./stitch.sh \"" + base_filename + "*.root\" " + outfilename + ".root";
  std::cout << "Running Command: " << command << std::endl;
  system(command.c_str());
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
        MantisCopyATree(filename, optList[i].c_str());
    }
    else
    {
      if(dataList[i])
        MantisCopyATreeNoWeight(filename, optList[i].c_str());
    }
  }

  string outfilename = "converted_" + string(filename);
  std::cout << "All Trees Copied to " << outfilename << std::endl;
}

void MantisROOT::Sig2Noise(std::vector<string> filenames)
{
  for(int i=0;i<filenames.size();++i)
    SNR(filenames[i].c_str());

  std::cout << "Signal to Noise Ratio Analysis Complete." << std::endl;
}

void MantisROOT::ZScore(const char* file1, const char* file2, std::vector<string> objects)
{
  for(int i=0;i<objects.size();++i)
    ZTest(file1, file2, objects[i].c_str());

  std::cout << "ZScore Analysis Complete." << std::endl;
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

void MantisROOT::Show(string name)
{
  if(!name.compare("CopyTrees"))
    std::cout << "CopyTrees(const char* filename, std::vector<string> ObjectsToCopy)" << std::endl;
  else if(!name.compare("Sig2Noise"))
    std::cout << "Sig2Noise(std::vector<string> filenames)" << std::endl;
  else if(!name.compare("ZScore"))
    std::cout << "ZScore(const char* file1, const char* file2, std::vector<string> objects)" << std::endl;
  else if(!name.compare("Integral"))
    std::cout << "Integral(std::vector<TTree*> trees)" << std::endl << "Integral(std::vector<TTree*> trees, TCut cut1)" << std::endl;
  else if(!name.compare("PredictThickness"))
    std::cout << "PredictThickness(std::vector<string> obj)" << std::endl << "PredictThickness(std::vector<string> obj, double Er)" << std::endl;
  else if(!name.compare("RebinHisto"))
    std::cout << "RebinHisto(std::vector<string> inFile, std::vector<string> ObjName," << std::endl
              << "std::vector<string> OutObjName, int nbins, double Emin, double Emax)" << std::endl
              << "RebinHisto(std::vector<string> inFile, std::vector<string> ObjName," << std::endl
              << "std::vector<string> OutObjName, int nbins, double Emin, double Emax, TCut cut1)" << std::endl;
  else if(!name.compare("VarRebin"))
    std::cout << "VarRebin(std::vector<string> inFile, std::vector<string> ObjName," << std::endl
              << " std::vector<string> OutObjName, int nbins, double Ecut1, double Ecut2," << std::endl
              << " TCut cut1, double binwidth1, double binwidth2)" << std::endl;

  else if(!name.compare("CombineFiles"))
    std::cout << "CombineFiles(string base_filename, string outfilename)" << std::endl;
  else
    std::cout << "Error Function Not Found." << std::endl;
}

TFile* MantisROOT::OpenFile(const char* filename)
{
  return (new TFile(filename));
}
