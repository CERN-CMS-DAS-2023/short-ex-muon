import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonExercise2")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(0)
)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False),
SkipEvent = cms.untracked.vstring('ProductNotFound'))

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('file:/eos/user/c/cmsdas/2023/short-ex-muon/dymm.root'))

process.demo = cms.EDAnalyzer("MuonExercise2",
                                       MuonTag     = cms.InputTag("slimmedMuons"),
                                       GenPartTag  = cms.InputTag("prunedGenParticles"),
                                       UseRochCorr = cms.untracked.bool(False),
                                       RndmSeed    = cms.untracked.uint32(2345)
)

process.TFileService = cms.Service("TFileService",
          fileName = cms.string('histos2.root')
)

process.p = cms.Path(process.demo)
