#include <iostream>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TF1.h"
#include "TNtuple.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TChain.h"
#include "TMath.h"
#include "TLatex.h"
#include "TStyle.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc,char* argv[]){
gStyle->SetOptFit(0001);

   TChain          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

  fChain=new TChain("tr2");
  //fChain->Add("ES.13091.FULL.PHYSICS.20190322064336.017.dat.root");
  fChain->Add("/scratchfs/bes/lixr/tmp1/tes/all0408.root");

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           ncbig;
   Int_t           i_maxbignpe;
   Int_t           i_mindt;
   Int_t           i_minbigdftime;
   Int_t           i_mindtanode;
   Int_t           i_minbigtime;
   Int_t           ncsm;
   Int_t           igcell;
   Double_t        big3_npe[4];   //[ncbig]
   Double_t        big3_anode[4];   //[ncbig]
   Double_t        big3_dynode[4];   //[ncbig]
   Double_t        big3_dftime[4];   //[ncbig]
   Double_t        dtime[4];   //[ncbig]
   Double_t        dtanode[4];   //[ncbig]
   Double_t        sm3_npe[4];   //[ncbig]
   Double_t        sm3_anode[4];   //[ncbig]
   Double_t        sm3_dynode[4];   //[ncbig]
   Double_t        sm3_dftime[4];   //[ncbig]
   Double_t        sm3_dtanode[4];   //[ncbig]
   Double_t        sm3_dtdynode[4];   //[ncbig]
   Int_t           ievt;
   Int_t           filetime;
   Double_t        big_tmean;
   Double_t        big_tfitmean;
   Double_t        big_tfitsigma;
   Int_t           sm_nhit;
   Int_t           big_nhit;

   // List of branches
   TBranch        *b_ncbig;   //!
   TBranch        *b_i_maxbignpe;   //!
   TBranch        *b_i_mindt;   //!
   TBranch        *b_i_minbigdftime;   //!
   TBranch        *b_i_mindtanode;   //!
   TBranch        *b_i_minbigtime;   //!
   TBranch        *b_ncsm;   //!
   TBranch        *b_igcell;   //!
   TBranch        *b_big3_npe;   //!
   TBranch        *b_big3_anode;   //!
   TBranch        *b_big3_dynode;   //!
   TBranch        *b_big3_dftime;   //!
   TBranch        *b_dtime;   //!
   TBranch        *b_dtanode;   //!
   TBranch        *b_sm3_npe;   //!
   TBranch        *b_sm3_anode;   //!
   TBranch        *b_sm3_dynode;   //!
   TBranch        *b_sm3_dftime;   //!
   TBranch        *b_sm3_dtanode;   //!
   TBranch        *b_sm3_dtdynode;   //!
   TBranch        *b_ievt;   //!
   TBranch        *b_filetime;   //!
   TBranch        *b_big_tmean;   //!
   TBranch        *b_big_tfitmean;   //!
   TBranch        *b_big_tfitsigma;   //!
   TBranch        *b_sm_nhit;   //!
   TBranch        *b_big_nhit;   //!

//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ES.13091.FULL.PHYSICS.20190322064336.017.dat.root");
//   if (!tree) return;
//   fChain = tree;
//   fCurrent = -1;

   fChain->SetBranchAddress("ncbig", &ncbig, &b_ncbig);
   fChain->SetBranchAddress("i_maxbignpe", &i_maxbignpe, &b_i_maxbignpe);
   fChain->SetBranchAddress("i_mindt", &i_mindt, &b_i_mindt);
   fChain->SetBranchAddress("i_minbigdftime", &i_minbigdftime, &b_i_minbigdftime);
   fChain->SetBranchAddress("i_mindtanode", &i_mindtanode, &b_i_mindtanode);
   fChain->SetBranchAddress("i_minbigtime", &i_minbigtime, &b_i_minbigtime);
   fChain->SetBranchAddress("ncsm", &ncsm, &b_ncsm);
   fChain->SetBranchAddress("igcell", &igcell, &b_igcell);
   fChain->SetBranchAddress("big3_npe", big3_npe, &b_big3_npe);
   fChain->SetBranchAddress("big3_anode", big3_anode, &b_big3_anode);
   fChain->SetBranchAddress("big3_dynode", big3_dynode, &b_big3_dynode);
   fChain->SetBranchAddress("big3_dftime", big3_dftime, &b_big3_dftime);
   fChain->SetBranchAddress("dtime", dtime, &b_dtime);
   fChain->SetBranchAddress("dtanode", dtanode, &b_dtanode);
   fChain->SetBranchAddress("sm3_npe", sm3_npe, &b_sm3_npe);
   fChain->SetBranchAddress("sm3_anode", sm3_anode, &b_sm3_anode);
   fChain->SetBranchAddress("sm3_dynode", sm3_dynode, &b_sm3_dynode);
   fChain->SetBranchAddress("sm3_dftime", sm3_dftime, &b_sm3_dftime);
   fChain->SetBranchAddress("sm3_dtanode", sm3_dtanode, &b_sm3_dtanode);
   fChain->SetBranchAddress("sm3_dtdynode", sm3_dtdynode, &b_sm3_dtdynode);
   fChain->SetBranchAddress("ievt", &ievt, &b_ievt);
   fChain->SetBranchAddress("filetime", &filetime, &b_filetime);
   fChain->SetBranchAddress("big_tmean", &big_tmean, &b_big_tmean);
   fChain->SetBranchAddress("big_tfitmean", &big_tfitmean, &b_big_tfitmean);
   fChain->SetBranchAddress("big_tfitsigma", &big_tfitsigma, &b_big_tfitsigma);
   fChain->SetBranchAddress("sm_nhit", &sm_nhit, &b_sm_nhit);
   fChain->SetBranchAddress("big_nhit", &big_nhit, &b_big_nhit);

