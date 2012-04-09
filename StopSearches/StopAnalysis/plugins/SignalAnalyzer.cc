
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TMath.h"
#include "TDirectory.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopTagFunctor.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace pat;

class SignalAnalyzer : public edm::EDAnalyzer {
   public:
      explicit SignalAnalyzer(const edm::ParameterSet&);
      ~SignalAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

   InputTag genParticleSrc_;
   InputTag jetSrc_;
   InputTag metSrc_;
   InputTag electronSrc_;
   InputTag muonSrc_;
   InputTag topTagSrc_;
   InputTag wTagSrc_;
   
   double jetPtCut_;
   double muonPtCut_;   
   double electronPtCut_;

   double topDeltaRMatchDef_;
   double wDeltaRMatchDef_;
   double jetDeltaRMatchDef_;
   double muonDeltaRMatchDef_;
   double electronDeltaRMatchDef_;
   string topTagName_;
  
   double topMass_minCut_;
   double topMass_maxCut_;
   unsigned topNSubjets_minCut_;
   double topMinMass_minCut_;

   TH1F * genSumPt_; 
   TH1F * genMET_;

   TH1F * genTop0Pt_; 
   TH1F * genTop1Pt_; 
   TH1F * genW0Pt_; 
   TH1F * genW1Pt_; 
   TH1F * genBottom0Pt_; 
   TH1F * genBottom1Pt_; 
   TH1F * genTop0Eta_; 
   TH1F * genTop1Eta_; 
   TH1F * genW0Eta_; 
   TH1F * genW1Eta_; 
   TH1F * genBottom0Eta_; 
   TH1F * genBottom1Eta_; 
   TH1F * genMetTop0DeltaPhi_; 
   TH1F * genMetTop1DeltaPhi_; 
   TH1F * genTop0Top1DeltaPhi_; 

   TH1F * recoMET_;
   TH1F * recoTop0Pt_; 
   TH1F * recoTop1Pt_; 
   TH1F * recoTop0Eta_; 
   TH1F * recoTop1Eta_; 
   TH1F * recoTop0GenPt_; 
   TH1F * recoTop1GenPt_; 
   TH1F * recoTop0GenEta_; 
   TH1F * recoTop1GenEta_; 
   TH1F * recoMetTop0DeltaPhi_; 
   TH1F * recoMetTop1DeltaPhi_; 
   TH1F * recoTop0Top1DeltaPhi_; 
   TH1F * recoFakeTopPt_;
   TH1F * recoFakeTopEta_;


   TH2F * genBottomWDeltaRVsTopPt_; 
   TH2F * genQ1Q2DeltaRVsWPt_; 
   TH2F * genTop0Top1DeltaPhiVsMET_;
   TH2F * genTop0PtVsTop1Pt_;


};

