{

   vector<string> fileNames;
   vector<string> histoNames;
   vector<string> legendNames;
   

   fileNames.push_back("../data/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6_METTop.root"); 
   fileNames.push_back("../data/TTJets_TuneZ2_7TeV_madgraph_tauola_METTop.root");
   fileNames.push_back("../data/WJetsToLNu_TuneZ2_7TeV_madgraph-tauola_METTop.root");
   fileNames.push_back("../data/ZJetsToNuNu_100_HT_200_7TeV_madgraph_METTop.root");
   fileNames.push_back("../data/ZJetsToNuNu_200_HT_inf_7TeV_madgraph_METTop.root");
   fileNames.push_back("../data/ZJetsToNuNu_50_HT_100_7TeV_madgraph_METTops.root");

   histoNames.push_back("metTopAnalyzer/METTopCuts");
   histoNames.push_back("metTopAnalyzer/METAndTop0Mt");
   histoNames.push_back("metTopAnalyzer/METAndTop1Mt");
   histoNames.push_back("metTopAnalyzer/METAndTop0DeltaPhi");
   histoNames.push_back("metTopAnalyzer/METAndTop1DeltaPhi");
   histoNames.push_back("metTopAnalyzer/topCandMass");
   histoNames.push_back("metTopAnalyzer/topCandMinMass");
   histoNames.push_back("metTopAnalyzer/topCandPt");
   histoNames.push_back("metTopAnalyzer/topCandEta");
   histoNames.push_back("metTopAnalyzer/top0Mass");
   histoNames.push_back("metTopAnalyzer/top0MinMass");
   histoNames.push_back("metTopAnalyzer/top0Pt");
   histoNames.push_back("metTopAnalyzer/top0Eta");
   histoNames.push_back("metTopAnalyzer/top1Mass");
   histoNames.push_back("metTopAnalyzer/top1MinMass");
   histoNames.push_back("metTopAnalyzer/top1Eta");
   histoNames.push_back("metTopAnalyzer/top1Pt");
 

   legendNames.push_back("QCD_Pt_15to3000");
   legendNames.push_back("TTJets"); 
   legendNames.push_back("WJetsToLNu");
   legendNames.push_back("ZJetsToNuNu_100_HT_200");
   legendNames.push_back("ZJetsToNuNu_200_HT_inf");
   legendNames.push_back("ZJetsToNuNu_50_HT_100");

   stackHistos(fileNames, histoNames, legendNames, "Backgrounds", false);
   
}
