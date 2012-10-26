// -*- C++ -*-
//
// Package:    HEPTopTagSelectors
// Class:      HEPTopTagSelectors
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
using namespace edm;
using namespace reco;
using namespace std;

class HEPTopTagSelector : public edm::EDProducer {
   public:
      explicit HEPTopTagSelector(const edm::ParameterSet&);
      ~HEPTopTagSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      bool passDijetMassCriteria(BasicJetCollection ::const_iterator iJet, double m12MinCut, double m13MaxCut, double rMinCut, double rMaxCut);
      bool passM23Cut(BasicJetCollection ::const_iterator iJet, double m23MinCut, double m23MaxCut);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);




      InputTag HEPTopTagSrc_;
      double ptCut_;
      double etaCut_;
      double topMassMinCut_;
      double topMassMaxCut_;
      double m13MinCut_;
      double m13MaxCut_;
      double rMinCut_;
      double rMaxCut_;
      double m23MinCut_;
      double m23MaxCut_;
      
      bool antiDijetCuts_;
      bool light_;


};

struct GreaterByPtCandPtr 
{
  bool operator()( const edm::Ptr<reco::Candidate> & t1, const edm::Ptr<reco::Candidate> & t2 ) const 
  {return t1->pt() > t2->pt();}
};


typedef reco::Candidate::LorentzVector LorentzVector;

HEPTopTagSelector::HEPTopTagSelector(const edm::ParameterSet& iConfig)
{


   HEPTopTagSrc_ = iConfig.getParameter<InputTag>("HEPTopTagSrc");
 
   ptCut_ = iConfig.getParameter<double>("ptCut");
   etaCut_ = iConfig.getParameter<double>("etaCut");
   topMassMinCut_ = iConfig.getParameter<double>("topMassMinCut");
   topMassMaxCut_ = iConfig.getParameter<double>("topMassMaxCut");
   m13MinCut_ = iConfig.getParameter<double>("m13MinCut");
   m13MaxCut_ = iConfig.getParameter<double>("m13MaxCut");
   rMinCut_ = iConfig.getParameter<double>("rMinCut");
   rMaxCut_ = iConfig.getParameter<double>("rMaxCut");
   m23MinCut_ = iConfig.getParameter<double>("m23MinCut");
   m23MaxCut_ = iConfig.getParameter<double>("m23MaxCut");
   
   antiDijetCuts_ = iConfig.getParameter<bool>("antiDijetCuts");
   light_ = iConfig.getParameter<bool>("light");
   

   if(light_)
   {
      produces<vector<LorentzVector> >();
      produces<vector<LorentzVector> >("subjets").setBranchAlias("subjets");
   }
   else
   {
      produces<BasicJetCollection > ();
      produces<vector<PFJet>  > ("subjets").setBranchAlias("subjets");
   }

  
}


HEPTopTagSelector::~HEPTopTagSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HEPTopTagSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   
   Handle<BasicJetCollection > HEPTopTags;
   iEvent.getByLabel(HEPTopTagSrc_, HEPTopTags);
   BasicJetCollection ::const_iterator HEPTopTag;

   Jet::Constituents subjets;

   auto_ptr<BasicJetCollection > SelectedHEPTopTags( new BasicJetCollection () );
   auto_ptr<vector<PFJet>  >  SelectedSubjets( new vector<PFJet>   () );
   auto_ptr<vector<LorentzVector> > SelectedHEPTopTagsP4( new vector<LorentzVector>() );
   auto_ptr<vector<LorentzVector> > SelectedSubjetsP4( new vector<LorentzVector>() );

   for(HEPTopTag = HEPTopTags->begin(); HEPTopTag != HEPTopTags->end(); HEPTopTag++)
   {
      if(HEPTopTag->pt() < ptCut_) continue;

      if(abs(HEPTopTag->eta()) > etaCut_) continue;

      if(HEPTopTag->mass() < topMassMinCut_ || 
         HEPTopTag->mass() > topMassMaxCut_ ) continue;

      if(antiDijetCuts_)
      {
         if(passDijetMassCriteria(HEPTopTag,m13MinCut_, m13MaxCut_, rMinCut_, 
                                   rMaxCut_)) continue;
      }
      else
      {
         if(!passDijetMassCriteria(HEPTopTag,m13MinCut_, m13MaxCut_, 
                                   rMinCut_, rMaxCut_)) continue;
      }
      
      if(!passM23Cut(HEPTopTag, m23MinCut_, m23MaxCut_)) continue;

      SelectedHEPTopTags->push_back(*HEPTopTag);
      SelectedHEPTopTagsP4->push_back(HEPTopTag->p4());
      subjets = HEPTopTag->getJetConstituents();
      for( unsigned i = 0; i < subjets.size(); i++)
      {
         const PFJet * subjet = dynamic_cast<const PFJet*>(subjets[i].get()); 

         SelectedSubjets->push_back(*subjet);
         SelectedSubjetsP4->push_back(subjet->p4());
      }
   }

   if(light_)
   {
       iEvent.put(SelectedHEPTopTagsP4);
       iEvent.put(SelectedSubjetsP4, "subjets");  
   }
   else
   {
      iEvent.put(SelectedHEPTopTags);
      iEvent.put(SelectedSubjets, "subjets");
   }
}

