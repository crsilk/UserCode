#include <memory>
#include <string>
#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TLatex.h"
#include "TMath.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Candidate/interface/Candidate.h"

using namespace std;
using namespace edm;
using namespace reco;

class METTopAnalyzer : public edm::EDAnalyzer {
   public:
      explicit METTopAnalyzer(const edm::ParameterSet&);
      ~METTopAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      InputTag METP4Src_;

      InputTag topCandP4Src_;
      InputTag topCandMassSrc_;
      InputTag topCandMinMassSrc_;
      InputTag topCandNSubjetsSrc_;
      InputTag topCandPassTopTagSrc_;

      InputTag eventWeightSrc_;
      InputTag pdfWeightSrc_;
      bool useEDMEventWeight_;
      double eventWeight_;
      double topCandPtDef_;
      
      int nTopCandCut_;
      vector<double> pt_MinCuts_;
      vector<double> pt_MaxCuts_;
      vector<double> eta_MinCuts_;
      vector<double> eta_MaxCuts_;
      vector<double> deltaPhi_MinCuts_;
      vector<double> deltaPhi_MaxCuts_;
      vector<double> topMass_MinCuts_;
      vector<double> topMass_MaxCuts_;
      vector<int> nSubjets_MinCuts_;
      vector<int> nSubjets_MaxCuts_;
      vector<double> minMass_MinCuts_;
      vector<double> minMass_MaxCuts_;

      TH1F * METAfterKinCuts_;
      TH1F * METAfterTopCuts_;
      TH1F * METAndTop0Mt_;
      TH1F * METAndTop0DeltaPhi_;
      TH1F * METAndTop1Mt_;
      TH1F * METAndTop1DeltaPhi_;
      
      TH1F * topCandMass_;
      TH1F * topCandMinMass_;
      TH1F * topCandPt_;
      TH1F * topCandEta_;

      TH1F * top0Mass_;
      TH1F * top0MinMass_;
      TH1F * top0Pt_;
      TH1F * top0Eta_;
      TH1F * top1Mass_;
      TH1F * top1MinMass_;
      TH1F * top1Pt_;
      TH1F * top1Eta_;
      TH1F * cutFlow_;
      
};

METTopAnalyzer::METTopAnalyzer(const edm::ParameterSet& iConfig)

