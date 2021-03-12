#include "CopyTreeNoWeight.cc"
void CopyTrees(const char* filename)
{
  //gROOT->ProcessLine(".L CopyTreeNoWeight.cc");
  CopyTreeNoWeight(filename, "ChopOut");
  CopyTreeNoWeight(filename, "IntObjIn");
  CopyTreeNoWeight(filename, "DetInfo");
  std::cout << "All Trees Copied." << std::endl;
}
