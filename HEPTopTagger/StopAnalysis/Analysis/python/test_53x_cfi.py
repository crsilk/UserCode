import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/53X_ntuples/ZJetsToNuNu_400_HT_inf_ext_TuneZ2Star_8TeV_madgraph_Summer12/dhare/ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph_ext/Summer12_DR53X-PU_S10_V7A-v1_NOCUTS_12Oct2012V3/b9d339f81100b66394e7e5c0a998fe80/susypat_11_1_OxM.root",
])