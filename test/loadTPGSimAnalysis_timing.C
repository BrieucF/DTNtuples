#include "TROOT.h"

void loadTPGSimAnalysis_timing()
{
  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
//   gROOT->ProcessLine(".L DTNtupleTPGSimAnalyzer_Resolution.C++");
  gROOT->ProcessLine(".L DTNtupleTPGSimAnalyzer_timing.C++");
}
