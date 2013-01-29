import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/53X_ntuples/QCD_HT_250To500_MGPythia_v1_lpc1/seema/QCD_HT-250To500_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-250To500_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_68_1_AmQ.root",

])
