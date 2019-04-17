{
TChain *ch10=new TChain("t98");
TChain *ch11=new TChain("t98");
TChain *ch12=new TChain("t98");
TChain *ch13=new TChain("t98");

ch10->Add("/eos/user/l/lixr/muonbackground/mergeroot/x220pm20_y250pm20_evt2w/run1-500_coreset-2_thin4_smtype12_evt.root");
ch11->Add("/eos/user/l/lixr/muonbackground/mergeroot/x220pm20t50_y250pm20t50_evt2w/run1-500_coreset-2_thin4_smtype12_evt.root");
ch12->Add("/eos/user/l/lixr/muonbackground/mergeroot/x220pm50t100_y250pm50t100_evt2w/run1-500_coreset-2_thin4_smtype12_evt.root");
ch13->Add("/eos/user/l/lixr/muonbackground/mergeroot/x220pm100t220_y250pm100t220_evt2w/run1-500_coreset-2_thin4_smtype12_evt.root");

//gPad->SetGridx();
//gPad->SetGridy();
TH1D *h10 =new TH1D("h10","h10",200,0,200);
TH1D *h11 =new TH1D("h11","h11",200,0,200);
TH1D *h12 =new TH1D("h12","h12",200,0,200);
TH1D *h13 =new TH1D("h13","h13",200,0,200);
TH1D *h20 =new TH1D("h20","h20",200,0,200);
TH1D *h21 =new TH1D("h21","h21",200,0,200);
TH1D *h22 =new TH1D("h22","h22",200,0,200);
TH1D *h23 =new TH1D("h23","h23",200,0,200);
double x,y; 


ch10->Draw("evtnpei98[73]>>h10");
ch11->Draw("evtnpei98[73]>>h11");
ch12->Draw("evtnpei98[73]>>h12");
ch13->Draw("evtnpei98[73]>>h13");


h10->Add(h11,5);
h10->Add(h12,46.875);
h10->Add(h13,63.687);

h10->Draw();


for(int i=0;i<200;i++){x=h10->GetBinCenter(i+1);y=h10->GetBinContent(i+1);y=y*pow(x,3);
         h20->SetBinContent(i+1,y);}

c1=new TCanvas("c1","c1",1000,700);
c1->Divide(2,1);
c1->cd(1); h10->Draw();
c1->cd(2); h20->Draw();

/*

TCanvas *c1=new TCanvas("c1","c1",1000,700);
c1->Divide(3,1);
c1->cd(1); // gPad->SetLogx(); 
gPad->SetLogy();  
ch10->Draw("evtnpei98[73]>>h10");

c1->cd(2); // gPad->SetLogx();  
gPad->SetLogy(); 
ch12->Draw("evtnpei98[73]>>h12");

c1->cd(3); // gPad->SetLogx(); 
gPad->SetLogy(); 
ch13->Draw("evtnpei98[73]>>h13");

for(int i=0;i<200;i++){x=h10->GetBinCenter(i+1);y=h10->GetBinContent(i+1);y=y*pow(x,2);
      h20->SetBinContent(i+1,y);}
for(int i=0;i<200;i++){x=h12->GetBinCenter(i+1);y=h12->GetBinContent(i+1);y=y*pow(x,2);
      h22->SetBinContent(i+1,y);}
for(int i=0;i<200;i++){x=h13->GetBinCenter(i+1);y=h13->GetBinContent(i+1);y=y*pow(x,2);
      h23->SetBinContent(i+1,y);}


TCanvas *c2=new TCanvas("c2","c2",1000,700);
c2->Divide(3,1);
c2->cd(1); gPad->SetLogy();  h20->Draw();
c2->cd(2); gPad->SetLogy();  h22->Draw();
c2->cd(3); gPad->SetLogy();  h23->Draw();

*/

}
