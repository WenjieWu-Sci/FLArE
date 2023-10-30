#include "geometry/SpectrometerMagnetConstruction.hh"
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

SpectrometerMagnetConstruction::SpectrometerMagnetConstruction()
{
  // load materials
  fMaterials = DetectorConstructionMaterial::GetInstance();

  // choose magnet option
  G4String opt = GeometricalParameters::Get()->GetSpectrometerMagnetOption(); 
  if( opt == GeometricalParameters::magnetOption::SAMURAI ){
    
    G4cout << "Building SAMURAI spectrometer magnet" << G4endl;
    fMagnetWindowX = GeometricalParameters::Get()->GetSpectrometerMagnetWindowX();
    fMagnetWindowY = GeometricalParameters::Get()->GetSpectrometerMagnetWindowY();
    fMagnetWindowZ = GeometricalParameters::Get()->GetSpectrometerMagnetWindowZ();
    fMagnetYokeThicknessX = GeometricalParameters::Get()->GetSpectrometerMagnetYokeThickX();
    fMagnetYokeThicknessY = GeometricalParameters::Get()->GetSpectrometerMagnetYokeThickY();
   
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
   
    GeometricalParameters::Get()->SetMagnetTotalSizeZ(fMagnetWindowZ);
    GeometricalParameters::Get()->SetTrackingStationTotalSizeZ(totThickness);
    GeometricalParameters::Get()->SetFASER2TotalSizeZ(fMagnetWindowZ+2*gapToMagnet+2*totThickness);

    BuildSAMURAIDesign(); //sets logical volumes
    BuildTrackingStation(); //sets assembly volume
    
    fMagnetAssembly = new G4AssemblyVolume();
    G4RotationMatrix *noRot = new G4RotationMatrix();

    // center of the assembly is the center of the magnet window
    G4ThreeVector magCenter(0.,0.,0.);

    // placing magnet + yoke
    fMagnetAssembly->AddPlacedVolume(fMagnetWindow,magCenter,noRot);
    fMagnetAssembly->AddPlacedVolume(fMagnetYoke,magCenter,noRot);

    // middle position of pre-magnet and post-magnet tracking stations
    G4ThreeVector offset(0, 0, fMagnetWindowZ/2.+ gapToMagnet + totThickness/2.);
    G4ThreeVector preStationsCenter = magCenter - offset;
    G4ThreeVector postStationsCenter = magCenter + offset;
  
    // placing tracking stations (before/after magnet)
    for (int i= 0; i<fNTrackingStations; ++i) { 
      G4ThreeVector T(0, 0, -totThickness/2.+0.5*stationThickness+i*(fTrackingStationGap+stationThickness));
      G4ThreeVector Tp = postStationsCenter + T;
      G4ThreeVector Tm = preStationsCenter + T;
      fMagnetAssembly->AddPlacedAssembly(fTrackingStation, Tm, noRot); //place before magnet
      fMagnetAssembly->AddPlacedAssembly(fTrackingStation, Tp, noRot); //place after magnet
    }

  } else if ( opt == GeometricalParameters::magnetOption::CrystalPulling ){
 
    G4cout << "Building CrystalPulling spectrometer magnet" << G4endl;
    fMagnetLengthZ = GeometricalParameters::Get()->GetSpectrometerMagnetLengthZ();
    fMagnetInnerR = GeometricalParameters::Get()->GetSpectrometerMagnetInnerR();
    fMagnetOuterR = GeometricalParameters::Get()->GetSpectrometerMagnetOuterR();
    fNMagnets = GeometricalParameters::Get()->GetNSpectrometerMagnets();
    fMagnetGap = GeometricalParameters::Get()->GetSpectrometerMagnetGap();
    
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
    GeometricalParameters::Get()->SetFASER2TotalSizeZ(totalLengthZ);    

    BuildCrystalPullingDesign(); //sets logical volumes
    BuildTrackingStation();
    
    fMagnetAssembly = new G4AssemblyVolume();
    G4RotationMatrix *noRot = new G4RotationMatrix();

    // center of the assembly is the center of middle magnet window
    // if fNMagnets is even, it falls in the gap between the two middle magnets
    G4ThreeVector magCenter(0.,0.,0.);
   
    // each magnet comes with a set of N tracking stations just before it
    // the gap between each magnet is then given by magnetSpacing
    G4double magnetSpacing = 2*fMagnetGap + totThickness;
    
    for(int i=0; i<fNMagnets; i++){
      G4double offset = (i-0.5*(fNMagnets-1))*(magnetSpacing+fMagnetLengthZ);
      G4ThreeVector magPos = magCenter + G4ThreeVector(0.,0.,offset);
      fMagnetAssembly->AddPlacedVolume(fMagnetWindow,magPos,noRot);
      fMagnetAssembly->AddPlacedVolume(fMagnetYoke,magPos,noRot);

      // placing tracking stations (before each magnet)
      G4double trackingStationOffset = -fMagnetGap-0.5*fMagnetLengthZ;
      for (int j= 0; j<fNTrackingStations; ++j) { 
        G4ThreeVector T(0., 0., trackingStationOffset-j*fTrackingStationGap-0.5*(i+1)*stationThickness);
        G4ThreeVector Tpos = magPos + T; 
        fMagnetAssembly->AddPlacedAssembly(fTrackingStation, Tpos, noRot);
      }
    }
    
    // placing last set of tracking stations (after last magnet)
    G4ThreeVector offset(0, 0, magnetsLengthZ/2.+ fMagnetGap + totThickness/2.);
    G4ThreeVector postStationsCenter = magCenter + offset;

    for (int i= 0; i<fNTrackingStations; ++i) { 
      G4ThreeVector T(0, 0, -totThickness/2.+0.5*stationThickness+i*(fTrackingStationGap+stationThickness));
      G4ThreeVector Tp = postStationsCenter + T;
      fMagnetAssembly->AddPlacedAssembly(fTrackingStation, Tp, noRot);
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
  fHorTrackingScinBar->SetVisAttributes(stationVis);  
  fVerTrackingScinBar->SetVisAttributes(stationVis);  
}

SpectrometerMagnetConstruction::~SpectrometerMagnetConstruction()
{ 
  delete fMagnetAssembly;
  delete fMagnetWindow;
  delete fMagnetYoke;
}

void SpectrometerMagnetConstruction::BuildSAMURAIDesign()
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

void SpectrometerMagnetConstruction::BuildCrystalPullingDesign()
{
  auto magnetWindowSolid = new G4Tubs("MagnetWindow",0.,fMagnetInnerR,fMagnetLengthZ/2.,0.,CLHEP::twopi);
  auto magnetYokeSolid = new G4Tubs("MagnetYoke",fMagnetInnerR,fMagnetOuterR,fMagnetLengthZ/2.,0.,CLHEP::twopi);

  fMagnetYoke = new G4LogicalVolume(magnetYokeSolid, fMaterials->Material("Iron"), "FASER2MagnetYokeLogical");
  fMagnetWindow = new G4LogicalVolume(magnetWindowSolid, fMaterials->Material("Air"), "FASER2MagnetWindowLogical");
}

void SpectrometerMagnetConstruction::BuildTrackingStation()
{
  // Each tracking station is made of 2 layers
  // 1st layer: fNScinBarsY horizontal modules
  // 2nd layer: fNScinBarsX vertical modules
  G4double horizontalScinSize = fTrackingStationY / fNScinBarsY; // y size of horizontal scin
  G4double verticalScinSize = fTrackingStationX / fNScinBarsX; // x size of vertical scin

  // layer volume: same size, but first has horizontal bars, second one has vertical bars
  auto trkLayerSolid = new G4Box("trkLayerBox", fTrackingStationX/2, fTrackingStationY/2., fScinThickness/2.);
  auto trkHorLayerLogical = new G4LogicalVolume(trkLayerSolid, fMaterials->Material("Polystyrene"), "trkHorLayerLogical"); 
  auto trkVerLayerLogical = new G4LogicalVolume(trkLayerSolid, fMaterials->Material("Polystyrene"), "trkVerLayerLogical"); 
  
  // first layer is made of 2 horizontal pieces
  auto trkHorScinSolid = new G4Box("trkHorScinSolid", fTrackingStationX/2., horizontalScinSize/2., fScinThickness/2.);
  fHorTrackingScinBar = new G4LogicalVolume(trkHorScinSolid, fMaterials->Material("Polystyrene"), "trkHorScinLogical");
  new G4PVReplica("trkHorScinPhysical", fHorTrackingScinBar,
                  trkHorLayerLogical, kYAxis, fNScinBarsY, horizontalScinSize);
  // secondo layer is made of 7 vertical pieces
  auto trkVerScinSolid = new G4Box("trkHorScinSolid", verticalScinSize/2., fTrackingStationY/2., fScinThickness/2.);
  fVerTrackingScinBar = new G4LogicalVolume(trkVerScinSolid, fMaterials->Material("Polystyrene"), "trkVerScinLogical");
  new G4PVReplica("trkVerScinPhysical", fVerTrackingScinBar,
                  trkVerLayerLogical, kXAxis, fNScinBarsX, verticalScinSize);
  
  fTrackingStation = new G4AssemblyVolume(); //one assembly has 2 layers
  G4RotationMatrix rot(0, 0, 0);
  G4ThreeVector pos(0, 0, -fScinThickness/2.);
  fTrackingStation->AddPlacedVolume(trkHorLayerLogical,pos,&rot);
  pos.setZ(fScinThickness/2.);
  fTrackingStation->AddPlacedVolume(trkVerLayerLogical,pos,&rot);
}
