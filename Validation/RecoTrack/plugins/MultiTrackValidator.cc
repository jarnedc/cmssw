#include "Validation/RecoTrack/interface/MultiTrackValidator.h"
#include "DQMServices/ClientConfig/interface/FitSlicesYTool.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/Associations/interface/TrackToTrackingParticleAssociator.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/PatternTools/interface/TSCBLBuilderNoMaterial.h"
#include "SimTracker/TrackAssociation/plugins/ParametersDefinerForTPESProducer.h"
#include "SimTracker/TrackAssociation/plugins/CosmicParametersDefinerForTPESProducer.h"
#include "SimGeneral/TrackingAnalysis/interface/TrackingParticleNumberOfLayers.h"

#include "DataFormats/TrackReco/interface/DeDxData.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/Ref.h"
#include "CommonTools/Utils/interface/associationMapFilterValues.h"
#include<type_traits>


#include "TMath.h"
#include <TF1.h>
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/PtEtaPhiMass.h"
//#include <iostream>

using namespace std;
using namespace edm;

typedef edm::Ref<edm::HepMCProduct, HepMC::GenParticle > GenParticleRef;

MultiTrackValidator::MultiTrackValidator(const edm::ParameterSet& pset):
  MultiTrackValidatorBase(pset,consumesCollector()),
  parametersDefinerIsCosmic_(parametersDefiner == "CosmicParametersDefinerForTP"),
  calculateDrSingleCollection_(pset.getUntrackedParameter<bool>("calculateDrSingleCollection")),
  doPlotsOnlyForTruePV_(pset.getUntrackedParameter<bool>("doPlotsOnlyForTruePV")),
  doSummaryPlots_(pset.getUntrackedParameter<bool>("doSummaryPlots")),
  doSimPlots_(pset.getUntrackedParameter<bool>("doSimPlots")),
  doSimTrackPlots_(pset.getUntrackedParameter<bool>("doSimTrackPlots")),
  doRecoTrackPlots_(pset.getUntrackedParameter<bool>("doRecoTrackPlots")),
  dodEdxPlots_(pset.getUntrackedParameter<bool>("dodEdxPlots")),
  doPVAssociationPlots_(pset.getUntrackedParameter<bool>("doPVAssociationPlots")),
  doSeedPlots_(pset.getUntrackedParameter<bool>("doSeedPlots"))
{
  ParameterSet psetForHistoProducerAlgo = pset.getParameter<ParameterSet>("histoProducerAlgoBlock");
  histoProducerAlgo_ = std::make_unique<MTVHistoProducerAlgoForTracker>(psetForHistoProducerAlgo, consumesCollector());

  dirName_ = pset.getParameter<std::string>("dirName");
  UseAssociators = pset.getParameter< bool >("UseAssociators");

  if(dodEdxPlots_) {
    m_dEdx1Tag = consumes<edm::ValueMap<reco::DeDxData> >(pset.getParameter< edm::InputTag >("dEdx1Tag"));
    m_dEdx2Tag = consumes<edm::ValueMap<reco::DeDxData> >(pset.getParameter< edm::InputTag >("dEdx2Tag"));
  }

  if(doPlotsOnlyForTruePV_ || doPVAssociationPlots_) {
    label_tv = consumes<TrackingVertexCollection>(pset.getParameter< edm::InputTag >("label_tv"));
    recoVertexToken_ = consumes<edm::View<reco::Vertex> >(pset.getUntrackedParameter<edm::InputTag>("label_vertex"));
    vertexAssociatorToken_ = consumes<reco::VertexToTrackingVertexAssociator>(pset.getUntrackedParameter<edm::InputTag>("vertexAssociator"));
  }

  tpSelector = TrackingParticleSelector(pset.getParameter<double>("ptMinTP"),
					pset.getParameter<double>("minRapidityTP"),
					pset.getParameter<double>("maxRapidityTP"),
					pset.getParameter<double>("tipTP"),
					pset.getParameter<double>("lipTP"),
					pset.getParameter<int>("minHitTP"),
					pset.getParameter<bool>("signalOnlyTP"),
					pset.getParameter<bool>("intimeOnlyTP"),
					pset.getParameter<bool>("chargedOnlyTP"),
					pset.getParameter<bool>("stableOnlyTP"),
					pset.getParameter<std::vector<int> >("pdgIdTP"));

  cosmictpSelector = CosmicTrackingParticleSelector(pset.getParameter<double>("ptMinTP"),
						    pset.getParameter<double>("minRapidityTP"),
						    pset.getParameter<double>("maxRapidityTP"),
						    pset.getParameter<double>("tipTP"),
						    pset.getParameter<double>("lipTP"),
						    pset.getParameter<int>("minHitTP"),
						    pset.getParameter<bool>("chargedOnlyTP"),
						    pset.getParameter<std::vector<int> >("pdgIdTP"));


  ParameterSet psetVsPhi = psetForHistoProducerAlgo.getParameter<ParameterSet>("TpSelectorForEfficiencyVsPhi");
  dRtpSelector = TrackingParticleSelector(psetVsPhi.getParameter<double>("ptMin"),
					  psetVsPhi.getParameter<double>("minRapidity"),
					  psetVsPhi.getParameter<double>("maxRapidity"),
					  psetVsPhi.getParameter<double>("tip"),
					  psetVsPhi.getParameter<double>("lip"),
					  psetVsPhi.getParameter<int>("minHit"),
					  psetVsPhi.getParameter<bool>("signalOnly"),
					  psetVsPhi.getParameter<bool>("intimeOnly"),
					  psetVsPhi.getParameter<bool>("chargedOnly"),
					  psetVsPhi.getParameter<bool>("stableOnly"),
					  psetVsPhi.getParameter<std::vector<int> >("pdgId"));

  dRtpSelectorNoPtCut = TrackingParticleSelector(0.0,
                                                 psetVsPhi.getParameter<double>("minRapidity"),
                                                 psetVsPhi.getParameter<double>("maxRapidity"),
                                                 psetVsPhi.getParameter<double>("tip"),
                                                 psetVsPhi.getParameter<double>("lip"),
                                                 psetVsPhi.getParameter<int>("minHit"),
                                                 psetVsPhi.getParameter<bool>("signalOnly"),
                                                 psetVsPhi.getParameter<bool>("intimeOnly"),
                                                 psetVsPhi.getParameter<bool>("chargedOnly"),
                                                 psetVsPhi.getParameter<bool>("stableOnly"),
                                                 psetVsPhi.getParameter<std::vector<int> >("pdgId"));

  useGsf = pset.getParameter<bool>("useGsf");

  _simHitTpMapTag = mayConsume<SimHitTPAssociationProducer::SimHitTPAssociationList>(pset.getParameter<edm::InputTag>("simHitTpMapTag"));
  token_genparticlePlusGEANT  = mayConsume<vector<reco::GenParticle>>(pset.getParameter<edm::InputTag>("genParticlePlusGEANT"));
  token_beamspot  = mayConsume<reco::BeamSpot>(pset.getParameter<edm::InputTag>("beamSpot"));

  if(calculateDrSingleCollection_) {
    labelTokenForDrCalculation = consumes<edm::View<reco::Track> >(pset.getParameter<edm::InputTag>("trackCollectionForDrCalculation"));
  }

  if(UseAssociators) {
    for (auto const& src: associators) {
      associatorTokens.push_back(consumes<reco::TrackToTrackingParticleAssociator>(src));
    }
  } else {   
    for (auto const& src: associators) {
      associatormapStRs.push_back(consumes<reco::SimToRecoCollection>(src));
      associatormapRtSs.push_back(consumes<reco::RecoToSimCollection>(src));
    }
  }

  if(doSeedPlots_) {
    for(const auto& tag: pset.getParameter< std::vector<edm::InputTag> >("label")) {
      seedToTrackTokens_.push_back(consumes<std::vector<int>>(tag));
    }
  }
}


