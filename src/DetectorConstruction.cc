//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes, nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: DetectorConstruction.cc, v 1.18 2010-10-23 19:27:38 gum Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

#include "DetectorConstruction.hh"
#include "DR_PMTSD.hh"
#include "SurfaceProperty.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4ExplicitEuler.hh"
#include "G4ChordFinder.hh"
#include "G4EqMagElectricField.hh"
#include "G4PropagatorInField.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SubtractionSolid.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "G4MaterialPropertiesTable.hh"

#include "G4UserLimits.hh"

#include <G4TransportationManager.hh>
#include <G4MagneticField.hh>
#include <G4UniformMagField.hh>
#include <G4FieldManager.hh>
#include "CreateTree.hh"
#include <algorithm>
#include <string>
#include <sstream>
#include "G4MultiUnion.hh"

using namespace CLHEP;

DetectorConstruction::DetectorConstruction(const string &configFileName)
{
  //---------------------------------------
  //-----Parameters from config file ------
  //---------------------------------------

  ConfigFile config(configFileName);
  config.readInto(checkOverlaps, "checkOverlaps");
  config.readInto(world_material, "world_material");
  config.readInto(pointingAngle, "pointingAngle");
  config.readInto(ecal_material, "ecal_material");
  config.readInto(ecal_length, "ecal_length");
  config.readInto(ecal_face, "ecal_face");

  B_field_intensity = config.read<double>("B_field_intensity") * tesla;

  //-----------------
  // world size
  expHall_x = 101. * cm;
  expHall_y = 101. * cm;
  expHall_z = 301. * cm;
  B_field_IsInitialized = false;

  //-----------------
  initializeMaterials();
  initializeSurface();
}

//---- ---- ---- ---- ---- ---- ---- ---- ----  ---- ---- ---- ---- ---- ----

DetectorConstruction::~DetectorConstruction()
{
  delete stepLimit;
}

//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  G4cout << ">>>>>> DetectorConstruction::Construct ()::begin <<<<<<" << G4endl;

  //------------------------------------
  //------------- Geometry -------------
  //------------------------------------

  // The experimental Hall
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  G4VSolid *worldS = new G4Box("worldS", 0.5 * expHall_x, 0.5 * expHall_y, 0.5 * expHall_z);
  G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, WoMaterial, "worldLV", 0, 0, 0);
  G4VPhysicalVolume *worldPV = new G4PVPlacement(0, G4ThreeVector(), worldLV, "worldPV", 0, false, 0, checkOverlaps);

  double alveola_thickness = 1 * mm;

  // The Calorimeter
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  G4Box *ECalS = new G4Box("ECalS", 0.5 * ecal_face, 0.5 * ecal_face, 0.5 * ecal_length);
  G4LogicalVolume *ECalL = new G4LogicalVolume(ECalS, EcalMaterial, "ECalL", 0, 0, 0);
  //-----------------
  // ECal surface
  //G4LogicalSkinSurface *ECalSurface = new G4LogicalSkinSurface("ECalSurface", ECalL, fIdealPolishedSurface);

  // ECal physical placement
  const int NECAL_CRYST = 1;
  G4VPhysicalVolume *ECalP[NECAL_CRYST];

  char name[60];
  G4double x_pos[NECAL_CRYST];
  G4double y_pos[NECAL_CRYST];
  int nArrayECAL = (int)sqrt(NECAL_CRYST);
  int iCrystal;
  for (int iX = 0; iX < nArrayECAL; iX++)
  {
    for (int iY = 0; iY < nArrayECAL; iY++)
    {

      G4RotationMatrix *piRotEcal = new G4RotationMatrix;
      piRotEcal->rotateX(pointingAngle * deg);

      iCrystal = nArrayECAL * iX + iY;
      x_pos[iCrystal] = (iX - nArrayECAL / 2) * (ecal_face + alveola_thickness);
      y_pos[iCrystal] = (iY - nArrayECAL / 2) * (ecal_face / cos(pointingAngle * deg) + alveola_thickness);
      if (nArrayECAL / 2 != 0)
      {
        x_pos[iCrystal] -= alveola_thickness;
        y_pos[iCrystal] -= alveola_thickness;
      }
      cout << " x_pos [" << iCrystal << "] = " << x_pos[iCrystal] << " :: y_pos[" << iCrystal << "] = " << y_pos[iCrystal] << " :: angle = [" << pointingAngle * iX << ", " << pointingAngle * iY << "] " << endl;
      sprintf(name, "ECalP_%d", iCrystal);
      ECalP[iCrystal] = new G4PVPlacement(piRotEcal, G4ThreeVector(x_pos[iCrystal], y_pos[iCrystal], 0), ECalL, name, worldLV, false, 0, checkOverlaps);
    }
  }

  //-----------------------------------------------------
  //------------- sensitive detector  -------------------
  //-----------------------------------------------------
  auto sdman = G4SDManager::GetSDMpointer();
  auto fDRDetSD = new DR_PMTSD("/DR_Det");
  sdman->AddNewDetector(fDRDetSD);
  ECalL->SetSensitiveDetector(fDRDetSD);

  //-----------------------------------------------------
  //------------- Visualization attributes --------------
  //-----------------------------------------------------
  G4Colour white(1.00, 1.00, 1.00);        // white
  G4Colour gray(0.50, 0.50, 0.50);         // gray
  G4Colour black(0.00, 0.00, 0.00);        // black
  G4Colour red(1.00, 0.00, 0.00);          // red
  G4Colour green(0.00, 1.00, 0.00);        // green
  G4Colour blue(0.00, 0.00, 1.00);         // blue
  G4Colour lightblue(0.678, 0.847, 0.902); // light blue
  G4Colour cyan(0.00, 1.00, 1.00);         // cyan
  G4Colour air(0.90, 0.94, 1.00);          // air
  G4Colour magenta(1.00, 0.00, 1.00);      // magenta
  G4Colour yellow(1.00, 1.00, 0.00);       // yellow
  G4Colour brass(0.80, 0.60, 0.40);        // brass
  G4Colour brown(0.70, 0.40, 0.10);        // brown

  G4VisAttributes *VisAttWorld = new G4VisAttributes(white);
  VisAttWorld->SetVisibility(true);
  VisAttWorld->SetForceWireframe(true);
  worldLV->SetVisAttributes(VisAttWorld);

  G4VisAttributes *VisFiber = new G4VisAttributes(lightblue);
  VisFiber->SetVisibility(true);
  VisFiber->SetForceWireframe(false);
  ECalL->SetVisAttributes(VisFiber);

  if (B_field_intensity > 0.1 * tesla)
    ConstructField();

  G4cout << ">>>>>> DetectorConstruction::Construct ()::end <<< " << G4endl;
  return worldPV;
}