{
   ////////////////////////////////
   //Grab all the  input tags//////
   ////////////////////////////////
   METP4Src_ = iConfig.getParameter<InputTag>("METP4Src");

   topCandP4Src_ = iConfig.getParameter<InputTag>("topCandP4Src");
   topCandMassSrc_ = iConfig.getParameter<InputTag>("topCandMassSrc");
   topCandMinMassSrc_ = iConfig.getParameter<InputTag>("topCandMinMassSrc");
   topCandNSubjetsSrc_ = iConfig.getParameter<InputTag>("topCandNSubjetsSrc");
   topCandPassTopTagSrc_ = iConfig.getParameter<InputTag>("topCandPassTopTagSrc");

   eventWeightSrc_ = iConfig.getParameter<InputTag>("eventWeightSrc");
   pdfWeightSrc_ = iConfig.getParameter<InputTag>("pdfWeightSrc");
   useEDMEventWeight_ = iConfig.getParameter<bool>("useEDMEventWeight");
   eventWeight_ = iConfig.getParameter<double>("eventWeight");
   topCandPtDef_ = iConfig.getParameter<double>("topCandPtDef");   
   
   /////////////////////////////////////////////////////////////////////
   //Grab all the cut parameters (in order in which they will be used)//
   /////////////////////////////////////////////////////////////////////
   nTopCandCut_ = iConfig.getParameter<int>("nTopCandCut");
   pt_MinCuts_ = iConfig.getParameter<vector<double> >("pt_MinCuts");
   pt_MaxCuts_ = iConfig.getParameter<vector<double> >("pt_MaxCuts");
   eta_MinCuts_ = iConfig.getParameter<vector<double> >("eta_MinCuts");
   eta_MaxCuts_ = iConfig.getParameter<vector<double> >("eta_MaxCuts");
   deltaPhi_MinCuts_ = iConfig.getParameter<vector<double> >("deltaPhi_MinCuts");
   deltaPhi_MaxCuts_ = iConfig.getParameter<vector<double> >("deltaPhi_MaxCuts");
   topMass_MinCuts_ = iConfig.getParameter<vector<double> >("topMass_MinCuts");
   topMass_MaxCuts_ = iConfig.getParameter<vector<double> >("topMass_MaxCuts");
   nSubjets_MinCuts_ = iConfig.getParameter<vector<int> >("nSubjets_MinCuts");
   nSubjets_MaxCuts_ = iConfig.getParameter<vector<int> >("nSubjets_MaxCuts");
   minMass_MinCuts_ = iConfig.getParameter<vector<double> >("minMass_MinCuts");
   minMass_MaxCuts_ = iConfig.getParameter<vector<double> >("minMass_MaxCuts");
   
   //////////////////////////////
   //Define all the histograms///
   //////////////////////////////
   Service<TFileService> rootFile;

   METAfterKinCuts_ = rootFile->make<TH1F>("METKinCuts_", "mE_{T}", 2000, 0, 
                                          2000);
   METAfterTopCuts_ = rootFile->make<TH1F>("METTopCuts", "mE_{T}", 2000, 0, 
                                           2000);
   METAndTop0Mt_ = rootFile->make<TH1F>("METAndTop0Mt", 
                                        "M_{T} of mE_{T} and top1", 5000, 0, 
                                        5000);
   METAndTop1Mt_ = rootFile->make<TH1F>("METAndTop1Mt", 
                                        "M_{T} of mE_{T} and top2",5000, 0, 
                                        5000);
   METAndTop0DeltaPhi_ = rootFile->make<TH1F>("METAndTop0DeltaPhi", 
                                              "mE_T #Delta #phi", 1000, 0, 
                                              2 * TMath::Pi());
   METAndTop1DeltaPhi_ = rootFile->make<TH1F>("METAndTop1DeltaPhi", 
                                            "mE_T #Delta #phi", 1000, 0, 
                                              2 * TMath::Pi());
   topCandMass_ = rootFile->make<TH1F>("topCandMass", "TopCand mass", 5000, 0, 
                                       5000); 
   topCandMinMass_ = rootFile->make<TH1F>("topCandMinMass", "TopCand Min Mass",
                                          5000, 0, 5000); 
   topCandPt_ = rootFile->make<TH1F>("topCandPt", "TopCand p_{T}", 5000, 0, 
                                     5000); 
   topCandEta_ = rootFile->make<TH1F>("topCandEta", "TopCand #eta", 1000, -5.0, 
                                      5.0); 

   top0Mass_ = rootFile->make<TH1F>("top0Mass", "Top0 mass", 5000, 0, 5000); 
   top0MinMass_ = rootFile->make<TH1F>("top0MinMass", "Top0 Min Mass", 5000,0,
                                       5000);  
   top0Pt_ = rootFile->make<TH1F>("top0Pt", "Top0 p_{T}", 5000, 0, 5000); 
   top0Eta_ = rootFile->make<TH1F>("top0Eta", "Top0 #eta", 1000, -5.0, 5.0);
   top1Mass_ = rootFile->make<TH1F>("top1Mass", "Top1 mass", 5000, 0, 5000); 
   top1MinMass_ = rootFile->make<TH1F>("top1MinMass", "Top1 Min Mass", 5000, 
                                       0, 5000); 
   top1Eta_ = rootFile->make<TH1F>("top1Eta", "Top1 #eta", 1000, -5.0, 5.0); 
   top1Pt_ = rootFile->make<TH1F>("top1Pt", "Top1 p_{T}", 5000, 0, 5000); 

   cutFlow_ = rootFile->make<TH1F>("cutFlow", "Cut Flow", 10, 0, 10);

}


METTopAnalyzer::~METTopAnalyzer()
{ 
}

