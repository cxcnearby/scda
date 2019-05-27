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
  TFile *f = new TFile("analall.root", "RECREATE");
  TTree *T = new TTree("anal", "data from file");
  T->ReadFile(
      "/home/changxc/mywork/code/repo/scda/output/auto/allcombtest.txt",
      "date:igcell:nevent0:duration:rate:nevent1:time_df_p1:time_df_p1err:time_"
      "df_p2:time_df_p2err:time_df_p3:time_df_p3err:time_df_anode_p1:"
      "time_df_anode_p1err:time_df_anode_p2:time_df_anode_p2err:time_df_anode_"
      "p3:time_df_anode_p3err:bd2sa_p1:bd2sa_p1err");
  T->Write();

  TH2F *hh1[datelength];
  TH2F *ha1;
  TH2F *hh2[datelength];
  TH2F *ha2;
  TH2F *hh3[datelength];
  TH2F *ha3;

  char buf1[500];
  char buf2[500];
  char buf3[500];
  int color[10] = {1, 2, 3, 4, 5, 6, 7, 14, 28, 50};

  TCanvas *c1 = new TCanvas("c1", "c1title", 1500, 700);
  gStyle->SetPalette(1);

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
  TLegend *leg1 = new TLegend(.2, 0.15, 0.8, 0.35, "date");
  sprintf(buf1, "0%d", date[0]);
  leg1->AddEntry(hh1[0], buf1, "p");
  for (int day = 1; day < datelength; day++) {
    hh1[day]->Draw("same");
    sprintf(buf1, "0%d", date[day]);
    leg1->AddEntry(hh1[day], buf1, "p");
  }
  leg1->SetNColumns(15);
  leg1->Draw();
  sprintf(buf1, "pictest/time_df_anode_p2:igcell.png");
  c1->SaveAs(buf1);


  TCanvas *c1a = new TCanvas("c1a", "c1atitle", 1500, 700);

  for (int icell = 0; icell < 900; icell++) {
    sprintf(buf1, "time_df_anode_p2:date>>ha1(201,301,502,1500,0,3000)");
    sprintf(buf2, "igcell==%d", icell);
    T->Draw(buf1, buf2);
    sprintf(buf1, "ha1");
    ha1 = (TH2F *)gDirectory->Get(buf1);
    ha1->SetMarkerStyle(4);
    ha1->SetMarkerSize(1);
    ha1->SetMarkerColor(2);
    ha1->Draw();
    ha1->SetStats(0);
    sprintf(buf1, "time_df_anode_p2:date_%03d", icell);
    ha1->SetTitle(buf1);
    sprintf(buf1, "pictest/time_df_anode/time_df_anode_p2:date_%03d.png", icell);
    c1a->SaveAs(buf1);
  }

  TCanvas *c2 = new TCanvas("c2", "c2title", 1500, 700);
  for (int day = 0; day < datelength; day++) {
    sprintf(buf1, "rate:igcell>>hh2_%d(4500,0,900,1000,0,20)",  date[day]);
    sprintf(buf2, "date==%d",  date[day]);
    T->Draw(buf1, buf2);
    sprintf(buf1, "hh2_%d",  date[day]);
    hh2[day] = (TH2F *)gDirectory->Get(buf1);
    hh2[day]->SetMarkerStyle(8);
    hh2[day]->SetMarkerSize(0.5);
    hh2[day]->SetMarkerColor(day);
  }
  hh2[0]->Draw();
  hh2[0]->SetStats(0);
  hh2[0]->SetTitle("rate:igcell");
  TLegend *leg2 = new TLegend(0.3, 0.7, 0.9, 0.9, "date");
  sprintf(buf1, "0%d", date[0]);
  leg2->AddEntry(hh2[0], buf1, "p");
  for (int day = 1; day < datelength; day++) {
    hh2[day]->Draw("same");
    sprintf(buf1, "0%d", date[day]);
    leg2->AddEntry(hh2[day], buf1, "p");
  }
  leg2->SetNColumns(15);
  leg2->Draw();
  sprintf(buf1, "pictest/rate:igcell.png");
  c2->SaveAs(buf1);


  TCanvas *c2a = new TCanvas("c2a", "c2atitle", 1500, 700);
  for (int icell = 0; icell < 900; icell++) {
    sprintf(buf1, "rate:date>>ha2(201,301,502,1000,0,20)");
    sprintf(buf2, "igcell==%d", icell);
    T->Draw(buf1, buf2);
    sprintf(buf1, "ha2");
    ha2 = (TH2F *)gDirectory->Get(buf1);
    ha2->SetMarkerStyle(4);
    ha2->SetMarkerSize(1);
    ha2->SetMarkerColor(2);
    ha2->Draw();
    ha2->SetStats(0);
    sprintf(buf1, "rate:date_%03d", icell);
    ha2->SetTitle(buf1);
    sprintf(buf1, "pictest/rate/rate:date_%03d.png", icell);
    c2a->SaveAs(buf1);
  }


  TCanvas *c3 = new TCanvas("c3", "c3title", 1500, 700);
  for (int day = 0; day < datelength; day++) {
    sprintf(buf1, "bd2sa_p1:igcell>>hh3_%d(4500,0,900,1000,0,200)", date[day]);
    sprintf(buf2, "date==%d", date[day]);
    T->Draw(buf1, buf2);
    sprintf(buf1, "hh3_%d", date[day]);
    hh3[day] = (TH2F *)gDirectory->Get(buf1);
    hh3[day]->SetMarkerStyle(8);
    hh3[day]->SetMarkerSize(0.5);
    hh3[day]->SetMarkerColor(day);
  }
  hh3[0]->Draw();
  hh3[0]->SetStats(0);
  hh3[0]->SetTitle("dynode_b/anode_s:igcell");
  TLegend *leg3 = new TLegend(0.3, 0.7, 0.9, 0.9, "date");
  sprintf(buf1, "0%d", date[0]);
  leg3->AddEntry(hh3[0], buf1, "p");
  for (int day = 1; day < datelength; day++) {
    hh3[day]->Draw("same");
    sprintf(buf1, "0%d", date[day]);
    leg3->AddEntry(hh3[day], buf1, "p");
  }
  leg3->SetNColumns(15);
  leg3->Draw();
  sprintf(buf1, "pictest/dynode_b2anode_s:igcell.png");
  c3->SaveAs(buf1);


  TCanvas *c3a = new TCanvas("c3a", "c3atitle", 1500, 700);
  for (int icell = 0; icell < 900; icell++) {
    sprintf(buf1, "bd2sa_p1:date>>ha3(201,301,502,1000,0,200)");
    sprintf(buf2, "igcell==%d", icell);
    T->Draw(buf1, buf2);
    sprintf(buf1, "ha3");
    ha3 = (TH2F *)gDirectory->Get(buf1);
    ha3->SetMarkerStyle(4);
    ha3->SetMarkerSize(1);
    ha3->SetMarkerColor(2);
    ha3->Draw();
    ha3->SetStats(0);
    sprintf(buf1, "dynode_b/anode_s:date_%03d", icell);
    ha3->SetTitle(buf1);
    sprintf(buf1, "pictest/db2as/dynode_b2anode_s:date_%03d.png", icell);
    c3a->SaveAs(buf1);
  }
  f->Close();
}
