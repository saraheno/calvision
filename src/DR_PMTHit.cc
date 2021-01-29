
#include "DR_PMTHit.hh"
#include "G4VisManager.hh"

G4Allocator<DR_PMTHit> DR_PMTHitAllocator;

G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

DR_PMTHit::DR_PMTHit()
: G4VHit()
{
  fCellID      =0.;
  fEnergy      = 0.;
  fPhotonCount = 0;
  fTime        = 0.;
  fLength      = 0.;
  fBounceCount = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DR_PMTHit::DR_PMTHit(G4int cellID)
: G4VHit()
{
  fCellID      =cellID;
  fEnergy      = 0.;
  fPhotonCount = 0;
  fTime        = 0.;
  fLength      = 0.;
  fBounceCount = 0;
  }

DR_PMTHit::~DR_PMTHit(){}

DR_PMTHit::DR_PMTHit( const DR_PMTHit & right )
  : G4VHit()
{
  *this = right;
}

const DR_PMTHit&
DR_PMTHit::operator=( const DR_PMTHit & right )
{
  fCellID      = right.fCellID;
  fEnergy      = right.fEnergy;
  fPhotonCount = right.fPhotonCount;
  fTime        = right.fTime;
  fLength      = right.fLength;
  fBounceCount = right.fBounceCount;
  return *this;
}

G4int
DR_PMTHit::operator==( const DR_PMTHit& right ) const
{
  return fCellID == right.fCellID           &&
         fEnergy == right.fEnergy           &&
         fPhotonCount == right.fPhotonCount &&
         fTime == right.fTime               &&
         fBounceCount == right.fBounceCount  &&
         fLength == right.fLength;
}

void
DR_PMTHit::Draw()
{
}

void
DR_PMTHit::Print(){}
