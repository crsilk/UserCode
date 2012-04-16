#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <memory>
#include <sstream>
#include <stdlib.h>

using namespace edm;
using namespace std;
using namespace reco;


class ScanValidator : public edm::EDAnalyzer 
{
public:
  explicit ScanValidator(const edm::ParameterSet&);
  ~ScanValidator();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  vector<string> split(string fstring, string splitter);
  typedef std::vector<std::string>::const_iterator comments_const_iterator;
  typedef math::PtEtaPhiMLorentzVector PolarLorentzVector;
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  InputTag inputTagSource_;
  InputTag inputTagPFJets_;
  InputTag inputTagPFMET_;
  InputTag inputTagPFCandidates_;
  InputTag inputTagGenParticles_;
  InputTag inputTagGenJets_;
  InputTag inputTagPFTaus_;
  bool dileptonFilter_;
  bool getComment_;
  string modelTag_;
  double NEvents_;
  double leptonCut_;
  double tauCut_;
  double jetCut_;
  TH2F * NEvents_histo;
  TH2F * crossSection_histo;
  TH2F * Efficiency_histo;
  TH1F * GenHT_histo;
  TH1F * GenMET_histo;
  TH1F * GenNJets_histo;
  TH1F * GenJet1Pt_histo;
  TH1F * GenNElectrons_histo;
  TH1F * GenElectron1Pt_histo;
  TH1F * GenNMuons_histo;
  TH1F * GenMuon1Pt_histo;
  TH1F * HT_histo;
  TH1F * NJets_histo;
  TH1F * Jet1Pt_histo;
  TH1F * PFMET_histo;
  TH1F * NElectrons_histo;
  TH1F * Electron1Pt_histo;
  TH1F * NMuons_histo;
  TH1F * Muon1Pt_histo;
};


ScanValidator::ScanValidator(const edm::ParameterSet& iConfig)

{
   inputTagSource_  = iConfig.getParameter<InputTag>("source");
   inputTagPFJets_ =  iConfig.getParameter<InputTag>("pfjets");
   inputTagPFMET_ = iConfig.getParameter<InputTag>("pfmet");
   inputTagPFCandidates_ =  iConfig.getParameter<InputTag>("pfcandidates");
   inputTagGenParticles_ = iConfig.getParameter<InputTag>("genparticles");
   inputTagGenJets_ = iConfig.getParameter<InputTag>("genjets");
   inputTagPFTaus_ = iConfig.getParameter<InputTag>("pftaus");
   dileptonFilter_ = iConfig.getParameter<bool>("dileptonFilter");
   getComment_ = iConfig.getParameter<bool>("getcomment");
   modelTag_ = iConfig.getParameter<string>("modelTag");
   NEvents_ = iConfig.getParameter<int>("nevents");
   leptonCut_ = iConfig.getParameter<double>("leptonCut");
   tauCut_ = iConfig.getParameter<double>("tauCut");
   jetCut_ = iConfig.getParameter<double>("jetCut");

   Service<TFileService> rootFile;
   NEvents_histo = rootFile->make<TH2F>("NEvents_histo", "NEvents", 150, 0, 3000, 50, 0, 1000);
   crossSection_histo = rootFile->make<TH2F>("crossSection_histo", "Cross Section", 150, 0, 3000, 50, 0, 1000);
   Efficiency_histo =rootFile->make<TH2F>("Efficiency_histo", "Efficiency", 150, 0, 3000, 50, 0, 1000);
   GenHT_histo = rootFile->make<TH1F>("GenHT_histo", "Gen HT", 300, 0, 3000);
   GenMET_histo = rootFile->make<TH1F>("GenMET_histo", "Gen MET", 300, 0, 3000);
   GenNJets_histo = rootFile->make<TH1F>("GenNJets_histo", "Gen NJets", 50, 0, 50);
   GenJet1Pt_histo = rootFile->make<TH1F>("GenJet1Pt_histo", "1st Gen jet pt", 200, 0, 2000);
   GenNElectrons_histo =rootFile->make<TH1F>("GenNElectrons_histo", "Gen NElectrons", 20, 0, 20);
   GenNMuons_histo =rootFile->make<TH1F>("GenNMuons_histo", "Gen NMuons", 20, 0, 20);
   GenElectron1Pt_histo = rootFile->make<TH1F>("GenElectron1Pt_histo", "1st Gen electron pt", 200, 0, 2000);
   GenMuon1Pt_histo = rootFile->make<TH1F>("GenMuon1Pt_histo", "1st Gen muon pt", 200, 0, 2000);
   HT_histo = rootFile->make<TH1F>("HT_histo", "HT", 300, 0, 3000);
   PFMET_histo = rootFile->make<TH1F>("PFMET_histo", "MET", 300, 0, 3000);
   NJets_histo = rootFile->make<TH1F>("NJets_histo", "NJets", 50, 0, 50);
   Jet1Pt_histo = rootFile->make<TH1F>("Jet1Pt_histo", "1st jet pt", 200, 0, 2000);
   NElectrons_histo =rootFile->make<TH1F>("NElectrons_histo", "NElectrons", 20, 0, 20);
   Electron1Pt_histo = rootFile->make<TH1F>("Electron1Pt_histo", "1st electron pt", 200, 0, 2000);
   NMuons_histo =rootFile->make<TH1F>("NMuons_histo", "NMuons", 20, 0, 20);
   Muon1Pt_histo = rootFile->make<TH1F>("Muon1Pt_histo", "1st muon pt", 200, 0, 2000);
}


