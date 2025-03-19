#include "HepMCPrimaryGeneratorAction.hh"


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


void HepMCPrimaryGeneratorAction::GeneratePrimaryVertex(G4Event* anEvent, G4ThreeVector vtx_offset)
{
    // generate next event
    std::shared_ptr<HepMC3::GenEvent> HepMCEvent = GenerateHepMCEvent();
    if(!HepMCEvent) {
        G4cout << "HepMCInterface: no generated particles. run terminated..." << G4endl;
        G4RunManager::GetRunManager()-> AbortRun();
        return;
    }

    HepMC2G4(HepMCEvent, anEvent, vtx_offset);
}


void HepMCPrimaryGeneratorAction::HepMC2G4(const std::shared_ptr<HepMC3::GenEvent> hepmcevt, G4Event* g4event, G4ThreeVector vtx_offset)
{
    for (const auto& vertex : hepmcevt->vertices()) {

        // check world boundary
        HepMC3::FourVector pos = vertex->position();

        G4double vtx_x_offset = vtx_offset.x()*mm;
        G4double vtx_y_offset = vtx_offset.y()*mm;
        G4double vtx_z_offset = vtx_offset.z()*mm;
        G4LorentzVector xvtx(pos.x()+vtx_x_offset, pos.y()+vtx_y_offset, pos.z()+vtx_z_offset, pos.t());
        
        if (! CheckVertexInsideWorld(xvtx.vect()*mm))
        { 
        std::cout << "WARNING: tried to generate vertex outside of world volume!" << std::endl;
        std::cout << "WARNING: position was (" << pos.x() << ", "<<  pos.y() << ", " << pos.z() << ", " << pos.t() << ")" << std::endl;
        continue;
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
