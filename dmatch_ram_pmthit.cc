// A sister version of match_ram_pmt.cc. The only difference: this code use the
// wcdahit data instead of wcdaevent data.

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

int smigcell;
std::vector<std::vector<int>> smfee(900);
std::vector<std::vector<int>> smdb(900);
std::vector<std::vector<int>> smpmt(900);
std::vector<std::vector<int>> smanode(900);
std::vector<std::vector<int>> smdynode(900);
std::vector<std::vector<long>> smtime(900);
std::vector<std::vector<long>> smtimedf(900); // small pmt

void wcdapls::Loop() {}
void wcdahits::Loop() {}

int main(int argc, char *argv[]) {
#include "bigpmtpos.h"
#include "smpmtpos.h"

  if (argc < 4) {
    std::cout << argv[0] << " bfile sfile ofile.root" << endl;
    exit(0);
  }

  clock_t start, finish;
  start = clock();

  string bpath = "/home/changxc/mywork/code/repo/scda/data/bigdata";
  string spath = "/home/changxc/mywork/code/repo/scda/data/smdata";
  string bfile = argv[1], sfile = argv[2], rootfile = argv[3];

  wcdahits big(bpath, bfile);
  wcdapls sm(spath, sfile);

  Int_t b_igcell, b_fee_b, b_ch, b_anode_b, b_dynode_b;
  Float_t b_x, b_y;
  Long64_t b_entry, b_tot, b_time_b;
  double_t b_npe_b;
  // big pmt
  int th_dynode = 0; // threshold of big selection.

  Int_t b_fee_s, b_db, b_pmt, b_anode_s, b_dynode_s;
  Long64_t b_time_s, b_time_diff;
  double_t b_npe_s;
  // small pmt

  Long64_t stamp[900] = {0}; // save the latest event position in each igcell.
  Long64_t stamp_retreat[900] = {
      0};                  // measure the number which stamp[] should retreat to
                           // catch a very close hit in the same big pmt.
  Long64_t timewin = 4000; // half width of the matching time window. (ns)
  Long64_t timerej = 0;

  Long64_t timelow = 500;
  Long64_t timeup = 2500; // lower and upper bound for time window.

  TFile *f_matchevents = new TFile(rootfile.c_str(), "recreate");
  TTree *t_match =
      new TTree("tmatch", "big vs small match events"); // store the result

  t_match->Branch("entry_number", &b_entry, "b_entry/L");
  t_match->Branch("total_number", &b_tot, "b_tot/L");
  t_match->Branch("igcell", &b_igcell, "b_igcell/I");
  t_match->Branch("fee_b", &b_fee_b, "b_fee_b/I");
  t_match->Branch("channel", &b_ch, "b_ch/I");
  t_match->Branch("X", &b_x, "b_x/F");
  t_match->Branch("Y", &b_y, "b_y/F");
  t_match->Branch("anode_b", &b_anode_b, "b_anode_big/I");
  t_match->Branch("dynode_b", &b_dynode_b, "b_dynode_big/I");
  t_match->Branch("npe_b", &b_npe_b, "b_npe_b/D");
  t_match->Branch("time_b", &b_time_b, "b_time_big/L");
  t_match->Branch("fee_s", &b_fee_s, "b_fee_s/I");
  t_match->Branch("db", &b_db, "b_db/I");
  t_match->Branch("pmt", &b_pmt, "b_pmt/I");
  t_match->Branch("anode_s", &b_anode_s, "b_anode_peak/I");
  t_match->Branch("dynode_s", &b_dynode_s, "b_dynode_peak/I");
  t_match->Branch("npe_s", &b_npe_s, "b_npe_s/D");
  t_match->Branch("time_s", &b_time_s, "b_time_small/L");
  t_match->Branch("time_df", &b_time_diff, "b_time_s - b_time_b /L");

  //  put small data into RAM.
  if (sm.fChain == 0)
    exit(0);
  Long64_t snentries = sm.fChain->GetEntriesFast();
  for (Long64_t sjentry = 0; sjentry < snentries; sjentry++) {
    Long64_t sientry = sm.LoadTree(sjentry);
    if (sientry < 0)
      break;
    sm.fChain->GetEntry(sjentry);
    smigcell = smpmtig_jd[sm.fee][sm.db][sm.pmt];
    if (sm.anode_peak - sm.anode_ped > 0) {
      if (smigcell < 0 || smigcell > 899)
        continue;
      smfee[smigcell].push_back(sm.fee);
      smdb[smigcell].push_back(sm.db);
      smpmt[smigcell].push_back(sm.pmt);
      smanode[smigcell].push_back(sm.anode_peak - sm.anode_ped);
      smdynode[smigcell].push_back(sm.dynode_peak - sm.dynode_ped);
      smtime[smigcell].push_back((sm.second + 1) * 1000000000LL +
                                 sm.ns * 20LL); // TODO
    }
  }

  b_tot = 0;

  if (big.fChain == 0)
    exit(0);
  Long64_t bnentries = big.fChain->GetEntriesFast();
  for (Long64_t bjentry = 0; bjentry < bnentries; bjentry++) {
    Long64_t bientry = big.LoadTree(bjentry);
    if (bientry < 0)
      break;
    big.fChain->GetEntry(bjentry);

    if (!(big.dynode_charge > th_dynode))
      continue;
    b_tot++;
    b_entry = bjentry;
    b_fee_b = big.fee;
    b_ch = big.ch;
    b_igcell = bigpmtig[b_fee_b][b_ch];
    b_x = bigpmtx[b_fee_b][b_ch];
    b_y = bigpmty[b_fee_b][b_ch];
    b_anode_b = big.anode_charge;
    b_dynode_b = big.dynode_charge;
    b_time_b = big.low_th_fine_time * 0.333 + big.coarse_time * 16 +
               big.second * 1000000000LL;
    Double_t q0 = b_anode_b / 22.0;
    Double_t q1 = b_dynode_b / 0.50094; // 0.50094 = 0.726 * 0.69
    b_npe_b = q0;
    if (q0 <= 0)
      b_npe_b = 1.e-20;
    if (q0 > 160 && q1 > 0)
      b_npe_b = q1; // npe transforming.

    for (Long64_t i = stamp[b_igcell]; i < smfee[b_igcell].size(); i++) {
      b_time_s = smtime[b_igcell][i];
      b_time_diff = b_time_s - b_time_b;
      if (b_time_diff < timelow) {
        continue;
      }
      if (b_time_diff < timeup) {
        b_fee_s = smfee[b_igcell][i];
        b_db = smdb[b_igcell][i];
        b_pmt = smpmt[b_igcell][i];
        b_anode_s = smanode[b_igcell][i];
        b_dynode_s = smdynode[b_igcell][i];
        if (b_anode_s < 4000)
          b_npe_s = b_anode_s * 2.;
        else
          b_npe_s = b_dynode_s * 120. * 2.;
        stamp[b_igcell] = i;
        t_match->Fill();
      } else
        break;
    }
    if ((b_tot % 10000) == 0)
      std::cout << b_tot << "\r" << flush;
  }
  f_matchevents->Write();

  finish = clock();
  std::cout << "big hits " << b_tot << " total time "
            << double((finish - start) / CLOCKS_PER_SEC) << " s" << endl;

  return 0;
}
