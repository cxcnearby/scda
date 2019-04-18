{
  FILE *fp_log;
  TChain *fChain; //! pointer to the analyzed TTree or TChain
  fChain = new TChain("tmatch");
  fChain->Add("m0329.root");

  if ((fp_log = fopen("2fitpars0329_600_50_df.txt", "w+")) == NULL) {
    printf("cannot create log file\n");
    exit(0);
  }
  // Fixed size dimensions of array or collections stored in the TTree if any.

  // Declaration of leaf types
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

  // List of branches
  TBranch *b_entry_big;        //!
  TBranch *b_tot;              //!
  TBranch *b_igcell;           //!
  TBranch *b_fee_big;          //!
  TBranch *b_ch;               //!
  TBranch *b_x;                //!
  TBranch *b_y;                //!
  TBranch *b_anode_big;        //!
  TBranch *b_dynode_big;       //!
  TBranch *b_npe_big;          //!
  TBranch *b_time_big;         //!
  TBranch *b_deltatime_big;    //!
  TBranch *b_entry_small;      //!
  TBranch *b_fee_small;        //!
  TBranch *b_db;               //!
  TBranch *b_pmt;              //!
  TBranch *b_anode_small;      //!
  TBranch *b_dynode_small;     //!
  TBranch *b_npe_small;        //!
  TBranch *b_time_small;       //!
  TBranch *b_time_anode;       //!
  TBranch *b_time_dynode;      //!
  TBranch *b_deltatime_small;  //!
  TBranch *b_deltatime_anode;  //!
  TBranch *b_deltatime_dynode; //!
  TBranch *b_time_df;          //!
  TBranch *b_time_df_anode;    //!
  TBranch *b_time_df_dynode;   //!

  fChain->SetBranchAddress("entry_number_b", &entry_number_b, &b_entry_big);
  fChain->SetBranchAddress("total_number", &total_number, &b_tot);
  fChain->SetBranchAddress("igcell", &igcell, &b_igcell);
  fChain->SetBranchAddress("fee_b", &fee_b, &b_fee_big);
  fChain->SetBranchAddress("channel", &channel, &b_ch);
  fChain->SetBranchAddress("X", &X, &b_x);
  fChain->SetBranchAddress("Y", &Y, &b_y);
  fChain->SetBranchAddress("anode_b", &anode_b, &b_anode_big);
  fChain->SetBranchAddress("dynode_b", &dynode_b, &b_dynode_big);
  fChain->SetBranchAddress("npe_b", &npe_b, &b_npe_big);
  fChain->SetBranchAddress("time_b", &time_b, &b_time_big);
  fChain->SetBranchAddress("deltatime_b", &deltatime_b, &b_deltatime_big);
  fChain->SetBranchAddress("entry_number_s", &entry_number_s, &b_entry_small);
  fChain->SetBranchAddress("fee_s", &fee_s, &b_fee_small);
  fChain->SetBranchAddress("db", &db, &b_db);
  fChain->SetBranchAddress("pmt", &pmt, &b_pmt);
  fChain->SetBranchAddress("anode_s", &anode_s, &b_anode_small);
  fChain->SetBranchAddress("dynode_s", &dynode_s, &b_dynode_small);
  fChain->SetBranchAddress("npe_s", &npe_s, &b_npe_small);
  fChain->SetBranchAddress("time_s", &time_s, &b_time_small);
  fChain->SetBranchAddress("anodetime_s", &anodetime_s, &b_time_anode);
  fChain->SetBranchAddress("dynodetime_s", &dynodetime_s, &b_time_dynode);
  fChain->SetBranchAddress("deltatime_s", &deltatime_s, &b_deltatime_small);
  fChain->SetBranchAddress("deltatime_anode", &deltatime_anode,
                           &b_deltatime_anode);
  fChain->SetBranchAddress("deltatime_dynode", &deltatime_dynode,
                           &b_deltatime_dynode);

  fChain->SetBranchAddress("time_df", &time_df, &b_time_df);
  fChain->SetBranchAddress("time_df_anode", &time_df_anode, &b_time_df_anode);
  fChain->SetBranchAddress("time_df_dynode", &time_df_dynode,
                           &b_time_df_dynode);

  char buf1[500];
  char buf2[500];
  char buf3[500];

  TH1F *hh1[900];
  // TH1F *hh2[900];
  // TH1F *hh3[900];
  // TH1F *hh4[900];
  int cellstart = 0;
  int cellend = 899;

  TF1 *f1 = new TF1("f1", "gaus");
  f1->SetLineColor(kRed);

  double rms1[900];
  // double rms2[900];
  // double rms3[900];
  // double rms4[900];
  double mean1[900];
  // double mean2[900];
  // double mean3[900];
  // double mean4[900];

  double par[3];
  double parerr[3];

  TCanvas *c1 = new TCanvas("c1", "c1", 1500, 700);
  // c1->Divide(2, 2);
  for (int icell = cellstart; icell <= cellend; icell++) {
    c1->Update();

    // c1->cd(1);
    sprintf(buf1, "time_df>>h1_%d", icell);

    sprintf(buf2, "igcell==%d&&anode_s>50&&dynode_b>600", icell);
    fChain->Draw(buf1, buf2);
    sprintf(buf1, "h1_%d", icell);

    hh1[icell] = (TH1F *)gDirectory->Get(buf1);
    hh1[icell]->GetXaxis()->SetTitle("time_sm-time_big (ns) ");

    if (hh1[icell]->Integral() == 0) continue;

    mean1[icell] = hh1[icell]->GetMean();
    rms1[icell] = hh1[icell]->GetRMS();
    hh1[icell]->Fit("f1", "", "", mean1[icell] - 2 * rms1[icell],
                    mean1[icell] + 2 * rms1[icell]);
    f1->GetParameters(&par[0]);
    parerr[0] = f1->GetParError(0);
    parerr[1] = f1->GetParError(1);
    parerr[2] = f1->GetParError(2);

    fprintf(fp_log, "%3d\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\t%9.3f\n", icell,
            par[0], parerr[0], par[1], parerr[1], par[2], parerr[2]);

    sprintf(buf1, "pic2/cell0329_600_50_df_%03d.png", icell);
    c1->SaveAs(buf1);
  }
  fclose(fp_log);
}
