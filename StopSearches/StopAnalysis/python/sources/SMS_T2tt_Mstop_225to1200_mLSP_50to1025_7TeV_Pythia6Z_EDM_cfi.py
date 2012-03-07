import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_10_1_aa1.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_11_1_Ale.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_1_1_bmg.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_12_1_30N.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_13_1_UYk.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_14_1_sjL.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_15_1_MAM.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_16_1_xAn.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_17_1_14J.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_18_1_sVB.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_19_1_t4S.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_20_1_WiB.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_21_1_8U1.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_2_1_ArW.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_22_1_XsA.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_23_1_ZMO.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_24_1_xCX.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_25_1_fby.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_26_1_ywU.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_27_1_6VG.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_28_1_VUY.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_29_1_qzt.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_30_1_t9V.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_31_1_aut.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_3_1_vkn.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_32_1_iDz.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_33_1_zjg.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_34_1_Lfl.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_35_1_KAH.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_36_1_BT1.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_37_1_sBj.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_38_1_Xxc.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_39_1_HKx.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_40_1_KHO.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_41_1_yZD.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_4_1_RWb.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_42_1_d6W.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_43_1_JPp.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_44_1_bqd.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_45_1_hsU.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_46_1_2di.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_47_1_d8w.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_48_1_7n5.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_49_1_Gaw.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_50_1_xdX.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_51_1_2PH.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_5_1_eme.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_6_1_VNO.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_7_1_iZd.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_8_1_TZB.root",
"dcap:///pnfs/cms/WAX/resilient/csilkw2/StopSearches/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_ultraslim_9_1_Q3Y.root"	])
