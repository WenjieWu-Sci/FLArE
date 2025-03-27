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
  fMagnetWindowZ = GeometricalParameters::Get()->GetFASER2MagnetWindowZ();
  fMagnetYokeThicknessX = GeometricalParameters::Get()->GetFASER2MagnetYokeThickX();
  fMagnetYokeThicknessY = GeometricalParameters::Get()->GetFASER2MagnetYokeThickY();

  fMagnetInnerR = GeometricalParameters::Get()->GetFASER2MagnetInnerR();
  fMagnetOuterR = GeometricalParameters::Get()->GetFASER2MagnetOuterR();
  fNMagnets = (opt == GeometricalParameters::magnetOption::SAMURAI) ? 1 : GeometricalParameters::Get()->GetNFASER2Magnets();
  fMagnetGap = GeometricalParameters::Get()->GetFASER2MagnetGap();

  fDecayVolumeLength = GeometricalParameters::Get()->GetFASER2DecayVolumeLength();
  
  fNTrackingStations = GeometricalParameters::Get()->GetNTrackingStations();
  fTrackingStationX = GeometricalParameters::Get()->GetFASER2TrackerX();
  fTrackingStationY = GeometricalParameters::Get()->GetFASER2TrackerY();
  fScinThickness = GeometricalParameters::Get()->GetScintillatorThickness();
  fTrackingStationGap = GeometricalParameters::Get()->GetTrackingStationGap();
  
  //TODO Implement these in the DetectorConstructionMessenger
  fUpstreamTrackingStationGap = GeometricalParameters::Get()->GetFASER2UpstreamTrackingStationGap();
  fDownstreamTrackingStationGap = GeometricalParameters::Get()->GetFASER2DownstreamTrackingStationGap();

  fEMCaloThickness = GeometricalParameters::Get()->GetFASER2EMCaloThickness();
  fHadCaloThickness = GeometricalParameters::Get()->GetFASER2HadCaloThickness();
  fIronWallThickness = GeometricalParameters::Get()->GetFASER2IronWallThickness();

  fVetoLengthX = GeometricalParameters::Get()->GetFASER2VetoLengthX();
  fVetoLengthY = GeometricalParameters::Get()->GetFASER2VetoLengthY();
  fVetoShieldThickness = GeometricalParameters::Get()->GetFASER2VetoShieldThickness();

  //* Work out the maximum extent of the whole of the FASER2 detector so we can make the physical volume
  //* We want to avoid any overlaps as this can be problematic for GENIE when it reads the GDML file
  G4double hallZOffset = GeometricalParameters::Get()->GetHallHeadDistance();
  G4double FASER2ZOffset = GeometricalParameters::Get()->GetFASER2Position().z();
  G4double vetoLength = 2*fScinThickness + fVetoShieldThickness;
  G4double upstreamTrackerLength = fNTrackingStations*fScinThickness + (fNTrackingStations - 1)*fTrackingStationGap + fUpstreamTrackingStationGap;
  G4double magnetLength = fNMagnets*fMagnetWindowZ + (fNMagnets-1)*fMagnetGap;
  G4double downstreamTrackerLength = fNTrackingStations*fScinThickness + (fNTrackingStations - 1)*fTrackingStationGap + fDownstreamTrackingStationGap;
  G4double caloLength = fTrackingStationGap+fEMCaloThickness+fHadCaloThickness+fIronWallThickness;
  G4double muonLength = fScinThickness+fTrackingStationGap+0.5*m;
  
  G4double totLengthZ =  vetoLength + fDecayVolumeLength +  upstreamTrackerLength + magnetLength + downstreamTrackerLength + caloLength + muonLength;
  
  std::vector<G4double> lengths{fMagnetWindowX+2*fMagnetYokeThicknessX, fVetoLengthX, fMagnetOuterR};
  std::vector<G4double> heights{fMagnetWindowY+2*fMagnetYokeThicknessY, fVetoLengthY, fMagnetOuterR};
  
  auto totLengthX = std::max_element(lengths.begin(), lengths.end());
  auto totLengthY = std::max_element(heights.begin(), heights.end());

  // Set the total size of the FASER2 detector in the GeometricalParameters so that it can be accessed elsewhere
  GeometricalParameters::Get()->SetMagnetTotalSizeZ(magnetLength);
  GeometricalParameters::Get()->SetFASER2TotalSizeZ(totLengthZ);
  GeometricalParameters::Get()->SetTrackingStationTotalSizeZ(upstreamTrackerLength);
  GeometricalParameters::Get()->SetMagnetZPosition(hallZOffset + FASER2ZOffset - totLengthZ/2 + vetoLength + fDecayVolumeLength + upstreamTrackerLength + magnetLength/2);
  G4RotationMatrix *noRot = new G4RotationMatrix();
  
  //* Create the FASER2 volume - all components go in here
  auto containerSolid = new G4Box("FASER2Solid", *totLengthX/2., *totLengthY/2., totLengthZ/2.);
  fFASER2Assembly = new G4LogicalVolume(containerSolid, fMaterials->Material("Air"), "FASER2Logical");

  //* ------------ Place components in order that they appear ------------ *// 
  
  G4double componentPosition = -totLengthZ/2; //* We'll increment this variable as we move through the detector to place things
  
  //* Place veto stations
  componentPosition += fScinThickness/2;
  G4VSolid* vetoScinBox = new G4Box("vetoScinBox", fVetoLengthX/2, fVetoLengthY/2, fScinThickness/2);
  fVetoScinLogical = new G4LogicalVolume(vetoScinBox, fMaterials->Material("Polystyrene"), "VetoScinLogical");  
  new G4PVPlacement(noRot, G4ThreeVector(0,0, componentPosition), fVetoScinLogical, "Veto1ScinPhysical", fFASER2Assembly, false, 0, true);

  componentPosition += fScinThickness/2 + fVetoShieldThickness/2;
  G4VSolid* vetoShieldBox = new G4Box("vetoShieldBox", fVetoLengthX/2, fVetoLengthY/2, fVetoShieldThickness/2);
  fVetoShieldLogical = new G4LogicalVolume(vetoShieldBox, fMaterials->Material("Lead"), "VetoShieldLogical");
  new G4PVPlacement(noRot, G4ThreeVector(0,0, componentPosition), fVetoShieldLogical, "VetoShieldPhysical", fFASER2Assembly, false, 0, true);

  componentPosition += fVetoShieldThickness/2 + fScinThickness/2;
  new G4PVPlacement(noRot, G4ThreeVector(0,0, componentPosition), fVetoScinLogical, "Veto2ScinPhysical", fFASER2Assembly, false, 0, true);

  //* Place decay volume
  componentPosition += fScinThickness/2 + fDecayVolumeLength/2; //* We'll increment this variable as we move through the detector to place things
  G4VSolid* decayVolBox = new G4Box("decayVolBox", GeometricalParameters::Get()->GetFASER2MagnetWindowX()/2, GeometricalParameters::Get()->GetFASER2MagnetWindowY()/2, fDecayVolumeLength/2);
  fDecayVolumeLogical  = new G4LogicalVolume(decayVolBox, fMaterials->Material("Air"), "FASER2DecayVolLogical");
  
  new G4PVPlacement(noRot, G4ThreeVector(0,0, componentPosition), fDecayVolumeLogical, "FASER2DecayVolPhysical", fFASER2Assembly, false, 0, true);
  
  //* Place upstream tracking stations
  componentPosition += fDecayVolumeLength/2 + fScinThickness/2.;
  for (int i= 0; i<fNTrackingStations; ++i) { 

    std::string name = "F2UpstreamTrackerLayer_" + std::to_string(i+1);
    auto trkLayerSolid = new G4Box(name+"Box", fTrackingStationX/2, fTrackingStationY/2., fScinThickness/2.);
    auto trkLayerLogical = new G4LogicalVolume(trkLayerSolid, fMaterials->Material("Polystyrene"), name+"Logical");
    auto trkLayerPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,componentPosition), trkLayerLogical, name+"Phys", fFASER2Assembly, false, 0);
    fTrackingStationsLogical.push_back(trkLayerLogical);
    
    if (i != fNTrackingStations-1)
    {
      componentPosition += fScinThickness + fTrackingStationGap;
    }
  }

  //* Place magnet(s)
  componentPosition += fScinThickness/2 + fUpstreamTrackingStationGap;
  if( opt == GeometricalParameters::magnetOption::SAMURAI ){
    
    componentPosition += fMagnetWindowZ/2;
    G4cout << "Building SAMURAI spectrometer magnet" << G4endl;
    BuildSAMURAIDesign(); //sets logical volumes
    
    // placing magnet + yoke
    new G4PVPlacement(noRot, G4ThreeVector(0,0,componentPosition), fMagnetWindow, "FASER2MagnetWindowPhysical", fFASER2Assembly, false, 0, false);
    new G4PVPlacement(noRot, G4ThreeVector(0,0,componentPosition), fMagnetYoke, "FASER2MagnetYokePhysical", fFASER2Assembly, false, 0, false);
  }
  else if ( opt == GeometricalParameters::magnetOption::CrystalPulling ){
    
    G4cout << "Building CrystalPulling spectrometer magnet" << G4endl;
    BuildCrystalPullingDesign();
    
    componentPosition += fMagnetWindowZ/2;
    for(int i=0; i<fNMagnets; i++)
    {  
        G4ThreeVector magPos = G4ThreeVector(0.,0.,componentPosition);
        new G4PVPlacement(noRot, magPos, fMagnetWindow, "FASER2MagnetWindowPhysical", fFASER2Assembly, false, i, false);
        new G4PVPlacement(noRot, magPos, fMagnetYoke, "FASER2MagnetYokePhysical", fFASER2Assembly, false, i, false);
        if (i != fNMagnets-1){
          componentPosition += fMagnetGap + fMagnetWindowZ;
        }
    }
  } 
  else {
    G4cout << "ERROR: unknown FASER2 spectrometer magnet option!" << G4endl;  
  }

  //* Place upstream tracking stations
  componentPosition += fMagnetWindowZ/2 + fDownstreamTrackingStationGap;
  for (int i= 0; i<fNTrackingStations; ++i) { 

    std::string name = "F2DownstreamTrackerLayer_" + std::to_string(i+1);
    auto trkLayerSolid = new G4Box(name+"Box", fTrackingStationX/2, fTrackingStationY/2., fScinThickness/2.);
    auto trkLayerLogical = new G4LogicalVolume(trkLayerSolid, fMaterials->Material("Polystyrene"), name+"Logical");
    auto trkLayerPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,componentPosition), trkLayerLogical, name+"Phys", fFASER2Assembly, false, 0);
    fTrackingStationsLogical.push_back(trkLayerLogical);

    componentPosition += fScinThickness + fTrackingStationGap;
  }
  
  //* Place 'calorimeters' and iron wall
  //TODO: Calorimeter volumes are just dummy placeholder boxes - detailed simulation studies required!
  auto ECalBox = new G4Box("ECalBox", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fEMCaloThickness/2.);
  auto HCalBox = new G4Box("HCalBox", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fHadCaloThickness/2.);
  auto IronWallBox = new G4Box("HCalBox", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fIronWallThickness/2.);

  fEMCalLogical = new G4LogicalVolume(ECalBox, fMaterials->Material("Copper"), "FASER2ECAlLogical");
  fHadCalLogical = new G4LogicalVolume(HCalBox, fMaterials->Material("Iron"), "FASER2HCAlLogical");
  fIronWallLogical = new G4LogicalVolume(IronWallBox, fMaterials->Material("Iron"), "FASER2IronWallLogical");
  
  componentPosition += fEMCaloThickness/2;
  new G4PVPlacement(noRot, G4ThreeVector(0,0,componentPosition), fEMCalLogical, "FASER2ECalPhysical", fFASER2Assembly, false, 0, false);
  componentPosition += fEMCaloThickness/2 + fHadCaloThickness/2;
  new G4PVPlacement(noRot, G4ThreeVector(0,0,componentPosition), fHadCalLogical, "FASER2HCalPhysical", fFASER2Assembly, false, 0, false);
  componentPosition += fHadCaloThickness/2 + fIronWallThickness/2;
  new G4PVPlacement(noRot, G4ThreeVector(0,0,componentPosition), fIronWallLogical, "FASERIronWallPhysical", fFASER2Assembly, false, 0, false);
  componentPosition += fIronWallThickness/2;

  //* Add muon detector
  componentPosition += fTrackingStationGap + fScinThickness/2;
  auto muonDetSolid = new G4Box("muonTrkLayerBox", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fScinThickness/2.);
  fMuonDetLogical = new G4LogicalVolume(muonDetSolid, fMaterials->Material("Polystyrene"), "muonTrkLayerLogical");
  auto muonDetPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,componentPosition), fMuonDetLogical, "muonTrkLayerPhys", fFASER2Assembly, false, 0);

  //* Set visualisation attributes for all components
  SetVisualisation();

}

