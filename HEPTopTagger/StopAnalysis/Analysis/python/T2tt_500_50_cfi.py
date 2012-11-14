import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/lacroix/2012SEP05/T2tt/lacroix/T2tt_mStop500_mLSP50_NOCUTS_09Aug2012V1/susypat_T2tt_500_50_job0.root",
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/lacroix/2012SEP05/T2tt/lacroix/T2tt_mStop500_mLSP50_NOCUTS_09Aug2012V1/susypat_T2tt_500_50_job1.root",
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/lacroix/2012SEP05/T2tt/lacroix/T2tt_mStop500_mLSP50_NOCUTS_09Aug2012V1/susypat_T2tt_500_50_job2.root",
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/lacroix/2012SEP05/T2tt/lacroix/T2tt_mStop500_mLSP50_NOCUTS_09Aug2012V1/susypat_T2tt_500_50_job3.root",
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/lacroix/2012SEP05/T2tt/lacroix/T2tt_mStop500_mLSP50_NOCUTS_09Aug2012V1/susypat_T2tt_500_50_job4.root",
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/lacroix/2012SEP05/T2tt/lacroix/T2tt_mStop500_mLSP50_NOCUTS_09Aug2012V1/susypat_T2tt_500_50_job5.root",
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/lacroix/2012SEP05/T2tt/lacroix/T2tt_mStop500_mLSP50_NOCUTS_09Aug2012V1/susypat_T2tt_500_50_job6.root",
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/lacroix/2012SEP05/T2tt/lacroix/T2tt_mStop500_mLSP50_NOCUTS_09Aug2012V1/susypat_T2tt_500_50_job7.root",
"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/lacroix/2012SEP05/T2tt/lacroix/T2tt_mStop500_mLSP50_NOCUTS_09Aug2012V1/susypat_T2tt_500_50_job8.root",
])