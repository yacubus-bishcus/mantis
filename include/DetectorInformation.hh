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

#ifndef DetectorInformation_h
#define DetectorInformation_h 1

class DetectorInformation
{
  static DetectorInformation *instance;

  DetectorInformation();
public:
  static DetectorInformation *Instance()
  {
    if(!instance)
    {
      instance = new DetectorInformation;
    }
    return instance;
  }

  // Set Data Functions here
  void setEndChop(G4double z_pos){EndChop = z_pos;}
  G4double getEndChop()const{return EndChop;}
  void setEndIntObj(G4double val){EndIntObj = val;}
  G4double getEndIntObj()const{return EndIntObj;}

  G4double EndChop, EndIntObj;
  ~DetectorInformation();
};

#endif