MultiTrackValidator::~MultiTrackValidator() {
   OutputFile->cd();
   
   //For the eveluation of the cuts for efficient MC production
   h_EvaluationCutsForEfiicientMC->Write();

   //for the tracking particles	
   dir_TrackingParticle->cd();
   h_TrackingParticle_charge->Write();
   h_TrackingParticle_pt->Write();
   h_TrackingParticle_eta->Write();
   h_TrackingParticle_phi->Write();
   h_TrackingParticle_pdgId->Write();
   h_TrackingParticle_vx->Write();
   h_TrackingParticle_vy->Write();
   h_TrackingParticle_vz->Write();
   h_TrackingParticle_lxy->Write();
   h_TrackingParticle_status->Write();
   h_TrackingParticle_numberOfHits->Write();
   //for the tracking particles with a matching track
   dir_TrackingParticleMatchFound->cd();
   h_TrackingParticleMatchFound_charge->Write();
   h_TrackingParticleMatchFound_pt->Write();
   h_TrackingParticleMatchFound_eta->Write();
   h_TrackingParticleMatchFound_phi->Write();
   h_TrackingParticleMatchFound_pdgId->Write();
   h_TrackingParticleMatchFound_vx->Write();
   h_TrackingParticleMatchFound_vy->Write();
   h_TrackingParticleMatchFound_vz->Write();
   h_TrackingParticleMatchFound_lxy->Write();
   h_TrackingParticleMatchFound_status->Write();
   h_TrackingParticleMatchFound_numberOfHits->Write();
   //for the tracking particles with no matching track
   dir_TrackingParticleMatchNotFound->cd();
   h_TrackingParticleMatchNotFound_charge->Write();
   h_TrackingParticleMatchNotFound_pt->Write();
   h_TrackingParticleMatchNotFound_eta->Write();
   h_TrackingParticleMatchNotFound_phi->Write();
   h_TrackingParticleMatchNotFound_pdgId->Write();
   h_TrackingParticleMatchNotFound_vx->Write();
   h_TrackingParticleMatchNotFound_vy->Write();
   h_TrackingParticleMatchNotFound_vz->Write();
   h_TrackingParticleMatchNotFound_lxy->Write();
   h_TrackingParticleMatchNotFound_status->Write();
   h_TrackingParticleMatchNotFound_numberOfHits->Write();
   //for the tracks:
   dir_Track->cd();
   h_Track_charge->Write();
   h_Track_pt->Write();
   h_Track_eta->Write();
   h_Track_phi->Write();
   h_Track_vx->Write();
   h_Track_vy->Write();
   h_Track_vz->Write();	
   h_Track_lxy->Write();	
   h_Track_numberOfValidHits->Write();
   //For the TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed
   dir_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed->cd(); 
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_charge->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_pt->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_eta->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_phi->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vx->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vy->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vz->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_lxy->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_status->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_numberOfValidHits->Write();
   //For the TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed
   dir_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed->cd();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_charge->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_pt->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_eta->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_phi->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vx->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vy->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vz->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_lxy->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_status->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_numberOfValidHits->Write();
   //For the TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed
   dir_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed->cd();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_charge->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_pt->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_eta->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_phi->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vx->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vy->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vz->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_lxy->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_status->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_numberOfValidHits->Write();
   //For the TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed
   dir_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed->cd();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_charge->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_pt->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_eta->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_phi->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vx->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vy->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vz->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_lxy->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_status->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_numberOfValidHits->Write();
   ////For the TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed
   dir_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed->cd();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_charge->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_pt->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_eta->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_phi->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vx->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vy->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vz->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_lxy->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_status->Write();
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_numberOfValidHits->Write();
   //For the TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed
   dir_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed->cd();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_charge->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_pt->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_eta->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_phi->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vx->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vy->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vz->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_lxy->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_status->Write();
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_numberOfValidHits->Write();
   //For the TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed
   dir_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed->cd();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_charge->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_pt->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_eta->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_phi->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vx->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vy->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vz->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_lxy->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_status->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_numberOfValidHits->Write();
   //For the TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed
   dir_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed->cd();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_charge->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_pt->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_eta->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_phi->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vx->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vy->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vz->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_lxy->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_status->Write();
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_numberOfValidHits->Write();
   //comparing track to trackingparticles
   dir_TrackingParticleToTrack->cd();
   h_TrackingParticleToTrack_charge->Write();
   h_TrackingParticleToTrack_pt->Write();
   h_TrackingParticleToTrack_eta->Write();
   h_TrackingParticleToTrack_phi->Write();
   h_TrackingParticleToTrack_vx->Write();
   h_TrackingParticleToTrack_vy->Write();
   h_TrackingParticleToTrack_vz->Write();
   h_TrackingParticleToTrack_lxy->Write();
   //efficiency plots all tracking particles
   dir_eff_TrackingParticle->cd();
   teff_TrackingParticle_charge->Write();
   teff_TrackingParticle_pt->Write();
   teff_TrackingParticle_pt_lxy_eta_cut->Write();
   teff_TrackingParticle_pt_lxy_eta_status1_cut->Write();
   teff_TrackingParticle_eta->Write();
   teff_TrackingParticle_eta_pt_lxy_cut->Write();
   teff_TrackingParticle_eta_pt_lxy_status1_cut->Write();
   teff_TrackingParticle_phi->Write();
   teff_TrackingParticle_pdgId->Write();
   teff_TrackingParticle_vx->Write();
   teff_TrackingParticle_vy->Write();
   teff_TrackingParticle_vz->Write();
   teff_TrackingParticle_lxy->Write();
   teff_TrackingParticle_lxy_pt_eta_cut->Write();
   teff_TrackingParticle_lxy_pt_eta_status1_cut->Write();
   teff_TrackingParticle_status->Write();
   teff_TrackingParticle_numberOfHits->Write();

   //efficiency plots for trackingparticles as granddaughter of the antiS
   dir_eff_TrackingParticlegranddaughterantiS->cd();
   teff_TrackingParticlegranddaughterantiS_granddaughtertype->Write();
   h_TrackingParticlegranddaughterantiS_nRecoGranddaughters_with_charged_granddaughters->Write();
   h_TrackingParticlegranddaughterantiS_nRecoGranddaughters_all_granddaughters->Write();
   h_TrackingParticlegranddaughterantiS_nReconstructableGranddaughters_nRecoGranddaughters->Write();
   h_reconstructedKsDaughterCombination_reconstructedLDaughterCombination->Write();
   h_reconstructedKsDaughterCombination_reconstructedLDaughterCombination_4reconstructableGranddaughtersOnly->Write();
   //efficiency plots trackingparticles as daughters of the Ks
   dir_eff_TrackingParticlegranddaughterIsKsDaughter->cd();
   teff_TrackingParticlegranddaughterIsKsDaughter_pt->Write();
   h_TrackingParticlegranddaughterIsKsDaughter_pt_nom->Write();
   h_TrackingParticlegranddaughterIsKsDaughter_pt_all->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_eta->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_phi->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_lxy->Write();
   h_TrackingParticlegranddaughterIsKsDaughter_lxy_nom->Write();
   h_TrackingParticlegranddaughterIsKsDaughter_lxy_all->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_lxyz->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_vz->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_lxyV0CreationVertex->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_vzV0CreationVertex->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_lxyzV0CreationVertex->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_etaAntiS->Write();
   h_TrackingParticlegranddaughterIsKsDaughter_etaAntiS_all->Write();
   h_TrackingParticlegranddaughterIsKsDaughter_etaAntiS_nom->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_etaKs->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_pAntiS->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_pTKs->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS->Write();
   h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS_nom->Write();
   h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS_all->Write();
   teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex->Write();
   h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex_nom->Write();
   h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex_all->Write();
	

   //efficiency plots trackingparticles as pion daughters of the Lambda
   dir_eff_TrackingParticlegranddaughterIsPosPionLambdaDaughter->cd();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt_nom->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt_all->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_eta->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_phi->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy_nom->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy_all->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyz->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_vz->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex_nom->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex_all->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_vzV0CreationVertex->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyzV0CreationVertex->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS_all->Write();
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS_nom->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiLambda->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pAntiS->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiLambda->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS->Write();
   h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS_nom->Write();
   h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS_all->Write();
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex->Write();
   h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex_nom->Write();
   h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex_all->Write();
	

   //efficiency plots trackingparticles as anti proton daughters of the Lambda
   dir_eff_TrackingParticlegranddaughterIsProtonLambdaDaughter->cd();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt->Write();
   h_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt_nom->Write();
   h_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt_all->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_eta->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_phi->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy->Write();
   h_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy_nom->Write();
   h_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy_all->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyz->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_vz->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyV0CreationVertex->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_vzV0CreationVertex->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyzV0CreationVertex->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS->Write();
   h_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS_all->Write();
   h_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS_nom->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiLambda->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pAntiS->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiLambda->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS->Write();
   h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS_nom->Write();
   h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS_all->Write();
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex->Write();
   h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex_nom->Write();
   h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex_all->Write();
	

   //track to trackingparticle match, based on deltaR
   dir_TrackingParticleToTrackDeltaRMatch->cd();
   h_TrackingParticleToTrackDeltaRMatch_deltaR->Write(); 
   teff_TrackingParticleToTrackDeltaRMatch_charge->Write();
   teff_TrackingParticleToTrackDeltaRMatch_pt->Write();
   teff_TrackingParticleToTrackDeltaRMatch_pt_lxy_eta_cut->Write();
   teff_TrackingParticleToTrackDeltaRMatch_pt_lxy_eta_status1_cut->Write();
   teff_TrackingParticleToTrackDeltaRMatch_eta->Write();
   teff_TrackingParticleToTrackDeltaRMatch_eta_pt_lxy_cut->Write();
   teff_TrackingParticleToTrackDeltaRMatch_eta_pt_lxy_status1_cut->Write();
   teff_TrackingParticleToTrackDeltaRMatch_phi->Write();
   teff_TrackingParticleToTrackDeltaRMatch_pdgId->Write();
   teff_TrackingParticleToTrackDeltaRMatch_vx->Write();
   teff_TrackingParticleToTrackDeltaRMatch_vy->Write();
   teff_TrackingParticleToTrackDeltaRMatch_vz->Write();
   teff_TrackingParticleToTrackDeltaRMatch_lxy->Write();
   teff_TrackingParticleToTrackDeltaRMatch_lxy_pt_eta_cut->Write();
   teff_TrackingParticleToTrackDeltaRMatch_lxy_pt_eta_status1_cut->Write();
   teff_TrackingParticleToTrackDeltaRMatch_status->Write();
   teff_TrackingParticleToTrackDeltaRMatch_numberOfHits->Write();

   //track to genparticlesPlusGEANT match, based on deltaR
   dir_GenParticlesPlusGEANTToTrackDeltaRMatch->cd();
   h_GenParticlesPlusGEANTToTrackDeltaRMatch_deltaR->Write(); 
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_charge->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt_lxy_eta_cut->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt_lxy_eta_status1_cut->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta_pt_lxy_cut->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta_pt_lxy_status1_cut->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_phi->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pdgId->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vx->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vy->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vz->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy_pt_eta_cut->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy_pt_eta_status1_cut->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_status->Write();
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_numberOfHits->Write();
  
}


void MultiTrackValidator::bookHistograms(DQMStore::IBooker& ibook, edm::Run const&, edm::EventSetup const& setup) {

  const auto minColl = -0.5;
  const auto maxColl = label.size()-0.5;
  const auto nintColl = label.size();

  auto binLabels = [&](MonitorElement *me) {
    TH1 *h = me->getTH1();
    for(size_t i=0; i<label.size(); ++i) {
      h->GetXaxis()->SetBinLabel(i+1, label[i].label().c_str());
    }
    return me;
  };

  //Booking histograms concerning with simulated tracks
  if(doSimPlots_) {
    ibook.cd();
    ibook.setCurrentFolder(dirName_ + "simulation");

    histoProducerAlgo_->bookSimHistos(ibook);

    ibook.cd();
    ibook.setCurrentFolder(dirName_);
  }

  for (unsigned int ww=0;ww<associators.size();ww++){
    ibook.cd();
    // FIXME: these need to be moved to a subdirectory whose name depends on the associator
    ibook.setCurrentFolder(dirName_);

    if(doSummaryPlots_) {
      if(doSimTrackPlots_) {
        h_assoc_coll.push_back(binLabels( ibook.book1D("num_assoc(simToReco)_coll", "N of associated (simToReco) tracks vs track collection", nintColl, minColl, maxColl) ));
        h_simul_coll.push_back(binLabels( ibook.book1D("num_simul_coll", "N of simulated tracks vs track collection", nintColl, minColl, maxColl) ));

        h_assoc_coll_allPt.push_back(binLabels( ibook.book1D("num_assoc(simToReco)_coll_allPt", "N of associated (simToReco) tracks vs track collection", nintColl, minColl, maxColl) ));
        h_simul_coll_allPt.push_back(binLabels( ibook.book1D("num_simul_coll_allPt", "N of simulated tracks vs track collection", nintColl, minColl, maxColl) ));

      }
      if(doRecoTrackPlots_) {
        h_reco_coll.push_back(binLabels( ibook.book1D("num_reco_coll", "N of reco track vs track collection", nintColl, minColl, maxColl) ));
        h_assoc2_coll.push_back(binLabels( ibook.book1D("num_assoc(recoToSim)_coll", "N of associated (recoToSim) tracks vs track collection", nintColl, minColl, maxColl) ));
        h_looper_coll.push_back(binLabels( ibook.book1D("num_duplicate_coll", "N of associated (recoToSim) looper tracks vs track collection", nintColl, minColl, maxColl) ));
        h_pileup_coll.push_back(binLabels( ibook.book1D("num_pileup_coll", "N of associated (recoToSim) pileup tracks vs track collection", nintColl, minColl, maxColl) ));
      }
    }

    for (unsigned int www=0;www<label.size();www++){
      ibook.cd();
      InputTag algo = label[www];
      string dirName=dirName_;
      if (algo.process()!="")
        dirName+=algo.process()+"_";
      if(algo.label()!="")
        dirName+=algo.label()+"_";
      if(algo.instance()!="")
        dirName+=algo.instance()+"_";
      if (dirName.find("Tracks")<dirName.length()){
        dirName.replace(dirName.find("Tracks"),6,"");
      }
      string assoc= associators[ww].label();
      if (assoc.find("Track")<assoc.length()){
        assoc.replace(assoc.find("Track"),5,"");
      }
      dirName+=assoc;
      std::replace(dirName.begin(), dirName.end(), ':', '_');

      ibook.setCurrentFolder(dirName.c_str());

      if(doSimTrackPlots_) {
        histoProducerAlgo_->bookSimTrackHistos(ibook);
        if(doPVAssociationPlots_) histoProducerAlgo_->bookSimTrackPVAssociationHistos(ibook);
      }

      //Booking histograms concerning with reconstructed tracks
      if(doRecoTrackPlots_) {
        histoProducerAlgo_->bookRecoHistos(ibook);
        if (dodEdxPlots_) histoProducerAlgo_->bookRecodEdxHistos(ibook);
        if (doPVAssociationPlots_) histoProducerAlgo_->bookRecoPVAssociationHistos(ibook);
      }

      if(doSeedPlots_) {
        histoProducerAlgo_->bookSeedHistos(ibook);
      }
    }//end loop www
  }// end loop ww
}


void MultiTrackValidator::FillHistoTrackingParticle(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticle_charge->Fill(tp.charge());
   h_TrackingParticle_pt->Fill(tp.pt());
   h_TrackingParticle_eta->Fill(tp.eta());
   h_TrackingParticle_phi->Fill(tp.phi());
   h_TrackingParticle_pdgId->Fill(tp.pdgId());
   h_TrackingParticle_vx->Fill(tp.vx());
   h_TrackingParticle_vy->Fill(tp.vy());
   h_TrackingParticle_vz->Fill(tp.vz());
   h_TrackingParticle_lxy->Fill(Lxy);
   h_TrackingParticle_status->Fill(tp.status());
   h_TrackingParticle_numberOfHits->Fill(tp.numberOfHits());

   /*if(tp.pdgId()==-1020000020) {
	std::cout << "found an antiS in the trackingParticleCollection, with status = " << tp.status() << " and vertex position: " << tp.vx() << "," << tp.vy() << "," << tp.vz() << std::endl;
	for (tv_iterator iTV = tp.decayVertices_begin(); iTV != tp.decayVertices_end(); ++iTV)
	{
		cout << " Decay vertices of the antiS:      " << (**iTV).position() << std::endl;
	}

	std::cout << "The beamspot location: " << beamspot.X() << "," << beamspot.Y() << "," << beamspot.Z() << endl;
   }

   if(abs(tp.pdgId())==310){
	std::cout << "found a Ks in the trackingParticleCollection, with status = " << tp.status() << " and vertex position: " << tp.vx() << "," << tp.vy() << "," << tp.vz() << std::endl;
   }
   if(abs(tp.pdgId())==3122){
	std::cout << "found a Lambda in the trackingParticleCollection, with status = " << tp.status() << " and vertex position: " << tp.vx() << "," << tp.vy() << "," << tp.vz() << std::endl;
   }*/


}