FASER2DetectorConstruction::~FASER2DetectorConstruction()
{ 
  delete fFASER2Assembly;
  delete fMagnetWindow;
  delete fMagnetYoke;
}

void FASER2DetectorConstruction::BuildSAMURAIDesign()
{
  auto magnetYokeBlock = new G4Box("MagnetYokeBlock", fMagnetWindowX/2.+fMagnetYokeThicknessX, fMagnetWindowY/2.+fMagnetYokeThicknessY, fMagnetWindowZ/2.);
  auto magnetWindowSolid = new G4Box("MagnetYokeWindow", fMagnetWindowX/2., fMagnetWindowY/2., fMagnetWindowZ/2.);
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
  auto magnetWindowSolid = new G4Tubs("MagnetWindow",0.,fMagnetInnerR,fMagnetWindowZ/2.,0.,CLHEP::twopi);
  auto magnetYokeSolid = new G4Tubs("MagnetYoke",fMagnetInnerR,fMagnetOuterR,fMagnetWindowZ/2.,0.,CLHEP::twopi);

  fMagnetYoke = new G4LogicalVolume(magnetYokeSolid, fMaterials->Material("Iron"), "FASER2MagnetYokeLogical");
  fMagnetWindow = new G4LogicalVolume(magnetWindowSolid, fMaterials->Material("Air"), "FASER2MagnetWindowLogical");
}


