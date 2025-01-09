#include <iostream>
#include <string>
#include <fstream>
#include <vector>

// ---------------------------------------------------------------------------------
// global definitions 
double E, w;
double x, y, z;
double xdircos, ydircos;
int N;

int xbins;
double xmin, xmax;
double xbinsize;

int ybins;
double ymin, ymax;
double ybinsize;

int Ebins;
double Emin, Emax;
double Ebinsize;

int dbins;
double dmin, dmax;
double dbinsize;

// files
std::string outpath_mu = "../background_input.root";
TFile *fin; //input
TFile *fout; //output

// ----------------------------------------------------------------------------------

void set_muon_binning(){

  // 20cm bins along X
  xbinsize = 20;
  xmin = -326.; xmax = 614.;
  xbins = int((xmax-xmin)/xbinsize);

  // 20cm bins along y
  ybinsize = 20;
  ymin = -159.; ymax = 601.;
  ybins = int((ymax-ymin)/ybinsize);

  // energy binning: 20 GeV
  Ebinsize = 20;
  Emin = 0., Emax = 4500.;
  Ebins = int((Emax-Emin)/Ebinsize);

  // x/y dircos binning: 0.01
  dbinsize = 0.01;
  dmin = -1., dmax = 1.;
  dbins = int((dmax-dmin)/dbinsize);

}

void set_neutron_binning(){

  // 20cm bins along X
  xbinsize = 20;
  xmin = -326.; xmax = 614.;
  xbins = int((xmax-xmin)/xbinsize);

  // 20cm bins along y
  ybinsize = 20;
  ymin = -159.; ymax = 601.;
  ybins = int((ymax-ymin)/ybinsize);

  // energy binning: 0.5 GeV
  Ebinsize = 0.5;
  Emin = 0., Emax = 100.;
  Ebins = int((Emax-Emin)/Ebinsize);

  // x/y dircos binning: 0.01
  dbinsize = 0.01;
  dmin = -1., dmax = 1.;
  dbins = int((dmax-dmin)/dbinsize);

}

double apply_normalization(double value){

  //input: number of mus per cm2 per GeV
  //The numbers in the files must be normalized in order to get the fluence rate (flux) for 5Lo considering:
  //- the instantaneous luminosity: 5e34 cm-2 s-1
  double n1 = 5.0e34; //cm-2 s-1
  //- the cross section of the p-p collision: 85 mb = 85e-3*1e-24 cm2
  double n2 = 85e-3*1e-24; //cm2
  //-  a normalization factor coming from the 2 step simulation approach: 2104211.0/14e14
  //   with this normalisation factor, any quantity is given per primary p-p collision.
  //double n3 = 2104211.0/14.0e14;         //CAUTION: this is for v1 files
  double n3 = (7509624.0/1.0e8)/1.0e8;     //CAUTION: this is for v2 files
  //therefore: 
  double N = n1*n2*n3;

  return N*value; // number of mus cm-2 GeV-1 s-1 (for 5Lo)
}

// ----------------------------------------------------------------------------------

void set_tree(TTree *t){

  N = t->GetEntries();

  t->SetBranchAddress("kinE", &E);
  t->SetBranchAddress("weight", &w);
  t->SetBranchAddress("x", &x);
  t->SetBranchAddress("y", &y);
  t->SetBranchAddress("z", &z);
  t->SetBranchAddress("xdircos",&xdircos);
  t->SetBranchAddress("ydircos",&ydircos);

}

