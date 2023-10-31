#include "PixelMap3D.hh"
#include "geometry/GeometricalParameters.hh"

#include <G4ThreeVector.hh>
#include <G4SystemOfUnits.hh>
#include <G4Poisson.hh>

#include <Math/ProbFunc.h>
#include <Randomize.hh>
#include <TAxis.h>
#include <TString.h>

PixelMap3D::PixelMap3D(const Int_t evtID, const Int_t nPrim, const Int_t PDG, const Double_t* res) :
  fEvtID(evtID), fNPrim(nPrim), fGeneratorPDG(PDG)
{
  G4cout<<"Creating PixelMap3D for event "<<fEvtID<<G4endl;
  fResX = res[0]; fResY = res[1]; fResZ = res[2];
}

PixelMap3D::~PixelMap3D() 
{
  fBinX.clear();
  fBinY.clear();
  fBinZ.clear();
  for (G4int iPrim= 0; iPrim< fNPrim; ++iPrim) {
    delete hitClusterZX[iPrim];
    delete hitClusterZY[iPrim];
    delete vtxHitClusterZX[iPrim];
    delete vtxHitClusterZY[iPrim];
  }
  hitClusterZX.clear();
  hitClusterZY.clear();
  vtxHitClusterZX.clear();
  vtxHitClusterZY.clear();
  //hitClusterZX.shrink_to_fit();
  //hitClusterZY.shrink_to_fit();
  //vtxHitClusterZX.shrink_to_fit();
  //vtxHitClusterZY.shrink_to_fit();
  delete hist3DEdep;
}

void PixelMap3D::SetPMBoundary(G4ThreeVector min_xyz, G4ThreeVector max_xyz)
{
  fPMXYZMin = min_xyz/mm;
  fPMXYZMax = max_xyz/mm;
  fPMShiftedCenterXYZ = G4ThreeVector((min_xyz.x()+max_xyz.x())/2,
                                      (min_xyz.y()+max_xyz.y())/2,
                                      min_xyz.z())/mm;
  fPMSizeXYZ = max_xyz/mm - min_xyz/mm;
}

