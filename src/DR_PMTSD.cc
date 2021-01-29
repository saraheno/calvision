
#include "DR_PMTHit.hh"
#include "DR_PMTSD.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "CreateTree.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"

DR_PMTSD::DR_PMTSD(G4String name)
    : G4VSensitiveDetector(name),
      fPMTHitsCollection(nullptr), HCID(-1)
{
  collectionName.insert("PMTHitsCollection");
}

DR_PMTSD::~DR_PMTSD() {}

void DR_PMTSD::Initialize(G4HCofThisEvent *HCE)
{
  fPMTHitsCollection = new DR_PMTHitsCollection(GetName(), collectionName[0]);
  if (HCID < 0)
  {
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(fPMTHitsCollection);
  }
  HCE->AddHitsCollection(HCID, fPMTHitsCollection);
}

G4bool
DR_PMTSD::ProcessHits(G4Step *,
                      G4TouchableHistory *)
{
  return false;
}

G4bool
DR_PMTSD::ProcessHits_constStep(const G4Step *, G4TouchableHistory *)
{
  return true;
}

void DR_PMTSD::EndOfEvent(G4HCofThisEvent *)
{
  G4int nHits = fPMTHitsCollection->entries();
  if (verboseLevel >= 1)
  {
    G4cout << "[DR_] PMT collection: " << nHits << " hits" << G4endl;
    if (verboseLevel >= 2)
    {
      fPMTHitsCollection->PrintAllHits();
    }
  }
  fPMTHitsCollection->PrintAllHits();
}

void DR_PMTSD::clear() {}

void DR_PMTSD::DrawAll() {}

void DR_PMTSD::PrintAll() {}