// ------------ method called once each job just before starting event loop  ------------
void 
HEPTopTagSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HEPTopTagSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
HEPTopTagSelector::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HEPTopTagSelector::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HEPTopTagSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HEPTopTagSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HEPTopTagSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

bool HEPTopTagSelector::passDijetMassCriteria(BasicJetCollection ::const_iterator iJet, double m13MinCut, double m13MaxCut, double rMinCut, double rMaxCut)
{
  reco::Jet::Constituents subjets = iJet->getJetConstituents();
  bool _massCriteria=false;
  if ( subjets.size() == 3 )  // Check that I have 3 subjets (should be given but just to be sure...)
    { 
      sort(subjets.begin(), subjets.end(), GreaterByPtCandPtr()); //Sort them by pt
      reco::Candidate::LorentzVector p4_1=subjets[0]->p4();
      reco::Candidate::LorentzVector p4_2=subjets[1]->p4();
      reco::Candidate::LorentzVector p4_3=subjets[2]->p4();
      
      //m:
      double m123=(p4_1+p4_2+p4_3).mass();
      double m12=(p4_1+p4_2).mass();
      double m13=(p4_1+p4_3).mass();
      double m23=(p4_2+p4_3).mass();
      
      
      bool Cond1=(atan(m13/m12)>m13MinCut && m13MaxCut > atan(m13/m12) && 
                  (m23/m123>rMinCut && rMaxCut>m23/m123));
      bool Cond2=(((m23/m123)*(m23/m123) < 1-rMinCut*rMinCut*(1+(m13/m12)*(m13/m12))) &&
                  ((m23/m123)*(m23/m123) > 1-rMaxCut*rMaxCut*(1+(m13/m12)*(m13/m12))) );
      bool Cond3= (((m23/m123)*(m23/m123) < 1-rMinCut*rMinCut*(1+(m12/m13)*(m12/m13))) &&
                   ((m23/m123)*(m23/m123) > 1-rMaxCut*rMaxCut*(1+(m12/m13)*(m12/m13))));

      if(Cond1 || Cond2 || Cond3) _massCriteria=true;
    }
  return _massCriteria;
}
bool HEPTopTagSelector::passM23Cut(BasicJetCollection ::const_iterator iJet, double m23MinCut, double m23MaxCut)
{
   reco::Jet::Constituents subjets = iJet->getJetConstituents();
   if ( subjets.size() == 3 )  // Check that I have 3 subjets (should be given but just to be sure...)
   { 
      sort(subjets.begin(), subjets.end(), GreaterByPtCandPtr()); //Sort them by pt
      reco::Candidate::LorentzVector p4_1=subjets[0]->p4();
      reco::Candidate::LorentzVector p4_2=subjets[1]->p4();
      reco::Candidate::LorentzVector p4_3=subjets[2]->p4();
      
      //m:
      double m123=(p4_1+p4_2+p4_3).mass();
      double m23=(p4_2+p4_3).mass();
      
      if (m23/m123 > m23MinCut && m23/m123 < m23MaxCut) return true;
      else return false;

   }
   return false;
}
//define this as a plug-in
DEFINE_FWK_MODULE(HEPTopTagSelector);
