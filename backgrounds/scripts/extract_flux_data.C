#include <iostream>
#include <string>
#include <fstream>
#include <vector>

// ---------------------------------------------------------------------------------
// global defintions 
int gen;
double E, w;
double x, y, z;
int evt, pid, run;
double xdir, ydir;
double xdec, ydec, zdec;
double xint, yint, zint;
int decid, intid;

// ----------------------------------------------------------------------------------

// for string delimiter
std::vector<std::string> split (std::string s, std::string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
    token = s.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }

  res.push_back (s.substr (pos_start));
  return res;
}

void read_and_fill(std::string filepath, TTree *t){
  
  std::ifstream ifile(filepath.c_str(),ios_base::in);
  
  // skip header
  std::string line; 
  int start=0;
  while(std::getline(ifile,line)){
    if(line.find("#")!=std::string::npos) continue;
    else{
       start = ifile.tellg();  
       break;
    }
  }
  
  while( ifile >> run >> evt >> pid >> gen >> E >> w >> x >> y 
          >> xdir >> ydir >> z >> xdec >> ydec >> zdec 
          >> decid >> xint >> yint >> zint >> intid ){
    t->Fill();
  }
}

void setup_tree(TTree *t){

  t->Branch("run",&run);
  t->Branch("evt",&evt);
  t->Branch("pid",&pid);
  t->Branch("gen_n", &gen);
  t->Branch("kinE", &E);
  t->Branch("weight", &w);
  t->Branch("x", &x);
  t->Branch("y", &y);
  t->Branch("z", &z);
  t->Branch("xdircos",&xdir);
  t->Branch("ydircos",&ydir);
  t->Branch("xdec", &xdec);
  t->Branch("ydec", &ydec);
  t->Branch("zdec", &zdec);
  t->Branch("xint", &xint);
  t->Branch("yint", &yint);
  t->Branch("zint", &zint);
  t->Branch("decid", &decid);
  t->Branch("intid", &intid);

}

void extract_flux_data(){

  TFile *fn = new TFile("Nmu_data.root", "RECREATE");
  std::string file_n = "/eos/user/m/mvicenzi/FPF_FLUKA/muons/flux_v2/Nm_FPF_entrance-cavern.dat";
  TTree *mu_minus = new TTree("mu_minus","negative mu"); 
  setup_tree(mu_minus);
  read_and_fill(file_n, mu_minus);
  fn->Write();
  fn->Close();
  
  TFile *fp = new TFile("Pmu_data.root", "RECREATE");
  std::string file_p = "/eos/user/m/mvicenzi/FPF_FLUKA/muons/flux_v2/Pm_FPF_entrance-cavern.dat";
  TTree *mu_plus = new TTree("mu_plus","positive mu"); 
  setup_tree(mu_plus);
  read_and_fill(file_p, mu_plus);
  fp->Write();
  fp->Close();

  TFile *fnn = new TFile("Neutron_data.root", "RECREATE");
  std::string file_nn = "/eos/user/m/mvicenzi/FPF_FLUKA/neutrons/neutrons_FPF_entrance-cavern.dat";
  TTree *neutrons = new TTree("neut","neutrons");
  setup_tree(neutrons);
  read_and_fill(file_nn, neutrons);
  fnn->Write();
  fnn->Close();
  
}
