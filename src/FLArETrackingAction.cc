#include "FLArETrackingAction.hh"
#include "FLArETrackInformation.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"

FLArETrackingAction::FLArETrackingAction() : G4UserTrackingAction() {;}

void FLArETrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
}

void FLArETrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  if (aTrack->GetParentID()==0) {
    if (aTrack->GetParticleDefinition()->GetPDGEncoding()==111) {
      // This is a primary pizero
      G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
      if (secondaries) {
        size_t nSeco = secondaries->size();
        if (nSeco>0) {
          for (size_t i=0; i<nSeco; ++i) {
            FLArETrackInformation* info =  new FLArETrackInformation();
            info->SetTrackIsFromPrimaryPizero(1);
            (*secondaries)[i]->SetUserInformation(info);
          }
        }
      } 
    } else if (aTrack->GetTrackID()==1 &&
               (abs(aTrack->GetParticleDefinition()->GetPDGEncoding())==15 ||
                abs(aTrack->GetParticleDefinition()->GetPDGEncoding())==13)) {
      // This is a primary tau- or mu (lepton that can decay)
      G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
      if (secondaries) {
        size_t nSeco = secondaries->size();
        if (nSeco>0) {
          for (size_t i=0; i<nSeco; ++i) {
            if ((*secondaries)[i]->GetCreatorProcess()->GetProcessName()=="Decay") {
              FLArETrackInformation* info =  new FLArETrackInformation();
              info->SetTrackIsFromPrimaryLepton(1);
              (*secondaries)[i]->SetUserInformation(info);
            }
          }
        }
      }
    }
  } else if (aTrack->GetParentID()==1 && aTrack->GetCreatorProcess()->GetProcessName()=="Decay") {
    // This is a pizero decayed from FSL (tau-)
    if (aTrack->GetParticleDefinition()->GetPDGEncoding()==111) {
      G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
      if (secondaries) {
        size_t nSeco = secondaries->size();
        if (nSeco>0) {
          for (size_t i=0; i<nSeco; ++i) {
            FLArETrackInformation* info =  new FLArETrackInformation();
            info->SetTrackIsFromFSLPizero(1);
            (*secondaries)[i]->SetUserInformation(info);
          }
        }
      }
    }
  }
  //FLArETrackInformation* aTrackInfo = (FLArETrackInformation*)(aTrack->GetUserInformation());
  //if (aTrackInfo) {
  //  if (aTrackInfo->IsTrackFromPrimaryTau() | aTrackInfo->IsTrackFromPrimaryPizero()) {
  //    std::cout<<aTrack->GetParentID()<<" "<<aTrack->GetParticleDefinition()->GetPDGEncoding()<<std::endl;
  //    aTrackInfo->Print();
  //  }
  //}
}
