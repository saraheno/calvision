#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"

using namespace CLHEP;

static const unsigned flatentries = 2;
static const double minenergy = 1.0 * eV;
static const double maxenergy = 8.0 * eV;

G4OpticalSurface *
MakeS_TyvekCrystal()
{
  const unsigned num = flatentries;
  double Ephoton[num] = {minenergy, maxenergy};
  double Reflectivity[num] = {0.979, 0.979};
  //////////////////////////////////
  // Realistic Crystal-Tyvek surface
  //////////////////////////////////
  G4OpticalSurface *surface = new G4OpticalSurface("TyvekOpSurface");
  surface->SetType(dielectric_LUT);
  surface->SetModel(LUT);
  surface->SetFinish(polishedtyvekair);

  G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();

  table->AddProperty("REFLECTIVITY", Ephoton, Reflectivity, num);

  surface->SetMaterialPropertiesTable(table);

  return surface;
}

G4OpticalSurface *
MakeS_ESR()
{
  const unsigned num = flatentries;
  double Ephoton[num] = {minenergy, maxenergy};
  double Reflectivity[num] = {0.985, 0.985};
  // source: https://www.osti.gov/servlets/purl/1184400
  //////////////////////////////////
  // ESR surface
  //////////////////////////////////
  G4OpticalSurface *surface = new G4OpticalSurface("ESROpSurface");
  surface->SetType(dielectric_LUT);
  surface->SetModel(LUT);
  surface->SetFinish(polishedvm2000air);

  G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();

  table->AddProperty("REFLECTIVITY", Ephoton, Reflectivity, num);

  surface->SetMaterialPropertiesTable(table);

  return surface;
}

G4OpticalSurface *
MakeS_IdealTyvekCrystal()
{
  //////////////////////////////////
  // Ideal Crystal-Tyvek surface
  //////////////////////////////////
  G4OpticalSurface *surface = new G4OpticalSurface("IdealTyvekOpSurface");
  surface->SetType(dielectric_LUT);
  surface->SetModel(LUT);
  surface->SetFinish(polishedtyvekair);

  return surface;
}

G4OpticalSurface *
MakeS_Polished()
{

  static const unsigned nentries = flatentries;
  static double phoE[nentries] = {minenergy, maxenergy};
  static double specularlobe[nentries] = {1.0, 1.0};

  //////////////////////////////////
  // Realistic polished surface
  //////////////////////////////////
  G4OpticalSurface *surface = new G4OpticalSurface("PolishedOpSurface");
  surface->SetType(dielectric_dielectric);
  surface->SetModel(unified);
  surface->SetFinish(ground);
  // necessary even for polished surfaces to enable UNIFIED code
  surface->SetSigmaAlpha(1.3 * degree); // Janecek2010 (1.3 * degree)

  G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();

  table->AddProperty(
      "SPECULARLOBECONSTANT", phoE, specularlobe, nentries);
  surface->SetMaterialPropertiesTable(table);

  return surface;
}

G4OpticalSurface *
MakeS_IdealPolished()
{
  //////////////////////////////////
  // Ideal polished surface
  //////////////////////////////////
  G4OpticalSurface *surface = new G4OpticalSurface("IdealOpSurface");
  surface->SetType(dielectric_dielectric);
  surface->SetModel(glisur);
  surface->SetFinish(polished);

  return surface;
}

G4OpticalSurface *
MakeS_Mirror()
{

  const unsigned nentries = flatentries;
  double phoE[nentries] = {minenergy, maxenergy};
  double reflectivity[nentries] = {0.9, 0.9};
  //////////////////////////////////
  // Mirror surface
  //////////////////////////////////
  G4OpticalSurface *surface = new G4OpticalSurface("MirrorOpSurface");
  surface->SetType(dielectric_metal);
  surface->SetFinish(polished);
  surface->SetModel(unified);

  G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();
  table->AddProperty("REFLECTIVITY", phoE, reflectivity, nentries);
  surface->SetMaterialPropertiesTable(table);

  return surface;
}

G4OpticalSurface *
MakeS_IdealMirror()
{
  const unsigned nentries = flatentries;
  double phoE[nentries] = {minenergy, maxenergy};
  double reflectivity[nentries] = {1.0, 1.0};
  //////////////////////////////////
  // Ideal mirror surface
  //////////////////////////////////
  G4OpticalSurface *surface = new G4OpticalSurface("MirrorOpSurface");
  surface->SetType(dielectric_metal);
  surface->SetFinish(polished);
  surface->SetModel(unified);

  G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();
  table->AddProperty("REFLECTIVITY", phoE, reflectivity, nentries);
  surface->SetMaterialPropertiesTable(table);

  return surface;
}

G4OpticalSurface *
MakeS_IdealWhiteSurface()
{
  //////////////////////////////////
  // Ideal mirror surface
  //////////////////////////////////
  G4OpticalSurface *surface = new G4OpticalSurface("WhiteOpSurface");
  surface->SetType(dielectric_metal);
  surface->SetFinish(ground);
  surface->SetModel(unified);

  double phoE[flatentries] = {minenergy, maxenergy};
  double reflectivity[flatentries] = {0.5, 0.5};

  G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();
  table->AddProperty("REFLECTIVITY", phoE, reflectivity, flatentries);
  surface->SetMaterialPropertiesTable(table);

  return surface;
}

G4OpticalSurface *
MakeS_Absorbing()
{
  const unsigned nentries = flatentries;
  double phoE[nentries] = {minenergy, maxenergy};
  double reflectivity[nentries] = {0.0, 0.0};
  //////////////////////////////////
  // Absorbing surface
  //////////////////////////////////
  G4OpticalSurface *surface = new G4OpticalSurface("AbsorbingOpSurface");
  surface->SetType(dielectric_dielectric);
  surface->SetFinish(groundfrontpainted);
  surface->SetModel(unified);

  G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();
  table->AddProperty("REFLECTIVITY", phoE, reflectivity, nentries);
  surface->SetMaterialPropertiesTable(table);

  return surface;
}

G4OpticalSurface *
MakeS_wrap()
{
  //////////////////////////////////
  // wrap surface
  //////////////////////////////////
  G4OpticalSurface *scintWrap = new G4OpticalSurface("ScintWrap");
  scintWrap->SetType(dielectric_metal);
  scintWrap->SetFinish(ground);
  scintWrap->SetModel(unified);
  double pp[2] = {0.1 * eV, 6 * eV};
  double reflectivity[2] = {1, 1};
  G4MaterialPropertiesTable *scintWrapProperty = new G4MaterialPropertiesTable();
  scintWrapProperty->AddProperty("REFLECTIVITY", pp, reflectivity, 2);
  scintWrap->SetMaterialPropertiesTable(scintWrapProperty);
  return scintWrap;
}

G4OpticalSurface *
MakeS_PCBSurface()
{
  // PCB is a flat gray surface for now
  double phoE2[flatentries] = {minenergy, maxenergy};
  double reflectivity[flatentries] = {0.5, 0.5};

  G4OpticalSurface *surface = new G4OpticalSurface("PCB_Surface");
  surface->SetType(dielectric_metal);
  surface->SetFinish(ground);
  surface->SetModel(unified);

  G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();

  table->AddProperty("REFLECTIVITY", phoE2, reflectivity, flatentries);
  surface->SetMaterialPropertiesTable(table);
  return surface;
}
