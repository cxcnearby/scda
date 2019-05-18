void anal_rootgenerator() {
  TFile *f = new TFile("anal.root", "RECREATE");
  TTree *T = new TTree("anal", "data from file");
  T->ReadFile(
      "/home/changxc/mywork/code/repo/scda/output/auto/combtest.txt",
      "date:igcell:nevent0:duration:rate:nevent1:time_df_p1:time_df_p1err:time_"
      "df_p2:time_df_p2err:time_df_p3:time_df_p3err:time_df_anode_p1:"
      "time_df_anode_p1err:time_df_anode_p2:time_df_anode_p2err:time_df_anode_"
      "p3:time_df_anode_p3err:bd2sa_p1:bd2sa_p1err");
  T->Write();

  TH2F *hh1[50];
  TH2F *hh2[50];
  TH2F *hh3[50];
  char buf1[500];
  char buf2[500];
  char buf3[500];
  int color[10] = {1, 2, 3, 4, 5, 6, 7, 14, 28, 50};

  TCanvas *c1 = new TCanvas("c1", "c1title", 1500, 700);

  for (int date = 20; date <= 29; date++) {
    sprintf(buf1, "time_df_anode_p2:igcell>>hh1_%d(4500,0,900,1500,0,3000)",
            date);
    sprintf(buf2, "date==%d", date);
    T->Draw(buf1, buf2);
    sprintf(buf1, "hh1_%d", date);
    hh1[date] = (TH2F *)gDirectory->Get(buf1);
    hh1[date]->SetMarkerStyle(8);
    hh1[date]->SetMarkerSize(0.5);
    hh1[date]->SetMarkerColor(color[date - 20]);
  }
  hh1[20]->Draw();
  hh1[20]->SetStats(0);
  hh1[20]->SetTitle("time_df_anode_p2:igcell");
  TLegend *leg1 = new TLegend(.8, .15, .9, .45);
  sprintf(buf1, "0320");
  leg1->AddEntry(hh1[20], buf1, "p");
  for (int date = 21; date <= 29; date++) {
    hh1[date]->Draw("same");
    sprintf(buf1, "03%d", date);
    leg1->AddEntry(hh1[date], buf1, "p");
  }
  leg1->Draw();

  TCanvas *c2 = new TCanvas("c2", "c2title", 1500, 700);
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

  for (int date = 21; date <= 29; date++) {
    hh1[date]->Write();
    hh2[date]->Write();
    hh3[date]->Write();
  }
}
