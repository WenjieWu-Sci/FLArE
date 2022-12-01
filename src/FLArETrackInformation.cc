#include "FLArETrackInformation.hh"

G4ThreadLocal G4Allocator<FLArETrackInformation> *
                                   aTrackInformationAllocator = 0;

FLArETrackInformation::FLArETrackInformation()
  : G4VUserTrackInformation()
{
  fFromPrimaryPizero = 0;
  fFromFSLPizero = 0;
  fFromPrimaryTau = 0;
}

FLArETrackInformation::FLArETrackInformation(const G4Track* aTrack) 
  : G4VUserTrackInformation()
{
  fFromPrimaryPizero = 0;
  fFromFSLPizero = 0;
  fFromPrimaryTau = 0;
}

FLArETrackInformation::~FLArETrackInformation()
{;}

FLArETrackInformation& FLArETrackInformation
::operator =(const FLArETrackInformation& aTrackInfo)
{
  fFromPrimaryPizero = aTrackInfo.fFromPrimaryPizero;
  fFromFSLPizero = aTrackInfo.fFromFSLPizero;
  fFromPrimaryTau = aTrackInfo.fFromPrimaryTau;

  return *this;
}

void FLArETrackInformation::Print() const
{
    G4cout << "Is from primary pizero " << fFromPrimaryPizero << G4endl;
    G4cout << "Is from final state lepton decay pizero " << fFromFSLPizero << G4endl;
    G4cout << "Is from primary tau    " << fFromPrimaryTau << G4endl;
}

