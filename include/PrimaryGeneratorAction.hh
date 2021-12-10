#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>

#include <TFile.h>
#include <TTree.h>
#include "Framework/Ntuple/NtpMCTreeHeader.h"
#include "Framework/Ntuple/NtpMCEventRecord.h"

class G4ParticleGun;
class G4GeneralParticleSource;
//class NtpMCEventRecord;

using namespace genie;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
    void GeneratePrimaries(G4Event* anEvent) override;

  private:
    G4ParticleGun* fGun;
    G4GeneralParticleSource* fGPS;

    TFile* ghep_file;
    TTree* ghep_tree;
    NtpMCTreeHeader* thdr;
    NtpMCEventRecord* mcrec;
};

#endif
