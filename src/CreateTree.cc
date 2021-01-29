#include "CreateTree.hh"
#include <algorithm>

using namespace std;

CreateTree *CreateTree::fInstance = NULL;

CreateTree::CreateTree(TString name)
{
  if (fInstance)
  {
    return;
  }

  this->fInstance = this;
  this->fname = name;
  this->ftree = new TTree(name, name);

  this->GetTree()->Branch("Event", &this->Event, "Event/I");

  inputInitialPosition = new vector<float>(3, 0.);
  inputMomentum = new vector<float>(4, 0.);
  primaryPosE1 = new vector<float>(3, 0.);
  primaryMomE1 = new vector<float>(4, 0.);

  this->GetTree()->Branch("inputInitialPosition", "vector<float>", &inputInitialPosition);
  this->GetTree()->Branch("inputMomentum", "vector<float>", &inputMomentum);
  this->GetTree()->Branch("primaryPosE1", "vector<float>", &primaryPosE1);
  this->GetTree()->Branch("primaryMomE1", "vector<float>", &primaryMomE1);

  this->GetTree()->Branch("depositedEnergyTotal", &this->depositedEnergyTotal, "depositedEnergyTotal/F");
  this->GetTree()->Branch("depositedIonEnergyTotal", &this->depositedIonEnergyTotal, "depositedIonEnergyTotal/F");
  this->GetTree()->Branch("depositedElecEnergyTotal", &this->depositedElecEnergyTotal, "depositedElecEnergyTotal/F");

  this->GetTree()->Branch("Edep_ECAL", &this->Edep_ECAL, "Edep_ECAL/F");
  this->GetTree()->Branch("IonEdep_ECAL", &this->IonEdep_ECAL, "IonEdep_ECAL/F");
  this->GetTree()->Branch("ElecEdep_ECAL", &this->ElecEdep_ECAL, "ElecEdep_ECAL/F");

  this->GetTree()->Branch("proton_EIon", &this->proton_EIon, "proton_EIon/F");
  this->GetTree()->Branch("Ninelastic", &this->Ninelastic, "Ninelastic/F");
  this->GetTree()->Branch("NGenPhoton", &this->NGenPhoton, "NGenPhoton/F");


  particle_escape = new vector<int>;
  KineticEnergy_escape = new vector<float>;
  positionx_escape = new vector<float>;
  positiony_escape = new vector<float>;
  positionz_escape = new vector<float>;
  this->GetTree()->Branch("particle_escape", "vector<int>", &particle_escape);
  this->GetTree()->Branch("KineticEnergy_escape", "vector<float>", &KineticEnergy_escape);
  this->GetTree()->Branch("positionx_escape", "vector<float>", &positionx_escape);
  this->GetTree()->Branch("positiony_escape", "vector<float>", &positiony_escape);
  this->GetTree()->Branch("positionz_escape", "vector<float>", &positionz_escape);
  h_EIon_beta = new TH2F("h_EIon_beta","",10000,-5000,5000,100,-0.1,1.1);

  this->Clear();
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

CreateTree::~CreateTree()
{
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

int CreateTree::Fill()
{
  //  this->GetTree()->Write(NULL, TObject::kOverwrite ); 
  return this->GetTree()->Fill();
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

bool CreateTree::Write(TFile *outfile)
{
  outfile->cd();
  ftree->Write();
  h_EIon_beta->Write();
  return true;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void CreateTree::Clear()
{
  Event = 0;

  for (int i = 0; i < 3; ++i)
  {
    inputInitialPosition->at(i) = 0.;
    primaryPosE1->at(i) = 0.;
  }
  for (int i = 0; i < 4; ++i)
  {
    inputMomentum->at(i) = 0.;
    primaryMomE1->at(i) = 0.;
  }

  depositedEnergyTotal = 0.;
  depositedIonEnergyTotal = 0.;
  depositedElecEnergyTotal = 0.;
  Edep_ECAL = 0.;
  IonEdep_ECAL = 0.;
  ElecEdep_ECAL = 0.;
  proton_EIon = 0.;
  Ninelastic = 0.;
  NGenPhoton = 0.;

  particle_escape->clear();
  KineticEnergy_escape->clear();
  positionx_escape->clear();
  positiony_escape->clear();
  positionz_escape->clear();

}
