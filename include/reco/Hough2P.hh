#ifndef HOUGH2P_HH
#define HOUGH2P_HH

class TH2F;

namespace hough {
  class Hough2P {
    public:
      Hough2P();
      ~Hough2P();

      /// Make the Hough map for the hit list h.
      /// \param h - The list of hits/spacepoint
      void Map(const TH2F& h);

    private:
      TH2F* fMapZX;
      TH2F* fMapZY;
  };
}

#endif
