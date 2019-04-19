#ifndef MultiTrackValidator_h
#define MultiTrackValidator_h

/** \class MultiTrackValidator
 *  Class that prodecs histrograms to validate Track Reconstruction performances
 *
 *  \author cerati
 */
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "Validation/RecoTrack/interface/MultiTrackValidatorBase.h"
#include "Validation/RecoTrack/interface/MTVHistoProducerAlgoForTracker.h"
#include "SimDataFormats/Associations/interface/VertexToTrackingVertexAssociator.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TEfficiency.h"
#include "TFile.h"
#include <TVectorD.h>
#include <TVector3.h>
class MultiTrackValidator : public DQMEDAnalyzer, protected MultiTrackValidatorBase {
 public:
  /// Constructor
  MultiTrackValidator(const edm::ParameterSet& pset);
  
  /// Destructor
  virtual ~MultiTrackValidator();


  /// Method called once per event
  void analyze(const edm::Event&, const edm::EventSetup& ) override;
  /// Method called to book the DQM histograms
  void bookHistograms(DQMStore::IBooker&, edm::Run const&, edm::EventSetup const&) override;
  void FillHistoTrackingParticle(const TrackingParticle& tp, TVector3 beamspot);
  void FillHistoTrackingParticleMatchFound(const TrackingParticle& tp, TVector3 beamspot);
  void FillHistoTrackingParticleMatchNotFound(const TrackingParticle& tp, TVector3 beamspot);
  void FillHistoTrack(const reco::Track& tk, TVector3 beamspot);
  void FillHistoMatchingTrackingParticleToTrack(const reco::Track * tk,const TrackingParticle& tp, TVector3 beamspot);
  void FillTEffTrackingParticle(const TrackingParticle& tp, bool matchFound, TVector3 beamspot);
  void FillTEffTrackingParticlegranddaugterAntiS(const TrackingParticle& tp, bool matchFound, TVector3 beamspot, std::string particleType);
  void FillTEffTrackingParticlegranddaughterIsKsDaughter(const TrackingParticle& tp, bool matchingTrackFound, TVector3 beamspot,TVector3 creationVertexV0,double pt_antiS,double eta_antiS, double pt_V0, double eta_V0, double p_antiS);
  void FillTEffTrackingParticlegranddaughterIsPosPionLambdaDaughter(const TrackingParticle& tp, bool matchingTrackFound, TVector3 beamspot,TVector3 creationVertexV0,double pt_antiS,double eta_antiS, double pt_V0, double eta_V0, double p_antiS);
  void FillTEffTrackingParticlegranddaughterIsNegProtonLambdaDaughter(const TrackingParticle& tp, bool matchingTrackFound, TVector3 beamspot,TVector3 creationVertexV0,double pt_antiS,double eta_antiS, double pt_V0, double eta_V0, double p_antiS);
  void FillHistoTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed(const TrackingParticle& tp,  TVector3 beamspot);
  void FillHitsoTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed(const TrackingParticle& tp,  TVector3 beamspot);
  void FillHistoTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed(const TrackingParticle& tp, TVector3 beamspot);
  void FillHistoTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed(const TrackingParticle& tp,  TVector3 beamspot);
  void FillHistoTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed(const TrackingParticle& tp,  TVector3 beamspot);
  void FillHitsoTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed(const TrackingParticle& tp,  TVector3 beamspot);
  void FillHistoTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed(const TrackingParticle& tp,  TVector3 beamspot);
  void FillHistoTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed(const TrackingParticle& tp,  TVector3 beamspot);
  void FillHistoTrackingParticleToTrackDeltaRMatch(const TrackingParticle& tp, const reco::Track& tk, TVector3 beamspot, double deltaRmin);
  void FillHistoGenParticlesPlusGEANTToTrackDeltaRMatch(const reco::GenParticle& genParticlePlusGEANT, const reco::Track& track, TVector3 beamspot, double deltaRmin); 


  double lxy(TVector3 v1, TVector3 v2);

 protected:
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

  //edm::InputTag m_tag_beamspot;
  edm::EDGetTokenT<std::vector<reco::GenParticle>> token_genparticlePlusGEANT;
  edm::EDGetTokenT<reco::BeamSpot> token_beamspot;

  TFile *OutputFile = new TFile("test_separate.root","RECREATE");
//  edm::Service<TFileService> OutputFile; 
  TH1I * h_EvaluationCutsForEfiicientMC = new TH1I("h_EvaluationCutsForEfiicientMC",";0 = #events/ 1 = #antiS encountered/ 2 = #antiS with correct granddaughters/ 3 = #antiS with corr granddaughters surviving the cuts for eff MC prod / 4 = #antiS with corr grandd surviving the backgroundcut",6,0,6);

