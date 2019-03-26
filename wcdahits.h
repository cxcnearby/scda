//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar 20 16:13:30 2019 by ROOT version 6.14/00
// from TTree wcda/WCDA TimedFragment Hits
// found on file: /eos/lhaaso/experiment/wcda/decoded/2019/0219/ES.10001.TimedFrag.20190219164631.007.root
//////////////////////////////////////////////////////////

#ifndef wcdahits_h
#define wcdahits_h

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


// Header file for the classes stored in the TTree if any.
#include "TObject.h"

using namespace std;

class wcdahits {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
 //WCDAHit         *Event;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   UInt_t          fee;
   UInt_t          ch;
   UInt_t          anode_charge;
   UInt_t          dynode_charge;
   UInt_t          charge_over_range_id;
   UInt_t          second;
   UInt_t          year;
   UInt_t          coarse_time;
   UInt_t          high_th_fine_time;
   UInt_t          low_th_fine_time;
   UInt_t          temprature_sensor_info;
   ULong_t         time;

   // List of branches
   TBranch        *b_Event_fUniqueID;   //!
   TBranch        *b_Event_fBits;   //!
   TBranch        *b_Event_fee;   //!
   TBranch        *b_Event_ch;   //!
   TBranch        *b_Event_anode_charge;   //!
   TBranch        *b_Event_dynode_charge;   //!
   TBranch        *b_Event_charge_over_range_id;   //!
   TBranch        *b_Event_second;   //!
   TBranch        *b_Event_year;   //!
   TBranch        *b_Event_coarse_time;   //!
   TBranch        *b_Event_high_th_fine_time;   //!
   TBranch        *b_Event_low_th_fine_time;   //!
   TBranch        *b_Event_temprature_sensor_info;   //!
   TBranch        *b_Event_time;   //!

   wcdahits(TTree *tree=0);
   wcdahits(const string path, const string file);

   virtual ~wcdahits();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef wcdahits_cxx
wcdahits::wcdahits(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/lhaaso/experiment/wcda/decoded/2019/0219/ES.10001.TimedFrag.20190219164631.007.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/eos/lhaaso/experiment/wcda/decoded/2019/0219/ES.10001.TimedFrag.20190219164631.007.root");
      }
      f->GetObject("wcda",tree);

   }
   Init(tree);
}

wcdahits::wcdahits(const string path, const string file) : fChain(0){
  string fpath = path +"/" +file;
  TTree *tree;

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fpath.c_str() );
  if(!f || !f->IsOpen() ){
    f = new TFile(fpath.c_str() );
  }
  f->GetObject("wcda",tree);

  Init(tree);
}



wcdahits::~wcdahits()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t wcdahits::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t wcdahits::LoadTree(Long64_t entry)
{
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

void wcdahits::Init(TTree *tree)
{
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
   fChain->SetBranchAddress("fee", &fee, &b_Event_fee);
   fChain->SetBranchAddress("ch", &ch, &b_Event_ch);
   fChain->SetBranchAddress("anode_charge", &anode_charge, &b_Event_anode_charge);
   fChain->SetBranchAddress("dynode_charge", &dynode_charge, &b_Event_dynode_charge);
   fChain->SetBranchAddress("charge_over_range_id", &charge_over_range_id, &b_Event_charge_over_range_id);
   fChain->SetBranchAddress("second", &second, &b_Event_second);
   fChain->SetBranchAddress("year", &year, &b_Event_year);
   fChain->SetBranchAddress("coarse_time", &coarse_time, &b_Event_coarse_time);
   fChain->SetBranchAddress("high_th_fine_time", &high_th_fine_time, &b_Event_high_th_fine_time);
   fChain->SetBranchAddress("low_th_fine_time", &low_th_fine_time, &b_Event_low_th_fine_time);
   fChain->SetBranchAddress("temprature_sensor_info", &temprature_sensor_info, &b_Event_temprature_sensor_info);
   fChain->SetBranchAddress("time", &time, &b_Event_time);
   Notify();
}

Bool_t wcdahits::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void wcdahits::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t wcdahits::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef wcdahits_cxx
