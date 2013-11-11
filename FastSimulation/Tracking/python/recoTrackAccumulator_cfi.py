import FWCore.ParameterSet.Config as cms

trackAccumulator = cms.PSet(
    GeneralTrackInputSignal = cms.InputTag("generalTracksBeforeMixing"),
    GeneralTrackInputPileup = cms.InputTag("generalTracks"),
    GeneralTrackOutput = cms.string("generalTracks"),
    accumulatorType = cms.string("RecoTrackAccumulator"),
    makeDigiSimLinks = cms.untracked.bool(False)
    )
