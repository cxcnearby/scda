{
  TChain *fChain; //! pointer to the analyzed TTree or TChain
  fChain = new TChain("tmatch");
  fChain->Add("mcomb07.root");
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
  TBranch *b_b_entry_big;        //!
  TBranch *b_b_tot;              //!
  TBranch *b_b_igcell;           //!
  TBranch *b_b_fee_big;          //!
  TBranch *b_b_ch;               //!
  TBranch *b_b_x;                //!
  TBranch *b_b_y;                //!
  TBranch *b_b_anode_big;        //!
  TBranch *b_b_dynode_big;       //!
  TBranch *b_b_npe_big;          //!
  TBranch *b_b_time_big;         //!
  TBranch *b_b_deltatime_big;    //!
  TBranch *b_b_entry_small;      //!
  TBranch *b_b_fee_small;        //!
  TBranch *b_b_db;               //!
  TBranch *b_b_pmt;              //!
  TBranch *b_b_anode_small;      //!
  TBranch *b_b_dynode_small;     //!
  TBranch *b_b_npe_small;        //!
  TBranch *b_b_time_small;       //!
  TBranch *b_b_time_anode;       //!
  TBranch *b_b_time_dynode;      //!
  TBranch *b_b_deltatime_small;  //!
  TBranch *b_b_deltatime_anode;  //!
  TBranch *b_b_deltatime_dynode; //!
  /* TBranch        *b_b_time_s - b_time_b ;   //!
     TBranch        *b_b_time_anode - b_time_b ;   //!
     TBranch        *b_b_time_dynode - b_time_b ;   //!
  */

  fChain->SetBranchAddress("entry_number_b", &entry_number_b, &b_b_entry_big);
  fChain->SetBranchAddress("total_number", &total_number, &b_b_tot);
  fChain->SetBranchAddress("igcell", &igcell, &b_b_igcell);
  fChain->SetBranchAddress("fee_b", &fee_b, &b_b_fee_big);
  fChain->SetBranchAddress("channel", &channel, &b_b_ch);
  fChain->SetBranchAddress("X", &X, &b_b_x);
  fChain->SetBranchAddress("Y", &Y, &b_b_y);
  fChain->SetBranchAddress("anode_b", &anode_b, &b_b_anode_big);
  fChain->SetBranchAddress("dynode_b", &dynode_b, &b_b_dynode_big);
  fChain->SetBranchAddress("npe_b", &npe_b, &b_b_npe_big);
  fChain->SetBranchAddress("time_b", &time_b, &b_b_time_big);
  fChain->SetBranchAddress("deltatime_b", &deltatime_b, &b_b_deltatime_big);
  fChain->SetBranchAddress("entry_number_s", &entry_number_s, &b_b_entry_small);
  fChain->SetBranchAddress("fee_s", &fee_s, &b_b_fee_small);
  fChain->SetBranchAddress("db", &db, &b_b_db);
  fChain->SetBranchAddress("pmt", &pmt, &b_b_pmt);
  fChain->SetBranchAddress("anode_s", &anode_s, &b_b_anode_small);
  fChain->SetBranchAddress("dynode_s", &dynode_s, &b_b_dynode_small);
  fChain->SetBranchAddress("npe_s", &npe_s, &b_b_npe_small);
  fChain->SetBranchAddress("time_s", &time_s, &b_b_time_small);
  fChain->SetBranchAddress("anodetime_s", &anodetime_s, &b_b_time_anode);
  fChain->SetBranchAddress("dynodetime_s", &dynodetime_s, &b_b_time_dynode);
  fChain->SetBranchAddress("deltatime_s", &deltatime_s, &b_b_deltatime_small);
  fChain->SetBranchAddress("deltatime_anode", &deltatime_anode,
                           &b_b_deltatime_anode);
  fChain->SetBranchAddress("deltatime_dynode", &deltatime_dynode,
                           &b_b_deltatime_dynode);

  fChain->SetBranchAddress("time_df", &time_df);
  fChain->SetBranchAddress("time_df_anode", &time_df_anode);
  fChain->SetBranchAddress("time_df_dynode", &time_df_dynode);

  char buf1[500];
  char buf2[500];
  char buf3[500];

  TH1F *hh1[900];
  TH1F *hh2[900];
  TH1F *hh3[900];
  TH1F *hh4[900];
  int cellend = 2;

  TF1 *f1 = new TF1("f1", "gaus");
  f1->SetLineColor(kRed);

  double rms1[900];
  double rms2[900];
  double rms3[900];
  double rms4[900];
  double mean1[900];
  double mean2[900];
  double mean3[900];
  double mean4[900];

  TCanvas *c1 = new TCanvas("c1", "c1", 1500, 700);
  c1->Divide(2, 2);
  for (int icell = 0; icell < cellend; icell++) {
    c1->Update();

    c1->cd(1);
    sprintf(buf1, "time_df_anode>>h1_%d", icell);

    sprintf(buf2, "igcell==%d&&anode_s>0&&dynode_b>100", icell);
    fChain->Draw(buf1, buf2);
    sprintf(buf1, "h1_%d", icell);

    hh1[icell] = (TH1F *)gDirectory->Get(buf1);
    hh1[icell]->GetXaxis()->SetTitle("time_sm-time_big (ns) ");

    mean1[icell] = hh1[icell]->GetMean();
    rms1[icell] = hh1[icell]->GetRMS();
    hh1[icell]->Fit("f1", "", "", mean1[icell] - 2 * rms1[icell],
                    mean1[icell] + 2 * rms1[icell]);

    sprintf(buf1, "pic/cell_%03d.pdf", icell);
    c1->SaveAs(buf1);
  }
}
