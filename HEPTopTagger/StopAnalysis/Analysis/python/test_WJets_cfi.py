import FWCore.ParameterSet.Config as cms
readFiles = cms.untracked.vstring(
	'dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/53X_ntuples/kasmi/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3/0c2ca37b7b411866265a5953adbfd963/susypat_340_1_DKm.root'
)
source = cms.Source("PoolSource",
    fileNames = readFiles
)
