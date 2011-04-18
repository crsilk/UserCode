// -*- C++ -*-
//
// Package:    CrossSectionAnalyzer
// Class:      CrossSectionAnalyzer
// 
/**\class CrossSectionAnalyzer CrossSectionAnalyzer.cc Configuration/CrossSectionAnalyzer/src/CrossSectionAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Mon Apr 18 09:24:33 CDT 2011
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TFile.h"
#include "TH2.h"

//
// class declaration
//

using namespace edm;
using namespace std;
class CrossSectionAnalyzer : public edm::EDAnalyzer {
   public:
      explicit CrossSectionAnalyzer(const edm::ParameterSet&);
      ~CrossSectionAnalyzer();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      string outputFileName;
      TFile* outputFile;
      TH2F* xsectionVsM0AndM12;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CrossSectionAnalyzer::CrossSectionAnalyzer(const edm::ParameterSet& iConfig)

{

   outputFileName = iConfig.getUntrackedParameter<string>("outputFile");
   
}


CrossSectionAnalyzer::~CrossSectionAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
CrossSectionAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<double> m0;
   Handle<double> m12;
   Handle<double> xsection;
   double oldXsection = 0;
   

   iEvent.getByLabel("susyScanM0", m0);
   iEvent.getByLabel("susyScanM12", m12);
   iEvent.getByLabel("susyScanCrossSection", xsection);

   oldXsection = xsectionVsM0AndM12->GetBinContent((int)(*m0 + .5), (int)(*m12 + .5));
   
   cout<<oldXsection<<" "<< *xsection<<endl;
   if( (float)oldXsection != (float)*xsection)
   {
      cout<<"here"<<endl;
      xsectionVsM0AndM12->SetBinContent( (int)(*m0 + .5), (int)(*m12 + .5), *xsection);
   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
CrossSectionAnalyzer::beginJob()
{
   
   outputFile = new TFile(outputFileName.c_str(), "RECREATE");
   xsectionVsM0AndM12 = new TH2F("xsectionVsM0AndM12", 
                                 "X section vs. M0  and M12", 
                                 3000, 0.0, 3000.0, 1500, 0.0, 1500.0);
   xsectionVsM0AndM12->SetName("CrossSection");
   xsectionVsM0AndM12->SetTitle("Cross Section");

   
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CrossSectionAnalyzer::endJob() {
   xsectionVsM0AndM12->Print();
   outputFile->cd();
   xsectionVsM0AndM12->Write();
   outputFile->Close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(CrossSectionAnalyzer);
