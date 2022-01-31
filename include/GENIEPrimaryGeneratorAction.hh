#ifndef GENIEPrimaryGeneratorAction_HH
#define GENIEPrimaryGeneratorAction_HH

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "globals.hh"
#include "Framework/Ntuple/NtpMCTreeHeader.h"
#include "Framework/Ntuple/NtpMCEventRecord.h"
#include "Framework/Interaction/InteractionType.h"
#include "Framework/Interaction/ScatteringType.h"

class G4GeneralParticleSource;
class G4Event;

using namespace genie;

class GENIEPrimaryGeneratorAction {
  public:
    GENIEPrimaryGeneratorAction(G4GeneralParticleSource*);
    ~GENIEPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent, G4String filename, G4int startIdx);
    InteractionType_t InteractionTypeId() { return int_type; };
    ScatteringType_t  ScatteringTypeId()  { return scattering_type; };
    G4int             FSLPDG() { return fslPDG; };
    TLorentzVector    FSLP4()  { return fslP4; };
    TLorentzVector    FSLX4()  { return fslX4; };
    
  private:
    G4GeneralParticleSource* fGPS;

    G4String ghepFileName;
    G4int ghepEvtStartIdx;
    TFile* ghep_file;
    TTree* ghep_tree;
    NtpMCTreeHeader* thdr;
    NtpMCEventRecord* mcrec;

    // https://internal.dunescience.org/doxygen/Generator_2src_2Framework_2Interaction_2InteractionType_8h_source.html#l00033
    // https://internal.dunescience.org/doxygen/ScatteringType_8h_source.html
    InteractionType_t int_type;
    ScatteringType_t  scattering_type;
    G4int fslPDG;
    TLorentzVector fslP4;
    TLorentzVector fslX4;
};

#endif
