#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <sstream>

// -----------------------------------------------------------------
// global definitions
double xmin, xmax, xsize;
double ymin, ymax, ysize;
double zmin, zmax, zsize;
int xbins, ybins, zbins;
std::vector<double> xc, yc, zc;
std::vector<double> v;

std::string outpath = "../muons/mu_fluence_histograms.root";
TFile *f; //input 
TFile *fout; //output

// -----------------------------------------------------------------

void clear_all(){
  xc.clear();
  yc.clear();
  zc.clear();
  v.clear();
}

void read_tree(TTree *t){

  TTreeReader reader(t);
  TTreeReaderValue<double> xmi(reader,"xmin");
  TTreeReaderValue<double> xma(reader,"xmax");
  TTreeReaderValue<double> ymi(reader,"ymin");
  TTreeReaderValue<double> yma(reader,"ymax");
  TTreeReaderValue<double> zmi(reader,"zmin");
  TTreeReaderValue<double> zma(reader,"zmax");
  TTreeReaderValue<int> nx(reader,"xbins");
  TTreeReaderValue<int> ny(reader,"ybins");
  TTreeReaderValue<int> nz(reader,"zbins");
  TTreeReaderValue<double> xbinsize(reader,"xsize");
  TTreeReaderValue<double> ybinsize(reader,"ysize");
  TTreeReaderValue<double> zbinsize(reader,"zsize");
  TTreeReaderArray<double> x(reader,"x");
  TTreeReaderArray<double> y(reader,"y");
  TTreeReaderArray<double> z(reader,"z");
  TTreeReaderArray<double> f(reader,"f");

  while( reader.Next() ){

    xmin = *xmi;	xmax = *xma;	
    ymin = *ymi;	ymax = *yma;	
    zmin = *zmi;	zmax = *zma;	
    xbins = *nx;	ybins = *ny;	zbins = *nz;
    xsize = *xbinsize;	ysize = *ybinsize;	zsize = *zbinsize;
    
    int nbins = xbins*ybins*zbins;
    for( int i = 0; i< nbins; i++){
      xc.push_back(x[i]);		
      yc.push_back(y[i]);		
      zc.push_back(z[i]);		
      v.push_back(f[i]);
    }
  }
}

// ----------------------------------------------------------------

void build_zx_projection(double miny, double maxy, std::string ts=""){

  std::map<std::pair<double,double>,double> zx_proj;
  std::map<std::pair<double,double>,int> zx_proj_counter;

  // for each zx bin, compute the average value along y
  // y range is specified by the arguments
  int nbins = xbins*ybins*zbins;
  for( int i = 0; i< nbins; i++){

    if( yc[i] < miny || yc[i] > maxy ) continue;
    std::pair<double,double> currBin = std::make_pair(zc[i],xc[i]);

    if( zx_proj.find(currBin) != zx_proj.end() ){
        zx_proj[currBin] += v[i];
        zx_proj_counter[currBin]++; 
    } else {
        zx_proj.insert(std::make_pair(currBin,v[i]));
        zx_proj_counter.insert(std::make_pair(currBin,1)); 
    }
  }
  for( auto it = zx_proj.begin(); it != zx_proj.end(); it++)
	it->second /= zx_proj_counter[it->first];

  // define the histogram
  std::string id = "h_zx_" + ts;
  std::ostringstream s;
  s <<  ts << " fluence in ZY averaged over y=(" << miny << "," << maxy << ")cm";
  TH2D *h_zx = new TH2D(id.c_str(),s.str().c_str(),zbins,zmin,zmax,xbins,xmin,xmax);
  
  for( auto it = zx_proj.begin(); it != zx_proj.end(); it++)
    h_zx->Fill(it->first.first,it->first.second,it->second);

  h_zx->SetMaximum(10.);
  h_zx->SetMinimum(0.001);
  h_zx->GetXaxis()->SetTitle("z [cm]");
  h_zx->GetYaxis()->SetTitle("x [cm]");
  h_zx->GetZaxis()->SetTitle("Fluence rate per 5L_{0} [cm^{-2}s^{-1}]");
  h_zx->SetStats(0);

  TCanvas *c0 = new TCanvas();
  c0->SetCanvasSize(1250,600);
  c0->SetWindowSize(1255+(1255-c0->GetWw()), 620+(620-c0->GetWh()));
  c0->SetRightMargin(0.15);
  c0->cd();
  h_zx->Draw("COL2Z");
  gPad->SetLogz();
  
  // save to image
  std::string path = "figs/"+id+".png"; 
  c0->SaveAs(path.c_str());

  // save to file
  fout->cd(ts.c_str());
  h_zx->Write();
  f->cd();
}

