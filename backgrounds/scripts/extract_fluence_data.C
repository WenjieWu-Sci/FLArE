#include <iostream>
#include <string>
#include <fstream>
#include <vector>

// ---------------------------------------------------------------------------------
// global definitions
double xmin, xmax, xbinsize;
double ymin, ymax, ybinsize;
double zmin, zmax, zbinsize;
int xbins, ybins, zbins;
std::vector<double> xc, yc, zc;
std::vector<double> val;

// ----------------------------------------------------------------------------------

double apply_normalization(double value){

  //input: number of mus per cm2
  //The numbers in the files must be normalized in order to get the fluence rate for 5Lo considering:
  //- the instantaneous luminosity: 5e34 cm-2 s-1
  double n1 = 5.0e34; //cm-2 s-1
  //- the cross section of the p-p collision: 85 mb = 85e-3*1e-24 cm2
  double n2 = 85e-3*1e-24; //cm2
  //-  a normalization factor coming from the 2 step simulation approach: 2104211.0/28.0e6
  double n3 = 2104211.0/28.0e6;
  //therefore: 
  double N =n1*n2*n3;
  
  return N*value; // n of mus cm-2 s-1 (for 5Lo)

}

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

std::vector<double> parse_header(std::string filepath){

  std::ifstream ifile(filepath.c_str(),ios_base::in);
  std::string line;
  std::vector<double> bin_info;
  int eoh = 0;
  while(std::getline(ifile,line)){
    
    if(line.find("track-length")!=std::string::npos) {
      eoh = ifile.tellg();
      break;
    } 
    if(line.find("coordinate")!=std::string::npos) {

      std::vector<std::string> ss = split(line," ");
      bin_info.push_back(std::strtod(ss[9].c_str(),nullptr));  //bin min limit
      bin_info.push_back(std::strtod(ss[12].c_str(),nullptr)); //bin max limit
      bin_info.push_back(std::strtod(ss[17].c_str(),nullptr)); //nbins
      bin_info.push_back(std::strtod(ss[20].c_str(),nullptr)); //bin size
      for(int i=0; i<ss.size(); i++) std::cout << /*i << ": " <<*/ ss[i] << " ";
      std::cout << std::endl;
    }
  }

  bin_info.push_back(eoh);
  return bin_info;
}


void read_and_fill(std::string filepath){

  std::vector<double> bins = parse_header(filepath);  
  //for (auto it = bins.begin(); it!= bins.end(); it++) std::cout << *it << std::endl;
  xmin = bins[0]; xmax = bins[1]; xbinsize = bins[3];
  ymin = bins[4]; ymax = bins[5]; ybinsize = bins[7];
  zmin = bins[8]; zmax = bins[9]; zbinsize = bins[11];
  xbins = int(bins[2]); ybins = int(bins[6]); zbins = int(bins[10]);
 
  int eoh = bins[12];

  std::ifstream ifile(filepath.c_str(),ios_base::in);
  ifile.seekg(eoh);
  double v;

  std::cout << "nx " << xbins << " ny " << ybins << " nz " << zbins << std::endl;
 
  for(int iz=0; iz<zbins; iz++){
    for(int iy=0; iy<ybins; iy++){
      for(int ix=0; ix<xbins; ix++){

	if( ifile.good() ) ifile >> v;

	xc.push_back(xmin + (0.5+ix)*xbinsize);
	yc.push_back(ymin + (0.5+iy)*ybinsize);
	zc.push_back(zmin + (0.5+iz)*zbinsize);
	val.push_back(apply_normalization(v));
    	
      }
    }
  }
}

void setup_tree(TTree *t){

  xc.clear();
  yc.clear();
  zc.clear();
  val.clear();

  t->Branch("xmin", &xmin);
  t->Branch("xmax", &xmax);
  t->Branch("xbins", &xbins);
  t->Branch("xsize", &xbinsize);
  t->Branch("ymin", &ymin);
  t->Branch("ymax", &ymax);
  t->Branch("ybins", &ybins);
  t->Branch("ysize", &ybinsize);
  t->Branch("zmin", &zmin);
  t->Branch("zmax", &zmax);
  t->Branch("zbins", &zbins);
  t->Branch("zsize", &zbinsize);
  t->Branch("x", &xc);
  t->Branch("y", &yc);
  t->Branch("z", &zc);
  t->Branch("f", &val);

}

void extract_fluence_data(){

  TFile *f = new TFile("mu_fluence.root", "RECREATE");

  std::string file_n = "/eos/user/m/mvicenzi/FPF_FLUKA/muons/fluence/usrbin_Nmuons.txt";
  std::string file_p = "/eos/user/m/mvicenzi/FPF_FLUKA/muons/fluence/usrbin_Pmuons.txt";
  TTree *mu_minus = new TTree("mu_minus","negative mu"); 
  TTree *mu_plus = new TTree("mu_plus","positive mu"); 

  setup_tree(mu_minus);
  read_and_fill(file_n);
  mu_minus->Fill();

  setup_tree(mu_plus);
  read_and_fill(file_p);
  mu_plus->Fill();

  f->Write();
  f->Close();

}
