#include "geometry/FASER2DetectorConstruction.hh"
#include "geometry/GeometricalParameters.hh"
#include "DetectorConstructionMaterial.hh"

#include "G4AssemblyVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"

FASER2DetectorConstruction::FASER2DetectorConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();

  // choose magnet option
  GeometricalParameters::magnetOption opt = GeometricalParameters::Get()->GetFASER2MagnetOption(); 

  fMagnetWindowX = GeometricalParameters::Get()->GetFASER2MagnetWindowX();
  fMagnetWindowY = GeometricalParameters::Get()->GetFASER2MagnetWindowY();
  fMagnetYokeThicknessX = GeometricalParameters::Get()->GetFASER2MagnetYokeThickX();
  fMagnetYokeThicknessY = GeometricalParameters::Get()->GetFASER2MagnetYokeThickY();

  fMagnetLengthZ = GeometricalParameters::Get()->GetFASER2MagnetLengthZ();
  fMagnetInnerR = GeometricalParameters::Get()->GetFASER2MagnetInnerR();
  fMagnetOuterR = GeometricalParameters::Get()->GetFASER2MagnetOuterR();
  fNMagnets = (opt == GeometricalParameters::magnetOption::SAMURAI) ? 1 : GeometricalParameters::Get()->GetNFASER2Magnets();
  fMagnetGap = GeometricalParameters::Get()->GetFASER2MagnetGap();

  fDecayVolumeLength = GeometricalParameters::Get()->GetfFASER2DecayVolumeLength();
  
  fNTrackingStations = GeometricalParameters::Get()->GetNTrackingStations();
  fTrackingStationX = GeometricalParameters::Get()->GetFASER2TrackerX();
  fTrackingStationY = GeometricalParameters::Get()->GetFASER2TrackerY();
  fScinThickness = GeometricalParameters::Get()->GetScintillatorThickness();
  fTrackingStationGap = GeometricalParameters::Get()->GetTrackingStationGap();
  
  fUpstreamTrackingStationGap = fTrackingStationGap;
  fDownstreamTrackingStationGap = 4*m;
  fEMCaloThickness = 1*m;
  fHadCaloThickness = 2*m;
  fIronWallThickness = 3*m;

  G4double magnetLength = fNMagnets*fMagnetLengthZ + fNMagnets*fMagnetGap;
  G4double caloLength = fTrackingStationGap+fEMCaloThickness+fHadCaloThickness+fIronWallThickness;
  G4double muonLength = fScinThickness+fTrackingStationGap+0.5*m;

  G4double totThickness = fNTrackingStations*fScinThickness + (fNTrackingStations-1)*fTrackingStationGap;
  
  G4double totLengthZ =  fDecayVolumeLength + fUpstreamTrackingStationGap + fDownstreamTrackingStationGap + 2*totThickness + magnetLength + caloLength + muonLength;
  
  G4double totLengthX = fMagnetWindowX+2*fMagnetYokeThicknessX;
  G4double totLengthY = fMagnetWindowY+2*fMagnetYokeThicknessY;

  GeometricalParameters::Get()->SetMagnetTotalSizeZ(fMagnetLengthZ);
  GeometricalParameters::Get()->SetTrackingStationTotalSizeZ(totThickness);
  GeometricalParameters::Get()->SetFASER2TotalSizeZ(totLengthZ);
  G4RotationMatrix *noRot = new G4RotationMatrix();
  
  //* Create the FASER2 volume - all components go in here
  auto containerSolid = new G4Box("FASER2Solid", totLengthX/2., totLengthY/2., totLengthZ/2.);
  fFASER2Assembly = new G4LogicalVolume(containerSolid, fMaterials->Material("Air"), "FASER2Logical");

  //* ------------ Place components in order that they appear ------------ *// 
  //* placing decay volume
  G4VSolid* DV_box = new G4Box("DV_box", GeometricalParameters::Get()->GetFASER2MagnetWindowX()/2, GeometricalParameters::Get()->GetFASER2MagnetWindowY()/2, fDecayVolumeLength/2);
  G4LogicalVolume* DV_log  = new G4LogicalVolume(DV_box, fMaterials->Material("Air"), "FASER2DecayVolLogical");
  
  G4double component_pos = -totLengthZ/2 + fDecayVolumeLength/2; //* We'll increment this variable as we move through the detector to place things
  G4ThreeVector DV_loc = G4ThreeVector(0,0, component_pos);
  std::cout << "Placing FASER2 Decay Volume at: " << DV_loc << std::endl;
  new G4PVPlacement(noRot, DV_loc, DV_log, "FASER2DecayVolPhysical", fFASER2Assembly, false, 0, true);
  
  G4VisAttributes* F2_logVisAtt = new G4VisAttributes(G4Colour::Red());
  F2_logVisAtt->SetForceWireframe(true);
  F2_logVisAtt->SetForceSolid(false);
  F2_logVisAtt->SetVisibility(true);
  fFASER2Assembly->SetVisAttributes(F2_logVisAtt);
  
  G4VisAttributes* DV_logVisAtt = new G4VisAttributes(G4Colour(0.8,0.8,0.8,0.3));
  DV_logVisAtt->SetForceWireframe(true);
  DV_logVisAtt->SetForceSolid(true);
  DV_log->SetVisAttributes(DV_logVisAtt);
  
  //* Place upstream tracking stations
  component_pos += fDecayVolumeLength/2;
  for (int i= 0; i<fNUpstreamTrackers; ++i) { 

    std::string name = "F2UpstreamTrackerLayer_" + std::to_string(i+1);
    auto trkLayerSolid = new G4Box(name+"Box", fTrackingStationX/2, fTrackingStationY/2., fScinThickness/2.);
    auto trkLayerLogical = new G4LogicalVolume(trkLayerSolid, fMaterials->Material("Polystyrene"), name+"Logical");
    
    G4VisAttributes* stationVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
    stationVis->SetVisibility(true);
    trkLayerLogical->SetVisAttributes(stationVis);

    auto trkLayerPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,component_pos), trkLayerLogical, name+"Phys", fFASER2Assembly, false, 0);
    fTrackingStationsLogical.push_back(trkLayerLogical);
    component_pos += fScinThickness + fTrackingStationGap;
  }

  //* Place magnet(s)
  if( opt == GeometricalParameters::magnetOption::SAMURAI ){
    
    G4ThreeVector magCenter(0.,0.,component_pos+fMagnetLengthZ/2);
    G4cout << "Building SAMURAI spectrometer magnet" << G4endl;
    BuildSAMURAIDesign(); //sets logical volumes
    
    // placing magnet + yoke
    new G4PVPlacement(noRot, magCenter, fMagnetWindow, "FASER2MagnetWindowPhysical", fFASER2Assembly, false, 0, false);
    new G4PVPlacement(noRot, magCenter, fMagnetYoke, "FASER2MagnetYokePhysical", fFASER2Assembly, false, 0, false);
  }
  else if ( opt == GeometricalParameters::magnetOption::CrystalPulling ){
    
    G4cout << "Building CrystalPulling spectrometer magnet" << G4endl;
    BuildCrystalPullingDesign();
    
    component_pos += fMagnetLengthZ/2;
    for(int i=0; i<fNMagnets; i++)
    {  
        G4ThreeVector magPos = G4ThreeVector(0.,0.,component_pos);
        new G4PVPlacement(noRot, magPos, fMagnetWindow, "FASER2MagnetWindowPhysical", fFASER2Assembly, false, i, false);
        new G4PVPlacement(noRot, magPos, fMagnetYoke, "FASER2MagnetYokePhysical", fFASER2Assembly, false, i, false);
        if (i != fNMagnets-1){
          component_pos += fMagnetGap + fMagnetLengthZ;
        }
    }
  } 
  else {
    G4cout << "ERROR: unknown FASER2 spectrometer magnet option!" << G4endl;  
  }

  //* Place upstream tracking stations
  component_pos += fMagnetLengthZ + fDownstreamTrackingStationGap;
  for (int i= 0; i<fNDownstreamTrackers; ++i) { 

    std::string name = "F2DownstreamTrackerLayer_" + std::to_string(i+1);
    auto trkLayerSolid = new G4Box(name+"Box", fTrackingStationX/2, fTrackingStationY/2., fScinThickness/2.);
    auto trkLayerLogical = new G4LogicalVolume(trkLayerSolid, fMaterials->Material("Polystyrene"), name+"Logical");
    
    G4VisAttributes* stationVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
    stationVis->SetVisibility(true);
    trkLayerLogical->SetVisAttributes(stationVis);

    auto trkLayerPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,component_pos), trkLayerLogical, name+"Phys", fFASER2Assembly, false, 0);
    fTrackingStationsLogical.push_back(trkLayerLogical);
    component_pos += fScinThickness + fTrackingStationGap;
  }
  
  //* Place Calorimeters and iron wall
  auto ECalBox = new G4Box("ECalBox", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fEMCaloThickness/2.);
  auto HCalBox = new G4Box("HCalBox", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fHadCaloThickness/2.);
  auto IronWallBox = new G4Box("HCalBox", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fIronWallThickness/2.);

  fEMCalLogical = new G4LogicalVolume(ECalBox, fMaterials->Material("Iron"), "FASER2ECAlLogical");
  fHadCalLogical = new G4LogicalVolume(HCalBox, fMaterials->Material("Iron"), "FASER2HCAlLogical");
  fIronWallLogical = new G4LogicalVolume(IronWallBox, fMaterials->Material("Iron"), "FASER2IronWallLogical");
  
  component_pos += fEMCaloThickness/2;
  new G4PVPlacement(noRot, G4ThreeVector(0,0,component_pos), fEMCalLogical, "FASER2ECalPhysical", fFASER2Assembly, false, 0, false);
  component_pos += fEMCaloThickness/2 + fHadCaloThickness/2;
  new G4PVPlacement(noRot, G4ThreeVector(0,0,component_pos), fHadCalLogical, "FASER2HCalPhysical", fFASER2Assembly, false, 0, false);
  component_pos += fHadCaloThickness/2 + fIronWallThickness/2;
  new G4PVPlacement(noRot, G4ThreeVector(0,0,component_pos), fIronWallLogical, "FASERIronWallPhysical", fFASER2Assembly, false, 0, false);
  component_pos += fIronWallThickness/2;

  G4VisAttributes* ECalVis = new G4VisAttributes(G4Colour::Red());
  ECalVis->SetVisibility(true);
  // ECalVis->SetForceSolid(true);
  ECalVis->SetForceWireframe(true);
  fEMCalLogical->SetVisAttributes(ECalVis);
  
  G4VisAttributes* HCalVis = new G4VisAttributes(G4Colour::Brown());
  HCalVis->SetVisibility(true);
  // HCalVis->SetForceSolid(true);
  HCalVis->SetForceWireframe(true);
  fHadCalLogical->SetVisAttributes(HCalVis);

  G4VisAttributes* IronWallVis = new G4VisAttributes(G4Colour::Grey());
  IronWallVis->SetVisibility(true);
  // IronWallVis->SetForceSolid(true);
  IronWallVis->SetForceWireframe(true);
  fIronWallLogical->SetVisAttributes(IronWallVis);

  //* Add muon detector
  component_pos += 0.5*m;
  auto trkLayerSolid = new G4Box("muonTrkLayerBox", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fScinThickness/2.);
  auto trkLayerLogical = new G4LogicalVolume(trkLayerSolid, fMaterials->Material("Polystyrene"), "muonTrkLayerLogical");
  
  G4VisAttributes* stationVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  stationVis->SetVisibility(true);
  trkLayerLogical->SetVisAttributes(stationVis);

  auto trkLayerPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,component_pos), trkLayerLogical, "muonTrkLayerPhys", fFASER2Assembly, false, 0);
  fTrackingStationsLogical.push_back(trkLayerLogical);
  component_pos += fScinThickness + fTrackingStationGap;

  //* visibility
  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  fMagnetWindow->SetVisAttributes(nullVis);

  G4VisAttributes* magnetVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  magnetVis->SetVisibility(true);
  magnetVis->SetForceSolid(true);
  fMagnetYoke->SetVisAttributes(magnetVis);
}

