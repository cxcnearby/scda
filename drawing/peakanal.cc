#include <TCanvas.h>
#include <TChain.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

Double_t powerfunc(Double_t *x, Double_t *par);
Double_t gaussfunc(Double_t *x, Double_t *par);
Double_t combfunc(Double_t *x, Double_t *par);

int main(int argc, char *argv[]) {

  int IfSavePNG = 0;   // not save pic
  int cellstart = 0;   // 0
  int cellend = 899;   // 899
  int dynode_th = 100; // 100
  int anode_th0 = 30;  // 30
  int anode_th1 = 100; // 80

  if (argc < 4) {
    printf("%s  input.root  outdir  picdir  [IfSavePNG]  [cellstart]  "
           "[cellend]  [dynode_th]  [anode_th0]  [anode_th1]\n",
           argv[0]);
    exit(0);
  }
  string infile = argv[1], outdir = argv[2], picdir = argv[3];
  if (argc >= 5) {
    IfSavePNG = atoi(argv[4]);
  }
  if (argc >= 7) {
    cellstart = atoi(argv[5]);
    cellend = atoi(argv[6]);
  }
  string outroot = outdir + "/anal_peak_" + to_string(cellstart) + ".root";
  if (argc >= 8) {
    dynode_th = atoi(argv[7]);
  }
  if (argc >= 9) {
    anode_th0 = atoi(argv[8]);
  }
  if (argc >= 10) {
    anode_th1 = atoi(argv[9]);
  }

  Int_t b_igcell;
  Long64_t b_b2sevent;
  double_t b_1par[2];
  double_t b_1parerr[2];

  double_t b_2par[3];
  double_t b_2parerr[3];

  double_t b_3par[5];
  double_t b_3parerr[5];

  TFile *f_anal = new TFile(outroot.c_str(), "recreate");
  TTree *t_bd2sa = new TTree("bd2sa", "dynode_b : anode_s prof fit");
  TTree *t_peakfit = new TTree("peakfit", "dynode_b & anode_s peak fit");

  t_bd2sa->Branch("igcell", &b_igcell);
  t_bd2sa->Branch("events", &b_b2sevent);
  t_bd2sa->Branch("par0", &b_1par[0]);
  t_bd2sa->Branch("par0err", &b_1parerr[0]);
  t_bd2sa->Branch("par1", &b_1par[1]);
  t_bd2sa->Branch("par1err", &b_1parerr[1]);

  t_peakfit->Branch("igcell", &b_igcell);
  t_peakfit->Branch("dynode_b_par0", &b_2par[0], "landau fit");
  t_peakfit->Branch("dynode_b_par0err", &b_2parerr[0]);
  t_peakfit->Branch("dynode_b_par1", &b_2par[1]);
  t_peakfit->Branch("dynode_b_par1err", &b_2parerr[1]);
  t_peakfit->Branch("dynode_b_par2", &b_2par[2]);
  t_peakfit->Branch("dynode_b_par2err", &b_2parerr[2]);
  t_peakfit->Branch("anode_s_par0", &b_3par[0], "gauss fit");
  t_peakfit->Branch("anode_s_par0err", &b_3parerr[0]);
  t_peakfit->Branch("anode_s_par1", &b_3par[1]);
  t_peakfit->Branch("anode_s_par1err", &b_3parerr[1]);
  t_peakfit->Branch("anode_s_par2", &b_3par[2]);
  t_peakfit->Branch("anode_s_par2err", &b_3parerr[2]);
  t_peakfit->Branch("anode_s_par3", &b_3par[3]);
  t_peakfit->Branch("anode_s_par3err", &b_3parerr[3]);
  t_peakfit->Branch("anode_s_par4", &b_3par[4]);
  t_peakfit->Branch("anode_s_par4err", &b_3parerr[4]);

  Long64_t entry_number_b;
  Long64_t total_number;
  Int_t igcell;
  Int_t fee_b;
  Int_t channel;
  Float_t X;
  Float_t Y;
  Int_t anode_b;
  Int_t dynode_b;
  Double_t npe_b;
  Long64_t time_b;
  Long64_t deltatime_b;
  Long64_t entry_number_s;
  Int_t fee_s;
  Int_t db;
  Int_t pmt;
  Int_t anode_s;
  Int_t dynode_s;
  Double_t npe_s;
  Long64_t time_s;
  Long64_t anodetime_s;
  Long64_t dynodetime_s;
  Long64_t deltatime_s;
  Long64_t deltatime_anode;
  Long64_t deltatime_dynode;
  Long64_t time_df;
  Long64_t time_df_anode;
  Long64_t time_df_dynode;

  TChain *chain; //! pointer to the analyzed TTree or TChain
  chain = new TChain("tmatch");
  chain->Add(infile.c_str());
  chain->SetBranchAddress("entry_number_b", &entry_number_b);
  chain->SetBranchAddress("total_number", &total_number);
  chain->SetBranchAddress("igcell", &igcell);
  chain->SetBranchAddress("fee_b", &fee_b);
  chain->SetBranchAddress("channel", &channel);
  chain->SetBranchAddress("X", &X);
  chain->SetBranchAddress("Y", &Y);
  chain->SetBranchAddress("anode_b", &anode_b);
  chain->SetBranchAddress("dynode_b", &dynode_b);
  chain->SetBranchAddress("npe_b", &npe_b);
  chain->SetBranchAddress("time_b", &time_b);
  chain->SetBranchAddress("deltatime_b", &deltatime_b);
  chain->SetBranchAddress("entry_number_s", &entry_number_s);
  chain->SetBranchAddress("fee_s", &fee_s);
  chain->SetBranchAddress("db", &db);
  chain->SetBranchAddress("pmt", &pmt);
  chain->SetBranchAddress("anode_s", &anode_s);
  chain->SetBranchAddress("dynode_s", &dynode_s);
  chain->SetBranchAddress("npe_s", &npe_s);
  chain->SetBranchAddress("time_s", &time_s);
  chain->SetBranchAddress("anodetime_s", &anodetime_s);
  chain->SetBranchAddress("dynodetime_s", &dynodetime_s);
  chain->SetBranchAddress("deltatime_s", &deltatime_s);
  chain->SetBranchAddress("deltatime_anode", &deltatime_anode);
  chain->SetBranchAddress("deltatime_dynode", &deltatime_dynode);
  chain->SetBranchAddress("time_df", &time_df);
  chain->SetBranchAddress("time_df_anode", &time_df_anode);
  chain->SetBranchAddress("time_df_dynode", &time_df_dynode);

  char buf1[500], buf2[500];
  Long64_t tevent;

  TH2F *hh1;
  TH1F *h1;

  TF1 *f1 = new TF1("f1", "pol 1");
  f1->FixParameter(0, 0.);

  TF1 *f2 = new TF1("f2", "gaus");

  TF1 *f3 = new TF1("f3", "landau");

  TCanvas *c1 = new TCanvas("c1", "c1title");

  for (int icell = cellstart; icell <= cellend; icell++) {

    b_igcell = icell;
    gPad->SetLogx(0);
    gPad->SetLogy(0);

    /************ dynode_b:anode_s ************************/
    sprintf(buf1, "dynode_b:anode_s>>h1_%d(200,0,200)", icell);
    sprintf(buf2, "igcell==%d&&dynode_b>%d", icell, dynode_th);

    chain->Draw(buf1, buf2, "prof");
    sprintf(buf1, "h1_%d", icell);
    hh1 = (TH2F *)gDirectory->Get(buf1);

    b_b2sevent = hh1->Integral();
    if (b_b2sevent != 0) {
      hh1->Fit("f1", "QB", "", 0, anode_th1);
      f1->GetParameters(&b_1par[0]);
      b_1parerr[0] = f1->GetParError(0);
      b_1parerr[1] = f1->GetParError(1);
    } else {
      b_1par[0] = 0.;
      b_1par[1] = 0.;
      b_1parerr[0] = 0.;
      b_1parerr[1] = 0.;
    }
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);

    t_bd2sa->Fill();
    sprintf(buf1, "%s/bd2saall_%03d.png", picdir.c_str(), icell);
    if (IfSavePNG)
      c1->SaveAs(buf1);

    /************ dynode_b ************************/
    sprintf(buf1, "dynode_b>>h1_%d(200,0,200)", icell);
    sprintf(buf2, "igcell==%d", icell);
    // c1->SetLogy(1);

    chain->Draw(buf1, buf2);
    sprintf(buf1, "h1_%d", icell);
    h1 = (TH1F *)gDirectory->Get(buf1);

    tevent = h1->Integral();
    if (tevent != 0) {
      h1->Fit("f3", "Q", "", 30, 100);
      f3->GetParameters(&b_2par[0]);
      b_2parerr[0] = f3->GetParError(0);
      b_2parerr[1] = f3->GetParError(1);
      b_2parerr[2] = f3->GetParError(2);
    } else {
      b_2par[0] = 0.;
      b_2par[1] = 0.;
      b_2par[2] = 0.;
      b_2parerr[0] = 0.;
      b_2parerr[1] = 0.;
      b_2parerr[2] = 0.;
    }
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);

    sprintf(buf1, "%s/dynode_b_%03d.png", picdir.c_str(), icell);
    if (IfSavePNG)
      c1->SaveAs(buf1);

    /************ anode_s ************************/

    Double_t tmpbincontent[71];
    Double_t maxheight = 0.;
    Double_t maxwidth = 0.;

    sprintf(buf1, "anode_s>>h1_%d(1000,6,1000)", icell);
    sprintf(buf2, "igcell==%d", icell);
    // c1->SetLogy(1);

    chain->Draw(buf1, buf2);
    sprintf(buf1, "h1_%d", icell);
    h1 = (TH1F *)gDirectory->Get(buf1);
    gPad->SetLogx();
    gPad->SetLogy();

    tevent = h1->Integral();
    if (tevent != 0) {
      int maxi = 0;
      int xpeak = 6 + h1->GetMaximumBin();

      TF1 *fit = new TF1("fit", combfunc, xpeak, 700, 5);
      TF1 *backfit = new TF1("backfit", powerfunc, xpeak, 700, 2);
      TF1 *peakfit = new TF1("peakfit", gaussfunc, xpeak, 700, 3);

      for (int i = 0; i < 71; i++) {
        tmpbincontent[i] = h1->GetBinContent(i + 30);
        h1->SetBinContent(i + 30, 0.);
      }

      backfit->SetParameter(1, -2.8);
      h1->Fit("backfit", "QMRN");
      backfit->GetParameters(b_3par);
      b_3parerr[0] = backfit->GetParError(0);
      b_3parerr[1] = backfit->GetParError(1);

      for (int i = 0; i < 71; i++) {
        h1->SetBinContent(i + 30, tmpbincontent[i]);
        Double_t iheight = tmpbincontent[i] - backfit->Eval(i + 30 + 6);
        if (iheight > maxheight) {
          maxheight = iheight;
          maxi = i;
        }
        if (iheight < maxheight && iheight / maxheight > 0.3)
          maxwidth = i - maxi;
      }

      fit->SetParameter(0, b_3par[0]);
      fit->SetParameter(1, b_3par[1]);
      fit->SetParameter(2, maxheight);
      fit->SetParameter(3, maxi + 30);
      fit->SetParameter(4, maxwidth);
      fit->SetParLimits(0, b_3par[0] - b_3parerr[0], b_3par[0] + b_3parerr[0]);
      fit->SetParLimits(1, b_3par[1] - b_3parerr[1], b_3par[1] + b_3parerr[1]);
      // fit->SetParLimits(2,20,1000);
      fit->SetParLimits(3, maxi + 30, maxi + 30 + maxwidth);
      // fit->SetParLimits(4,1,100);

      h1->SetLineColor(kBlack);
      fit->SetLineColor(kBlue);
      backfit->SetLineColor(kRed);
      peakfit->SetLineColor(kMagenta);
      h1->Fit("fit", "QMR");

      fit->GetParameters(b_3par);
      b_3parerr[0] = fit->GetParError(0);
      b_3parerr[1] = fit->GetParError(1);
      b_3parerr[2] = fit->GetParError(2);
      b_3parerr[3] = fit->GetParError(3);
      b_3parerr[4] = fit->GetParError(4);

      backfit->SetParameters(b_3par);
      peakfit->SetParameters(&b_3par[2]);
      backfit->Draw("same");
      peakfit->Draw("same");
      gStyle->SetOptStat(0);
      gStyle->SetOptFit(1);
    } else {
      b_3par[0] = 0.;
      b_3par[1] = 0.;
      b_3par[2] = 0.;
      b_3par[3] = 0.;
      b_3par[4] = 0.;
      b_3parerr[0] = 0.;
      b_3parerr[1] = 0.;
      b_3parerr[2] = 0.;
      b_3parerr[3] = 0.;
      b_3parerr[4] = 0.;
    }

    t_peakfit->Fill();
    sprintf(buf1, "%s/anode_s_%03d.png", picdir.c_str(), icell);
    if (IfSavePNG)
      c1->SaveAs(buf1);
    sprintf(buf1, "h1_%d", icell);
    delete (TH1F *)gDirectory->Get(buf1);
  }
  f_anal->Write();
  f_anal->Close();
  return 0;
}

Double_t powerfunc(Double_t *x, Double_t *par) {
  return par[0] * TMath::Power(x[0], par[1]);
}
Double_t gaussfunc(Double_t *x, Double_t *par) {
  Double_t arg = 0;
  if (par[2] != 0)
    arg = (x[0] - par[1]) / par[2];
  Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
  return fitval;
}
Double_t combfunc(Double_t *x, Double_t *par) {
  return powerfunc(x, par) + gaussfunc(x, &par[2]);
}
