#include "SteppingAction.hh"
#include "RunAction.hh"

#include <G4Step.hh>
#include <G4Electron.hh>
#include <G4TrackStatus.hh>

#include <algorithm>

SteppingAction::SteppingAction(RunAction* runAction)
  : fRunAction(runAction)
{
}

void SteppingAction::UserSteppingAction(const G4Step* aStep) {
  G4Track* aTrack = aStep->GetTrack();
  // if the track is out of the active volumes, kill this track
  G4VPhysicalVolume* volume = aStep->GetPostStepPoint()->GetTouchable()->GetVolume();
  std::string active_volumes[8] = {"lArBox",
                                   "CryGapPhysical",
                                   "HadCatcherPhysical",
                                   "HadCalXCellPhysical",
                                   "HadCalYCellPhysical",
                                   "MuonFinderPhysical",
                                   "MuonFinderXCellPhysical",
                                   "MuonFinderYCellPhysical"};
  std::string active_logical[2] = {"HadCatcherLogical",
                                   "MuonFinderLogical"};

  // caveat
  // it will break the track if there is a gap between active volumes,
  // because the track in between active volumes will neither be in active_volumes, nor in active_logical,
  // it will be a track in world and be killed
  if (volume != nullptr) {
    if (std::find(std::begin(active_volumes), std::end(active_volumes), volume->GetName())==std::end(active_volumes)) {
      G4LogicalVolume* mother_logical = volume->GetMotherLogical();
      if (mother_logical != nullptr) {
        // for tracks in HadCatcher and MuonFinder absorber
        if (std::find(std::begin(active_logical), std::end(active_logical), mother_logical->GetName())==std::end(active_logical)) {
          aTrack->SetTrackStatus(G4TrackStatus::fStopAndKill); 
        }
      } else {
        // for tracks in world
        aTrack->SetTrackStatus(G4TrackStatus::fStopAndKill); 
      }
    }
  }
}
