////////////////////////////////////////////////////////////////////////
// \file    CircularFit.cc
// \brief   Classes to perform circular fits using analytical methods
//          for momentum estimation in magnetic fields
// \author  M. Vicenzi (mvicenzi@bnl.gov)
////////////////////////////////////////////////////////////////////////

#include "reco/CircleFit.hh"
#include "geometry/GeometricalParameters.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <vector>
#include <algorithm>

#include <TMath.h>
#include <TGraph.h>
#include <TF1.h>

namespace circularfitter {
  CircleFit::CircleFit(const std::vector<double> x, const std::vector<double> z)
  {
    fXc = -9999;
    fZc = -9999;
    fR = -9999;
    fChi2 = -9999;

    // This code follows an analytical method for circle fitting (Modified Least Squares method)
    // Ref: https://doi.org/10.1109/TIM.2003.820472
    // using paper formulas with y -> z
    
    if( x.size() != z.size() ) { fStatus = 1; return; }	
    int n = x.size(); 

    double sumx = 0, sumz = 0;                            // linear    terms
    double sumx2 = 0, sumz2 = 0, sumxz = 0;               // quadratic terms
    double sumxz2 = 0, sumx2z = 0, sumx3 = 0, sumz3 = 0;  // cubic     terms

    for (int i = 0; i < n; i++) {
      double xp = x.at(i);
      double zp = z.at(i);
      sumx += xp;
      sumz += zp;
      sumx2 += xp * xp;
      sumz2 += zp * zp;
      sumxz += xp * zp;
      sumxz2 += xp * zp * zp;
      sumx2z += xp * xp * zp;
      sumx3 += xp * xp * xp;
      sumz3 += zp * zp * zp;
    }

    double a = n * sumx2 - sumx * sumx;
    double b = n * sumxz - sumx * sumz;
    double c = n * sumz2 - sumz * sumz;
    double d = 0.5 * (n * sumxz2 - sumx * sumz2 + n * sumx3 - sumx * sumx2);
    double e = 0.5 * (n * sumx2z - sumz * sumx2 + n * sumz3 - sumz * sumz2);

    if (a * c - b * b == 0.) { fStatus = 2; return; }
 
    //center of the circle
    fXc = (d * c - b * e) / (a * c - b * b);
    fZc = (a * e - b * d) / (a * c - b * b);

    //radius of the circle
    fR = 0;
    for (int i = 0; i < n; i++) {
      double xp = x.at(i);
      double zp = z.at(i);
      double r2 = (xp - fXc) * (xp - fXc) + (zp - fZc) * (zp - fZc);

      fR += TMath::Sqrt(r2);
    }

    fR /= n;

    //compute chi2
    fChi2 = 0.0;
    for (int i = 0; i < n; i++) {
      double dist = TMath::Sqrt((z.at(i) - fZc) * (z.at(i) - fZc) + (x.at(i) - fXc) * (x.at(i) - fXc) ) - fR;
      fChi2 += dist*dist;
    }
  
    fChi2 /= n;
    fStatus = 0;
  }

  CircleFit::~CircleFit() 
  {
  }

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

  LineFit::LineFit(const std::vector<double> z, const std::vector<double> y)
  {
    fP0 = -9999;
    fP1 = -9999;
    fCosDip = -9999;
    
    if( z.size() != y.size() ) { fStatus = 1; return; }
    int n = z.size(); 	
    
    double S1 = 0.;
    double SZ = 0.;
    double SY = 0.;
    double SYZ = 0.;
    double SZZ = 0.;

    for (int i = 0; i < n; i++) {
      S1 += 1.;
      SZ += z.at(i);
      SY += y.at(i);
      SYZ += z.at(i) * y.at(i);
      SZZ += z.at(i) * z.at(i);
    }
    
    double D = S1 * SZZ - SZ * SZ;
    
    if (D == 0.) { fStatus = 2; return; }

    fP0 = (SY * SZZ - SZ * SYZ) / D;  // i.p. at x = 0
    fP1 = (S1 * SYZ - SZ * SY) / D;   // tg(theta)
    fCosDip = 1./TMath::Sqrt(1+fP1*fP1);    

    fChi2 = 0.;
    for (int i = 0; i < n; i++) {
      fChi2 += (y.at(i) - fP0 - fP1 * z.at(i)) * (y.at(i) - fP0 - fP1 * z.at(i));
    }
    
    fChi2 /= n;
    fStatus = 0;
  }

