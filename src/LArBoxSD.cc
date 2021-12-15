#include "LArBoxSD.hh"

#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4VProcess.hh>
#include <G4ProcessManager.hh>
#include <G4StepPoint.hh>

LArBoxSD::LArBoxSD(G4String name) : G4VSensitiveDetector(name) {
  collectionName.insert("lar_box");
}

G4bool LArBoxSD::ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist) {
  /*************************
   * Records:
   * Particle type, Parent ID, track ID, Step No,
   * Create process, Process name
   * ***********************/
  G4Track* aTrack = aStep->GetTrack();
  G4int TrackStatus = aTrack->GetTrackStatus();
  //G4cout<<"debug (track status): "<<TrackStatus<<G4endl;
  G4ThreeVector TrackVertex = aTrack->GetVertexPosition();
  G4double TrackLength      = aTrack->GetTrackLength();

  G4int ParticleName = aTrack->GetParticleDefinition()->GetPDGEncoding();
  G4int PID = aTrack->GetParentID();
  G4int TID = aTrack->GetTrackID();
  G4int Stepno = aTrack->GetCurrentStepNumber();
  G4String CreatorProcess;
  if (PID> 0) {
    CreatorProcess = aTrack->GetCreatorProcess()->GetProcessName();
  } else {
    CreatorProcess = "PrimaryParticle";
  }
  //G4cout<<"debug (create process): "<<CreatorProcess<<G4endl;
  G4StepPoint* PreStep          = aStep->GetPreStepPoint();
  G4ThreeVector PreStepPosition = PreStep->GetPosition();
  G4ThreeVector InitMomentum    = PreStep->GetMomentum();
  G4double InitKinEne           = PreStep->GetKineticEnergy();
  G4StepPoint* PostStep         = aStep->GetPostStepPoint();
  G4ThreeVector PostStepPosition= PostStep->GetPosition();
  G4String ProcessName          = PostStep->GetProcessDefinedStep()->GetProcessName();
  G4double StepLength           = aStep->GetStepLength();

  // extra info
  G4double edep      = aStep->GetTotalEnergyDeposit();
  G4ThreeVector pos  = 0.5*(PreStepPosition + PostStepPosition);
  G4String PosVolume = PostStep->GetPhysicalVolume()->GetName();
  G4int StepStatus   = PostStep->GetStepStatus();

  // Fill hit information
  LArBoxHit* hit = new LArBoxHit();
  hit->SetTrackStatus(TrackStatus);
  hit->SetTrackVertex(TrackVertex);
  hit->SetTrackLength(TrackLength);
  hit->SetParticle(ParticleName);
  hit->SetPID(PID);
  hit->SetTID(TID);
  hit->SetStepNo(Stepno);
  hit->SetPreStepPosition(PreStepPosition);
  hit->SetInitMomentum(InitMomentum);
  hit->SetInitKinEnergy(InitKinEne);
  hit->SetCreatorProcess(CreatorProcess);
  hit->SetProcessName(ProcessName);
  hit->SetStepLength(StepLength);
  hit->SetVolume(PosVolume);
  hit->SetStepStatus(StepStatus);

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
