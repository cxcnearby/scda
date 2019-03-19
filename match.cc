#define wcdaevent_cxx
#define wcdapls_cxx
#include "wcdaevent.h"
#include "wcdapls.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TNtupleD.h>
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

double smpmtx_jd[26][4][17];
double smpmty_jd[26][4][17];
double bigpmtx[101][10];
double bigpmty[101][10];
std::vector<int> smevt;
std::vector<Long64_t> sectime;
std::vector<Long64_t> smievt;
std::vector<Long64_t> smtime;
std::vector<Long64_t> smsecond;
std::vector<Long64_t> smns;
std::vector<int> smanode_peak;
std::vector<int> smanode_ped;
std::vector<int> smdynode_peak;
std::vector<int> smdynode_ped;
std::vector<int> smfee;
std::vector<int> smdb;
std::vector<int> smpmt;
int big_nhit; int sm_nhit; 
//TFile *f; 
//TTree *tr;
long bigsecond_evt0; double bigns_evt0;
long bigsecond_evtend; double bigns_evtend;
int smevt0=0;
int bigevt0=0;
int bigevt1=200;

void wcdaevent::Loop0()
{
 if (fChain == 0) return;
//int bigevt=17476;
LoadTree(bigevt0);
fChain->GetEntry(bigevt0);
bigsecond_evt0=hits_second[0];
bigns_evt0=hits_low_th_fine_time[0]*0.333+hits_coarse_time[0]*16.;

bigevt1=fChain->GetEntriesFast()-10;
//bigevt1=20;
LoadTree(bigevt1);
fChain->GetEntry(bigevt1);
bigsecond_evtend=hits_second[0];
bigns_evtend=hits_low_th_fine_time[0]*0.333+hits_coarse_time[0]*16.;


}

