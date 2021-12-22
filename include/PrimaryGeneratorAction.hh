#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

#include <TFile.h>
#include <TTree.h>
#include "Framework/Ntuple/NtpMCTreeHeader.h"
#include "Framework/Ntuple/NtpMCEventRecord.h"

class G4ParticleGun;
class G4GeneralParticleSource;
class PrimaryGeneratorMessenger;
//class NtpMCEventRecord;

using namespace genie;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
//    static PrimaryGeneratorAction* GetInstance();
//    void BeginOfRun();
//    void EndOfRun();
    void GeneratePrimaries(G4Event* anEvent) override;

    void setGenieInputFile(G4String val) { 
      ghepFileName = val; 
      std::cout<<"PrimaryGeneratorAction.hh "<<ghepFileName<<std::endl;
    }
    void setGenieStartEvt(G4int val) { ghepEvtStartIdx = val; }
    void setGenieStopEvt(G4int val) { ghepEvtStopIdx = val; }

  private:
//    static PrimaryGeneratorAction* GeneratorInstance;
    PrimaryGeneratorMessenger* genMessenger;

    G4ParticleGun* fGun;
    G4GeneralParticleSource* fGPS;

    G4String ghepFileName;
    G4int ghepEvtStartIdx;
    G4int ghepEvtStopIdx;
    TFile* ghep_file;
    TTree* ghep_tree;
    NtpMCTreeHeader* thdr;
    NtpMCEventRecord* mcrec;
};

#endif
