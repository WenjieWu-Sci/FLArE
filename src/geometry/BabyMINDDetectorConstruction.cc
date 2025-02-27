#include "geometry/BabyMINDDetectorConstruction.hh"
#include "geometry/GeometricalParameters.hh"
#include "DetectorConstructionMaterial.hh"

#include "G4AssemblyVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4ReplicatedSlice.hh"
#include "G4PVPlacement.hh"

BabyMINDDetectorConstruction::BabyMINDDetectorConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();

  G4cout << "Building BabyMIND Detector" << G4endl;
 
  // Reading geometrical parameters from storage
  fMagnetPlateThickness = GeometricalParameters::Get()->GetBabyMINDMagnetPlateThickness();
  fMagnetPlateSizeX = GeometricalParameters::Get()->GetBabyMINDMagnetPlateSizeX();
  fMagnetPlateSizeY = GeometricalParameters::Get()->GetBabyMINDMagnetPlateSizeY();
  fMagnetCentralPlateY = GeometricalParameters::Get()->GetBabyMINDMagnetCentralPlateY();
  fSlitSizeX = GeometricalParameters::Get()->GetBabyMINDSlitSizeX();
  fSlitSizeY = GeometricalParameters::Get()->GetBabyMINDSlitSizeY();
    
  fNVerticalBars = GeometricalParameters::Get()->GetBabyMINDNVerticalBars();
  fNHorizontalBars = GeometricalParameters::Get()->GetBabyMINDNHorizontalBars();
  fBarThickness = GeometricalParameters::Get()->GetBabyMINDBarThickness();
  fVerticalBarSizeX = GeometricalParameters::Get()->GetBabyMINDVerticalBarSizeX();
  fVerticalBarSizeY = GeometricalParameters::Get()->GetBabyMINDVerticalBarSizeY();
  fHorizontalBarSizeX = GeometricalParameters::Get()->GetBabyMINDHorizontalBarSizeX();
  fHorizontalBarSizeY = GeometricalParameters::Get()->GetBabyMINDHorizontalBarSizeY();
    
  fMagnetToScinSpacing = GeometricalParameters::Get()->GetBabyMINDMagnetToScinSpacing();
  fMagnetToMagnetSpacing = GeometricalParameters::Get()->GetBabyMINDMagnetToMagnetSpacing();
  fBlockToBlockSpacing = GeometricalParameters::Get()->GetBabyMINDBlockToBlockSpacing();
  fBlockPadding = GeometricalParameters::Get()->GetBabyMINDBlockPadding();
  fBlockSequence =GeometricalParameters::Get()->GetBabyMINDBlockSequence();

  BuildMagnetModule();
  BuildScintillatorModule(); 
 
  // let's find the total size of BabyMIND, this depends on the block sequence
  G4double BabyMINDTotalSizeZ = ComputeTotalSize();
  G4double BabyMINDTotalSizeX = (fMagnetPlateSizeX > fHorizontalBarSizeX) ? fMagnetPlateSizeX : fHorizontalBarSizeX;
  G4double BabyMINDTotalSizeY = (fMagnetPlateSizeY > fVerticalBarSizeY) ? fMagnetPlateSizeY : fVerticalBarSizeY;
  GeometricalParameters::Get()->SetBabyMINDTotalSizeZ(BabyMINDTotalSizeZ);

  // create a top-level logical volume container
  auto containerSolid = new G4Box("BabyMINDSolid", BabyMINDTotalSizeX/2., BabyMINDTotalSizeY/2., BabyMINDTotalSizeZ/2.);
  fBabyMINDAssembly = new G4LogicalVolume(containerSolid, fMaterials->Material("Air"), "BabyMINDLogical");

  // center of the assembly is...
  G4ThreeVector BabyMINDCenter(0.,0.,0.);
  G4RotationMatrix *noRot = new G4RotationMatrix();

  // starting position for placement
  G4ThreeVector prevPos(0.,0.,-BabyMINDTotalSizeZ/2.);
  char prev = 'i';
  int k = 0;

  for(auto c : fBlockSequence) {
    
    // shift accumulated since previous placement
    G4double shift = 0;
    G4ThreeVector currentPos = prevPos;

    switch(c) {

      case '|': // start/end of block padding, nothing to place
       
        // length is spacing from previous + full size
        shift = fBlockPadding;
        if(prev=='|') shift += fBlockToBlockSpacing;
        currentPos += G4ThreeVector(0.,0.,shift);
        break;
    
       case 'M': //magnet module (M)

        // shift to the middle of the module
        // spacing from previous + half its size
        if( prev == 'M' ) shift += fMagnetToMagnetSpacing;
        else if( prev == 'D' ) shift += fMagnetToScinSpacing;
        shift += fMagnetPlateThickness/2.;
        currentPos += G4ThreeVector(0.,0.,shift);
        // placing a magnet module
        new G4PVPlacement(noRot, currentPos, fMagnetPlate, "MagnetPlatePhysical", fBabyMINDAssembly, false, k, false);
        k++;
        // update pointer to the end of the module
        // shift by only remaining half size
        shift = fMagnetPlateThickness/2.;
        currentPos += G4ThreeVector(0.,0.,shift);
        break;
      
      case 'D': //detector module (D)
        
        // shift to the middle of the module
        // spacing from previous + half its size
        if( prev == 'M' ) shift += fMagnetToScinSpacing;
        else if( prev == 'D' ) shift += fMagnetToScinSpacing;
        shift += 2*fBarThickness;
        currentPos += G4ThreeVector(0.,0.,shift);
        // placing a detector module
        fDetectorModule->MakeImprint(fBabyMINDAssembly, currentPos, noRot, 0, false);
        // update pointer to the end of the module
        // shift by only remaining half size
        shift = 2*fBarThickness; 
        currentPos += G4ThreeVector(0.,0.,shift);
        break;
  
      default:
        G4cout << "ERROR: unrecognized char='" << c << "' in block sequence: '" << fBlockSequence << "'" << G4endl;
    }
    
    prevPos = currentPos;
    prev = c;

  }

  G4VisAttributes* steelVis = new G4VisAttributes(G4Colour(128./255, 128./255, 128./255, 0.8));
  steelVis->SetVisibility(true);
  fMagnetPlate->SetVisAttributes(steelVis);  
  
  G4VisAttributes* detectorVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  detectorVis->SetVisibility(true);
  fHorizontalBar->SetVisAttributes(detectorVis);  
  fVerticalBar->SetVisAttributes(detectorVis);  

  G4VisAttributes* spacersVis = new G4VisAttributes(G4Colour(170./255, 85./255, 0./255, 0.8));
  spacersVis->SetVisibility(true);
  fFirstVerLayerLogical->SetVisAttributes(spacersVis);  
  fFirstHorLayerLogical->SetVisAttributes(spacersVis);  
  fSecondVerLayerLogical->SetVisAttributes(spacersVis);  
  fSecondHorLayerLogical->SetVisAttributes(spacersVis);  
}

