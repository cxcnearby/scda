//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Feb 23 00:19:03 2019 by ROOT version 6.14/00
// from TTree wcda/WCDA Events
// found on file: ES.10001.WCDAEvent.20190215061937.015.root
//////////////////////////////////////////////////////////

#ifndef wcdaevent_h
#define wcdaevent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TLine.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TPad.h"
#include "TStyle.h"
#include "TLatex.h"
#include <iostream>
#include <fstream>

using namespace std;

// Header file for the classes stored in the TTree if any.
#include "TObject.h"

class wcdaevent {
 public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // modified by Wei Liu, 2019.03.18
  string path;
  string file;

  // Fixed size dimensions of array or collections stored in the TTree if any.
  static constexpr Int_t kMaxhits = 2175;

  // Declaration of leaf types
  //WCDAEvent       *Event;
  UInt_t          fUniqueID;
  UInt_t          fBits;
  UInt_t          nhit;
  ULong_t         evt;
  Int_t           hits_;
  UInt_t          hits_fUniqueID[kMaxhits];   //[hits_]
  UInt_t          hits_fBits[kMaxhits];   //[hits_]
  UInt_t          hits_fee[kMaxhits];   //[hits_]
  UInt_t          hits_ch[kMaxhits];   //[hits_]
  UInt_t          hits_anode_charge[kMaxhits];   //[hits_]
  UInt_t          hits_dynode_charge[kMaxhits];   //[hits_]
  UInt_t          hits_charge_over_range_id[kMaxhits];   //[hits_]
  UInt_t          hits_second[kMaxhits];   //[hits_]
  UInt_t          hits_year[kMaxhits];   //[hits_]
  UInt_t          hits_coarse_time[kMaxhits];   //[hits_]
  UInt_t          hits_high_th_fine_time[kMaxhits];   //[hits_]
  UInt_t          hits_low_th_fine_time[kMaxhits];   //[hits_]
  UInt_t          hits_temprature_sensor_info[kMaxhits];   //[hits_]
  ULong_t         hits_time[kMaxhits];   //[hits_]
  
  // List of branches
  TBranch        *b_Event_fUniqueID;   //!
  TBranch        *b_Event_fBits;   //!
  TBranch        *b_Event_nhit;   //!
  TBranch        *b_Event_evt;   //!
  TBranch        *b_Event_hits_;   //!
  TBranch        *b_hits_fUniqueID;   //!
  TBranch        *b_hits_fBits;   //!
  TBranch        *b_hits_fee;   //!
  TBranch        *b_hits_ch;   //!
  TBranch        *b_hits_anode_charge;   //!
  TBranch        *b_hits_dynode_charge;   //!
  TBranch        *b_hits_charge_over_range_id;   //!
  TBranch        *b_hits_second;   //!
  TBranch        *b_hits_year;   //!
  TBranch        *b_hits_coarse_time;   //!
  TBranch        *b_hits_high_th_fine_time;   //!
  TBranch        *b_hits_low_th_fine_time;   //!
  TBranch        *b_hits_temprature_sensor_info;   //!
  TBranch        *b_hits_time;   //!

  wcdaevent(TTree *tree=0);
  wcdaevent(string path, string file);

  virtual ~wcdaevent();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual void     Loop0();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
};
#endif

#ifdef wcdaevent_cxx
wcdaevent::wcdaevent(TTree *tree) : fChain(0){
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if(tree == 0){
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/workfs/ybj/liuwei/event_time/data/ES.10001.WCDAEvent.20190215005726.007.root");
    if(!f || !f->IsOpen()){
      f = new TFile("/workfs/ybj/liuwei/event_time/data/ES.10001.WCDAEvent.20190215005726.007.root");
    }
    f->GetObject("wcda",tree);
  }

  Init(tree);
}

wcdaevent::wcdaevent(const string path, const string file) : fChain(0){
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  string fpath = path +"/" +file;
  TTree *tree;

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fpath.c_str() );
  if(!f || !f->IsOpen() ){
    f = new TFile(fpath.c_str());
  }
  f->GetObject("wcda",tree);

  Init(tree);
}


wcdaevent::~wcdaevent(){
  if(!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t wcdaevent::GetEntry(Long64_t entry){
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t wcdaevent::LoadTree(Long64_t entry){
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }

  return centry;
}

void wcdaevent::Init(TTree *tree){
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);
  
  fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_Event_fUniqueID);
  fChain->SetBranchAddress("fBits", &fBits, &b_Event_fBits);
  fChain->SetBranchAddress("nhit", &nhit, &b_Event_nhit);
  fChain->SetBranchAddress("evt", &evt, &b_Event_evt);
  fChain->SetBranchAddress("hits", &hits_, &b_Event_hits_);
  fChain->SetBranchAddress("hits.fUniqueID", hits_fUniqueID, &b_hits_fUniqueID);
  fChain->SetBranchAddress("hits.fBits", hits_fBits, &b_hits_fBits);
  fChain->SetBranchAddress("hits.fee", hits_fee, &b_hits_fee);
  fChain->SetBranchAddress("hits.ch", hits_ch, &b_hits_ch);
  fChain->SetBranchAddress("hits.anode_charge", hits_anode_charge, &b_hits_anode_charge);
  fChain->SetBranchAddress("hits.dynode_charge", hits_dynode_charge, &b_hits_dynode_charge);
  fChain->SetBranchAddress("hits.charge_over_range_id", hits_charge_over_range_id, &b_hits_charge_over_range_id);
  fChain->SetBranchAddress("hits.second", hits_second, &b_hits_second);
  fChain->SetBranchAddress("hits.year", hits_year, &b_hits_year);
  fChain->SetBranchAddress("hits.coarse_time", hits_coarse_time, &b_hits_coarse_time);
  fChain->SetBranchAddress("hits.high_th_fine_time", hits_high_th_fine_time, &b_hits_high_th_fine_time);
  fChain->SetBranchAddress("hits.low_th_fine_time", hits_low_th_fine_time, &b_hits_low_th_fine_time);
  fChain->SetBranchAddress("hits.temprature_sensor_info", hits_temprature_sensor_info, &b_hits_temprature_sensor_info);
  fChain->SetBranchAddress("hits.time", hits_time, &b_hits_time);
  
  Notify();
}

Bool_t wcdaevent::Notify(){
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  
  return kTRUE;
}

void wcdaevent::Show(Long64_t entry){
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t wcdaevent::Cut(Long64_t entry){
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.

  return 1;
}
#endif // #ifdef wcdaevent_cxx
