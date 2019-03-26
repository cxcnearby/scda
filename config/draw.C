{
TChain *fChain=new TChain("tr");
fChain->Add("fout.root");
   // Declaration of leaf types
   Int_t           big_nhit;
   Int_t           big_evt;
   Int_t           ievt;
   Int_t           sm_nhit;
   Long64_t        dftime_mean;
   Double_t        sm_maxanode;
   Double_t        sm_maxdynode;
   Double_t        sm_anode[249];   //[sm_nhit]
   Double_t        sm_dynode[249];   //[sm_nhit]
   Double_t        sm_pmtx[249];   //[sm_nhit]
   Double_t        sm_pmty[249];   //[sm_nhit]
   Double_t        sm_pmtz[249];   //[sm_nhit]
   Long64_t        big_second[2507];   //[big_nhit]
   Double_t        big_ns[2507];   //[big_nhit]
   Int_t           big_anode[2507];   //[big_nhit]
   Int_t           big_dynode[2507];   //[big_nhit]
   Double_t        big_pmtx[2507];   //[big_nhit]
   Double_t        big_pmty[2507];   //[big_nhit]

   // List of branches
   TBranch        *b_big_nhit;   //!
   TBranch        *b_big_evt;   //!
   TBranch        *b_ievt;   //!
   TBranch        *b_sm_nhit;   //!
   TBranch        *b_dftime_mean;   //!
   TBranch        *b_sm_maxanode;   //!
   TBranch        *b_sm_maxdynode;   //!
   TBranch        *b_sm_anode;   //!
   TBranch        *b_sm_dynode;   //!
   TBranch        *b_sm_pmtx;   //!
   TBranch        *b_sm_pmty;   //!
   TBranch        *b_sm_pmtz;   //!
   TBranch        *b_big_second;   //!
   TBranch        *b_big_ns;   //!
   TBranch        *b_big_anode;   //!
   TBranch        *b_big_dynode;   //!
   TBranch        *b_big_pmtx;   //!
   TBranch        *b_big_pmty;   //!


   fChain->SetBranchAddress("big_nhit", &big_nhit, &b_big_nhit);
   fChain->SetBranchAddress("big_evt", &big_evt, &b_big_evt);
   fChain->SetBranchAddress("ievt", &ievt, &b_ievt);
   fChain->SetBranchAddress("sm_nhit", &sm_nhit, &b_sm_nhit);
   fChain->SetBranchAddress("dftime_mean", &dftime_mean, &b_dftime_mean);
   fChain->SetBranchAddress("sm_maxanode", &sm_maxanode, &b_sm_maxanode);
   fChain->SetBranchAddress("sm_maxdynode", &sm_maxdynode, &b_sm_maxdynode);
   fChain->SetBranchAddress("sm_anode", sm_anode, &b_sm_anode);
   fChain->SetBranchAddress("sm_dynode", sm_dynode, &b_sm_dynode);
   fChain->SetBranchAddress("sm_pmtx", sm_pmtx, &b_sm_pmtx);
   fChain->SetBranchAddress("sm_pmty", sm_pmty, &b_sm_pmty);
   fChain->SetBranchAddress("sm_pmtz", sm_pmtz, &b_sm_pmtz);
   fChain->SetBranchAddress("big_second", big_second, &b_big_second);
   fChain->SetBranchAddress("big_ns", big_ns, &b_big_ns);
   fChain->SetBranchAddress("big_anode", big_anode, &b_big_anode);
   fChain->SetBranchAddress("big_dynode", big_dynode, &b_big_dynode);
   fChain->SetBranchAddress("big_pmtx", big_pmtx, &b_big_pmtx);
   fChain->SetBranchAddress("big_pmty", big_pmty, &b_big_pmty);

char buf[500];
sprintf(buf,"bigPMT x:y:npe, big_evt=%d",big_evt);
cout<<"buf="<<buf<<endl;
TH2F *pos0=new TH2F("position_absolute",buf,30,-75,75,30,-75,75);
sprintf(buf,"smallPMT x:y:npe, big_evt=%d",big_evt);
TH2F *pos1=new TH2F("position_absolute",buf,30,-75,75,30,-75,75);
pos0->GetXaxis()->SetTitle("x (m)");
pos0->GetYaxis()->SetTitle("y (m)");
pos1->GetXaxis()->SetTitle("x (m)");
pos1->GetYaxis()->SetTitle("y (m)");

TLine line(-45,-75,-45,75);
line.SetLineStyle(3);
line.SetLineColor(1);

for(int i=0; i<fChain->GetEntries(); i++)
{

fChain->GetEntry(i);
if(sm_nhit<30) continue;

sprintf(buf,"wcdaevt%d-bigNhit%d_wcdaNPE",big_evt,big_nhit);
TH2F *pos0=new TH2F("position_absolute",buf,30,-75,75,30,-75,75);
//sprintf(buf,"smallPMT x:y:npe, big_evt=%d",big_evt);
sprintf(buf,"wcdaevt%d-smallNhit%d_wcda++NPE",big_evt,sm_nhit);
TH2F *pos1=new TH2F("position_absolute",buf,30,-75,75,30,-75,75);
pos0->GetXaxis()->SetTitle("x (m)");
pos0->GetYaxis()->SetTitle("y (m)");
pos1->GetXaxis()->SetTitle("x (m)");
pos1->GetYaxis()->SetTitle("y (m)");
pos0->SetStats(0);
pos1->SetStats(0);



for(int i=0; i<big_nhit; i++)
 {

      Double_t q0 = big_anode[i]/22.0;
      Double_t q1 = big_dynode[i]/0.726/0.69;
      Double_t q = q0;
      if (q>160&&q1>0) q = q1;
      if (q<=0) q = 1.e-20;

  pos0->Fill(big_pmty[i],big_pmtx[i],q);
 
}


for(int i=0; i<sm_nhit; i++)
 {
  pos1->Fill(sm_pmty[i],sm_pmtx[i],sm_anode[i]*2.0);
 }


TCanvas *c0=new TCanvas("c0","c0",1500,700);
c0->Divide(2,1);
c0->cd(1);
gPad->SetLogz();
 pos0->Draw("colz");
line.DrawLine(-45,-75,-45,75);
line.DrawLine(-15,-75,-15,75);
line.DrawLine(15,-75,15,75);
line.DrawLine(45,-75,45,75);

line.DrawLine(-75,-45,75,-45);
line.DrawLine(-75,-15,75,-15);
line.DrawLine(-75,15,75,15);
line.DrawLine(-75,45,75,45);


c0->cd(2); 
gPad->SetLogz();
pos1->Draw("colz");
line.DrawLine(-45,-75,-45,75);
line.DrawLine(-15,-75,-15,75);
line.DrawLine(15,-75,15,75);
line.DrawLine(45,-75,45,75);

line.DrawLine(-75,-45,75,-45);
line.DrawLine(-75,-15,75,-15);
line.DrawLine(-75,15,75,15);
line.DrawLine(-75,45,75,45);

sprintf(buf,"wcdaevt%d.pdf",big_evt);

c0->SaveAs(Form("pic/%s",buf));
}   

}
