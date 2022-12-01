double GetTotalEnergy(double px, double py, double pz, double m);
void drawEvd(std::string save_dir, int evtID,
             std::vector<TH2F*>& hitClustersZX,
             std::vector<TH2F*>& hitClustersZY,
             std::vector<TH2F*>& vtxHitClustersZX,
             std::vector<TH2F*>& vtxHitClustersZY,
             int nuPDG, double nuE, double nuX, double nuY, double nuZ,
             int* trackPDG, double* Px, double* Py, double* Pz, double* Pmass,
             double* VtxX, double* VtxY, double* VtxZ);
void drawOneEvd(double nuX, double nuY, double nuZ,
                TH2F* hitClusterZX, TH2F* hitClusterZY,
                TString save_filename, TString text_content, bool drawVtx);


void plot_evd() {
  gStyle->SetOptTitle(0);

  //TFile* sfile = new TFile("/dune/app/users/wenjiewu/FLArE_Dev/FLArE_build/test_diffusion.root", "read");
  //TFile* sfile = new TFile("/dune/app/users/wenjiewu/FLArE_Dev/FLArE_build/test_diffusion.root", "read");
  TFile* sfile = new TFile("/dune/app/users/wenjiewu/FLArE_Dev/FLArE_build/test_5_9.root", "read");
  std::string save_dir = "pics_tmp";

  TTree* stree = (TTree*)sfile->Get("evt");

  int evtID;
  int nuPDG;
  double nuE, nuX, nuY, nuZ;
  int nPrimaryParticle;
  int primaryTrackPDG[10000];
  double Px[10000], Py[10000], Pz[10000], Pmass[10000];
  double VtxX[10000], VtxY[10000], VtxZ[10000];
  stree->SetBranchAddress("evtID", &evtID);
  stree->SetBranchAddress("nuPDG", &nuPDG);
  stree->SetBranchAddress("nuE", &nuE);
  stree->SetBranchAddress("nuX", &nuX);
  stree->SetBranchAddress("nuY", &nuY);
  stree->SetBranchAddress("nuZ", &nuZ);
  stree->SetBranchAddress("nPrimaryParticle", &nPrimaryParticle);
  stree->SetBranchAddress("primaryTrackPDG", primaryTrackPDG);
  stree->SetBranchAddress("Px", Px);
  stree->SetBranchAddress("Py", Py);
  stree->SetBranchAddress("Pz", Pz);
  stree->SetBranchAddress("Pmass", Pmass);
  stree->SetBranchAddress("VtxX", VtxX);
  stree->SetBranchAddress("VtxY", VtxY);
  stree->SetBranchAddress("VtxZ", VtxZ);

  for (int ientry= 0; ientry< stree->GetEntries(); ++ientry) {
    stree->GetEntry(ientry);

    std::vector<TH2F*> hitClustersZX;
    std::vector<TH2F*> hitClustersZY;
    std::vector<TH2F*> vtxHitClustersZX;
    std::vector<TH2F*> vtxHitClustersZY;
    TString histname;
    histname.Form("edep/evt_%d/evt_%d_tot_zx",evtID,evtID);
    hitClustersZX.push_back((TH2F*)sfile->Get(histname));
    histname.Form("edep/evt_%d/evt_%d_tot_zy",evtID,evtID);
    hitClustersZY.push_back((TH2F*)sfile->Get(histname));
    histname.Form("edep_vtx/evt_%d/evt_%d_tot_zx_vtx",evtID,evtID);
    vtxHitClustersZX.push_back((TH2F*)sfile->Get(histname));
    histname.Form("edep_vtx/evt_%d/evt_%d_tot_zy_vtx",evtID,evtID);
    vtxHitClustersZY.push_back((TH2F*)sfile->Get(histname));
    for (int iprim= 0; iprim< nPrimaryParticle; ++iprim) {
      histname.Form("edep/evt_%d/evt_%d_prong_%d_zx",evtID,evtID,iprim);
      hitClustersZX.push_back((TH2F*)sfile->Get(histname));
      histname.Form("edep/evt_%d/evt_%d_prong_%d_zy",evtID,evtID,iprim);
      hitClustersZY.push_back((TH2F*)sfile->Get(histname));
      histname.Form("edep_vtx/evt_%d/evt_%d_prong_%d_zx_vtx",evtID,evtID,iprim);
      vtxHitClustersZX.push_back((TH2F*)sfile->Get(histname));
      histname.Form("edep_vtx/evt_%d/evt_%d_prong_%d_zy_vtx",evtID,evtID,iprim);
      vtxHitClustersZY.push_back((TH2F*)sfile->Get(histname));
    }

    drawEvd(save_dir, evtID, hitClustersZX, hitClustersZY, vtxHitClustersZX, vtxHitClustersZY, nuPDG, nuE,
            nuX, nuY, nuZ, primaryTrackPDG, Px, Py, Pz, Pmass, VtxX, VtxY, VtxZ);

    hitClustersZX.clear();
    hitClustersZY.clear();
    vtxHitClustersZX.clear();
    vtxHitClustersZY.clear();
  }
}