  //for the tracking particles
  //TFileDirectory dir_TrackingParticle= OutputFile->mkdir("dir_TrackingParticle");
  TDirectory *dir_TrackingParticle = OutputFile->mkdir("dir_TrackingParticle");
  TH1F * h_TrackingParticle_charge = new TH1F("h_TrackingParticle_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticle_pt = new TH1F("h_TrackingParticle_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticle_eta = new TH1F("h_TrackingParticle_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticle_phi = new TH1F("h_TrackingParticle_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticle_pdgId = new TH1F("h_TrackingParticle_pdgId","pdgId",20000,-10000,10000); 
  TH1F * h_TrackingParticle_vx = new TH1F("h_TrackingParticle_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticle_vy = new TH1F("h_TrackingParticle_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticle_vz = new TH1F("h_TrackingParticle_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticle_lxy = new TH1F("h_TrackingParticle_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticle_status = new TH1F("h_TrackingParticle_status","status ",200,-100,100); 
  TH1F * h_TrackingParticle_numberOfHits = new TH1F("h_TrackingParticle_numberOfHits","numberOfHits ",50,0,50); 

  //for the tracking particles with matching track
  TDirectory* dir_TrackingParticleMatchFound= OutputFile->mkdir("dir_TrackingParticleMatchFound");
  TH1F * h_TrackingParticleMatchFound_charge = new TH1F("h_TrackingParticleMatchFound_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticleMatchFound_pt = new TH1F("h_TrackingParticleMatchFound_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticleMatchFound_eta = new TH1F("h_TrackingParticleMatchFound_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticleMatchFound_phi = new TH1F("h_TrackingParticleMatchFound_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticleMatchFound_pdgId = new TH1F("h_TrackingParticleMatchFound_pdgId","pdgId",20000,-10000,10000); 
  TH1F * h_TrackingParticleMatchFound_vx = new TH1F("h_TrackingParticleMatchFound_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticleMatchFound_vy = new TH1F("h_TrackingParticleMatchFound_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticleMatchFound_vz = new TH1F("h_TrackingParticleMatchFound_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticleMatchFound_lxy = new TH1F("h_TrackingParticleMatchFound_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticleMatchFound_status = new TH1F("h_TrackingParticleMatchFound_status","status ",200,-100,100); 
  TH1F * h_TrackingParticleMatchFound_numberOfHits = new TH1F("h_TrackingParticleMatchFound_numberOfHits","numberOfHits ",50,0,50); 

  //for the tracking particles with no matching track
  TDirectory* dir_TrackingParticleMatchNotFound= OutputFile->mkdir("dir_TrackingParticleMatchNotFound");
  TH1F * h_TrackingParticleMatchNotFound_charge = new TH1F("h_TrackingParticleMatchNotFound_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticleMatchNotFound_pt = new TH1F("h_TrackingParticleMatchNotFound_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticleMatchNotFound_eta = new TH1F("h_TrackingParticleMatchNotFound_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticleMatchNotFound_phi = new TH1F("h_TrackingParticleMatchNotFound_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticleMatchNotFound_pdgId = new TH1F("h_TrackingParticleMatchNotFound_pdgId","pdgId",20000,-10000,10000); 
  TH1F * h_TrackingParticleMatchNotFound_vx = new TH1F("h_TrackingParticleMatchNotFound_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticleMatchNotFound_vy = new TH1F("h_TrackingParticleMatchNotFound_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticleMatchNotFound_vz = new TH1F("h_TrackingParticleMatchNotFound_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticleMatchNotFound_lxy = new TH1F("h_TrackingParticleMatchNotFound_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticleMatchNotFound_status = new TH1F("h_TrackingParticleMatchNotFound_status","status ",200,-100,100); 
  TH1F * h_TrackingParticleMatchNotFound_numberOfHits = new TH1F("h_TrackingParticleMatchNotFound_numberOfHits","numberOfHits ",50,0,50); 

  //For the tracks
  TDirectory* dir_Track= OutputFile->mkdir("dir_Track");
  TH1F * h_Track_charge= new TH1F("h_Track_charge","charge",100,-50,50); 
  TH1F * h_Track_pt = new TH1F("h_Track_pt","pt (GeV)",10000,0,10); 
  TH1F * h_Track_eta = new TH1F("h_Track_eta","#eta",800,-4,4); 
  TH1F * h_Track_phi = new TH1F("h_Track_phi","#phi (rad)",800,-4,4); 
  TH1F * h_Track_vx = new TH1F("h_Track_vx","vx (cm)",4000,-200,200); 
  TH1F * h_Track_vy = new TH1F("h_Track_vy","vy (cm)",4000,-200,200); 
  TH1F * h_Track_vz = new TH1F("h_Track_vz","vz (cm)",4000,-200,200); 
  TH1F * h_Track_lxy = new TH1F("h_Track_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_Track_status = new TH1F("h_Track_status","status",200,-100,100); 
  TH1F * h_Track_numberOfValidHits = new TH1F("h_Track_numberOfHits","numberOfValidHits ",50,0,50); 

  //For the TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed
  TDirectory* dir_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed= OutputFile->mkdir("TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed");
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_charge= new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_pt = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_eta = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_phi = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vx = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vz = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_lxy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_status = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_status","status",200,-100,100); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_numberOfValidHits = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndReconstructed_numberOfHits","numberOfValidHits ",50,0,50); 

  //For the TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed
  TDirectory* dir_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed= OutputFile->mkdir("TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed");
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_charge= new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_pt = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_eta = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_phi = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vx = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vz = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_lxy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_status = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_status","status",200,-100,100); 
  TH1F * h_TrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_numberOfValidHits = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosKsDaughterAndNotReconstructed_numberOfHits","numberOfValidHits ",50,0,50); 

  //For the TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed
  TDirectory* dir_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed= OutputFile->mkdir("TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed");
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_charge= new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_pt = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_eta = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_phi = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vx = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vz = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_lxy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_status = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_status","status",200,-100,100); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_numberOfValidHits = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndReconstructed_numberOfHits","numberOfValidHits ",50,0,50); 

  //For the TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed
  TDirectory* dir_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed= OutputFile->mkdir("TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed");
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_charge= new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_pt = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_eta = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_phi = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vx = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vz = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_lxy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_status = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_status","status",200,-100,100); 
  TH1F * h_TrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_numberOfValidHits = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegKsDaughterAndNotReconstructed_numberOfHits","numberOfValidHits ",50,0,50); 

  //For the TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed
  TDirectory* dir_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed= OutputFile->mkdir("TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed");
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_charge= new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_pt = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_eta = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_phi = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vx = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vz = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_lxy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_status = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_status","status",200,-100,100); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_numberOfValidHits = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndReconstructed_numberOfHits","numberOfValidHits ",50,0,50); 

  //For the TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed
  TDirectory* dir_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed= OutputFile->mkdir("TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed");
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_charge= new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_pt = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_eta = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_phi = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vx = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vz = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_lxy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_status = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_status","status",200,-100,100); 
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_numberOfValidHits = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsPosPionLambdaDaughterAndNotReconstructed_numberOfHits","numberOfValidHits ",50,0,50); 


  //For the TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed
  TDirectory* dir_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed= OutputFile->mkdir("TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed");
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_charge= new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_pt = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_eta = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_phi = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vx = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vz = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_lxy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_status = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_status","status",200,-100,100); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_numberOfValidHits = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndReconstructed_numberOfHits","numberOfValidHits ",50,0,50); 

  //For the TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed
  TDirectory* dir_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed= OutputFile->mkdir("TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed");
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_charge= new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_charge","charge",100,-50,50); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_pt = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_pt","pt (GeV)",10000,0,10); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_eta = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_eta","#eta",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_phi = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_phi","#phi (rad)",800,-4,4); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vx = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vx","vx (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vy","vy (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vz = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_vz","vz (cm)",4000,-200,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_lxy = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_lxy","lxy (cm)",2000,0,200); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_status = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_status","status",200,-100,100); 
  TH1F * h_TrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_numberOfValidHits = new TH1F("h_TrackingParticleTrackingParticlegranddaughterIsNegProtonLambdaDaughterAndNotReconstructed_numberOfHits","numberOfValidHits ",50,0,50); 


  //check difference between matched tracks and trackingparticles  
  TDirectory* dir_TrackingParticleToTrack= OutputFile->mkdir("dir_TrackingParticleToTrack");
  TH1F * h_TrackingParticleToTrack_charge = new TH1F("h_TrackingParticleToTrack_charge","charge_trackingparticle - charge_track",100,-50,50);
  TH1F * h_TrackingParticleToTrack_pt = new TH1F("h_TrackingParticleToTrack_pt","pt_trackingparticle - pt_track",1000,-50,50);
  TH1F * h_TrackingParticleToTrack_eta = new TH1F("h_TrackingParticleToTrack_eta","eta_trackingparticle - eta_track",800,-4,4);
  TH1F * h_TrackingParticleToTrack_phi = new TH1F("h_TrackingParticleToTrack_phi","phi_trackingparticle - phi_track",800,-4,4);
  TH1F * h_TrackingParticleToTrack_vx = new TH1F("h_TrackingParticleToTrack_vx","vx_trackingparticle - vx_track",4000,-200,200);
  TH1F * h_TrackingParticleToTrack_vy = new TH1F("h_TrackingParticleToTrack_vy","vy_trackingparticle - vy_track",4000,-200,200);
  TH1F * h_TrackingParticleToTrack_vz = new TH1F("h_TrackingParticleToTrack_vz","vz_trackingparticle - vz_track",4000,-200,200);
  TH1F * h_TrackingParticleToTrack_lxy = new TH1F("h_TrackingParticleMatchFound_lxy","lxy_trackingparticle - lxy_track (cm)",2000,-100,100); 

  //efficiency plots
  //efficiency plots any tracking particles
  TDirectory* dir_eff_TrackingParticle= OutputFile->mkdir("dir_eff_TrackingParticle");
  TEfficiency * teff_TrackingParticle_charge      	= new TEfficiency("teff_TrackingParticle_charge",";charge",10,-5,5);     
  TEfficiency * teff_TrackingParticle_pt	  	= new TEfficiency("teff_TrackingParticle_pt",";pt(GeV)",100,0,20);
  TEfficiency * teff_TrackingParticle_pt_lxy_eta_cut = new TEfficiency("teff_TrackingParticle_pt_lxy_eta_cut",";pt(GeV)",100,0,20);
  TEfficiency * teff_TrackingParticle_pt_lxy_eta_status1_cut = new TEfficiency("teff_TrackingParticle_pt_lxy_eta_status1_cut",";pt(GeV)",100,0,20);
  TEfficiency * teff_TrackingParticle_eta		= new TEfficiency("teff_TrackingParticle_eta",";#eta",80,-4,4);
  TEfficiency * teff_TrackingParticle_eta_pt_lxy_cut    = new TEfficiency("teff_TrackingParticle_eta_pt_lxy_cut",";#eta",80,-4,4);
  TEfficiency * teff_TrackingParticle_eta_pt_lxy_status1_cut    = new TEfficiency("teff_TrackingParticle_eta_pt_lxy_status1_cut",";#eta",80,-4,4);
  TEfficiency * teff_TrackingParticle_phi		= new TEfficiency("teff_TrackingParticle_phi",";#phi (rad)",80,-4,4);
  TEfficiency * teff_TrackingParticle_pdgId		= new TEfficiency("teff_TrackingParticle_pdgId",";pdgId",20000,-10000,10000);
  TEfficiency * teff_TrackingParticle_vx		= new TEfficiency("teff_TrackingParticle_vx",";vx(cm)",200,-100,100);
  TEfficiency * teff_TrackingParticle_vy		= new TEfficiency("teff_TrackingParticle_vy",";vy(cm)",200,-100,100);
  TEfficiency * teff_TrackingParticle_vz		= new TEfficiency("teff_TrackingParticle_vz",";vz(cm)",400,-200,200);
  TEfficiency * teff_TrackingParticle_lxy		= new TEfficiency("teff_TrackingParticle_lxy",";lxy(cm)",100,0,100);
  TEfficiency * teff_TrackingParticle_lxy_pt_eta_cut    = new TEfficiency("teff_TrackingParticle_lxy_pt_eta_cut",";lxy(cm)",100,0,100);
  TEfficiency * teff_TrackingParticle_lxy_pt_eta_status1_cut    = new TEfficiency("teff_TrackingParticle_lxy_pt_eta_status1_cut",";lxy(cm)",100,0,100);
  TEfficiency * teff_TrackingParticle_status		= new TEfficiency("teff_TrackingParticle_status",";status",200,-100,100);
  TEfficiency * teff_TrackingParticle_numberOfHits 	= new TEfficiency("teff_TrackingParticle_numberOfHits",";numberOfHits",50,0,50);

  //efficiency plots for trackingparticles as granddaughter of the antiS
  TDirectory* dir_eff_TrackingParticlegranddaughterantiS= OutputFile->mkdir("dir_eff_TrackingParticlegranddaughterantiS");
  TEfficiency *teff_TrackingParticlegranddaughterantiS_granddaughtertype  = new TEfficiency("teff_TrackingParticlegranddaughterantiS_granddaughtertype","reco efficiency versus type of daughter; 0 = Ks,#pi+ / 1 = Ks,#pi- / 2 = #bar{#Lambda},#pi+ / 3 = #bar{#Lambda},#bar{p} / 4 = any other tracking particle; reco efficiency",5,0,5);
  TH1I *h_TrackingParticlegranddaughterantiS_nRecoGranddaughters_with_charged_granddaughters =  new TH1I("h_TrackingParticlegranddaughterantiS_nRecoGranddaughters_with_charged_granddaughters","# reco granddaughters per event, for single antiS events with AntiS going to charged granddaughters",5,0,5);
  TH1I *h_TrackingParticlegranddaughterantiS_nRecoGranddaughters_all_granddaughters=  new TH1I("h_TrackingParticlegranddaughterantiS_nRecoGranddaughters_all_granddaughters","# reco granddaughters per event, for single antiS events with AntiS going to any granddaughter",5,0,5);
  TH2I *h_TrackingParticlegranddaughterantiS_nReconstructableGranddaughters_nRecoGranddaughters=  new TH2I("h_TrackingParticlegranddaughterantiS_nReconstructableGranddaughters_nRecoGranddaughters",";#reconstructable (=charged pions or protons) antiS granddaughters per event;# reco granddaughters per event, for single antiS events",5,0,5,5,0,5);
  TH2I *h_reconstructedKsDaughterCombination_reconstructedLDaughterCombination=  new TH2I("h_reconstructedKsDaughterCombination_reconstructedLDaughterCombination","Combinatorics of reconstructed granddaughters for any event; combinatorics of Lambda daughter; combinatorics of Ks daughter",6,0,6,6,0,6);
  TH2I *h_reconstructedKsDaughterCombination_reconstructedLDaughterCombination_4reconstructableGranddaughtersOnly=  new TH2I("h_reconstructedKsDaughterCombination_reconstructedLDaughterCombination_4reconstructableGranddaughtersOnly","Combinatorics of reconstructed granddaughters for events with 4 reconstructable (=charged granddaughters) granddaughters; combinatorics of Lambda daughter; combinatorics of Ks daughter",6,0,6,6,0,6);

  //efficiency plots trackingparticles as daughters of the Ks
  TDirectory* dir_eff_TrackingParticlegranddaughterIsKsDaughter= OutputFile->mkdir("dir_eff_TrackingParticlegranddaughterIsKsDaughter");
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_pt	  	= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_pt",";pt(GeV)",100,0,20);
  TH1F * h_TrackingParticlegranddaughterIsKsDaughter_pt_nom	  	= new TH1F("h_TrackingParticlegranddaughterIsKsDaughter_pt_nom",";pt(GeV)",200,0,20);
  TH1F * h_TrackingParticlegranddaughterIsKsDaughter_pt_all	  	= new TH1F("h_TrackingParticlegranddaughterIsKsDaughter_pt_all",";pt(GeV)",200,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_eta		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_eta",";#eta",80,-4,4);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_phi		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_phi",";#phi (rad)",80,-4,4);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_lxy		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_lxy",";lxy(cm)",100,0,100);
  TH1F * h_TrackingParticlegranddaughterIsKsDaughter_lxy_nom		= new TH1F("h_TrackingParticlegranddaughterIsKsDaughter_lxy_nom",";lxy(cm)",100,0,100);
  TH1F * h_TrackingParticlegranddaughterIsKsDaughter_lxy_all		= new TH1F("h_TrackingParticlegranddaughterIsKsDaughter_lxy_all",";lxy(cm)",100,0,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_lxyz		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_lxyz",";lxyz(cm)",100,0,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_vz		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_vz",";vz(cm)",100,-200,200);
 
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_lxyV0CreationVertex              = new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_lxyV0CreationVertex",";lxy (beamspot, V0 creation vertex)(cm); granddaughter track RECO eff",100,0,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_vzV0CreationVertex              = new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_vzV0CreationVertex",";vz  (V0 creation vertex)(cm); granddaughter track RECO eff",100,-200,200);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_lxyzV0CreationVertex              = new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_lxyzV0CreationVertex",";lxyz  (beamspot, V0 creation vertex)(cm); granddaughter track RECO eff",50,-100,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_etaAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_etaAntiS",";#eta antiS; granddaughter track RECO eff",160,-8,8);
  TH1F * h_TrackingParticlegranddaughterIsKsDaughter_etaAntiS_nom		= new TH1F("teff_TrackingParticlegranddaughterIsKsDaughter_etaAntiS_nom",";#eta antiS; granddaughter track RECO eff",160,-8,8);
  TH1F * h_TrackingParticlegranddaughterIsKsDaughter_etaAntiS_all		= new TH1F("teff_TrackingParticlegranddaughterIsKsDaughter_etaAntiS_all",";#eta antiS; granddaughter track RECO eff",160,-8,8);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_etaKs		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_etaKs",";#eta Ks; granddaughter track RECO eff",80,-4,4);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_pAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_pAntiS",";p antiS (GeV); granddaughter track RECO eff",100,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS",";pT antiS (GeV); granddaughter track RECO eff",100,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_pTKs		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_pTKs",";pT Ks (GeV); granddaughter track RECO eff",100,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS",";pT antiS (GeV); #eta antiS; granddaughter track RECO eff",80,0,20,16,-8,8);
  TH2D* h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS_nom		= new TH2D("h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS_nom",";pT antiS (GeV); #eta antiS; granddaughter track RECO eff",80,0,20,16,-8,8);
  TH2D* h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS_all		= new TH2D("h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_etaAntiS_all",";pT antiS (GeV); #eta antiS; granddaughter track RECO eff",80,0,20,16,-8,8);
  TEfficiency * teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex		= new TEfficiency("teff_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex",";pT antiS (GeV); lxyz(beamspot, V0 creation vertex) (cm); granddaughter track RECO eff",80,0,20,10,0,100);
  TH2D* h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex_nom		= new TH2D("h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex_nom",";pT antiS (GeV); lxyz(beamspot, V0 creation vertex) (cm); granddaughter track RECO eff",80,0,20,10,0,100);
  TH2D* h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex_all		= new TH2D("h2_TrackingParticlegranddaughterIsKsDaughter_pTAntiS_lxyzV0CreationVertex_all",";pT antiS (GeV); lxyz(beamspot, V0 creation vertex) (cm); granddaughter track RECO eff",80,0,20,10,0,100);


  //efficiency plots trackingparticles as pion daughters of the Lambda
  TDirectory* dir_eff_TrackingParticlegranddaughterIsPosPionLambdaDaughter= OutputFile->mkdir("dir_eff_TrackingParticlegranddaughterIsPosPionLambdaDaughter");
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt	  	= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt",";pt(GeV)",100,0,20);
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt_all	  	= new TH1F("h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt_all",";pt(GeV)",200,0,20);
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt_nom	  	= new TH1F("h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pt_nom",";pt(GeV)",200,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_eta		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_eta",";#eta",80,-4,4);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_phi		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_phi",";#phi (rad)",80,-4,4);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy",";lxy(cm)",100,0,100);
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy_nom		= new TH1F("TH1F_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy_nom",";lxy(cm)",100,0,100);
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy_all		= new TH1F("TH1F_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxy_all",";lxy(cm)",100,0,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyz		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyz",";lxyz(cm)",100,0,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_vz		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_vz",";vz(cm)",100,-200,200);

  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex              = new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex",";lxy (beamspot, V0 creation vertex)(cm); granddaughter track RECO eff",100,0,100);
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex_nom              = new TH1F("h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex_nom",";lxy (beamspot, V0 creation vertex)(cm); granddaughter track RECO eff",100,0,100);
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex_all              = new TH1F("h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyV0CreationVertex_all",";lxy (beamspot, V0 creation vertex)(cm); granddaughter track RECO eff",100,0,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_vzV0CreationVertex              = new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_vzV0CreationVertex",";vz  (V0 creation vertex)(cm); granddaughter track RECO eff",100,-200,200);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyzV0CreationVertex              = new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_lxyzV0CreationVertex",";lxyz  (beamspot, V0 creation vertex)(cm); granddaughter track RECO eff",50,-100,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS",";#eta antiS; granddaughter track RECO eff",160,-8,8);
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS_nom		= new TH1F("h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS_nom",";#eta antiS; granddaughter track RECO eff",160,-8,8);
  TH1F * h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS_all		= new TH1F("h_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiS_all",";#eta antiS; granddaughter track RECO eff",160,-8,8);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiLambda		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_etaAntiLambda",";#eta #bar{#Lambda}; granddaughter track RECO eff",80,-4,4);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pAntiS",";p antiS (GeV); granddaughter track RECO eff",100,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS",";pT antiS (GeV); granddaughter track RECO eff",100,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiLambda		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiLambda",";pT #bar{#Lambda} (GeV); granddaughter track RECO eff",100,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS",";pT antiS (GeV); #eta antiS; granddaughter track RECO eff",80,0,20,16,-8,8);
  TH2D* h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS_nom		= new TH2D("h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS_nom",";pT antiS (GeV); #eta antiS; granddaughter track RECO eff",80,0,20,16,-8,8);
  TH2D* h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS_all		= new TH2D("h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_etaAntiS_all",";pT antiS (GeV); #eta antiS; granddaughter track RECO eff",80,0,20,16,-8,8);
  TEfficiency * teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex		= new TEfficiency("teff_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex",";pT antiS (GeV); lxyz(beamspot, V0 creation vertex) (cm); granddaughter track RECO eff",80,0,20,10,0,100);
  TH2D* h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex_nom		= new TH2D("h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex_nom",";pT antiS (GeV); lxyz(beamspot, V0 creation vertex) (cm); granddaughter track RECO eff",80,0,20,10,0,100);
  TH2D* h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex_all		= new TH2D("h2_TrackingParticlegranddaughterIsPosPionLambdaDaughter_pTAntiS_lxyzV0CreationVertex_all",";pT antiS (GeV); lxyz(beamspot, V0 creation vertex) (cm); granddaughter track RECO eff",80,0,20,10,0,100);

  //efficiency plots trackingparticles as anti proton daughters of the Lambda
  TDirectory* dir_eff_TrackingParticlegranddaughterIsProtonLambdaDaughter= OutputFile->mkdir("dir_eff_TrackingParticlegranddaughterIsProtonLambdaDaughter");
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt	  	= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt","pt(GeV)",100,0,20);
  TH1F* h_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt_nom	  	= new TH1F("h_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt_nom","pt(GeV)",200,0,20);
  TH1F* h_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt_all	  	= new TH1F("h_TrackingParticlegranddaughterIsProtonLambdaDaughter_pt_all","pt(GeV)",200,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_eta		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_eta","#eta",80,-4,4);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_phi		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_phi","#phi (rad)",80,-4,4);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy","lxy(cm)",100,0,100);
  TH1F * h_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy_nom		= new TH1F("h_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy_nom","lxy(cm)",100,0,100);
  TH1F * h_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy_all		= new TH1F("h_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxy_all","lxy(cm)",100,0,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyz		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyz","lxy(cm)",100,0,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_vz		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_vz","vz(cm)",100,-200,200);

  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyV0CreationVertex              = new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyV0CreationVertex",";lxy (beamspot, V0 creation vertex)(cm); granddaughter track RECO eff",100,0,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_vzV0CreationVertex              = new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_vzV0CreationVertex",";vz  (V0 creation vertex)(cm); granddaughter track RECO eff",100,-200,200);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyzV0CreationVertex              = new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_lxyzV0CreationVertex",";lxyz  (beamspot, V0 creation vertex)(cm); granddaughter track RECO eff",50,-100,100);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS",";#eta antiS; granddaughter track RECO eff",160,-8,8);
  TH1F * h_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS_nom		= new TH1F("h_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS_nom",";#eta antiS; granddaughter track RECO eff",160,-8,8);
  TH1F * h_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS_all		= new TH1F("h_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiS_all",";#eta antiS; granddaughter track RECO eff",160,-8,8);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiLambda		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_etaAntiLambda",";#eta #bar{#Lambda}; granddaughter track RECO eff",80,-4,4);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pAntiS",";p antiS (GeV); granddaughter track RECO eff",100,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS",";pT antiS (GeV); granddaughter track RECO eff",100,0,20);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiLambda		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiLambda",";pT #bar{#Lambda} (GeV); granddaughter track RECO eff",100,0,20);
  TH2D* h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS_nom		= new TH2D("h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS_nom",";pT antiS (GeV); #eta antiS; granddaughter track RECO eff",80,0,20,16,-8,8);
  TH2D* h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS_all		= new TH2D("h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS_all",";pT antiS (GeV); #eta antiS; granddaughter track RECO eff",80,0,20,16,-8,8);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_etaAntiS",";pT antiS (GeV); #eta antiS; granddaughter track RECO eff",80,0,20,16,-8,8);
  TEfficiency * teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex		= new TEfficiency("teff_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex",";pT antiS (GeV); lxyz(beamspot, V0 creation vertex) (cm); granddaughter track RECO eff",80,0,20,10,0,100);
  TH2D * h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex_nom		= new TH2D("h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex_nom",";pT antiS (GeV); lxyz(beamspot, V0 creation vertex) (cm); granddaughter track RECO eff",80,0,20,10,0,100);
  TH2D * h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex_all		= new TH2D("h2_TrackingParticlegranddaughterIsProtonLambdaDaughter_pTAntiS_lxyzV0CreationVertex_all",";pT antiS (GeV); lxyz(beamspot, V0 creation vertex) (cm); granddaughter track RECO eff",80,0,20,10,0,100);

  //efficiency plots trackingparticles as anti proton daughters of the Lambda

  //matching the trackng particles to the tracks based on deltaR
  TDirectory* dir_TrackingParticleToTrackDeltaRMatch = OutputFile->mkdir("dir_TrackingParticleToTrackDeltaRMatch");
  TH1D *h_TrackingParticleToTrackDeltaRMatch_deltaR = new TH1D("h_TrackingParticleToTrackDeltaRMatch_deltaR","deltaR between RECO track and trackingParticle; #Delta R; #entries",1000,0,10);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_charge      	= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_charge",";charge",10,-5,5);     
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_pt	  	= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_pt",";pt(GeV)",100,0,20);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_pt_lxy_eta_cut = new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_pt_lxy_eta_cut",";pt(GeV)",100,0,20);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_pt_lxy_eta_status1_cut = new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_pt_lxy_eta_status1_cut",";pt(GeV)",100,0,20);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_eta		= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_eta",";#eta",80,-4,4);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_eta_pt_lxy_cut    = new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_eta_pt_lxy_cut",";#eta",80,-4,4);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_eta_pt_lxy_status1_cut    = new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_eta_pt_lxy_status1_cut",";#eta",80,-4,4);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_phi		= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_phi",";#phi (rad)",80,-4,4);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_pdgId		= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_pdgId",";pdgId",20000,-10000,10000);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_vx		= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_vx",";vx(cm)",200,-100,100);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_vy		= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_vy",";vy(cm)",200,-100,100);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_vz		= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_vz",";vz(cm)",200,-100,100);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_lxy		= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_lxy",";lxy(cm)",100,0,100);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_lxy_pt_eta_cut    = new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_lxy_pt_eta_cut",";lxy(cm)",100,0,10);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_lxy_pt_eta_status1_cut    = new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_lxy_pt_eta_status1_cut",";lxy(cm)",100,0,10);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_status		= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_status",";status",200,-100,100);
  TEfficiency * teff_TrackingParticleToTrackDeltaRMatch_numberOfHits 	= new TEfficiency("teff_TrackingParticleToTrackDeltaRMatch_numberOfHits",";numberOfHits",50,0,50);


  //matching the genparticlesplusgeant to the tracks based on deltaR
  TDirectory* dir_GenParticlesPlusGEANTToTrackDeltaRMatch = OutputFile->mkdir("dir_GenParticlesPlusGEANTToTrackDeltaRMatch");
  TH1D *h_GenParticlesPlusGEANTToTrackDeltaRMatch_deltaR = new TH1D("h_GenParticlesPlusGEANTToTrackDeltaRMatch_deltaR","deltaR between RECO track and trackingParticle; #Delta R; #entries",1000,0,10);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_charge      	= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_charge",";charge",10,-5,5);     
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt	  	= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt",";pt(GeV)",100,0,20);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt_lxy_eta_cut = new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt_lxy_eta_cut",";pt(GeV)",100,0,20);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt_lxy_eta_status1_cut = new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pt_lxy_eta_status1_cut",";pt(GeV)",100,0,20);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta		= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta",";#eta",80,-4,4);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta_pt_lxy_cut    = new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta_pt_lxy_cut",";#eta",80,-4,4);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta_pt_lxy_status1_cut    = new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_eta_pt_lxy_status1_cut",";#eta",80,-4,4);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_phi		= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_phi",";#phi (rad)",80,-4,4);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pdgId		= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_pdgId",";pdgId",20000,-10000,10000);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vx		= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vx",";vx(cm)",200,-100,100);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vy		= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vy",";vy(cm)",200,-100,100);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vz		= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_vz",";vz(cm)",200,-100,100);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy		= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy",";lxy(cm)",100,0,100);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy_pt_eta_cut    = new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy_pt_eta_cut",";lxy(cm)",100,0,100);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy_pt_eta_status1_cut    = new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_lxy_pt_eta_status1_cut",";lxy(cm)",100,0,100);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_status		= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_status",";status",200,-100,100);
  TEfficiency * teff_GenParticlesPlusGEANTToTrackDeltaRMatch_numberOfHits 	= new TEfficiency("teff_GenParticlesPlusGEANTToTrackDeltaRMatch_numberOfHits",";numberOfHits",50,0,50);


};


#endif
