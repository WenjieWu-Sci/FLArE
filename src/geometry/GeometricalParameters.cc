#include "geometry/GeometricalParameters.hh"
#include "G4SystemOfUnits.hh"

GeometricalParameters *GeometricalParameters::me = 0;

GeometricalParameters::GeometricalParameters()
{
  // **** DEFAULT VALUES ***
  
  // experimental hall
  fHallHeadDistance = 3.1*m;
  fHallOffsetX = 1.44*m;
  fHallOffsetY = 2.21*m;

  // FLArE TPC volume
  fFLArETPCMaterialOption = tpcMaterialOption::LiquidArgon;
  fFLArETPCConfigOption   = tpcConfigOption::ThreeBySeven;
  fTPCSizeX = 1.8*m;  // optimized by energy containment  
  fTPCSizeY = 1.8*m;  // optimized by energy containment
  fTPCSizeZ = 7.0*m;  // optimized by energy containment
  fTPCFidVolSize = G4ThreeVector(1.0*m, 1.0*m, 7.0*m);
  fInsulationThickness = 80 * cm;
  fMuonCatcherLength = 36*cm; //updates during construction
  fHadCalLength = 120*cm;     //updates during construction
  fFLArEPos = G4ThreeVector(0., 0., 4300.*mm); 
  
  // BabyMIND
  fUseBabyMIND = false; //default
  fBabyMINDMagnetPlateThickness = 30*mm;
  fBabyMINDMagnetPlateSizeX = 3.5*m;
  fBabyMINDMagnetPlateSizeY = 2.*m;
  fBabyMINDMagnetCentralPlateY = 1.*m;
  fBabyMINDSlitSizeX = 2.8*m;
  fBabyMINDSlitSizeY = 10*mm;
  fBabyMINDFieldStrength = 1.5*tesla;
  fBabyMINDNVerticalBars = 16;
  fBabyMINDNHorizontalBars = 96;
  fBabyMINDBarThickness = 7.5*mm;
  fBabyMINDVerticalBarSizeX = 21.*cm;
  fBabyMINDVerticalBarSizeY = 2.*m;
  fBabyMINDHorizontalBarSizeX = 3.*m;
  fBabyMINDHorizontalBarSizeY = 3.*cm;
  fBabyMINDTotalSizeZ = 3300*mm; //updates during construction
  fBabyMINDMagnetToScinSpacing = 15*mm;
  fBabyMINDMagnetToMagnetSpacing = 20*mm;
  fBabyMINDBlockToBlockSpacing = 30*mm;
  fBabyMINDBlockPadding = 100*mm;
  fBabyMINDBlockSequence = "|MMMMD||DMMMD||DMMMMD||MMDMMD||MMDMMD||MDMDMD||DMMMD|";

  // FASER2 magnet
  fFASER2MagnetOption = magnetOption::SAMURAI;
  fFASER2MagnetField = 1.0*tesla;
  fMagnetTotalSizeZ = 4*m; //updates during construction
  fTrackingStationTotalSizeZ = 2.62*m; //updates during construction
  fMagnetZPos = 40*m; //updates during construction
  fFASER2TotalSizeZ = 25*m; //updates during construction
  fFASER2DecayVolumeLength = 10*m;
  // SAMURAI design
  fFASER2MagnetWindowX = 3.0*m;
  fFASER2MagnetWindowY = 1.0*m;
  fFASER2MagnetWindowZ = 4.0*m;
  fFASER2MagnetYokeThickX = 1.5*m;
  fFASER2MagnetYokeThickY = 0.5*m;
  // CrystalPulling design
  fFASER2MagnetInnerR = 0.8*m;
  fFASER2MagnetOuterR = 1.2*m;
  fNFASER2Magnets = 3;
  fFASER2MagnetGap = 0.25*m;
  fFASER2MagnetSpacing = 0.76*m; //updates during construction
  // Tracking stations
  fNTrackingStations = 3;
  fScintillatorThickness = 1*cm;
  fTrackingStationGap = 0.5*m;
  fFASER2Pos = G4ThreeVector(0., 0., 42636.*mm);
  fFASER2TrackerX = 3*m;
  fFASER2TrackerY = 1*m;  
  fFASER2UpstreamTrackingStationGap = 0.5*m;
  fFASER2DownstreamTrackingStationGap = 4*m;

  fFASER2FillCaloAndWall = true; //! Simulating the additional interactions in Copper/Iron is time consuming and often not needed 
  fFASER2EMCaloThickness = 1*m; //! These values are placeholders and require dedicated optmisation studies!
  fFASER2HadCaloThickness = 2*m; //! These values are placeholders and require dedicated optmisation studies!
  fFASER2IronWallThickness = 3*m; //! These values are placeholders and require dedicated optmisation studies!
  fFASER2VetoLengthX = 4*m; //! These values are placeholders and require dedicated optmisation studies!
  fFASER2VetoLengthY = 3*m; //! These values are placeholders and require dedicated optmisation studies!
  fFASER2VetoShieldThickness = 10*cm; //! These values are placeholders and require dedicated optmisation studies!

  // FASERnu2 emulsion detector
  fFASERnu2TotalSizeZ = 8.5*m; //updates during construction
  fNEmulsionTungstenLayers = 3300;
  fTungstenThickness = 2*mm;
  fEmulsionThickness = 0.420*mm;
  fEmulsionTungstenSizeX = 40*cm;
  fEmulsionTungstenSizeY = 40*cm;
  fVetoInterfaceSizeZ = 20*cm;
  fVetoInterfaceSizeX = 80*cm;
  fVetoInterfaceSizeY = 80*cm;
  fFASERnu2Pos = G4ThreeVector(0., 0., 22023.*mm);

  // FORMOSA
  fFORMOSATotalSizeZ = 5*m; //updates during construction
  fNScinBarsX = 20;
  fNScinBarsY = 20;
  fScintillatorBarSizeX = 5*cm;
  fScintillatorBarSizeY = 5*cm;
  fScintillatorBarSizeZ = 1*m;
  fNScintillatorModules = 4;
  fPMTSizeSpacing = 33*cm;
  fFORMOSAPos = G4ThreeVector(0., 0., 13870.*mm);
}

