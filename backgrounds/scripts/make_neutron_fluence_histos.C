#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <sstream>
#include <algorithm>

// -----------------------------------------------------------------
// global definitions
double xmin=-326., xmax=614., xsize=20.; //cm
double ymin=-159., ymax=601., ysize=20.; //cm
int xbins=47, ybins=38;

int N; //total entries
double x, y, z; //particle position
double w; //statistical weight

std::string outpath = "../neutrons/n_fluence_histograms.root";
TFile *f; //input 
TFile *fout; //output

// -----------------------------------------------------------------

double apply_normalization(double value){

  //input: number of neutrons per cm2
  //The numbers in the files must be normalized in order to get the fluence rate (flux) for 5Lo considering:
  //- the instantaneous luminosity: 5e34 cm-2 s-1
  double n1 = 5.0e34; //cm-2 s-1
  //- the cross section of the p-p collision: 85 mb = 85e-3*1e-24 cm2
  double n2 = 85e-3*1e-24; //cm2
  //-  a normalization factor coming from the 2 step simulation approach: 2104211.0/14e14
  //   with this normalisation factor, any quantity is given per primary p-p collision.
  double n3 = (7509624.0/1.0e8)/1.0e8; 
  //therefore: 
  double N =n1*n2*n3;

  return N*value; // number of neutrons cm-2 s-1 (for 5Lo)
}

// ----------------------------------------------------------------------------------

void setup_tree(TTree *t){

  N = t->GetEntries();
  t->SetBranchAddress("weight", &w);
  t->SetBranchAddress("x", &x);
  t->SetBranchAddress("y", &y);
  t->SetBranchAddress("z", &z);

}

// ----------------------------------------------------------------

void build_xy_projection(TTree *t, std::string ts=""){

  //we only have data from a single plane (not at constant z!)
  //lets keep track of how much is spanned over z 
  std::vector<double> zpoints;  

  // define the first histogram
  std::string id = "h_xy_" + ts;
  std::ostringstream s;
  s << "Neutron fluence in XY at FPF entrance";
  f->cd();
  TH2D *h_xy = new TH2D(id.c_str(),s.str().c_str(),xbins,xmin,xmax,ybins,ymin,ymax);
  
  for( int i=0; i<N; i++){
    t->GetEntry(i);
    h_xy->Fill(x,y,w);
    zpoints.push_back(z);

    if ( i % 100000 == 0 ) std::cout << i << " entries processed" << std::endl;
  }

  std::sort(zpoints.begin(),zpoints.end());
  double zmin = zpoints[0];
  double zmax = zpoints[zpoints.size()-1];
  s << " z=[" << zmin << "," << zmax << "] cm";  

  //apply normalization
  double bin_area = xsize*ysize; //cm2
  
  for( int i=1; i<h_xy->GetNbinsX()+1; i++){
    for( int k=1; k<h_xy->GetNbinsY()+1; k++){    
      double content = h_xy->GetBinContent(i,k)/bin_area; // now is cm-2
      content = apply_normalization(content); // cm-2 s-1 per 5L0
      h_xy->SetBinContent(i,k,content);
    }
  }
  
  std::string idz = "h_xy_zoom_" + ts;
  TH2D *h_xy_zoom = new TH2D(idz.c_str(),s.str().c_str(),10.,-106.,94.,10.,-99.,101.);
  
  std::ofstream outcsv;
  std::string csvpath = "zoom_bins_" + ts + ".csv";
  outcsv.open(csvpath,ios::out);
  outcsv << "x,y,fluence" << std::endl;

  for(int ii=1; ii<h_xy->GetNbinsX()+1; ii++){
    for(int jj=1; jj<h_xy->GetNbinsY()+1; jj++){
    
      double xbin = h_xy->GetXaxis()->GetBinCenter(ii);
      double ybin = h_xy->GetYaxis()->GetBinCenter(jj);
      double content = h_xy->GetBinContent(ii,jj);
      h_xy_zoom->Fill(xbin,ybin,content);

      if( xbin > -100. && xbin < 100. && ybin > -100. && ybin < 100. ) 
        outcsv << xbin << "," << ybin << "," << content << std::endl; 
    }
  }

  outcsv.close();
  
  h_xy->SetTitle(s.str().c_str());
  h_xy->SetMaximum(10.);
  h_xy->SetMinimum(0.001);
  h_xy->GetXaxis()->SetTitle("x [cm]");
  h_xy->GetYaxis()->SetTitle("y [cm]");
  h_xy->GetYaxis()->SetTitleOffset(1.3);
  h_xy->GetZaxis()->SetTitle("Fluence rate per 5L_{0} [cm^{-2}s^{-1}]");
  h_xy->GetZaxis()->SetTitleOffset(1.3);
  h_xy->SetStats(0);

  TCanvas *c0 = new TCanvas();
  c0->SetCanvasSize(800,600);
  c0->SetWindowSize(805+(805-c0->GetWw()), 620+(620-c0->GetWh()));
  c0->SetRightMargin(0.15);
  c0->cd();
  h_xy->Draw("COL2Z");
  gPad->SetLogz();
  
  TCanvas *c1 = new TCanvas();
  c1->SetCanvasSize(800,600);
  c1->SetWindowSize(805+(805-c1->GetWw()), 620+(620-c1->GetWh()));
  c1->SetRightMargin(0.15);
  c1->cd();
  //h_xy_zoom->SetMaximum(10.);
  //h_xy_zoom->SetMinimum(0.001);
  h_xy_zoom->GetXaxis()->SetTitle("x [cm]");
  h_xy_zoom->GetYaxis()->SetTitle("y [cm]");
  h_xy_zoom->GetYaxis()->SetTitleOffset(1.3);
  h_xy_zoom->GetZaxis()->SetTitle("Fluence rate per 5L_{0} [cm^{-2}s^{-1}]");
  h_xy_zoom->GetZaxis()->SetTitleOffset(1.3);
  h_xy_zoom->SetStats(0);
  h_xy_zoom->Draw("COL2Z TEXT");
  //gPad->SetLogz();
 
  // save to image
  std::string path = "figs/"+id+".png"; 
  std::string pathz = "figs/"+idz+".png"; 
  c0->SaveAs(path.c_str());
  c1->SaveAs(pathz.c_str());

  // save to file
  fout->cd(ts.c_str());
  h_xy->Write();
  h_xy_zoom->Write();
  f->cd();
} 

// ----------------------------------------------------------------

void make_neutron_fluence_histos(){

  //input
  f = new TFile("/eos/user/m/mvicenzi/FPF_FLUKA/neutrons/Neutron_data.root", "READ");
  std::string m_n = "neut";
  TTree *neut = (TTree*) f->Get(m_n.c_str()); 
 
  //output
  fout = new TFile(outpath.c_str(),"RECREATE");
  TDirectory *dir_n = fout->mkdir(m_n.c_str());

  //histogram making
  gStyle->SetPalette(kRainBow);

  setup_tree(neut);
  build_xy_projection(neut,m_n);

  fout->Close();
}