void make_histos(TTree *t, std::string name){

  std::string label = "hxyE_" + name;
  std::string labeld = "hdir_" + name;
  
  std::string title = name + " (x,y,E) correlations at FPF entrance";
  std::string titled = name + " (xdircos,ydircos,E) correlations at FPF entrance";

  TH3D *h = new TH3D(label.c_str(),title.c_str(),xbins,xmin,xmax,ybins,ymin,ymax,Ebins,Emin,Emax);
  TH3D *hd = new TH3D(labeld.c_str(),titled.c_str(),dbins,dmin,dmax,dbins,dmin,dmax,Ebins,Emin,Emax);
  
  std::cout << N << " entries" << std::endl;
  std::cout << xbins << " bins (" << ybinsize << " GeV) from " << xmin << " to " << xmax << std::endl;
  std::cout << ybins << " bins (" << xbinsize << " GeV) from " << ymin << " to " << ymax << std::endl;
  std::cout << Ebins << " bins (" << Ebinsize << " GeV) from " << Emin << " to " << Emax << std::endl;
  std::cout << dbins << " bins (" << dbinsize << ") from " << dmin << " to " << dmax << std::endl;

  for (int i=0; i<N; i++){

    t->GetEntry(i);

    // discard if outside binning limits
    if ( y< ymin || y > ymax ) continue;
    if ( x< xmin || x > xmax ) continue;
    
    h->Fill(x, y, E, w);
    hd->Fill(xdircos , ydircos, E, w);

    if ( i % 100000 == 0 ) std::cout << i << " entries processed" << std::endl; 
  
  }

  // apply normalizations: we are doing this only to the xyE histogram
  // this is not important for sampling out events for the generator
  // this is important to know how many events correspond to what integrated luminosity
  for( int i=1; i<h->GetNbinsX()+1; i++){
    for( int j=1; j<h->GetNbinsY()+1; j++){
      for( int k=1; k<h->GetNbinsZ()+1; k++){  	    

      double area = (xbinsize)*(ybinsize); // area for every bin
      double content = h->GetBinContent(i,j,k)/Ebinsize/area; // now is GeV-1 cm-2
      double normalized_content = apply_normalization(content); // GeV-1 cm-2 s-1 per 5L0
      h->SetBinContent(i,j,k,normalized_content);
      }
    }
  }

/*
  gStyle->SetPalette(kRainBow);
  TCanvas *c0 = new TCanvas();
  c0->SetCanvasSize(800,600);
  c0->SetWindowSize(805+(805-c0->GetWw()), 620+(620-c0->GetWh()));
  c0->SetRightMargin(0.15);
  c0->Draw();
  h->GetXaxis()->SetTitle("x [cm]");
  h->GetYaxis()->SetTitle("y [cm]");
  h->GetZaxis()->SetTitle("E [GeV]");
  h->GetXaxis()->SetTitleOffset(1.3);
  h->GetYaxis()->SetTitleOffset(1.3);
  h->GetZaxis()->SetTitleOffset(1.3);
  h->SetStats(0);
  h->Draw("BOX2Z");
*/
  
  // save to file
  fout->cd(name.c_str());
  h->Write();
  hd->Write();
  fin->cd();
}

void make_generator_input(){

  fout = new TFile(outpath_mu.c_str(),"RECREATE");
  std::string mu_m = "mu_minus";
  std::string mu_p = "mu_plus";
  std::string neut = "neut";
  TDirectory *dir_minus = fout->mkdir(mu_m.c_str());
  TDirectory *dir_plus= fout->mkdir(mu_p.c_str());
  TDirectory *dir_neut = fout->mkdir(neut.c_str());

  set_muon_binning();

  // negative muons
  fin = new TFile("/eos/user/m/mvicenzi/FPF_FLUKA/muons/flux_v2/Nmu_data.root", "READ");
  TTree *tmu_n = (TTree*) fin->Get(mu_m.c_str());
  std::cout << "Setting up: " << mu_m << std::endl;
  set_tree(tmu_n);
  make_histos(tmu_n,mu_m);
  
  // positive muons
  fin = new TFile("/eos/user/m/mvicenzi/FPF_FLUKA/muons/flux_v2/Pmu_data.root", "READ");
  TTree *tmu_p = (TTree*) fin->Get(mu_p.c_str());
  std::cout << "Setting up: " << mu_p << std::endl;
  set_tree(tmu_p);
  make_histos(tmu_p,mu_p);

  set_neutron_binning();

  // neutrons
  fin = new TFile("/eos/user/m/mvicenzi/FPF_FLUKA/neutrons/Neutron_data.root", "READ");
  TTree *tn = (TTree*) fin->Get(neut.c_str());
  std::cout << "Setting up: " << neut << std::endl;
  set_tree(tn);
  make_histos(tn,neut);

  fout->Close(); //close file

}
