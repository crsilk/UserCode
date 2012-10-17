readFiles = cms.untracked.vstring(
	'/store/user/lpcsusyhad/crsilk/TTJets_TuneZ2star_8TeV-madgraph-tauola/SUSYPAT_20_1_nXk.root'
)
process.source = cms.Source("PoolSource",
    fileNames = readFiles
)
