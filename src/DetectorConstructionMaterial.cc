#include "DetectorConstructionMaterial.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

DetectorConstructionMaterial* DetectorConstructionMaterial::theDetectorConstructionMaterial = NULL;

DetectorConstructionMaterial::DetectorConstructionMaterial() {
  G4double a,z,density,fractionmass;
  G4String name,symbol;
  G4int nel,natoms;

  //------------
  // elements
  //------------

  a=1.01*g/mole;
  elH=new G4Element(name="hydrogen",symbol="H2",z=1.,a);

  a=2.01*g/mole;
  elD=new G4Element(name="deuterium",symbol="D",z=1.,a);

  a=4.*g/mole;
  elHe=new G4Element(name="helium",symbol="He",z=2.,a);

  a=6.94*g/mole;
  elLi=new G4Element(name="lithium",symbol="Li",z=3.,a);

  a=9.01*g/mole;
  elBe=new G4Element(name="berillium",symbol="Be",z=4.,a);

  a=12.01*g/mole;
  elC=new G4Element(name="carbon",symbol="C",z=6.,a);

  a=14.01*g/mole;
  elN=new G4Element(name="nitrogen",symbol="N2",z=7.,a);

  a=16.*g/mole;
  elO=new G4Element(name="oxygen",symbol="O2",z=8.,a);

  a=20.18*g/mole;
  elNe=new G4Element(name="neon",symbol="Ne",z=10.,a);

  a=22.99*g/mole;
  elNa=new G4Element(name="sodium",symbol="Na",z=11.,a);

  a=24.305*g/mole;
  elMg=new G4Element(name="magnesium",symbol="Mg",z=12.,a);

  a=26.98*g/mole;
  elAl=new G4Element(name="aluminium",symbol="Al",z=13.,a);

  a=28.085*g/mole;
  elSi=new G4Element(name="silicon",symbol="Si",z=14.,a);
  
  a=30.973*g/mole;
  elP= new G4Element(name="phosphorus",symbol="P",z=15.,a);

  a=32.06*g/mole;
  elS=new G4Element(name="sulfur",symbol="S",z=16.,a);

  a=40.08*g/mole;
  elCa=new G4Element(name="calcium",symbol="Ca",z=20.,a);

  a=47.867*g/mole;
  elTi=new G4Element(name="titanium",symbol="Ti",z=22.,a);

  a=55.850*g/mole;
  elFe=new G4Element(name="iron",symbol="Fe",z=26.,a);

  a=63.54*g/mole;
  elCu=new G4Element(name="copper",symbol="Cu",z=29.,a);

  a=183.85*g/mole;
  elW=new G4Element(name="tungstenm",symbol="W",z=74.,a);

  a=207.19*g/mole;
  elPb=new G4Element(name="lead",symbol="Pb",z=82.,a);

  a=238.03*g/mole;
  elU=new G4Element(name="uranium",symbol="U",z=92.,a);


  //-------------------
  // simple materials
  //-------------------

  density = 2.7*g/cm3;
  a = 26.98*g/mole;
  Aluminium = new G4Material(name="Aluminium",z=13.,a,density);
  FakeAluminium = new G4Material(name="FakeAluminium",z=13.,a,density/2);
  
  density = 7.87*g/cm3;
  a = 55.85*g/mole;
  Iron = new G4Material(name="Iron",z=26.,a,density);

  density = 8.96*g/cm3;
  a = 63.54*g/mole;
  Copper = new G4Material(name="Copper",z=29.,a,density);

  density = 19.3*g/cm3;
  a = 183.85*g/mole;
  Tungsten = new G4Material(name="Tungsten",z=74.,a,density);

  density = 11.35*g/cm3;
  a = 207.19*g/mole;
  Lead = new G4Material(name="Lead",z=82.,a,density);

  density = 1.4*g/cm3;
  a = 39.95*g/mole;
  LiquidArgon = new G4Material(name="LiquidArgon",z=18.,a,density);

  density = 2.413*g/cm3;
  a = 84*g/mole;
  LiquidKrypton = new G4Material(name="LiquidKrypton",z=36.,a,density);

  density = 0.002*g/cm3;
  a = 39.95*g/mole;
  ArgonGas = new G4Material(name="ArgonGas",z=18.,a,density);

  density = 8.96*g/cm3;
  a = 58.69*g/mole;
  Nickel = new G4Material(name="Nickel",z=28.,a,density);
  
  //------------------
  // mixtures
  //------------------

  density = 1.290*mg/cm3;
  Air = new G4Material(name="Air",density, nel=2);
  Air->AddElement(elN, 0.7);
  Air->AddElement(elO, 0.3);

  density              = 1.e-5*g/cm3;
  G4double pressure    = 2.e-2*bar;
  G4double temperature = CLHEP::STP_Temperature;         //from PhysicalConstants.h
  Vacuum = new G4Material(name="Vacuum", density, nel=1,
			  kStateGas,temperature,pressure);
  Vacuum->AddMaterial(Air, fractionmass=1.);

  density = 1.977*mg/cm3;
  CO2 = new G4Material(name="CO2", density, nel=2);
  CO2->AddElement(elC, 1);
  CO2->AddElement(elO, 2);

  density = 2.2*g/cm3;
  SiO2 = new G4Material(name="SiO2", density, nel=2);
  SiO2->AddElement(elSi, 1);
  SiO2->AddElement(elO, 2);

  density = 1.562*g/cm3;
  P2O5 = new G4Material(name="P2O5", density, nel=2);
  P2O5->AddElement(elP, 2);
  P2O5->AddElement(elO, 5);

  density = 3.97*g/cm3;
  Al2O3 = new G4Material(name="Al2O3", density, nel=2);
  Al2O3->AddElement(elAl, 2);
  Al2O3->AddElement(elO, 3);

  density = 5.24*g/cm3;
  Fe2O3 = new G4Material(name="Fe2O3", density, nel=2);
  Fe2O3->AddElement(elFe, 2);
  Fe2O3->AddElement(elO, 3);

  density = 5.745*g/cm3;
  FeO = new G4Material(name="FeO", density, nel=2);
  FeO->AddElement(elFe, 1);
  FeO->AddElement(elO, 1);

  density = 3.35*g/cm3;
  CaO = new G4Material(name="CaO", density, nel=2);
  CaO->AddElement(elCa, 1);
  CaO->AddElement(elO, 1);

  density = 3.58*g/cm3;
  MgO = new G4Material(name="MgO", density, nel=2);
  MgO->AddElement(elMg, 1);
  MgO->AddElement(elO, 1);

  density = 2.27*g/cm3;
  Na2O = new G4Material(name="Na2O", density, nel=2);
  Na2O->AddElement(elNa, 2);
  Na2O->AddElement(elO, 1);

  density = 4.23*g/cm3;
  TiO2 = new G4Material(name="TiO2", density, nel=2);
  TiO2->AddElement(elTi, 1);
  TiO2->AddElement(elO, 2);

  // must have the right composition for stainless steel
  density = 8.96*g/cm3;
  StainlessSteel = new G4Material(name="StainlessSteel",density,nel=1);
  StainlessSteel->AddElement(elO, fractionmass = 1.);

  // LS
  density = 0.859*g/cm3;
  LS = new G4Material(name="LiquidScintillator", density, nel=5);
  LS->AddElement(elC, 0.87924);
  LS->AddElement(elH, 0.1201);
  LS->AddElement(elO, 0.00034);
  LS->AddElement(elN, 0.00027);
  LS->AddElement(elS, 0.00005);

  // Source: 
  // Plastic scintillator tiles (used both in CMS hadron calorimeter
  // and ATLAS hadron barrel calorimeter): 
  //     X0 = 42.4 cm  and  lambda_I = 79.360 cm.  
  density = 1.032*g/cm3;
  polystyrene = new G4Material("Polystyrene", density, nel=2);
  polystyrene->AddElement(elC, natoms=19);
  polystyrene->AddElement(elH, natoms=21);

  density = 2.58*g/cm3;
  fibrous_glass = new G4Material("fibrous_glass", density, 7);
  fibrous_glass->AddMaterial(SiO2 , 0.600);
  fibrous_glass->AddMaterial(Al2O3, 0.118);
  fibrous_glass->AddMaterial(Fe2O3, 0.001);
  fibrous_glass->AddMaterial(CaO  , 0.224);
  fibrous_glass->AddMaterial(MgO  , 0.034);
  fibrous_glass->AddMaterial(Na2O , 0.010);
  fibrous_glass->AddMaterial(TiO2 , 0.013);

  // The following fractional components are placeholders,
  // to be fixed (though they have very little effect,
  // as long as the density is correct)
  density = 0.13*g/cm3;
  polyurethane_foam = new G4Material("polyurethane_foam", density, 7);
  polyurethane_foam->AddMaterial(SiO2 , 0.600);
  polyurethane_foam->AddMaterial(Al2O3, 0.118);
  polyurethane_foam->AddMaterial(Fe2O3, 0.001);
  polyurethane_foam->AddMaterial(CaO  , 0.224);
  polyurethane_foam->AddMaterial(MgO  , 0.034);
  polyurethane_foam->AddMaterial(Na2O , 0.010);
  polyurethane_foam->AddMaterial(TiO2 , 0.013);

  // for the cryostat foam insulation: Reinforced polyurethane foam
  // Source
  // https://github.com/DUNE/dunecore/blob/b0269b11c6ad9acde6a59db6ea7fd46e88318273/dunecore/Geometry/gdml/dune35t4apa_v4_3mmpitch_nowires.gdml#L203-L208
  density = 0.2525*g/cm3;
  R_PUF = new G4Material("R_PUF", density, 2);
  R_PUF->AddMaterial(polyurethane_foam, 0.95);
  R_PUF->AddMaterial(fibrous_glass    , 0.05);

  // Rock
  density = 2.82*g/cm3;
  Rock = new G4Material(name="Rock", density, 11);
  Rock->AddMaterial(SiO2  , 0.5267);
  Rock->AddMaterial(FeO   , 0.1174);
  Rock->AddMaterial(Al2O3 , 0.1025);
  Rock->AddElement (elO   , 0.0771);
  Rock->AddMaterial(MgO   , 0.0473);
  Rock->AddMaterial(CO2   , 0.0422);
  Rock->AddMaterial(CaO   , 0.0382);
  Rock->AddElement (elC   , 0.024);
  Rock->AddElement (elS   , 0.0186);
  Rock->AddMaterial(Na2O  , 0.0053);
  Rock->AddMaterial(P2O5  , 0.0007);


  // Placeholder for AgBr emulsion
  // currently something similar (?) straight out of NIST database
  G4NistManager* nist = G4NistManager::Instance(); 
  Emulsion = nist->FindOrBuildMaterial("G4_PHOTO_EMULSION");

}