void PixelMap3D::InitializePM()
{
  // binning definition
  // x: drift direction on the transverse plane
  // y: orthogonal to x on the transverse plane
  // z: beam direction
  fNBinX = std::ceil(fPMSizeXYZ.x()/fResX);
  for (G4int ibin=0; ibin<fNBinX+1; ++ibin){
    fBinX.push_back(ibin*fResX + fPMXYZMin.x() - fPMShiftedCenterXYZ.x());
  }
  fNBinY = std::ceil(fPMSizeXYZ.y()/fResY);
  for (G4int ibin=0; ibin<fNBinY+1; ++ibin){
    fBinY.push_back(ibin*fResY + fPMXYZMin.y() - fPMShiftedCenterXYZ.y());
  }
  fNBinZ = std::ceil(fPMSizeXYZ.z()/fResZ);
  for (G4int ibin=0; ibin<fNBinZ+1; ++ibin){
    fBinZ.push_back(ibin*fResZ + fPMXYZMin.z() - fPMShiftedCenterXYZ.z());
  }
  const G4int num_bins_xyz[3] = {fNBinX, fNBinY, fNBinZ};

  TString histname, histtitle;
  histname.Form("evt_%d_3DHit", fEvtID);
  histtitle.Form("3DView: EvtID %d PDG %d", fEvtID, fGeneratorPDG);
  hist3DEdep = new THnSparseF(histname, histtitle, 3, num_bins_xyz);
  hist3DEdep->SetBinEdges(0, &fBinX[0]); 
  hist3DEdep->SetBinEdges(1, &fBinY[0]);
  hist3DEdep->SetBinEdges(2, &fBinZ[0]);

  hitClusterZX.resize(fNPrim+1);
  hitClusterZY.resize(fNPrim+1);
  vtxHitClusterZX.resize(fNPrim+1);
  vtxHitClusterZY.resize(fNPrim+1);
  histname.Form("evt_%d_tot_zx", fEvtID);
  histtitle.Form("ViewZX: EvtID %d PDG %d", fEvtID, fGeneratorPDG);
  hitClusterZX[0] = new TH2F(histname, histtitle, fNBinZ, &fBinZ[0], fNBinX, &fBinX[0]);
  hitClusterZX[0]->GetXaxis()->SetTitle("Z [mm]");
  hitClusterZX[0]->GetYaxis()->SetTitle("X [mm]");
  histname.Form("evt_%d_tot_zy", fEvtID);
  histtitle.Form("ViewZY: EvtID %d PDG %d", fEvtID, fGeneratorPDG);
  hitClusterZY[0] = new TH2F(histname, histtitle, fNBinZ, &fBinZ[0], fNBinY, &fBinY[0]);
  hitClusterZY[0]->GetXaxis()->SetTitle("Z [mm]");
  hitClusterZY[0]->GetYaxis()->SetTitle("Y [mm]");
  histname.Form("evt_%d_tot_zx_vtx",fEvtID);
  histtitle.Form("VtxViewZX: EvtID %d PDG %d", fEvtID, fGeneratorPDG);
  vtxHitClusterZX[0] = new TH2F(histname, histtitle, 1500, -20, 280, 600, -60, 60);
  vtxHitClusterZX[0]->GetXaxis()->SetTitle("Z [mm]");
  vtxHitClusterZX[0]->GetYaxis()->SetTitle("X [mm]");
  histname.Form("evt_%d_tot_zy_vtx",fEvtID);
  histtitle.Form("VtxViewZY: EvtID %d PDG %d", fEvtID, fGeneratorPDG);
  vtxHitClusterZY[0] = new TH2F(histname, histtitle, 1500, -20, 280, 600, -60, 60);
  vtxHitClusterZY[0]->GetXaxis()->SetTitle("Z [mm]");
  vtxHitClusterZY[0]->GetYaxis()->SetTitle("Y [mm]");
  for (int iPrim=0; iPrim< fNPrim; ++iPrim) {
    histname.Form("evt_%d_prong_%d_zx",fEvtID,iPrim);
    histtitle.Form("ViewZX: EvtID %d Prong %d", fEvtID, iPrim);
    hitClusterZX[iPrim+1] = new TH2F(histname, histtitle, fNBinZ, &fBinZ[0], fNBinX, &fBinX[0]);
    hitClusterZX[iPrim+1]->GetXaxis()->SetTitle("Z [mm]");
    hitClusterZX[iPrim+1]->GetYaxis()->SetTitle("X [mm]");
    histname.Form("evt_%d_prong_%d_zy",fEvtID,iPrim);
    histtitle.Form("ViewZY: EvtID %d Prong %d", fEvtID, iPrim);
    hitClusterZY[iPrim+1] = new TH2F(histname, histtitle, fNBinZ, &fBinZ[0], fNBinY, &fBinY[0]);
    hitClusterZY[iPrim+1]->GetXaxis()->SetTitle("Z [mm]");
    hitClusterZY[iPrim+1]->GetYaxis()->SetTitle("Y [mm]");
    histname.Form("evt_%d_prong_%d_zx_vtx",fEvtID,iPrim);
    histtitle.Form("VtxViewZX: EvtID %d Prong %d", fEvtID, iPrim);
    vtxHitClusterZX[iPrim+1] = new TH2F(histname, histtitle, 1500, -20, 280, 600, -60, 60);
    vtxHitClusterZX[iPrim+1]->GetXaxis()->SetTitle("Z [mm]");
    vtxHitClusterZX[iPrim+1]->GetYaxis()->SetTitle("X [mm]");
    histname.Form("evt_%d_prong_%d_zy_vtx",fEvtID,iPrim);
    histtitle.Form("VtxViewZY: EvtID %d Prong %d", fEvtID, iPrim);
    vtxHitClusterZY[iPrim+1] = new TH2F(histname, histtitle, 1500, -20, 280, 600, -60, 60);
    vtxHitClusterZY[iPrim+1]->GetXaxis()->SetTitle("Z [mm]");
    vtxHitClusterZY[iPrim+1]->GetYaxis()->SetTitle("Y [mm]");
  }
}

