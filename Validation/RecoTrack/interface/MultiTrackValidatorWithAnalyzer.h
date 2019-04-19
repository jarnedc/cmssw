#ifndef MultiTrackValidatorWithAnalyzer_h
#define MultiTrackValidatorWithAnalyzer_h

/** \class MultiTrackValidator
 *  Class that prodecs histrograms to validate Track Reconstruction performances
 *
 *  \author cerati
 */
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
//#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
//#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
//#include "Validation/RecoTrack/interface/MultiTrackValidatorBase.h"
#include "Validation/RecoTrack/interface/MTVHistoProducerAlgoForTracker.h"
#include "SimDataFormats/Associations/interface/VertexToTrackingVertexAssociator.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TEfficiency.h"
#include "TFile.h"


#include <memory>

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "SimDataFormats/Associations/interface/TrackToTrackingParticleAssociator.h"


//#include "SimTracker/Common/interface/TrackingParticleSelector.h"
#include "CommonTools/RecoAlgos/interface/CosmicTrackingParticleSelector.h"
//#include "DataFormats/Common/interface/ValueMap.h"

//#include <DQMServices/Core/interface/DQMStore.h>

//#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <iostream>
#include <sstream>
#include <string>

#include "FWCore/Framework/interface/MakerMacros.h"

class PileupSummaryInfo;
namespace reco {
class DeDxData;
}

class MultiTrackValidatorWithAnalyzer  : public edm::EDAnalyzer {
 public:
  /// Constructor
  explicit MultiTrackValidatorWithAnalyzer(edm::ParameterSet const& pset);

  virtual void beginJob();
  void analyze( edm::Event const& iEvent, edm::EventSetup const& iSetup ) ;
  virtual void endJob();
  virtual ~MultiTrackValidatorWithAnalyzer();

  /// Method called to book the DQM histograms
  //void bookHistograms(DQMStore::IBooker&, edm::Run const&, edm::EventSetup const&) override;


 protected:
  //from Base
  std::vector<edm::InputTag> associators;
  edm::EDGetTokenT<TrackingParticleCollection> label_tp_effic;
  edm::EDGetTokenT<TrackingParticleCollection> label_tp_fake;
  edm::EDGetTokenT<TrackingVertexCollection> label_tv;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo> > label_pileupinfo;

  std::vector<edm::EDGetTokenT<std::vector<PSimHit> > > simHitTokens_;
  std::string parametersDefiner;


  std::vector<edm::InputTag> label;
  std::vector<edm::EDGetTokenT<edm::View<reco::Track> > > labelToken;
  std::vector<edm::EDGetTokenT<edm::View<TrajectorySeed> > > labelTokenSeed;
  edm::EDGetTokenT<reco::BeamSpot>  bsSrc;

  edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > m_dEdx1Tag;
  edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > m_dEdx2Tag;

  bool ignoremissingtkcollection_;



  //these are used by MTVGenPs
  bool UseAssociators;
  const bool parametersDefinerIsCosmic_;
  const bool calculateDrSingleCollection_;
  const bool doPlotsOnlyForTruePV_;
  const bool doSummaryPlots_;
  const bool doSimPlots_;
  const bool doSimTrackPlots_;
  const bool doRecoTrackPlots_;
  const bool dodEdxPlots_;
  const bool doPVAssociationPlots_;
  const bool doSeedPlots_;
  std::unique_ptr<MTVHistoProducerAlgoForTracker> histoProducerAlgo_;

 private:
  std::vector<edm::EDGetTokenT<reco::TrackToTrackingParticleAssociator>> associatorTokens;
  std::vector<edm::EDGetTokenT<reco::SimToRecoCollection>> associatormapStRs;
  std::vector<edm::EDGetTokenT<reco::RecoToSimCollection>> associatormapRtSs;

  std::string dirName_;

  bool useGsf;
  // select tracking particles 
  //(i.e. "denominator" of the efficiency ratio)
  TrackingParticleSelector tpSelector;				      
  CosmicTrackingParticleSelector cosmictpSelector;
  TrackingParticleSelector dRtpSelector;				      
  TrackingParticleSelector dRtpSelectorNoPtCut;

  edm::EDGetTokenT<SimHitTPAssociationProducer::SimHitTPAssociationList> _simHitTpMapTag;
  edm::EDGetTokenT<edm::View<reco::Track> > labelTokenForDrCalculation;
  edm::EDGetTokenT<edm::View<reco::Vertex> > recoVertexToken_;
  edm::EDGetTokenT<reco::VertexToTrackingVertexAssociator> vertexAssociatorToken_;
  std::vector<edm::EDGetTokenT<std::vector<int>>> seedToTrackTokens_;

  std::vector<MonitorElement *> h_reco_coll, h_assoc_coll, h_assoc2_coll, h_simul_coll, h_looper_coll, h_pileup_coll;
  std::vector<MonitorElement *> h_assoc_coll_allPt, h_simul_coll_allPt;


  edm::Service<TFileService> m_fs;
  TString a = ""; //"WjetsMC" "ZeroBias" "MET" "MinBiasMC" "SingleMuon"
  TString b = a + "";

  //--------- Histogram Declaration --------------------//
  std::map<TString, TH1F *> histos_th1f;
  std::map<TString, TH2F *> histos_th2f;
  std::map<TString, TEfficiency *> histos_teff;


};


#endif
