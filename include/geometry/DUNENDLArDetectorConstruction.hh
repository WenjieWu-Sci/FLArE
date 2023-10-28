#ifndef DUNENDLARDETECTORCONSTRUCTION_HH
#define DUNENDLARDETECTORCONSTRUCTION_HH

#include <G4GDMLParser.hh>

/// \class DUNENDLArDetectorConstruction
/// This class builds DUNE's ND-LAr

class G4VPhysicalVolume;

class DUNENDLArDetectorConstruction {
  public:
    DUNENDLArDetectorConstruction();
    ~DUNENDLArDetectorConstruction();

    // Return logical volumes
    G4VPhysicalVolume* GetWorldPhysVol() { return worldPhys; }

    // Render the geometry
    void BuildVolumes();

  private:
    // Extended reader
    //
    G4GDMLReadStructure* fReader;
    
    // Extended writer
    //
    G4GDMLWriteStructure* fWriter;

    // GDMLparser
    // 
    G4GDMLParser* parser;

    G4VPhysicalVolume* worldPhys;
};

#endif
