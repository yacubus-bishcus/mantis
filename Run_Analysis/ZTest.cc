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

#include "hIntegral.cc"

double ZTest(double c1, double c2)
{
  double zscore = abs(c1 - c2)/(sqrt(pow(sqrt(c1),2) + pow(sqrt(c2),2)));
  //std::cout << "The Z-Score: " << zscore << std::endl;
  return zscore;
}

void ZTest(const char* file1, const char* file2, const char* inObj)
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
  if(f !=0)
    f->cd();

  TTree *inTree;

  f->GetObject(inObj, inTree);
  double c1 = hIntegral(inTree, 0);
  double c11 = hIntegral(inTree,1);
  double c12 = hIntegral(inTree,2);
  double c13 = hIntegral(inTree,3);

  delete inTree;
  f->Close();

  TFile *f1 = new TFile(file2);
  if(f1 != 0)
    f1->cd();

  TTree *inTree2;
  f1->GetObject(inObj,inTree2);
  double c2 = hIntegral(inTree2,0);
  double c21 = hIntegral(inTree2,1);
  double c22 = hIntegral(inTree2,2);
  double c23 = hIntegral(inTree2,3);

  std::cout << "TTree Method Z-Score: " << ZTest(c1,c2) << std::endl;
  std::cout << "Bin Center Method Z-Score: " << ZTest(c11,c21) << std::endl;
  std::cout << "Mean Method Z-Score: " << ZTest(c12,c22) << std::endl;
  std::cout << "Integral Method Z-Score: " << ZTest(c13,c23) << std::endl;

  delete inTree2;
  f1->Close();

}
