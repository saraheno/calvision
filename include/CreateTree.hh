#ifndef CreateTree_H
#define CreateTree_H 1

#include <iostream>
#include <vector>
#include "TString.h"
#include <map>

#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"

class CreateTree
{
private:
  TTree *ftree;
  TString fname;

public:
  CreateTree(TString name);
  ~CreateTree();

  TTree *GetTree() const { return ftree; };
  TString GetName() const { return fname; };
  void AddEnergyDeposit(int index, float deposit);
  void AddScintillationPhoton(int index);
  void AddCerenkovPhoton(int index);
  int Fill();
  bool Write(TFile *);
  void Clear();

  static CreateTree *Instance() { return fInstance; };
  static CreateTree *fInstance;

  int Event;

  std::vector<float> *inputMomentum;        // Px Py Pz E
  std::vector<float> *inputInitialPosition; // x, y, z
  std::vector<float> *primaryMomE1; // Px Py Pz E
  std::vector<float> *primaryPosE1; // x, y, z

  float depositedEnergyTotal;
  float depositedIonEnergyTotal;
  float depositedElecEnergyTotal;
  float Edep_ECAL;
  float IonEdep_ECAL;
  float ElecEdep_ECAL;
  float proton_EIon;
  float Ninelastic;
  float NGenPhoton
  ;

  TH2F *h_EIon_beta; 
  std::vector<int> *particle_escape;
  std::vector<float> *KineticEnergy_escape;
  std::vector<float> *positionx_escape;
  std::vector<float> *positiony_escape;
  std::vector<float> *positionz_escape;

};


#endif
