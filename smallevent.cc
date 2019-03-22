#define wcdaevent_cxx
#define wcdapls_cxx

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "TCanvas.h"
#include "TChain.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMath.h"
#include "TPad.h"
#include "TStyle.h"
#include "TTree.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>

#include "wcdaevent.h"
#include "wcdapls.h"

#include "definition.h"
//#include "std_lib_facilities.h"

using namespace std;

double smpmtx_jd[26][4][17];
double smpmty_jd[26][4][17];
int smpmtig_jd[26][4][17];
std::vector<int> smevt;
std::vector<long> sectime;
std::vector<long> smtime;
std::vector<long> ssmsecond;
std::vector<long> ssmns;
std::vector<int> smanode_peak;
std::vector<int> smanode_ped;
std::vector<int> smanode;
std::vector<int> smdynode_peak;
std::vector<int> smdynode_ped;
std::vector<int> smdynode;
std::vector<int> smfee;
std::vector<int> smdb;
std::vector<int> smpmt;
std::vector<double> smpmtx;
std::vector<double> smpmty;
std::vector<double> smpmtz;

long bigsecond_evt0;
double bigns_evt0;
long bigsecond_evtend;
double bigns_evtend;
int smevt0;
int bigevt0 = 0;

void wcdaevent::Loop0() {
  if (fChain == 0)
    return;

  // int bigevt=17476;
  LoadTree(bigevt0);
  fChain->GetEntry(bigevt0);
  bigsecond_evt0 = hits_second[0];
  bigns_evt0 = hits_low_th_fine_time[0] * 0.333 + hits_coarse_time[0] * 16.;

  LoadTree(fChain->GetEntriesFast() - 1);
  fChain->GetEntry(fChain->GetEntriesFast() - 1);
  bigsecond_evtend = hits_second[0];
  bigns_evtend = hits_low_th_fine_time[0] * 0.333 + hits_coarse_time[0] * 16.;
}

void wcdaevent::Loop() {}

void wcdapls::Loop() {
#include "smpmtpos.h"

  // modified by Wei Liu, 2019.03.14
  double pmtx_, pmty_, ns_, time_, time2_, dftime_ = 5e3 /*ns*/;
  int eve_, nhit_, anode_, dynode_, fee_, ch_, sec_;

  vector<struct big> bigevent;
  struct big bevt_;

  stringstream fname;
  ifstream rf;
  ofstream wf, wf2;
  fname << "big_event2.dat";
  rf.open(fname.str().c_str(), ios_base::in);
  if (rf.is_open()) {
    while (rf >> eve_ >> nhit_ >> pmtx_ >> pmty_ >> anode_ >> dynode_ >> fee_ >>
           ch_ >> sec_ >> ns_) {
      bevt_.eve = eve_;
      bevt_.nhit = nhit_;
      bevt_.pmtx = pmtx_;
      bevt_.pmty = pmty_;
      bevt_.anode = anode_;
      bevt_.dynode = dynode_;
      bevt_.fee = fee_;
      bevt_.ch = ch_;
      bevt_.sec = sec_;
      bevt_.ns = ns_;
      time_ = sec_ * 1e9 + ns;
      bevt_.time = time_;

      bigevent.push_back(bevt_);
    }
  } else{}
//    error("can't open data file : ",
//          fname.str()); // file is implicitly closed when leaving the function.
  rf.close();

#if 0
  for(int i = 0; i != bigevent.size(); ++i)
    cout << bigevent[i].eve << " " << bigevent[i].nhit << " " << bigevent[i].pmtx << " " << bigevent[i].pmty << " " << bigevent[i].anode << " " << bigevent[i].dynode << " " << bigevent[i].fee << " " << bigevent[i].ch << " " << bigevent[i].sec << " " << setprecision(15) << bigevent[i].ns << " " << bigevent[i].time << " " << endl;
#endif

  fname.str("");
  fname << "smallevent_D100.txt";
  wf.open(fname.str().c_str(), ios_base::out);

  fname.str("");
  fname << "eventmatch_D100.txt";
  wf2.open(fname.str().c_str(), ios_base::out);

  smevt.clear();
  sectime.clear();

  std::vector<int> tmpsmevt;
  std::vector<long> tmpsectime;
  tmpsmevt.clear();
  tmpsectime.clear();

#if 0
  Long64_t nentries = fChain->GetEntriesFast();
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    fChain->GetEntry(jentry); 
    smevt.push_back(jentry);
    sectime.push_back(time);
    
    long tmp1; int tmp2; 
    if(jentry==1&&(sectime[jentry]<sectime[jentry-1])){
      //cout<<"ientry==1"<<"time is wrong."<<endl; 
      tmp1=sectime[jentry];   sectime[jentry]=sectime[jentry-1];   sectime[jentry-1]=tmp1;
      tmp2=smevt[jentry];   smevt[jentry]=smevt[jentry-1]; smevt[jentry-1]=tmp2;
    }
    if(jentry>1&&sectime[jentry]<sectime[jentry-1]){
      //cout<<"ientry= "<<jentry<<"time is wrong."<<endl;
      tmpsmevt.clear();
      tmpsectime.clear();
      int j;
      for(j=1; j<=jentry; j++){
	tmpsectime.push_back(sectime[jentry-j]);
	tmpsmevt.push_back(smevt[jentry-j]);
	if(j<jentry&&sectime[jentry]<=sectime[jentry-j]&&(sectime[jentry]>=sectime[jentry-j-1])) 
	  { break;}
      }
      if(j==(jentry+1)) j=j-1;
      sectime[jentry-j]=sectime[jentry];
      smevt[jentry-j]=smevt[jentry];
      int tmpnum=0; 
      for(int k=jentry; k>jentry-j; k--){
	sectime[k]=tmpsectime[tmpnum];
	smevt[k]=tmpsmevt[tmpnum];
	tmpnum++;
      }
    }
  }

  cout<< "time check is done." <<endl;

  for(int i=0; i<smevt.size(); i++){
    fChain->GetEntry(smevt[i]);

    smtime.push_back(time);
    smanode_peak.push_back(anode_peak);
    smanode_ped.push_back(anode_ped);
    smdynode_peak.push_back(dynode_peak);
    smdynode_ped.push_back(dynode_ped);
    smfee.push_back(fee);
    smdb.push_back(db);
    smpmt.push_back(pmt);
  }
