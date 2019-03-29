#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
void fittimedf() {
  TFile *f = new TFile("comb.root");
  TH1F *h = new TH1F("h","npe_b",300,1,301);
  Long64_t n = 
}