#ifndef HepMCPrimaryGeneratorAction_HH
#define HepMCPrimaryGeneratorAction_HH


#include "G4RunManager.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include <HepMC3/Print.h>
#include "globals.hh"
#include <TLorentzVector.h>

class G4GeneralParticleSource;
class G4Event;


class HepMCPrimaryGeneratorAction {
    public:
        HepMCPrimaryGeneratorAction(G4GeneralParticleSource* gps);
        ~HepMCPrimaryGeneratorAction();

        void GeneratePrimaryVertex(G4Event* anEvent, G4ThreeVector vtx_offset = G4ThreeVector(0,0,0), G4bool place_in_decay_volume = false);
        void LoadFile(G4String filename, G4bool override, G4bool useHepMC2);

    private:
        G4GeneralParticleSource* fGPS;
        
        G4String HepMCFileName{""};
        HepMC3::Reader* asciiInput = nullptr;
        G4bool isFileLoaded{false};
        
        std::shared_ptr<HepMC3::GenEvent> GenerateHepMCEvent();
        G4bool CheckVertexInsideWorld (const G4ThreeVector& pos) const;
        void HepMC2G4(const std::shared_ptr<HepMC3::GenEvent> hepmcevt, G4Event* g4event, G4ThreeVector vtx_offset = G4ThreeVector(0,0,0), G4bool place_in_decay_volume = false);
        
};


#endif