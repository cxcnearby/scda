{
  //  string mpath = "ConfigmPath";    // matched path
  int cellstart = 0;   // 0
  int cellend = 899;   // 899
  int dynode_th = 100; // 100
  int anode_th0 = 30;  // 30
  int anode_th1 = 80;  // 80

  string mfile = "match*.root";
  //  string mfile = mpath + "/match*.root";

  FILE *fp_rate;
  FILE *fp_timedf;
  FILE *fp_bd2sa;
  if ((fp_rate = fopen("anal_rate.txt", "w+")) == NULL ||
      (fp_timedf = fopen("anal_timedf.txt", "w+")) == NULL ||
      (fp_bd2sa = fopen("anal_bd2sa.txt", "w+")) == NULL) {
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

  TChain *chain = new TChain("tmatch");
  chain->Add(mfile.c_str());
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

  Long64_t duration = 0;
  Long64_t cnentries = chain->GetEntries();
  for (Long64_t i = 0, tmp_time_s = 0; i < cnentries; i++) {
    chain->GetEntry(i);
    if (tmp_time_s == 0)
      tmp_time_s = time_s;
    Long64_t gaptime = time_s - tmp_time_s;
    if (gaptime > 100000000000LL)
      gaptime = 0;
    duration += gaptime;
    tmp_time_s = time_s;
  }
  duration /= 1000000000LL;
  printf("duration = %lld s\n", duration);
  if (duration == 0)
    exit(0);

  char buf1[500], buf2[500];

  Long64_t ievent;
  double irate;

  Long64_t tevent0, tevent1;
  double mean0, mean1, rms0, rms1;
  double par0[3], par1[3];
  double parerr0[3], parerr1[3];

  Long64_t b2sevent;
  double par2[2];
  double parerr2[2];

  TH1F *h0[900];
  TH1F *h1[900];
  TF1 *f1 = new TF1("f1", "gaus");

  TH2F *h2[900];
  TF1 *f2 = new TF1("f2", "pol 1");
  f2->FixParameter(0, 0.);

  TCanvas *c1 = new TCanvas("c1", "c1title");

  for (int icell = cellstart; icell <= cellend; icell++) {
    /************ rate ************************/
    printf("********  igcell = %3d  ********\n", icell);
    sprintf(buf1, "");
    sprintf(buf2, "igcell==%d&&dynode_b>%d", icell, dynode_th);
    ievent = chain->Draw(buf1, buf2);
    irate = double(ievent) / double(duration);

    fprintf(fp_rate, "%3d\t%lld\t%lld\t%lf\n", icell, ievent, duration, irate);

    /************ time_df & time_df_anode ************************/
    sprintf(buf1, "time_df>>h0_%d", icell);
    sprintf(buf2, "igcell==%d&&dynode_b>%d&&anode_s>%d", icell, dynode_th,
            anode_th0);

    chain->Draw(buf1, buf2);
    sprintf(buf1, "h0_%d", icell);
    h0[icell] = (TH1F *)gDirectory->Get(buf1);

    tevent0 = h0[icell]->Integral();
    if (tevent0 != 0) {
      mean0 = h0[icell]->GetMean();
      rms0 = h0[icell]->GetRMS();
      h0[icell]->Fit("f1", "Q", "", mean0 - 2 * rms0, mean0 + 2 * rms0);
      f1->GetParameters(&par0[0]);
      parerr0[0] = f1->GetParError(0);
      parerr0[1] = f1->GetParError(1);
      parerr0[2] = f1->GetParError(2);
    } else {
      mean0 = 0.;
      rms0 = 0.;
      par0[0] = 0.;
      par0[1] = 0.;
      par0[2] = 0.;
      parerr0[0] = 0.;
      parerr0[1] = 0.;
      parerr0[2] = 0.;
    }

    sprintf(buf1, "time_df_anode>>h1_%d", icell);
    sprintf(buf2, "igcell==%d&&dynode_b>%d&&anode_s>%d", icell, dynode_th,
            anode_th0);

    chain->Draw(buf1, buf2);
    sprintf(buf1, "h1_%d", icell);
    h1[icell] = (TH1F *)gDirectory->Get(buf1);

    tevent1 = h1[icell]->Integral();
    if (tevent1 != 0) {
      mean1 = h1[icell]->GetMean();
      rms1 = h1[icell]->GetRMS();
      h1[icell]->Fit("f1", "Q", "", mean1 - 2 * rms1, mean1 + 2 * rms1);
      f1->GetParameters(&par1[0]);
      parerr1[0] = f1->GetParError(0);
      parerr1[1] = f1->GetParError(1);
      parerr1[2] = f1->GetParError(2);
    } else {
      mean1 = 0.;
      rms1 = 0.;
      par1[0] = 0.;
      par1[1] = 0.;
      par1[2] = 0.;
      parerr1[0] = 0.;
      parerr1[1] = 0.;
      parerr1[2] = 0.;
    }

    fprintf(fp_timedf,
            "%3d\t%lld\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t%lld\t%9."
            "3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\n",
            icell, tevent0, par0[0], parerr0[0], par0[1], parerr0[1], par0[2],
            parerr0[2], tevent1, par1[0], parerr1[0], par1[1], parerr1[1],
            par1[2], parerr1[2]);

    /************ dynode_b:anode_s ************************/
    sprintf(buf1, "dynode_b:anode_s>>h2_%d(200,0,200)", icell);
    sprintf(buf2, "igcell==%d&&dynode_b>%d", icell, dynode_th);

    chain->Draw(buf1, buf2, "prof");
    sprintf(buf1, "h2_%d", icell);
    h2[icell] = (TH2F *)gDirectory->Get(buf1);

    b2sevent = h2[icell]->Integral();
    if (b2sevent != 0) {
      h2[icell]->Fit("f2", "QB", "", 0, anode_th1);
      f2->GetParameters(&par2[0]);
      parerr2[0] = f2->GetParError(0);
      parerr2[1] = f2->GetParError(1);
    } else {
      par2[0] = 0.;
      par2[1] = 0.;
      parerr2[0] = 0.;
      parerr2[1] = 0.;
    }
    fprintf(fp_bd2sa, "%3d\t%9.3f\t%9.3f\t%9.3f\t%9.3f\n", icell, par2[0],
            parerr2[0], par2[1], parerr2[1]);
  }
  fclose(fp_rate);
  fclose(fp_timedf);
  fclose(fp_bd2sa);
}