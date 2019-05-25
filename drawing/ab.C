void ab(){
 TFile *f=new TFile("qiao1.root","RECREATE");
 TTree *T=new TTree("nt","data from file");
 float nlines=T->ReadFile("qiao1.txt","A1");
 printf("found %f points\n",nlines);
 T->Write();
}