FASER2DetectorConstruction::~FASER2DetectorConstruction()
{ 
  delete fFASER2Assembly;
  delete fMagnetWindow;
  delete fMagnetYoke;
}

void FASER2DetectorConstruction::BuildSAMURAIDesign()
{
  auto magnetYokeBlock = new G4Box("MagnetYokeBlock", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fMagnetLengthZ/2.);
  auto magnetWindowSolid = new G4Box("MagnetYokeWindow", fMagnetWindowX/2., fMagnetWindowY/2., fMagnetLengthZ/2.);
  auto magnetYokeSolid = new G4SubtractionSolid("MagnetYoke",
				magnetYokeBlock, // block - window = hollow block
				magnetWindowSolid, 
				0, // no rotation
				G4ThreeVector(0,0,0)); //no translation
  
  fMagnetYoke = new G4LogicalVolume(magnetYokeSolid, fMaterials->Material("Iron"), "FASER2MagnetYokeLogical");
  fMagnetWindow = new G4LogicalVolume(magnetWindowSolid, fMaterials->Material("Air"), "FASER2MagnetWindowLogical"); 
}

void FASER2DetectorConstruction::BuildCrystalPullingDesign()
{
  auto magnetWindowSolid = new G4Tubs("MagnetWindow",0.,fMagnetInnerR,fMagnetLengthZ/2.,0.,CLHEP::twopi);
  auto magnetYokeSolid = new G4Tubs("MagnetYoke",fMagnetInnerR,fMagnetOuterR,fMagnetLengthZ/2.,0.,CLHEP::twopi);

  fMagnetYoke = new G4LogicalVolume(magnetYokeSolid, fMaterials->Material("Iron"), "FASER2MagnetYokeLogical");
  fMagnetWindow = new G4LogicalVolume(magnetWindowSolid, fMaterials->Material("Air"), "FASER2MagnetWindowLogical");
}

void FASER2DetectorConstruction::BuildTrackingStation()
{
  // layer volume: same size, but first has horizontal bars, second one has vertical bars
  auto trkLayerSolid = new G4Box("trkLayerBox", fTrackingStationX/2, fTrackingStationY/2., fScinThickness/2.);
  auto trkLayerLogical = new G4LogicalVolume(trkLayerSolid, fMaterials->Material("Polystyrene"), "trkLayerLogical");
  
  G4VisAttributes* stationVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  stationVis->SetVisibility(true);
  trkLayerLogical->SetVisAttributes(stationVis);

  fTrackingStation = new G4AssemblyVolume();
  G4RotationMatrix rot(0, 0, 0);
  G4ThreeVector pos(0, 0, -fScinThickness/2.);
  pos.setZ(fScinThickness/2.);
  auto trkLayerPhysical = new G4PVPlacement(0, pos, trkLayerLogical, "TrkPhys", fFASER2Assembly, false, 0);
  fTrackingStation->AddPlacedVolume(trkLayerLogical,pos,&rot);
  fTrackingStationsLogical.push_back(trkLayerLogical);
}
