#define wcdahits_cxx
#define wcdapls_cxx
#include "wcdahits.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

double bigpmtx[101][10];
double bigpmty[101][10];
int bigpmtig[101][10];

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
  Long64_t
      entry; // add by myselt to give a number for each hit to check disorder.
  Long64_t deltatime; // add by myself to check disorder.
};

vector<s_wcdahits> data;
s_wcdahits hits;

Long64_t temptime[900] = {0};

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
    hits.entry = jentry;
    Long64_t tmp = second * 1000000000LL + int(low_th_fine_time * 0.333) +
                   coarse_time * 16LL;
    hits.deltatime = tmp - temptime[bigpmtig[fee][ch]];
    temptime[bigpmtig[fee][ch]] = tmp;
    data.push_back(hits);
  }
}

int main(int argc, char *argv[]) {
#include "bigpmtpos.h"

  if (argc < 5) {
    std::cout << argv[0] << "  bpath  bfile  sorted.root  Ifcheck(0no/1yes)"
              << endl;
    exit(0);
  }
  clock_t start, finish;
  start = clock();

  string bpath = argv[1], bfile = argv[2], sortedfile = argv[3];
  int Ifcheck = atoi(argv[4]);

  wcdahits big(bpath, bfile);

  big.Loop();

  TFile *f_sorted = new TFile(sortedfile.c_str(), "recreate");
  TTree *t = new TTree("wcda", "sorted events");
  TTree *t_c = new TTree("check", "tree_for_time_check");

  t_c->Branch("fUniqueID", &hits.fUniqueID, "b_fUniqueID/i");
  t_c->Branch("fBits", &hits.fBits, "b_fBits/i");
  t_c->Branch("fee", &hits.fee, "b_fee/i");
  t_c->Branch("ch", &hits.ch, "b_ch/i");
  t_c->Branch("anode_charge", &hits.anode_charge, "b_anode_charge/i");
  t_c->Branch("dynode_charge", &hits.dynode_charge, "b_dynode_charge/i");
  t_c->Branch("charge_over_range_id", &hits.charge_over_range_id,
              "b_charge_over_range_id/i");
  t_c->Branch("second", &hits.second, "b_second/i");
  t_c->Branch("year", &hits.year, "b_year/i");
  t_c->Branch("coarse_time", &hits.coarse_time, "b_coarse_time/i");
  t_c->Branch("high_th_fine_time", &hits.high_th_fine_time,
              "b_high_th_fine_time/i");
  t_c->Branch("low_th_fine_time", &hits.low_th_fine_time,
              "b_low_th_fine_time/i");
  t_c->Branch("temprature_sensor_info", &hits.temprature_sensor_info,
              "b_temprature_sensor_info/i");
  t_c->Branch("time", &hits.time, "b_time/l");
  t_c->Branch("entry", &hits.entry, "b_entry/L");
  t_c->Branch("deltatime", &hits.deltatime, "b_deltatime/L");

  t->Branch("fUniqueID", &hits.fUniqueID, "b_fUniqueID/i");
  t->Branch("fBits", &hits.fBits, "b_fBits/i");
  t->Branch("fee", &hits.fee, "b_fee/i");
  t->Branch("ch", &hits.ch, "b_ch/i");
  t->Branch("anode_charge", &hits.anode_charge, "b_anode_charge/i");
  t->Branch("dynode_charge", &hits.dynode_charge, "b_dynode_charge/i");
  t->Branch("charge_over_range_id", &hits.charge_over_range_id,
            "b_charge_over_range_id/i");
  t->Branch("second", &hits.second, "b_second/i");
  t->Branch("year", &hits.year, "b_year/i");
  t->Branch("coarse_time", &hits.coarse_time, "b_coarse_time/i");
  t->Branch("high_th_fine_time", &hits.high_th_fine_time,
            "b_high_th_fine_time/i");
  t->Branch("low_th_fine_time", &hits.low_th_fine_time, "b_low_th_fine_time/i");
  t->Branch("temprature_sensor_info", &hits.temprature_sensor_info,
            "b_temprature_sensor_info/i");
  t->Branch("time", &hits.time, "b_time/l");

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