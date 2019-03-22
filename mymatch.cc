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
double smpmtig_jd[26][4][17];
double bigpmtx[101][10];
double bigpmty[101][10];
double bigpmtig[101][10];

void wcdaevent::Loop()
{
  UInt_t b_igcell, b_fee_b, b_ch, b_anode_b, b_dynode_b;
  Float_t b_x, b_y;
  Long64_t b_entry, b_tot;
  ULong64_t b_evt, b_time_b; // big pmt

  UInt_t b_fee_s, b_db, b_pmt, b_anode_peak, b_anode_ped, b_dynode_peak,
      b_dynode_ped;
  ULong64_t b_time_s, b_time_diff; //small pmt
  
   
  TFile *f_matchevents = new TFile("matchevents.root", "recreate");
  TTree *t_match = new TTree("tmatch", "big vs small match events");

  t_match->Branch("entry_number", &b_entry, "b_entry/L");
  t_match->Branch("total_number", &b_tot, "b_tot/L");
  t_match->Branch("event_time", &b_evt, "b_evt/l");
  t_match->Branch("igcell", &b_igcell, "b_igcell/i");
  t_match->Branch("fee_b", &b_fee_b, "b_fee_b/i");
  t_match->Branch("chanel", &b_ch, "b_ch/i");
  t_match->Branch("X", &b_x, "b_x/F");
  t_match->Branch("Y", &b_y, "b_y/F");
  t_match->Branch("anode_b", &b_anode_b, "b_anode_big/i");
  t_match->Branch("dynode_b", &b_dynode_b, "b_dynode_big/i");
  t_match->Branch("time_b", &b_time_b, "b_time_big/l");
  t_match->Branch("fee_s", &b_fee_s, "b_fee_s/i");
  t_match->Branch("db", &b_db, "b_db/i");
  t_match->Branch("pmt", &b_pmt, "b_pmt/i");
  t_match->Branch("anode_peak", &b_anode_peak, "b_anode_peak/i");
  t_match->Branch("anode_ped", &b_anode_ped, "b_anode_ped/i");
  t_match->Branch("dynode_peak", &b_dynode_peak, "b_dynode_peak/i");
  t_match->Branch("dynode_ped", &b_dynode_ped, "b_dynode_ped/i");
  t_match->Branch("time_s", &b_time_s, "b_time_small/i");


   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}