void PixelMap3D::FillEntry(const Double_t* pos_xyz, const Double_t* vtx_xyz, const Double_t edep, const Int_t idxPrim)
{
  G4double pos_x = pos_xyz[0] - fPMShiftedCenterXYZ.x();
  G4double pos_y = pos_xyz[1] - fPMShiftedCenterXYZ.y();
  G4double pos_z = pos_xyz[2] - fPMShiftedCenterXYZ.z();
  G4double vtx_x = vtx_xyz[0] - fPMShiftedCenterXYZ.x();
  G4double vtx_y = vtx_xyz[1] - fPMShiftedCenterXYZ.y();
  G4double vtx_z = vtx_xyz[2] - fPMShiftedCenterXYZ.z();
  Double_t relative_pos_xyz[3] = {pos_x, pos_y, pos_z};
  hist3DEdep->Fill(relative_pos_xyz, edep);
  hitClusterZX[0]->Fill(pos_z, pos_x, edep);
  hitClusterZY[0]->Fill(pos_z, pos_y, edep);
  hitClusterZX[idxPrim+1]->Fill(pos_z, pos_x, edep);
  hitClusterZY[idxPrim+1]->Fill(pos_z, pos_y, edep);
  vtxHitClusterZX[0]->Fill(pos_z-vtx_z, pos_x-vtx_x, edep);
  vtxHitClusterZY[0]->Fill(pos_z-vtx_z, pos_y-vtx_y, edep);
  vtxHitClusterZX[idxPrim+1]->Fill(pos_z-vtx_z, pos_x-vtx_x, edep);
  vtxHitClusterZY[idxPrim+1]->Fill(pos_z-vtx_z, pos_y-vtx_y, edep);
}