void FASER2DetectorConstruction::SetVisualisation()
{
  //? Moved all the detector visualisation settings to this function - perhaps in future we could set these with a messenger class or config file?
  //* Detector bounding box
  G4VisAttributes* F2_logVisAtt = new G4VisAttributes(G4Colour::Grey());
  F2_logVisAtt->SetForceWireframe(true);
  F2_logVisAtt->SetForceSolid(false);
  F2_logVisAtt->SetVisibility(true);
  fFASER2Assembly->SetVisAttributes(F2_logVisAtt);

  //* Veto station
  G4VisAttributes* vetoScin_logVisAtt = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  vetoScin_logVisAtt->SetForceWireframe(true);
  vetoScin_logVisAtt->SetForceSolid(false);
  vetoScin_logVisAtt->SetVisibility(true);
  fVetoScinLogical->SetVisAttributes(vetoScin_logVisAtt);

  G4VisAttributes* vetoShield_logVisAtt = new G4VisAttributes(G4Colour::Grey());
  vetoShield_logVisAtt->SetForceWireframe(true);
  vetoShield_logVisAtt->SetForceSolid(true);
  vetoShield_logVisAtt->SetVisibility(true);
  fVetoShieldLogical->SetVisAttributes(vetoShield_logVisAtt);

  //* Decay volume
  G4VisAttributes* DV_logVisAtt = new G4VisAttributes(G4Colour(0.8,0.8,0.8,0.3));
  DV_logVisAtt->SetForceWireframe(true);
  DV_logVisAtt->SetForceSolid(true);
  fDecayVolumeLogical->SetVisAttributes(DV_logVisAtt);

  //* Tracking stations
  for (auto trkLayerLogical : fTrackingStationsLogical)
  {
    G4VisAttributes* stationVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
    stationVis->SetVisibility(true);
    trkLayerLogical->SetVisAttributes(stationVis);
  }

  //* Magnets
  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  fMagnetWindow->SetVisAttributes(nullVis);

  G4VisAttributes* magnetVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  magnetVis->SetVisibility(true);
  magnetVis->SetForceSolid(true);
  fMagnetYoke->SetVisAttributes(magnetVis);

  //* 'Calorimeters' & iron wall
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

  //* Muon detector
  G4VisAttributes* stationVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  stationVis->SetVisibility(true);
  fMuonDetLogical->SetVisAttributes(stationVis);
} 