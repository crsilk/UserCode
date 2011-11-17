import FWCore.ParameterSet.Config as cms

process = cms.Process("GetTracks")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
	'file:/uscmst1b_scratch/lpc1/3DayLifetime/crsilk/temp/0007195C-0F7E-E011-BD2B-002618943986.root'
	
    )
)

process.demo = cms.EDAnalyzer('GetTracks',
							  PFJets = cms.InputTag("ak5PFJets")
)


process.p = cms.Path(process.demo)
