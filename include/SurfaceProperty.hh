#ifndef SURFACE_PROPERTY_HH
#define SURFACE_PROPERTY_HH

#include "G4OpticalSurface.hh"

extern G4OpticalSurface* MakeS_TyvekCrystal();
extern G4OpticalSurface* MakeS_ESR();
extern G4OpticalSurface* MakeS_IdealTyvekCrystal();
extern G4OpticalSurface* MakeS_IdealTyvek();
extern G4OpticalSurface* MakeS_Polished();
extern G4OpticalSurface* MakeS_IdealPolished();
extern G4OpticalSurface* MakeS_Mirror();
extern G4OpticalSurface* MakeS_IdealMirror();
extern G4OpticalSurface* MakeS_IdealWhiteSurface();
extern G4OpticalSurface* MakeS_Absorbing();
extern G4OpticalSurface* MakeS_wrap();
extern G4OpticalSurface* MakeS_PCBSurface();

#endif
