#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "DR_PMTSD.hh"
#include "DetectorConstruction.hh"
#include "TString.h"
#include "TRandom3.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"
#include <time.h>

#include "G4EventManager.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"

#include <iostream>
#include <fstream>
#include <vector>
#include "TTree.h"

using namespace std;
using namespace CLHEP;

//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

SteppingAction::SteppingAction(DetectorConstruction *detectorConstruction,
                               const G4int &scint, const G4int &cher) : fDetectorConstruction(detectorConstruction),
                                                                        propagateScintillation(scint),
                                                                        propagateCerenkov(cher)
{
  maxtracklength = 500000. * mm;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

SteppingAction::~SteppingAction()
{
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void SteppingAction::UserSteppingAction(const G4Step *theStep)
{

  G4Track *theTrack = theStep->GetTrack();
  G4ParticleDefinition *particleType = theTrack->GetDefinition();

  G4StepPoint *thePrePoint = theStep->GetPreStepPoint();
  G4StepPoint *thePostPoint = theStep->GetPostStepPoint();
  const G4ThreeVector &thePrePosition = thePrePoint->GetPosition();
  const G4ThreeVector &thePostPosition = thePostPoint->GetPosition();
  G4VPhysicalVolume *thePrePV = thePrePoint->GetPhysicalVolume();
  G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();
  G4String thePrePVName = "";
  if (thePrePV)
    thePrePVName = thePrePV->GetName();
  G4String thePostPVName = "";
  if (thePostPV)
    thePostPVName = thePostPV->GetName();
  //G4VSolid* thePreS = thePrePV->GetLogicalVolume()->GetSolid();
  //G4VSolid* thePostS = thePostPV->GetLogicalVolume()->GetSolid();

  G4int nStep = theTrack->GetCurrentStepNumber();
  G4int TrPDGid = theTrack->GetDefinition()->GetPDGEncoding();

  // get position
  //
  G4double global_x = thePrePosition.x() / mm;
  G4double global_y = thePrePosition.y() / mm;
  G4double global_z = thePrePosition.z() / mm;

  // get energy
  //
  G4double energy = theStep->GetTotalEnergyDeposit();
  G4double energyIon = energy - theStep->GetNonIonizingEnergyDeposit();
  G4double energyElec = 0;

  if (abs(TrPDGid) == 11 || abs(TrPDGid) == 22)
    energyElec = energyIon;

  // store total energy
  //
  CreateTree::Instance()->depositedEnergyTotal += energy / GeV;
  CreateTree::Instance()->depositedIonEnergyTotal += energyIon / GeV;
  CreateTree::Instance()->depositedElecEnergyTotal += energyElec / GeV;

  // get the beta and ion energy of each particle and store in 2D hist
  //
  CreateTree::Instance()->h_EIon_beta->Fill(TrPDGid, thePrePoint->GetBeta(), energyIon / GeV);

  // total ionizing energy of proton and number of Inelastic collisions
  //
  if (abs(TrPDGid) == 2212)
  {
    CreateTree::Instance()->proton_EIon += energyIon / GeV;
  }
  if (thePostPoint->GetProcessDefinedStep()->GetProcessName().contains("Inelast"))
  {
    CreateTree::Instance()->Ninelastic++;
  }

  // escaped particles
  //
  /*
  bool outworld = ((theStep->GetPostStepPoint())->GetStepStatus()) == fWorldBoundary;
  if (outworld)
  {
    CreateTree::Instance()->particle_escape->push_back(TrPDGid);
    CreateTree::Instance()->KineticEnergy_escape->push_back((theStep->GetPostStepPoint())->GetKineticEnergy() / GeV);
    CreateTree::Instance()->positionx_escape->push_back(thePostPosition.x() / mm);
    CreateTree::Instance()->positiony_escape->push_back(thePostPosition.y() / mm);
    CreateTree::Instance()->positionz_escape->push_back(thePostPosition.z() / mm);
  }
  */

  if (particleType == G4OpticalPhoton::OpticalPhotonDefinition())
  {
    // optic photons
    //
    G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
    float photWL = MyMaterials::fromEvToNm(theTrack->GetTotalEnergy() / eV);
    //only consider 300 to 1000nm
    if (photWL > 1000 || photWL < 300)
    {
      theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    }
    else
    {
      //only consider Cerenkov and Scintillation
      if ((processName == "Cerenkov") || (processName == "Scintillation"))
      {
        if (nStep == 1)
        {
          CreateTree::Instance()->NGenPhoton++; //number of gen photons
        }
        else if (!propagateCerenkov && (processName == "Cerenkov"))
        {
          theTrack->SetTrackStatus(fKillTrackAndSecondaries);
        }

        else if (!propagateScintillation && (processName == "Scintillation"))
        {
          theTrack->SetTrackStatus(fKillTrackAndSecondaries);
        }
      }
      else
      {
        theTrack->SetTrackStatus(fKillTrackAndSecondaries);
      }
    }
  }
  else
  {
    // non optical photon
    //
    if (thePrePVName.contains("world") && thePostPVName.contains("ECalP_"))
    {
      if (theTrack->GetParentID() == 0) // select only the primary particle
      {
        CreateTree::Instance()->primaryPosE1->at(0) = global_x;
        CreateTree::Instance()->primaryPosE1->at(1) = global_y;
        CreateTree::Instance()->primaryPosE1->at(2) = global_z;

        CreateTree::Instance()->primaryMomE1->at(0) = thePrePoint->GetMomentum().x() / GeV;
        CreateTree::Instance()->primaryMomE1->at(1) = thePrePoint->GetMomentum().y() / GeV;
        CreateTree::Instance()->primaryMomE1->at(2) = thePrePoint->GetMomentum().z() / GeV;
        CreateTree::Instance()->primaryMomE1->at(3) = thePrePoint->GetTotalEnergy() / GeV;
      }
    }
    // store total energy in Calorimeter
    //
    if (thePrePVName.contains("ECalP_"))
    {
      CreateTree::Instance()->Edep_ECAL += energy / GeV;
      CreateTree::Instance()->IonEdep_ECAL += energyIon / GeV;
      CreateTree::Instance()->ElecEdep_ECAL += energyElec / GeV;
    }
  }

  return;
}
