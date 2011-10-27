import FWCore.ParameterSet.Config as cms

process = cms.Process("DeltaPhiAnalyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("UserCode.DeltaPhiAnalyzer.deltaphianalyzer_cfi")



readFiles = cms.untracked.vstring()
readFiles.extend( ['/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/00336BCD-7280-E011-BEF0-0018F3D096E8.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/20587715-7580-E011-B5B6-001A92971B94.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/2468F16B-7280-E011-A3E4-00304867BEC0.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/300217DE-8480-E011-8C00-00304867918A.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/30A02781-7480-E011-BEC0-0030486792BA.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/32877EA4-7080-E011-A01F-0018F3D096FE.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/34CA7190-6F80-E011-ADFE-0026189438B5.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/3C32910F-7580-E011-B4EB-0018F3D0970A.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/12E98B14-7080-E011-818A-001A92811706.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/525C1F85-7C80-E011-B292-0018F3D096D4.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/72D35B47-7380-E011-9FFA-002354EF3BE2.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/748EA7BC-7280-E011-94AE-001A92810AE4.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/78469E91-7780-E011-AB06-001BFCDBD1BE.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/784F0674-6F80-E011-910B-002618943957.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/7A5207BB-7280-E011-AE6C-001A928116DA.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/7AC6F90B-7580-E011-9353-0026189438C0.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/7C1E2CAB-7180-E011-86A7-001A92810AA8.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/7C56D2E0-7180-E011-900C-0026189438D6.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/7EA7AEE8-7180-E011-9004-0018F3D09704.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/2463A6B0-7380-E011-976A-0018F3D095EA.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/8A81B8BE-7480-E011-8382-002354EF3BDA.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/92E4CFC2-7080-E011-84CA-0018F3D09608.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/3EBB7BBD-9280-E011-9D4C-001A92971B20.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/9CEB17A3-7380-E011-924B-0018F3D09604.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/9E46D196-7380-E011-B189-0026189438E1.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/AA4AB09B-7580-E011-B194-002618943916.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/AECF2C90-7180-E011-9241-00261894380A.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/B250746D-7280-E011-B403-002618943829.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/B2ABCBE1-7380-E011-9F9C-00261894393B.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/B4B0290F-7380-E011-98F7-001731EF61B4.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/B4ED479D-7580-E011-A9BC-002618943856.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/40393D77-6F80-E011-A358-001A92971B74.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/BE03A335-7680-E011-9F70-001A92810ADE.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/C0E6D1A1-7380-E011-B614-001A92810ADE.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/461F2EC8-9180-E011-BAF2-002354EF3BDA.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/CAE2DC6E-7280-E011-BA9A-0026189438EB.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/CCED9777-7480-E011-AE52-002618943896.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/86ABE1EF-7180-E011-9ED8-0018F3D095EE.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/D85CF4A3-7580-E011-96D5-0030486792BA.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/94C1873B-7A80-E011-8671-00261894385A.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/DC86F0BE-7680-E011-B795-002618943856.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/E0C5964E-7180-E011-8982-002618943959.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/E63BFDE1-7380-E011-9B9A-002618943977.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/B8B7D4C5-7480-E011-A025-002618943896.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/E8D6A097-7380-E011-B891-001A9281172E.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/EC7DD0E9-7380-E011-B975-001A92971B38.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/EE5BE14B-7180-E011-89A0-002618943894.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/F29982F8-7780-E011-A700-0018F3D096FE.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/F40BA61B-9A80-E011-B369-00261894385A.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/FAA51310-A780-E011-8D94-00304867915A.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/F8D575A7-7080-E011-B80D-001A92971B7E.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/FC6570B2-7280-E011-B434-0026189438E1.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/FE20479B-7380-E011-9AF8-00304867920C.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/FE31149A-6E80-E011-A69A-0026189438EA.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/C8D6B206-8680-E011-99C1-001A92971B90.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/D29BA6E2-7380-E011-9D7D-00261894385A.root',
       '/store/mc/Summer11/QCDContact_Pt-15to3000_L-2000_TuneD6T_Flat_7TeV-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/DC75ACCF-7480-E011-81BE-0018F3D0965A.root' ])




process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = readFiles
#	 fileNames = cms.untracked.vstring("file:/uscmst1b_scratch/lpc1/3DayLifetime/crsilk/temp/06037E03-45CB-E011-A6F5-00261894384A.root")
)

#process.load("CMGTools.Common.sources.SMS_T2tt_Mstop_225to1200_mLSP_50to1025_7TeV_Pythia6Z.Summer11_PU_START42_V11_FastSim_v1.AODSIM.source_cff")



process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.TFileService = cms.Service("TFileService",
	fileName = cms.string("deltaphianalyzer_QCD.root")

)
process.p = cms.Path(process.deltaPhiAnalyzer)
