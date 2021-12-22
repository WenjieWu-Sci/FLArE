#ifndef GENIEPrimaryGeneratorAction_HH
#define GENIEPrimaryGeneratorAction_HH

#include <TFile.h>
#include <TTree.h>
#include "globals.hh"
#include "Framework/Ntuple/NtpMCTreeHeader.h"
#include "Framework/Ntuple/NtpMCEventRecord.h"

class G4GeneralParticleSource;
class G4Event;

using namespace genie;

class GENIEPrimaryGeneratorAction {
  public:
    GENIEPrimaryGeneratorAction(G4GeneralParticleSource*);
    ~GENIEPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent, G4String filename, G4int startIdx);
    
  private:
    G4GeneralParticleSource* fGPS;

    G4String ghepFileName;
    G4int ghepEvtStartIdx;
    TFile* ghep_file;
    TTree* ghep_tree;
    NtpMCTreeHeader* thdr;
    NtpMCEventRecord* mcrec;
};

#endif
