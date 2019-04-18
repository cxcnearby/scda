#define wcdahits_cxx
#define wcdapls_cxx
#include "wcdapls.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

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
};

vector<s_wcdapls> data;
s_wcdapls hits;

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
    data.push_back(hits);
  }
}

int main(int argc, char *argv[]) {

  if (argc < 4) {
    std::cout << argv[0] << " spath sfile sorted.root" << endl;
    exit(0);
  }
  clock_t start, finish;
  start = clock();

  string spath = argv[1], sfile = argv[2], sortedfile = argv[3];

  wcdapls sm(spath, sfile);

  sm.Loop();

  sort(data.begin(), data.end(), comp);

  TFile *f_sorted = new TFile(sortedfile.c_str(), "recreate");
  TTree *t = new TTree("wcdapls", "sorted events");

  t->Branch("fUniqueID", &hits.fUniqueID, "b_fUniqueID/l");
  t->Branch("fBits", &hits.fBits, "b_fBits/l");
  t->Branch("fee", &hits.fee, "b_fee/l");
  t->Branch("flag", &hits.flag, "b_flag/l");
  t->Branch("bdb", &hits.bdb, "b_bdb/l");
  t->Branch("db", &hits.db, "b_db/l");
  t->Branch("pmt", &hits.pmt, "b_pmt/l");
  t->Branch("time", &hits.time, "b_time/l");
  t->Branch("second", &hits.second, "b_second/l");
  t->Branch("ns", &hits.ns, "b_ns/l");
  t->Branch("anode_peak", &hits.anode_peak, "b_anode_peak/l");
  t->Branch("anode_ped", &hits.anode_ped, "b_anode_ped/l");
  t->Branch("anode_time", &hits.anode_time, "b_anode_time/l");
  t->Branch("dynode_peak", &hits.dynode_peak, "b_dynode_peak/l");
  t->Branch("dynode_ped", &hits.dynode_ped, "b_dynode_ped/l");
  t->Branch("dynode_time", &hits.dynode_time, "b_dynode_time/l");
  t->Branch("tail", &hits.tail, "b_tail/l");
  t->Branch("l1id", &hits.l1id, "b_l1id/l");
  t->Branch("dt", &hits.dt, "b_dt/l");
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