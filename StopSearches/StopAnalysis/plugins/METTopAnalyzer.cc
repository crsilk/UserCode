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
#include "TDirectory.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"

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
      InputTag jetsP4Src_;
      InputTag muonsP4Src_;
      InputTag topCandP4Src_;
      InputTag genJetsP4Src_;
      InputTag topCandMassSrc_;
      InputTag topCandMinMassSrc_;
      InputTag topCandNSubjetsSrc_;
      InputTag topCandPassTopTagSrc_;
      InputTag eventWeightSrc_;
      InputTag pdfWeightSrc_;
      InputTag modelParametersSrc_;
      InputTag alphaTSrc_;
      InputTag HTSrc_;
      InputTag MHTSrc_;
      InputTag metTopDeltaPhiSrc_;
      InputTag metTopMtSrc_;
      InputTag topTagMatchMatrixSrc_;
      InputTag wTagMatchMatrixSrc_;

      bool useEDMEventWeight_;
      double eventWeight_;
      double topCandPtDef_;
      bool enactCuts_;

      double jetPtCut_;
      double muonPtCut_;
      
      vector<double> modelParameters_;
      int nTopCandCut_;
      vector<double> pt_MinCuts_;
      vector<double> pt_MaxCuts_;
      vector<double> eta_MinCuts_;
      vector<double> eta_MaxCuts_;
      vector<double> metTopDeltaPhi_MinCuts_;
      vector<double> metTopDeltaPhi_MaxCuts_;
      vector<double> topMass_MinCuts_;
      vector<double> topMass_MaxCuts_;
      vector<int> nSubjets_MinCuts_;
      vector<int> nSubjets_MaxCuts_;
      vector<double> minMass_MinCuts_;
      vector<double> minMass_MaxCuts_;

      double METCut_;
      double alphaTCut_;
   
      TH1F * HT_;
      TH1F * MHT_;
      TH1F * MET_;
      TH1F * nJets_;
      TH1F * jetPt_;
      TH1F * jetEta_;
      TH1F * nMuons_;
      TH1F * muonPt_;
      TH1F * muonEta_;
      TH1F * top0Mass_;
      TH1F * top0MinMass_;
      TH1F * top0Pt_;
      TH1F * top0Eta_;
      TH1F * top1Mass_;
      TH1F * top1MinMass_;
      TH1F * top1Pt_;
      TH1F * top1Eta_;
      TH1F * METAndTop0Mt_;
      TH1F * METAndTop0DeltaPhi_;
      TH1F * METAndTop1Mt_;
      TH1F * METAndTop1DeltaPhi_;
      TH1F * muonAndTop0DeltaR_;
      TH1F * muonAndTop1DeltaR_;
      TH1F * top0AndTop1DeltaPhi_;
      TH1F * top0AndTop1AlphaT_;
      TH1F * cutFlow_;
      
};

