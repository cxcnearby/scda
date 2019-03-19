//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Feb 23 18:38:00 2019 by ROOT version 5.34/03
// from TTree wcdapls/WCDAPLS TimedFragment Hits
// found on file: ES.10017.TimedFrag.20190215143314.001.dat.root
//////////////////////////////////////////////////////////

#ifndef wcdapls_h
#define wcdapls_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TObject.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class wcdapls {
 public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  
  // Declaration of leaf types
  //WCDAPLSHit      *Event;
  UInt_t          fUniqueID;
  UInt_t          fBits;
  UInt_t          fee;
  UInt_t          flag;
  UInt_t          bdb;
  UInt_t          db;
  UInt_t          pmt;
  ULong_t         time;
  UInt_t          second;
  UInt_t          ns;
  UInt_t          anode_peak;
  UInt_t          anode_ped;
  UInt_t          anode_time;
  UInt_t          dynode_peak;
  UInt_t          dynode_ped;
  UInt_t          dynode_time;
  UInt_t          tail;
  UInt_t          l1id;
  Int_t           dt;
  
  // List of branches
  TBranch        *b_Event_fUniqueID;   //!
  TBranch        *b_Event_fBits;   //!
  TBranch        *b_Event_fee;   //!
  TBranch        *b_Event_flag;   //!
  TBranch        *b_Event_bdb;   //!
  TBranch        *b_Event_db;   //!
  TBranch        *b_Event_pmt;   //!
  TBranch        *b_Event_time;   //!
  TBranch        *b_Event_second;   //!
  TBranch        *b_Event_ns;   //!
  TBranch        *b_Event_anode_peak;   //!
  TBranch        *b_Event_anode_ped;   //!
  TBranch        *b_Event_anode_time;   //!
  TBranch        *b_Event_dynode_peak;   //!
  TBranch        *b_Event_dynode_ped;   //!
  TBranch        *b_Event_dynode_time;   //!
  TBranch        *b_Event_tail;   //!
  TBranch        *b_Event_l1id;   //!
  TBranch        *b_Event_dt;   //!
  
  wcdapls(TTree *tree=0);
  wcdapls(const string path, const string file);

  virtual ~wcdapls();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
};
#endif

#ifdef wcdapls_cxx
wcdapls::wcdapls(TTree *tree) : fChain(0){
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if(tree == 0){
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/workfs/ybj/liuwei/event_time/data/ES.10003.TimedFrag.20190215004023.007.dat.root");
    if(!f || !f->IsOpen()){
      f = new TFile("/workfs/ybj/liuwei/event_time/data/ES.10003.TimedFrag.20190215004023.007.dat.root");
    }
    f->GetObject("wcdapls",tree);
  }

  Init(tree);
}

wcdapls::wcdapls(const string path, const string file) : fChain(0){
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  string fpath = path +"/" +file;
  TTree *tree;

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fpath.c_str() );
  if(!f || !f->IsOpen() ){
    f = new TFile(fpath.c_str() );
  }
  f->GetObject("wcdapls",tree);

  Init(tree);
}



wcdapls::~wcdapls(){
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t wcdapls::GetEntry(Long64_t entry){
  // Read contents of entry.
  if (!fChain) return 0;
  
  return fChain->GetEntry(entry);
}

Long64_t wcdapls::LoadTree(Long64_t entry){
  // Set the environment to read one entry
  if(!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if(centry < 0) return centry;
  if(fChain->GetTreeNumber() != fCurrent){
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
   
  return centry;
}

void wcdapls::Init(TTree *tree){
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
  fChain->SetBranchAddress("flag", &flag, &b_Event_flag);
  fChain->SetBranchAddress("bdb", &bdb, &b_Event_bdb);
  fChain->SetBranchAddress("db", &db, &b_Event_db);
  fChain->SetBranchAddress("pmt", &pmt, &b_Event_pmt);
  fChain->SetBranchAddress("time", &time, &b_Event_time);
  fChain->SetBranchAddress("second", &second, &b_Event_second);
  fChain->SetBranchAddress("ns", &ns, &b_Event_ns);
  fChain->SetBranchAddress("anode_peak", &anode_peak, &b_Event_anode_peak);
  fChain->SetBranchAddress("anode_ped", &anode_ped, &b_Event_anode_ped);
  fChain->SetBranchAddress("anode_time", &anode_time, &b_Event_anode_time);
  fChain->SetBranchAddress("dynode_peak", &dynode_peak, &b_Event_dynode_peak);
  fChain->SetBranchAddress("dynode_ped", &dynode_ped, &b_Event_dynode_ped);
  fChain->SetBranchAddress("dynode_time", &dynode_time, &b_Event_dynode_time);
  fChain->SetBranchAddress("tail", &tail, &b_Event_tail);
  fChain->SetBranchAddress("l1id", &l1id, &b_Event_l1id);
  fChain->SetBranchAddress("dt", &dt, &b_Event_dt);

  Notify();
}

Bool_t wcdapls::Notify(){
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void wcdapls::Show(Long64_t entry){
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t wcdapls::Cut(Long64_t entry){
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}
#endif // #ifdef wcdapls_cxx
