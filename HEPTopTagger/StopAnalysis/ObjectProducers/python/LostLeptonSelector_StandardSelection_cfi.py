import FWCore.ParameterSet.Config as cms

lostLeptons = cms.EDProducer(
	"LostLeptonSelector",
	genSrc = cms.InputTag("status3GenParticles"),
	electronSrc = cms.InputTag("patElectronsIDIso"),
	muonSrc = cms.InputTag("patMuonsPFIDIso"),
	tauSrc = cms.InputTag("selectedPatTausPF"),
	isolatedTrackSrc = cms.InputTag("isolatedTracks"),
   )