#endif

  smtime.clear();
  ssmsecond.clear();
  ssmns.clear();
  smanode_peak.clear();
  smanode_ped.clear();
  smdynode_peak.clear();
  smdynode_ped.clear();
  smfee.clear();
  smdb.clear();
  smpmt.clear();

  Long64_t nentries = fChain->GetEntriesFast();
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    fChain->GetEntry(jentry);

    if ((anode_peak - anode_ped) > 0) {
      smevt.push_back(jentry);

      sectime.push_back(time);
      ssmsecond.push_back(second);
      ssmns.push_back(ns * 20);
      time2_ = second * 1e9 + ns * 20 + 1e9;

      smfee.push_back(fee);
      smdb.push_back(db);
      smpmt.push_back(pmt);

      pmtx_ = smpmtx_jd[fee][db][pmt];
      smpmtx.push_back(pmtx_);
      pmty_ = smpmty_jd[fee][db][pmt];
      smpmty.push_back(pmty_);
      smpmtz.push_back(0);

      smanode_peak.push_back(anode_peak);
      smanode_ped.push_back(anode_ped);
      anode_ = anode_peak - anode_ped;
      smanode.push_back(anode_);

      smdynode_peak.push_back(dynode_peak);
      smdynode_ped.push_back(dynode_ped);
      dynode_ = dynode_peak - dynode_ped;
      smdynode.push_back(dynode_);

      wf << jentry << " " << pmtx_ << " " << pmty_ << " " << fee << " " << db
         << " " << pmt << " " << anode_peak << " " << anode_ped << " " << anode_
         << " " << dynode_peak << " " << dynode_ped << " " << dynode_ << " "
         << time << " " << second << " " << ns << " " << time2_ << " " << endl;

      for (int i = 0; i < bigevent.size(); ++i) {
        if (bigevent[i].pmtx == pmtx_ && bigevent[i].pmty == pmty_)
          if (bigevent[i].sec == second + 1) // second equal
            if (fabs(ns * 20 - bigevent[i].ns) <= dftime_) {
              wf2 << bigevent[i].eve << "\t" << bigevent[i].nhit << "\t"
                  << bigevent[i].pmtx << "\t" << bigevent[i].pmty << "\t"
                  << bigevent[i].fee << "\t" << bigevent[i].ch << "\t"
                  << bigevent[i].anode << "\t" << bigevent[i].dynode << "\t"
                  << bigevent[i].sec << "\t" << setprecision(9)
                  << bigevent[i].ns << "\t" << bigevent[i].time << "\t";
              wf2 << jentry << "\t" << fee << "\t" << db << "\t" << pmt << "\t"
                  << anode_ << "\t" << dynode_ << "\t" << second + 1 << "\t"
                  << ns * 20 << "\t" << endl;
            }
      }
    }
  }
  wf.close();
  wf2.close();

  std::cout << "nentries=" << nentries << ",length=" << sectime.size()
            << std::endl;
}

int main(int argc, char *argv[]) {
  // record starting time
  clock_t start, end;
  start = clock();

  time_t tp1, tp2;
  tp1 = time(NULL);

  cout << "x111" << endl;

  string bpath = argv[1], bfile = argv[2], spath = argv[3], sfile = argv[4];

  wcdaevent big(bpath, bfile);
  wcdapls sm(spath, sfile);

  big.Loop0();
  big.Loop();
  sm.Loop();

  // record ending time
  end = clock();
  double tim = (double)(end - start) / CLOCKS_PER_SEC;

  tp2 = time(NULL);
  printf("time is %f, %f\n", tim, difftime(tp2, tp1));

  return 0;
}
