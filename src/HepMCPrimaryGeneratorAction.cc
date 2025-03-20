#include "HepMCPrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "geometry/GeometricalParameters.hh"


HepMCPrimaryGeneratorAction::HepMCPrimaryGeneratorAction(G4GeneralParticleSource* gps)
    : fGPS(gps)
{
}

HepMCPrimaryGeneratorAction::~HepMCPrimaryGeneratorAction()
{
    delete asciiInput;
}

void HepMCPrimaryGeneratorAction::LoadFile(G4String filename, G4bool override, G4bool useHepMC2)
{   
    // The override check is so that the file isn't reloaded when reading data in event loop
    // Can't load the data in the constructor since I **think** that is called before the messenger can parse the filename
    // TODO: Would be nice if we could load specific events - no functionallity in HepMC to do this - could make use of `skip` method?

    HepMCFileName = filename;
    if (!isFileLoaded)
    {   
        asciiInput = (useHepMC2) 
                     ? static_cast<HepMC3::Reader*>(new HepMC3::ReaderAsciiHepMC2(HepMCFileName)) 
                     : static_cast<HepMC3::Reader*>(new HepMC3::ReaderAscii(HepMCFileName));

        isFileLoaded = true;
    }
    else if(override)
    {
        delete asciiInput;
        asciiInput = (useHepMC2) 
                     ? static_cast<HepMC3::Reader*>(new HepMC3::ReaderAsciiHepMC2(HepMCFileName)) 
                     : static_cast<HepMC3::Reader*>(new HepMC3::ReaderAscii(HepMCFileName));

        std::cout << "HepMCPrimaryGeneratorAction::LoadFile   Overriding existing file" << std::endl;
    }
}


std::shared_ptr<HepMC3::GenEvent> HepMCPrimaryGeneratorAction::GenerateHepMCEvent()
{ 
  std::shared_ptr<HepMC3::GenEvent> evt = std::make_shared<HepMC3::GenEvent>();
  asciiInput->read_event(*evt);
  //// HepMC3::Print::content(*evt);
  return evt;
}


G4bool HepMCPrimaryGeneratorAction::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()
                                                 -> GetNavigatorForTracking();

  G4VPhysicalVolume* world= navigator-> GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()-> GetSolid();
  EInside qinside= solid-> Inside(pos);

  if( qinside != kInside) return false;
  else return true;
}


static G4LogicalVolume* FindLogicalVolumeByName(const G4String& name) {
    G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
    for (auto lv : *lvStore) {
        if (lv->GetName() == name) {
            return lv;
        }
    }
    return nullptr; 
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


static G4bool CheckVertexInDecayVolume(const G4ThreeVector& pos)
{   
     G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
     G4VPhysicalVolume* physicalVolume = navigator->LocateGlobalPointAndSetup(pos);

     //! Seems not to work - always seems to think vertex is in hallPV even though it definitely isn't
     if (physicalVolume && physicalVolume->GetName() == "FASER2DecayVolPhysical") {
         return true;
     } else {
        std::cout  << "Vertex not in decay volume - it is in" << physicalVolume->GetName() << std::endl;
        return false;
     }
}

void HepMCPrimaryGeneratorAction::GeneratePrimaryVertex(G4Event* anEvent, G4ThreeVector vtx_offset, G4bool placeInDecayVolume)
{
    // generate next event
    std::shared_ptr<HepMC3::GenEvent> HepMCEvent = GenerateHepMCEvent();
    if(!HepMCEvent) {
        G4cout << "HepMCInterface: no generated particles. run terminated..." << G4endl;
        G4RunManager::GetRunManager()-> AbortRun();
        return;
    }

    HepMC2G4(HepMCEvent, anEvent, vtx_offset, placeInDecayVolume);
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


void HepMCPrimaryGeneratorAction::HepMC2G4(const std::shared_ptr<HepMC3::GenEvent> hepmcevt, G4Event* g4event, G4ThreeVector vtx_offset, G4bool placeInDecayVolume)
{
    for (const auto& vertex : hepmcevt->vertices()) {

        HepMC3::FourVector pos = vertex->position();

        G4double vtx_x_offset = vtx_offset.x()*mm;
        G4double vtx_y_offset = vtx_offset.y()*mm;
        G4double vtx_z_offset = vtx_offset.z()*mm;

        std::cout << "Applying offset to primary vertex of " << vtx_offset << std::endl;
        G4LorentzVector xvtx(pos.x()+vtx_x_offset, pos.y()+vtx_y_offset, pos.z()+vtx_z_offset, pos.t());
        
        // Check that vertex is inside of the world volume
        if (! CheckVertexInsideWorld(xvtx.vect()*mm))
        { 
        std::cout << "WARNING: tried to generate vertex outside of world volume!" << std::endl;
        std::cout << "WARNING: position was (" << xvtx.x() << ", "<<  xvtx.y() << ", " << xvtx.z() << ", " << xvtx.t() << ")" << std::endl;
        std::cout << "Not going to generate a primary vertex for this event" << std::endl;
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
        G4PrimaryVertex* g4vtx = new G4PrimaryVertex(xvtx.x()*mm, xvtx.y()*mm, xvtx.z()*mm, xvtx.t()*mm/c_light);

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