BabyMINDDetectorConstruction::~BabyMINDDetectorConstruction()
{ 
  delete fBabyMINDAssembly;
  delete fDetectorModule;
  delete fMagnetPlate;
  delete fVerticalBar;
  delete fHorizontalBar;
  delete fFirstVerLayerLogical;
  delete fFirstHorLayerLogical;
  delete fSecondVerLayerLogical;
  delete fSecondHorLayerLogical;
}

void BabyMINDDetectorConstruction::BuildMagnetModule()
{
  // Build the ARMCO steel plate by subtraction:
  // full steel plate, then subtracts 2 slits
  auto fullPlateSolid = new G4Box("steelPLateSolid", fMagnetPlateSizeX/2., fMagnetPlateSizeY/2., fMagnetPlateThickness/2.);
  auto slitSolid = new G4Box("slitSolid",fSlitSizeX/2.,fSlitSizeY/2.,fMagnetPlateThickness/2.);
  G4double shift = fMagnetCentralPlateY/2. + fSlitSizeY/2.;
  auto oneSlitPlateSolid = new G4SubtractionSolid("oneSlitPlateSolid",
				fullPlateSolid, 
				slitSolid, 
				0, // no rotation
				G4ThreeVector(0,shift,0)); //no translation
  auto twoSlitPlateSolid = new G4SubtractionSolid("magnetPlateSolid",
				oneSlitPlateSolid, 
				slitSolid, 
				0, // no rotation
				G4ThreeVector(0,-shift,0)); //no translation

  // for the future: build an assembly by adding the coils wrapping around the slits
  // for now: this is it, make it a logical volume!
  fMagnetPlate = new G4LogicalVolume(twoSlitPlateSolid, fMaterials->Material("ARMCO"), "MagnetPlateLogical");
}