void PixelMap3D::FillEntryWithToyElectronTransportation(const Double_t* pos_xyz, const Double_t* vtx_xyz, Double_t edep, const Int_t idxPrim)
{
  G4double pos_x = pos_xyz[0] - fPMShiftedCenterXYZ.x();
  G4double pos_y = pos_xyz[1] - fPMShiftedCenterXYZ.y();
  G4double pos_z = pos_xyz[2] - fPMShiftedCenterXYZ.z();
  G4double vtx_x = vtx_xyz[0] - fPMShiftedCenterXYZ.x();
  G4double vtx_y = vtx_xyz[1] - fPMShiftedCenterXYZ.y();
  G4double vtx_z = vtx_xyz[2] - fPMShiftedCenterXYZ.z();
  // https://lar.bnl.gov/properties/
  G4double DT = 13.2327; // Transverse diffusion coefficients @ 500 V/cm, cm^2/s
  G4double DL = 6.627;   // Longitudinal diffusion coeeficients @ 500 V/cm, cm^2/s
  G4double drift_time = DistanceToAnode(pos_x)/1.6*1e-6; // 1.6 mm/us at 500 V/cm
  G4double sigma_t = TMath::Sqrt(2*DT*drift_time)*10;    // mm
  G4double sigma_l = TMath::Sqrt(2*DL*drift_time)*10;    // mm
  G4int binx1 = hist3DEdep->GetAxis(0)->FindBin(pos_x-5*sigma_l);
  G4int binx2 = hist3DEdep->GetAxis(0)->FindBin(pos_x+5*sigma_l);
  G4int biny1 = hist3DEdep->GetAxis(1)->FindBin(pos_y-5*sigma_t);
  G4int biny2 = hist3DEdep->GetAxis(1)->FindBin(pos_y+5*sigma_t);
  G4int binz1 = hist3DEdep->GetAxis(2)->FindBin(pos_z-5*sigma_t);
  G4int binz2 = hist3DEdep->GetAxis(2)->FindBin(pos_z+5*sigma_t);
  for (int zbin= binz1; zbin<= binz2; ++zbin) {
    for (int xbin= binx1; xbin<= binx2; ++xbin) {
      for (int ybin = biny1; ybin<= biny2; ++ybin) {
        // assume smearing at three directions is independent to each other
        G4double prob_x = ROOT::Math::normal_cdf(hist3DEdep->GetAxis(0)->GetBinLowEdge(xbin)+hist3DEdep->GetAxis(0)->GetBinWidth(xbin), sigma_l, pos_x) - 
                        ROOT::Math::normal_cdf(hist3DEdep->GetAxis(0)->GetBinLowEdge(xbin), sigma_l, pos_x);
        G4double prob_y = ROOT::Math::normal_cdf(hist3DEdep->GetAxis(1)->GetBinLowEdge(ybin)+hist3DEdep->GetAxis(1)->GetBinWidth(ybin), sigma_t, pos_y) - 
                        ROOT::Math::normal_cdf(hist3DEdep->GetAxis(1)->GetBinLowEdge(ybin), sigma_t, pos_y);
        G4double prob_z = ROOT::Math::normal_cdf(hist3DEdep->GetAxis(2)->GetBinLowEdge(zbin)+hist3DEdep->GetAxis(2)->GetBinWidth(zbin), sigma_t, pos_z) - 
                        ROOT::Math::normal_cdf(hist3DEdep->GetAxis(2)->GetBinLowEdge(zbin), sigma_t, pos_z);
        G4double weight = prob_x * prob_y * prob_z;
        hitClusterZX[0]->AddBinContent(hitClusterZX[0]->GetBin(zbin, xbin), weight*edep);
        hitClusterZX[idxPrim+1]->AddBinContent(hitClusterZX[idxPrim+1]->GetBin(zbin, xbin), weight*edep);
        hitClusterZY[0]->AddBinContent(hitClusterZY[0]->GetBin(zbin, ybin), weight*edep);
        hitClusterZY[idxPrim+1]->AddBinContent(hitClusterZY[idxPrim+1]->GetBin(zbin, ybin), weight*edep);
        G4int xyz_bin[3] = {xbin, ybin, zbin};
        G4double pos_center[3];
        for (G4int i= 0; i< 3; ++i) {
          pos_center[i] = hist3DEdep->GetAxis(i)->GetBinCenter(xyz_bin[i]);
        }
        hist3DEdep->Fill(pos_center, weight*edep);
      } // end of y-bins
    } // end of x-bins
  } // end of z-bins

  binx1 = std::max(vtxHitClusterZX[0]->GetYaxis()->FindBin(pos_x-vtx_x-5*sigma_l), 1);
  binx2 = std::min(vtxHitClusterZX[0]->GetYaxis()->FindBin(pos_x-vtx_x+5*sigma_l), vtxHitClusterZX[0]->GetYaxis()->GetNbins());
  biny1 = std::max(vtxHitClusterZY[0]->GetYaxis()->FindBin(pos_y-vtx_y-5*sigma_t), 1);
  biny2 = std::min(vtxHitClusterZY[0]->GetYaxis()->FindBin(pos_y-vtx_y+5*sigma_t), vtxHitClusterZY[0]->GetYaxis()->GetNbins());
  binz1 = std::max(vtxHitClusterZX[0]->GetXaxis()->FindBin(pos_z-vtx_z-5*sigma_t), 1);
  binz2 = std::min(vtxHitClusterZX[0]->GetXaxis()->FindBin(pos_z-vtx_z+5*sigma_t), vtxHitClusterZX[0]->GetXaxis()->GetNbins());
  for (int zbin= binz1; zbin<= binz2; ++zbin) {
    for (int xbin= binx1; xbin<= binx2; ++xbin) {
      double weight = (ROOT::Math::normal_cdf(vtxHitClusterZX[0]->GetYaxis()->GetBinLowEdge(xbin)+vtxHitClusterZX[0]->GetYaxis()->GetBinWidth(xbin), sigma_l, pos_x-vtx_x)
          - ROOT::Math::normal_cdf(vtxHitClusterZX[0]->GetYaxis()->GetBinLowEdge(xbin), sigma_l, pos_x-vtx_x))*
                     (ROOT::Math::normal_cdf(vtxHitClusterZX[0]->GetXaxis()->GetBinLowEdge(zbin)+vtxHitClusterZX[0]->GetXaxis()->GetBinWidth(zbin), sigma_t, pos_z-vtx_z)
          - ROOT::Math::normal_cdf(vtxHitClusterZX[0]->GetXaxis()->GetBinLowEdge(zbin), sigma_t, pos_z-vtx_z));
      vtxHitClusterZX[0]->AddBinContent(vtxHitClusterZX[0]->GetBin(zbin, xbin), weight*edep);
      vtxHitClusterZX[idxPrim+1]->AddBinContent(vtxHitClusterZX[idxPrim+1]->GetBin(zbin, xbin), weight*edep);
    }
    for (int ybin= biny1; ybin<= biny2; ++ybin) {
      double weight = (ROOT::Math::normal_cdf(vtxHitClusterZY[0]->GetYaxis()->GetBinLowEdge(ybin)+vtxHitClusterZY[0]->GetYaxis()->GetBinWidth(ybin), sigma_t, pos_y-vtx_y)
          - ROOT::Math::normal_cdf(vtxHitClusterZY[0]->GetYaxis()->GetBinLowEdge(ybin), sigma_t, pos_y-vtx_y))*
                     (ROOT::Math::normal_cdf(vtxHitClusterZX[0]->GetXaxis()->GetBinLowEdge(zbin)+vtxHitClusterZX[0]->GetXaxis()->GetBinWidth(zbin), sigma_t, pos_z-vtx_z)
          - ROOT::Math::normal_cdf(vtxHitClusterZX[0]->GetXaxis()->GetBinLowEdge(zbin), sigma_t, pos_z-vtx_z));
      vtxHitClusterZY[0]->AddBinContent(vtxHitClusterZY[0]->GetBin(zbin, ybin), weight*edep);
      vtxHitClusterZY[idxPrim+1]->AddBinContent(vtxHitClusterZY[idxPrim+1]->GetBin(zbin, ybin), weight*edep);
    }
  }
}

