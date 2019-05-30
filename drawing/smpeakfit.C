Double_t powerfunc(Double_t *x, Double_t *par) {
    return par[0]*TMath::Power(x[0], par[1]);
}
Double_t gaussfunc(Double_t *x, Double_t *par) {
  Double_t arg = 0;
  if (par[2]!=0) arg = (x[0] - par[1])/par[2];
  Double_t fitval = par[0]*TMath::Exp(-0.5*arg*arg);
  return fitval;
}
Double_t combfunc(Double_t *x, Double_t *par) {
    return powerfunc(x,par)+gaussfunc(x,&par[2]);
}
void smpeakfit() {
    TFile *f = new TFile("/afs/ihep.ac.cn/users/c/changxc/eos/scdanew/output/comball.root");
    TTree *t = (TTree*)f->Get("tmatch");
    Int_t igcell, dynode_b, anode_s;
    t->SetBranchAddress("igcell", &igcell);
    t->SetBranchAddress("dynode_b", &dynode_b);
    t->SetBranchAddress("anode_s", &anode_s);

    Double_t tmpbincontent[71];
    Double_t maxheight=0.;
    Double_t maxwidth=0.;

    Double_t par[5];
    Double_t parerr[5];

    TCanvas *c1 = new TCanvas("c1", "c1title");
    t->Draw("anode_s>>h(1000,6,1006)","igcell==500");
    TH1F *h1;
    h1 = (TH1F *)gDirectory->Get("h");
    gPad->SetLogx();
    gPad->SetLogy();

    int maxi=0;
    int xpeak=6+h1->GetMaximumBin();

    TF1 *fit = new TF1("fit",combfunc,xpeak,700,5);
    TF1 *backfit = new TF1("backfit",powerfunc,xpeak,700,2);
    TF1 *peakfit = new TF1("peakfit",gaussfunc,xpeak ,700,3);

    for (int i = 0; i < 71; i++) {
        tmpbincontent[i] = h1->GetBinContent(i+30);
        h1->SetBinContent(i+30,0.);
    }

    backfit->SetParameter(1,-2.8);
    h1->Fit("backfit","MR");
    backfit->GetParameters(par);
    parerr[0] = backfit->GetParError(0);
    parerr[1] = backfit->GetParError(1);

    for (int i = 0; i < 71; i++) {
        h1->SetBinContent(i+30,tmpbincontent[i]);
        Double_t iheight = tmpbincontent[i]- backfit->Eval(i+30+6);
        if (iheight>maxheight) {
            maxheight = iheight;
            maxi = i;
            }
        if (iheight<maxheight&&iheight/maxheight>0.3) maxwidth = i - maxi;
    }


    fit->SetParameter(0,par[0]);
    fit->SetParameter(1,par[1]);
    fit->SetParameter(2,maxheight);
    fit->SetParameter(3,maxi + 30);
    fit->SetParameter(4,maxwidth);
    fit->SetParLimits(0,par[0]-parerr[0],par[0]+parerr[0]);
    fit->SetParLimits(1,par[1]-parerr[1],par[1]+parerr[1]);
    // fit->SetParLimits(2,20,1000);
    fit->SetParLimits(3,maxi + 30,maxi + 30+maxwidth);
    // fit->SetParLimits(4,1,100);
    printf("%f %f %f %d %f\n", par[0], par[1], maxheight, maxi + 30, maxwidth);

    h1->SetLineColor(kBlack);
    fit->SetLineColor(kBlue);
    backfit->SetLineColor(kRed);
    peakfit->SetLineColor(kMagenta);
    h1->Fit("fit","MR");

    fit->GetParameters(par);
    backfit->SetParameters(par);
    peakfit->SetParameters(&par[2]);
    backfit->Draw("same");
    peakfit->Draw("same");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);
}
