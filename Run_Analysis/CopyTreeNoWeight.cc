void CopyTreeNoWeight(const char* filename, const char* tObj)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cerr << "File not found." << std::endl;
    exit(100);
  }
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  TTree *oldTree;
  f->GetObject(tObj, oldTree);

  oldTree->SetBranchStatus("*",0);
  if(!string(tObj).compare("ChopOut") || !string(tObj).compare("DetInfo"))
  {
    for (auto activeBranchName : {"Energy"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("IntObjIn"))
  {
    for (auto activeBranchName : {"Energy","Angle"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else
  {
    std::cerr << "ERROR: Object Name not found." << std::endl;
    exit(1);
  }

  string outfilename = "converted_" + string(filename);

  // Create a new file + a clone of old tree header. Do not copy events
  TFile *newfile;
  if(gSystem->AccessPathName(outfilename.c_str()))
    newfile = new TFile(outfilename.c_str(), "recreate");
  else
    newfile = new TFile(outfilename.c_str(),"update");

  auto newtree = oldTree->CloneTree();

  newtree->Print();
  // write to new file
  newfile->Write();
  newfile->Close();
} // end of CopyTreeNoWeight
