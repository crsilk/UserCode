import FWCore.ParameterSet.Config as cms
readFiles = cms.untracked.vstring(
	'dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/QCD_Pt_15to3000_TuneZ2_Flat_8TeV_pythia6_Summer12/samantha/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/Summer12_PU_S7_START52_V9_RA2_NoCuts_09Aug2012V1/f42b6bbcc7ea08f6809cc21efa861dac/susypat_513_2_o9k.root'
)
source = cms.Source("PoolSource",
    fileNames = readFiles
)
