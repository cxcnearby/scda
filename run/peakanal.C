{
  int cellstart = 0;   // 0
  int cellend = 0;   // 899
  int dynode_th = 100; // 100
  int anode_th0 = 30;  // 30
  int anode_th1 = 100;  // 80

  FILE *fp_peakfit;
  FILE *fp_bd2saall;
  if ((fp_peakfit = fopen("/afs/ihep.ac.cn/users/c/changxc/eos/scdanew/output/anal_peakfit.txt", "w")) == NULL || (fp_bd2saall = fopen("/afs/ihep.ac.cn/users/c/changxc/eos/scdanew/output/anal_bd2saall.txt", "w")) == NULL) {
    printf("cannot create log files\n");
    exit(0);
  }

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
  chain->Add("/afs/ihep.ac.cn/users/c/changxc/eos/scdanew/output/comball.root");
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


  Long64_t b2sevent;
  double par1[2];
  double parerr1[2];

  Long64_t tevent;
  double par2[3];
  double parerr2[3];


  TH2F *hh1;
  TH1F *h1;

  TF1 *f1 = new TF1("f1", "pol 1");
  f1->FixParameter(0, 0.);

  TF1 *f2 = new TF1("f2", "gaus");

  TF1 *f3 = new TF1("f3", "landau");

  TCanvas *c1 = new TCanvas("c1", "c1title");

  for (int icell = cellstart; icell <= cellend; icell++) {
    /************ dynode_b:anode_s ************************/
    sprintf(buf1, "dynode_b:anode_s>>h1_%d(200,0,200)", icell);
    sprintf(buf2, "igcell==%d&&dynode_b>%d", icell, dynode_th);

    chain->Draw(buf1, buf2, "prof");
    sprintf(buf1, "h1_%d", icell);
    hh1 = (TH2F *)gDirectory->Get(buf1);

    b2sevent = hh1->Integral();
    if (b2sevent != 0) {
      hh1->Fit("f1", "QB", "", 0, anode_th1);
      f1->GetParameters(&par1[0]);
      parerr1[0] = f1->GetParError(0);
      parerr1[1] = f1->GetParError(1);
    } else {
      par1[0] = 0.;
      par1[1] = 0.;
      parerr1[0] = 0.;
      parerr1[1] = 0.;
    }
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);

    fprintf(fp_bd2saall, "%3d\t%9.3f\t%9.3f\t%9.3f\t%9.3f\n", icell, par1[0], parerr1[0], par1[1], parerr1[1]);
    sprintf(buf1, "bd2saall_%03d.png", icell);
    c1->SaveAs(buf1);

        /************ dynode_b ************************/
    sprintf(buf1, "dynode_b>>h1_%d(200,0,200)", icell);
    sprintf(buf2, "igcell==%d", icell);
    //c1->SetLogy(1);

    chain->Draw(buf1, buf2);
    sprintf(buf1, "h1_%d", icell);
    h1 = (TH1F *)gDirectory->Get(buf1);

    tevent = h1->Integral();
    if (tevent != 0) {
      h1->Fit("f3", "Q", "", 30, 100);
      f2->GetParameters(&par2[0]);
      parerr2[0] = f2->GetParError(0);
      parerr2[1] = f2->GetParError(1);
      parerr2[2] = f2->GetParError(2);
    } else {
      par2[0] = 0.;
      par2[1] = 0.;
      par2[2] = 0.;
      parerr2[0] = 0.;
      parerr2[1] = 0.;
      parerr2[2] = 0.;
    }
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);

    fprintf(fp_peakfit,
            "%3d\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t",
            icell, par2[0], parerr2[0], par2[1], parerr2[1], par2[2],
            parerr2[2]);
    sprintf(buf1, "dynode_b_%03d.png", icell);
    c1->SaveAs(buf1);


    /************ anode_s ************************/
    sprintf(buf1, "anode_s>>h1_%d(200,0,200)", icell);
    sprintf(buf2, "pow(anode_s,2.8)*(igcell==%d)", icell);
    //c1->SetLogy(1);

    chain->Draw(buf1, buf2);
    sprintf(buf1, "h1_%d", icell);
    h1 = (TH1F *)gDirectory->Get(buf1);

    tevent = h1->Integral();
    if (tevent != 0) {
      h1->Fit("f2", "Q", "", 30, 100);
      f2->GetParameters(&par2[0]);
      parerr2[0] = f2->GetParError(0);
      parerr2[1] = f2->GetParError(1);
      parerr2[2] = f2->GetParError(2);
    } else {
      par2[0] = 0.;
      par2[1] = 0.;
      par2[2] = 0.;
      parerr2[0] = 0.;
      parerr2[1] = 0.;
      parerr2[2] = 0.;
    }
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);

    fprintf(fp_peakfit,
            "%9.3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\n",
            par2[0], parerr2[0], par2[1], parerr2[1], par2[2],
            parerr2[2]);
    sprintf(buf1, "anode_s_%03d.png", icell);
    c1->SaveAs(buf1);
  }
  fclose(fp_bd2saall);
  fclose(fp_peakfit);
}