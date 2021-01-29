#ifndef DR_PMTSD_h
#define DR_PMTSD_h

#include "DR_PMTHit.hh"
#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class DR_PMTSD : public G4VSensitiveDetector
{
public:

  DR_PMTSD( G4String name );
  ~DR_PMTSD();


  void Initialize( G4HCofThisEvent* HCE );
  G4bool ProcessHits( G4Step* aStep, G4TouchableHistory* ROhist );
  // A version of processHits that keeps aStep constant
  G4bool ProcessHits_constStep( const G4Step*       aStep,
                                G4TouchableHistory* ROhist );

  void EndOfEvent( G4HCofThisEvent* HCE );

  void clear();
  void DrawAll();
  void PrintAll();

private:

  DR_PMTHitsCollection* fPMTHitsCollection;
  G4int HCID;


};

#endif