void MultiTrackValidator::FillHistoTrackingParticleMatchFound(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticleMatchFound_charge->Fill(tp.charge());
   h_TrackingParticleMatchFound_pt->Fill(tp.pt());
   h_TrackingParticleMatchFound_eta->Fill(tp.eta());
   h_TrackingParticleMatchFound_phi->Fill(tp.phi());
   h_TrackingParticleMatchFound_pdgId->Fill(tp.pdgId());
   h_TrackingParticleMatchFound_vx->Fill(tp.vx());
   h_TrackingParticleMatchFound_vy->Fill(tp.vy());
   h_TrackingParticleMatchFound_vz->Fill(tp.vz());
   h_TrackingParticleMatchFound_lxy->Fill(Lxy);
   h_TrackingParticleMatchFound_status->Fill(tp.status());
   h_TrackingParticleMatchFound_numberOfHits->Fill(tp.numberOfHits());

}

void MultiTrackValidator::FillHistoTrackingParticleMatchNotFound(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticleMatchNotFound_charge->Fill(tp.charge());
   h_TrackingParticleMatchNotFound_pt->Fill(tp.pt());
   h_TrackingParticleMatchNotFound_eta->Fill(tp.eta());
   h_TrackingParticleMatchNotFound_phi->Fill(tp.phi());
   h_TrackingParticleMatchNotFound_pdgId->Fill(tp.pdgId());
   h_TrackingParticleMatchNotFound_vx->Fill(tp.vx());
   h_TrackingParticleMatchNotFound_vy->Fill(tp.vy());
   h_TrackingParticleMatchNotFound_vz->Fill(tp.vz());
   h_TrackingParticleMatchNotFound_lxy->Fill(Lxy);
   h_TrackingParticleMatchNotFound_status->Fill(tp.status());
   h_TrackingParticleMatchNotFound_numberOfHits->Fill(tp.numberOfHits());

}

void MultiTrackValidator::FillHistoTrack(const reco::Track& tk,TVector3 beamspot){
   TVector3 vxyz(tk.vx(),tk.vy(),tk.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_Track_charge->Fill(tk.charge());
   h_Track_pt->Fill(tk.pt());
   h_Track_eta->Fill(tk.eta());
   h_Track_phi->Fill(tk.phi());
   h_Track_vx->Fill(tk.vx());
   h_Track_vy->Fill(tk.vy());
   h_Track_vz->Fill(tk.vz());
   h_Track_lxy->Fill(Lxy);
   h_Track_numberOfValidHits->Fill(tk.numberOfValidHits());

}
////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiTrackValidator::FillHistoTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_charge->Fill(tp.charge());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_pt->Fill(tp.pt());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_eta->Fill(tp.eta());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_phi->Fill(tp.phi());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vx->Fill(tp.vx());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vy->Fill(tp.vy());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vz->Fill(tp.vz());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_lxy->Fill(Lxy);
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_status->Fill(tp.status());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_numberOfValidHits->Fill(tp.numberOfHits());

}

void MultiTrackValidator::FillHitsoTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_charge->Fill(tp.charge());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_pt->Fill(tp.pt());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_eta->Fill(tp.eta());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_phi->Fill(tp.phi());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vx->Fill(tp.vx());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vy->Fill(tp.vy());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vz->Fill(tp.vz());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_lxy->Fill(Lxy);
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_status->Fill(tp.status());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_numberOfValidHits->Fill(tp.numberOfHits());

}

void MultiTrackValidator::FillHistoTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_charge->Fill(tp.charge());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_pt->Fill(tp.pt());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_eta->Fill(tp.eta());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_phi->Fill(tp.phi());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vx->Fill(tp.vx());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vy->Fill(tp.vy());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vz->Fill(tp.vz());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_lxy->Fill(Lxy);
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_status->Fill(tp.status());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_numberOfValidHits->Fill(tp.numberOfHits());

}

void MultiTrackValidator::FillHistoTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_charge->Fill(tp.charge());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_pt->Fill(tp.pt());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_eta->Fill(tp.eta());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_phi->Fill(tp.phi());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vx->Fill(tp.vx());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vy->Fill(tp.vy());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vz->Fill(tp.vz());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_lxy->Fill(Lxy);
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_status->Fill(tp.status());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_numberOfValidHits->Fill(tp.numberOfHits());

}
////////////////////////////////////////
void MultiTrackValidator::FillHistoTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_charge->Fill(tp.charge());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_pt->Fill(tp.pt());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_eta->Fill(tp.eta());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_phi->Fill(tp.phi());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vx->Fill(tp.vx());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vy->Fill(tp.vy());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vz->Fill(tp.vz());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_lxy->Fill(Lxy);
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_status->Fill(tp.status());
   h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_numberOfValidHits->Fill(tp.numberOfHits());

}

void MultiTrackValidator::FillHitsoTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_charge->Fill(tp.charge());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_pt->Fill(tp.pt());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_eta->Fill(tp.eta());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_phi->Fill(tp.phi());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vx->Fill(tp.vx());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vy->Fill(tp.vy());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vz->Fill(tp.vz());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_lxy->Fill(Lxy);
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_status->Fill(tp.status());
   h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_numberOfValidHits->Fill(tp.numberOfHits());

}

void MultiTrackValidator::FillHistoTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_charge->Fill(tp.charge());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_pt->Fill(tp.pt());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_eta->Fill(tp.eta());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_phi->Fill(tp.phi());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vx->Fill(tp.vx());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vy->Fill(tp.vy());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vz->Fill(tp.vz());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_lxy->Fill(Lxy);
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_status->Fill(tp.status());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_numberOfValidHits->Fill(tp.numberOfHits());

}

void MultiTrackValidator::FillHistoTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed(const TrackingParticle& tp,TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_charge->Fill(tp.charge());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_pt->Fill(tp.pt());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_eta->Fill(tp.eta());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_phi->Fill(tp.phi());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vx->Fill(tp.vx());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vy->Fill(tp.vy());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vz->Fill(tp.vz());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_lxy->Fill(Lxy);
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_status->Fill(tp.status());
   h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_numberOfValidHits->Fill(tp.numberOfHits());

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MultiTrackValidator::FillHistoMatchingTrackingParticleToTrack(const reco::Track * tk,const TrackingParticle& tp, TVector3 beamspot){
   TVector3 vxyz_tk(tk->vx(),tk->vy(),tk->vz());
   TVector3 vxyz_tp(tp.vx(),tp.vy(),tp.vz());
   double Lxy_tk = lxy(beamspot,vxyz_tk);
   double Lxy_tp = lxy(beamspot,vxyz_tp);
   h_TrackingParticleToTrack_charge->Fill(tp.charge()-tk->charge());
   h_TrackingParticleToTrack_pt->Fill(tp.pt()-tk->pt());
   h_TrackingParticleToTrack_eta->Fill(tp.eta()-tk->eta());
   h_TrackingParticleToTrack_phi->Fill(reco::deltaPhi(tp.phi(),tk->phi()));
   h_TrackingParticleToTrack_vx->Fill(tp.vx()-tk->vx());
   h_TrackingParticleToTrack_vy->Fill(tp.vy()-tk->vy());
   h_TrackingParticleToTrack_vz->Fill(tp.vz()-tk->vz());
   h_TrackingParticleToTrack_lxy->Fill(Lxy_tp-Lxy_tk);

}

void MultiTrackValidator::FillTEffTrackingParticle(const TrackingParticle& tp, bool matchFound, TVector3 beamspot){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   teff_TrackingParticle_charge->Fill(matchFound,tp.charge());
   teff_TrackingParticle_pt->Fill(matchFound,tp.pt());
   if( Lxy < 3.5 && abs(tp.eta()) < 2.5 ) teff_TrackingParticle_pt_lxy_eta_cut->Fill(matchFound,tp.pt());
   if( Lxy < 3.5 && abs(tp.eta()) < 2.5 && tp.status() == 1) teff_TrackingParticle_pt_lxy_eta_status1_cut->Fill(matchFound,tp.pt());
   teff_TrackingParticle_eta->Fill(matchFound,tp.eta());
   if( tp.pt() > 0.9 && Lxy < 3.5 ) teff_TrackingParticle_eta_pt_lxy_cut->Fill(matchFound,tp.eta());
   if( tp.pt() > 0.9 && Lxy < 3.5 && tp.status() == 1) teff_TrackingParticle_eta_pt_lxy_status1_cut->Fill(matchFound,tp.eta());
   teff_TrackingParticle_phi->Fill(matchFound,tp.phi());
   teff_TrackingParticle_pdgId->Fill(matchFound,tp.pdgId());
   teff_TrackingParticle_vx->Fill(matchFound,tp.vx());
   teff_TrackingParticle_vy->Fill(matchFound,tp.vy());
   teff_TrackingParticle_vz->Fill(matchFound,tp.vz());
   teff_TrackingParticle_lxy->Fill(matchFound,Lxy);
   if(tp.pt() > 0.9 && abs(tp.eta()) < 2.5)teff_TrackingParticle_lxy_pt_eta_cut->Fill(matchFound,Lxy);
   if(tp.pt() > 0.9 && abs(tp.eta()) < 2.5 && tp.status() == 1)teff_TrackingParticle_lxy_pt_eta_status1_cut->Fill(matchFound,Lxy);
   teff_TrackingParticle_status->Fill(matchFound,tp.status());
   teff_TrackingParticle_numberOfHits->Fill(matchFound,tp.numberOfHits());
}

 
void MultiTrackValidator::FillTEffTrackingParticlegranddaugterAntiS(const TrackingParticle& tp, bool matchFound, TVector3 beamspot, string particleType){
   if(particleType == "granddaughterIsPosKsDaughter") teff_TrackingParticlegranddaughterantiS_granddaughtertype->Fill(matchFound,0);
   else if(particleType == "granddaughterIsNegKsDaughter") teff_TrackingParticlegranddaughterantiS_granddaughtertype->Fill(matchFound,1);
   else if(particleType == "granddaughterIsPosPionLambdaDaughter") teff_TrackingParticlegranddaughterantiS_granddaughtertype->Fill(matchFound,2);
   else if(particleType == "granddaughterIsNegProtonLambdaDaughter") teff_TrackingParticlegranddaughterantiS_granddaughtertype->Fill(matchFound,3);
   else teff_TrackingParticlegranddaughterantiS_granddaughtertype->Fill(matchFound,4);
}

void MultiTrackValidator::FillTEffTrackingParticlegranddaughterIsKsDaughter(const TrackingParticle& tp, bool matchFound, TVector3 beamspot,TVector3 creationVertexV0,double pt_antiS,double eta_antiS, double pt_V0, double eta_V0, double p_antiS){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   double Lxyz = sqrt(Lxy*Lxy+ (beamspot.Z()-tp.vz()) * (beamspot.Z()-tp.vz()) );
   teff_TrackingParticlegranddaughterIsKsDaughter_pt->Fill(matchFound,tp.pt());
   h_TrackingParticlegranddaughterIsKsDaughter_pt_all->Fill(tp.pt());
   if(matchFound)h_TrackingParticlegranddaughterIsKsDaughter_pt_nom->Fill(tp.pt());
   teff_TrackingParticlegranddaughterIsKsDaughter_eta->Fill(matchFound,tp.eta());
   teff_TrackingParticlegranddaughterIsKsDaughter_phi->Fill(matchFound,tp.phi());
   teff_TrackingParticlegranddaughterIsKsDaughter_lxy->Fill(matchFound,Lxy);
   h_TrackingParticlegranddaughterIsKsDaughter_lxy_all->Fill(Lxy);
   if(matchFound)h_TrackingParticlegranddaughterIsKsDaughter_lxy_nom->Fill(Lxy);
   teff_TrackingParticlegranddaughterIsKsDaughter_lxyz->Fill(matchFound,Lxyz);
   teff_TrackingParticlegranddaughterIsKsDaughter_vz->Fill(matchFound,tp.vz());

   double Lxy_beamspot_V0CreationVertex = lxy(beamspot,creationVertexV0);
   double Lxyz_beamspot_V0CreationVertex = sqrt(Lxy_beamspot_V0CreationVertex*Lxy_beamspot_V0CreationVertex + (beamspot.Z()-creationVertexV0.Z()) * (beamspot.Z()-creationVertexV0.Z()) );
   teff_TrackingParticlegranddaughterIsKsDaughter_lxyV0CreationVertex->Fill(matchFound,Lxy_beamspot_V0CreationVertex);
   teff_TrackingParticlegranddaughterIsKsDaughter_vzV0CreationVertex->Fill(matchFound,creationVertexV0.Z());
   teff_TrackingParticlegranddaughterIsKsDaughter_lxyzV0CreationVertex->Fill(matchFound,Lxyz_beamspot_V0CreationVertex);
   teff_TrackingParticlegranddaughterIsKsDaughter_etaAntiS->Fill(matchFound,eta_antiS);
   h_TrackingParticlegranddaughterIsKsDaughter_etaAntiS_all->Fill(eta_antiS);
   if(matchFound)h_TrackingParticlegranddaughterIsKsDaughter_etaAntiS_nom->Fill(eta_antiS);
   teff_TrackingParticlegranddaughterIsKsDaughter_etaKs->Fill(matchFound,eta_V0);
   teff_TrackingParticlegranddaughterIsKsDaughter_pAntiS->Fill(matchFound,p_antiS);
   teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS->Fill(matchFound,pt_antiS);
   teff_TrackingParticlegranddaughterIsKsDaughter_pTKs->Fill(matchFound,pt_V0);
   teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS->Fill(matchFound,pt_antiS,eta_antiS);
   if(matchFound)h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS_nom->Fill(pt_antiS,eta_antiS);
   h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS_all->Fill(pt_antiS,eta_antiS);
   teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex->Fill(matchFound,pt_antiS,Lxyz_beamspot_V0CreationVertex);	
   if(matchFound)h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex_nom->Fill(pt_antiS,Lxyz_beamspot_V0CreationVertex);	
   h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex_all->Fill(pt_antiS,Lxyz_beamspot_V0CreationVertex);	
}

void MultiTrackValidator::FillTEffTrackingParticlegranddaughterIsPosPionLambdaDaughter(const TrackingParticle& tp, bool matchFound, TVector3 beamspot,TVector3 creationVertexV0,double pt_antiS,double eta_antiS, double pt_V0, double eta_V0, double p_antiS){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   double Lxyz = sqrt(Lxy*Lxy+ (beamspot.Z()-tp.vz()) * (beamspot.Z()-tp.vz()) );
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt->Fill(matchFound,tp.pt());
   if(matchFound)h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt_nom->Fill(tp.pt());
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt_all->Fill(tp.pt());
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_eta->Fill(matchFound,tp.eta());
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_phi->Fill(matchFound,tp.phi());
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy->Fill(matchFound,Lxy);
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy_all->Fill(Lxy);
   if(matchFound)h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy_nom->Fill(Lxy);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyz->Fill(matchFound,Lxyz);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_vz->Fill(matchFound,tp.vz());

   double Lxy_beamspot_V0CreationVertex = lxy(beamspot,creationVertexV0);
   double Lxyz_beamspot_V0CreationVertex = sqrt(Lxy_beamspot_V0CreationVertex*Lxy_beamspot_V0CreationVertex + (beamspot.Z()-creationVertexV0.Z()) * (beamspot.Z()-creationVertexV0.Z()) );
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex->Fill(matchFound,Lxy_beamspot_V0CreationVertex);
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex_all->Fill(Lxy_beamspot_V0CreationVertex);
   if(matchFound)h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex_nom->Fill(Lxy_beamspot_V0CreationVertex);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_vzV0CreationVertex->Fill(matchFound,creationVertexV0.Z());
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyzV0CreationVertex->Fill(matchFound,Lxyz_beamspot_V0CreationVertex);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS->Fill(matchFound,eta_antiS);
   h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS_all->Fill(eta_antiS);
   if(matchFound)h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS_nom->Fill(eta_antiS);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiLambda->Fill(matchFound,eta_V0);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pAntiS->Fill(matchFound,p_antiS);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS->Fill(matchFound,pt_antiS);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiLambda->Fill(matchFound,pt_V0);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS->Fill(matchFound,pt_antiS,eta_antiS);
   if(matchFound)h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS_nom->Fill(pt_antiS,eta_antiS);
   h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS_all->Fill(pt_antiS,eta_antiS);
   teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex->Fill(matchFound,pt_antiS,Lxyz_beamspot_V0CreationVertex);	
   if(matchFound)h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex_nom->Fill(pt_antiS,Lxyz_beamspot_V0CreationVertex);	
   h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex_all->Fill(pt_antiS,Lxyz_beamspot_V0CreationVertex);	
}

void MultiTrackValidator::FillTEffTrackingParticlegranddaughterIsNegProtonLambdaDaughter(const TrackingParticle& tp, bool matchFound, TVector3 beamspot,TVector3 creationVertexV0,double pt_antiS,double eta_antiS, double pt_V0, double eta_V0, double p_antiS){
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);
   double Lxyz = sqrt(Lxy*Lxy+ (beamspot.Z()-tp.vz()) * (beamspot.Z()-tp.vz()) );
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt->Fill(matchFound,tp.pt());
   h_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt_all->Fill(tp.pt());
   if(matchFound)h_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt_nom->Fill(tp.pt());
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_eta->Fill(matchFound,tp.eta());
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_phi->Fill(matchFound,tp.phi());
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy->Fill(matchFound,Lxy);
   h_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy_all->Fill(Lxy);
   if(matchFound)h_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy_nom->Fill(Lxy);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyz->Fill(matchFound,Lxyz);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_vz->Fill(matchFound,tp.vz());

   double Lxy_beamspot_V0CreationVertex = lxy(beamspot,creationVertexV0);
   double Lxyz_beamspot_V0CreationVertex = sqrt(Lxy_beamspot_V0CreationVertex*Lxy_beamspot_V0CreationVertex + (beamspot.Z()-creationVertexV0.Z()) * (beamspot.Z()-creationVertexV0.Z()) );
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyV0CreationVertex->Fill(matchFound,Lxy_beamspot_V0CreationVertex);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_vzV0CreationVertex->Fill(matchFound,creationVertexV0.Z());
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyzV0CreationVertex->Fill(matchFound,Lxyz_beamspot_V0CreationVertex);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS->Fill(matchFound,eta_antiS);
   h_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS_all->Fill(eta_antiS);
   if(matchFound)h_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS_nom->Fill(eta_antiS);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiLambda->Fill(matchFound,eta_V0);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pAntiS->Fill(matchFound,p_antiS);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS->Fill(matchFound,pt_antiS);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiLambda->Fill(matchFound,pt_V0);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS->Fill(matchFound,pt_antiS,eta_antiS);
   if(matchFound)h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS_nom->Fill(pt_antiS,eta_antiS);
   h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS_all->Fill(pt_antiS,eta_antiS);
   teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex->Fill(matchFound,pt_antiS,Lxyz_beamspot_V0CreationVertex);	
   if(matchFound)h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex_nom->Fill(pt_antiS,Lxyz_beamspot_V0CreationVertex);	
   h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex_all->Fill(pt_antiS,Lxyz_beamspot_V0CreationVertex);	
}


