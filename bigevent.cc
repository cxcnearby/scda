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

double bigpmtx[101][10];
double bigpmty[101][10];
int bigevt0 = 0;

void wcdaevent::Loop() {
#include "bigpmtpos.h"

  // cout<< "wcdaevent-Loop Begin" << endl;
  int ievt = 0;
  int big_evt;
  const Int_t kMaxTrack = 10000;

  long big_second0;
  double big_ns0;
  long big_second[kMaxTrack];
  double big_ns[kMaxTrack];
  double big_pmtx[kMaxTrack];
  double big_pmty[kMaxTrack];
  double big_pmtz[kMaxTrack];
  int big_anode[kMaxTrack];
  int big_dynode[kMaxTrack];
  int big_fee[kMaxTrack];
  int big_ch[kMaxTrack];

  // CXC 19/3/16

  Int_t b_hit, b_fee, b_ch, b_anode, b_dynode;
  Float_t b_x, b_y;
  Double_t b_ns;
  Long64_t b_entry, b_tot, b_sec;
  ULong64_t b_evt, b_gtime;

  TFile *f_bigpmtsimple = new TFile("bigsimple.root", "recreate");
  TTree *t_b = new TTree("tb", "useful parameters");

  t_b->Branch("entry_number", &b_entry, "b_entry/L");
  t_b->Branch("event_time", &b_evt, "b_evt/l");
  t_b->Branch("hit_number", &b_hit, "b_hit/I");
  t_b->Branch("total_number", &b_tot, "b_tot/L");
  t_b->Branch("fee", &b_fee, "b_fee/I");
  t_b->Branch("chanel", &b_ch, "b_ch/I");
  t_b->Branch("X_coordinate", &b_x, "b_x/F");
  t_b->Branch("Y_coordinate", &b_y, "b_y/F");
  t_b->Branch("anode_charge", &b_anode, "b_anode/I");
  t_b->Branch("dynode_charge", &b_dynode, "b_dynode/I");
  t_b->Branch("second", &b_sec, "b_sec/L");
  t_b->Branch("nano_second", &b_ns, "b_ns/D");
  t_b->Branch("given_time", &b_gtime, "b_gtime/l");

  ULong64_t tmp_gtime = 1;

  // CXC 19/3/16

  if (fChain == 0)
    return;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  b_tot = 0; // CXC 19/3/16
  for (Long64_t jentry = bigevt0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
      break;
    nb = fChain->GetEntry(jentry);
    nbytes += nb;

    int big_nhit = nhit;
    big_evt = jentry;
    b_entry = jentry; // CXC 19/3/16
    for (int ibig = 0; ibig < big_nhit; ibig++) {
      big_fee[ibig] = hits_fee[ibig];
      big_ch[ibig] = hits_ch[ibig];
      big_pmtx[ibig] = bigpmtx[big_fee[ibig]][big_ch[ibig]];
      big_pmty[ibig] = bigpmty[big_fee[ibig]][big_ch[ibig]];
      big_pmtz[ibig] = 0;
      big_anode[ibig] = hits_anode_charge[ibig];
      big_dynode[ibig] = hits_dynode_charge[ibig];
      big_second[ibig] = hits_second[ibig];
      big_ns[ibig] =
          hits_low_th_fine_time[ibig] * 0.333 + hits_coarse_time[ibig] * 16.;

      // CXC 19/3/16
      b_evt = evt;
      b_hit = ibig;
      b_fee = hits_fee[ibig];
      b_ch = hits_ch[ibig];
      b_x = bigpmtx[big_fee[ibig]][big_ch[ibig]];
      b_y = bigpmty[big_fee[ibig]][big_ch[ibig]];
      b_anode = hits_anode_charge[ibig];
      b_dynode = hits_dynode_charge[ibig];
      b_sec = hits_second[ibig];
      b_ns = hits_low_th_fine_time[ibig] * 0.333 + hits_coarse_time[ibig] * 16.;
      b_gtime = hits_time[ibig];

      // CXC 19/3/16

      if (big_dynode[ibig] > 0) {
        /*        cout << big_evt << " " << ibig << " " << big_pmtx[ibig] << " "
                    << big_pmty[ibig] << " " << big_anode[ibig] << " "
                    << big_dynode[ibig] << " " << big_fee[ibig] << " " <<
           big_ch[ibig]
                    << " " << big_second[ibig] << " " << std::setprecision(11)
                    << big_ns[ibig] << endl; */
        ievt = ievt + 1;
        // CXC 19/3/16
        if ((b_gtime < tmp_gtime)) {
          cout << b_tot << " " << b_gtime << " " << tmp_gtime << " "
               << tmp_gtime - b_gtime << " " << big_evt << " " << ibig << " "
               << big_pmtx[ibig] << " " << big_pmty[ibig] << " "
               << big_anode[ibig] << " " << big_dynode[ibig] << " "
               << big_fee[ibig] << " " << big_ch[ibig] << " "
               << big_second[ibig] << " " << std::setprecision(11)
               << big_ns[ibig] << endl;
        }
        tmp_gtime = b_gtime;
        t_b->Fill();
        b_tot++;
        // CXC 19/3/16
      }
    }
  }
  f_bigpmtsimple->Write(); // CXC 19/3/16
}

void wcdaevent::Loop0() {}
void wcdapls::Loop() {}

int main() {
  wcdaevent t;
  t.Loop();
  return 0;
}
