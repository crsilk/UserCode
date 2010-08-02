// system include files
#include "TProofOutputFile.h"
#include "TFile.h"
#include "TTree.h"
#include "boost/shared_ptr.hpp"

// user include files
#include "PFAnalyses/ChrisAnalysis/interface/FWLiteTSelector.h"
#include "PFAnalyses/CommonTools/interface/FWLiteTreeAnalyzer.h"
#include "PFAnalyses/ChrisAnalysis/interface/ChrisAnalyzer.h"


#include "TSystem.h"
#include "TH1F.h"
#include "THashList.h"
#include "TDirectory.h"
#include "TList.h"
#include "TCollection.h"
#include "TROOT.h"
#include "TFileMerger.h"

namespace pfAnalyses{
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
FWLiteTSelector::FWLiteTSelector() : 
  everythingOK_(false){ }
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
FWLiteTSelector::~FWLiteTSelector(){ }
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void FWLiteTSelector::Begin(TTree * iTree) {   Init(iTree); }
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void FWLiteTSelector::SlaveBegin(TTree *iTree) { 

  Init(iTree);

  fProofFile_ = new TProofOutputFile("SimpleNtuple.root","LOCAL");


  std::vector<FWLiteAnalyzer*> myAnalyzers;

  myAnalyzers.push_back(new ChrisAnalyzer("ChrisAnalyzer"));
 

  TString option = GetOption();

  std::string cfgFileName_(option.Data());
  fwLiteTreeAnalyzer_ =  new FWLiteTreeAnalyzer("TreeAnalyzer",cfgFileName_,fProofFile_);
  fwLiteTreeAnalyzer_->init(myAnalyzers);

  sampleName_ = fwLiteTreeAnalyzer_->getSampleName();

  lastEntry_ = 0;  
  tmp_ = 0;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void FWLiteTSelector::Init(TTree *iTree) { 
  if(iTree==0) return;
  tree_ = iTree;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Bool_t FWLiteTSelector::Notify() { 

  TFile* file = tree_->GetCurrentFile();
  setupNewFile(*file);
 lastEntry_ = 0;  
  return everythingOK_ ? kTRUE: kFALSE; 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Bool_t FWLiteTSelector::Process(Long64_t iEntry) { 

  for(int i=lastEntry_;i<iEntry;++i) ++(*event_);

  //fwLiteTreeAnalyzer_->analyze(const_cast<fwlite::ChainEvent&>(event_->to(iEntry)));
  fwLiteTreeAnalyzer_->analyze(const_cast<fwlite::ChainEvent&>(*event_));

  lastEntry_ = iEntry;

  return kTRUE; 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void FWLiteTSelector::SlaveTerminate() { 

  std::cout<<"FWLiteTSelector::SlaveTerminate()"<<std::endl;
  fwLiteTreeAnalyzer_->finalize();
  delete fwLiteTreeAnalyzer_;
  //Write the ntuple to the file
  fOutput->Add(fProofFile_);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void FWLiteTSelector::Terminate() {

  std::cout<<"FWLiteTSelector::Terminate()"<<std::endl;
  std::cout<<"Final normalisation of the histograms in the Summary directory"<<std::endl;
  
  TProofOutputFile *proofFile = (TProofOutputFile*)fOutput->FindObject("SimpleNtuple.root");
  fileName_ = proofFile->GetOutputFileName();
  //gROOT->GetListOfFiles()->Print();
  //gSystem->Exec("ls -ltr; pwd");
  if(fileName_.size()){
    TFile *file = new TFile(fileName_.c_str(),"UPDATE");
    file->Recover();
    TH1F *hStats = (TH1F*)file->Get("Statistics/hStats");
    int nNodes = (int)hStats->GetBinContent(4);
    float crossSection = hStats->GetBinContent(1)/nNodes;
    float presEff = hStats->GetBinContent(2)/nNodes;
    int nEventsAnalyzed = (int)hStats->GetBinContent(5);
    float weight = crossSection*presEff/nEventsAnalyzed;
    ////////
    /*
    std::cout<<"Number of nodes used: "<<nNodes<<std::endl;
    std::cout<<"Mean cross section: "<<crossSection<<" [pb] "<<std::endl;
    std::cout<<"Number of events analyzed: "<<nEventsAnalyzed<<std::endl;
    std::cout<<"Preselection efficiency: "<<presEff<<std::endl;
    std::cout<<"Event weight: sigma*presEff/nTotal: "<<weight<<std::endl;
    */
    ////////
    if(weight>0){
      TDirectory *summary = (TDirectory*)file->Get("Summary");
      TList *list = summary->GetList();
      TIter next(list);
      TObject *obj = 0;
      while ((obj = next())){
	if(obj->IsA()->InheritsFrom("TH1")){ 
	  TH1 *h = (TH1*)summary->Get(obj->GetName());
	  h->Scale(weight);
	}
	if(obj->IsA()->InheritsFrom("TDirectory")){ 
	  TDirectory* aDir = (TDirectory*)summary->Get(obj->GetName());
	  TList *listSubDir = aDir->GetList();
	  TIter next2(listSubDir);
	  TObject *obj2 = 0;
	  while ((obj2 = next2())){
	    if(obj2->IsA()->InheritsFrom("TH1")){ 
	      TH1 *h1 = (TH1*)aDir->Get(obj2->GetName());
	      h1->Scale(weight);
	    }
	  }
	}
      }
    }
    //summary->Write("",TObject::kOverwrite);
    file->Write("",TObject::kOverwrite);
    file->Close();
    delete file;
  }  
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void FWLiteTSelector::setupNewFile(TFile& iFile) { 

  std::vector<std::string> fileVector;
  fileVector.push_back(iFile.GetName());

  std::cout<<"fileName: "<<iFile.GetName()<<std::endl;
  event_ = boost::shared_ptr<fwlite::ChainEvent>( new fwlite::ChainEvent(fileVector));
  std::cout<<"size: "<<event_->size()<<std::endl;
  everythingOK_ = true;

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
}