void MultiTrackValidator::FillHistoTrackingParticleToTrackDeltaRMatch(const TrackingParticle& tp, const reco::Track& tk, TVector3 beamspot, double deltaRmin){


  bool matchFound = false;
  if(deltaRmin < 0.03) matchFound = true;
  
   TVector3 vxyz(tp.vx(),tp.vy(),tp.vz());
   double Lxy = lxy(beamspot,vxyz);

   teff_TrackingParticleToTrackDeltaRMatch_charge->Fill(matchFound,tp.charge());
   teff_TrackingParticleToTrackDeltaRMatch_pt->Fill(matchFound,tp.pt());
   if( Lxy < 3.5 && abs(tp.eta()) < 2.5 ) teff_TrackingParticleToTrackDeltaRMatch_pt_lxy_eta_cut->Fill(matchFound,tp.pt());
   if( Lxy < 3.5 && abs(tp.eta()) < 2.5 && tp.status() == 1) teff_TrackingParticleToTrackDeltaRMatch_pt_lxy_eta_status1_cut->Fill(matchFound,tp.pt());
   teff_TrackingParticleToTrackDeltaRMatch_eta->Fill(matchFound,tp.eta());
   if( tp.pt() > 0.9 && Lxy < 3.5 ) teff_TrackingParticleToTrackDeltaRMatch_eta_pt_lxy_cut->Fill(matchFound,tp.eta());
   if( tp.pt() > 0.9 && Lxy < 3.5 && tp.status() == 1) teff_TrackingParticleToTrackDeltaRMatch_eta_pt_lxy_status1_cut->Fill(matchFound,tp.eta());
   teff_TrackingParticleToTrackDeltaRMatch_phi->Fill(matchFound,tp.phi());
   teff_TrackingParticleToTrackDeltaRMatch_pdgId->Fill(matchFound,tp.pdgId());
   teff_TrackingParticleToTrackDeltaRMatch_vx->Fill(matchFound,tp.vx());
   teff_TrackingParticleToTrackDeltaRMatch_vy->Fill(matchFound,tp.vy());
   teff_TrackingParticleToTrackDeltaRMatch_vz->Fill(matchFound,tp.vz());
   teff_TrackingParticleToTrackDeltaRMatch_lxy->Fill(matchFound,Lxy);
   if(tp.pt() > 0.9 && abs(tp.eta()) < 2.5)teff_TrackingParticleToTrackDeltaRMatch_lxy_pt_eta_cut->Fill(matchFound,Lxy);
   if(tp.pt() > 0.9 && abs(tp.eta()) < 2.5 && tp.status() == 1)teff_TrackingParticleToTrackDeltaRMatch_lxy_pt_eta_status1_cut->Fill(matchFound,Lxy);
   teff_TrackingParticleToTrackDeltaRMatch_status->Fill(matchFound,tp.status());
   teff_TrackingParticleToTrackDeltaRMatch_numberOfHits->Fill(matchFound,tp.numberOfHits());
  
}
		
void MultiTrackValidator::FillHistoGenParticlesPlusGEANTToTrackDeltaRMatch(const reco::GenParticle& gpPlusGEANT, const reco::Track& tk, TVector3 beamspot, double deltaRmin){
  bool matchFound = false;
  if(deltaRmin < 0.03) matchFound = true;
  
   TVector3 vxyz(gpPlusGEANT.vx(),gpPlusGEANT.vy(),gpPlusGEANT.vz());
   double Lxy = lxy(beamspot,vxyz);
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_charge->Fill(matchFound,gpPlusGEANT.charge());
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt->Fill(matchFound,gpPlusGEANT.pt());
   if( Lxy < 3.5 && abs(gpPlusGEANT.eta()) < 2.5 ) teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt_lxy_eta_cut->Fill(matchFound,gpPlusGEANT.pt());
   if( Lxy < 3.5 && abs(gpPlusGEANT.eta()) < 2.5 && gpPlusGEANT.status() == 1) teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt_lxy_eta_status1_cut->Fill(matchFound,gpPlusGEANT.pt());
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta->Fill(matchFound,gpPlusGEANT.eta());
   if( gpPlusGEANT.pt() > 0.9 && Lxy < 3.5 ) teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta_pt_lxy_cut->Fill(matchFound,gpPlusGEANT.eta());
   if( gpPlusGEANT.pt() > 0.9 && Lxy < 3.5 && gpPlusGEANT.status() == 1) teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta_pt_lxy_status1_cut->Fill(matchFound,gpPlusGEANT.eta());
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_phi->Fill(matchFound,gpPlusGEANT.phi());
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pdgId->Fill(matchFound,gpPlusGEANT.pdgId());
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vx->Fill(matchFound,gpPlusGEANT.vx());
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vy->Fill(matchFound,gpPlusGEANT.vy());
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vz->Fill(matchFound,gpPlusGEANT.vz());
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy->Fill(matchFound,Lxy);
   if(gpPlusGEANT.pt() > 0.9 && abs(gpPlusGEANT.eta()) < 2.5)teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy_pt_eta_cut->Fill(matchFound,Lxy);
   if(gpPlusGEANT.pt() > 0.9 && abs(gpPlusGEANT.eta()) < 2.5 && gpPlusGEANT.status() == 1)teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy_pt_eta_status1_cut->Fill(matchFound,Lxy);
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_status->Fill(matchFound,gpPlusGEANT.status());
   teff_GenParticlesPlusGEANTToTrackDeltaRMatch_numberOfHits->Fill(matchFound,tk.numberOfValidHits());
}