void BabyMINDDetectorConstruction::BuildScintillatorModule()
{
  fDetectorModule = new G4AssemblyVolume();
  G4RotationMatrix* noRot = new G4RotationMatrix();

  // Each detector module has 4 layers (2+2)
  // The two halves need to be complementary to cover the gaps from the spacers
  // Strategy is the following:
  // - plastic mother volume for each layer to mimic spacers
  // - embed scintillator bars in mother value (can't use replicas...)
  // - adjust offsets/gaps to make things match
    
  // single bars (solids + logicals )
  auto verticalBarSolid = new G4Box("verticalBarSolid", fVerticalBarSizeX/2., fVerticalBarSizeY/2., fBarThickness/2.);
  auto horizontalBarSolid = new G4Box("horizontalBarSolid", fHorizontalBarSizeX/2., fHorizontalBarSizeY/2., fBarThickness/2.);
  fHorizontalBar = new G4LogicalVolume( horizontalBarSolid, fMaterials->Material("Polystyrene"), "BabyMINDHorBarLogical");
  fVerticalBar = new G4LogicalVolume( verticalBarSolid, fMaterials->Material("Polystyrene"), "BabyMINDVerBarLogical");
 
  // single layer (solid + logical)
  auto layerSolid = new G4Box("layerSolid",fHorizontalBarSizeX/2.,fVerticalBarSizeY/2.,fBarThickness/2.);
  fFirstVerLayerLogical = new G4LogicalVolume(layerSolid, fMaterials->Material("Polyethylene"), "firstVerLayerLogical");
  fFirstHorLayerLogical = new G4LogicalVolume(layerSolid, fMaterials->Material("Polyethylene"), "firstHorLayerLogical");
  fSecondVerLayerLogical = new G4LogicalVolume(layerSolid, fMaterials->Material("Polyethylene"), "secondVerLayerLogical");
  fSecondHorLayerLogical = new G4LogicalVolume(layerSolid, fMaterials->Material("Polyethylene"), "secondHorLayerLogical");

  // vertical layers
  G4int nGaps = fNVerticalBars/2;
  G4double gapWidth = (fHorizontalBarSizeX - (fNVerticalBars/2.)*fVerticalBarSizeX)/nGaps;

  for(int i=0; i<fNVerticalBars/2.; i++){

    // x position of the center of the vertical bars
    // w.r.t the mother volume reference frame
    G4double firstLayerCenter = -fHorizontalBarSizeX/2. + 0.5*fVerticalBarSizeX + i*(fVerticalBarSizeX + gapWidth);
    G4double secondLayerCenter = -fHorizontalBarSizeX/2. + gapWidth + 0.5*fVerticalBarSizeX + i*(fVerticalBarSizeX + gapWidth);
    G4ThreeVector firstLayer( firstLayerCenter, 0., 0.);
    G4ThreeVector secondLayer( secondLayerCenter, 0., 0.);
  
    new G4PVPlacement( noRot, // no rotation
		       firstLayer, //translation
                       fVerticalBar, // logical volume
                       "BabyMINDVerBarPhysical", // name
                       fFirstVerLayerLogical, // mother logical volume
                       false, // always
                       i, //copy number
                       false); // checkOverlap
    
    new G4PVPlacement( noRot, // no rotation
		       secondLayer, //translation
                       fVerticalBar, // logical volume
                       "BabyMINDVerBarPhysical", // name
                       fSecondVerLayerLogical, // mother logical volume
                       false, // always
                       i+fNVerticalBars/2., //copy number
                       false); // checkOverlap
  }
  
  // horizontal layers
  nGaps = fNHorizontalBars/2;
  gapWidth = (fVerticalBarSizeY - (fNHorizontalBars/2.)*fHorizontalBarSizeY)/nGaps;

  for(int i=0; i<fNHorizontalBars/2.; i++){

    // y position of the center of the horizontal bars
    // w.r.t the mother volume reference frame
    G4double firstLayerCenter = -fVerticalBarSizeY/2. + 0.5*fHorizontalBarSizeY + i*(fHorizontalBarSizeY + gapWidth);
    G4double secondLayerCenter = -fVerticalBarSizeY/2. + gapWidth + 0.5*fHorizontalBarSizeY + i*(fHorizontalBarSizeY + gapWidth);
    G4ThreeVector firstLayer( 0., firstLayerCenter, 0.);
    G4ThreeVector secondLayer( 0., secondLayerCenter, 0.);
  
    new G4PVPlacement( noRot, // no rotation
		       firstLayer, //translation
                       fHorizontalBar, // logical volume
                       "BabyMINDHorBarPhysical", // name
                       fFirstHorLayerLogical, // mother logical volume
                       false, // always
                       i, //copy number
                       false); // checkOverlap
    
    new G4PVPlacement( noRot, // no rotation
		       secondLayer, //translation
                       fHorizontalBar, // logical volume
                       "BabyMINDHorBarPhysical", // name
                       fSecondHorLayerLogical, // mother logical volume
                       false, // always
                       i+fNHorizontalBars/2., //copy number
                       false); // checkOverlap
  }
  
  // add the layers to the detector assembly
  // center of assembly is in the middle of the two halves
  G4ThreeVector pos_fV(0,0,-1.5*fBarThickness);
  G4ThreeVector pos_fH(0,0,-0.5*fBarThickness);
  G4ThreeVector pos_sH(0,0,0.5*fBarThickness);
  G4ThreeVector pos_sV(0,0,1.5*fBarThickness);

  fDetectorModule->AddPlacedVolume(fFirstVerLayerLogical,pos_fV,noRot);
  fDetectorModule->AddPlacedVolume(fFirstHorLayerLogical,pos_fH,noRot);
  fDetectorModule->AddPlacedVolume(fSecondHorLayerLogical,pos_sH,noRot);
  fDetectorModule->AddPlacedVolume(fSecondVerLayerLogical,pos_sV,noRot);
  
}

