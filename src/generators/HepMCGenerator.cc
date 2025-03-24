#include "generators/GeneratorBase.hh"
#include "generators/HepMCGenerator.hh"
#include "generators/HepMCGeneratorMessenger.hh"

#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include <HepMC3/Print.h>

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Exception.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4RunManager.hh"

HepMCGenerator::HepMCGenerator()
{
  fGeneratorName = "hepmc";
  fMessenger = new HepMCGeneratorMessenger(this);

  fAsciiInput = nullptr;
  fVtxOffset = G4ThreeVector(0,0,0);
  fUseHepMC2 = false;
}

HepMCGenerator::~HepMCGenerator()
{
  delete fAsciiInput;
  delete fMessenger;
}

void HepMCGenerator::LoadData()
{   
  // this is called only once from PrimaryGeneratorAction, no need to worry about data bein reloaded anymore
  // TODO: Would be nice if we could load specific events - no functionallity in HepMC to do this - could make use of `skip` method?
  
  fAsciiInput = (fUseHepMC2) 
              ? static_cast<HepMC3::Reader*>(new HepMC3::ReaderAsciiHepMC2(fHepMCFilename)) 
              : static_cast<HepMC3::Reader*>(new HepMC3::ReaderAscii(fHepMCFilename));

  if( fAsciiInput->failed() ){
    G4String err = "Cannot open HepMC file : " + fHepMCFilename;
    G4Exception("HepMCGenerator", "FileError", FatalErrorInArgument, err.c_str());
  }
}

std::shared_ptr<HepMC3::GenEvent> HepMCGenerator::GenerateHepMCEvent()
{ 
  std::shared_ptr<HepMC3::GenEvent> evt = std::make_shared<HepMC3::GenEvent>();
  fAsciiInput->read_event(*evt);
  //// HepMC3::Print::content(*evt);
  return evt;
}


G4bool HepMCGenerator::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()
                                                 -> GetNavigatorForTracking();

  G4VPhysicalVolume* world= navigator-> GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()-> GetSolid();
  EInside qinside= solid-> Inside(pos);

  if( qinside != kInside) return false;
  else return true;
}


static G4VPhysicalVolume* FindPhysicalVolumeByName(const G4String& name) {
  G4PhysicalVolumeStore* pvStore = G4PhysicalVolumeStore::GetInstance();
  for (auto pv : *pvStore) {
      if (pv->GetName() == name) { 
          return pv; 
      }
  }
  return nullptr; // Return nullptr if no match is found
}


void HepMCGenerator::GeneratePrimaries(G4Event* anEvent)
{

  // complete line from PrimaryGeneratorAction..
  G4cout << ") : HepMC" << ((fUseHepMC2) ? "2" : "3") << " Generator ===oooOOOooo===" << G4endl;
  
  G4cout << "oooOOOooo Event # " << anEvent->GetEventID() << " oooOOOooo" << G4endl;
  G4cout << "GeneratePrimaries from file " << fHepMCFilename << G4endl;

  // generate next event
  std::shared_ptr<HepMC3::GenEvent> HepMCEvent = GenerateHepMCEvent();
  if(!HepMCEvent) {
    G4cout << "HepMCInterface: no generated particles. run terminated..." << G4endl;
    G4RunManager::GetRunManager()-> AbortRun();
    return;
  }

  HepMC2G4(HepMCEvent, anEvent);
}


