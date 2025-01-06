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

double ymin, ymax;
double ybinsize;

int Ebins;
double Emin, Emax;
double Ebinsize;

// files
std::string outpath_mu = "../muons/mu_flux_histograms.root";
TFile *fin; //input
TFile *fout; //output

// ----------------------------------------------------------------------------------

void set_muons_binning(){
  // 100cm bins along X
  xbins = 9;
  xmin = -300.; xmax = 600.;
  xbinsize = (xmax-xmin)/xbins;

  // full height along y (entire hall)
  ymin = -159.; ymax = 601.;
  ybinsize = (ymax-ymin);

  // energy binning: 20 GeV
  Ebins = 225;
  Emin = 0., Emax = 4500.;
  Ebinsize = (Emax-Emin)/Ebins;
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
  double n3 = (7509624.0/1.0e8)/1.0e8; //CAUTION: this is for v2 files
  //therefore: 
  double N =n1*n2*n3;

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

  std::string title = name + " flux at FPF entrance";
  TH2D *h = new TH2D("h",title.c_str(),xbins,xmin,xmax,Ebins,Emin,Emax);
  std::cout << N << " entries" << std::endl;

  for (int i=0; i<N; i++){
    t->GetEntry(i);

    // full hall over y, binning only in x
    if ( y< ymin || y > ymax ) continue;
    h->Fill( x, E, w);

    if ( i % 100000 == 0 ) std::cout << i << " entries processed" << std::endl;
  }

  //apply normalizations
  for( int i=1; i<h->GetNbinsX()+1; i++){
    for( int k=1; k<h->GetNbinsY()+1; k++){

      double area = (xbinsize)*(ybinsize); // area for every bin
      double  content = h->GetBinContent(i,k)/Ebinsize/area; // now is GeV-1 cm-2
      content = apply_normalization(content); // GeV-1 cm-2 s-1 per 5L0
      h->SetBinContent(i,k,content);
    }
  }

  //now make 1D histograms for every x bin 
  TH1D **hx = new TH1D*[xbins];
  std::vector<std::string> labels;
  for( int j=1; j<h->GetNbinsX()+1; j++ ){
     
     std::ostringstream id, tt;
     id << "h_" << h->GetXaxis()->GetBinLowEdge(j) << "_" << h->GetXaxis()->GetBinLowEdge(j)+xbinsize;
     tt << h->GetXaxis()->GetBinLowEdge(j) << " cm < x < " << h->GetXaxis()->GetBinLowEdge(j)+xbinsize << " cm";
     labels.push_back(tt.str());

     hx[j-1] = new TH1D(id.str().c_str(),title.c_str(),Ebins,Emin,Emax);
     for( int k=1; k<Ebins+1; k++){
	hx[j-1]->SetBinContent(k,h->GetBinContent(j,k)); 
     }
  }
	
  gStyle->SetPalette(kRainBow);

  TCanvas *c0 = new TCanvas();
  c0->SetCanvasSize(800,600);
  c0->SetWindowSize(805+(805-c0->GetWw()), 620+(620-c0->GetWh()));
  c0->SetRightMargin(0.15);
  c0->Draw();
  h->GetXaxis()->SetTitle("x [cm]");
  h->GetYaxis()->SetTitle("E [GeV]");
  h->GetYaxis()->SetTitleOffset(1.3);
  h->GetZaxis()->SetTitle("Flux [GeV^{-1} cm^{-2} s^{-1} per 5L_{0}]");
  h->GetZaxis()->SetTitleOffset(1.3);
  h->SetStats(0);
  h->Draw("COLZ");
  gPad->SetLogz();

  int color[9] = { kBlue, kBlack, kRed, kMagenta, kGreen, kGray, kYellow, kCyan, kOrange };

  TCanvas *c1 = new TCanvas();
  c1->SetCanvasSize(900,500);
  c1->SetWindowSize(905+(905-c1->GetWw()), 520+(520-c1->GetWh()));
  c1->cd();
  for( int j=1; j<h->GetNbinsX()+1; j++){
    hx[j-1]->GetXaxis()->SetTitle("E [GeV]");
    hx[j-1]->GetYaxis()->SetTitle("Flux [GeV^{-1} cm^{-2} s^{-1} per 5L_{0}]");
    hx[j-1]->SetLineColor(color[j-1]);
    hx[j-1]->SetStats(0);
    if(j==0) hx[j-1]->Draw();
    else hx[j-1]->Draw("SAME");
  }
  gPad->SetLogy();

  TLegend* legend = new TLegend(0.7,0.6,0.9,0.9);
  for( int j=0; j<h->GetNbinsX(); j++){
    legend->AddEntry(hx[j],labels[j].c_str(),"l");
  }
  legend->Draw();

  // save to image
  std::string path = "figs/flux_"+name+"_FPF_entrance.png"; 
  c1->SaveAs(path.c_str());

  // save to file
  fout->cd(name.c_str());
  for( int j=0; j<h->GetNbinsX(); j++){
    hx[j]->SetTitle(labels[j].c_str());
    hx[j]->Write();
  }
  fin->cd();
}


void make_muon_flux_histos(){

  fout = new TFile(outpath_mu.c_str(),"RECREATE");
  std::string mu_m = "mu_minus";
  std::string mu_p = "mu_plus";
  TDirectory *dir_minus = fout->mkdir(mu_m.c_str());
  TDirectory *dir_plus= fout->mkdir(mu_p.c_str());

  set_muons_binning();

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

  fout->Close(); //close muons

}
