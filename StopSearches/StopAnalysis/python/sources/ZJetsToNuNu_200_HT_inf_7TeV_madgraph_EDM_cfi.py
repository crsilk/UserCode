import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_33_1_i52.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_39_1_1DJ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_32_1_qat.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_19_1_ysL.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_44_1_iQm.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_34_1_onL.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_5_1_jx4.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_15_1_jJG.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_51_1_9oY.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_12_1_P3v.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_1_1_ifz.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_46_1_zrz.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_35_1_KhF.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_22_1_eNa.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_23_1_ZDT.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_24_1_BGp.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_48_1_5RP.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_50_1_R84.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_43_1_Eee.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_37_1_xeo.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_10_1_Cz6.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_49_1_nZz.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_27_1_gos.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_8_1_FbS.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_4_1_67p.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_36_1_PKQ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_14_1_Pox.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_30_1_hFT.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_17_1_Un5.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_9_1_wt6.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_13_1_DuQ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_16_1_AXs.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_3_1_irL.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_25_1_gwX.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_47_1_2iH.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_29_1_jJm.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_18_1_pkN.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_45_1_OhU.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_2_1_4C3.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_20_1_Qld.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_41_1_FrD.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_28_1_jMK.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_38_1_Pdq.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_31_1_uLU.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_40_1_kgY.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_6_1_hfV.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_21_1_vZJ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_26_1_7wP.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_11_1_ASw.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_42_1_WkH.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_ultraslim_7_1_5wo.root",
])