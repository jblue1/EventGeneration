import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# File service
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName=cms.string("out.root")

# load filelist
fileList = FileUtils.loadListFromFile("filelist.txt")
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(*fileList)
)

# since different data gen runs have same event numbers
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.demo = cms.EDAnalyzer('MyAnalyzer',
    ak4PFJets=cms.InputTag("ak4PFJets"),
    ak4PFJetsCHS=cms.InputTag("ak4PFJetsCHS"),
    ak4GenJets=cms.InputTag("ak4GenJets"),
    genParticles=cms.InputTag("genParticles"))

process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# how many events to process (-1 = all)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10000))


process.p = cms.Path(process.demo)