METTopAnalyzer::METTopAnalyzer(const edm::ParameterSet& iConfig)
{
   ////////////////////////////////
   //Grab all the  input tags//////
   ////////////////////////////////
   METP4Src_ = iConfig.getParameter<InputTag>("METP4Src");
   jetsP4Src_ = iConfig.getParameter<InputTag>("jetsP4Src");
   muonsP4Src_ = iConfig.getParameter<InputTag>("muonsP4Src");
   genJetsP4Src_  = iConfig.getParameter<InputTag>("genJetsP4Src");
   topCandP4Src_ = iConfig.getParameter<InputTag>("topCandP4Src");
   topCandMassSrc_ = iConfig.getParameter<InputTag>("topCandMassSrc");
   topCandMinMassSrc_ = iConfig.getParameter<InputTag>("topCandMinMassSrc");
   topCandNSubjetsSrc_ = iConfig.getParameter<InputTag>("topCandNSubjetsSrc");
   topCandPassTopTagSrc_ = iConfig.getParameter<InputTag>("topCandPassTopTagSrc");

   eventWeightSrc_ = iConfig.getParameter<InputTag>("eventWeightSrc");
   pdfWeightSrc_ = iConfig.getParameter<InputTag>("pdfWeightSrc");
   modelParametersSrc_ = iConfig.getParameter<InputTag>("modelParametersSrc");
   useEDMEventWeight_ = iConfig.getParameter<bool>("useEDMEventWeight");
   eventWeight_ = iConfig.getParameter<double>("eventWeight");
   topCandPtDef_ = iConfig.getParameter<double>("topCandPtDef");   
   enactCuts_ = iConfig.getParameter<bool>("enactCuts");
   /////////////////////////////////////////////////////////////////////
   //Grab all the cut parameters (in order in which they will be used)//
   /////////////////////////////////////////////////////////////////////
   modelParameters_ = iConfig.getParameter<vector<double> >("modelParameters");
   jetPtCut_ = iConfig.getParameter<double>("jetPtCut");
   muonPtCut_ = iConfig.getParameter<double>("muonPtCut");
   nTopCandCut_ = iConfig.getParameter<int>("nTopCandCut");
   pt_MinCuts_ = iConfig.getParameter<vector<double> >("pt_MinCuts");
   pt_MaxCuts_ = iConfig.getParameter<vector<double> >("pt_MaxCuts");
   eta_MinCuts_ = iConfig.getParameter<vector<double> >("eta_MinCuts");
   eta_MaxCuts_ = iConfig.getParameter<vector<double> >("eta_MaxCuts");
   metTopDeltaPhi_MinCuts_ = iConfig.getParameter<vector<double> >("metTopDeltaPhi_MinCuts");
   metTopDeltaPhi_MaxCuts_ = iConfig.getParameter<vector<double> >("metTopDeltaPhi_MaxCuts");
   topMass_MinCuts_ = iConfig.getParameter<vector<double> >("topMass_MinCuts");
   topMass_MaxCuts_ = iConfig.getParameter<vector<double> >("topMass_MaxCuts");
   nSubjets_MinCuts_ = iConfig.getParameter<vector<int> >("nSubjets_MinCuts");
   nSubjets_MaxCuts_ = iConfig.getParameter<vector<int> >("nSubjets_MaxCuts");
   minMass_MinCuts_ = iConfig.getParameter<vector<double> >("minMass_MinCuts");
   minMass_MaxCuts_ = iConfig.getParameter<vector<double> >("minMass_MaxCuts");

   METCut_ = iConfig.getParameter<double>("METCut");
   alphaTCut_ = iConfig.getParameter<double>("alphaTCut");
   
   //////////////////////////////
   //Define all the histograms///
   //////////////////////////////
   Service<TFileService> fileService;
   
   //Directories

   //BeforeCuts
   HT_ = fileService->make<TH1F>("HT", "HT", 2000, 0, 2000);
   MHT_ = fileService->make<TH1F>("MHT", "MHT", 2000, 0, 2000);
   MET_ = fileService->make<TH1F>("MET", "mE_{T}", 2000, 0, 2000);
   nJets_ = fileService->make<TH1F>("nJets", "Jet multiplicity", 50, 0, 50);
   jetPt_ = fileService->make<TH1F>("jetPt", "Jet p_{T}", 2000, 0, 2000);
   jetEta_ = fileService->make<TH1F>("jetEta", "Jet #eta", 1000, -5.0, 5.0);
   nMuons_ = fileService->make<TH1F>("nMuons", "Muon multiplicity", 10, 0, 10);
   muonPt_ = fileService->make<TH1F>("muonPt", "Muon p_{T}", 2000, 0, 2000);
   muonEta_ = fileService->make<TH1F>("muonEta", "Muon #eta", 1000, -5.0, 5.0);
   
   top0Mass_ = fileService->make<TH1F>("top0Mass", 
                                            "Top0 mass", 500, 0, 500); 
   top0MinMass_ = fileService->make<TH1F>("top0MinMass", 
                                               "Top0 Min Mass", 500,0, 500);  
   top0Pt_ = fileService->make<TH1F>("top0Pt", "Top0 p_{T}", 
                                             5000, 0, 5000); 
   top0Eta_ = fileService->make<TH1F>("top0Eta", "Top0 #eta", 
                                           1000, -5.0, 5.0);
   METAndTop0DeltaPhi_ = fileService->make<TH1F>("METAndTop0DeltaPhi",  "mE_{T} and Top 0 #Delta #phi", 1000, 0,  2 * TMath::Pi());
   muonAndTop0DeltaR_ = fileService->make<TH1F>("muonAndTop0DeltaR",  "muon and Top 0 #Delta R", 1000, 0, 2 * TMath::Pi());
   top1Mass_ = fileService->make<TH1F>("top1Mass", "Top1 mass", 
                                            500, 0, 500); 
   top1MinMass_ = fileService->make<TH1F>("top1MinMass", 
                                               "Top1 Min Mass", 500,  0, 500); 
   top1Eta_ = fileService->make<TH1F>("top1Eta", "Top1 #eta", 
                                           1000, -5.0, 5.0); 
   top1Pt_ = fileService->make<TH1F>("top1Pt", "Top1 p_{T}", 
                                             5000,  0, 5000); 

   METAndTop0Mt_ = fileService->make<TH1F>("METAndTop0Mt", 
                                        "M_{T} of mE_{T} and top0", 5000, 0, 
                                        5000);
   METAndTop1Mt_ = fileService->make<TH1F>("METAndTop1Mt", 
                                        "M_{T} of mE_{T} and top1",5000, 0, 
                                        5000);
   METAndTop1DeltaPhi_ = fileService->make<TH1F>("METAndTop1DeltaPhi",  "mE_{T} and Top 1 #Delta #phi", 1000, 0, 2 * TMath::Pi());
   muonAndTop1DeltaR_ = fileService->make<TH1F>("muonAndTop1DeltaR",  "muon and Top 1 #Delta R", 1000, 0, 2 * TMath::Pi());
   top0AndTop1DeltaPhi_ = fileService->make<TH1F>("top0AndTop1DeltaPhi",  "Top 0 and Top 1 #Delta #phi", 1000, 0, 2 * TMath::Pi());
   top0AndTop1AlphaT_ = fileService->make<TH1F>("top0AndTop1AlphaT", "Top 0 and Top1 #alpha_{T}", 200, 0 , 2.0);
   cutFlow_ = fileService->make<TH1F>("cutFlow", "Cut Flow", 10, 0, 10);

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
   Candidate::PolarLorentzVector METP4 = h_METP4->front();

   Handle<vector<Candidate::PolarLorentzVector> > h_jetsP4;
   iEvent.getByLabel(jetsP4Src_, h_jetsP4);
   vector<Candidate::PolarLorentzVector> jetsP4 = *h_jetsP4;

   Handle<vector<Candidate::PolarLorentzVector> > h_muonsP4;
   iEvent.getByLabel(muonsP4Src_, h_muonsP4);
   vector<Candidate::PolarLorentzVector> muonsP4 = *h_muonsP4;

   Handle<vector<Candidate::PolarLorentzVector> > h_genJetsP4;
   iEvent.getByLabel(genJetsP4Src_, h_genJetsP4);
   vector<Candidate::PolarLorentzVector> genJetsP4 = *h_genJetsP4;

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

   Handle<vector<double> > h_modelParameters;
   bool findModelParameters = iEvent.getByLabel(modelParametersSrc_, h_modelParameters);
   vector<double>::const_iterator modelParameter;

   Handle<double> h_alphaT;
   iEvent.getByLabel(alphaTSrc_, h_alphaT);
   double alphaT = *h_alphaT;

   Handle<double> h_HT;
   iEvent.getByLabel(HTSrc_, h_HT);
   double HT = *h_HT;

   Handle<vector<Candidate::PolarLorentzVector> > h_MHTP4;
   iEvent.getByLabel(MHTSrc_, h_MHTP4);
   Candidate::PolarLorentzVector MHTP4 = h_MHTP4->front();

   Handle<vector<double> > h_metTopDeltaPhi;
   iEvent.getByLabel(metTopDeltaPhiSrc_, h_metTopDeltaPhi);
   vector<double> metTopDeltaPhi;

   Handle<vector<double> > h_metTopMt;
   iEvent.getByLabel(metTopMtSrc_, h_metTopMt);
   vector<double> metTopMt;

   Handle<vector<vector<int> > > h_topTagMatchMatrix;
   iEvent.getByLabel(topTagMatchMatrixSrc_, h_topTagMatchMatrix);
   vector<vector<int> >::const_iterator topTagMatchMatrixRow;

   

   int nTopCand = 0;
   vector<double> topTopDeltaPhi;
   vector<double> jetsPt;
   vector<double> jetsEta;
   vector<double> muonTop0DeltaR;
   vector<double> muonTop1DeltaR;
   vector<double> muonsPt;
   vector<double> muonsEta;
   
   vector<double> Mt;
   unsigned tempCount = 0;
   double tempDeltaPhi = 0;
   int jetNumber = 0;
   int muonNumber = 0;
   
   ///////////////////////////////////////////
   //Preliminary stuff before cuts are made///
   ///////////////////////////////////////////
   if(useEDMEventWeight_)
   {
      eventWeight_ = *h_eventWeight;
   }

   if(findModelParameters)
   {
      for(modelParameter = h_modelParameters->begin(); 
          modelParameter != h_modelParameters->end(); modelParameter++) 
      {
         if(*modelParameter != modelParameters_[tempCount])
            return;
         tempCount++;
      }
   }

   for(unsigned i = 0; i < topCandP4.size(); i++) 
      if(topCandP4[i].pt() > topCandPtDef_) nTopCand++;


   cutFlow_->Fill(0.5, eventWeight_);   
   if(enactCuts_)
   {
      /////////////////////////////////
      //Event-level Cuts///////////////
      /////////////////////////////////
      
      //number of top candidates cut
      if(nTopCand < nTopCandCut_) return;
      cutFlow_->Fill(1.5, eventWeight_);
      
      //met cut
      if(METP4.pt() < METCut_) return;
      cutFlow_->Fill(2.5,eventWeight_);
      
      //alphaT cut
      if(alphaT > -1.0)
      {
         if(alphaT < alphaTCut_) return;
      }
      cutFlow_->Fill(3.5, eventWeight_);
      
      
      /////////////////////////////////   
      //Kinematic cuts/////////////////
      /////////////////////////////////
      
      //top candidate pt cuts
      for(int i = 0; i < nTopCandCut_; i++)
         if(pt_MinCuts_[i] > topCandP4[i].pt() || 
            pt_MaxCuts_[i] < topCandP4[i].pt()) return;   
      cutFlow_->Fill(4.5, eventWeight_);
      
      //top candidate eta cuts
      for(int i = 0; i < nTopCandCut_; i++)
         if(eta_MinCuts_[i] > topCandP4[i].eta() || 
            eta_MaxCuts_[i] < topCandP4[i].eta()) return;   
      cutFlow_->Fill(5.5, eventWeight_);
      
      //met and top delta phi cuts
      for(int i = 0; i < nTopCandCut_; i++)
         if(metTopDeltaPhi_MinCuts_[i] > metTopDeltaPhi[i] || 
            metTopDeltaPhi_MaxCuts_[i] < metTopDeltaPhi[i]) 
            return;   
      cutFlow_->Fill(6.5, eventWeight_);
      
      /////////////////////////////////
      //Top cuts///////////////////////
      /////////////////////////////////
      
      //top candidate mass cuts
      for(int i = 0; i < nTopCandCut_; i++)
         if(topMass_MinCuts_[i] > topCandMass[i] || 
            topMass_MaxCuts_[i] < topCandMass[i]) return;   
      cutFlow_->Fill(7.5, eventWeight_);
      
      //number of subjets in top candidate cut
      for(int i = 0; i < nTopCandCut_; i++)
         if(nSubjets_MinCuts_[i] > topCandNSubjets[i] || 
            nSubjets_MaxCuts_[i] < topCandNSubjets[i]) return;   
      cutFlow_->Fill(8.5, eventWeight_);
      
      //top candidate min mass cut
      for(int i = 0; i < nTopCandCut_; i++)
         if(minMass_MinCuts_[i] > topCandMinMass[i] || 
            minMass_MaxCuts_[i] < topCandMinMass[i]) return;   
      cutFlow_->Fill(9.5, eventWeight_);
   }
   /////////////////////////////////
   //Fill histograms after Cuts/////
   /////////////////////////////////
   for( unsigned i = 0; i < jetsP4.size(); i++)
   {
      if(jetsP4[i].pt() < jetPtCut_) continue;
      jetNumber++;      

      jetPt_->Fill(jetsP4[i].pt(), eventWeight_);
      jetEta_->Fill(jetsP4[i].eta(), eventWeight_);
   }

   nJets_->Fill(jetNumber, eventWeight_);
   HT_->Fill(HT, eventWeight_);
   MHT_->Fill(MHTP4.pt(), eventWeight_);
   MET_->Fill(METP4.pt(), eventWeight_);

   for( unsigned i = 0; i < muonsP4.size(); i++)
   {
      if(muonsP4[i].pt() < muonPtCut_) continue;
      
      muonPt_->Fill(muonsP4[i].pt(), eventWeight_);
      muonEta_->Fill(muonsP4[i].eta(), eventWeight_);
      muonNumber++;

      if(topCandP4.size() > 0)
      {
         muonAndTop0DeltaR_->Fill(deltaR(muonsP4[i], topCandP4[0]), 
                                  eventWeight_);
      }

      if(topCandP4.size() > 1)
      {
         muonAndTop1DeltaR_->Fill(deltaR(muonsP4[i], topCandP4[1]), 
                                  eventWeight_);
      }
   }
   nMuons_->Fill(muonNumber, eventWeight_);


   if(topCandP4.size() > 0)
   {
      METAndTop0Mt_->Fill(metTopMt[0], eventWeight_);
      METAndTop0DeltaPhi_->Fill(metTopDeltaPhi[0], eventWeight_);
      top0Mass_->Fill(topCandMass[0],eventWeight_);
      top0MinMass_->Fill(topCandMinMass[0],eventWeight_);
      top0Pt_->Fill(topCandP4[0].pt(),eventWeight_);
      top0Eta_->Fill(topCandP4[0].eta(),eventWeight_);
   }
   if(topCandP4.size() > 1) 
   {
      METAndTop1Mt_->Fill(metTopMt[1], eventWeight_);
      METAndTop1DeltaPhi_->Fill(metTopDeltaPhi[1], eventWeight_);
      top1Mass_->Fill(topCandMass[1],eventWeight_);
      top1MinMass_->Fill(topCandMinMass[1],eventWeight_);
      top1Pt_->Fill(topCandP4[1].pt(),eventWeight_);
      top1Eta_->Fill(topCandP4[1].eta(),eventWeight_);

      tempDeltaPhi =  fabs(topCandP4[0].phi() - topCandP4[1].phi());
      if (tempDeltaPhi > 3.14159)
         tempDeltaPhi = 2 * 3.14159 - tempDeltaPhi;
      top0AndTop1DeltaPhi_->Fill(tempDeltaPhi, eventWeight_);
      
      top0AndTop1AlphaT_->Fill(alphaT, eventWeight_);

   }

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