GeometricalParameters* GeometricalParameters::Get()
{
  if (!me)
    me = new GeometricalParameters();
  return me; 
}

GeometricalParameters::tpcMaterialOption GeometricalParameters::ConvertStringToTPCMaterialOption(G4String val)
{
  if (val == "LAr") {
    return tpcMaterialOption::LiquidArgon;
  } else if (val == "LKr") {
    return tpcMaterialOption::LiquidKrypton;
  } else {
    G4cout<<"WARNING: unknow material for the TPC volume! Set it to default."<<G4endl;
    return tpcMaterialOption::LiquidArgon;
  }
}

GeometricalParameters::tpcConfigOption GeometricalParameters::ConvertStringToTPCConfigOption(G4String val)
{
  if (val == "single") {
    return tpcConfigOption::Single;
  } else if (val == "3x7") {
    return tpcConfigOption::ThreeBySeven;
  } else {
    G4cout<<"WARNING: unknow configuration for the TPC volume! Set it to default."<<G4endl;
    return tpcConfigOption::Single;
  }
}

GeometricalParameters::magnetOption GeometricalParameters::ConvertStringToMagnetOption(G4String val)
{
  if (val == "SAMURAI" || val == "samurai")
    return magnetOption::SAMURAI;
  else if (val == "CrystalPulling" || val == "crystalpulling")     
    return magnetOption::CrystalPulling;
  else {
    G4cout << "ERROR: unknown FASER2 magnet geometry!" << G4endl;
    return magnetOption::unknown;
  }
}

G4ThreeVector GeometricalParameters::GetFASER2MagnetField()
{
  if (fFASER2MagnetOption == magnetOption::SAMURAI )
    return G4ThreeVector(0.,fFASER2MagnetField,0.); //field along Y
  else if (fFASER2MagnetOption == magnetOption::CrystalPulling)     
    return G4ThreeVector(0.,fFASER2MagnetField,0.); //field along Y (for now, FIXME?)
  else
    return G4ThreeVector(0.,0.,0.);
}

