#include "SteppingAction.hh"
#include "RunAction.hh"

#include <G4Step.hh>
#include <G4Electron.hh>
#include <G4TrackStatus.hh>
#include <G4SystemOfUnits.hh>

#include <TMath.h>

SteppingAction::SteppingAction(RunAction* runAction)
  : fRunAction(runAction)
{
}

void SteppingAction::UserSteppingAction(const G4Step* aStep) {
 
  //TrackLiveDebugging(aStep);
	
  G4Track* aTrack = aStep->GetTrack();
  G4ThreeVector post_pos = aStep->GetPostStepPoint()->GetPosition();
  
  // if the track is out of the active volumes/area, kill this track
  G4VPhysicalVolume* volume = aStep->GetPostStepPoint()->GetTouchable()->GetVolume();
  
  if( volume==nullptr ) aTrack->SetTrackStatus(G4TrackStatus::fStopAndKill);
  else if( volume->GetName() == "world" ){       
	
	G4double endFLArE = 4 * m;
	G4double totYokeX = 4.5 * m;
	G4double totYokeY = 3 *m;
	
	//if before end of FLArE, kill
	if( post_pos.z() < endFLArE ) aTrack->SetTrackStatus(G4TrackStatus::fStopAndKill); 
	
	//if after enf of FLArE, kill if outside FASER magnet cross-section
	else if ( TMath::Abs(post_pos.x()) > totYokeX/2. || TMath::Abs(post_pos.y()) > totYokeY/2. )
		aTrack->SetTrackStatus(G4TrackStatus::fStopAndKill);
  }
  
  /*// if the track is out of the active volumes, kill this track
  G4VPhysicalVolume* volume = aStep->GetPostStepPoint()->GetTouchable()->GetVolume();
  std::string active_volumes[9] = {"lArBox",
                                   "TPCModulePhysical",
                                   "CryGapPhysical",
                                   "HadCatcherPhysical",
                                   "HadCalXCellPhysical",
                                   "HadCalYCellPhysical",
                                   "MuonFinderPhysical",
                                   "MuonFinderXCellPhysical",
                                   "MuonFinderYCellPhysical"};
  std::string active_logical[2] = {"HadCatcherLogical",
                                   "MuonFinderLogical"};

  // it will break the track if there is a gap between active volumes,
  // because the track in between active volumes will neither be in active_volumes, nor in active_logical,
  // it will be a track in world and be killed
  if (volume != nullptr) {
    if (std::find(std::begin(active_volumes), std::end(active_volumes), volume->GetName())==std::end(active_volumes)) {
      G4LogicalVolume* mother_logical = volume->GetMotherLogical();
      if (mother_logical != nullptr) {
        // for tracks in HadCatcher and MuonFinder absorber
        if (std::find(std::begin(active_logical), std::end(active_logical), mother_logical->GetName())
            ==std::end(active_logical)) {
          aTrack->SetTrackStatus(G4TrackStatus::fStopAndKill); 
        }
      } else {
        // for tracks in world
        aTrack->SetTrackStatus(G4TrackStatus::fStopAndKill); 
      }
    }
  }*/
}

void SteppingAction::TrackLiveDebugging(const G4Step* step){

	G4Track* track = step->GetTrack();

	if( track->GetTrackStatus() != fAlive && track->GetTrackStatus() != fStopButAlive) return;

	G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4LogicalVolume* volume_after = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

	G4ThreeVector pre_pos = step->GetPostStepPoint()->GetPosition();
	G4ThreeVector post_pos = step->GetPostStepPoint()->GetPosition();
	G4double edep = step->GetTotalEnergyDeposit();
	 
	int PDG = track->GetParticleDefinition()->GetPDGEncoding();
	G4String ParticleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();
	int TID = track->GetTrackID();
	int SID = step->GetTrack()->GetCurrentStepNumber();

	std::cout << "Track " << TID << " - " << "PDG " << PDG << " " << ParticleName << std::endl;
	std::cout << "stepping... " << SID << " edep" << edep << std::endl;
	std::cout << "(" << pre_pos.x() << "," << pre_pos.y() << "," << pre_pos.z() << ") in " << volume->GetName();
	std::cout << " ---> "  << "(" << post_pos.x() << "," << post_pos.y() << "," << post_pos.z() << ") in " << volume_after->GetName() << std::endl;
}