G4double BabyMINDDetectorConstruction::ComputeTotalSize()
{
  G4double BabyMINDTotalSizeZ = 0.;
  char prev = 'i';
  for(auto c : fBlockSequence) {
    switch(c) {
      case '|': // block padding
        BabyMINDTotalSizeZ += fBlockPadding;
        if(prev=='|') BabyMINDTotalSizeZ += fBlockToBlockSpacing;
        break;
      case 'M': //magnet module (M)
        BabyMINDTotalSizeZ += fMagnetPlateThickness;
        if( prev == 'M' ) BabyMINDTotalSizeZ += fMagnetToMagnetSpacing;
        else if( prev == 'D' ) BabyMINDTotalSizeZ += fMagnetToScinSpacing;
        break;
      case 'D': //detector module (D)
        BabyMINDTotalSizeZ += 4*fBarThickness;
        if( prev == 'M' ) BabyMINDTotalSizeZ += fMagnetToScinSpacing;
        else if( prev == 'D' ) BabyMINDTotalSizeZ += fMagnetToScinSpacing;
        break;
      default:
        G4cout << "ERROR: unrecognized char='" << c << "' in block sequence: '" << fBlockSequence << "'" << G4endl;
    }
    prev = c;
  }
  return BabyMINDTotalSizeZ;
}