char buf1[500];
char buf2[500];
char buf3[500];
double rms1[900]; double rms2[900]; double rms3[900]; double rms4[900]; double rms5[900]; double rms6[900];
double mean1[900]; double mean2[900]; double mean3[900]; double mean4[900]; double mean5[900]; double mean6[900];

TH1F *hh1[900];
TH1F *hh2[900];
TH1F *hh3[900];
TH1F *hh4[900];
TH1F *hh5[900];
TH1F *hh6[900];
TH2F *hh7[900];
TH2F *hh8[900];
int cell0=atoi(argv[1]);
int cellend=cell0+100;
//int cellend=cell0+2;

cout<<"cell0="<<cell0<<endl;
TF1 *f1=new TF1("f1","gaus");
f1->SetLineColor(kRed);

TCanvas *c1=new TCanvas("c1","c1",1300,700);
c1->Divide(2,4); 
 for(int icell=cell0; icell<cellend; icell++)
  {
 c1->Update();

 c1->cd(1);
sprintf(buf1,"dtime>>h1_%d",icell);
sprintf(buf2,"igcell==%d&&ncbig==1&&sm3_npe[0]>20&&big3_npe[0]>1200",icell);
  fChain->Draw(buf1,buf2);
sprintf(buf1,"h1_%d",icell);
hh1[icell]=(TH1F*)gDirectory->Get(buf1);
hh1[icell]->GetXaxis()->SetTitle("time_sm-time_big (ns) ");
mean1[icell]=hh1[icell]->GetMean();
rms1[icell]=hh1[icell]->GetRMS();
hh1[icell]->Fit("f1","","",mean1[icell]-2*rms1[icell], mean1[icell]+2*rms1[icell]);
mean1[icell]=f1->GetParameter(1);
rms1[icell]=f1->GetParameter(2);

 c1->cd(2);
sprintf(buf1,"dtanode>>h2_%d",icell);
sprintf(buf2,"igcell==%d&&ncbig==1&&sm3_npe[0]>20&&big3_npe[0]>1200",icell);
  fChain->Draw(buf1,buf2);
sprintf(buf1,"h2_%d",icell);
hh2[icell]=(TH1F*)gDirectory->Get(buf1);
hh2[icell]->GetXaxis()->SetTitle("time_sm_anodepeak-time_big (ns) ");
mean2[icell]=hh2[icell]->GetMean();
rms2[icell]=hh2[icell]->GetRMS();
hh2[icell]->Fit("f1","","",mean2[icell]-2*rms2[icell], mean2[icell]+2*rms2[icell]);
mean2[icell]=f1->GetParameter(1);
rms2[icell]=f1->GetParameter(2);


 c1->cd(3);
sprintf(buf1,"dtime>>h3_%d",icell);
sprintf(buf2,"igcell==%d&&ncbig==1&&sm3_npe[0]>100&&big3_npe[0]>1200",icell);
  fChain->Draw(buf1,buf2);
sprintf(buf1,"h3_%d",icell);
hh3[icell]=(TH1F*)gDirectory->Get(buf1);
hh3[icell]->GetXaxis()->SetTitle("time_sm-time_big (ns) ");
mean3[icell]=hh3[icell]->GetMean();
rms3[icell]=hh3[icell]->GetRMS();
hh3[icell]->Fit("f1","","",mean3[icell]-2*rms3[icell], mean3[icell]+2*rms3[icell]);
mean3[icell]=f1->GetParameter(1);
rms3[icell]=f1->GetParameter(2);

 c1->cd(4);
sprintf(buf1,"dtanode>>h4_%d",icell);
sprintf(buf2,"igcell==%d&&ncbig==1&&sm3_npe[0]>100&&big3_npe[0]>1200",icell);
  fChain->Draw(buf1,buf2);
sprintf(buf1,"h4_%d",icell);
hh4[icell]=(TH1F*)gDirectory->Get(buf1);
hh4[icell]->GetXaxis()->SetTitle("time_sm_anodepeak-time_big (ns) ");
mean4[icell]=hh4[icell]->GetMean();
rms4[icell]=hh4[icell]->GetRMS();
hh4[icell]->Fit("f1","","",mean4[icell]-2*rms4[icell], mean4[icell]+2*rms4[icell]);
mean4[icell]=f1->GetParameter(1);
rms4[icell]=f1->GetParameter(2);



 c1->cd(5);
sprintf(buf1,"dtime>>h5_%d",icell);
sprintf(buf2,"igcell==%d&&ncbig==1&&sm3_npe[0]>300&&big3_npe[0]>1200",icell);
  fChain->Draw(buf1,buf2);
sprintf(buf1,"h5_%d",icell);
hh5[icell]=(TH1F*)gDirectory->Get(buf1);
hh5[icell]->GetXaxis()->SetTitle("time_sm-time_big (ns) ");
mean5[icell]=hh5[icell]->GetMean();
rms5[icell]=hh5[icell]->GetRMS();

 c1->cd(6);
sprintf(buf1,"dtanode>>h6_%d",icell);
sprintf(buf2,"igcell==%d&&ncbig==1&&sm3_npe[0]>300&&big3_npe[0]>1200",icell);
  fChain->Draw(buf1,buf2);
sprintf(buf1,"h6_%d",icell);
hh6[icell]=(TH1F*)gDirectory->Get(buf1);
hh6[icell]->GetXaxis()->SetTitle("time_sm_anodepeak-time_big (ns) ");
mean6[icell]=hh6[icell]->GetMean();
rms6[icell]=hh6[icell]->GetRMS();



 c1->cd(7);
gPad->SetGridx();
gPad->SetGridy();
sprintf(buf1,"dtime:sm3_npe[0]>>h7_%d",icell);
sprintf(buf2,"igcell==%d&&ncbig==1&&big3_npe[0]>200",icell);
  fChain->Draw(buf1,buf2);
sprintf(buf1,"h7_%d",icell);
hh7[icell]=(TH2F*)gDirectory->Get(buf1);
hh7[icell]->GetXaxis()->SetTitle("sm_NPE");
hh7[icell]->GetYaxis()->SetTitle("time_sm-time_big (ns)");


 c1->cd(8);
gPad->SetGridx();
gPad->SetGridy();
sprintf(buf1,"dtanode:sm3_npe[0]>>h8_%d",icell);
sprintf(buf2,"igcell==%d&&ncbig==1&&big3_npe[0]>200",icell);
  fChain->Draw(buf1,buf2);
sprintf(buf1,"h8_%d",icell);
hh8[icell]=(TH2F*)gDirectory->Get(buf1);
hh8[icell]->GetXaxis()->SetTitle("sm_NPE");
hh8[icell]->GetYaxis()->SetTitle("time_sm_anodepeak-time_big (ns)");




sprintf(buf1,"/scratchfs/bes/lixr/tmp1/tes/pic/cell_%03d.pdf",icell);
c1->SaveAs(buf1);

  } 

sprintf(buf1,"/scratchfs/bes/lixr/tmp1/tes/matchroot/fout%d.root",cell0);
TFile *fout=new TFile(buf1,"recreate");
TNtuple *tmatch=new TNtuple("tmatch","tmatch","dtmean1:dtmean2:dtmean3:dtmean4:dtmean5:dtmean6:dtrms1:dtrms2:dtrms3:dtrms4:dtrms5:dtrms6:igcell");

for(int icell=cell0; icell<cellend; icell++) {
tmatch->Fill(mean1[icell],mean2[icell],mean3[icell],mean4[icell],mean5[icell],mean6[icell],rms1[icell],rms2[icell],rms3[icell],rms4[icell],rms5[icell],rms6[icell],icell);
}
fout->Write();

}
