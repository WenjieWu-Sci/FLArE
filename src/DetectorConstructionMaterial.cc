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
  elH=new G4Element(name="Hydrogen",symbol="H2",z=1.,a);

  a=2.01*g/mole;
  elD=new G4Element(name="Deuterium",symbol="D",z=1.,a);

  a=4.*g/mole;
  elHe=new G4Element(name="Helium",symbol="He",z=2.,a);

  a=6.94*g/mole;
  elLi=new G4Element(name="Lithium",symbol="Li",z=3.,a);

  a=9.01*g/mole;
  elBe=new G4Element(name="Berillium",symbol="Be",z=4.,a);

  a=12.01*g/mole;
  elC=new G4Element(name="Carbon",symbol="C",z=6.,a);

  a=14.01*g/mole;
  elN=new G4Element(name="Nitrogen",symbol="N2",z=7.,a);

  a=16.*g/mole;
  elO=new G4Element(name="Oxygen",symbol="O2",z=8.,a);

  a=20.18*g/mole;
  elNe=new G4Element(name="Neon",symbol="Ne",z=10.,a);

  a=22.99*g/mole;
  elNa=new G4Element(name="Sodium",symbol="Na",z=11.,a);

  a=26.98*g/mole;
  elAl=new G4Element(name="Aluminium",symbol="Al",z=13.,a);

  a=28.085*g/mole;
  elSi=new G4Element(name="Silicon",symbol="Si",z=14.,a);

  a=40.08*g/mole;
  elCa=new G4Element(name="Calcium",symbol="Ca",z=20.,a);

  a=55.850*g/mole;
  elFe=new G4Element(name="Iron",symbol="Fe",z=26.,a);

  a=63.54*g/mole;
  elCu=new G4Element(name="Copper",symbol="Cu",z=29.,a);

  a=183.85*g/mole;
  elW=new G4Element(name="Tungstenm",symbol="W",z=74.,a);

  a=207.19*g/mole;
  elPb=new G4Element(name="Lead",symbol="Pb",z=82.,a);

  a=238.03*g/mole;
  elU=new G4Element(name="Uranium",symbol="U",z=92.,a);

  a=32.06*g/mole;
  elS=new G4Element(name="Sulfur",symbol="S",z=16.,a);

  //-------------------
  // simple materials
  //-------------------

  density = 2.7*g/cm3;
  a = 26.98*g/mole;
  Aluminium = new G4Material(name="Aluminium",z=13.,a,density);
  
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
  if(what == "Iron")               material = Iron;
  if(what == "Copper")             material = Copper;
  if(what == "Tungsten")           material = Tungsten;
  if(what == "Lead")               material = Lead;
  if(what == "ArgonGas")           material = ArgonGas;
  if(what == "StainlessSteel")     material = StainlessSteel;
  if(what == "Nickel")             material = Nickel;
  if(what == "LiquidScintillator") material = LS;
  if(what == "Polystyrene")        material = polystyrene;

  return material;
}
