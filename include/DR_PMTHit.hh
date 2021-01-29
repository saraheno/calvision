#ifndef DR_PMTHit_h
#define DR_PMTHit_h

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"

class G4VTouchable;

// --------------------------------------------------
// DR_PMTHit Class
// --------------------------------------------------

class DR_PMTHit : public G4VHit
{
public:

  DR_PMTHit();
  DR_PMTHit(G4int);
  ~DR_PMTHit();
  DR_PMTHit( const DR_PMTHit & right );

  const DR_PMTHit& operator=( const DR_PMTHit& right );
  G4int              operator==( const DR_PMTHit& right ) const;

  inline void* operator new( size_t );
  inline void  operator delete( void* aHit );

  void Draw();
  void Print();

  void
  SetCellID( G4int cellid ){fCellID = cellid;}
  void
  SetEnergy( G4double energy ){fEnergy = energy;}
  void
  SetPhotonCount( G4int photonCount ){fPhotonCount = photonCount;}
  void
  AddEnergy( G4double energy ){fEnergy += energy;}
  void
  IncPhotonCount(){fPhotonCount++;}
  void
  SetBounceCount( G4int x ){ fBounceCount = x ; }


  G4int
  GetCellID() const { return fCellID; }
  G4double
  GetEnergy() const { return fEnergy; }
  G4int
  GetPhotonCount() const { return fPhotonCount;}

  void
  SetTime( const G4double t ){fTime = t;};
  G4double
  GetTime() const {return fTime;};

  void
  SetLength( const double x ){ fLength = x ; }
  double
  GetLength() const { return fLength; }

  G4int
  GetBounceCount() const {return fBounceCount; }

private:
  G4int fCellID;
  G4double fEnergy;   // Total photon energy deposited in PMT
  G4int fPhotonCount;   // Total number of photons detected by PMT
  G4double fTime;
  G4double fLength; // Track length of photon hitting the detector
  G4int  fBounceCount;
};

// --------------------------------------------------
// Type Definitions
// --------------------------------------------------
typedef G4THitsCollection<DR_PMTHit> DR_PMTHitsCollection;
extern G4Allocator<DR_PMTHit> DR_PMTHitAllocator;

// --------------------------------------------------
// Operator Overloads
// --------------------------------------------------

inline void*
DR_PMTHit::operator new( size_t )
{
  void* aHit;
  aHit = (void*)DR_PMTHitAllocator.MallocSingle();
  return aHit;
}

inline void
DR_PMTHit::operator delete( void* aHit )
{
  DR_PMTHitAllocator.FreeSingle( (DR_PMTHit*)aHit );
}

#endif