void DetectorConstruction::initializeMaterials()
{
  //-----------------
  // define materials

  WoMaterial = NULL;
  if (world_material == 0)
    WoMaterial = MyMaterials::Vacuum();
  else if (world_material == 1)
    WoMaterial = MyMaterials::Air();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid fibre world material specifier " << world_material << G4endl;
    exit(-1);
  }
  G4cout << "Wo. material: " << WoMaterial << G4endl;

  EcalMaterial = NULL;
  if (ecal_material == 1)
    EcalMaterial = MyMaterials::Quartz();
  else if (ecal_material == 2)
    EcalMaterial = MyMaterials::SiO2();
  else if (ecal_material == 3)
    EcalMaterial = MyMaterials::SiO2_Ce();
  else if (ecal_material == 4)
    EcalMaterial = MyMaterials::LuAG_Ce();
  else if (ecal_material == 5)
    EcalMaterial = MyMaterials::YAG_Ce();
  else if (ecal_material == 6)
    EcalMaterial = MyMaterials::LSO();
  else if (ecal_material == 7)
    EcalMaterial = MyMaterials::LYSO();
  else if (ecal_material == 8)
    EcalMaterial = MyMaterials::LuAG_undoped();
  else if (ecal_material == 9)
    EcalMaterial = MyMaterials::GAGG_Ce();
  else if (ecal_material == 10)
    EcalMaterial = MyMaterials::LuAG_Pr();
  else if (ecal_material == 11)
    EcalMaterial = MyMaterials::PbF2();
  else if (ecal_material == 12)
    EcalMaterial = MyMaterials::PlasticBC408();
  else if (ecal_material == 13)
    EcalMaterial = MyMaterials::PlasticBC418();
  else if (ecal_material == 14)
    EcalMaterial = MyMaterials::PWO();
  else if (ecal_material == 15)
    EcalMaterial = MyMaterials::Acrylic();
  else if (ecal_material == 16)
    EcalMaterial = MyMaterials::copper();
  else if (ecal_material == 17)
    EcalMaterial = MyMaterials::BGO();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid fibre clad material specifier " << ecal_material << G4endl;
    exit(-1);
  }
  G4cout << "ECAL material: " << EcalMaterial << G4endl;
}

//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void DetectorConstruction::ConstructField()
{
  G4cout << ">>>>>> DetectorConstruction::ConstructField ()::begin <<<<<<" << G4endl;

  static G4TransportationManager *trMgr = G4TransportationManager::GetTransportationManager();

  // A field object is held by a field manager
  // Find the global Field Manager
  G4FieldManager *globalFieldMgr = trMgr->GetFieldManager();

  if (!B_field_IsInitialized)
  {
    // magnetic field parallel to the beam direction (w/ tilt)
    G4ThreeVector fieldVector(0.0522 * B_field_intensity, 0.0522 * B_field_intensity, 0.9973 * B_field_intensity);

    B_field = new G4UniformMagField(fieldVector);
    globalFieldMgr->SetDetectorField(B_field);
    globalFieldMgr->CreateChordFinder(B_field);
    globalFieldMgr->GetChordFinder()->SetDeltaChord(0.005 * mm);
    B_field_IsInitialized = true;
  }

  G4cout << ">>>>>> DetectorConstruction::ConstructField ()::end <<< " << G4endl;
  return;
}

void DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((stepLimit) && (maxStep > 0.))
    stepLimit->SetMaxAllowedStep(maxStep);
}

//-----------------------------------------------
//------------- Surface properties --------------
//-----------------------------------------------
void DetectorConstruction::initializeSurface()
{
  //-----------------
  // Surface properties are important for optical simulations
  fIdealPolishedSurface = MakeS_IdealPolished();
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
