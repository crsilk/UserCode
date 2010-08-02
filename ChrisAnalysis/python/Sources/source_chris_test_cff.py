
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	#fileNames = cms.untracked.vstring('file:~/public/CMSSW_3_5_8/src/PFAnalyses/CommonTools/prod/AODIntegrationTest.root')
        fileNames = cms.untracked.vstring('file:~/scratch0/CMSSW_3_6_1_patch2/src/PhysicsTools/PatAlgos/test/SingleGammaPt1000_PF2PAT.root')
        
)
