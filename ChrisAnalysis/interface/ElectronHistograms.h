
#ifndef ElectronHistograms_h
#define ElectronHistograms_h

// Base class for histogram managing.
//
// Original Author:  Artur Kalinowski
//         Created:  Wed Jul 22 12:56:54 CEST 2009
// $Id: ElectronHistograms.h,v 1.1 2010/05/06 07:36:38 bianchi Exp $
//
//
#include "PFAnalyses/CommonTools/interface/AnalysisHistograms.h"
#include "DataFormats/PatCandidates/interface/Electron.h"


class ElectronHistograms: public AnalysisHistograms {
 public:

  ElectronHistograms(TFileDirectory *myDir, const std::string & name="");

  virtual ~ElectronHistograms();

  /// histograms related to the GSF electron only
  void fill(const pat::Electron& ele, double eventWeight);

  /// histograms connected to a primary vertex
  void fillWithVertex(const pat::Electron& ele, const math::XYZPoint & vtx,double eventWeight);

 private:

  virtual void defineHistograms();


};

#endif