void PixelMap3D::FillEntryWithToySingleElectronTransportation(const Double_t* pos_xyz, const Double_t* vtx_xyz, Double_t edep, const Int_t idxPrim) 
{
  G4double pos_x = pos_xyz[0] - fPMShiftedCenterXYZ.x();
  G4double pos_y = pos_xyz[1] - fPMShiftedCenterXYZ.y();
  G4double pos_z = pos_xyz[2] - fPMShiftedCenterXYZ.z();
  G4double vtx_x = vtx_xyz[0] - fPMShiftedCenterXYZ.x();
  G4double vtx_y = vtx_xyz[1] - fPMShiftedCenterXYZ.y();
  G4double vtx_z = vtx_xyz[2] - fPMShiftedCenterXYZ.z();
  // https://lar.bnl.gov/properties/
  G4double DT = 13.2327; // Transverse diffusion coefficients @ 500 V/cm, cm^2/s
  G4double DL = 6.627;   // Longitudinal diffusion coeeficients @ 500 V/cm, cm^2/s
  
  G4double Wion = 23.6*1e-6;  // MeV/pair
  G4int avg_electrons = G4int(edep/Wion);
  G4int num_electrons = 0;
  if(avg_electrons < 20) num_electrons = G4int(G4Poisson(avg_electrons)+0.5);
  else  num_electrons = G4int(G4RandGauss::shoot(avg_electrons, sqrt(avg_electrons))+0.5);
  if(num_electrons < 0) num_electrons = 0 ;  
  
  G4double drift_time = DistanceToAnode(pos_x)/1.6*1e-6; // 1.6 mm/us at 500 V/cm
  G4double sigma_t = TMath::Sqrt(2*DT*drift_time)*10;    // mm
  G4double sigma_l = TMath::Sqrt(2*DL*drift_time)*10;    // mm
  for (int ielectron= 0; ielectron< num_electrons; ++ielectron) {
    G4double smeared_x = G4RandGauss::shoot(pos_x, sigma_l);
    G4double smeared_y = G4RandGauss::shoot(pos_y, sigma_t);
    G4double smeared_z = G4RandGauss::shoot(pos_z, sigma_t);
    hitClusterZX[0]->Fill(smeared_z, smeared_x, Wion);
    hitClusterZX[idxPrim+1]->Fill(smeared_z, smeared_x, Wion);
    hitClusterZY[0]->Fill(smeared_z, smeared_y, Wion);
    hitClusterZY[idxPrim+1]->Fill(smeared_z, smeared_y, Wion);
    vtxHitClusterZX[0]->Fill(smeared_z-vtx_z, smeared_x-vtx_x, Wion);
    vtxHitClusterZX[idxPrim+1]->Fill(smeared_z-vtx_z, smeared_x-vtx_x, Wion);
    vtxHitClusterZY[0]->Fill(smeared_z-vtx_z, smeared_y-vtx_y, Wion);
    vtxHitClusterZY[idxPrim+1]->Fill(smeared_z-vtx_z, smeared_y-vtx_y, Wion);
    G4double smeared_xyz[3] = {smeared_x, smeared_y, smeared_z};
    hist3DEdep->Fill(smeared_xyz, Wion);
  }
}

