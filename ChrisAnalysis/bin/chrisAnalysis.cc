#include <iostream>
#include <sys/stat.h> 
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStopwatch.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "PFAnalyses/CommonTools/interface/FWLiteTreeAnalyzer.h"
#include "PFAnalyses/ChrisAnalysis/interface/ChrisAnalyzer.h"

int main(int argc, char ** argv){


  std::string cfgFileName = "ps.cfg";


  if(argc<2){
    std::cout<<"Usage: chrisAnalysis cfg.py"<<std::endl;
    return 1;
  }
  else cfgFileName = argv[1];

  std::cout<<"Start"<<std::endl;
  TStopwatch timer;
  timer.Start();
  //----------------------------------------------------------
  AutoLibraryLoader::enable();

  std::vector<FWLiteAnalyzer*> myAnalyzers;

  myAnalyzers.push_back(new ChrisAnalyzer("ChrisAnalyzer")); 
  
  FWLiteTreeAnalyzer *tree = new FWLiteTreeAnalyzer("TreeAnalyzer",cfgFileName);
  tree->init(myAnalyzers);
  int nEventsAnalyzed = tree->loop();
  tree->finalize();
  //----------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  printf("Analyzed events: %d \n",nEventsAnalyzed);
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
  printf("%4.2f events / RealTime second .\n", nEventsAnalyzed/rtime);
  printf("%4.2f events / CpuTime second .\n", nEventsAnalyzed/ctime);
  
  TFile* FP = new TFile("delRdelP.root", "CREATE","delRdelP");

  TH2D* PhoEff = new TH2D("PhoEff", "Photon Efficiency", 16, .05, .8, 16, .75, 12); 
  TH2D* PhoPure = new TH2D("PhoPure", "Photon 'Purity'", 16, .05, .8, 16, .75, 12); 
  TH2D* PhoQual = new TH2D("PhoQual", "Photon Quality" , 16, .05, .8, 16, .75, 12); 
  
  
  //TH1D* PFFake = (TH1D*)FP->Get("PFFake");
  //TH1D* KL0Fake = (TH1D*)FP->Get("KL0Fake");
  //TH1D* PionFake = (TH1D*)FP->Get("PionFake");
  //TH1D* EleFake = (TH1D*)FP->Get("EleFake");

  /*double PT = myAnalyzers[0]->getPt();
  PT = PT+.5;
  int pt = (int)PT;
  */
  double phoqual;
    
  for( int i = 0; i < 16; i++)
    {
      for( int j = 0; j <16; j++)
	{
	  phoqual = sqrt(myAnalyzers[0]->getPhotonEfficiency(i,j)*myAnalyzers[0]->getPhotonPurity( i, j));
	  PhoEff->SetBinContent(i, j,  myAnalyzers[0]->getPhotonEfficiency(i, j));
	  PhoPure->SetBinContent(i, j, myAnalyzers[0]->getPhotonPurity(i, j));
	  PhoQual->SetBinContent(i, j, phoqual);
	}
    }
  //PFFake->SetBinContent(pt/10, myAnalyzers[0]->getWrongPFMatchRate());
  //KL0Fake->SetBinContent(pt/10, myAnalyzers[0]->getKL0FakeRate());
  //PionFake->SetBinContent(pt/10, myAnalyzers[0]->getPionFakeRate());
  //EleFake->SetBinContent(pt/10, myAnalyzers[0]->getEleFakeRate());

  FP ->Write();
    
  delete tree;
      
  std::cout<<"Done"<<std::endl;
  return 0;
}






