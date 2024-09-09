#include "DetectorConstructionMaterial.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

DetectorConstructionMaterial* DetectorConstructionMaterial::theDetectorConstructionMaterial = NULL;

DetectorConstructionMaterial::DetectorConstructionMaterial() {
  G4NistManager* nist = G4NistManager::Instance(); 
  G4double a,z,density,fractionmass;
  G4String name,symbol;
  G4int nel,natoms;
  
  //------------
  // elements
  //------------

  elH = nist->FindOrBuildElement("H");
  elHe= nist->FindOrBuildElement("He");
  elLi= nist->FindOrBuildElement("Li");
  elBe= nist->FindOrBuildElement("Be");
  elC = nist->FindOrBuildElement("C");
  elN = nist->FindOrBuildElement("N");
  elO = nist->FindOrBuildElement("O");
  elNe= nist->FindOrBuildElement("Ne");
  elNa= nist->FindOrBuildElement("Na");
  elMg= nist->FindOrBuildElement("Mg");
  elAl= nist->FindOrBuildElement("Al");
  elSi= nist->FindOrBuildElement("Si");
  elP = nist->FindOrBuildElement("P");
  elS = nist->FindOrBuildElement("S");
  elCa= nist->FindOrBuildElement("Ca");
  elTi= nist->FindOrBuildElement("Ti");
  elFe= nist->FindOrBuildElement("Fe");
  elCu= nist->FindOrBuildElement("Cu");
  elW = nist->FindOrBuildElement("W");
  elPb= nist->FindOrBuildElement("Pb");
  elU = nist->FindOrBuildElement("U");
  elMn= nist->FindOrBuildElement("Mn");
  elSn= nist->FindOrBuildElement("Sn");

  a=2.01*g/mole;
  elD=new G4Element(name="deuterium",symbol="D",z=1.,a);

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

  LiquidArgon = nist->FindOrBuildMaterial("G4_lAr");
  
  LiquidKrypton = nist->FindOrBuildMaterial("G4_lKr");

  density = 0.002*g/cm3;
  a = 39.95*g/mole;
  ArgonGas = new G4Material(name="ArgonGas",z=18.,a,density);

  density = 8.96*g/cm3;
  a = 58.69*g/mole;
  Nickel = new G4Material(name="Nickel",z=28.,a,density);
  
  //------------------
  // mixtures
  //------------------

  // Air
  Air = nist->FindOrBuildMaterial("G4_AIR");

  // Vacuum (= very low density air)
  density              = 1.e-5*g/cm3;
  G4double pressure    = 2.e-2*bar;
  G4double temperature = CLHEP::STP_Temperature;         //from PhysicalConstants.h
  Vacuum = new G4Material(name="Vacuum", density, nel=1,
			  kStateGas,temperature,pressure);
  Vacuum->AddMaterial(Air, fractionmass=1.);

  // Oxides
  CO2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
  SiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  Al2O3 = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  Fe2O3 = nist->FindOrBuildMaterial("G4_FERRIC_OXIDE");
  FeO = nist->FindOrBuildMaterial("G4_FERROUS_OXIDE");
  CaO = nist->FindOrBuildMaterial("G4_CALCIUM_OXIDE");
  MgO = nist->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
  Na2O = nist->FindOrBuildMaterial("G4_POTASSIUM_OXIDE");
  TiO2 = nist->FindOrBuildMaterial("G4_TITANIUM_DIOXIDE");
  
  density = 1.562*g/cm3;
  P2O5 = new G4Material(name="P2O5", density, nel=2);
  P2O5->AddElement(elP, 2);
  P2O5->AddElement(elO, 5);
  
  // Stainless steel
  StainlessSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  
  // common plastic (polyethilene)
  Polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

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
  Emulsion = nist->FindOrBuildMaterial("G4_PHOTO_EMULSION");

  // ARMCO (almost pure iron, but still technically steel))
  // source:  https://www.aksteel.co.uk/products/armco-pure-iron/#chemical-composition
  density = 7.8*g/cm3;
  ARMCO = new G4Material(name="ARMCO", density, 6);
  ARMCO->AddElement(elFe, 0.9985);
  ARMCO->AddElement(elMn, 0.0010);
  ARMCO->AddElement(elCu, 0.0002);
  ARMCO->AddElement(elSn, 0.0001);
  ARMCO->AddElement(elC, 0.0001);
  ARMCO->AddElement(elP, 0.0001);

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
  if(what == "ARMCO")              material = ARMCO;
  if(what == "Polyethylene")       material = Polyethylene;

  return material;
}