void
METTopAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   //////////////////////////
   //Define variables////////
   //////////////////////////
   Handle<vector<Candidate::PolarLorentzVector> > h_METP4;
   iEvent.getByLabel(METP4Src_, h_METP4);
   vector<Candidate::PolarLorentzVector> METP4 = *h_METP4;

   Handle<vector<Candidate::PolarLorentzVector> > h_topCandP4;
   iEvent.getByLabel(topCandP4Src_ , h_topCandP4);
   vector<Candidate::PolarLorentzVector> topCandP4 = *h_topCandP4;

   Handle<vector<double> > h_topCandMass;
   iEvent.getByLabel(topCandMassSrc_ , h_topCandMass);
   vector<double> topCandMass = *h_topCandMass;

   Handle<vector<double> > h_topCandMinMass;
   iEvent.getByLabel(topCandMinMassSrc_ , h_topCandMinMass);
   vector<double> topCandMinMass = *h_topCandMinMass;

   Handle<vector<double> > h_topCandNSubjets;
   iEvent.getByLabel(topCandNSubjetsSrc_ , h_topCandNSubjets);
   vector<double> topCandNSubjets = *h_topCandNSubjets;
   
   Handle<vector<int> > h_topCandPassTopTag;
   iEvent.getByLabel(topCandPassTopTagSrc_ , h_topCandPassTopTag);
   vector<int> topCandPassTogTag = *h_topCandPassTopTag;

   Handle<double> h_eventWeight;
   iEvent.getByLabel(eventWeightSrc_, h_eventWeight);
   

   Handle<double> h_pdfWeight;
   iEvent.getByLabel(pdfWeightSrc_, h_pdfWeight);

   int nTopCand = 0;
   vector<double> deltaPhi;
   vector<double> Mt;
   
   ///////////////////////////////////////////
   //Preliminary stuff before cuts are made///
   ///////////////////////////////////////////
   if(h_METP4->size() != 1)
      cout<<"MET size does not equal one!!!"<<endl;

   for(unsigned i = 0; i < topCandP4.size(); i++) 
   {
      deltaPhi.push_back(fabs(METP4[i].phi() - topCandP4[i].phi()));
      Mt.push_back( (METP4[i] + topCandP4[i]).Mt());

      if(topCandP4[i].pt() > topCandPtDef_) nTopCand++;
   }
   cutFlow_->Fill(0.5, eventWeight_);
   
   /////////////////////////////////
   //Number of top candidates Cut///
   /////////////////////////////////
   if(nTopCand != nTopCandCut_) return;
   cutFlow_->Fill(1.5, eventWeight_);
   
   /////////////////////////////////   
   //Kinematic cuts/////////////////
   /////////////////////////////////

   for(int i = 0; i < nTopCand; i++)
      if(pt_MinCuts_[i] > topCandP4[i].pt() || 
         pt_MaxCuts_[i] < topCandP4[i].pt()) return;   
   cutFlow_->Fill(2.5, eventWeight_);

   for(int i = 0; i < nTopCand; i++)
      if(eta_MinCuts_[i] > topCandP4[i].eta() || 
         eta_MaxCuts_[i] < topCandP4[i].eta()) return;   
   cutFlow_->Fill(3.5, eventWeight_);

   for(int i = 0; i < nTopCand; i++)
      if(deltaPhi_MinCuts_[i] > deltaPhi[i] || 
         deltaPhi_MaxCuts_[i] < deltaPhi[i]) 
         return;   
   cutFlow_->Fill(3.5, eventWeight_);
   
   /////////////////////////////////
   //Fill histograms after KinCuts//
   /////////////////////////////////
   
   METAfterKinCuts_->Fill(METP4[0].pt(),eventWeight_);
   for(int i = 0; i < nTopCand; i++)
   {
      topCandMass_->Fill(topCandP4[i].mass(), eventWeight_);
      topCandMinMass_->Fill(topCandMinMass[i], eventWeight_);
      topCandPt_->Fill(topCandP4[i].pt(), eventWeight_);
      topCandEta_->Fill(topCandP4[i].eta(), eventWeight_);
   }
   
   /////////////////////////////////
   //Top cuts///////////////////////
   /////////////////////////////////
   for(int i = 0; i < nTopCand; i++)
      if(topMass_MinCuts_[i] > topCandP4[i].mass() || 
         topMass_MaxCuts_[i] < topCandP4[i].mass()) return;   
   cutFlow_->Fill(4.5, eventWeight_);

   for(int i = 0; i < nTopCand; i++)
      if(nSubjets_MinCuts_[i] > topCandNSubjets[i] || 
         nSubjets_MaxCuts_[i] < topCandNSubjets[i]) return;   
   cutFlow_->Fill(5.5, eventWeight_);
  
   for(int i = 0; i < nTopCand; i++)
      if(minMass_MinCuts_[i] > topCandMinMass[i] || 
         minMass_MaxCuts_[i] < topCandMinMass[i]) return;   
   cutFlow_->Fill(6.5, eventWeight_);
   
   /////////////////////////////////
   //Fill histograms after TopCuts//
   /////////////////////////////////
   METAfterTopCuts_->Fill(METP4[0].pt(), eventWeight_);

   METAndTop0Mt_->Fill(Mt[0], eventWeight_);
   METAndTop0DeltaPhi_->Fill(deltaPhi[0], eventWeight_);
   top0Mass_->Fill(topCandP4[0].mass(),eventWeight_);
   top0MinMass_->Fill(topCandMinMass[0],eventWeight_);
   top0Pt_->Fill(topCandP4[0].pt(),eventWeight_);
   top0Eta_->Fill(topCandP4[0].eta(),eventWeight_);
   
   if(nTopCandCut_ != 2) return;

   METAndTop1Mt_->Fill(Mt[1], eventWeight_);
   METAndTop1DeltaPhi_->Fill(deltaPhi[1], eventWeight_);
   top1Mass_->Fill(topCandP4[1].mass(),eventWeight_);
   top1MinMass_->Fill(topCandMinMass[1],eventWeight_);
   top1Pt_->Fill(topCandP4[1].pt(),eventWeight_);
   top1Eta_->Fill(topCandP4[1].eta(),eventWeight_);

   return;
}

void 
METTopAnalyzer::beginJob()
{
}

void 
METTopAnalyzer::endJob() 
{
}

void 
METTopAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

void 
METTopAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

void 
METTopAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

void 
METTopAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

void
METTopAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {

  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
DEFINE_FWK_MODULE(METTopAnalyzer);
