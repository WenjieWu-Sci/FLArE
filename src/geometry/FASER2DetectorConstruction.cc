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
  if( opt == GeometricalParameters::magnetOption::SAMURAI ){
    
    G4cout << "Building SAMURAI spectrometer magnet" << G4endl;
    fMagnetWindowX = GeometricalParameters::Get()->GetFASER2MagnetWindowX();
    fMagnetWindowY = GeometricalParameters::Get()->GetFASER2MagnetWindowY();
    fMagnetWindowZ = GeometricalParameters::Get()->GetFASER2MagnetWindowZ();
    fMagnetYokeThicknessX = GeometricalParameters::Get()->GetFASER2MagnetYokeThickX();
    fMagnetYokeThicknessY = GeometricalParameters::Get()->GetFASER2MagnetYokeThickY();
   
    fNTrackingStations = GeometricalParameters::Get()->GetNTrackingStations();
    fTrackingStationX = fMagnetWindowX + 0.5*m; //match magnet size + bending plane
    fTrackingStationY = fMagnetWindowY; // match magnet size
    fNScinBarsY = GeometricalParameters::Get()->GetNScintillatorBarsY();
    fNScinBarsX = GeometricalParameters::Get()->GetNScintillatorBarsX();
    fScinThickness = GeometricalParameters::Get()->GetScintillatorThickness();
    fTrackingStationGap = GeometricalParameters::Get()->GetTrackingStationGap();
    G4double gapToMagnet = fTrackingStationGap;
    G4double stationThickness = 2*fScinThickness;
    G4double totThickness = fNTrackingStations*stationThickness + (fNTrackingStations-1)*fTrackingStationGap;
    G4double totLengthZ = fMagnetWindowZ+2*gapToMagnet+2*totThickness+20*m;
    G4double totLengthX = fMagnetWindowX+2*fMagnetYokeThicknessX;
    G4double totLengthY = fMagnetWindowY+2*fMagnetYokeThicknessY;

    GeometricalParameters::Get()->SetMagnetTotalSizeZ(fMagnetWindowZ);
    GeometricalParameters::Get()->SetTrackingStationTotalSizeZ(totThickness);
    GeometricalParameters::Get()->SetFASER2TotalSizeZ(totLengthZ);

    BuildSAMURAIDesign(); //sets logical volumes
    BuildTrackingStation(); //sets assembly volume
    
    auto containerSolid = new G4Box("FASER2Solid", totLengthX/2., totLengthY/2., totLengthZ/2.);
    fFASER2Assembly = new G4LogicalVolume(containerSolid, fMaterials->Material("Air"), "FASER2Logical");

    // center of the assembly is the center of the magnet window
    G4ThreeVector magCenter(0.,0.,0.);
    G4RotationMatrix *noRot = new G4RotationMatrix();

    // placing magnet + yoke
    new G4PVPlacement(noRot, magCenter, fMagnetWindow, "FASER2MagnetWindowPhysical", fFASER2Assembly, false, 0, false);
    new G4PVPlacement(noRot, magCenter, fMagnetYoke, "FASER2MagnetYokePhysical", fFASER2Assembly, false, 0, false);

    // middle position of pre-magnet and post-magnet tracking stations
    G4ThreeVector offset(0, 0, fMagnetWindowZ/2.+ gapToMagnet + totThickness/2.);
    G4ThreeVector preStationsCenter = magCenter - offset;
    G4ThreeVector postStationsCenter = magCenter + offset;

    // placing decay volume
    G4VSolid* DV_box = new G4Box("DV_box", GeometricalParameters::Get()->GetFASER2MagnetWindowX()/2, GeometricalParameters::Get()->GetFASER2MagnetWindowY()/2, 10./2.*m);
    G4LogicalVolume* DV_log  = new G4LogicalVolume(DV_box, fMaterials->Material("Air"), "DV_log");
    
    G4ThreeVector T1(0, 0, -totThickness/2.+0.5*stationThickness+(fNTrackingStations-1)*(fTrackingStationGap+stationThickness));
    G4ThreeVector DV_loc = preStationsCenter-G4ThreeVector(0,0, 10/2*m)-T1;
    std::cout << "Placing FASER2 Decay Volume at: " << DV_loc << std::endl;
    new G4PVPlacement(noRot, DV_loc, DV_log, "FASER2DecayVolPhysical", fFASER2Assembly, false, 0, true);
    
    G4VisAttributes* DV_logVisAtt = new G4VisAttributes(G4Colour(0.8,0.8,0.8,0.3));
    DV_logVisAtt->SetForceWireframe(true);
    DV_logVisAtt->SetForceSolid(true);
    DV_log->SetVisAttributes(DV_logVisAtt);
  
    // placing tracking stations (before/after magnet)
    std::cout << "Placing " << fNTrackingStations * 2 << " tracking layers" << std::endl;
    for (int i= 0; i<fNTrackingStations; ++i) { 
      G4ThreeVector T(0, 0, -totThickness/2.+0.5*stationThickness+i*(fTrackingStationGap+stationThickness));
      G4ThreeVector Tp = postStationsCenter + T;
      G4ThreeVector Tm = preStationsCenter + T;

      std::cout << "Placing tracker at " << Tp << std::endl;
      std::cout << "Placing tracker at " << Tm << std::endl;

      fTrackingStation->MakeImprint(fFASER2Assembly, Tm, noRot, 0, false); //place before magnet
      fTrackingStation->MakeImprint(fFASER2Assembly, Tp, noRot, 0, false); //place after magnet

      auto trkLayerSolid1 = new G4Box("trkLayerBox", fTrackingStationX/2, fTrackingStationY/2., fScinThickness/2.);
      auto trkLayerLogical1 = new G4LogicalVolume(trkLayerSolid1, fMaterials->Material("Polystyrene"), "trkLayerLogical");

      fTrackingStationsLogical.push_back(trkLayerLogical1);

      auto trkLayerSolid2 = new G4Box("trkLayerBox", fTrackingStationX/2, fTrackingStationY/2., fScinThickness/2.);
      auto trkLayerLogical2 = new G4LogicalVolume(trkLayerSolid2, fMaterials->Material("Polystyrene"), "trkLayerLogical");
      
      fTrackingStationsLogical.push_back(trkLayerLogical2);

    }

  } else if ( opt == GeometricalParameters::magnetOption::CrystalPulling ){
 
    G4cout << "Building CrystalPulling spectrometer magnet" << G4endl;
    fMagnetLengthZ = GeometricalParameters::Get()->GetFASER2MagnetLengthZ();
    fMagnetInnerR = GeometricalParameters::Get()->GetFASER2MagnetInnerR();
    fMagnetOuterR = GeometricalParameters::Get()->GetFASER2MagnetOuterR();
    fNMagnets = GeometricalParameters::Get()->GetNFASER2Magnets();
    fMagnetGap = GeometricalParameters::Get()->GetFASER2MagnetGap();
    
    fNTrackingStations = GeometricalParameters::Get()->GetNTrackingStations();
    fTrackingStationX = 2*fMagnetInnerR + 0.5*m ; //match magnet size + bending plane (for now, FIXME?)
    fTrackingStationY = 2*fMagnetInnerR; // match magnet size
    fNScinBarsY = GeometricalParameters::Get()->GetNScintillatorBarsY();
    fNScinBarsX = GeometricalParameters::Get()->GetNScintillatorBarsX();
    fScinThickness = GeometricalParameters::Get()->GetScintillatorThickness();
    fTrackingStationGap = GeometricalParameters::Get()->GetTrackingStationGap();
    G4double stationThickness = 2*fScinThickness;

    // length of a tracking station set (tracking stations + gaps between them)
    G4double totThickness = fNTrackingStations*stationThickness + (fNTrackingStations-1)*fTrackingStationGap; 
    GeometricalParameters::Get()->SetTrackingStationTotalSizeZ(totThickness);
    
    // length of magnet assembly (includes all magnets and tracking stations sets between them + gaps);
    // it differs from the total length for the two external tracking stations sets
    G4double magnetsLengthZ = fNMagnets*fMagnetLengthZ + (fNMagnets-1)*(2*fMagnetGap+totThickness);
    GeometricalParameters::Get()->SetMagnetTotalSizeZ(magnetsLengthZ);
     
    // total length (all magnets, all tracking stations sets, all gaps)
    G4double totalLengthZ = 2*(totThickness+fMagnetGap) + magnetsLengthZ;
    G4double totalLengthY = 2*fMagnetOuterR;
    G4double totalLengthX = 2*fMagnetOuterR;
    GeometricalParameters::Get()->SetFASER2TotalSizeZ(totalLengthZ);    

    BuildCrystalPullingDesign(); //sets logical volumes
    BuildTrackingStation();
    
    auto containerSolid = new G4Box("FASER2Solid", totalLengthX/2., totalLengthY/2., totalLengthZ/2.);
    fFASER2Assembly = new G4LogicalVolume(containerSolid, fMaterials->Material("Air"), "FASER2Logical");

    // center of the assembly is the center of middle magnet window
    // if fNMagnets is even, it falls in the gap between the two middle magnets
    G4ThreeVector magCenter(0.,0.,0.);
    G4RotationMatrix *noRot = new G4RotationMatrix();
   
    // each magnet comes with a set of N tracking stations just before it
    // the gap between each magnet is then given by magnetSpacing
    G4double magnetSpacing = 2*fMagnetGap + totThickness;
    GeometricalParameters::Get()->SetFASER2MagnetSpacing(magnetSpacing);    

    for(int i=0; i<fNMagnets; i++){
      G4double offset = (i-0.5*(fNMagnets-1))*(magnetSpacing+fMagnetLengthZ);
      G4ThreeVector magPos = magCenter + G4ThreeVector(0.,0.,offset);
      new G4PVPlacement(noRot, magPos, fMagnetWindow, "FASER2MagnetWindowPhysical", fFASER2Assembly, false, i, false);
      new G4PVPlacement(noRot, magPos, fMagnetYoke, "FASER2MagnetYokePhysical", fFASER2Assembly, false, i, false);

      // placing tracking stations (before each magnet)
      G4double trackingStationOffset = -fMagnetGap-0.5*fMagnetLengthZ;
      for (int j= 0; j<fNTrackingStations; ++j) { 
        G4ThreeVector T(0., 0., trackingStationOffset-j*fTrackingStationGap-0.5*(i+1)*stationThickness);
        G4ThreeVector Tpos = magPos + T; 
        fTrackingStation->MakeImprint(fFASER2Assembly, Tpos, noRot, i, false); 
      }
    }
    
    // placing last set of tracking stations (after last magnet)
    G4ThreeVector offset(0, 0, magnetsLengthZ/2.+ fMagnetGap + totThickness/2.);
    G4ThreeVector postStationsCenter = magCenter + offset;

    for (int i= 0; i<fNTrackingStations; ++i) { 
      G4ThreeVector T(0, 0, -totThickness/2.+0.5*stationThickness+i*(fTrackingStationGap+stationThickness));
      G4ThreeVector Tp = postStationsCenter + T;
      fTrackingStation->MakeImprint(fFASER2Assembly, Tp, noRot, fNMagnets, false);
    }
   
  } else {
    G4cout << "ERROR: unknown FASER2 spectrometer magnet option!" << G4endl;  
  }

  // visibility
  G4VisAttributes* nullVis = new G4VisAttributes(G4Colour(167./255, 168./255, 189./255));
  nullVis->SetVisibility(false);
  fMagnetWindow->SetVisAttributes(nullVis);

  G4VisAttributes* magnetVis = new G4VisAttributes(G4Colour(234./255, 173./255, 26./255, 0.8));
  magnetVis->SetVisibility(true);
  fMagnetYoke->SetVisAttributes(magnetVis);
  
  G4VisAttributes* stationVis = new G4VisAttributes(G4Colour(34./255, 148./255, 83./255, 0.8));
  stationVis->SetVisibility(true);
  for (auto& station : fTrackingStationsLogical)
  {
    station->SetVisAttributes(stationVis);
  }
  
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
  auto magnetWindowSolid = new G4Tubs("MagnetWindow",0.,fMagnetInnerR,fMagnetLengthZ/2.,0.,CLHEP::twopi);
  auto magnetYokeSolid = new G4Tubs("MagnetYoke",fMagnetInnerR,fMagnetOuterR,fMagnetLengthZ/2.,0.,CLHEP::twopi);

  fMagnetYoke = new G4LogicalVolume(magnetYokeSolid, fMaterials->Material("Iron"), "FASER2MagnetYokeLogical");
  fMagnetWindow = new G4LogicalVolume(magnetWindowSolid, fMaterials->Material("Air"), "FASER2MagnetWindowLogical");
}

void FASER2DetectorConstruction::BuildTrackingStation()
{
  // Each tracking station is made of 2 layers
  // 1st layer: fNScinBarsY horizontal modules
  // 2nd layer: fNScinBarsX vertical modules
  G4double horizontalScinSize = fTrackingStationY / fNScinBarsY; // y size of horizontal scin
  G4double verticalScinSize = fTrackingStationX / fNScinBarsX; // x size of vertical scin

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
  fTrackingStation->AddPlacedVolume(trkLayerLogical,pos,&rot);
}