SignalAnalyzer::SignalAnalyzer(const edm::ParameterSet& iConfig)
{
   ////////////////////////////////
   //Grab all the  input tags//////
   ////////////////////////////////
   genParticleSrc_ = iConfig.getParameter<InputTag>("genParticleSrc");
   jetSrc_ = iConfig.getParameter<InputTag>("jetSrc");
   metSrc_ = iConfig.getParameter<InputTag>("metSrc");
   electronSrc_ = iConfig.getParameter<InputTag>("electronSrc");
   muonSrc_ = iConfig.getParameter<InputTag>("muonSrc");
   topTagSrc_ = iConfig.getParameter<InputTag>("topTagSrc");
   wTagSrc_ = iConfig.getParameter<InputTag>("wTagSrc");

   

   ///////////////////////////////
   //Grab all the parameters//
   ///////////////////////////////
   jetPtCut_ = iConfig.getParameter<double>("jetPtCut");
   muonPtCut_ = iConfig.getParameter<double>("muonPtCut");
   electronPtCut_ = iConfig.getParameter<double>("electronPtCut");
   
   topDeltaRMatchDef_ = iConfig.getParameter<double>("topDeltaRMatchDef");
   wDeltaRMatchDef_ = iConfig.getParameter<double>("wDeltaRMatchDef");
   jetDeltaRMatchDef_ = iConfig.getParameter<double>("jetDeltaRMatchDef");
   muonDeltaRMatchDef_ = iConfig.getParameter<double>("muonDeltaRMatchDef");
   electronDeltaRMatchDef_ = iConfig.getParameter<double>("electronDeltaRMatchDef");   
   topTagName_ = iConfig.getParameter<std::string>("topTagName");

   topMass_minCut_ = iConfig.getParameter<double>("topMass_minCut");
   topMass_maxCut_ = iConfig.getParameter<double>("topMass_maxCut");
   topNSubjets_minCut_ = iConfig.getParameter<unsigned>("topNSubjets_minCut");
   topMinMass_minCut_ = iConfig.getParameter<double>("topMinMass_minCut");

   //////////////////////////////
   //Define all the histograms///
   //////////////////////////////
   Service<TFileService> fileService;

   genSumPt_ = fileService->make<TH1F>("genSumPt", "Gen-level #Sigma p_{T}", 200, 0, 2000);
   genMET_ = fileService->make<TH1F>("genMET", "Gen-level MET", 200, 0, 2000);

   genTop0Pt_ = fileService->make<TH1F>("genTop0Pt", "Gen-level Top0 p_{T}", 1000, 0, 1000);
   genTop1Pt_ = fileService->make<TH1F>("genTop1Pt", "Gen-level Top1 p_{T}", 1000, 0, 1000);
   genW0Pt_ = fileService->make<TH1F>("genW0Pt", "Gen-level W0 p_{T}", 1000, 0, 1000);
   genW1Pt_ = fileService->make<TH1F>("genW1Pt", "Gen-level W1 p_{T}", 1000, 0, 1000);
   genBottom0Pt_ = fileService->make<TH1F>("genBottom0Pt", "Gen-level Bottom0 p_{T}", 1000, 0, 1000);
   genBottom1Pt_ = fileService->make<TH1F>("genBottom1Pt", "Gen-level Bottom1 p_{T}", 1000, 0, 1000);

   genTop0Eta_ = fileService->make<TH1F>("genTop0Eta", "Gen-level Top0 #eta", 100, -5.0, 5.0);
   genTop1Eta_ = fileService->make<TH1F>("genTop1Eta", "Gen-level Top1 #eta", 100, -5.0, 5.0);
   genW0Eta_ = fileService->make<TH1F>("genW0Eta", "Gen-level W0 #eta", 100, -5.0, 5.0);
   genW1Eta_ = fileService->make<TH1F>("genW1Eta", "Gen-level W1 #eta", 100, -5.0, 5.0);
   genBottom0Eta_ = fileService->make<TH1F>("genBottom0Eta", "Gen-level Bottom0 #eta", 100, -5.0, 5.0);
   genBottom1Eta_ = fileService->make<TH1F>("genBottom1Eta", "Gen-level Bottom1 #eta", 100, -5.0, 5.0);

   genMetTop0DeltaPhi_ = fileService->make<TH1F>("genMetTop0DeltaPhi", "Gen-level Top0 and Met #Delta #phi", 100, 0, TMath::Pi());
   genMetTop1DeltaPhi_ = fileService->make<TH1F>("genMetTop1DeltaPhi", "Gen-level Top1 and Met #Delta #phi", 100, 0, TMath::Pi());
   genTop0Top1DeltaPhi_ = fileService->make<TH1F>("genTop0Top1DeltaPhi", "Gen-level Top0 and Top1 #Delta #phi", 100, 0, TMath::Pi());

   genBottomWDeltaRVsTopPt_ = fileService->make<TH2F>("genBottomWDeltaRVsTopPt", "Gen W and Bottom #Delta R Vs. Top p_{t}", 200,0, 2000, 100, 0, 5);
   genQ1Q2DeltaRVsWPt_ = fileService->make<TH2F>("genQ1Q2DeltaRVsWPt", "Gen Quark 1 and 2 #Delta R Vs.W  p_{t}", 400, 0, 2000, 100, 0, 5);
   genTop0Top1DeltaPhiVsMET_ = fileService->make<TH2F>("genTop0Top1DeltaPhiVsMET", "Gen Top 0 and 1 #Delta #phi Vs. MET", 400, 0, 2000, 100, 0, TMath::Pi());
   genTop0PtVsTop1Pt_ = fileService->make<TH2F>("genTop0PtVsTop1Pt", "Gen Top 0 p_{t} Vs. Top 1 p_{t}", 200, 0, 2000, 200, 0 , 2000);


   recoMET_ = fileService->make<TH1F>("recoMET", "Reco-level MET", 200, 0, 2000);

   recoTop0Pt_ = fileService->make<TH1F>("recoTop0Pt", "Reco-level Top0 p_{T}", 1000, 0, 1000);
   recoTop1Pt_ = fileService->make<TH1F>("recoTop1Pt", "Reco-level Top1 p_{T}", 1000, 0, 1000);

   recoTop0Eta_ = fileService->make<TH1F>("recoTop0Eta", "Reco-level Top0 #eta", 100, -5.0, 5.0);
   recoTop1Eta_ = fileService->make<TH1F>("recoTop1Eta", "Reco-level Top1 #eta", 100, -5.0, 5.0);

   recoTop0GenPt_ = fileService->make<TH1F>("recoTop0GenPt", "Reco-level Top0 p_{T}", 1000, 0, 1000);
   recoTop1GenPt_ = fileService->make<TH1F>("recoTop1GenPt", "Reco-level Top1 p_{T}", 1000, 0, 1000);

   recoTop0GenEta_ = fileService->make<TH1F>("recoTop0GenEta", "Reco-level Top0 #eta", 100, -5.0, 5.0);
   recoTop1GenEta_ = fileService->make<TH1F>("recoTop1GenEta", "Reco-level Top1 #eta", 100, -5.0, 5.0);

   recoMetTop0DeltaPhi_ = fileService->make<TH1F>("recoMetTop0DeltaPhi", "Reco-level Top0 and Met #Delta #phi", 100, 0, TMath::Pi());
   recoMetTop1DeltaPhi_ = fileService->make<TH1F>("recoMetTop1DeltaPhi", "Reco-level Top1 and Met #Delta #phi", 100, 0, TMath::Pi());
   recoTop0Top1DeltaPhi_ = fileService->make<TH1F>("recoTop0Top1DeltaPhi", "Reco-level Top0 and Top1 #Delta #phi", 100, 0, TMath::Pi());

   recoFakeTopPt_ = fileService->make<TH1F>("recoFakeTopPt", "Reco-level Fake Tops' p_{t}", 200, 0, 2000);
   recoFakeTopEta_ = fileService->make<TH1F>("recoFakeTopEta", "Reco-level Fake Tops' #eta", 100, -5.0, 5.0);

   
}


