void anal_rootgeneratorall() {
  int date[] = {301, 302, 303, 304, 306, 307, 308, 309, 310, 311, 312, 313, 314,
                315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327,
                328, 329, 330, 331, 401, 402, 403, 404, 405, 406, 407, 408, 409,
                410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422,
                423, 424, 425, 426, 427, 428, 429, 430, 501, 502};
  const int datelength = sizeof(date) / sizeof(int);
  //   int x1[901];
  //   for (int i = 0; i<901; i++) {
  //       x1[i]=i;
  //   }
  //   const int xlength = sizeof(x1)/sizeof(int);
  //   int y1[3000];
  //   for (int i=0; i<3000;i++) {
  //       y1[i]=i+1;
  //   }
  //   const int ylength = sizeof(y1)/sizeof(int);
  TFile *f = new TFile("anal.root", "RECREATE");
  TTree *T = new TTree("anal", "data from file");
  T->ReadFile(
      "/home/changxc/mywork/code/repo/scda/output/auto/allcombtest.txt",
      "date:igcell:nevent0:duration:rate:nevent1:time_df_p1:time_df_p1err:time_"
      "df_p2:time_df_p2err:time_df_p3:time_df_p3err:time_df_anode_p1:"
      "time_df_anode_p1err:time_df_anode_p2:time_df_anode_p2err:time_df_anode_"
      "p3:time_df_anode_p3err:bd2sa_p1:bd2sa_p1err");
  T->Write();

  TH2F *hh1[datelength];
  TH2F *ha1[900];
  TH2F *hh2[datelength];
  TH2F *hh3[datelength];
  char buf1[500];
  char buf2[500];
  char buf3[500];
  int color[10] = {1, 2, 3, 4, 5, 6, 7, 14, 28, 50};

  TCanvas *c1 = new TCanvas("c1", "c1title", 1500, 700);
  //  gStyle->SetPalette(100);

  for (int day = 0; day < datelength; day++) {
    sprintf(buf1, "time_df_anode_p2:igcell>>hh1_%d(4500,0,900,1500,0,3000)",
            date[day]);
    sprintf(buf2, "date==%d", date[day]);
    T->Draw(buf1, buf2);
    sprintf(buf1, "hh1_%d", date[day]);
    hh1[day] = (TH2F *)gDirectory->Get(buf1);
    hh1[day]->SetMarkerStyle(8);
    hh1[day]->SetMarkerSize(0.5);
    hh1[day]->SetMarkerColor(day);
  }
  hh1[0]->Draw();
  hh1[0]->SetStats(0);
  hh1[0]->SetTitle("time_df_anode_p2:igcell");
  TLegend *leg1 = new TLegend(.15, 0.15, 0.85, 0.35, "date");
  sprintf(buf1, "0%d", date[0]);
  leg1->AddEntry(hh1[0], buf1, "p");
  for (int day = 1; day < datelength; day++) {
    hh1[day]->Draw("same");
    sprintf(buf1, "0%d", date[day]);
    leg1->AddEntry(hh1[day], buf1, "p");
  }
  leg1->SetNColumns(15);
  leg1->Draw();


  for (int icell = 0; icell < 900; icell++) {
    sprintf(buf1, "time_df_anode_p2:date>>ha1_%d(4500,0,900,1500,0,3000)",
            icell);
    sprintf(buf2, "igcell==%d", icell);
    T->Draw(buf1, buf2);
    sprintf(buf1, "ha1_%d", icell);
    ha1[icell] = (TH2F *)gDirectory->Get(buf1);
    ha1[icell]->SetMarkerStyle(8);
    ha1[icell]->SetMarkerSize(0.5);
    ha1[icell]->SetMarkerColor(icell);
  }
  ha1[0]->Draw();
  ha1[0]->SetStats(0);
  ha1[0]->SetTitle("time_df_anode_p2:date");
  TLegend *leg2 = new TLegend(.15, 0.15, 0.85, 0.35, "date");
  sprintf(buf1, "0%d", date[0]);
  leg2->AddEntry(ha1[0], buf1, "p");
  for (int icell = 1; icell < 900; icell++) {
    ha1[icell]->Draw("same");
    sprintf(buf1, "0%d", icell);
    leg2->AddEntry(ha1[icell], buf1, "p");
  }
  leg2->SetNColumns(15);
  leg2->Draw();

  /*   TCanvas *c2 = new TCanvas("c2", "c2title", 1500, 700);
    for (int date = 20; date <= 29; date++) {
      sprintf(buf1, "rate:igcell>>hh2_%d(4500,0,900,1000,0,20)", date);
      sprintf(buf2, "date==%d", date);
      T->Draw(buf1, buf2);
      sprintf(buf1, "hh2_%d", date);
      hh2[date] = (TH2F *)gDirectory->Get(buf1);
      hh2[date]->SetMarkerStyle(8);
      hh2[date]->SetMarkerSize(0.5);
      hh2[date]->SetMarkerColor(color[date - 20]);
    }
    hh2[20]->Draw();
    hh2[20]->SetStats(0);
    hh2[20]->SetTitle("rate:igcell");
    TLegend *leg2 = new TLegend(.85, .65, .95, .95);
    sprintf(buf1, "0320");
    leg2->AddEntry(hh2[20], buf1, "p");
    for (int date = 21; date <= 29; date++) {
      hh2[date]->Draw("same");
      sprintf(buf1, "03%d", date);
      leg2->AddEntry(hh2[date], buf1, "p");
    }
    leg2->Draw();

    TCanvas *c3 = new TCanvas("c3", "c3title", 1500, 700);
    for (int date = 20; date <= 29; date++) {
      sprintf(buf1, "bd2sa_p1:igcell>>hh3_%d(4500,0,900,1000,0,200)", date);
      sprintf(buf2, "date==%d", date);
      T->Draw(buf1, buf2);
      sprintf(buf1, "hh3_%d", date);
      hh3[date] = (TH2F *)gDirectory->Get(buf1);
      hh3[date]->SetMarkerStyle(8);
      hh3[date]->SetMarkerSize(0.5);
      hh3[date]->SetMarkerColor(color[date - 20]);
    }
    hh3[20]->Draw();
    hh3[20]->SetStats(0);
    hh3[20]->SetTitle("dynode_b/anode_s:igcell");
    TLegend *leg3 = new TLegend(.85, .65, .95, .95);
    sprintf(buf1, "0320");
    leg3->AddEntry(hh3[20], buf1, "p");
    for (int date = 21; date <= 29; date++) {
      hh3[date]->Draw("same");
      sprintf(buf1, "03%d", date);
      leg3->AddEntry(hh3[date], buf1, "p");
    }
    leg3->Draw();
   */
  for (int day = 0; day < datelength; day++) {
    hh1[day]->Write();
    //   hh2[day]->Write();
    //   hh3[day]->Write();
  }
}
