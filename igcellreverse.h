void smigcellreverse(int igcell, int *fee, int *db, int *pmt, float *x, float *y) {
  double smpmtx_jd[26][4][17];
  double smpmty_jd[26][4][17];
  int smpmtig_jd[26][4][17];

  for (int ifee = 1; ifee <= 25; ifee++) {
    for (int idb = 1; idb <= 3; idb++) {
      for (int ipmt = 1; ipmt <= 16; ipmt++) {
        if (igcell == smpmtig_jd[ifee][idb][ipmt]) {
            *fee = ifee;
            *db = idb;
            *pmt = ipmt;
            *x = smpmtx_jd[ifee][idb][ipmt];
            *y = smpmty_jd[ifee][idb][ipmt];
        }
      }
    }
  }
}

void bigigcellreverse(int igcell, int *fee, int *pmt, float *x, float *y) {
  double bigpmtx[101][10];
  double bigpmty[101][10];
  int bigpmtig[101][10];

  for (int ifee = 1; ifee <= 100; ifee++) {
    for (int ipmt = 1; ipmt <= 9; ipmt++) {
      if (igcell == bigpmtig[ifee][ipmt]) {
          *fee = ifee;
          *pmt = ipmt;
          *x = bigpmtx[ifee][ipmt];
          *y = bigpmty[ifee][ipmt];
      }
    }
  }
}