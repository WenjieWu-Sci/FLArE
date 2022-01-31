#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

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
    G4Element *elH, *elD, *elHe, *elLi, *elBe, *elC, *elN;
    G4Element *elNe, *elAl, *elFe, *elCu, *elW, *elPb, *elU, *elO;
    G4Element *elCa, *elNa, *elSi, *elS;

    G4Material *Aluminium, *Iron, *Copper, *Tungsten, *Lead;
    G4Material *Air, *Vacuum, *ArgonGas, *StainlessSteel, *Nickel; 
    G4Material *LiquidArgon, *LiquidKrypton, *LS, *polystyrene;
};

#endif