void PixelMap3D::WriteToFile(TFile* thefile, G4bool save3D, G4bool save2D)
{
  std::string dirname;
  if (save3D) {
    dirname = "edep3D/evt_"+std::to_string(fEvtID)+"/";
    thefile->mkdir(dirname.c_str());
    thefile->cd(dirname.c_str());
    hist3DEdep->Write();
  }
  if (save2D) {
    dirname = "edep2D/evt_"+std::to_string(fEvtID)+"/";
    thefile->mkdir(dirname.c_str());
    thefile->cd(dirname.c_str());
    for (int i=0; i<fNPrim+1; ++i) {
      if (hitClusterZX[i]->GetEntries()==0) hitClusterZX[i]->SetEntries(1);
      if (hitClusterZY[i]->GetEntries()==0) hitClusterZY[i]->SetEntries(1);
      hitClusterZX[i]->Write();
      hitClusterZY[i]->Write();
    }
    dirname = "edep2Dvtx/evt_"+std::to_string(fEvtID)+"/";
    thefile->mkdir(dirname.c_str());
    thefile->cd(dirname.c_str());
    for (int i=0; i<fNPrim+1; ++i) {
      if (vtxHitClusterZX[i]->GetEntries()==0) vtxHitClusterZX[i]->SetEntries(1);
      if (vtxHitClusterZY[i]->GetEntries()==0) vtxHitClusterZY[i]->SetEntries(1);
      vtxHitClusterZX[i]->Write();
      vtxHitClusterZY[i]->Write();
    }
  }
}

G4double PixelMap3D::DistanceToAnode(G4double x) {
  G4double anode_x[6] = {-900, -301, -299, 299, 301, 900};
  G4double distance = 9999;
  for (int i= 0; i< 6; ++i) { 
    G4double _distance = abs(x - anode_x[i]);
    distance = std::min(distance, _distance);
  }

  return distance;
}
