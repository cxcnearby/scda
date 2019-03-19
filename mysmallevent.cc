#define wcdaevent_cxx
#define wcdapls_cxx
#include "wcdaevent.h"
#include "wcdapls.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>

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
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

double smpmtx_jd[26][4][17];
double smpmty_jd[26][4][17];
std::vector<int> smevt;
std::vector<long> sectime;
std::vector<long> smtime;
std::vector<long> ssmsecond;
std::vector<long> ssmns;
std::vector<int> smanode_peak;
std::vector<int> smanode_ped;
std::vector<int> smdynode_peak;
std::vector<int> smdynode_ped;
std::vector<int> smfee;
std::vector<int> smdb;
std::vector<int> smpmt;

void wcdaevent::Loop0() {}

void wcdaevent::Loop() {}

void wcdapls::Loop() {
#include "smpmtpos.h"
  double x, y;
  smevt.clear();
  sectime.clear();

  std::vector<int> tmpsmevt;
  std::vector<long> tmpsectime;
  tmpsmevt.clear();
  tmpsectime.clear();

  // CXC 19/3/16

  UInt_t b_fee, b_db, b_pmt, b_anode_peak, b_anode_ped, b_dynode_peak,
      b_dynode_ped, b_sec, b_ns;
  Float_t b_x, b_y;
  Long64_t b_entry, b_tot;
  ULong64_t b_gtime;
  Long64_t b_time_gap;

  TFile *f_smpmtsimple = new TFile("smsimple.root", "recreate");
  TTree *t_b = new TTree("tb", "useful parameters");

  t_b->Branch("entry_number", &b_entry, "b_entry/L");
  t_b->Branch("total_number", &b_tot, "b_tot/L");
  t_b->Branch("fee", &b_fee, "b_fee/i");
  t_b->Branch("db", &b_db, "b_db/i");
  t_b->Branch("pmt", &b_pmt, "b_ch/i");
  t_b->Branch("X_coordinate", &b_x, "b_x/F");
  t_b->Branch("Y_coordinate", &b_y, "b_y/F");
  t_b->Branch("anode_charge_peak", &b_anode_peak, "b_anode_peak/i");
  t_b->Branch("anode_charge_ped", &b_anode_ped, "b_anode_ped/i");
  t_b->Branch("dynode_charge_peak", &b_dynode_peak, "b_dynode_peak/i");
  t_b->Branch("dynode_charge_ped", &b_dynode_ped, "b_dynode_ped/i");
  t_b->Branch("second", &b_sec, "b_sec/i");
  t_b->Branch("nano_second", &b_ns, "b_ns/i");
  t_b->Branch("given_time", &b_gtime, "b_gtime/l");
  t_b->Branch("time_gap", &b_time_gap, "b_time_gap/L");

  ULong64_t tmp_gtime = 0;

  // CXC 19/3/16

  Long64_t nentries = fChain->GetEntriesFast();
  b_tot = 0; // CXC 19/3/16

  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    fChain->GetEntry(jentry);
    b_entry = jentry; // CXC 19/3/16

    x = smpmtx_jd[fee][db][pmt];
    y = smpmty_jd[fee - 1][db][pmt];
    /*   cout << jentry << " " << x << " " << y << " " << anode_peak << " "
           << anode_ped << " " << dynode_peak << " " << dynode_ped << " " << fee
           << " " << db << " " << pmt << " " << second << " " << ns << endl;
   */
    // CXC 19/3/16
    b_fee = fee;
    b_db = db;
    b_pmt = pmt;
    b_x = smpmtx_jd[fee][db][pmt];
    b_y = smpmty_jd[fee - 1][db][pmt];
    b_anode_peak = anode_peak;
    b_anode_ped = anode_ped;
    b_dynode_peak = dynode_peak;
    b_dynode_ped = dynode_ped;
    b_sec = second;
    b_ns = ns;
    b_gtime = second * 1000000000LL + ns * 20LL;
    if (b_entry == 0)
      tmp_gtime = b_gtime;
    b_time_gap = b_gtime - tmp_gtime;
    if (b_gtime < tmp_gtime || 1) {
      cout << jentry << " " << b_time_gap << " " << x << " " << y << " "
           << anode_peak << " " << anode_ped << " " << dynode_peak << " "
           << dynode_ped << " " << fee << " " << db << " " << pmt << " "
           << second << " " << ns << endl;
    }
    tmp_gtime = b_gtime;
    // CXC 19/3/16

    // CXC 19/3/16
    t_b->Fill();
    b_tot++;
    // CXC 19/3/16
    /*
            smevt.push_back(jentry);
            sectime.push_back(time);

            long tmp1; int tmp2;
            if(jentry==1&&(sectime[jentry]<sectime[jentry-1])){
                    //cout<<"ientry==1"<<"time is wrong."<<endl;
                    tmp1=sectime[jentry];   sectime[jentry]=sectime[jentry-1];
       sectime[jentry-1]=tmp1; tmp2=smevt[jentry];
       smevt[jentry]=smevt[jentry-1]; smevt[jentry-1]=tmp2;
            }
            if(jentry>1&&sectime[jentry]<sectime[jentry-1]){
                    //cout<<"ientry= "<<jentry<<"time is wrong."<<endl;
                    tmpsmevt.clear();
                    tmpsectime.clear();
                    int j;
                    for(j=1; j<=jentry; j++)
                    {
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
    */
  }
  f_smpmtsimple->Write(); // CXC 19/3/16

  /*
  cout<< "time check is done." <<endl;
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

  for(int i=0; i<smevt.size(); i++)
  {
  fChain->GetEntry(smevt[i]);
  smtime.push_back(time);
  smanode_peak.push_back(anode_peak);
  smanode_ped.push_back(anode_ped);
  smdynode_peak.push_back(dynode_peak);
  smdynode_ped.push_back(dynode_ped);
  smfee.push_back(fee);
  smdb.push_back(db);
  smpmt.push_back(pmt);
  cout<<fee<<" "<<db<<" "<<pmt<<" "<<anode_peak<<endl;
  }

  std::cout<<"nentries="<<nentries<<",length="<<sectime.size()<<std::endl;
  */
}

int main() {
  // cout << "x111" << endl;
  wcdapls m;
  m.Loop();
  return 0;
}
