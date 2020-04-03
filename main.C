#include "HGCTB18_SIM.C"
#include "TROOT.h"
#include <algorithm>


int main(int argc, const char *argv[]){
  gROOT->ProcessLine("#include <vector>");
  //gROOT->ProcessLine("#include <map>");

  string outfile = argv[2];
  float cutE = atof(argv[3]);
  long int nTotEvt = atof(argv[4]);
  long int nPrintEvt = atof(argv[5]);

  HGCTB18_SIM m(argv[1]);
  m.Loop(outfile,cutE,nTotEvt,nPrintEvt);
}
