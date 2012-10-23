from RecoJets.JetProducers.kt4PFJets_cfi import *
from StopAnalysis.ObjectProducers.ElectronSelector_VetoElectrons_cfi import *
#from StopAnalysis.ObjectProducers.MuonSelector_VetoMuons_cfi import *

kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

stopElectrons = cms.Sequence(
	kt6PFJetsForIsolation *
	vetoElectrons
	)


