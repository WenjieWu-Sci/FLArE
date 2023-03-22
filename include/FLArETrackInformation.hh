#ifndef FLARETRACKINFORMATION_HH
#define FLARETRACKINFORMATION_HH

#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class FLArETrackInformation : public G4VUserTrackInformation
{
  public:
    FLArETrackInformation();
    FLArETrackInformation(const G4Track* aTrack);
    //FLArETrackInformation(const FLArETrackInformation* aTrackInfo);
    virtual ~FLArETrackInformation();

    inline void *operator new(size_t);
    inline void operator delete(void *aTrackInfo);

    FLArETrackInformation& operator =(const FLArETrackInformation& right);

    virtual void Print() const;

  public:
    inline void SetTrackIsFromPrimaryPizero(G4int i) {fFromPrimaryPizero = i;}
    inline void SetTrackIsFromFSLPizero(G4int i) {fFromFSLPizero = i;}
    inline void SetTrackIsFromPrimaryLepton(G4int i) {fFromPrimaryLepton = i;}
    inline G4int IsTrackFromPrimaryPizero() const {return fFromPrimaryPizero;}
    inline G4int IsTrackFromFSLPizero() const {return fFromFSLPizero;}
    inline G4int IsTrackFromPrimaryLepton() const {return fFromPrimaryLepton;}

  private:
    G4int fFromPrimaryPizero;
    G4int fFromFSLPizero;
    G4int fFromPrimaryLepton;
};

extern G4ThreadLocal
 G4Allocator<FLArETrackInformation> * aTrackInformationAllocator;

inline void* FLArETrackInformation::operator new(size_t)
{
  if(!aTrackInformationAllocator)
    aTrackInformationAllocator = new G4Allocator<FLArETrackInformation>;
  return (void*)aTrackInformationAllocator->MallocSingle();
}

inline void FLArETrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator->FreeSingle((FLArETrackInformation*)aTrackInfo);}

#endif
