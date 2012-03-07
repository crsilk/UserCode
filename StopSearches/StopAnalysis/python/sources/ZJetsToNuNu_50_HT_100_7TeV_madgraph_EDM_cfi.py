import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [




	"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_10_1_C0F.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_11_1_YID.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_1_1_IKr.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_12_1_BRE.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_13_1_okq.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_14_1_iCY.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_15_1_cnv.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_16_1_DrL.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_17_1_aOO.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_18_1_Uv8.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_19_1_VVF.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_20_1_7YS.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_21_1_kxR.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_2_1_AMa.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_22_1_WtN.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_23_1_m9B.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_24_1_niT.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_25_1_XKM.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_26_1_yKZ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_27_1_iiP.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_28_1_yUx.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_29_1_kGZ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_30_1_shq.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_31_1_lCq.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_3_1_ToC.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_32_1_TCD.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_33_1_8m2.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_34_1_k5S.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_35_1_qv8.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_36_1_fVy.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_37_1_6UU.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_38_1_amQ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_39_1_9eP.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_40_1_gJm.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_41_1_03t.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_4_1_HMR.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_42_1_Zag.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_43_1_Twr.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_44_1_Xwe.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_45_1_N0W.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_46_1_ebL.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_47_1_WeR.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_48_1_6s7.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_49_1_c7P.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_50_1_wjf.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_51_1_wwN.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_5_1_TIQ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_6_1_7zI.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_7_1_d2g.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_8_1_BYw.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_50_HT_100_7TeV-madgraph/ttbsm_ultraslim_9_1_rfm.root"

	])
