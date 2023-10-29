#include "geometry/DUNENDLArDetectorConstruction.hh"
#include "geometry/GeometricalParameters.hh"

#include <G4VisAttributes.hh>
#include <G4GDMLParser.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4GDMLReadStructure.hh>
#include <G4Material.hh>

//Color extension include for reading
//
#include "geometry/G03ColorReader.hh"

// Color extension include for writing
//
#include "geometry/G03ColorWriter.hh"

DUNENDLArDetectorConstruction::DUNENDLArDetectorConstruction()
{
  G4cout<<"Building ND LAr"<<G4endl;
  fReader = new G03ColorReader;
  fWriter = new G03ColorWriter;
  parser = new G4GDMLParser(fReader, fWriter);
  parser->Read(GeometricalParameters::Get()->GetNDLArGDMLName(), false);
  worldPhys = parser->GetWorldVolume();

  BuildVolumes();
}

DUNENDLArDetectorConstruction::~DUNENDLArDetectorConstruction()
{
  delete fReader;
  delete fWriter;
  delete parser;
}

void DUNENDLArDetectorConstruction::BuildVolumes() 
{
  //parser.GetVolume("volTPC")->SetVisAttributes(absorVis);
  //parser.GetVolume("volTPCActive")->SetVisAttributes(absorVis);
  std::stringstream ss;
  ss << worldPhys->GetTranslation() << "\n\n";
  ss << "Found World:  " << worldPhys->GetName() << "\n";
  ss << "World LV:  " << worldPhys->GetLogicalVolume()->GetName() << "\n";
  G4LogicalVolumeStore* pLVStore = G4LogicalVolumeStore::GetInstance();
  ss << "Found " << pLVStore->size() << " logical volumes." 
    << "\n\n";
  G4PhysicalVolumeStore* pPVStore = G4PhysicalVolumeStore::GetInstance();
  ss << "Found " << pPVStore->size() << " physical volumes."
       << "\n\n";
  //G4SDManager* SDman = G4SDManager::GetSDMpointer();
  const G4GDMLAuxMapType* auxmap = parser->GetAuxMap();
  ss << "Found " << auxmap->size() << " volume(s) with auxiliary information."
       << "\n\n";
  ss << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
  G4cout << ss.str();
  G4cout << G4endl;

  for (auto const& [volume, auxes] : *auxmap) {
    G4cout << "Volume " << volume->GetName()
      << " has the following list of auxiliary information: \n";
    for (auto const& aux : auxes) {
      G4cout << "--> Type: " << aux.type << " Value: " << aux.value << "\n";
    }
  }

  std::map<G4String,G4Colour> Kolor;
  Kolor["Steel"]        = G4Colour::Red();
  Kolor["Copper"]       = G4Colour::Yellow();
  Kolor["PVT"]          = G4Colour::Green();
  Kolor["FR4"]          = G4Colour::Gray();
  Kolor["LAr"]          = G4Colour::Cyan();
  Kolor["Polyurethane"] = G4Colour::Grey();
  //Kolor["Rock"]         = G4Colour::Blue();

  G4VisAttributes* noVis = new G4VisAttributes();
  noVis->SetVisibility(false);
  for(auto pos=pLVStore->begin(); pos!=pLVStore->end(); pos++) {
    if (G4String((*pos)->GetName()).contains("DetEnclosure")) {
      (*pos)->SetVisAttributes(noVis);
      continue;
    }
    if (G4String((*pos)->GetMaterial()->GetName()).contains("Air")) {
      (*pos)->SetVisAttributes(noVis);
      continue;
    }
    if (G4String((*pos)->GetMaterial()->GetName()).contains("Rock")) {
      (*pos)->SetVisAttributes(noVis);
      continue;
    }
    G4int Ndaughters = (*pos)->GetNoDaughters();
    G4cout<<(*pos)->GetName()<<" NDaughters = "<<Ndaughters<<" , "<<(*pos)->GetMaterial()->GetName()<<G4endl;
    if (Kolor.find(G4String((*pos)->GetMaterial()->GetName())) != Kolor.end()) {
      G4double red   = Kolor[(*pos)->GetMaterial()->GetName()].GetRed();
      G4double green = Kolor[(*pos)->GetMaterial()->GetName()].GetGreen();
      G4double blue  = Kolor[(*pos)->GetMaterial()->GetName()].GetBlue();
      G4VisAttributes* vis;
      switch ( Ndaughters )
      {
        case 0:  vis = new G4VisAttributes(G4Colour(red, green, blue, 1.0));
        case 1:  vis = new G4VisAttributes(G4Colour(red, green, blue, 0.9));
        case 2:  vis = new G4VisAttributes(G4Colour(red, green, blue, 0.6));
        case 3:  vis = new G4VisAttributes(G4Colour(red, green, blue, 0.4));
        default: vis = new G4VisAttributes(G4Colour(red, green, blue, 0.1));
      }
      vis->SetVisibility(true);
      (*pos)->SetVisAttributes(vis);
      continue;
    }

    //if ((*pos)->GetVisAttributes()) {
    //  G4cout<<" VisAttributes of "<<(*pos)->GetName()<<" is not empty"<<G4endl;
    //  (*pos)->SetVisAttributes((*pos)->GetVisAttributes());
    //}
  }
}
