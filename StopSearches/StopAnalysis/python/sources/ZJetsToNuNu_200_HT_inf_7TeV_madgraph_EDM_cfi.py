import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
	"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_0.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_10.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_11.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_12.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_13.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_14.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_15.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_16.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_17.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_18.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_19.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_1.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_20.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_21.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_22.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_23.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_24.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_25.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_26.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_27.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_28.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_29.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_2.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_30.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_31.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_32.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_33.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_34.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_35.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_36.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_37.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_38.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_39.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_3.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_40.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_4.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_5.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_6.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_7.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_8.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_9.root"
])
