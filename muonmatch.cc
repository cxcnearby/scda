#define wcdahits_cxx
#define wcdapls_cxx
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
#include "wcdahits.h"
#include "wcdapls.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TNtupleD.h>
#include <TStyle.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

double smpmtx_jd[26][4][17];
double smpmty_jd[26][4][17];
double bigpmtx[101][10];
double bigpmty[101][10];
std::vector<int> smevt;
std::vector<Long64_t> sectime;
std::vector<Long64_t> smievt;
std::vector<Long64_t> smtime;
std::vector<Long64_t> smsecond;
std::vector<Long64_t> smns;
std::vector<int> smanode_peak;
std::vector<int> smanode_ped;
std::vector<int> smdynode_peak;
std::vector<int> smdynode_ped;
std::vector<int> smfee;
std::vector<int> smdb;
std::vector<int> smpmt;
int big_nhit;
int sm_nhit;
// TFile *f;
// TTree *tr;
long bigsecond_evt0;
double bigns_evt0;
long bigsecond_evtend;
double bigns_evtend;
int smevt0 = 0;
int bigevt0 = 0;
int bigevt1 = 100;

void wcdahits::Loop0() {
  if (fChain == 0)
    return;
  LoadTree(bigevt0);
  fChain->GetEntry(bigevt0);
  bigsecond_evt0 = second;

  // bigevt1=fChain->GetEntriesFast()-10;
  LoadTree(bigevt1);
  fChain->GetEntry(bigevt1);
  bigsecond_evtend = second;
}

void wcdahits::Loop() {
#include "bigpmtpos.h"
#include "smpmtpos.h"

  TFile *fout = new TFile("/scratchfs/bes/lixr/tmp1/fmuon.root", "recreate");
  TTree *tr = new TTree("tr", "tr");
  int big_evt;
  int sm_evt;
  int ievt;
  double big_anode;
  double big_dynode;
  double big_npe;
  double sm_anode;
  double sm_dynode;
  double sm_npe;
  double big_pmtx;
  double big_pmty;
  double sm_pmtx;
  double sm_pmty;
  Long64_t big_second;
  double big_ns;
  Long64_t sm_second;
  double sm_ns;
  double dftime;
  int sm_nhit = 0;

  tr->Branch("big_evt", &big_evt);
  tr->Branch("sm_evt", &sm_evt);
  tr->Branch("ievt", &ievt);
  tr->Branch("dftime", &dftime);
  tr->Branch("big_anode", &big_anode);
  tr->Branch("big_dynode", &big_dynode);
  tr->Branch("big_npe", &big_npe);
  tr->Branch("sm_anode", &sm_anode);
  tr->Branch("sm_dynode", &sm_dynode);
  tr->Branch("sm_npe", &sm_npe);
  tr->Branch("big_pmtx", &big_pmtx);
  tr->Branch("big_pmty", &big_pmty);
  tr->Branch("sm_pmtx", &sm_pmtx);
  tr->Branch("sm_pmty", &sm_pmty);
  tr->Branch("big_second", &big_second);
  tr->Branch("big_ns", &big_ns);
  tr->Branch("sm_second", &sm_second);
  tr->Branch("sm_ns", &sm_ns);

  if (fChain == 0)
    return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  for (Long64_t jentry = bigevt0; jentry < bigevt1; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
      break;
    fChain->GetEntry(jentry);

    Long64_t bigsecond = second;
    double bigns = low_th_fine_time * 0.333 + coarse_time;
    cout << "ibig=" << jentry << ",bigsec=" << bigsecond << ",bigns=" << bigns
         << ",smsize=" << smsecond.size() << endl;

    sm_nhit = 0;
    double timewin = 4000;
    double dftime = 10000000;
    for (int i = smevt0; i < smsecond.size(); i++) {

      dftime =
          (smsecond[i] + 1 - bigsecond) * 10000000000LL + (smns[i] - bigns);
      cout << "ism=" << i << ",dftime=" << dftime << endl;
      if (fabs(dftime) < timewin) {

        big_evt = jentry;
        sm_evt = smievt[i];
        big_second = bigsecond;
        big_ns = bigns;
        sm_second = smsecond[i] + 1;
        sm_ns = smns[i];

        big_anode = anode_charge;
        big_dynode = dynode_charge;
        sm_anode = smanode_peak[i] - smanode_ped[i];
        sm_dynode = smdynode_peak[i] - smdynode_ped[i];

        sm_pmtx = smpmtx_jd[smfee[i]][smdb[i]][smpmt[i]];
        sm_pmty = smpmty_jd[smfee[i]][smdb[i]][smpmt[i]];

        big_pmtx = bigpmtx[fee][ch];
        big_pmty = bigpmty[fee][ch];

        Double_t q0 = big_anode / 22.0;
        Double_t q1 = big_dynode / 0.726 / 0.69;
        Double_t q = q0;
        if (q > 160 && q1 > 0)
          q = q1;
        if (q <= 0)
          q = 1.e-20;
        big_npe = q;

        sm_npe = sm_anode * 2.0;
        sm_nhit++;

        tr->Fill();
      }

      if (dftime > timewin) {
        smevt0 = i;
        break;
      }
    }
  }

  fout->Write();
}