void wcdaevent::Loop()
{
#include "smpmtpos.h"
#include "bigpmtpos.h"
/*
TFile *fsm=new TFile("smpmt.root","read");
int fee_1; int db_1; int pmt_1; Long64_t time_1; Long64_t second_1; Long64_t ns_1; double anode_1; double dynode_1; int smevt_1; int evt_1;
TTree *trpls=(TTree*)fsm->Get("trpls");

trpls->SetBranchAddress("fee_1",&fee_1);
trpls->SetBranchAddress("db_1",&db_1);
trpls->SetBranchAddress("pmt_1",&pmt_1);
trpls->SetBranchAddress("time_1",&time_1);
trpls->SetBranchAddress("second_1",&second_1);
trpls->SetBranchAddress("ns_1",&ns_1);
trpls->SetBranchAddress("anode_1",&anode_1);
trpls->SetBranchAddress("dynode_1",&dynode_1);
trpls->SetBranchAddress("smevt_1",&smevt_1);
trpls->SetBranchAddress("evt_1",&evt_1);

*/

int ievt=0;
int big_evt;  

double sm_maxanode=-100;  double sm_maxdynode=-100;
double big_maxanode=-100;  double big_maxdynode=-100;
const Int_t kMaxTrack = 10000;
const Int_t kMaxTrack2 = 10000;
int sm_time[kMaxTrack];
int sm_fee[kMaxTrack];
int sm_db[kMaxTrack];
int sm_ch[kMaxTrack];
int sm_evt[kMaxTrack];
double sm_anode[kMaxTrack];
double sm_dynode[kMaxTrack];
double sm_pmtx[kMaxTrack];
double sm_pmty[kMaxTrack];
double sm_pmtz[kMaxTrack];
Long64_t sm_dftime[kMaxTrack];

long big_second0; 
double big_ns0;
long big_second[kMaxTrack2];
double big_ns[kMaxTrack2];
double big_pmtx[kMaxTrack2];
double big_pmty[kMaxTrack2];
double big_pmtz[kMaxTrack2];
Long64_t big_dftime[kMaxTrack2];
int big_anode[kMaxTrack2];
int big_dynode[kMaxTrack2];
int big_fee[kMaxTrack2];
int big_ch[kMaxTrack2];

long dftime_mean;
std::cout<<"111"<<std::endl;
TFile *f=new TFile("/scratchfs/bes/lixr/tmp1/fout.root","recreate");
TTree *tr=new TTree("tr","tr");

tr->Branch("big_nhit",&big_nhit,"big_nhit/I");
tr->Branch("big_evt",&big_evt,"big_evt/I");
tr->Branch("ievt",&ievt,"ievt/I");
tr->Branch("sm_nhit",&sm_nhit,"sm_nhit/I");

tr->Branch("dftime_mean",&dftime_mean,"dftime_mean/L");
tr->Branch("sm_maxanode",&sm_maxanode,"sm_maxanode/D");
tr->Branch("sm_maxdynode",&sm_maxdynode,"sm_maxdynode/D");
//tr->Branch("sm_time",sm_time,"sm_time[sm_nhit]/L");
tr->Branch("sm_anode",sm_anode,"sm_anode[sm_nhit]/D");
tr->Branch("sm_dynode",sm_dynode,"sm_dynode[sm_nhit]/D");
//tr->Branch("sm_evt",sm_evt,"sm_evt[sm_nhit]/I");
tr->Branch("sm_pmtx",sm_pmtx,"sm_pmtx[sm_nhit]/D");
tr->Branch("sm_pmty",sm_pmty,"sm_pmty[sm_nhit]/D");
tr->Branch("sm_pmtz",sm_pmtz,"sm_pmtz[sm_nhit]/D");
tr->Branch("sm_dftime",sm_dftime,"sm_dftime[sm_nhit]/L");
//tr->Branch("sm_fee",sm_fee,"sm_fee[sm_nhit]/I");
//tr->Branch("sm_db",sm_db,"sm_db[sm_nhit]/I");
//tr->Branch("sm_ch",sm_ch,"sm_ch[sm_nhit]/I");

//tr->Branch("big_second0",&big_second0,"big_second0/L");
//tr->Branch("big_ns0",&big_ns0,"big_ns0/D");
//tr->Branch("big_maxanode",&big_maxanode,"big_maxanode/D");
//tr->Branch("big_maxdynode",&big_maxdynode,"big_maxdynode/D");
tr->Branch("big_second",big_second,"big_second[big_nhit]/L");
tr->Branch("big_ns",big_ns,"big_ns[big_nhit]/D");
tr->Branch("big_anode",big_anode,"big_anode[big_nhit]/I");
tr->Branch("big_dynode",big_dynode,"big_dynode[big_nhit]/I");
tr->Branch("big_dftime",big_dftime,"big_dftime[big_nhit]/I");
//tr->Branch("big_fee",big_fee,"big_fee[big_nhit]/I");
//tr->Branch("big_ch",big_ch,"big_ch[big_nhit]/I");
tr->Branch("big_pmtx",big_pmtx,"big_pmtx[big_nhit]/D");
tr->Branch("big_pmty",big_pmty,"big_pmty[big_nhit]/D");
//tr->Branch("big_pmtz",big_pmtz,"big_pmtz[big_nhit]/D");


  if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=bigevt0; jentry<bigevt1;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
//      nb = 
fChain->GetEntry(jentry);   
//      nbytes += nb;

  Long64_t bigsecond=hits_second[0]; 
  double bigns=hits_low_th_fine_time[0]*0.333+hits_coarse_time[0]*16.;
//  Long64_t smsecond; int smns;
if(jentry%1000==0) cout<<"bigevt="<<jentry<<",big_nhit="<<nhit<<endl;
big_nhit=nhit;
big_evt=jentry;

big_second0=bigsecond; 
big_ns0=bigns;
big_maxanode=-100;
big_maxdynode=-100;
long big_secondtmp=0;
double big_nsmeantmp=0;
for(int ibig=0; ibig<big_nhit; ibig++)
{
//cout<<"ibig="<<ibig<<endl;
big_fee[ibig]=hits_fee[ibig];
big_ch[ibig]=hits_ch[ibig];
big_pmtx[ibig]=bigpmtx[big_fee[ibig]][big_ch[ibig]];
big_pmty[ibig]=bigpmty[big_fee[ibig]][big_ch[ibig]];
big_pmtz[ibig]=0;
big_anode[ibig]=hits_anode_charge[ibig];
big_dynode[ibig]=hits_dynode_charge[ibig];
big_second[ibig]=hits_second[ibig];
big_ns[ibig]=hits_low_th_fine_time[ibig]*0.333+hits_coarse_time[ibig]*16.;
big_dftime[ibig]=(big_second[ibig]-bigsecond)*1000000000LL+(big_ns[ibig]-bigns);

if(big_anode[ibig]>big_maxanode) big_maxanode=big_anode[ibig];
if(big_dynode[ibig]>big_maxdynode) big_maxdynode=big_dynode[ibig];
big_secondtmp=big_secondtmp+big_second[ibig]-big_second0;
big_nsmeantmp=big_nsmeantmp+big_ns[ibig]-big_ns0;
//cout<<"b222"<<endl;
}
double big_timemean0=(big_secondtmp*1e9+big_nsmeantmp)/big_nhit;

double dftime=10000000;
 dftime_mean=0;
int smnhit=0;
sm_maxanode=-100;
sm_maxdynode=-100;
long sm_secondtmp=0;
double sm_nstmp=0;

for(int i=smevt0; i<smsecond.size(); i++)
{

dftime=(smsecond[i]+1-bigsecond)*10000000000LL+(smns[i]-bigns);
//Long64_t dfsecond=smsecond-bigsecond;
//dftime=dftime+0.6*1e9;
//cout<<"dftime="<<dftime<<endl;
//printf("ibig=%d, ism=%d,dftime=%f,smsecond=%ld,bigsecond=%ld,dfsecond=%ld\n",jentry, i, dftime,smsecond,bigsecond,dfsecond);
double timewin=4000;
if(fabs(dftime)<timewin)
{

cout<<"smnhit="<<smnhit<<endl;
//dftime_mean=dftime_mean+dftime;
sm_anode[smnhit]=smanode_peak[i]-smanode_ped[i];
cout<<"444"<<endl;
sm_dynode[smnhit]=smdynode_peak[i]-smdynode_ped[i];
sm_pmtx[smnhit]=smpmtx_jd[smfee[i]][smdb[i]][smpmt[i]];
sm_pmty[smnhit]=smpmty_jd[smfee[i]][smdb[i]][smpmt[i]];
sm_pmtz[smnhit]=0;
sm_time[smnhit]=smtime[i];
sm_dftime[smnhit]=dftime;
//sm_fee[smnhit]=fee_1;
//sm_db[smnhit]=db_1;
//sm_ch[smnhit]=pmt_1;
//sm_evt[smnhit]=smievt[i];
//if(anode_1>sm_maxanode) sm_maxanode=anode_1;
//if(dynode_1>sm_maxdynode) sm_maxdynode=dynode_1;

//sm_secondtmp=sm_secondtmp+(smsecond-big_second0);
//sm_nstmp=sm_nstmp+(smns-big_ns0);
//cout<<"bigi="<<jentry<<",smi="<<i<<",fee="<<sm_fee[smnhit]<<",db="<<sm_db[smnhit]<<",pmt="<<sm_ch[smnhit]<<",x="<<sm_pmtx[smnhit]<<",y="<<sm_pmty[smnhit]<<endl;

smnhit++;
//std::cout<<"bigi="<<i<<",time="<<sectime[i]<<",smsecond="<<smsecond<<",smns="<<smns<<",bigsecond="<<bigsecond<<",bigns="<<bigns<<",dftime="<<dftime<<std::endl;
}

//double sm_timemean0=(sm_secondtmp*1e9+sm_nstmp)/sm_nhit;
//dftime_mean=(dftime_mean)/sm_nhit;
sm_nhit=smnhit;
if(big_nhit>=10000||sm_nhit>=10000) cout<<"bignum,big_nhit="<<big_nhit<<",sm_nhit="<<sm_nhit<<endl;
//cout<<"dftime_mean="<<dftime_mean<<endl;
if(dftime>timewin) 
{smevt0=i; 
break;}
}
//cout<<"ievt="<<ievt<<",big_evt="<<big_evt<<",smevt0="<<smevt0<<"big_nhit="<<big_nhit<<",sm_nhit="<<sm_nhit<<",bigsecod="<<bigsecond<<",bigns="<<bigns<<endl;
//if(ievt>3) break; 

ievt=ievt+1; 
tr->Fill();
//cout<<"bigi="<<jentry<<"fill"<<endl;
}
cout<<"write1"<<endl;
//f->Write();
cout<<"write2"<<endl;
//delete f;
}

