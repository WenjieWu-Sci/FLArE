#ifndef DETECTORCONSTRUCTIONMATERIAL_HH
#define DETECTORCONSTRUCTIONMATERIAL_HH

#include "globals.hh"

class G4Material;
class G4Element;

class DetectorConstructionMaterial {
  public:
    DetectorConstructionMaterial();
    ~DetectorConstructionMaterial() {;};
    static DetectorConstructionMaterial* GetInstance();
    G4Material* Material(G4String);

    static DetectorConstructionMaterial* theDetectorConstructionMaterial;

  private:
    G4Element *elH, *elD, *elHe, *elLi, *elBe, *elC, *elN, *elO;
    G4Element *elNe, *elNa, *elMg, *elAl, *elSi, *elP, *elS, *elCa; 
    G4Element *elTi, *elFe, *elCu, *elW, *elPb, *elU;

    G4Material *Aluminium, *FakeAluminium, *Iron, *Copper, *Tungsten, *Lead;
    G4Material *CO2, *SiO2, *P2O5, *Al2O3, *Fe2O3, *FeO, *CaO, *MgO, *Na2O, *TiO2;
    G4Material *Air, *Vacuum, *ArgonGas, *StainlessSteel, *Nickel; 
    G4Material *LiquidArgon, *LiquidKrypton, *LS, *Rock, *polystyrene;
    G4Material *fibrous_glass, *polyurethane_foam, *R_PUF;
    G4Material *Emulsion;
};

#endif
