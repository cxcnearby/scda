#define wcdahits_cxx
#define wcdapls_cxx
#include "wcdahits.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

struct s_wcdahits {
  UInt_t fUniqueID;
  UInt_t fBits;
  UInt_t fee;
  UInt_t ch;
  UInt_t anode_charge;
  UInt_t dynode_charge;
  UInt_t charge_over_range_id;
  UInt_t second;
  UInt_t year;
  UInt_t coarse_time;
  UInt_t high_th_fine_time;
  UInt_t low_th_fine_time;
  UInt_t temprature_sensor_info;
  ULong_t time;
};

vector<s_wcdahits> data;
s_wcdahits hits;

bool comp(s_wcdahits a, s_wcdahits b) {
  if (a.second == b.second) {
    Long64_t a_ns = a.low_th_fine_time * 0.333 + a.coarse_time * 16LL;
    Long64_t b_ns = b.low_th_fine_time * 0.333 + b.coarse_time * 16LL;
    return a_ns < b_ns;
  } else
    return a.second < b.second;
}

void wcdahits::Loop() {
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
    hits.ch = ch;
    hits.anode_charge = anode_charge;
    hits.dynode_charge = dynode_charge;
    hits.charge_over_range_id = charge_over_range_id;
    hits.second = second;
    hits.year = year;
    hits.coarse_time = coarse_time;
    hits.high_th_fine_time = high_th_fine_time;
    hits.low_th_fine_time = low_th_fine_time;
    hits.temprature_sensor_info = temprature_sensor_info;
    hits.time = time;
    data.push_back(hits);
  }
}

int main(int argc, char *argv[]) {

  if (argc < 4) {
    std::cout << argv[0] << " bpath bfile sorted.root" << endl;
    exit(0);
  }
  clock_t start, finish;
  start = clock();

  string bpath = argv[1], bfile = argv[2], sortedfile = argv[3];

  wcdahits big(bpath, bfile);

  big.Loop();

  sort(data.begin(), data.end(), comp);

  TFile *f_sorted = new TFile(sortedfile.c_str(), "recreate");
  TTree *t = new TTree("wcda", "sorted events");

  t->Branch("fUniqueID", &hits.fUniqueID, "b_fUniqueID/l");
  t->Branch("fBits", &hits.fBits, "b_fBits/l");
  t->Branch("fee", &hits.fee, "b_fee/l");
  t->Branch("ch", &hits.ch, "b_ch/l");
  t->Branch("anode_charge", &hits.anode_charge, "b_anode_charge/l");
  t->Branch("dynode_charge", &hits.dynode_charge, "b_dynode_charge/l");
  t->Branch("charge_over_range_id", &hits.charge_over_range_id,
            "b_charge_over_range_id/l");
  t->Branch("second", &hits.second, "b_second/l");
  t->Branch("year", &hits.year, "b_year/l");
  t->Branch("coarse_time", &hits.coarse_time, "b_coarse_time/l");
  t->Branch("high_th_fine_time", &hits.high_th_fine_time,
            "b_high_th_fine_time/l");
  t->Branch("low_th_fine_time", &hits.low_th_fine_time, "b_low_th_fine_time/l");
  t->Branch("temprature_sensor_info", &hits.temprature_sensor_info,
            "b_temprature_sensor_info/l");
  t->Branch("time", &hits.time, "b_time/l");
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