SignalAnalyzer::~SignalAnalyzer()
{
 
}


void
SignalAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   //////////////////////////
   //Define variables////////
   //////////////////////////
   Handle<vector<GenParticle> > h_genParticles;
   iEvent.getByLabel(genParticleSrc_, h_genParticles);
   vector<reco::GenParticle>::const_iterator genParticle;

   Handle<vector<pat::Jet> > h_jets;
   iEvent.getByLabel(jetSrc_, h_jets);
   vector<pat::Jet>::const_iterator jet;

   Handle<vector<pat::MET> > h_mets;
   iEvent.getByLabel(metSrc_, h_mets);
   vector<pat::MET>::const_iterator met = h_mets->begin();

   Handle<vector<pat::Electron> > h_electrons;
   iEvent.getByLabel(electronSrc_, h_electrons);
   vector<pat::Electron>::const_iterator electron;

   Handle<vector<pat::Muon> > h_muons;
   iEvent.getByLabel(muonSrc_, h_muons);
   vector<pat::Muon>::const_iterator muon;

   Handle<vector<pat::Jet> > h_wTags;
   iEvent.getByLabel(wTagSrc_, h_wTags);
   vector<pat::Jet>::const_iterator wTag;

   Handle<vector<pat::Jet> > h_topTags;
   iEvent.getByLabel(topTagSrc_, h_topTags);
   vector<pat::Jet> topTags = *h_topTags;
   
   Candidate::LorentzVector genMETVector(0, 0, 0, 0);
   double genSumPt = 0;
   double tempDeltaPhi = 0;
   vector<GenParticle> genTops;
   GenParticle::daughters topDaughts;
   GenParticle::daughters wDaughts;
   unsigned WNumber = 0;
   unsigned bNumber = 0;
   unsigned q1Number = 0;
   unsigned q2Number = 0;
   unsigned recoTop0Number = 1000;
   unsigned recoTop1Number = 1000;
   int tempCount = -1;
   /////////////
   //GEN-Level//
   /////////////
   for(genParticle = h_genParticles->begin(); 
       genParticle != h_genParticles->end(); genParticle++)
   {
      if(genParticle->status() == 1 && genParticle->pdgId() != 1000022 &&
         abs(genParticle->pdgId()) != 12 &&
         abs(genParticle->pdgId()) != 14 &&
         abs(genParticle->pdgId()) != 16)
         //Stable particles that aren't soley weakly interacting
      {
         genSumPt = genSumPt + genParticle->pt();
         genMETVector = genMETVector + genParticle->p4();
      }
      if(genParticle->status() == 3)
      {

      }
      if(abs(genParticle->pdgId()) == 6)// it's a (anti) top
         genTops.push_back(*genParticle);      
   }
   genMETVector = (-1)*genMETVector;
   cout<<"\n"<<endl;
   for(unsigned i = 0; i < genTops.size(); i++)
   {
      if(i == 0) //First top 
      {
         topDaughts = genTops[i].daughterRefVector();  

         for(unsigned j = 0; j < topDaughts.size(); j++)
         {
            if(topDaughts[j]->status() != 3) continue;
            
            if(abs(topDaughts[j]->pdgId()) == 24)//it's a W
            {
               WNumber = j;
               wDaughts = topDaughts[j]->daughterRefVector();

               for(unsigned k = 0; k < wDaughts.size(); k++)
               {
                  if(wDaughts[k]->status() != 3) continue;
                  if(abs(wDaughts[k]->pdgId()) == 11 || 
                     abs(wDaughts[k]->pdgId()) == 13 || 
                     abs(wDaughts[k]->pdgId()) == 15) return; //veto leptonic
                  if(q1Number == 0) 
                     q1Number = k;
                  else
                     q2Number = k;             
               }
            }

            if(abs(topDaughts[j]->pdgId()) == 5)// it's a b
            {
               bNumber = j;
               
            }
         }
         
         //Fill histograms
         
         genTop0Pt_->Fill(genTops[i].pt());
         genTop0Eta_->Fill(genTops[i].eta());
         genBottom0Pt_->Fill(topDaughts[bNumber]->pt());
         genBottom0Eta_->Fill(topDaughts[bNumber]->eta());
         genW0Pt_->Fill(topDaughts[WNumber]->pt());
         genW0Eta_->Fill(topDaughts[WNumber]->eta());
         
         tempDeltaPhi = fabs(genMETVector.phi() - genTops[i].phi());
      if(tempDeltaPhi > TMath::Pi())
         tempDeltaPhi = 2*TMath::Pi() - tempDeltaPhi;
      genMetTop0DeltaPhi_->Fill(tempDeltaPhi);

      genBottomWDeltaRVsTopPt_->Fill(genTops[i].pt(), 
                                     deltaR(topDaughts[WNumber]->p4(), 
                                            topDaughts[bNumber]->p4()));
      
      genQ1Q2DeltaRVsWPt_->Fill(topDaughts[WNumber]->pt(), 
                                deltaR(wDaughts[q1Number]->p4(), 
                                       wDaughts[q2Number]->p4()));


      }
      if(i == 1) //Second top
      {
         WNumber = 0;
         bNumber = 0;
         q1Number = 0;
         q2Number = 0;
         
         topDaughts = genTops[i].daughterRefVector();  

         for(unsigned j = 0; j < topDaughts.size(); j++)
         {
            if(topDaughts[j]->status() != 3) continue;
            
            if(abs(topDaughts[j]->pdgId()) == 24)//it's a W
            {
               WNumber = j;
               wDaughts = topDaughts[j]->daughterRefVector();

               for(unsigned k = 0; k < wDaughts.size(); k++)
               {
                  if(wDaughts[k]->status() != 3) continue;

                  if(q1Number == 0) 
                     q1Number = k;
                  else
                     q2Number = k;             
               }
            }

            if(abs(topDaughts[j]->pdgId()) == 5)// it's a b
            {
               bNumber = j;
               
            }
         }
         
         //Fill histograms
         

         genTop1Pt_->Fill(genTops[i].pt());
         genTop1Eta_->Fill(genTops[i].eta());
         genBottom1Pt_->Fill(topDaughts[bNumber]->pt());
         genBottom1Eta_->Fill(topDaughts[bNumber]->eta());
         genW1Pt_->Fill(topDaughts[WNumber]->pt());
         genW1Eta_->Fill(topDaughts[WNumber]->eta());
         
         tempDeltaPhi = fabs(genMETVector.phi() - genTops[i].phi());
      if(tempDeltaPhi > TMath::Pi())
         tempDeltaPhi = 2*TMath::Pi() - tempDeltaPhi;
      genMetTop1DeltaPhi_->Fill(tempDeltaPhi);

         

      genBottomWDeltaRVsTopPt_->Fill(genTops[i].pt(), 
                                     deltaR(topDaughts[WNumber]->p4(), 
                                            topDaughts[bNumber]->p4()));
      
      genQ1Q2DeltaRVsWPt_->Fill(topDaughts[WNumber]->pt(), 
                                deltaR(wDaughts[q1Number]->p4(), 
                                       wDaughts[q2Number]->p4()));

      }
   }
   
   genSumPt_->Fill(genSumPt);
   genMET_->Fill(genMETVector.pt());


   tempDeltaPhi = fabs(genTops[0].phi() - genTops[1].phi());
   if(tempDeltaPhi > TMath::Pi())
      tempDeltaPhi = 2*TMath::Pi() - tempDeltaPhi;
   genTop0Top1DeltaPhi_->Fill(tempDeltaPhi);
   genTop0Top1DeltaPhiVsMET_->Fill(genMETVector.pt(), tempDeltaPhi);
   genTop0PtVsTop1Pt_->Fill(genTops[0].pt(), genTops[1].pt());

   //////////////
   //Reco-level//
   //////////////

   recoMET_->Fill(met->pt());

   for(unsigned i = 0; i < topTags.size(); i++)
   {
      const reco::CATopJetTagInfo * catopTag = dynamic_cast<reco::CATopJetTagInfo const *>(topTags[i].tagInfo(topTagName_));
      
      tempCount++;
      if(topTags[i].mass() < topMass_minCut_ || 
         topTags[i].mass() >topMass_maxCut_ || 
         topTags[i].numberOfDaughters() < topNSubjets_minCut_ || 
         catopTag->properties().minMass < topMinMass_minCut_) continue;
         
      if(deltaR(genTops[0].p4(), topTags[i].p4()) < topDeltaRMatchDef_) 
      {
         recoTop0Number = tempCount;
      }
      else if(deltaR(genTops[1].p4(), topTags[i].p4()) < topDeltaRMatchDef_) 
      {
         
         recoTop1Number = tempCount;
      }
      else
      {
         recoFakeTopPt_->Fill(topTags[i].pt());
         recoFakeTopEta_->Fill(topTags[i].pt());
      }
   }
   

   if(recoTop0Number < 1000)
   {
      recoTop0Pt_->Fill(topTags[recoTop0Number].pt());
      recoTop0Eta_->Fill(topTags[recoTop0Number].eta());
      recoTop0GenPt_->Fill(genTops[0].pt());
      recoTop0GenEta_->Fill(genTops[0].eta());

      tempDeltaPhi = fabs(met->phi() - topTags[recoTop0Number].phi());
      if(tempDeltaPhi > TMath::Pi())
         tempDeltaPhi = 2*TMath::Pi() - tempDeltaPhi;
      recoMetTop0DeltaPhi_->Fill(tempDeltaPhi);
   }

   if(recoTop1Number < 1000)
   {
      recoTop1Pt_->Fill(topTags[recoTop1Number].pt());
      recoTop1Eta_->Fill(topTags[recoTop1Number].eta());
      recoTop1GenPt_->Fill(genTops[1].pt());
      recoTop1GenEta_->Fill(genTops[1].eta());
      
      tempDeltaPhi = fabs(met->phi()  - topTags[recoTop1Number].phi());
      if(tempDeltaPhi > TMath::Pi())
         tempDeltaPhi = 2*TMath::Pi() - tempDeltaPhi;
      recoMetTop1DeltaPhi_->Fill(tempDeltaPhi);
   }
   if(recoTop0Number < 1000 && recoTop1Number < 1000)
   {

      tempDeltaPhi = fabs(topTags[recoTop0Number].phi() - topTags[recoTop1Number].phi());       
      if(tempDeltaPhi > TMath::Pi())
         tempDeltaPhi = 2*TMath::Pi() - tempDeltaPhi;
      recoTop0Top1DeltaPhi_->Fill(tempDeltaPhi);
   }

   
}

void 
SignalAnalyzer::beginJob()
{
}
void 
SignalAnalyzer::endJob() 
{
}

void 
SignalAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
void 
SignalAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
void 
SignalAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

void 
SignalAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

void
SignalAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {

  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(SignalAnalyzer);
