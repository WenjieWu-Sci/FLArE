#include <algorithm>
#include <vector>

#include "LArBoxSD.hh"
#include "FLArETrackInformation.hh"

#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4VProcess.hh>
#include <G4ProcessManager.hh>
#include <G4StepPoint.hh>

LArBoxSD::LArBoxSD(G4String name) : G4VSensitiveDetector(name) {
  collectionName.insert("lar_box");
}

G4bool LArBoxSD::ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist) {
  G4Track* aTrack = aStep->GetTrack();

  // enumerator: fAlive, fStopButAlive, fStopAndKill, fKillTrackAndSecondaries, fSuspend, fPostponeToNextEvent
  // https://apc.u-paris.fr/~franco/g4doxy/html/G4TrackStatus_8hh.html#734825af9cdc612606614fdce0545157
  // http://geant4.in2p3.fr/2005/Workshop/ShortCourse/session1/M.Asai.pdf
  G4int TrackStatus = aTrack->GetTrackStatus();
  //G4cout<<"debug (track status): "<<TrackStatus<<G4endl;

  G4ThreeVector TrackVertex = aTrack->GetVertexPosition();
  G4double TrackLength      = aTrack->GetTrackLength();

  G4int ParticleName = aTrack->GetParticleDefinition()->GetPDGEncoding();
  G4double ParticleMass = aTrack->GetParticleDefinition()->GetPDGMass();
  G4int PID          = aTrack->GetParentID();
  G4int TID          = aTrack->GetTrackID();
  G4int Stepno       = aTrack->GetCurrentStepNumber();
  G4String CreatorProcess;
  if (PID> 0) {
    CreatorProcess = aTrack->GetCreatorProcess()->GetProcessName();
  } else {
    CreatorProcess = "PrimaryParticle";
  }
  G4StepPoint* PreStep          = aStep->GetPreStepPoint();
  G4ThreeVector PreStepPosition = PreStep->GetPosition();
  G4ThreeVector InitMomentum    = PreStep->GetMomentum();
  G4double InitKinEne           = PreStep->GetKineticEnergy();
  G4StepPoint* PostStep         = aStep->GetPostStepPoint();
  G4ThreeVector PostStepPosition= PostStep->GetPosition();
  G4double StepLength           = aStep->GetStepLength();
  G4String ProcessName;
  if (PostStep->GetProcessDefinedStep()) {
    ProcessName = PostStep->GetProcessDefinedStep()->GetProcessName();
  } else {
    // https://apc.u-paris.fr/~franco/g4doxy/html/G4StepPoint_8icc-source.html#l00181
    // If the pointer is 0, this means the Step is defined
    // by the user defined limit in the current volume.
    ProcessName = "UserDefinedLimit";
  }

//  if (CreatorProcess=="PrimaryParticle") {
//    G4cout<<"debug (create process): "<<CreatorProcess<<G4endl;
//    G4cout<<ParticleName<<" "<<PID<<" "<<TID<<" "<<Stepno<<G4endl;
//  }

  // extra info
  G4double edep      = aStep->GetTotalEnergyDeposit();
  G4ThreeVector pos  = 0.5*(PreStepPosition + PostStepPosition);
  G4String PosVolume = PostStep->GetPhysicalVolume()->GetName();
  G4int StepStatus   = PostStep->GetStepStatus();

  FLArETrackInformation* aTrackInfo = (FLArETrackInformation*)(aTrack->GetUserInformation());
  G4int trackIsFromPrimaryPizero = 0;
  G4int trackIsFromFSLPizero = 0;
  G4int trackIsFromPrimaryTau = 0;
  if (aTrackInfo) {
    trackIsFromPrimaryPizero = aTrackInfo->IsTrackFromPrimaryPizero();
    trackIsFromFSLPizero = aTrackInfo->IsTrackFromFSLPizero();
    trackIsFromPrimaryTau = aTrackInfo->IsTrackFromPrimaryTau();
  }

  // Fill hit information
  LArBoxHit* hit = new LArBoxHit();
  hit->SetTrackStatus(TrackStatus);
  hit->SetTrackVertex(TrackVertex);
  hit->SetTrackLength(TrackLength);
  hit->SetParticle(ParticleName);
  hit->SetParticleMass(ParticleMass);
  hit->SetPID(PID);
  hit->SetTID(TID);
  hit->SetStepNo(Stepno);
  hit->SetPreStepPosition(PreStepPosition);
  hit->SetPostStepPosition(PostStepPosition);
  hit->SetInitMomentum(InitMomentum);
  hit->SetInitKinEnergy(InitKinEne);
  hit->SetCreatorProcess(CreatorProcess);
  hit->SetProcessName(ProcessName);
  hit->SetStepLength(StepLength);
  hit->SetVolume(PosVolume);
  hit->SetStepStatus(StepStatus);
  hit->SetTrackIsFromPrimaryPizero(trackIsFromPrimaryPizero);
  hit->SetTrackIsFromFSLPizero(trackIsFromFSLPizero);
  hit->SetTrackIsFromPrimaryTau(trackIsFromPrimaryTau);

  hit->SetEdep(edep);
  hit->SetEdepPosition(pos);

  fHitsCollection->insert(hit);

  return true;
}

void LArBoxSD::Initialize(G4HCofThisEvent* hcof) {
  fHitsCollection = new LArBoxHitsCollection(SensitiveDetectorName, collectionName[0]);
  if (fHitsCollectionId < 0) {
    fHitsCollectionId = G4SDManager::GetSDMpointer()->GetCollectionID(GetName() + "/" + collectionName[0]);
  }
  hcof->AddHitsCollection(fHitsCollectionId, fHitsCollection);
}