void wcdapls::Loop() {
  int fee_1;
  int db_1;
  int pmt_1;
  Long64_t time_1;
  Long64_t second_1;
  Long64_t ns_1;
  double anode_1;
  double dynode_1;
  int smevt_1;
  int evt_1;
  smevt.clear();
  sectime.clear();

  std::vector<int> tmpsmevt;
  std::vector<long> tmpsectime;
  tmpsmevt.clear();
  tmpsectime.clear();

  double dfsecond = -100000;
  Long64_t nentries = fChain->GetEntriesFast();
  // nentries=1000;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    fChain->GetEntry(jentry);
    dfsecond = (second - bigsecond_evtend);
    if (dfsecond > 100)
      break;
    cout << ",df0=" << second - bigsecond_evt0
         << ",df1=" << second - bigsecond_evtend << endl;

    smevt.push_back(jentry);
    sectime.push_back(second * 1000000000LL + ns * 20LL);

    long tmp1;
    int tmp2;
    if (jentry == 1 && (sectime[jentry] < sectime[jentry - 1])) {
      tmp1 = sectime[jentry];
      sectime[jentry] = sectime[jentry - 1];
      sectime[jentry - 1] = tmp1;
      tmp2 = smevt[jentry];
      smevt[jentry] = smevt[jentry - 1];
      smevt[jentry - 1] = tmp2;
    }

    if (jentry > 1) {
      if (sectime[jentry] >= sectime[jentry - 1])
        continue;

      if (sectime[jentry] < sectime[jentry - 1]) {
        tmpsmevt.clear();
        tmpsectime.clear();
        int j;
        for (j = 1; j <= jentry; j++) {
          tmpsectime.push_back(sectime[jentry - j]);
          tmpsmevt.push_back(smevt[jentry - j]);
          if (j < jentry && sectime[jentry] <= sectime[jentry - j] &&
              (sectime[jentry] >= sectime[jentry - j - 1])) {
            break;
          }
        }
        if (j == (jentry + 1))
          j = j - 1;
        sectime[jentry - j] = sectime[jentry];
        smevt[jentry - j] = smevt[jentry];
        int tmpnum = 0;
        for (int k = jentry; k > jentry - j; k--) {
          sectime[k] = tmpsectime[tmpnum];
          smevt[k] = tmpsmevt[tmpnum];
          tmpnum++;
        }
      }
    }
  }

  smievt.clear();
  smtime.clear();
  smsecond.clear();
  smns.clear();
  smanode_peak.clear();
  smanode_ped.clear();
  smdynode_peak.clear();
  smdynode_ped.clear();
  smfee.clear();
  smdb.clear();
  smpmt.clear();

  // fee:db:pmt:second:ns:anode:dynode:smevt:evt
  for (int i = 0; i < smevt.size(); i++) {
    // cout<<"111"<<endl;
    fChain->GetEntry(smevt[i]);
    // if((second-bigsecond_evt0)>-5)
    {
      smievt.push_back(smevt[i]);
      smtime.push_back(second * 1000000000LL + ns * 20LL);
      smsecond.push_back(second);
      smns.push_back(ns * 20LL);
      smanode_peak.push_back(anode_peak);
      smanode_ped.push_back(anode_ped);
      smdynode_peak.push_back(dynode_peak);
      smdynode_ped.push_back(dynode_ped);
      smfee.push_back(fee);
      smdb.push_back(db);
      smpmt.push_back(pmt);
      // fee_1=fee; db_1=db; pmt_1=pmt; time_1=time; second_1=second; ns_1=ns;
      // anode_1=anode_peak-anode_ped; dynode_1=dynode_peak-dynode_ped;
      // smevt_1=smevt[i]; evt_1=i;
      // trpls->Fill();
    }
  }
  // foutsm->Write();

  /*
  long dftime;
  for(int i=0; i<smsecond.size(); i++)
  {
  dftime=(smsecond[i]-smsecond[i-1])*1000000000LL+(smns[i]-smns[i-1]);
  if(dftime<0) {cout<<"noo"<<endl;

  cout<<"dftime2="<<(smtime[i]-smtime[i-1])<<endl;
  }

  }
  */

  std::cout << "nentries=" << nentries << ",length=" << smsecond.size()
            << ",smevt0=" << smevt0 << std::endl;
}
int main(int argc, char *argv[]) {
  cout << "x111" << endl;

  string bpath = argv[1], bfile = argv[2], spath = argv[3], sfile = argv[4];

  wcdahits big(bpath, bfile);
  wcdapls sm(spath, sfile);

  big.Loop0();
  sm.Loop();
  big.Loop();

  return 0;
}
