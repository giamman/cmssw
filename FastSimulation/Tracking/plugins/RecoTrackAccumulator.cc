#include "RecoTrackAccumulator.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/one/EDProducer.h"

RecoTrackAccumulator::RecoTrackAccumulator(const edm::ParameterSet& conf, edm::one::EDProducerBase& mixMod, edm::ConsumesCollector& iC) :
  GeneralTrackInputSignal_(conf.getParameter<edm::InputTag>("GeneralTrackInputSignal")),
  GeneralTrackInputPileup_(conf.getParameter<edm::InputTag>("GeneralTrackInputPileup")),
  GeneralTrackOutput_(conf.getParameter<std::string>("GeneralTrackOutput")),
  GeneralTrackExtraInputSignal_(conf.getParameter<edm::InputTag>("GeneralTrackExtraInputSignal")),
  GeneralTrackExtraInputPileup_(conf.getParameter<edm::InputTag>("GeneralTrackExtraInputPileup")),
  GeneralTrackExtraOutput_(conf.getParameter<std::string>("GeneralTrackExtraOutput"))
{

  mixMod.produces<reco::TrackCollection>(GeneralTrackOutput_);
  mixMod.produces<reco::TrackExtraCollection>(GeneralTrackOutput_);

  iC.consumes<reco::TrackCollection>(GeneralTrackInputSignal_);
  iC.consumes<reco::TrackExtraCollection>(GeneralTrackExtraInputSignal_);

  iC.consumes<reco::TrackCollection>(GeneralTrackInputPileup_);
  iC.consumes<reco::TrackExtraCollection>(GeneralTrackExtraInputPileup_);
}
  
RecoTrackAccumulator::~RecoTrackAccumulator() {
    
}
  
void RecoTrackAccumulator::initializeEvent(edm::Event const& e, edm::EventSetup const& iSetup) {
    
  NewTrackList_ = std::auto_ptr<reco::TrackCollection>(new reco::TrackCollection());
  NewTrackExtraList_ = std::auto_ptr<reco::TrackExtraCollection>(new reco::TrackExtraCollection());

}
  
void RecoTrackAccumulator::accumulate(edm::Event const& e, edm::EventSetup const& iSetup) {
  

  edm::Handle<reco::TrackCollection> tracks;
  edm::Handle<reco::TrackExtraCollection> trackExtras;
  e.getByLabel(GeneralTrackInputSignal_, tracks);
  e.getByLabel(GeneralTrackExtraInputSignal_, trackExtras);


  if (tracks.isValid()) {
    for (auto const& track : *tracks) {
      NewTrackList_->push_back(track);
      // corresponding TrackExtra:
      const reco::TrackExtraRef & trackExtraRef_(track.extra());
      NewTrackExtraList_->push_back(*trackExtraRef_);
      // alternative: recreate from scratch
      /*
      reco::TrackExtra trackExtra(track.outerPosition(),
				  track.outerMomentum(),
				  track.outerOk(),
				  track.innerPosition(),
				  track.innerMomentum(),
				  track.innerOk(),
				  track.outerStateCovariance(),
				  track.outerDetId(),
				  track.innerStateCovariance(),
				  track.innerDetId(),
				  track.seedDirection(),
				  track.seedRef()); 
      NewTrackExtraList_->push_back(trackExtra);
      */
    }
    //    for (auto const& trackExtra : *trackExtras) {
    //      NewTrackExtraList_->push_back(trackExtra);
    //    }
  }

}

void RecoTrackAccumulator::accumulate(PileUpEventPrincipal const& e, edm::EventSetup const& iSetup) {

  if (e.bunchCrossing()==0) {
    edm::Handle<reco::TrackCollection> tracks;
    edm::Handle<reco::TrackExtraCollection> trackExtras;
    e.getByLabel(GeneralTrackInputPileup_, tracks);
    e.getByLabel(GeneralTrackExtraInputPileup_, trackExtras);
    
    if (tracks.isValid()) {
      //      for (reco::TrackCollection::const_iterator track = tracks->begin();  track != tracks->end();  ++track) {
      for (auto const& track : *tracks) {
	//	NewTrackList_->push_back(*track);
	NewTrackList_->push_back(track);
	reco::TrackExtra trackExtra(track.outerPosition(),
				    track.outerMomentum(),
				    track.outerOk(),
				    track.innerPosition(),
				    track.innerMomentum(),
				    track.innerOk(),
				    track.outerStateCovariance(),
				    track.outerDetId(),
				    track.innerStateCovariance(),
				    track.innerDetId(),
				    track.seedDirection(),
				    track.seedRef()); 
	NewTrackExtraList_->push_back(trackExtra);
      }
      //      for (auto const& trackExtra : *trackExtras) {
      //	NewTrackExtraList_->push_back(trackExtra);
      //      }
    }
  }

}

void RecoTrackAccumulator::finalizeEvent(edm::Event& e, const edm::EventSetup& iSetup) {
  
  e.put( NewTrackList_, GeneralTrackOutput_ );
  e.put( NewTrackExtraList_, GeneralTrackOutput_ );

}