ScanValidator::~ScanValidator()
{
 
}


vector<string> ScanValidator::split(string fstring, string splitter)
{
   vector<string> returnVector;
   size_t cursor;
   string beforeSplitter;
   string afterSplitter = fstring;
   if(fstring.find(splitter) == string::npos)
   {
      cout<<"No "<<splitter<<" found"<<endl;
      returnVector.push_back(fstring);      
      return returnVector;
   }
   else
   {
      while(afterSplitter.find(splitter) != string::npos)
      {
         cursor = afterSplitter.find(splitter);

         beforeSplitter = afterSplitter.substr(0, cursor);
         afterSplitter = afterSplitter.substr(cursor +1, afterSplitter.size());
         
         returnVector.push_back(beforeSplitter);
         
         if(afterSplitter.find(splitter) == string::npos)
            returnVector.push_back(afterSplitter);
      }
      return returnVector;
   }
}


void
ScanValidator::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  Handle<LHEEventProduct> product;
  comments_const_iterator comment;
  
  Handle<PFJetCollection> pfjets;
  iEvent.getByLabel(inputTagPFJets_, pfjets);
  PFJetCollection::const_iterator pfjet;

  Handle<PFMETCollection> pfmets;
  iEvent.getByLabel(inputTagPFMET_, pfmets);
  PFMETCollection::const_iterator pfmet = pfmets->begin();

  Handle<PFCandidateCollection> pfcandidates;
  iEvent.getByLabel(inputTagPFCandidates_, pfcandidates);
  PFCandidateCollection::const_iterator pfcandidate;

  Handle<GenParticleCollection> genparticles;
  iEvent.getByLabel(inputTagGenParticles_, genparticles);
  GenParticleCollection::const_iterator genparticle;

  Handle<GenJetCollection> genjets;
  iEvent.getByLabel(inputTagGenJets_, genjets);
  GenJetCollection::const_iterator genjet;

  Handle<PFTauCollection> pftaus;
  iEvent.getByLabel(inputTagPFTaus_, pftaus);
  PFTauCollection::const_iterator pftau;


  Particle::PolarLorentzVector pfcandP4 = PolarLorentzVector(0,0,0,0);
  Particle::PolarLorentzVector genparticleP4 = PolarLorentzVector(0,0,0,0);  
  string tempString;
  vector<string> parameters;
  vector<string> strings;
  double param1;
  double param2;
  double crossSection;
  double efficiency;
  bool commentIsThere = false;
  double genHT = 0;
  double maxGenJetPt = 0;
  int nGenElectrons = 0;
  int nGenMuons = 0;
  int nGenJets = 0;
  double maxGenElectronPt = 0;
  double maxGenMuonPt = 0;
  double maxJetPt = 0;
  double maxElectronPt = 0;
  double nElectrons = 0;
  double maxMuonPt = 0;
  double nMuons = 0;
  double nTaus = 0;

  for(genparticle = genparticles->begin(); 
      genparticle != genparticles->end(); genparticle++)
  {
     if( genparticle->status() == 1)
     {
        if(abs(genparticle->pdgId()) != 12 && 
           abs(genparticle->pdgId()) != 14 && 
           abs(genparticle->pdgId()) != 16 &&
           abs(genparticle->pdgId()) != 1000022) 
           genparticleP4 = genparticleP4 + genparticle->p4();

        if(abs(genparticle->pdgId()) == 11)
        {
           nGenElectrons++;
           if(genparticle->pt() > maxGenElectronPt)
              maxGenElectronPt = genparticle->pt();
        }
        if(abs(genparticle->pdgId()) == 13)
        {
           nGenMuons++;
           if(genparticle->pt() > maxGenMuonPt)
              maxGenMuonPt = genparticle->pt();
        }
     }
  }
      
  for(genjet = genjets->begin(); 
      genjet != genjets->end(); genjet++)
  {
     nGenJets++;
     genHT = genHT + genjet->pt();
     if(genjet->pt() > maxGenJetPt)
        maxGenJetPt = genjet->pt();
  }

  for (pfcandidate = pfcandidates->begin(); pfcandidate != pfcandidates->end();
       pfcandidate++)
    {      
      pfcandP4 = pfcandP4 + pfcandidate->p4();

	  if (pfcandidate->pt() < leptonCut_) continue;

	  if(abs(pfcandidate->pdgId()) ==11)
      {
         nElectrons++;
         if(pfcandidate->pt() > maxElectronPt)
            maxElectronPt = pfcandidate->pt();
      }

	  if(abs(pfcandidate->pdgId()) ==11)
      {
         nMuons++;
         if(pfcandidate->pt() > maxMuonPt)
            maxMuonPt = pfcandidate->pt();
      }
    }

  for( pftau = pftaus->begin(); pftau != pftaus->end();pftau++)
    {
      if( pftau->pt() < tauCut_) continue;
      
      nTaus++;
    }
  

  if(dileptonFilter_ && nGenMuons + nGenElectrons < 2) return;
  
  if(getComment_)
    {
      iEvent.getByLabel(inputTagSource_, product);
      for(comment = product->comments_begin(); 
	  comment != product->comments_end(); comment++)
	{
	  if(comment->find(modelTag_) != string::npos)
	    {
	      commentIsThere =true;
	      tempString = comment->substr(comment->find(modelTag_),
					   comment->size());
	      tempString = tempString.substr(0, tempString.find(" "));
	      parameters = split(tempString, "_");
	      param1 = atof(parameters[1].c_str());
	      param2 = atof(parameters[2].c_str());
	      
	      strings = split(comment->substr(0, comment->size()), " ");
	      crossSection = atof(strings[4].c_str());

	      tempString = comment->substr(comment->find_last_of(" "),
					   comment->find("\n"));
//	      efficiency = atof(tempString.c_str());
	      
	      NEvents_histo->Fill( param1, param2);
//	      Efficiency_histo->Fill( param1, param2, efficiency/NEvents_);
	      crossSection_histo->Fill(param1, param2, crossSection);
	    }
	}
    }
  
  if(getComment_ && !commentIsThere) return;
  
  double ht = 0;
  double njets = 0;
  for (pfjet = pfjets->begin(); pfjet != pfjets->end(); pfjet++)
    {
       ht = ht + pfjet->et();
       if (pfjet->pt() < jetCut_)
          continue;
       if(pfjet->pt() > maxJetPt)
          maxJetPt = pfjet->pt();
       njets++;

    }
  
  GenHT_histo->Fill(genHT);
  GenMET_histo->Fill(genparticleP4.pt());

  GenNJets_histo->Fill(nGenJets);
  if (nGenJets != 0)
     GenJet1Pt_histo->Fill(maxGenJetPt);

  GenNElectrons_histo->Fill(nGenElectrons);
  if (nGenElectrons != 0)
     GenElectron1Pt_histo->Fill(maxGenElectronPt);
  GenNMuons_histo->Fill(nGenMuons);
  if (nGenMuons != 0)
     GenMuon1Pt_histo->Fill(maxGenMuonPt);



  HT_histo->Fill(ht);
  NJets_histo->Fill(njets);
  NElectrons_histo->Fill(nElectrons);
  NMuons_histo->Fill(nMuons);
  if (njets != 0)
     Jet1Pt_histo->Fill(maxJetPt);
  if( nElectrons != 0.0)
    Electron1Pt_histo->Fill(maxElectronPt);
  if( nMuons != 0.0)
    Muon1Pt_histo->Fill(maxMuonPt);
  
  PFMET_histo->Fill(pfmet->pt());
  

}


// ------------ method called once each job just before starting event loop  ------------
void 
ScanValidator::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ScanValidator::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
ScanValidator::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
ScanValidator::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
ScanValidator::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
ScanValidator::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ScanValidator::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ScanValidator);