void MultiTrackValidator::analyze(const edm::Event& event, const edm::EventSetup& setup){
  using namespace reco;

  LogDebug("TrackValidator") << "\n====================================================" << "\n"
                             << "Analyzing new event" << "\n"
                             << "====================================================\n" << "\n";

  std::cout << "new event" << std::endl;






  edm::ESHandle<ParametersDefinerForTP> parametersDefinerTPHandle;
  setup.get<TrackAssociatorRecord>().get(parametersDefiner,parametersDefinerTPHandle);
  auto parametersDefinerTP = parametersDefinerTPHandle->clone();

  edm::Handle<TrackingParticleCollection>  TPCollectionHeff ;
  event.getByToken(label_tp_effic,TPCollectionHeff);
  TrackingParticleCollection const & tPCeff = *(TPCollectionHeff.product());

  //ADDED BY JARNE
  //get genParticlesPlusGEANT collection
  edm::Handle<std::vector<reco::GenParticle>> thegenParticlePlusGEANTHandle;
  event.getByToken(token_genparticlePlusGEANT, thegenParticlePlusGEANTHandle);
  const std::vector<reco::GenParticle>* genParticlePlusGEANTCollection = thegenParticlePlusGEANTHandle.product();
  //get beamspot collection
  edm::Handle<reco::BeamSpot> theBeamSpotHandle;
  event.getByToken(token_beamspot, theBeamSpotHandle);
  const reco::BeamSpot* theBeamSpot = theBeamSpotHandle.product();
  math::XYZPoint beamspotXYZ(theBeamSpot->position());
  TVector3 beamspot(beamspotXYZ.X(),beamspotXYZ.Y(),beamspotXYZ.Z());  
  //count number of antiS events
  int nAntiSInThisEvent = 0;
  for(size_t i=0; i<tPCeff.size(); ++i){
	const TrackingParticle& tp = tPCeff[i];
	if(tp.pdgId() == -1020000020) nAntiSInThisEvent++;
  }
  if(nAntiSInThisEvent>1) return;

  //loop over the GenParticlesPlusGEANT and print the kinematics of the antiS to be able to compare later with the antiS in the trackingparticlescollection 
  h_EvaluationCutsForEfiicientMC->Fill(0);
  for(size_t i_gen=0; i_gen<genParticlePlusGEANTCollection->size(); i_gen++){
	const reco::GenParticle& gpPlusGEANT = genParticlePlusGEANTCollection->at(i_gen);
	bool gpIsAntiS = false;
	if(gpPlusGEANT.pdgId() == -1020000020) gpIsAntiS = true;
	
/*	if(gpIsAntiS){
		cout << "pt, eta, phi, vx, vy, vz of genParticlePlusGEANT antiS: " << gpPlusGEANT.pt() << ", "<< gpPlusGEANT.eta() << ", "<< gpPlusGEANT.phi() << ", "<< gpPlusGEANT.vx() << ", "<< gpPlusGEANT.vy() << ", "<< gpPlusGEANT.vz() << endl;
	}
*/
	//first check if you are really going to the granddaughters you want
	if(gpIsAntiS){
		if(fabs(gpPlusGEANT.eta())>4) continue;
		h_EvaluationCutsForEfiicientMC->Fill(1);
	//	std::cout << "gen particles is antiS" << std::endl;
		if(gpPlusGEANT.numberOfDaughters()  == 2){
			//std::cout << "gen particle has 2 daughters" << std::endl;
			if(gpPlusGEANT.daughter(0)->numberOfDaughters()  == 2 && gpPlusGEANT.daughter(1)->numberOfDaughters() == 2){
				//std::cout << "gen particle has 4 grandddaughters" << std::endl;
				//std::cout << "daughter0 pdgId: " << gpPlusGEANT.daughter(0)->pdgId()  << std::endl;
				//std::cout << "daughter1 pdgId: " << gpPlusGEANT.daughter(1)->pdgId()  << std::endl;
				const reco::Candidate* Ks = nullptr;
				const reco::Candidate* antiL = nullptr ;
				if(fabs(gpPlusGEANT.daughter(0)->pdgId()) == 310)Ks = gpPlusGEANT.daughter(0); //first one is always the Ks, cause you put that one first
				if(gpPlusGEANT.daughter(1)->pdgId() == -3122)antiL = gpPlusGEANT.daughter(1);
				bool goodDaughtersKs = false;
				bool goodDaughtersantiL = false;
			  	if(Ks && antiL){
					//std::cout << "gen particle has Ks and Lambda as daughter" << std::endl;
					if((Ks->daughter(0)->pdgId() == 211 && Ks->daughter(1)->pdgId() == -211) || (Ks->daughter(0)->pdgId() == -211 && Ks->daughter(1)->pdgId() == 211) )goodDaughtersKs = true;
					if((antiL->daughter(0)->pdgId() == -2212 && antiL->daughter(1)->pdgId() == 211) || (antiL->daughter(0)->pdgId() == 211 && antiL->daughter(1)->pdgId() == -2212) )goodDaughtersantiL = true;
				}	
				bool goodGranddaughtersAntiS = false;
				if(goodDaughtersKs && goodDaughtersantiL) goodGranddaughtersAntiS = true;
				//if(goodGranddaughtersAntiS) std::cout << "gen particle has correct granddaughters" << std::endl;
				if(goodGranddaughtersAntiS) h_EvaluationCutsForEfiicientMC->Fill(2);
				//double lxy_antiSInteraction = 999.;
				double lxy_KsDaughter0 = 999.;
				double lxy_KsDaughter1 = 999.;
				double lxy_LambdaDaughter0 = 999.;
				double lxy_LambdaDaughter1 = 999.;
				if(goodGranddaughtersAntiS){//calculate the displacement

				//	TVector3 vertex_V0(Ks->vx(),Ks->vy(),Ks->vz());
				//	lxy_antiSInteraction = lxy(beamspot,vertex_V0);
					
					TVector3 vertex_KsDaughter0(Ks->daughter(0)->vx(),Ks->daughter(0)->vy(),Ks->daughter(0)->vz());
					lxy_KsDaughter0 = lxy(beamspot,vertex_KsDaughter0);
					
					TVector3 vertex_KsDaughter1(Ks->daughter(1)->vx(),Ks->daughter(1)->vy(),Ks->daughter(1)->vz());
					lxy_KsDaughter1 = lxy(beamspot,vertex_KsDaughter1);

					TVector3 vertex_LambdaDaughter0(antiL->daughter(0)->vx(),antiL->daughter(0)->vy(),antiL->daughter(0)->vz());
					lxy_LambdaDaughter0 = lxy(beamspot,vertex_LambdaDaughter0);
					
					TVector3 vertex_LambdaDaughter1(antiL->daughter(1)->vx(),antiL->daughter(1)->vy(),antiL->daughter(1)->vz());
					lxy_LambdaDaughter1 = lxy(beamspot,vertex_LambdaDaughter1);
					
				}
				bool EventSurvivesCutsForEfficientMC = false;			
			 	if(goodGranddaughtersAntiS  && antiL->daughter(0)->pt() > 0.1 && antiL->daughter(1)->pt() > 0.1 && Ks->daughter(0)->pt() > 0.1 && Ks->daughter(1)->pt() > 0.1 && lxy_KsDaughter0 < 70 && lxy_KsDaughter1 < 70 && lxy_LambdaDaughter0 < 70 && lxy_LambdaDaughter1 < 70) EventSurvivesCutsForEfficientMC = true;
				if(EventSurvivesCutsForEfficientMC) h_EvaluationCutsForEfiicientMC->Fill(3);
				if(goodGranddaughtersAntiS && reco::deltaR(antiL->eta(),antiL->phi(),Ks->eta(),Ks->phi())<3)h_EvaluationCutsForEfiicientMC->Fill(4);
			}
		}
	}
  }

  bool granddaughterIsNegKsDaughterAndReconstructed = false;
  bool granddaughterIsPosKsDaughterAndReconstructed = false;
  bool granddaughterIsPosPionLambdaDaughterAndReconstructed = false;
  bool granddaughterIsNegProtonLambdaDaughterAndReconstructed = false;
  int nReconstructedAntiSGranddaughtersInThisEvent = 0;
  int nReconstructableAntiSGranddaughtersInThisEvent = 0;
  //cout << "label.size " << label.size() << endl;
  for(size_t i=0; i<tPCeff.size(); ++i) {

     bool granddaughterIsNegKsDaughter = false;
     bool granddaughterIsPosKsDaughter = false;
     bool granddaughterIsPosPionLambdaDaughter = false;
     bool granddaughterIsNegProtonLambdaDaughter = false;

     bool matchingTrackFound = false;
     const reco::Track *matchedTrackPointer = nullptr;

     TrackingParticleRef tpr(TPCollectionHeff,i);
     const TrackingParticle& tp = tPCeff[i];
     if(tp.charge() == 0) continue;

     int w=0; //counter counting the number of sets of histograms
	  for (unsigned int ww=0;ww<associators.size();ww++){
	    for (unsigned int www=0;www<label.size();www++, w++){ // need to increment w here, since there are many continues in the loop body
	      edm::Handle<View<Track> >  trackCollection;
	      if(!event.getByToken(labelToken[www], trackCollection)&&ignoremissingtkcollection_)continue;

	      reco::SimToRecoCollection const * simRecCollP=nullptr;
	      reco::SimToRecoCollection simRecCollL;

	      edm::Handle<reco::TrackToTrackingParticleAssociator> theAssociator;
	      event.getByToken(associatorTokens[ww], theAssociator);
	       simRecCollL = std::move(theAssociator->associateSimToReco(trackCollection,TPCollectionHeff));

	      const edm::View<Track> *trackC = (trackCollection.product());

	      if(i == 0){//only need to fill this once, not for every trackingParticle, because like that you would have duplicates

		//find best matching track to trackingparticle, based on deltaR
		double deltaRmintktp = 999;
 		double best_i_tktp = 0;
		for(size_t i_tk=0; i_tk<trackC->size(); ++i_tk) {

			  const Track& track = trackC->at(i_tk);
			  FillHistoTrack(track,beamspot);

			  double tp_phi = tp.phi();
			  double track_phi = track.phi();
			  double deltaPhi = reco::deltaPhi(tp_phi,track_phi); 
			  double tp_eta = tp.eta();
			  double track_eta = track.eta();
			  double deltaEta = tp_eta-track_eta;
			  double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
			  h_TrackingParticleToTrackDeltaRMatch_deltaR->Fill(deltaR);
			  if(deltaR < deltaRmintktp) {
				deltaRmintktp = deltaR;
				best_i_tktp = i_tk;
			  }
			
		}
		if(trackC->size()>0)FillHistoTrackingParticleToTrackDeltaRMatch(tp,trackC->at(best_i_tktp),beamspot,deltaRmintktp);

		//find best matching track to genParticlePlusGEANT based on DeltaR
		for(size_t i_gen=0; i_gen<genParticlePlusGEANTCollection->size(); i_gen++){
		   const reco::GenParticle& gpPlusGEANT = genParticlePlusGEANTCollection->at(i_gen);
		   double deltaRmin = 999;
		   size_t best_i_tk = 0;
		   for(size_t i_tk=0; i_tk<trackC->size(); ++i_tk) {
			  const Track& tk = trackC->at(i_tk);

			  double gpPlusGEANT_phi = gpPlusGEANT.phi();
			  double track_phi = tk.phi();
			  double deltaPhi = reco::deltaPhi(gpPlusGEANT_phi,track_phi); 
			  double gpPlusGEANT_eta = gpPlusGEANT.eta();
			  double track_eta = tk.eta();
			  double deltaEta = gpPlusGEANT_eta-track_eta;
			  double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);

			  h_GenParticlesPlusGEANTToTrackDeltaRMatch_deltaR->Fill(deltaR);
			  if(deltaR < deltaRmin) {
				deltaRmin = deltaR;
				best_i_tk = i_tk;
			  }
  
		   }
		   if(trackC->size()>0)FillHistoGenParticlesPlusGEANTToTrackDeltaRMatch(gpPlusGEANT, trackC->at(best_i_tk), beamspot, deltaRmin);	
		}

	      } 


	      simRecCollP = &simRecCollL;
	      reco::SimToRecoCollection const & simRecColl = *simRecCollP;

		if(simRecColl.find(tpr) != simRecColl.end()){
		  auto const & rt = simRecColl[tpr];
		  if (rt.size()!=0) {
		    // isRecoMatched = true; // UNUSED
		    matchedTrackPointer = rt.begin()->first.get();
		    //cout << "Jarne:  TrackingParticle #" << i << " with pt,eta,phi:  << << tp.pt() << ", "  << tp.eta() << " , " << tp.phi() <<  " ASSOCIATED with quality:" << rt.begin()->second << endl;
		    matchingTrackFound = true;
		  }
		}else{
		    //<< "Jarne:   TrackingParticle #" << i
		    //<< " with pt,eta,phi: "<< tp.pt() << " , " < tp.eta() << " , " << tp.phi() << " , "< " NOT associated to any reco::Track" << endl;
                    matchingTrackFound = false;
		}
	   }//end loop over www
	  }//end loop over ww

   //fill histograms with kinematic variables: two categories: the ones where a match was found and the ones where no match was found
   FillHistoTrackingParticle(tp,beamspot); 
   if(matchingTrackFound)FillHistoTrackingParticleMatchFound(tp,beamspot);
   else FillHistoTrackingParticleMatchNotFound(tp,beamspot);

   //if matched track was found then check how good trackingparticle and track match
   if(matchedTrackPointer)FillHistoMatchingTrackingParticleToTrack(matchedTrackPointer, tp,beamspot);
   //fill efficiencies for tp matching on hits
   FillTEffTrackingParticle(tp,matchingTrackFound,beamspot);

   //big loop which checks if the current tp is a granddaughter of the antiS
   TVector3 creationVertexV0(999.,999.,999.);
   double pt_antiS = 999.; double p_antiS = 999.; double pt_V0 = 999.;
   double eta_antiS = 999.;double eta_V0 = 999.;

   if(abs(tp.pdgId()) == 211 || tp.pdgId() == - 2212){//found a charged pion or antiproton
	double granddaughterVx = tp.vx();
	double granddaughterVy = tp.vy();
	double granddaughterVz = tp.vz();
	for(size_t j=0; j<tPCeff.size(); ++j) {//now find the daughters which have a decay vertex = the production vertex of the granddaughters
     		const TrackingParticle& tp_daughter = tPCeff[j];
		if(abs(tp_daughter.pdgId()) == 310 || tp_daughter.pdgId() == -3122){//daughter has to be a Ks or Lambda
			tv_iterator tp_daughter_firstDecayVertex = tp_daughter.decayVertices_begin();
			double daughterdecayVx = (**tp_daughter_firstDecayVertex).position().X();
			double daughterdecayVy = (**tp_daughter_firstDecayVertex).position().Y();
			double daughterdecayVz = (**tp_daughter_firstDecayVertex).position().Z();	
			TVector3 creationVertexV0Temp( tp_daughter.vx(), tp_daughter.vy(), tp_daughter.vz()); creationVertexV0 = creationVertexV0Temp; pt_V0=tp_daughter.pt();eta_V0 = tp_daughter.eta();
		 	if(granddaughterVx == daughterdecayVx && granddaughterVy == daughterdecayVy && granddaughterVz == daughterdecayVz){//daughter decay vertex has to match the granddaughter creation vertex
				for(size_t k=0; k<tPCeff.size(); ++k) {//loop to find the antiS
					const TrackingParticle& tp_S = tPCeff[k];
					if(tp_S.pdgId() == -1020000020){//found the S
						tv_iterator tp_S_firstDecayVertex = tp_S.decayVertices_begin();
						double SdecayVx = (**tp_S_firstDecayVertex).position().X();
						double SdecayVy = (**tp_S_firstDecayVertex).position().Y();
						double SdecayVz = (**tp_S_firstDecayVertex).position().Z();
						pt_antiS = tp_S.pt(); p_antiS = tp_S.p(); eta_antiS = tp_S.eta();
						if(tp_daughter.vx() == SdecayVx && tp_daughter.vy() == SdecayVy && tp_daughter.vz() == SdecayVz){
							//cout << "pt, eta, phi, vx, vy, vz of trackingParticle antiS: " << tp_S.pt() << ", "<< tp_S.eta() << ", "<< tp_S.phi() << ", "<< tp_S.vx() << ", "<< tp_S.vy() << ", "<< tp_S.vz() << endl;
							nReconstructableAntiSGranddaughtersInThisEvent++;
							if(matchingTrackFound)nReconstructedAntiSGranddaughtersInThisEvent++;	
							if(abs(tp_daughter.pdgId()) == 310 && tp.pdgId() == 211){ granddaughterIsPosKsDaughter = true; }
							else if(abs(tp_daughter.pdgId()) == 310 && tp.pdgId() == -211){ granddaughterIsNegKsDaughter = true;}
							else if(abs(tp_daughter.pdgId()) == 3122 && tp.pdgId() == 211){ granddaughterIsPosPionLambdaDaughter = true;}
							else if(abs(tp_daughter.pdgId()) == 3122 && tp.pdgId() == -2212){ granddaughterIsNegProtonLambdaDaughter = true;}
							
							if(granddaughterIsPosKsDaughter && matchingTrackFound) granddaughterIsPosKsDaughterAndReconstructed = true;
							else if(granddaughterIsNegKsDaughter && matchingTrackFound) granddaughterIsNegKsDaughterAndReconstructed = true;
							else if(granddaughterIsPosPionLambdaDaughter && matchingTrackFound) granddaughterIsPosPionLambdaDaughterAndReconstructed = true;
							else if(granddaughterIsNegProtonLambdaDaughter && matchingTrackFound) granddaughterIsNegProtonLambdaDaughterAndReconstructed = true;
							/*
							std::cout << "antiS chain reconstructed, antiS creation vertex: " << tp_S.vx() << "," << tp_S.vy() << "," << tp_S.vz() << std::endl;
							std::cout << "antiS chain reconstructed, antiS decay vertex: " << SdecayVx << "," << SdecayVy << "," << SdecayVz << std::endl;
							std::cout << "antiS chain reconstructed, daughter creation vertex: " << tp_daughter.vx() << "," << tp_daughter.vy() << "," << tp_daughter.vz() << std::endl;
							std::cout << "antiS chain reconstructed, daughter decay vertex: " << daughterdecayVx << "," << daughterdecayVy << "," << daughterdecayVz << std::endl;
							std::cout << "antiS chain reconstructed, granddaughter creation vertex: " << tp.vx() << "," << tp.vy() << "," << tp.vz() << std::endl;
							std::cout << "antiS chain reconstructed, granddaughter pt: " << tp.pt() << std::endl;
							*/
						}
					}//end if antiS
				}//end loop over the tp to find the antiS
			}//end check if granddaughter vertex matches the the daughter decay vertex	
		}//end check for pdgId daughter
	}//end loop over tp to find daughters
   }//end check of granddaughter pdgId

 if(granddaughterIsPosKsDaughter || granddaughterIsNegKsDaughter) FillTEffTrackingParticlegranddaughterIsKsDaughter(tp,matchingTrackFound,beamspot,creationVertexV0,pt_antiS,eta_antiS,pt_V0,eta_V0, p_antiS);
 else if(granddaughterIsPosPionLambdaDaughter) FillTEffTrackingParticlegranddaughterIsPosPionLambdaDaughter(tp,matchingTrackFound,beamspot,creationVertexV0,pt_antiS,eta_antiS,pt_V0,eta_V0, p_antiS);
 else if(granddaughterIsNegProtonLambdaDaughter) FillTEffTrackingParticlegranddaughterIsNegProtonLambdaDaughter(tp,matchingTrackFound,beamspot,creationVertexV0,pt_antiS,eta_antiS,pt_V0,eta_V0, p_antiS);

 if(matchingTrackFound){
	 if(granddaughterIsPosKsDaughter) FillHistoTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed(tp,beamspot);
	 else if(granddaughterIsNegKsDaughter) FillHitsoTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed(tp,beamspot);
	 else if(granddaughterIsPosPionLambdaDaughter) FillHistoTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed(tp,beamspot);
	 else if(granddaughterIsNegProtonLambdaDaughter) FillHistoTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed(tp,beamspot);
 }
 else{
	 if(granddaughterIsPosKsDaughter) FillHistoTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed(tp,beamspot);
	 else if(granddaughterIsNegKsDaughter) FillHitsoTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed(tp,beamspot);
	 else if(granddaughterIsPosPionLambdaDaughter) FillHistoTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed(tp,beamspot);
	 else if(granddaughterIsNegProtonLambdaDaughter) FillHistoTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed(tp,beamspot);
 }

 string particle_type = "None";
 if(granddaughterIsPosKsDaughter) particle_type = "granddaughterIsPosKsDaughter";
 else if(granddaughterIsNegKsDaughter) particle_type = "granddaughterIsNegKsDaughter";
 else if(granddaughterIsPosPionLambdaDaughter) particle_type = "granddaughterIsPosPionLambdaDaughter";
 else if(granddaughterIsNegProtonLambdaDaughter) particle_type = "granddaughterIsNegProtonLambdaDaughter";
 
 FillTEffTrackingParticlegranddaugterAntiS(tp, matchingTrackFound, beamspot, particle_type);

 }//end loop over selected_tPCeff

 if(nReconstructableAntiSGranddaughtersInThisEvent==4) h_TrackingParticlegranddaughterantiS_nRecoGranddaughters_with_charged_granddaughters->Fill(nReconstructedAntiSGranddaughtersInThisEvent); 
 h_TrackingParticlegranddaughterantiS_nRecoGranddaughters_all_granddaughters->Fill(nReconstructedAntiSGranddaughtersInThisEvent); 
 h_TrackingParticlegranddaughterantiS_nReconstructableGranddaughters_nRecoGranddaughters->Fill(nReconstructableAntiSGranddaughtersInThisEvent,nReconstructedAntiSGranddaughtersInThisEvent);


  //LUT for which particles got reconstructed: Ks vs Lambda
  //for L
  int reconstructedLDaughterCombination = 0;
  if(granddaughterIsPosPionLambdaDaughterAndReconstructed && granddaughterIsNegProtonLambdaDaughterAndReconstructed) reconstructedLDaughterCombination = 4;
  if(!granddaughterIsPosPionLambdaDaughterAndReconstructed && granddaughterIsNegProtonLambdaDaughterAndReconstructed) reconstructedLDaughterCombination = 3;
  if(granddaughterIsPosPionLambdaDaughterAndReconstructed && !granddaughterIsNegProtonLambdaDaughterAndReconstructed) reconstructedLDaughterCombination = 2;
  if(!granddaughterIsPosPionLambdaDaughterAndReconstructed && !granddaughterIsNegProtonLambdaDaughterAndReconstructed) reconstructedLDaughterCombination = 1;

  //for Ks
  int reconstructedKsDaughterCombination = 0;
  if(granddaughterIsPosKsDaughterAndReconstructed && granddaughterIsNegKsDaughterAndReconstructed) reconstructedKsDaughterCombination = 4;
  if(!granddaughterIsPosKsDaughterAndReconstructed && granddaughterIsNegKsDaughterAndReconstructed) reconstructedKsDaughterCombination = 3;
  if(granddaughterIsPosKsDaughterAndReconstructed && !granddaughterIsNegKsDaughterAndReconstructed) reconstructedKsDaughterCombination = 2;
  if(!granddaughterIsPosKsDaughterAndReconstructed && !granddaughterIsNegKsDaughterAndReconstructed) reconstructedKsDaughterCombination = 1;


  h_reconstructedKsDaughterCombination_reconstructedLDaughterCombination->Fill(reconstructedLDaughterCombination,reconstructedKsDaughterCombination);
  if(nReconstructableAntiSGranddaughtersInThisEvent==4) h_reconstructedKsDaughterCombination_reconstructedLDaughterCombination_4reconstructableGranddaughtersOnly->Fill(reconstructedLDaughterCombination,reconstructedKsDaughterCombination);

  


  //END ADDED BY JARNE








