#ifndef IsolationHistograms_h
#define IsolationHistograms_h


#include "PFAnalyses/CommonTools/interface/AnalysisHistograms.h"


class IsolationHistograms: public AnalysisHistograms {
 public:

  IsolationHistograms(TFileDirectory *myDir, const std::string & name="");

  virtual ~IsolationHistograms();

  void fill( double iso, double isoRel, double eventWeight);
 
 private:

  virtual void defineHistograms();


};

#endif