  line LineFit::GetLine()
  {
    line l;
    l.q = fP0;
    l.m = fP1;
    return l;
  }

  LineFit::~LineFit() 
  {
  }

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------

  void CircleExtractor::splitHits(std::vector<double> magnetZs,
                                  std::vector<double> x, std::vector<double> y, std::vector<double> z, 
                                  std::vector<std::vector<hit>> &hits)
  {
    // first step is to sort the separators
    std::sort(magnetZs.begin(), magnetZs.end());    
    hits.resize(magnetZs.size()+1);

    //scan all recorded hits
    for(unsigned int i=0; i<z.size(); i++){
      
      hit h;
      h.x = x.at(i); h.y = y.at(i); h.z = z.at(i);
      
      bool assigned = false;
      for(unsigned int j=0; j<magnetZs.size(); j++){  
        if( h.z < magnetZs[j] ){ //if the hit comes before, save it
          hits[j].push_back(h);
          assigned = true;
          break;   
        }
      }     
     
      //if it is not before any magnet
      if(!assigned)
        hits[hits.size()-1].push_back(h); 
    }
  }

  CircleExtractor::CircleExtractor(const std::vector<double> x, const std::vector<double> y, const std::vector<double> z)
  {
   
    // two cases depending on the magnet design:
    // there can be many magnets, so many magnet positions
    // get the magnet position(s) for each geometry (SAMURAI or CrystalPulling design) first
    G4String opt = GeometricalParameters::Get()->GetSpectrometerMagnetOption();
    G4int nmag = 0;
    std::vector<double> zin;
    std::vector<double> zout;
    
    // STEP 1: get the geometry
    if( opt == GeometricalParameters::magnetOption::SAMURAI ){

      // there is only one magnet, so one magnet position
      nmag = 1;
      fzpos.push_back(GeometricalParameters::Get()->GetMagnetZPosition());
      double size = GeometricalParameters::Get()->GetMagnetTotalSizeZ();
      zin.push_back(fzpos[0] - size/2.);  //begin of magnet window
      zout.push_back(fzpos[0] + size/2.); //end of magnet window
    
    }else if ( opt == GeometricalParameters::magnetOption::CrystalPulling ){
      
      double zcenter = GeometricalParameters::Get()->GetMagnetZPosition();
      double size = GeometricalParameters::Get()->GetSpectrometerMagnetLengthZ();
      double spacing = GeometricalParameters::Get()->GetSpectrometerMagnetSpacing();
      nmag = GeometricalParameters::Get()->GetNSpectrometerMagnets();
      for(int i=0; i<nmag; i++){
	double offset = (i-0.5*(nmag-1))*(spacing+size);
	fzpos.push_back( zcenter + offset );
        zin.push_back( zcenter + offset - size/2. );  //begin of magnet window
        zout.push_back( zcenter + offset + size/2. ); //end of magnet window
        //G4cout << "center zin zout " << zpos[i] << " " << zin[i] << " " << zout[i] <<G4endl;
      }
    }
    else {  G4cout << "ERROR: unknown FASER2 spectrometer magnet option!" << G4endl; }

    // STEP 2: split hits in before/after each magnet
    std::vector<std::vector<hit>> hits;
    splitHits( fzpos, x, y, z, hits);
       
    // STEP 3: apply the procedure for each magnet available
    // Each magnet uses its own pre and post hits (i and i+1)   
    // field is along Y: look in z-x plan  (for now: FIXME??)
    for(int i=0; i<nmag; i++){ 

      std::vector<double> zpre; std::vector<double> xpre;
      std::vector<double> zpost; std::vector<double> xpost;
       
      for( unsigned int j=0; j< hits[i].size(); j++){
        zpre.push_back( hits[i][j].z );
        xpre.push_back( hits[i][j].x );
      }   
      for( unsigned int j=0; j< hits[i+1].size(); j++){
        zpost.push_back( hits[i+1][j].z );
        xpost.push_back( hits[i+1][j].x );
      }
      
      //and now the actual circle extraction 
      computeCircle( zpre, xpre, zpost, xpost, zin[i], zout[i]);   
    }
  }