DetectorConstructionMaterial* DetectorConstructionMaterial::GetInstance() {
  if (theDetectorConstructionMaterial == NULL) {
    theDetectorConstructionMaterial = new DetectorConstructionMaterial();
  }
  return theDetectorConstructionMaterial;
}

G4Material* DetectorConstructionMaterial::Material(G4String what) {
  G4Material* material = 0;
  if(what == "Air")                material = Air;
  if(what == "Vacuum")             material = Vacuum;
  if(what == "LiquidArgon")        material = LiquidArgon;
  if(what == "LiquidKrypton")      material = LiquidKrypton;
  if(what == "Aluminium")          material = Aluminium;
  if(what == "FakeAluminium")      material = FakeAluminium;
  if(what == "Iron")               material = Iron;
  if(what == "Copper")             material = Copper;
  if(what == "Tungsten")           material = Tungsten;
  if(what == "Lead")               material = Lead;
  if(what == "ArgonGas")           material = ArgonGas;
  if(what == "StainlessSteel")     material = StainlessSteel;
  if(what == "Nickel")             material = Nickel;
  if(what == "LiquidScintillator") material = LS;
  if(what == "Polystyrene")        material = polystyrene;
  if(what == "R_PUF")              material = R_PUF;
  if(what == "Rock")               material = Rock;
  if(what == "Emulsion")           material = Emulsion;

  return material;
}
