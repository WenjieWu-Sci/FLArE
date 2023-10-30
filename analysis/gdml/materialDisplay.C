void materialDisplay(TString filename,Bool_t checkoverlaps=kFALSE,TString label="geom-test.root" )
{
  Int_t PriKolor[] = {  2,  3,  4,  5,  6,  7,  8, 9, 28, 30, 38, 40, 41, 42, 46 };
  Int_t PriIndex = 0;
  std::map<TString,Int_t> Kolor;
  Kolor["Steel"] = kGreen;
  Kolor["Copper"] = kYellow;
  Kolor["Aluminum"] = kRed;
  Kolor["FR4"] = kGray;

  //TGeoManager *geo2 = TGeoManager::Import(filename, "geo2");
  TFile* sfile = new TFile("nd_hall_only_lar.root","r");
  TGeoManager* geo2 = (TGeoManager*)sfile->Get("Default");
  if ( checkoverlaps )
  {
    geo2->CheckOverlaps(1e-5,"d");
    geo2->CheckOverlaps(1e-5,"s10000000");
    geo2->PrintOverlaps();
  }
  geo2->SetVisLevel(20);
  TGeoVolume *volume = NULL;
  TObjArray *volumes = geo2->GetListOfVolumes();
  Int_t nvolumes = volumes->GetEntries();
  for ( int i = 0; i < nvolumes; i++ )
  {
    volume = (TGeoVolume*)volumes->At(i);
    volume->SetVisContainers(kTRUE);
    if ( TString(volume->GetName()).Contains("DetEnclosure"))
    {
      volume->SetVisibility(kFALSE);
      continue;
    }
    if ( TString(volume->GetMaterial()->GetName()).Contains("Air"))
    {
      volume->SetVisibility(kFALSE);
      continue;
    }
    Int_t daughters = volume->GetNdaughters();
    cout << volume->GetName() << " NDaughters = " << volume->GetMaterial()->GetName() << " " << daughters << endl;
    volume->SetLineColor(Kolor[volume->GetMaterial()->GetName()]);
    switch ( daughters )
    {
      case 0: volume->SetTransparency(100);
      case 1: volume->SetTransparency(80);
      case 2: volume->SetTransparency(60);
      case 3: volume->SetTransparency(40);
      default: volume->SetTransparency(10);
    }

  }
  geo2->Export(label);
  geo2->GetTopVolume()->Draw("ogl");
}