void drawEvd(std::string save_dir, int evtID,
             std::vector<TH2F*>& hitClustersZX,
             std::vector<TH2F*>& hitClustersZY,
             std::vector<TH2F*>& vtxHitClustersZX,
             std::vector<TH2F*>& vtxHitClustersZY,
             int nuPDG, double nuE, double nuX, double nuY, double nuZ,
             int* trackPDG, double* Px, double* Py, double* Pz, double* Pmass,
             double* VtxX, double* VtxY, double* VtxZ) {


  for (int idx= 0; idx<hitClustersZX.size(); ++idx) {
    TString save_filename;
    TString text_content;
    if (idx==0) {
      save_filename.Form("%s/evt_%d_tot_Edep.pdf",save_dir.c_str(),evtID);
      text_content.Form("EvtID %d nuPDG %d nuE %.2f GeV nuVtx (%.1f, %.1f, %.1f) mm ",evtID,nuPDG,nuE,nuX,nuY,nuZ);
      drawOneEvd(nuX, nuY, nuZ, hitClustersZX[idx], hitClustersZY[idx], save_filename, text_content, true);
    } else {
      save_filename.Form("%s/evt_%d_Prong_%d_Edep.pdf",save_dir.c_str(),evtID,idx-1);
      text_content.Form("EvtID %d PDG %d Etot %.1f GeV (%.1f, %.1f, %.1f) mm ",evtID,trackPDG[idx-1],
          GetTotalEnergy(Px[idx-1],Py[idx-1],Pz[idx-1],Pmass[idx-1])/1000,VtxX[idx-1],VtxY[idx-1],VtxZ[idx-1]);
      drawOneEvd(VtxX[idx-1], VtxY[idx-1], VtxZ[idx-1], hitClustersZX[idx], hitClustersZY[idx], save_filename, text_content, true);
    }

    if (idx==0) {
      save_filename.Form("%s/evt_vtx_%d_tot_Edep.pdf",save_dir.c_str(),evtID);
      text_content.Form("EvtID %d nuPDG %d nuE %.2f GeV nuVtx (%.1f, %.1f, %.1f) mm ",evtID,nuPDG,nuE,nuX,nuY,nuZ);
      drawOneEvd(nuX, nuY, nuZ, vtxHitClustersZX[idx], vtxHitClustersZY[idx], save_filename, text_content, false);
    } else {
      save_filename.Form("%s/evt_vtx_%d_Prong_%d_Edep.pdf",save_dir.c_str(),evtID,idx-1);
      text_content.Form("EvtID %d PDG %d Etot %.1f GeV (%.1f, %.1f, %.1f) mm ",evtID,trackPDG[idx-1],
          GetTotalEnergy(Px[idx-1],Py[idx-1],Pz[idx-1],Pmass[idx-1])/1000,VtxX[idx-1],VtxY[idx-1],VtxZ[idx-1]);
      drawOneEvd(VtxX[idx-1], VtxY[idx-1], VtxZ[idx-1], vtxHitClustersZX[idx], vtxHitClustersZY[idx], save_filename, text_content, false);
    }
  }
}