/*

  edm::Handle<TrackingParticleCollection>  TPCollectionHfake ;
  event.getByToken(label_tp_fake,TPCollectionHfake);




  if(parametersDefinerIsCosmic_) {
    edm::Handle<SimHitTPAssociationProducer::SimHitTPAssociationList> simHitsTPAssoc;
    //warning: make sure the TP collection used in the map is the same used in the MTV!
    event.getByToken(_simHitTpMapTag,simHitsTPAssoc);
    parametersDefinerTP->initEvent(simHitsTPAssoc);
    cosmictpSelector.initEvent(simHitsTPAssoc);
  }

  const reco::Vertex::Point *thePVposition = nullptr;
  const TrackingVertex::LorentzVector *theSimPVPosition = nullptr;
  if(doPlotsOnlyForTruePV_ || doPVAssociationPlots_) {
    edm::Handle<TrackingVertexCollection> htv;
    event.getByToken(label_tv, htv);

    edm::Handle<edm::View<reco::Vertex> > hvertex;
    event.getByToken(recoVertexToken_, hvertex);

    edm::Handle<reco::VertexToTrackingVertexAssociator> hvassociator;
    event.getByToken(vertexAssociatorToken_, hvassociator);

    auto v_r2s = hvassociator->associateRecoToSim(hvertex, htv);
    auto pvPtr = hvertex->refAt(0);
    if(!(pvPtr->isFake() || pvPtr->ndof() < 0)) { // skip junk vertices
      auto pvFound = v_r2s.find(pvPtr);
      if(pvFound != v_r2s.end()) {
        int simPVindex = -1;
        int i=0;
        for(const auto& vertexRefQuality: pvFound->val) {
          const TrackingVertex& tv = *(vertexRefQuality.first);
          if(tv.eventId().event() == 0 && tv.eventId().bunchCrossing() == 0) {
            simPVindex = i;
          }
          ++i;
        }
        if(simPVindex >= 0) {
          if(doPVAssociationPlots_) {
            thePVposition = &(pvPtr->position());
            theSimPVPosition = &(pvFound->val[simPVindex].first->position());
          }
        }
        else if(doPlotsOnlyForTruePV_)
          return;
      }
      else if(doPlotsOnlyForTruePV_)
        return;
    }
    else if(doPlotsOnlyForTruePV_)
      return;
  }

  edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
  event.getByToken(bsSrc,recoBeamSpotHandle);
  reco::BeamSpot const & bs = *recoBeamSpotHandle;

  edm::Handle< std::vector<PileupSummaryInfo> > puinfoH;
  event.getByToken(label_pileupinfo,puinfoH);
  PileupSummaryInfo puinfo;

  for (unsigned int puinfo_ite=0;puinfo_ite<(*puinfoH).size();++puinfo_ite){
    if ((*puinfoH)[puinfo_ite].getBunchCrossing()==0){
      puinfo=(*puinfoH)[puinfo_ite];
      break;
    }
  }

  
  //edm::Handle<TrackingVertexCollection> tvH;
  //event.getByToken(label_tv,tvH);
  //TrackingVertexCollection const & tv = *tvH;

  // Calculate the number of 3D layers for TPs
  //
  // I would have preferred to produce the ValueMap to Event and read
  // it from there, but there would have been quite some number of
  // knock-on effects, and again the fact that we take two TP
  // collections do not support Ref<TP>'s would have complicated that.
  //
  // In principle we could use the SimHitTPAssociationList read above
  // for parametersDefinerIsCosmic_, but since we don't currently
  // support Ref<TP>s, we can't in general use it since eff/fake TP
  // collections can, in general, be different.
  TrackingParticleNumberOfLayers tpNumberOfLayersAlgo(event, simHitTokens_);
  auto nlayers_tPCeff_ptrs = tpNumberOfLayersAlgo.calculate(TPCollectionHeff, setup);
  const auto& nLayers_tPCeff = *(std::get<TrackingParticleNumberOfLayers::nTrackerLayers>(nlayers_tPCeff_ptrs));
  const auto& nPixelLayers_tPCeff = *(std::get<TrackingParticleNumberOfLayers::nPixelLayers>(nlayers_tPCeff_ptrs));
  const auto& nStripMonoAndStereoLayers_tPCeff = *(std::get<TrackingParticleNumberOfLayers::nStripMonoAndStereoLayers>(nlayers_tPCeff_ptrs));

  // Precalculate TP selection (for efficiency), and momentum and vertex wrt PCA
  //
  // TODO: ParametersDefinerForTP ESProduct needs to be changed to
  // EDProduct because of consumes.
  //
  // In principle, we could just precalculate the momentum and vertex
  // wrt PCA for all TPs for once and put that to the event. To avoid
  // repetitive calculations those should be calculated only once for
  // each TP. That would imply that we should access TPs via Refs
  // (i.e. View) in here, since, in general, the eff and fake TP
  // collections can be different (and at least HI seems to use that
  // feature). This would further imply that the
  // RecoToSimCollection/SimToRecoCollection should be changed to use
  // View<TP> instead of vector<TP>, and migrate everything.
  //
  // Or we could take only one input TP collection, and do another
  // TP-selection to obtain the "fake" collection like we already do
  // for "efficiency" TPs.
  std::vector<size_t> selected_tPCeff;
  std::vector<std::tuple<TrackingParticle::Vector, TrackingParticle::Point>> momVert_tPCeff;
  selected_tPCeff.reserve(tPCeff.size());
  momVert_tPCeff.reserve(tPCeff.size());
  int nIntimeTPs = 0;
  if(parametersDefinerIsCosmic_) {
    for(size_t j=0; j<tPCeff.size(); ++j) {
      TrackingParticleRef tpr(TPCollectionHeff, j);

      TrackingParticle::Vector momentum = parametersDefinerTP->momentum(event,setup,tpr);
      TrackingParticle::Point vertex = parametersDefinerTP->vertex(event,setup,tpr);
      if(doSimPlots_) {
        histoProducerAlgo_->fill_generic_simTrack_histos(momentum, vertex, tpr->eventId().bunchCrossing());
      }
      if(tpr->eventId().bunchCrossing() == 0)
        ++nIntimeTPs;

      if(cosmictpSelector(tpr,&bs,event,setup)) {
        selected_tPCeff.push_back(j);
        momVert_tPCeff.emplace_back(momentum, vertex);
      }
    }
  }
  else {
    size_t j=0;
    for(auto const& tp: tPCeff) {

      // TODO: do we want to fill these from all TPs that include IT
      // and OOT (as below), or limit to IT+OOT TPs passing tpSelector
      // (as it was before)? The latter would require another instance
      // of tpSelector with intimeOnly=False.
      if(doSimPlots_) {
        histoProducerAlgo_->fill_generic_simTrack_histos(tp.momentum(), tp.vertex(), tp.eventId().bunchCrossing());
      }
      if(tp.eventId().bunchCrossing() == 0)
        ++nIntimeTPs;

      if(tpSelector(tp)) {
        selected_tPCeff.push_back(j);
	TrackingParticleRef tpr(TPCollectionHeff, j);
        TrackingParticle::Vector momentum = parametersDefinerTP->momentum(event,setup,tpr);
        TrackingParticle::Point vertex = parametersDefinerTP->vertex(event,setup,tpr);
        momVert_tPCeff.emplace_back(momentum, vertex);
      }
      ++j;
    }
  }
  if(doSimPlots_) {
    histoProducerAlgo_->fill_simTrackBased_histos(nIntimeTPs);
  }

  //calculate dR for TPs
  float dR_tPCeff[tPCeff.size()];
  {
    float etaL[tPCeff.size()], phiL[tPCeff.size()];
    for(size_t iTP: selected_tPCeff) {
      //calculare dR wrt inclusive collection (also with PU, low pT, displaced)
      auto const& tp2 = tPCeff[iTP];
      auto  && p = tp2.momentum();
      etaL[iTP] = etaFromXYZ(p.x(),p.y(),p.z());
      phiL[iTP] = atan2f(p.y(),p.x());
    }
    auto i=0U;
    for ( auto const & tp : tPCeff) {
      double dR = std::numeric_limits<double>::max();
      if(dRtpSelector(tp)) {//only for those needed for efficiency!
        auto  && p = tp.momentum();
        float eta = etaFromXYZ(p.x(),p.y(),p.z());
        float phi = atan2f(p.y(),p.x());
        for(size_t iTP: selected_tPCeff) {
          //calculare dR wrt inclusive collection (also with PU, low pT, displaced)
	  if (i==iTP) {continue;}
          auto dR_tmp = reco::deltaR2(eta, phi, etaL[iTP], phiL[iTP]);
          if (dR_tmp<dR) dR=dR_tmp;
        }  // ttp2 (iTP)
      }
      dR_tPCeff[i++] = std::sqrt(dR);
    }  // tp
  }

  edm::Handle<View<Track> >  trackCollectionForDrCalculation;
  if(calculateDrSingleCollection_) {
    event.getByToken(labelTokenForDrCalculation, trackCollectionForDrCalculation);
  }

  // dE/dx
  // at some point this could be generalized, with a vector of tags and a corresponding vector of Handles
  // I'm writing the interface such to take vectors of ValueMaps
  std::vector<const edm::ValueMap<reco::DeDxData> *> v_dEdx;
  if(dodEdxPlots_) {
    edm::Handle<edm::ValueMap<reco::DeDxData> > dEdx1Handle;
    edm::Handle<edm::ValueMap<reco::DeDxData> > dEdx2Handle;
    event.getByToken(m_dEdx1Tag, dEdx1Handle);
    event.getByToken(m_dEdx2Tag, dEdx2Handle);
    v_dEdx.push_back(dEdx1Handle.product());
    v_dEdx.push_back(dEdx2Handle.product());
  }

  int w=0; //counter counting the number of sets of histograms
  for (unsigned int ww=0;ww<associators.size();ww++){
    for (unsigned int www=0;www<label.size();www++, w++){ // need to increment w here, since there are many continues in the loop body
      //
      //get collections from the event
      //
      edm::Handle<View<Track> >  trackCollection;
      if(!event.getByToken(labelToken[www], trackCollection)&&ignoremissingtkcollection_)continue;

      reco::RecoToSimCollection const * recSimCollP=nullptr;
      reco::SimToRecoCollection const * simRecCollP=nullptr;
      reco::RecoToSimCollection recSimCollL;
      reco::SimToRecoCollection simRecCollL;

      //associate tracks
      LogTrace("TrackValidator") << "Analyzing "
                                 << label[www] << " with "
                                 << associators[ww] <<"\n";
      if(UseAssociators){
        edm::Handle<reco::TrackToTrackingParticleAssociator> theAssociator;
        event.getByToken(associatorTokens[ww], theAssociator);

	LogTrace("TrackValidator") << "Calling associateRecoToSim method" << "\n";
	recSimCollL = std::move(theAssociator->associateRecoToSim(trackCollection,
                                                                   TPCollectionHfake));
         recSimCollP = &recSimCollL;
	LogTrace("TrackValidator") << "Calling associateSimToReco method" << "\n";
	simRecCollL = std::move(theAssociator->associateSimToReco(trackCollection,
                                                                   TPCollectionHeff));
        simRecCollP = &simRecCollL;
      }
      else{
	Handle<reco::SimToRecoCollection > simtorecoCollectionH;
	event.getByToken(associatormapStRs[ww], simtorecoCollectionH);
	simRecCollP = simtorecoCollectionH.product();

        // We need to filter the associations of the current track
        // collection only from SimToReco collection, otherwise the
        // SimToReco histograms get false entries
        simRecCollL = associationMapFilterValues(*simRecCollP, *trackCollection);
        simRecCollP = &simRecCollL;

	Handle<reco::RecoToSimCollection > recotosimCollectionH;
	event.getByToken(associatormapRtSs[ww],recotosimCollectionH);
	recSimCollP = recotosimCollectionH.product();

        // In general, we should filter also the RecoToSim collection.
        // But, that would require changing the input type of TPs to
        // View<TrackingParticle>, and either replace current
        // associator interfaces with (View<Track>, View<TP>) or
        // adding the View,View interface (same goes for
        // RefToBaseVector,RefToBaseVector). Since there is currently
        // no compelling-enough use-case, we do not filter the
        // RecoToSim collection here. If an association using a subset
        // of the Sim collection is needed, user has to produce such
        // an association explicitly.
      }

      reco::RecoToSimCollection const & recSimColl = *recSimCollP;
      reco::SimToRecoCollection const & simRecColl = *simRecCollP;
 

      // Fill seed-specific histograms
      if(doSeedPlots_) {
        edm::Handle<std::vector<int>> hseedToTrack;
        event.getByToken(seedToTrackTokens_[www], hseedToTrack);
        const int failed = std::count(hseedToTrack->begin(), hseedToTrack->end(), -1);
        histoProducerAlgo_->fill_seed_histos(www, failed, hseedToTrack->size());
      }


      // ########################################################
      // fill simulation histograms (LOOP OVER TRACKINGPARTICLES)
      // ########################################################

      //compute number of tracks per eta interval
      //
      LogTrace("TrackValidator") << "\n# of TrackingParticles: " << tPCeff.size() << "\n";
      int ats(0);  	  //This counter counts the number of simTracks that are "associated" to recoTracks
      int st(0);    	  //This counter counts the number of simulated tracks passing the MTV selection (i.e. tpSelector(tp) )
      unsigned sts(0);   //This counter counts the number of simTracks surviving the bunchcrossing cut
      unsigned asts(0);  //This counter counts the number of simTracks that are "associated" to recoTracks surviving the bunchcrossing cut

      //loop over already-selected TPs for tracking efficiency
      for(size_t i=0; i<selected_tPCeff.size(); ++i) {
        size_t iTP = selected_tPCeff[i];
	TrackingParticleRef tpr(TPCollectionHeff, iTP);
	const TrackingParticle& tp = tPCeff[iTP];
        auto const& momVert = momVert_tPCeff[i];
	TrackingParticle::Vector momentumTP;
	TrackingParticle::Point vertexTP;

	double dxySim(0);
	double dzSim(0);
        double dxyPVSim = 0;
        double dzPVSim = 0;
	double dR=dR_tPCeff[iTP];

	//---------- THIS PART HAS TO BE CLEANED UP. THE PARAMETER DEFINER WAS NOT MEANT TO BE USED IN THIS WAY ----------
	//If the TrackingParticle is collison like, get the momentum and vertex at production state
	if(!parametersDefinerIsCosmic_)
	  {
	    momentumTP = tp.momentum();
	    vertexTP = tp.vertex();
	    //Calcualte the impact parameters w.r.t. PCA
	    const TrackingParticle::Vector& momentum = std::get<TrackingParticle::Vector>(momVert);
	    const TrackingParticle::Point& vertex = std::get<TrackingParticle::Point>(momVert);
	    dxySim = (-vertex.x()*sin(momentum.phi())+vertex.y()*cos(momentum.phi()));
	    dzSim = vertex.z() - (vertex.x()*momentum.x()+vertex.y()*momentum.y())/sqrt(momentum.perp2())
	      * momentum.z()/sqrt(momentum.perp2());

            if(theSimPVPosition) {
              // As in TrackBase::dxy(Point) and dz(Point)
              dxyPVSim = -(vertex.x()-theSimPVPosition->x())*sin(momentum.phi()) + (vertex.y()-theSimPVPosition->y())*cos(momentum.phi());
              dzPVSim = vertex.z()-theSimPVPosition->z() - ( (vertex.x()-theSimPVPosition->x()) + (vertex.y()-theSimPVPosition->y()) )/sqrt(momentum.perp2()) * momentum.z()/sqrt(momentum.perp2());
            }
	  }
	//If the TrackingParticle is comics, get the momentum and vertex at PCA
	else
	  {
	    momentumTP = std::get<TrackingParticle::Vector>(momVert);
	    vertexTP = std::get<TrackingParticle::Point>(momVert);
	    dxySim = (-vertexTP.x()*sin(momentumTP.phi())+vertexTP.y()*cos(momentumTP.phi()));
	    dzSim = vertexTP.z() - (vertexTP.x()*momentumTP.x()+vertexTP.y()*momentumTP.y())/sqrt(momentumTP.perp2())
	      * momentumTP.z()/sqrt(momentumTP.perp2());

            // Do dxy and dz vs. PV make any sense for cosmics? I guess not
	  }
	//---------- THE PART ABOVE HAS TO BE CLEANED UP. THE PARAMETER DEFINER WAS NOT MEANT TO BE USED IN THIS WAY ----------

        //This counter counts the number of simulated tracks passing the MTV selection (i.e. tpSelector(tp) ), but only for in-time TPs
        if(tp.eventId().bunchCrossing() == 0) {
          st++;
        }

	// in the coming lines, histos are filled using as input
	// - momentumTP
	// - vertexTP
	// - dxySim
	// - dzSim
        if(!doSimTrackPlots_)
          continue;

	// ##############################################
	// fill RecoAssociated SimTracks' histograms
	// ##############################################
	const reco::Track* matchedTrackPointer=0;
	if(simRecColl.find(tpr) != simRecColl.end()){
	  auto const & rt = simRecColl[tpr];
	  if (rt.size()!=0) {
	    ats++; //This counter counts the number of simTracks that have a recoTrack associated
	    // isRecoMatched = true; // UNUSED
	    matchedTrackPointer = rt.begin()->first.get();
	    //std::cout << "TrackingParticle #" << st
            //                           << " with pt=" << sqrt(momentumTP.perp2())
            //                           << " associated with quality:" << rt.begin()->second <<std::endl;
	  }
	}else{
	  //std::cout
	   // << "TrackingParticle #" << st
	   // << " with pt,eta,phi: "
	   // << sqrt(momentumTP.perp2()) << " , "
	   // << momentumTP.eta() << " , "
	   // << momentumTP.phi() << " , "
	   // << " NOT associated to any reco::Track" << std::endl;
	}




        int nSimHits = tp.numberOfTrackerHits();
        int nSimLayers = nLayers_tPCeff[tpr];
        int nSimPixelLayers = nPixelLayers_tPCeff[tpr];
        int nSimStripMonoAndStereoLayers = nStripMonoAndStereoLayers_tPCeff[tpr];
        histoProducerAlgo_->fill_recoAssociated_simTrack_histos(w,tp,momentumTP,vertexTP,dxySim,dzSim,dxyPVSim,dzPVSim,nSimHits,nSimLayers,nSimPixelLayers,nSimStripMonoAndStereoLayers,matchedTrackPointer,puinfo.getPU_NumInteractions(), dR, thePVposition);
          sts++;
          if(matchedTrackPointer)
            asts++;
          if(doSummaryPlots_) {
            if(dRtpSelectorNoPtCut(tp)) {
              h_simul_coll_allPt[ww]->Fill(www);
              if (matchedTrackPointer) {
                h_assoc_coll_allPt[ww]->Fill(www);
              }

              if(dRtpSelector(tp)) {
                h_simul_coll[ww]->Fill(www);
                if (matchedTrackPointer) {
                  h_assoc_coll[ww]->Fill(www);
                }
              }
            }
          }




      } // End  for (TrackingParticleCollection::size_type i=0; i<tPCeff.size(); i++)

      // ##############################################
      // fill recoTracks histograms (LOOP OVER TRACKS)
      // ##############################################
      if(!doRecoTrackPlots_)
        continue;
      LogTrace("TrackValidator") << "\n# of reco::Tracks with "
                                 << label[www].process()<<":"
                                 << label[www].label()<<":"
                                 << label[www].instance()
                                 << ": " << trackCollection->size() << "\n";

      int sat(0); //This counter counts the number of recoTracks that are associated to SimTracks from Signal only
      int at(0); //This counter counts the number of recoTracks that are associated to SimTracks
      int rT(0); //This counter counts the number of recoTracks in general

      //calculate dR for tracks
      const edm::View<Track> *trackCollectionDr = trackCollection.product();
      if(calculateDrSingleCollection_) {
        trackCollectionDr = trackCollectionForDrCalculation.product();
      }
      float dR_trk[trackCollection->size()];
      int i=0;
      float etaL[trackCollectionDr->size()];
      float phiL[trackCollectionDr->size()];
      for (auto const & track2 : *trackCollectionDr) {
         auto  && p = track2.momentum();
         etaL[i] = etaFromXYZ(p.x(),p.y(),p.z());
         phiL[i] = atan2f(p.y(),p.x());
         ++i;
      }
      for(View<Track>::size_type i=0; i<trackCollection->size(); ++i){
	auto const &  track = (*trackCollection)[i];
	auto dR = std::numeric_limits<float>::max();
        auto  && p = track.momentum();
        float eta = etaFromXYZ(p.x(),p.y(),p.z());
        float phi = atan2f(p.y(),p.x());
	for(View<Track>::size_type j=0; j<trackCollectionDr->size(); ++j){
	  auto dR_tmp = reco::deltaR2(eta, phi, etaL[j], phiL[j]);
	  if ( (dR_tmp<dR) & (dR_tmp>std::numeric_limits<float>::min())) dR=dR_tmp;
	}
	dR_trk[i] = std::sqrt(dR);
      }

      for(View<Track>::size_type i=0; i<trackCollection->size(); ++i){

	RefToBase<Track> track(trackCollection, i);
	rT++;
 
	bool isSigSimMatched(false);
	bool isSimMatched(false);
        bool isChargeMatched(true);
        int numAssocRecoTracks = 0;
	int nSimHits = 0;
	double sharedFraction = 0.;

        auto tpFound = recSimColl.find(track);
        isSimMatched = tpFound != recSimColl.end();
        if (isSimMatched) {
            const auto& tp = tpFound->val;
	    nSimHits = tp[0].first->numberOfTrackerHits();
            sharedFraction = tp[0].second;
            if (tp[0].first->charge() != track->charge()) isChargeMatched = false;
            if(simRecColl.find(tp[0].first) != simRecColl.end()) numAssocRecoTracks = simRecColl[tp[0].first].size();
	    at++;
	    for (unsigned int tp_ite=0;tp_ite<tp.size();++tp_ite){
              TrackingParticle trackpart = *(tp[tp_ite].first);
	      if ((trackpart.eventId().event() == 0) && (trackpart.eventId().bunchCrossing() == 0)){
	      	isSigSimMatched = true;
		sat++;
		break;
	      }
            }
	    LogTrace("TrackValidator") << "reco::Track #" << rT << " with pt=" << track->pt()
                                       << " associated with quality:" << tp.begin()->second <<"\n";
	} else {
	  LogTrace("TrackValidator") << "reco::Track #" << rT << " with pt=" << track->pt()
                                     << " NOT associated to any TrackingParticle" << "\n";
	}

	double dR=dR_trk[i];
	histoProducerAlgo_->fill_generic_recoTrack_histos(w,*track,bs.position(), thePVposition, isSimMatched,isSigSimMatched, isChargeMatched, numAssocRecoTracks, puinfo.getPU_NumInteractions(), nSimHits, sharedFraction, dR);
        if(doSummaryPlots_) {
          h_reco_coll[ww]->Fill(www);
          if(isSimMatched) {
            h_assoc2_coll[ww]->Fill(www);
            if(numAssocRecoTracks>1) {
              h_looper_coll[ww]->Fill(www);
            }
            if(!isSigSimMatched) {
              h_pileup_coll[ww]->Fill(www);
            }
          }
        }

	// dE/dx
	if (dodEdxPlots_) histoProducerAlgo_->fill_dedx_recoTrack_histos(w,track, v_dEdx);


	//Fill other histos
	if (!isSimMatched) continue;

	histoProducerAlgo_->fill_simAssociated_recoTrack_histos(w,*track);

	TrackingParticleRef tpr = tpFound->val.begin()->first;

	// TO BE FIXED LATER
//	if (associators[ww]=="trackAssociatorByChi2"){
//	  //association chi2
//	  double assocChi2 = -tp.begin()->second;//in association map is stored -chi2
//	  h_assochi2[www]->Fill(assocChi2);
//	  h_assochi2_prob[www]->Fill(TMath::Prob((assocChi2)*5,5));
//	}
//	else if (associators[ww]=="quickTrackAssociatorByHits"){
//	  double fraction = tp.begin()->second;
//	  h_assocFraction[www]->Fill(fraction);
//	  h_assocSharedHit[www]->Fill(fraction*track->numberOfValidHits());
//	}
	


	//Get tracking particle parameters at point of closest approach to the beamline
	TrackingParticle::Vector momentumTP = parametersDefinerTP->momentum(event,setup,tpr);
	TrackingParticle::Point vertexTP = parametersDefinerTP->vertex(event,setup,tpr);
	int chargeTP = tpr->charge();

	histoProducerAlgo_->fill_ResoAndPull_recoTrack_histos(w,momentumTP,vertexTP,chargeTP,
							     *track,bs.position());


	//TO BE FIXED
	//std::vector<PSimHit> simhits=tpr.get()->trackPSimHit(DetId::Tracker);
	//nrecHit_vs_nsimHit_rec2sim[w]->Fill(track->numberOfValidHits(), (int)(simhits.end()-simhits.begin() ));

      } // End of for(View<Track>::size_type i=0; i<trackCollection->size(); ++i)

      histoProducerAlgo_->fill_trackBased_histos(w,at,rT,st);

      LogTrace("TrackValidator") << "Total Simulated: " << st << "\n"
                                 << "Total Associated (simToReco): " << ats << "\n"
                                 << "Total Reconstructed: " << rT << "\n"
                                 << "Total Associated (recoToSim): " << at << "\n"
                                 << "Total Fakes: " << rT-at << "\n";
    } // End of  for (unsigned int www=0;www<label.size();www++)
  } //END of for (unsigned int ww=0;ww<associators.size();ww++)
*/
}

double MultiTrackValidator::lxy(TVector3 v1, TVector3 v2){
        double x1 = v1.X();
        double x2 = v2.X();
        double y1 = v1.Y();
        double y2 = v2.Y();
        return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}
