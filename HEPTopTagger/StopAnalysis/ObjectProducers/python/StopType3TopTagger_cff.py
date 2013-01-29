from StopAnalysis.ObjectProducers.topTagger_cfi import *

type3topTagger = topTagger.clone()
type3topTagger.taggingMode = cms.untracked.bool(True)

stopType3TopTagger = cms.Sequence( type3topTagger )

type3topTaggerCut = topTagger.clone()
type3topTaggerCut.taggingMode = cms.untracked.bool(False)
stopType3TopTaggerCut = cms.Sequence(type3topTaggerCut)

from SandBox.Stop.filterDoubles_cfi import *

topMassCuttype3topTagger = doublesFilter.clone()
topMassCuttype3topTagger.ResultSource = cms.InputTag("type3topTagger:bestTopJetMass")
topMassCuttype3topTagger.lowCut = cms.double(80)
topMassCuttype3topTagger.highCut = cms.double(270)

mTbmTtCuttye3topTagger = doublesFilter.clone()
mTbmTtCuttye3topTagger.ResultSource = cms.InputTag("type3topTagger:linearCombmTbJetPlusmTbestTopJet")
mTbmTtCuttye3topTagger.lowCut = cms.double(500)

MT2Cuttye3topTagger = doublesFilter.clone()
MT2Cuttye3topTagger.ResultSource = cms.InputTag("type3topTagger:MT2")
MT2Cuttye3topTagger.lowCut = cms.double(300)

stopType3topTaggerAllCuts = cms.Sequence(
   topMassCuttype3topTagger *
   mTbmTtCuttye3topTagger *
   MT2Cuttye3topTagger
)