  void CircleExtractor::computeCircle(std::vector<double> zpre, std::vector<double> xpre, std::vector<double> zpost,
                                      std::vector<double> xpost, double Zin, double Zout)
  {
      LineFit lf_pre(zpre,xpre);
      LineFit lf_post(zpost,xpost);
      line pre = lf_pre.GetLine();
      line post = lf_post.GetLine();
      fpre.push_back( pre );
      fpost.push_back( post );
    
      double Xin = extrapolateLine(Zin,pre);
      double Xout = extrapolateLine(Zout,post);
      std::pair<double,double> p = getPerpLineIntersection(Zin,Xin,Zout,Xout,pre,post);
      fXc.push_back( p.second );
      fZc.push_back( p.first );

      double R1 = getR(Zin,Xin,p.first,p.second);
      double R2 = getR(Zout,Xout,p.first,p.second);
      fR1.push_back( R1 );
      fR2.push_back( R2 );
  }
   
  double CircleExtractor::extrapolateLine(double z, line l)
  { 
    return l.q + z*l.m; 
  }
  
  std::pair<double,double> CircleExtractor::getPerpLineIntersection(double zin, double xin, double zout, double xout, line l1, line l2)
  {
    double m1 = l1.m;
    double m2 = l2.m;
    
    double z = m1*m2/(m1-m2)*(xout-xin) + m1*zout/(m1-m2) - m2*zin/(m1-m2);
    double x1 = xin - 1./m1*(z-zin);
    double x2 = xout - 1./m2*(z-zout);

    return std::make_pair(z,(x1+x2)/2.);
  }
  
  double CircleExtractor::getR(double za, double xa, double zb, double xb)
  {
    return TMath::Sqrt( (za-zb)*(za-zb) + (xa-xb)*(xa-xb) );
  }

  std::vector<double> CircleExtractor::GetR(){
    std::vector<double> R;
    for( unsigned int i=0; i<fR1.size(); i++)
	R.push_back( (fR1[i]+fR2[i])/2.);
    return R;
  }

  CircleExtractor::~CircleExtractor() 
  {
    fpre.clear();
    fpost.clear();
    fXc.clear();
    fZc.clear();
    fR1.clear();
    fR2.clear();
  }

// ----------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------

  ParabolicFit::ParabolicFit(std::vector<double> z, std::vector<double> x, double r0)
  {	
    TGraph *g = new TGraph();
    
    for(unsigned int i=0; i<z.size(); i++){  g->SetPoint(i,z.at(i),x.at(i)); }

    // parabolic in x-z plane: x = a + b*z + x*z2	
    TF1 *f = new TF1("f","[0]+[1]*x+[2]*x*x",z.front()-100,z.back()+100);
    f->SetParameter(0,0);
    f->SetParameter(1, (x.at(1)-x.at(0))/(z.at(1)-z.at(0)) );
    f->SetParameter(2, 1./(2*r0) );
	
    g->Fit("f", "QN");
	
    fA = f->GetParameter(0);
    fB = f->GetParameter(1);	
    double c = f->GetParameter(2);

    fR = 1./(2*c);
  }
  
  ParabolicFit::~ParabolicFit() 
  {
  }

}
