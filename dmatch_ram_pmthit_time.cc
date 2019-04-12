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

Long64_t temptime_b[900] = {0};
Long64_t temptime_s[900] = {0};
Long64_t temptime_anode[900] = {0};
Long64_t temptime_dynode[900] = {
    0}; // to calculate T_i - T_i-1, we need to store T_i-1 for each pmt and for
        // each kind of time.

int smigcell;
std::vector<std::vector<long>> smentry(900);
std::vector<std::vector<int>> smfee(900);
std::vector<std::vector<int>> smdb(900);
std::vector<std::vector<int>> smpmt(900);
std::vector<std::vector<int>> smanode(900);
std::vector<std::vector<int>> smdynode(900);
std::vector<std::vector<long>> smtime(900);
std::vector<std::vector<long>> smtime_anode(900);
std::vector<std::vector<long>> smtime_dynode(900);
std::vector<std::vector<long>> smtimedf(900); // small pmt

int IfCheckSmDisorder = 1; // a switch to determine if we should check the
                           // disorder of small pmt events.
int IfCheckBigDisorder = 0;

void wcdapls::Loop() {}
void wcdahits::Loop() {}

int main(int argc, char *argv[]) {
#include "bigpmtpos.h"
#include "smpmtpos.h"

  if (argc < 6) {
    std::cout << argv[0] << " bpath bfile spath sfile ofile.root" << endl;
    exit(0);
  }

  clock_t start, finish;
  start = clock();

  string bpath = argv[1], bfile = argv[2], spath = argv[3], sfile = argv[4],
         rootfile = argv[5];

  wcdahits big(bpath, bfile);
  wcdapls sm(spath, sfile);

  Int_t b_igcell, b_fee_b, b_ch, b_anode_b, b_dynode_b;
  Float_t b_x, b_y;
  Long64_t b_entry_b, b_tot, b_time_b, b_deltatime_b;
  double_t b_npe_b;
  // big pmt
  int th_dynode = 0; // threshold of big selection.

  Int_t b_fee_s, b_db, b_pmt, b_anode_s, b_dynode_s;
  Long64_t b_entry_s, b_time_s, b_deltatime_s, b_time_diff, b_time_anode,
      b_time_dynode, b_deltatime_anode, b_deltatime_dynode, b_time_diff_anode,
      b_time_diff_dynode;
  double_t b_npe_s;
  // small pmt

  Long64_t lastbutone; // the array number of the last but one.

  Long64_t stamp[900] = {0}; // save the latest event position in each igcell.
  Long64_t stamp_retreat[900] = {
      0};                  // measure the number which stamp[] should retreat to
                           // catch a very close hit in the same big pmt.
  Long64_t timewin = 4000; // half width of the matching time window. (ns)
  Long64_t timerej = 0;

  Long64_t timelow = 100;
  Long64_t timeup = 2500; // lower and upper bound for time window.

  TFile *f_matchevents = new TFile(rootfile.c_str(), "recreate");
  TTree *t_match =
      new TTree("tmatch", "big vs small match events"); // store the result

  t_match->Branch("entry_number_b", &b_entry_b, "b_entry_big/L");
  t_match->Branch("total_number", &b_tot, "b_tot/L");
  t_match->Branch("igcell", &b_igcell, "b_igcell/I");
  t_match->Branch("fee_b", &b_fee_b, "b_fee_big/I");
  t_match->Branch("channel", &b_ch, "b_ch/I");
  t_match->Branch("X", &b_x, "b_x/F");
  t_match->Branch("Y", &b_y, "b_y/F");
  t_match->Branch("anode_b", &b_anode_b, "b_anode_big/I");
  t_match->Branch("dynode_b", &b_dynode_b, "b_dynode_big/I");
  t_match->Branch("npe_b", &b_npe_b, "b_npe_big/D");
  t_match->Branch("time_b", &b_time_b, "b_time_big/L");
  t_match->Branch("deltatime_b", &b_deltatime_b, "b_deltatime_big/L");
  t_match->Branch("entry_number_s", &b_entry_s, "b_entry_small/L");
  t_match->Branch("fee_s", &b_fee_s, "b_fee_small/I");
  t_match->Branch("db", &b_db, "b_db/I");
  t_match->Branch("pmt", &b_pmt, "b_pmt/I");
  t_match->Branch("anode_s", &b_anode_s, "b_anode_small/I");
  t_match->Branch("dynode_s", &b_dynode_s, "b_dynode_small/I");
  t_match->Branch("npe_s", &b_npe_s, "b_npe_small/D");
  t_match->Branch("time_s", &b_time_s, "b_time_small/L");
  t_match->Branch("anodetime_s", &b_time_anode, "b_time_anode/L");
  t_match->Branch("dynodetime_s", &b_time_dynode, "b_time_dynode/L");
  t_match->Branch("deltatime_s", &b_deltatime_s, "b_deltatime_small/L");
  t_match->Branch("deltatime_anode", &b_deltatime_anode, "b_deltatime_anode/L");
  t_match->Branch("deltatime_dynode", &b_deltatime_dynode,
                  "b_deltatime_dynode/L");
  t_match->Branch("time_df", &b_time_diff, "b_time_s - b_time_b /L");
  t_match->Branch("time_df_anode", &b_time_diff_anode,
                  "b_time_anode - b_time_b /L");
  t_match->Branch("time_df_dynode", &b_time_diff_dynode,
                  "b_time_dynode - b_time_b /L");

  Int_t b_igcell_d, b_fee_d, b_db_d, b_pmt_d, b_anode_d, b_dynode_d;
  Long64_t b_entry_d, b_tot_d = 0, b_time_d, b_deltatime_d;
  TTree *t_smdisorder = new TTree(
      "tsm",
      "small event disorder: T_i - T_i-1"); // this tree is to store small pmt
                                            // time disorder data.
  t_smdisorder->Branch("entry", &b_entry_d, "b_entry /L");
  t_smdisorder->Branch("total", &b_tot_d, "b_total /L");
  t_smdisorder->Branch("igcell", &b_igcell_d, "b_igcell/I");
  t_smdisorder->Branch("fee", &b_fee_d, "b_fee/I");
  t_smdisorder->Branch("db", &b_db_d, "b_db/I");
  t_smdisorder->Branch("pmt", &b_pmt_d, "b_pmt/I");
  t_smdisorder->Branch("anode", &b_anode_d, "b_anode/I");
  t_smdisorder->Branch("dynode", &b_dynode_d, "b_dynode/I");
  t_smdisorder->Branch("time", &b_time_d, "b_time/L");
  t_smdisorder->Branch("deltatime", &b_deltatime_d, "T_i - T_i-1 /L");

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
      smentry[smigcell].push_back(sjentry);
      smfee[smigcell].push_back(sm.fee);
      smdb[smigcell].push_back(sm.db);
      smpmt[smigcell].push_back(sm.pmt);
      smanode[smigcell].push_back(sm.anode_peak - sm.anode_ped);
      smdynode[smigcell].push_back(sm.dynode_peak - sm.dynode_ped);
      smtime[smigcell].push_back((sm.second + 1LL) * 1000000000LL +
                                 sm.ns * 20LL - 64LL); // TODO
      smtime_anode[smigcell].push_back((sm.second + 1LL) * 1000000000LL +
                                       sm.ns * 20LL - 64LL +
                                       sm.anode_time * 20LL);
      smtime_dynode[smigcell].push_back((sm.second + 1LL) * 1000000000LL +
                                        sm.ns * 20LL - 64LL +
                                        sm.dynode_time * 20LL);
      if (IfCheckSmDisorder) { //  if we will store small pmt time disorder
                               //  data.
        b_tot_d++;
        b_entry_d = smentry[smigcell].back();
        b_igcell_d = smpmtig_jd[smfee[smigcell].back()][smdb[smigcell].back()]
                               [smpmt[smigcell].back()];
        b_fee_d = smfee[smigcell].back();
        b_db_d = smdb[smigcell].back();
        b_pmt_d = smpmt[smigcell].back();
        b_anode_d = smanode[smigcell].back();
        b_dynode_d = smdynode[smigcell].back();
        b_time_d = smtime[smigcell].back();
        lastbutone = smtime[smigcell].size() - 2;
        if (lastbutone < 0)
          lastbutone = 0;
        b_deltatime_d = smtime[smigcell].back() - smtime[smigcell][lastbutone];
        t_smdisorder->Fill();
      }
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
    b_entry_b = bjentry;
    b_fee_b = big.fee;
    b_ch = big.ch;
    b_igcell = bigpmtig[b_fee_b][b_ch];
    b_x = bigpmtx[b_fee_b][b_ch];
    b_y = bigpmty[b_fee_b][b_ch];
    b_anode_b = big.anode_charge;
    b_dynode_b = big.dynode_charge;
    Long64_t bigns = big.low_th_fine_time * 0.333 + big.coarse_time * 16LL;
    b_time_b = bigns + big.second * 1000000000LL;
    Double_t q0 = b_anode_b / 22.0;
    Double_t q1 = b_dynode_b / 0.50094; // 0.50094 = 0.726 * 0.69
    b_npe_b = q0;
    if (q0 <= 0)
      b_npe_b = 1.e-20;
    if (q0 > 160 && q1 > 0)
      b_npe_b = q1; // npe transforming.

    int IfSearch = 1; // a sign to determine if we search for the first matching
                      // event. do only 1 time for a big event.
    int IfSearchbig = 1;
    for (Long64_t i = stamp[b_igcell]; i < smfee[b_igcell].size(); i++) {
      if (IfSearchbig && IfCheckBigDisorder && i >= 50) {
        i = i - 50;
        IfSearchbig = 0;
      }
      b_time_s = smtime[b_igcell][i];
      b_time_diff = b_time_s - b_time_b;
      Long64_t i0 = i;
      Long64_t search_length = sqrt(smfee[b_igcell].size() - stamp[b_igcell]);
      while (IfSearch && i < smfee[b_igcell].size() && b_time_diff < timelow) {
        i0 = i;
        i += search_length;
        b_time_diff = smtime[b_igcell][i] - b_time_b;
      }
      if (IfSearch) {
        i = i0;
        b_time_s = smtime[b_igcell][i];
        b_time_diff = b_time_s - b_time_b;
        IfSearch = 0;
      }
      if (b_time_diff < timelow) {
        continue;
      }
      if (b_time_diff < timeup) {
        b_entry_s = smentry[b_igcell][i];
        b_fee_s = smfee[b_igcell][i];
        b_db = smdb[b_igcell][i];
        b_pmt = smpmt[b_igcell][i];
        b_anode_s = smanode[b_igcell][i];
        b_dynode_s = smdynode[b_igcell][i];
        b_time_anode = smtime_anode[b_igcell][i];
        b_time_dynode = smtime_dynode[b_igcell][i];
        b_time_diff_anode = b_time_anode - b_time_b;
        b_time_diff_dynode = b_time_dynode - b_time_b;
        b_deltatime_b = b_time_b - temptime_b[b_igcell];
        b_deltatime_s = b_time_s - temptime_s[b_igcell];
        b_deltatime_anode = b_time_anode - temptime_anode[b_igcell];
        b_deltatime_dynode = b_time_dynode - temptime_dynode[b_igcell];
        temptime_b[b_igcell] = b_time_b;
        temptime_s[b_igcell] = b_time_s;
        temptime_anode[b_igcell] = b_time_anode;
        temptime_dynode[b_igcell] = b_time_dynode;
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
      std::cout << " " << (b_entry_b * 100 / bnentries) << "%    " << b_tot
                << "\r" << flush;
  }
  f_matchevents->Write();
  f_matchevents->Close();
  finish = clock();
  std::cout << "big hits " << b_tot << " total time "
            << double((finish - start) / CLOCKS_PER_SEC) << " s" << endl;

  return 0;
}
