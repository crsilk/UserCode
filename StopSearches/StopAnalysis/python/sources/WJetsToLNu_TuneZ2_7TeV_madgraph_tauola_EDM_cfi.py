import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_10_1_kur.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_11_1_X4j.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_1_1_Cn6.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_12_1_B56.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_13_1_g8A.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_14_1_EFc.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_15_1_Kl7.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_16_1_mhx.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_17_1_ZuV.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_18_1_VN7.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_19_1_7s7.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_20_1_CSm.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_21_1_bhY.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_2_1_OQQ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_22_1_w94.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_23_1_EOd.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_24_1_PtU.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_25_1_ZTw.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_26_1_r7C.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_27_1_vtD.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_28_1_4CG.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_29_1_zj0.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_30_1_Vik.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_31_1_2pD.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_3_1_j2Z.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_32_1_Y9F.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_33_1_cJ1.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_34_1_A2Y.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_35_1_pT6.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_36_1_MbK.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_37_1_tTX.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_38_1_loK.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_39_1_Qk4.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_40_1_YMV.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_41_1_qJR.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_4_1_XIQ.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_42_1_aeG.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_43_1_7Ua.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_44_1_kr8.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_45_1_LNz.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_46_1_sk0.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_47_1_nbk.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_48_1_34n.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_49_1_5RA.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_50_1_Tkq.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_51_1_rl2.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_5_1_Y9Y.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_6_1_SzL.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_7_1_m12.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_8_1_GxI.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/ttbsm_ultraslim_9_1_TYs.root"
	])