void drawOneEvd(double nuX, double nuY, double nuZ,
                TH2F* hitClusterZX, TH2F* hitClusterZY,
                TString save_filename, TString text_content, bool drawVtx) {
  TLine* LArTPC = new TLine(7000, -900, 7000, 900);
  TLine* HadCalus = new TLine(7250, -900, 7250, 900);
  TLine* HadCalds = new TLine(8300, -900, 8300, 900);
  TLine* Muonds = new TLine(8660, -900, 8660, 900);
  LArTPC  ->SetLineStyle(3);
  HadCalus->SetLineStyle(3);
  HadCalds->SetLineStyle(3);
  Muonds  ->SetLineStyle(3);

  TMarker* vzx = new TMarker(nuZ, nuX, 30);
  TMarker* vzy = new TMarker(nuZ, nuY, 30);
  vzx->SetMarkerColor(kRed);
  vzy->SetMarkerColor(kRed);
  vzx->SetMarkerSize(2);
  vzy->SetMarkerSize(2);

  TCanvas* cc = new TCanvas("cc", "cc", 1000, 500);
  cc->SetTopMargin(0.01);
  cc->SetBottomMargin(0.01);
  cc->SetRightMargin(0.01);
  cc->SetLeftMargin(0.01);

  double xtitlesize = 0.15;
  double ypad1 = 1-0.95/(1/xtitlesize*2-1)*(1/xtitlesize-1);
  double ypad2 = ypad1 - 0.95/(1/xtitlesize*2-1)*(1/xtitlesize);
  double padratio = (1-ypad1)/(ypad1-ypad2);

  TPad* pad1 = new TPad("pad1", "pad1", 0, ypad1, 1, 1);
  pad1->SetTopMargin(0.01);
  pad1->SetBottomMargin(0);
  pad1->SetRightMargin(0.1);
  pad1->SetLeftMargin(0.1);
  pad1->SetLogz();
  pad1->Draw();
  pad1->cd();
  hitClusterZX->GetYaxis()->CenterTitle();
  hitClusterZX->GetYaxis()->SetTitleOffset(0.45);
  hitClusterZX->GetYaxis()->SetTitleSize(0.08);
  hitClusterZX->GetYaxis()->SetLabelSize(0.08);
  hitClusterZX->GetYaxis()->SetTickSize(0.01);
  hitClusterZX->GetZaxis()->SetLabelSize(0.08);
  hitClusterZX->GetZaxis()->SetLabelOffset(0.001);
  hitClusterZX->Draw("colz");
  if(drawVtx) vzx->Draw("same");
  LArTPC->Draw("same");
  HadCalus->Draw("same");
  HadCalds->Draw("same");
  Muonds->Draw("same");
  pad1->Modified();

  cc->cd();
  TPad* pad2 = new TPad("pad2", "pad2", 0, ypad2, 1, ypad1);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(xtitlesize);
  pad2->SetRightMargin(0.1);
  pad2->SetLeftMargin(0.1);
  pad2->SetLogz();
  pad2->Draw();
  pad2->cd();
  hitClusterZY->GetYaxis()->CenterTitle();
  hitClusterZY->GetYaxis()->SetTitleOffset(0.45/padratio);
  hitClusterZY->GetYaxis()->SetTitleSize(0.08*padratio);
  hitClusterZY->GetYaxis()->SetLabelSize(0.08*padratio);
  hitClusterZY->GetYaxis()->SetTickSize(0.01/padratio);
  hitClusterZY->GetXaxis()->CenterTitle();
  hitClusterZY->GetXaxis()->SetTitleOffset(0.9);
  hitClusterZY->GetXaxis()->SetTitleSize(0.08*padratio);
  hitClusterZY->GetXaxis()->SetLabelSize(0.08*padratio);
  hitClusterZY->GetZaxis()->SetLabelSize(0.08*padratio);
  hitClusterZY->GetZaxis()->SetLabelOffset(0.001/padratio);
  hitClusterZY->Draw("colz");
  if(drawVtx) vzy->Draw("same");
  LArTPC->Draw("same");
  HadCalus->Draw("same");
  HadCalds->Draw("same");
  Muonds->Draw("same");
  pad2->Modified();

  cc->cd();
  TPad* pad3 = new TPad("pad3", "pad3", 0, 0, 1, ypad2);
  pad3->SetTopMargin(0);
  pad3->SetBottomMargin(0);
  pad3->SetRightMargin(0.1);
  pad3->SetLeftMargin(0.1);
  pad3->Draw();
  pad3->cd();
  TText* text = new TText(0.1,0.3,text_content);
  text->SetTextSize(0.8);
  text->Draw();
  pad3->Modified();

  cc->cd();
  cc->SaveAs(save_filename);

  delete pad1;
  delete pad2;
  delete pad3;
  delete cc;

  delete vzx;
  delete vzy;
  delete LArTPC;
  delete HadCalus;
  delete HadCalds;
  delete Muonds;
}

double GetTotalEnergy(double px, double py, double pz, double m) {
  return TMath::Sqrt(px*px+py*py+pz*pz+m*m);
}
