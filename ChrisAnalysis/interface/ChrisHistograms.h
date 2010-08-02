
#ifndef ChrisHistograms_h
#define ChrisHistograms_h

// Base class for histogram managing.
//
// Original Author:  Artur Kalinowski
//         Created:  Wed Jul 22 12:56:54 CEST 2009
// $Id: ChrisHistograms.h,v 1.1 2010/04/20 10:32:40 cbern Exp $
//
//
#include "PFAnalyses/CommonTools/interface/AnalysisHistograms.h"

class ChrisHistograms: public AnalysisHistograms {
 public:

  ChrisHistograms(TFileDirectory *myDir, const std::string & name="");

  virtual ~ChrisHistograms();

  void fillHistograms(float et);

 private:

  virtual void defineHistograms();


};

#endif
