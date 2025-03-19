#ifndef GENIEPrimaryGeneratorAction_HH
#define GENIEPrimaryGeneratorAction_HH

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

class GENIEPrimaryGeneratorAction {
  public:
    GENIEPrimaryGeneratorAction(G4GeneralParticleSource*);
    ~GENIEPrimaryGeneratorAction();

  public:
    
    void GeneratePrimaries(G4Event* anEvent, G4String filename, G4int startIdx, G4int nuVtxOpt);
    void ShootParticle(G4Event* anEvent, G4int pdg, TLorentzVector p4);
    
    G4int DecodeInteractionType(bool cc, bool nc, bool em);
    G4int DecodeScatteringType(bool qel, bool res, bool dis, bool coh, bool dfr, 
			       bool imd, bool imdanh, bool mec, bool nuel,
	                       bool singlek, bool amnugamma);

    G4int             NeuIdx() { return neuIdx; };
    G4int             NeuPDG() { return neuPDG; };
    TLorentzVector    NeuP4()  { return neuP4; };
    TLorentzVector    NeuX4()  { return neuX4; };
    G4int InteractionTypeId() { return int_type; };
    G4int ScatteringTypeId()  { return scattering_type; };
    G4int             FSLPDG() { return fslPDG; };
    TLorentzVector    FSLP4()  { return fslP4; };
    G4double          GetW()   { return W; };
    
  private:
    G4GeneralParticleSource* fGPS;

    G4String gstFileName;
    G4int eventCounter;
    G4int gstEvtStartIdx;
    TFile* gst_file;
    TTree* gst_tree;

    G4int neuIdx;
    G4int neuPDG;
    TLorentzVector neuP4;
    TLorentzVector neuX4;
    G4int int_type;
    G4int scattering_type;
    G4int fslPDG;
    TLorentzVector fslP4;
    G4double W;
};

#endif
