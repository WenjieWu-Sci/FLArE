#include "geometry/GeometricalParameters.hh"
#include "G4SystemOfUnits.hh"

GeometricalParameters *GeometricalParameters::me = 0;

GeometricalParameters::GeometricalParameters()
{
  // **** DEFAULT VALUES ***
  
  // FLArE TPC volume
  fFLArETPCMaterialOption = tpcMaterialOption::LiquidArgon;
  fFLArETPCConfigOption   = tpcConfigOption::ThreeBySeven;
  fTPCSizeX = 1.8*m;  // optimized by energy containment  
  fTPCSizeY = 1.8*m;  // optimized by energy containment
  fTPCSizeZ = 7.0*m;  // optimized by energy containment
  fInsulationThickness = 80 * cm;
  fMuonCatcherLength = 36*cm; //updates during construction
  fHadCalLength = 120*cm;     //updates during construction
  fFLArEPos = G4ThreeVector(0., 0., 4300.*mm); 
  
  // FASER2 magnet
  fSpectrometerMagnetOption = magnetOption::SAMURAI;
  fSpectrometerMagnetField = 1.0*tesla;
  fMagnetTotalSizeZ = 4*m; //updates during construction
  fTrackingStationTotalSizeZ = 3.12*m; //updates during construction
  fMagnetZPos = 40*m; //updates during construction
  // SAMURAI design
  fSpectrometerMagnetWindowX = 3.0*m;
  fSpectrometerMagnetWindowY = 1.0*m;
  fSpectrometerMagnetWindowZ = 4.0*m;
  fSpectrometerMagnetYokeThickX = 1.5*m;
  fSpectrometerMagnetYokeThickY = 2.0*m;
  // CrystalPulling design
  fSpectrometerMagnetLengthZ = 1.25*m;
  fSpectrometerMagnetInnerR = 0.8*m;
  fSpectrometerMagnetOuterR = 1.2*m;
  fNSpectrometerMagnets = 3;
  fSpectrometerMagnetGap = 0.5*m;
  // Tracking stations
  fNTrackingStations = 6;
  fNScintillatorBarsY = 3;
  fNScintillatorBarsX = 7;
  fScintillatorThickness = 1*cm;
  fTrackingStationGap = 0.5*m;
  fFASER2Pos = G4ThreeVector(0., 0., 43036.*mm);

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
  fFASERnu2Pos = G4ThreeVector(0., 0., 22123.*mm);

  // FORMOSA
  fFORMOSATotalSizeZ = 5*m; //updates during construction
  fNScintillatorBarsX = 20;
  fNScintillatorBarsY = 20;
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

G4ThreeVector GeometricalParameters::GetSpectrometerMagnetField()
{
  if (fSpectrometerMagnetOption == magnetOption::SAMURAI )
    return G4ThreeVector(0.,fSpectrometerMagnetField,0.); //field along Y
  else if (fSpectrometerMagnetOption == magnetOption::CrystalPulling)     
    return G4ThreeVector(0.,fSpectrometerMagnetField,0.); //field along Y (for now, FIXME?)
  else
    return G4ThreeVector(0.,0.,0.);
}

