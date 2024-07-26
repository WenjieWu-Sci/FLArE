#include "geometry/SamplingCalorimeterConstruction.hh"
#include "geometry/GeometricalParameters.hh"
#include "DetectorConstructionMaterial.hh"

#include "G4AssemblyVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh" 
#include "G4PVReplica.hh"

SamplingCalorimeterConstruction::SamplingCalorimeterConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();

  G4cout << "Building a Sampling Calorimeter" << G4endl;

  // detector geometry configuration
  thicknessAbsorber = GeometricalParameters::Get()->GetSamplingCaloThicknessAbsorb();
  thicknessCaloX    = GeometricalParameters::Get()->GetSamplingCaloThicknessCaloX();;
  thicknessCaloY    = GeometricalParameters::Get()->GetSamplingCaloThicknessCaloY();
  fSamplingCaloSizeX   = GeometricalParameters::Get()->GetSamplingCaloSizeX();
  fSamplingCaloSizeY   = GeometricalParameters::Get()->GetSamplingCaloSizeY();
  fSamplingCaloNLayers = GeometricalParameters::Get()->GetSamplingCaloNLayers();

  BuildDetector();

  fSamplingCalorimeterAssembly = new G4AssemblyVolume();
  G4RotationMatrix* noRot = new G4RotationMatrix();
  G4ThreeVector detCenter(0., 0., 0.);
  fSamplingCalorimeterAssembly->AddPlacedAssembly(SamplingCaloAssembly, detCenter, noRot);
}

void SamplingCalorimeterConstruction::BuildDetector()
{
  G4double thicknessOneLayer = thicknessAbsorber + thicknessCaloX + thicknessCaloY;
  fSamplingCaloSizeZ = fSamplingCaloNLayers*thicknessOneLayer;
  // override fSamplingCaloSizeZ to this updated calculation
  GeometricalParameters::Get()->SetSamplingCaloSizeZ(fSamplingCaloSizeZ);

  // Absorber
  auto AbsorbLayerSolid
    = new G4Box("AbsorbLayerSamplingCalo", fSamplingCaloSizeX/2, fSamplingCaloSizeY/2, thicknessAbsorber/2);
  AbsorbLayerLogical
    = new G4LogicalVolume(AbsorbLayerSolid, fMaterials->Material("Tungsten"), "AbsorbLayerLogical");

  // x-plane
  auto CaloXLayerSolid
    = new G4Box("CaloXLayer", fSamplingCaloSizeX/2, fSamplingCaloSizeY/2, thicknessCaloX/2);
  auto CaloXLayerLogical
    = new G4LogicalVolume(CaloXLayerSolid, fMaterials->Material("Polystyrene"), "CaloXLayerLogical");

  auto CaloXCellSolid
    = new G4Box("CaloXCell", thicknessCaloX/2, fSamplingCaloSizeY/2, thicknessCaloX/2);
  CaloXCellLogical
    = new G4LogicalVolume(CaloXCellSolid, fMaterials->Material("Polystyrene"), "CaloXCellLogical");
  new G4PVReplica("CaloXCellPhysical", CaloXCellLogical,
                  CaloXLayerLogical, kXAxis, fSamplingCaloSizeX/thicknessCaloX, thicknessCaloX);

  // y-plane
  auto CaloYLayerSolid
    = new G4Box("CaloYLayer", fSamplingCaloSizeX/2, fSamplingCaloSizeY/2, thicknessCaloY/2);
  auto CaloYLayerLogical
    = new G4LogicalVolume(CaloYLayerSolid, fMaterials->Material("Polystyrene"), "CaloYLayerLogical");

  auto CaloYCellSolid
    = new G4Box("CaloYCell", fSamplingCaloSizeX/2, thicknessCaloY/2, thicknessCaloY/2);
  CaloYCellLogical
    = new G4LogicalVolume(CaloYCellSolid, fMaterials->Material("Polystyrene"), "CaloYCellLogical");
  new G4PVReplica("CaloYCellPhysical", CaloYCellLogical,
                  CaloYLayerLogical, kYAxis, fSamplingCaloSizeY/thicknessCaloY, thicknessCaloY);

  SamplingCaloAssembly = new G4AssemblyVolume();
  G4RotationMatrix Ra(0, 0, 0);
  for (int i= 0; i< fSamplingCaloNLayers; ++i) {
    G4RotationMatrix Rm(0, 0, 0);
    G4ThreeVector TAbsorb(0, 0, (i+0.5)*thicknessAbsorber+i*thicknessCaloX+i*thicknessCaloY-fSamplingCaloSizeZ/2.);
    G4ThreeVector TCaloX(0, 0, (i+1)*thicknessAbsorber+(i+0.5)*thicknessCaloX+i*thicknessCaloY-fSamplingCaloSizeZ/2.);
    G4ThreeVector TCaloY(0, 0, (i+1)*thicknessAbsorber+(i+1)*thicknessCaloX+(i+0.5)*thicknessCaloY-fSamplingCaloSizeZ/2.);
    SamplingCaloAssembly->AddPlacedVolume(AbsorbLayerLogical, TAbsorb, &Ra);
    SamplingCaloAssembly->AddPlacedVolume(CaloXLayerLogical, TCaloX, &Ra);
    SamplingCaloAssembly->AddPlacedVolume(CaloYLayerLogical, TCaloY, &Ra);
  }

  // visualization
  G4VisAttributes* CaloVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  CaloVis->SetVisibility(true);
  CaloXLayerLogical->SetVisAttributes(CaloVis);
  CaloYLayerLogical->SetVisAttributes(CaloVis);

  G4VisAttributes* absorVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  absorVis->SetVisibility(true);
  AbsorbLayerLogical->SetVisAttributes(absorVis);

  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  CaloXCellLogical->SetVisAttributes(CaloVis);
  CaloYCellLogical->SetVisAttributes(CaloVis);
}
