#define wcdaevent_cxx
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
#include "wcdaevent.h"
#include "wcdapls.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

double smpmtx_jd[26][4][17];
double smpmty_jd[26][4][17];
int smpmtig_jd[26][4][17];
double bigpmtx[101][10];
double bigpmty[101][10];
int bigpmtig[101][10];

void wcdaevent::Loop0() {}
void wcdaevent::Loop() {
#include "bigpmtpos.h"

  UInt_t b_igcell, b_fee_b, b_ch, b_anode_b, b_dynode_b;
  Float_t b_x, b_y;
  Long64_t b_entry, b_tot, b_time_b;
  ULong64_t b_evt; // big pmt

  ofstream outselect;
  outselect.open("big_event_selected.dat");
  if (!outselect.is_open()) {
    cout << "cannot write the data file!";
    exit(0);
  }

  b_tot = 0;

  if (fChain == 0)
    return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
      break;
    nb = fChain->GetEntry(jentry);
    nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    b_entry = jentry;
    b_evt = evt;
    for (int ihit = 0; ihit < nhit; ihit++) {
      b_fee_b = hits_fee[ihit];
      b_ch = hits_ch[ihit];
      b_igcell = bigpmtig[b_fee_b][b_ch];
      b_x = bigpmtx[b_fee_b][b_ch];
      b_y = bigpmty[b_fee_b][b_ch];
      b_anode_b = hits_anode_charge[ihit];
      b_dynode_b = hits_dynode_charge[ihit];
      b_time_b = hits_low_th_fine_time[ihit] * 0.333 +
                 hits_coarse_time[ihit] * 16. +
                 hits_second[ihit] * 1000000000LL;
      if (b_dynode_b > 500) {
        b_tot++;
        outselect << b_tot << " " << b_entry << " " << b_evt << " " << b_igcell
                  << " " << b_x << " " << b_y << " " << b_fee_b << " " << b_ch
                  << " " << b_anode_b << " " << b_dynode_b << " " << b_time_b
                  << endl;
      }
    }
  }
cout<<b_tot<<endl;//TODO
  outselect.close();
}

void wcdapls::Loop() {
#include "smpmtpos.h"

  UInt_t b_igcell, b_fee_b, b_ch, b_anode_b, b_dynode_b;
  Float_t b_x, b_y;
  Long64_t b_entry, b_tot, b_time_b;
  ULong64_t b_evt; // big pmt

  UInt_t b_fee_s, b_db, b_pmt, b_anode_s, b_dynode_s;
  Long64_t b_time_s, b_time_diff; // small pmt

  ULong64_t stamp[901] = {0}; // save the latest event position in each igcell.
  Long64_t timewin = 4000;    // half width of the matching time window. (ns)

  ifstream inselect;
  inselect.open("big_event_selected.dat");
  if (!inselect.is_open()) {
    cout << "cannot open the data file!";
    exit(0);
  }

  TFile *f_matchevents = new TFile("matchevents.root", "recreate");
  TTree *t_match =
      new TTree("tmatch", "big vs small match events"); // store the result

  t_match->Branch("entry_number", &b_entry, "b_entry/L");
  t_match->Branch("total_number", &b_tot, "b_tot/L");
  t_match->Branch("event_time", &b_evt, "b_evt/l");
  t_match->Branch("igcell", &b_igcell, "b_igcell/i");
  t_match->Branch("fee_b", &b_fee_b, "b_fee_b/i");
  t_match->Branch("channel", &b_ch, "b_ch/i");
  t_match->Branch("X", &b_x, "b_x/F");
  t_match->Branch("Y", &b_y, "b_y/F");
  t_match->Branch("anode_b", &b_anode_b, "b_anode_big/i");
  t_match->Branch("dynode_b", &b_dynode_b, "b_dynode_big/i");
  t_match->Branch("time_b", &b_time_b, "b_time_big/L");
  t_match->Branch("fee_s", &b_fee_s, "b_fee_s/i");
  t_match->Branch("db", &b_db, "b_db/i");
  t_match->Branch("pmt", &b_pmt, "b_pmt/i");
  t_match->Branch("anode_s", &b_anode_s, "b_anode_peak/i");
  t_match->Branch("dynode_s", &b_dynode_s, "b_dynode_peak/i");
  t_match->Branch("time_s", &b_time_s, "b_time_small/L");
  t_match->Branch("time_df", &b_time_diff, "b_time_s - b_time_b /L");

  b_tot = 0;

  if (fChain == 0)
    return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  while (inselect >> b_tot >> b_entry >> b_evt >> b_igcell >> b_x >> b_y >>
         b_fee_b >> b_ch >> b_anode_b >> b_dynode_b >> b_time_b) {
cout<<b_tot<<endl;//TODO
    for (Long64_t jentry = stamp[b_igcell]; jentry < nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
        break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      b_time_s = (second + 1) * 1000000000LL + ns * 20LL;
      b_time_diff = b_time_s - b_time_b;
      if (b_time_diff < -timewin)
        continue;
      if (b_time_diff > timewin) {
        stamp[b_igcell] = jentry;
        break;
      }
      if (b_igcell == smpmtig_jd[fee][db][pmt]) {
        b_anode_s = anode_peak - anode_ped;
        b_dynode_s = dynode_peak - dynode_ped;
        t_match->Fill();
      }
    }
//       break;
  }
  inselect.close();
  f_matchevents->Write();
}

int main(int argc, char *argv[]) {

  if (argc < 5) {
    cout << argv[0] << " bpath bfile spath sfile" << endl;
    exit(0);
  }

  time_t start, finish;
  start = time(NULL);

  string bpath = argv[1], bfile = argv[2], spath = argv[3], sfile = argv[4];

  wcdaevent big(bpath, bfile);
  wcdapls sm(spath, sfile);

  big.Loop();
  sm.Loop();

  finish = time(NULL);
  cout << "use time " << difftime(finish, start) << endl;

  return 0;
}