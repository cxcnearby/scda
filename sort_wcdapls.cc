#define wcdahits_cxx
#define wcdapls_cxx
#include "wcdapls.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

double smpmtx_jd[26][4][17];
double smpmty_jd[26][4][17];
int smpmtig_jd[26][4][17];

struct s_wcdapls {
  UInt_t fUniqueID;
  UInt_t fBits;
  UInt_t fee;
  UInt_t flag;
  UInt_t bdb;
  UInt_t db;
  UInt_t pmt;
  ULong_t time;
  UInt_t second;
  UInt_t ns;
  UInt_t anode_peak;
  UInt_t anode_ped;
  UInt_t anode_time;
  UInt_t dynode_peak;
  UInt_t dynode_ped;
  UInt_t dynode_time;
  UInt_t tail;
  UInt_t l1id;
  Int_t dt;
  Long64_t
      entry; // add by myselt to give a number for each hit to check disorder.
  Long64_t deltatime; // add by myself to check disorder.
};

vector<s_wcdapls> data;
s_wcdapls hits;

Long64_t temptime[900] = {0};

bool comp(s_wcdapls a, s_wcdapls b) {
  if (a.second == b.second)
    return a.ns < b.ns;
  else
    return a.second < b.second;
}

void wcdapls::Loop() {
  if (fChain == 0)
    return;

  Long64_t nentries = fChain->GetEntriesFast();

  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
      break;
    fChain->GetEntry(jentry);
    hits.fUniqueID = fUniqueID;
    hits.fBits = fBits;
    hits.fee = fee;
    hits.flag = flag;
    hits.bdb = bdb;
    hits.db = db;
    hits.pmt = pmt;
    hits.time = time;
    hits.second = second;
    hits.ns = ns;
    hits.anode_peak = anode_peak;
    hits.anode_ped = anode_ped;
    hits.anode_time = anode_time;
    hits.dynode_peak = dynode_peak;
    hits.dynode_ped = dynode_ped;
    hits.dynode_time = dynode_time;
    hits.tail = tail;
    hits.l1id = l1id;
    hits.dt = dt;
    hits.entry = jentry;
    Long64_t tmp = second * 1000000000LL + ns * 20LL;
    hits.deltatime = tmp - temptime[smpmtig_jd[fee][db][pmt]];
    temptime[smpmtig_jd[fee][db][pmt]] = tmp;
    data.push_back(hits);
    //   if we want to print the disordered hits, it should not be commented.
    if (hits.deltatime < 0) {
      for (int i = data.size() - 2; i != 0; i--) {
        s_wcdapls tmphits = data[i];
        if (tmphits.fee == hits.fee && tmphits.db == hits.db &&
            tmphits.pmt == hits.pmt) {
          cout << hits.entry << " " << hits.fee << " " << hits.db << " "
               << hits.pmt << " " << hits.second << " " << hits.ns << " "
               << hits.deltatime << " " << tmphits.entry << " "
               << tmphits.second << " " << tmphits.ns << endl;
          break;
        }
      }
    }
    //   if we want to print the disordered hits, it should not be commented.
  }
}

int main(int argc, char *argv[]) {
#include "smpmtpos.h"

  if (argc < 5) {
    std::cout << argv[0] << "  spath  sfile  sorted.root  Ifcheck(0no/1yes)"
              << endl;
    exit(0);
  }
  clock_t start, finish;
  start = clock();

  string spath = argv[1], sfile = argv[2], sortedfile = argv[3];
  int Ifcheck = atoi(argv[4]);

  wcdapls sm(spath, sfile);

  sm.Loop();

  TFile *f_sorted = new TFile(sortedfile.c_str(), "recreate");
  TTree *t = new TTree("wcdapls", "sorted events");
  TTree *t_c = new TTree("check", "tree_for_time_check");

  t_c->Branch("fUniqueID", &hits.fUniqueID, "b_fUniqueID/i");
  t_c->Branch("fBits", &hits.fBits, "b_fBits/i");
  t_c->Branch("fee", &hits.fee, "b_fee/i");
  t_c->Branch("flag", &hits.flag, "b_flag/i");
  t_c->Branch("bdb", &hits.bdb, "b_bdb/i");
  t_c->Branch("db", &hits.db, "b_db/i");
  t_c->Branch("pmt", &hits.pmt, "b_pmt/i");
  t_c->Branch("time", &hits.time, "b_time/l");
  t_c->Branch("second", &hits.second, "b_second/i");
  t_c->Branch("ns", &hits.ns, "b_ns/i");
  t_c->Branch("anode_peak", &hits.anode_peak, "b_anode_peak/i");
  t_c->Branch("anode_ped", &hits.anode_ped, "b_anode_ped/i");
  t_c->Branch("anode_time", &hits.anode_time, "b_anode_time/i");
  t_c->Branch("dynode_peak", &hits.dynode_peak, "b_dynode_peak/i");
  t_c->Branch("dynode_ped", &hits.dynode_ped, "b_dynode_ped/i");
  t_c->Branch("dynode_time", &hits.dynode_time, "b_dynode_time/i");
  t_c->Branch("tail", &hits.tail, "b_tail/i");
  t_c->Branch("l1id", &hits.l1id, "b_l1id/i");
  t_c->Branch("dt", &hits.dt, "b_dt/I");
  t_c->Branch("entry", &hits.entry, "b_entry/L");
  t_c->Branch("deltatime", &hits.deltatime, "b_deltatime/L");

  t->Branch("fUniqueID", &hits.fUniqueID, "b_fUniqueID/i");
  t->Branch("fBits", &hits.fBits, "b_fBits/i");
  t->Branch("fee", &hits.fee, "b_fee/i");
  t->Branch("flag", &hits.flag, "b_flag/i");
  t->Branch("bdb", &hits.bdb, "b_bdb/i");
  t->Branch("db", &hits.db, "b_db/i");
  t->Branch("pmt", &hits.pmt, "b_pmt/i");
  t->Branch("time", &hits.time, "b_time/l");
  t->Branch("second", &hits.second, "b_second/i");
  t->Branch("ns", &hits.ns, "b_ns/i");
  t->Branch("anode_peak", &hits.anode_peak, "b_anode_peak/i");
  t->Branch("anode_ped", &hits.anode_ped, "b_anode_ped/i");
  t->Branch("anode_time", &hits.anode_time, "b_anode_time/i");
  t->Branch("dynode_peak", &hits.dynode_peak, "b_dynode_peak/i");
  t->Branch("dynode_ped", &hits.dynode_ped, "b_dynode_ped/i");
  t->Branch("dynode_time", &hits.dynode_time, "b_dynode_time/i");
  t->Branch("tail", &hits.tail, "b_tail/i");
  t->Branch("l1id", &hits.l1id, "b_l1id/i");
  t->Branch("dt", &hits.dt, "b_dt/I");

  if (Ifcheck) {
    for (auto &v : data) {
      hits = v;
      t_c->Fill();
    }
  }

  sort(data.begin(), data.end(), comp);

  for (auto &v : data) {
    hits = v;
    t->Fill();
  }

  f_sorted->Write();
  f_sorted->Close();

  finish = clock();
  cout << "sorting time " << double((finish - start) / CLOCKS_PER_SEC) << " s"
       << endl;

  return 0;
}