static G4double GetStartOfDecayVolume()
{
    G4VPhysicalVolume* DVphysicalVolume = FindPhysicalVolumeByName("FASER2DecayVolPhysical");
    if (!DVphysicalVolume)
    {
        std::cout << "ERROR: Tried to retrive the FASER2 decay volume but it does not exist!" << std::endl;
        return 0;
    }
    G4ThreeVector DVPosition = DVphysicalVolume->GetTranslation();

    // std::cout << "DVPosition = " << DVPosition << std::endl;

    G4VPhysicalVolume* F2physicalVolume = FindPhysicalVolumeByName("FASER2Physical");
    G4ThreeVector F2Position = F2physicalVolume->GetTranslation();

    // std::cout << "F2Position = " << F2Position << std::endl;

    G4VPhysicalVolume* HallphysicalVolume = FindPhysicalVolumeByName("hallPV");
    G4ThreeVector HallPosition = HallphysicalVolume->GetTranslation();

    // std::cout << "HallPosition = " << HallPosition << std::endl;

    G4LogicalVolume* DVlogicalVol = DVphysicalVolume->GetLogicalVolume();
    G4VSolid* DVsolid = DVlogicalVol->GetSolid();
    G4Box* DVbox = dynamic_cast<G4Box*>(DVsolid);
    G4double DVzHalfLength = DVbox->GetZHalfLength();

    G4ThreeVector DVglobalPosition = HallPosition  + F2Position + DVPosition;

    return DVglobalPosition.z() - DVzHalfLength;
}


void HepMCGenerator::HepMC2G4(const std::shared_ptr<HepMC3::GenEvent> hepmcevt, G4Event* g4event)
{
  for (const auto& vertex : hepmcevt->vertices()) {

    // check world boundary
    HepMC3::FourVector pos = vertex->position(); // in mm, ns

    // offset is already dimensioned coming from parameter
    G4double vtx_x_offset = fVtxOffset.x();
    G4double vtx_y_offset = fVtxOffset.y();
    G4double vtx_z_offset = fVtxOffset.z();

    // declare the pos is mm
    G4LorentzVector xvtx(pos.x()*mm+vtx_x_offset, pos.y()*mm+vtx_y_offset, pos.z()*mm+vtx_z_offset, pos.t()*mm/c_light);
        
    if (! CheckVertexInsideWorld(xvtx.vect())){
      G4cout << "WARNING: tried to generate vertex outside of world volume!" << G4endl;
      G4cout << "WARNING: position was (" << pos.x() << ", "<<  pos.y() << ", " << pos.z() << ", " << pos.t() << ")" << G4endl;
      continue;
    }

    // If requested; work out where the decay volume starts and use that to offset the vertex so that it falls inside it
    //! Note: for this to work two assumptions are made
    //! 1. The vertices within the HepMC file start from (0,0,0). If this is not true then /hepmc/vtxOffset <x y z> MUST be set in the steering macro
    //! 2. The vertices in the HepMC file correspond to the the vertices in the 
    if (placeInDecayVolume)
    { 
      G4double decay_vol_start = GetStartOfDecayVolume();
      // std::cout << "Decay volume starts at " << decay_vol_start << std::endl;
      xvtx = G4LorentzVector(pos.x(), pos.y(), decay_vol_start+pos.z()+vtx_z_offset, pos.t());
    }


    // create G4PrimaryVertex and associated G4PrimaryParticles
    G4PrimaryVertex* g4vtx = new G4PrimaryVertex(xvtx.x(), xvtx.y(), xvtx.z(), xvtx.t());

    // G4cout << "Placing vertex at (" <<  xvtx.x()*mm << ", " << xvtx.y()*mm << ", " << xvtx.z()*mm << ", " << xvtx.t()*mm/c_light << ")" << G4endl;

    for (const auto& particle : vertex->particles_out())  {
      if( particle->status() == 1 ||  particle->status() == 5 )
      {
        G4int pdgcode = particle->pdg_id();
        pos = particle->momentum();
        G4LorentzVector p(pos.px(), pos.py(), pos.pz(), pos.e());
        G4PrimaryParticle* g4prim = new G4PrimaryParticle(pdgcode, p.x()*GeV, p.y()*GeV, p.z()*GeV);
        g4vtx->SetPrimary(g4prim);
      }
    }

    g4event->AddPrimaryVertex(g4vtx);
  }
}