void build_zy_projection(double minx, double maxx, std::string ts=""){

  std::map<std::pair<double,double>,double> zy_proj;
  std::map<std::pair<double,double>,int> zy_proj_counter;

  // for each zy bin, compute the average value along x
  // x range is specified by the arguments
  int nbins = xbins*ybins*zbins;
  for( int i = 0; i< nbins; i++){

    if( xc[i] < minx || xc[i] > maxx ) continue;
    std::pair<double,double> currBin = std::make_pair(zc[i],yc[i]);

    if( zy_proj.find(currBin) != zy_proj.end() ){
        zy_proj[currBin] += v[i];
        zy_proj_counter[currBin]++; 
    } else {
        zy_proj.insert(std::make_pair(currBin,v[i]));
        zy_proj_counter.insert(std::make_pair(currBin,1)); 
    }
  }
  for( auto it = zy_proj.begin(); it != zy_proj.end(); it++)
	it->second /= zy_proj_counter[it->first];

  // define the histogram
  std::string id = "h_zy_" + ts;
  std::ostringstream s;
  s <<  ts << " fluence in ZY averaged over x=(" << minx << "," << maxx << ")cm";
  TH2D *h_zy = new TH2D(id.c_str(),s.str().c_str(),zbins,zmin,zmax,ybins,ymin,ymax);
  
  for( auto it = zy_proj.begin(); it != zy_proj.end(); it++)
    h_zy->Fill(it->first.first,it->first.second,it->second);

  h_zy->SetMaximum(10.);
  h_zy->SetMinimum(0.001);
  h_zy->GetXaxis()->SetTitle("z [cm]");
  h_zy->GetYaxis()->SetTitle("y [cm]");
  h_zy->GetZaxis()->SetTitle("Fluence rate per 5L_{0} [cm^{-2}s^{-1}]");
  h_zy->SetStats(0);

  TCanvas *c0 = new TCanvas();
  c0->SetCanvasSize(1250,600);
  c0->SetWindowSize(1255+(1255-c0->GetWw()), 620+(620-c0->GetWh()));
  c0->SetRightMargin(0.15);
  c0->cd();
  h_zy->Draw("COL2Z");
  gPad->SetLogz();

  // save to image
  std::string path = "figs/"+id+".png"; 
  c0->SaveAs(path.c_str());

  // save to file
  fout->cd(ts.c_str());
  h_zy->Write();
  f->cd();
}

void build_xy_projection(double minz, double maxz, std::string ts=""){

  std::map<std::pair<double,double>,double> xy_proj;
  std::map<std::pair<double,double>,int> xy_proj_counter;

  // for each xy bin, compute the average value along z
  // z range is specified by the arguments
  int nbins = xbins*ybins*zbins;
  for( int i = 0; i< nbins; i++){

    if( zc[i] < minz || zc[i] > maxz ) continue;
    std::pair<double,double> currBin = std::make_pair(xc[i],yc[i]);

    if( xy_proj.find(currBin) != xy_proj.end() ){
        xy_proj[currBin] += v[i];
        xy_proj_counter[currBin]++; 
    } else {
        xy_proj.insert(std::make_pair(currBin,v[i]));
        xy_proj_counter.insert(std::make_pair(currBin,1)); 
    }
  }
  for( auto it = xy_proj.begin(); it != xy_proj.end(); it++)
	it->second /= xy_proj_counter[it->first];

  // define the histogram
  std::string id = "h_xy_" + ts;
  std::ostringstream s;
  s <<  ts << " fluence in XY averaged over z=(" << minz << "," <<maxz << ")cm";
  TH2D *h_xy = new TH2D(id.c_str(),s.str().c_str(),xbins,xmin,xmax,ybins,ymin,ymax);
  
  for( auto it = xy_proj.begin(); it != xy_proj.end(); it++)
    h_xy->Fill(it->first.first,it->first.second,it->second);

  h_xy->SetMaximum(10.);
  h_xy->SetMinimum(0.001);
  h_xy->GetXaxis()->SetTitle("x [cm]");
  h_xy->GetYaxis()->SetTitle("y [cm]");
  h_xy->GetYaxis()->SetTitleOffset(1.3);
  h_xy->GetZaxis()->SetTitle("Fluence rate per 5L_{0} [cm^{-2}s^{-1}]");
  h_xy->SetStats(0);

  TCanvas *c0 = new TCanvas();
  c0->SetCanvasSize(800,600);
  c0->SetWindowSize(805+(805-c0->GetWw()), 620+(620-c0->GetWh()));
  c0->SetRightMargin(0.15);
  c0->cd();
  h_xy->Draw("COL2Z");
  gPad->SetLogz();
 
  // save to image
  std::string path = "figs/"+id+".png"; 
  c0->SaveAs(path.c_str());

  // save to file
  fout->cd(ts.c_str());
  h_xy->Write();
  f->cd();
}

// ----------------------------------------------------------------

void make_muon_fluence_histos(){

  //input
  f = new TFile("/dune/data/users/mvicenzi/FPF_backgrounds/muons/fluence/mu_fluence.root", "READ");
  std::string mu_m = "mu_minus";
  std::string mu_p = "mu_plus";
  TTree *mu_minus = (TTree*) f->Get(mu_m.c_str()); 
  TTree *mu_plus = (TTree*) f->Get(mu_p.c_str()); 
 
  //output
  fout = new TFile(outpath.c_str(),"RECREATE");
  TDirectory *dir_minus = fout->mkdir(mu_m.c_str());
  TDirectory *dir_plus= fout->mkdir(mu_p.c_str());

  //histogram making
  gStyle->SetPalette(kRainBow);

  read_tree(mu_minus);
  build_zx_projection(-19.,21.,mu_m);
  build_zy_projection(-19.,21.,mu_m);
  build_xy_projection(61600.,61800.,mu_m);

  clear_all();

  read_tree(mu_plus);
  build_zx_projection(-19.,21.,mu_p);
  build_zy_projection(-19.,21.,mu_p);
  build_xy_projection(61600.,61800.,mu_p);

  fout->Close();
}