void wcdapls::Loop()
{
int fee_1; int db_1; int pmt_1; Long64_t time_1; Long64_t second_1; Long64_t ns_1; double anode_1; double dynode_1; int smevt_1; int evt_1;
/*
TFile *foutsm=new TFile("smpmt.root","recreate");
TTree *trpls=new TTree("trpls","trpls");

trpls->Branch("fee_1",&fee_1,"fee_1/I");
trpls->Branch("db_1",&db_1,"db_1/I");
trpls->Branch("pmt_1",&pmt_1,"pmt_1/I");
trpls->Branch("time_1",&time_1,"time_1/L");
trpls->Branch("second_1",&second_1,"second_1/L");
trpls->Branch("ns_1",&ns_1,"ns_1/L");
trpls->Branch("anode_1",&anode_1,"anode_1/D");
trpls->Branch("dynode_1",&dynode_1,"dynode_1/D");
trpls->Branch("smevt_1",&smevt_1,"smevt_1/I");
trpls->Branch("evt_1",&evt_1,"evt_1/I");
*/
//#include "smpmtpos.h"
smevt.clear();
sectime.clear();

std::vector<int> tmpsmevt;
std::vector<long> tmpsectime;
tmpsmevt.clear();
tmpsectime.clear();


double dfsecond=-100000;
Long64_t nentries = fChain->GetEntriesFast();
//nentries=2100;
 for (Long64_t jentry=0; jentry<nentries;jentry++) {
fChain->GetEntry(jentry); 
dfsecond=(second-bigsecond_evtend);
if(dfsecond>100) break;

smevt.push_back(jentry);
sectime.push_back(second*1000000000LL+ns*20LL);

long tmp1; int tmp2; 
if(jentry==1&&(sectime[jentry]<sectime[jentry-1])){ 
   tmp1=sectime[jentry];   sectime[jentry]=sectime[jentry-1];   sectime[jentry-1]=tmp1;
  tmp2=smevt[jentry];   smevt[jentry]=smevt[jentry-1]; smevt[jentry-1]=tmp2;
  }

  if(jentry>1){
    if(sectime[jentry]>=sectime[jentry-1]) continue;
  
    if(sectime[jentry]<sectime[jentry-1])
    {
     tmpsmevt.clear();
     tmpsectime.clear();
     int j;
     for(j=1; j<=jentry; j++)
      { 
       tmpsectime.push_back(sectime[jentry-j]);
       tmpsmevt.push_back(smevt[jentry-j]);
      if(j<jentry&&sectime[jentry]<=sectime[jentry-j]&&(sectime[jentry]>=sectime[jentry-j-1])) 
        {
        break;
        }
       }
  if(j==(jentry+1)) j=j-1;
     sectime[jentry-j]=sectime[jentry];
     smevt[jentry-j]=smevt[jentry];
     int tmpnum=0; 
     for(int k=jentry; k>jentry-j; k--)
     {
     sectime[k]=tmpsectime[tmpnum];
     smevt[k]=tmpsmevt[tmpnum];
     tmpnum++;
     }
}
  }

}

smievt.clear();
smtime.clear();
smsecond.clear();
smns.clear();
smanode_peak.clear();
smanode_ped.clear();
smdynode_peak.clear();
smdynode_ped.clear();
smfee.clear();
smdb.clear();
smpmt.clear();

//fee:db:pmt:second:ns:anode:dynode:smevt:evt
for(int i=0; i<smevt.size(); i++)
{
//cout<<"111"<<endl;
fChain->GetEntry(smevt[i]);
//if((second-bigsecond_evt0)>-5)
{
smievt.push_back(smevt[i]);
smtime.push_back(second*1000000000LL+ns*20LL);
smsecond.push_back(second);
smns.push_back(ns*20LL);
smanode_peak.push_back(anode_peak);
smanode_ped.push_back(anode_ped);
smdynode_peak.push_back(dynode_peak);
smdynode_ped.push_back(dynode_ped);
smfee.push_back(fee);
smdb.push_back(db);
smpmt.push_back(pmt);
//fee_1=fee; db_1=db; pmt_1=pmt; time_1=time; second_1=second; ns_1=ns; anode_1=anode_peak-anode_ped; dynode_1=dynode_peak-dynode_ped;
//smevt_1=smevt[i]; evt_1=i;
//trpls->Fill();
 }
}
//foutsm->Write();


/*
long dftime;
for(int i=0; i<smsecond.size(); i++)
{
dftime=(smsecond[i]-smsecond[i-1])*1000000000LL+(smns[i]-smns[i-1]);
if(dftime<0) {cout<<"noo"<<endl; 

cout<<"dftime2="<<(smtime[i]-smtime[i-1])<<endl;
}

}
*/

std::cout<<"nentries="<<nentries<<",length="<<sectime.size()<<",smevt0="<<smevt0<<std::endl;

}
int main(){
cout<<"x111"<<endl;


wcdapls m; 
wcdaevent t; 
t.Loop0();
m.Loop();
t.Loop();


return 0;
}
