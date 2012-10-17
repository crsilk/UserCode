// -*- C++ -*-
//
// Package:    IsolatedElectronSelectors
// Class:      IsolatedElectronSelectors
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


//includes needed for this file
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
using namespace edm;
using namespace pat;
using namespace std;

class IsolatedElectronSelector : public edm::EDProducer {
   public:
      explicit IsolatedElectronSelector(const edm::ParameterSet&);
      ~IsolatedElectronSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);


      InputTag electronSrc_;
      InputTag conversionSrc_;
      InputTag vertexSrc_;
      InputTag electronRhoSrc_;
      InputTag electronIsoValChargedSrc_;
      InputTag electronIsoValGammaSrc_;
      InputTag electronIsoValNeutralSrc_;





};


typedef reco::Candidate::LorentzVector LorentzVector;

IsolatedElectronSelector::IsolatedElectronSelector(const edm::ParameterSet& iConfig)
{


   electronSrc_ = iConfig.getParameter<InputTag>("electronSrc");
   conversionSrc_ = iConfig.getParameter<InputTag>("conversionSrc");
   vertexSrc_ = iConfig.getParameter<InputTag>("vertexSrc");
   electronRhoSrc_ = iConfig.getParameter<InputTag>("electronRhoSrc");
   electronIsoValChargedSrc_ = iConfig.getParameter<InputTag>("electronIsoValChargedSrc");
   electronIsoValGammaSrc_ = iConfig.getParameter<InputTag>("electronIsoValGammaSrc");
   electronIsoValNeutralSrc_ = iConfig.getParameter<InputTag>("electronIsoValNeutralSrc");


/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
 
   //if you want to put into the Run
   produces<ExampleData2,InRun>();
*/
   //now do what ever other initialization is needed
  
}


IsolatedElectronSelector::~IsolatedElectronSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
IsolatedElectronSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{



  Handle<pat::ElectronCollection> ElectronsHandle;
  iEvent.getByLabel(electronSrc_ ,ElectronsHandle); 

  Handle<reco::ConversionCollection> conversions_h;
  iEvent.getByLabel(conversionSrc_, conversions_h);
 
  Handle<reco::VertexCollection> vtxs_h;
  iEvent.getByLabel(vertexSrc_,vtxs_h); 

  Handle< edm::ValueMap<double> > electronIsoValPFIdCharged;  
  iEvent.getByLabel(electronIsoValChargedSrc_, electronIsoValPFIdCharged);

  Handle< edm::ValueMap<double> > electronIsoValPFIdGamma;
  iEvent.getByLabel(electronIsoValGammaSrc_, electronIsoValPFIdGamma);

  Handle< edm::ValueMap<double> > electronIsoValPFIdNeutral;
  iEvent.getByLabel(electronIsoValNeutralSrc_, electronIsoValPFIdNeutral);
  Handle<double> eleRho;
  iEvent.getByLabel(electronRhoSrc_,eleRho);
 

  vector <ElectronRef> isolatedElectrons;

  for(size_t i = 0; i < ElectronsHandle->size(); ++i)
    {
      ElectronRef ele (ElectronsHandle, i);  

      if(ele->pt()<30 || abs(ele->eta())>2.5 || !(ele->isEB() || ele->isEE()))
         continue;

	  double Aeff = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, fabs(ele->superCluster()->eta()) , ElectronEffectiveArea::kEleEAFall11MC);
	  edm::Ptr< reco::GsfElectron > gsfel = (edm::Ptr< reco::GsfElectron >) ele->originalObjectRef();
	  
      double charged =  (*electronIsoValPFIdCharged)[gsfel];
	  double photon = (*electronIsoValPFIdGamma)[gsfel];
	  double neutral = (*electronIsoValPFIdNeutral)[gsfel];
	  //-------------------------------------------------------------

	  if(charged + max (0., photon + neutral - (*eleRho)*Aeff) == 0 ) continue;
	  
      if(abs(ele->superCluster()->eta())>1.4442 && 
         abs(ele->superCluster()->eta())<1.566) continue;
      
      if((ele->isEB() && ele->hadronicOverEm()>0.12) && 
         (ele->isEE() && ele->hadronicOverEm()>0.10) ) continue;
      
      if ((ele->isEB() && fabs(ele->deltaPhiSuperClusterTrackAtVtx())>0.06) && 
          (ele->isEE() && fabs(ele->deltaPhiSuperClusterTrackAtVtx())>0.03)) 
         continue;
      
      if((ele->isEB() && abs(ele->deltaEtaSuperClusterTrackAtVtx())>0.004) && 
         (ele->isEE() && abs(ele->deltaEtaSuperClusterTrackAtVtx())>0.007) ) 
         continue;
      
      if((ele->isEB() && ele->scSigmaIEtaIEta()>0.01) && 
         (ele->isEE() && ele->scSigmaIEtaIEta()>0.03) ) continue;
      
      if (ele->gsfTrack()->trackerExpectedHitsInner().numberOfHits() > 1) 
         continue;

	     
	    
	      isolatedElectrons.push_back(ele);
	    
	}
}













/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/
 


// ------------ method called once each job just before starting event loop  ------------
void 
IsolatedElectronSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
IsolatedElectronSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
IsolatedElectronSelector::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
IsolatedElectronSelector::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
IsolatedElectronSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
IsolatedElectronSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
IsolatedElectronSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(IsolatedElectronSelector);
