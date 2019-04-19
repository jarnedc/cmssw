#include "Validation/RecoTrack/interface/MultiTrackValidatorWithAnalyzer.h"
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

MultiTrackValidatorWithAnalyzer::MultiTrackValidatorWithAnalyzer(const edm::ParameterSet& pset):
  //MultiTrackValidatorBase(pset,consumesCollector()),
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
    //m_dEdx1Tag = consumes<edm::ValueMap<reco::DeDxData> >(pset.getParameter< edm::InputTag >("dEdx1Tag"));
    //m_dEdx2Tag = consumes<edm::ValueMap<reco::DeDxData> >(pset.getParameter< edm::InputTag >("dEdx2Tag"));
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

void MultiTrackValidatorWithAnalyzer::beginJob(){

  TFileDirectory dir_TrackingParticle = m_fs->mkdir("TrackingParticle");
  histos_th1f[b+"h_TrackingParticle_charge"] = dir_TrackingParticle.make<TH1F>(b+"h_TrackingParticle_charge",b+"h_TrackingParticle_charge; charge",100,-50,50);

}



  

/*void MultiTrackValidator::bookHistograms(DQMStore::IBooker& ibook, edm::Run const&, edm::EventSetup const& setup) {

  //TFileDirectory dir_TrackingParticle = m_fs->mkdir("TrackingParticle");
  //histos_th1f[b+"h_TrackingParticle_charge"] = dir_TrackingParticle.make<TH1F>(b+"h_TrackingParticle_charge",b+"h_TrackingParticle_charge; charge",100,-50,50);

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
*/



void MultiTrackValidatorWithAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){
  using namespace reco;
  std::cout << "begin the eventloop" << std::endl;
  LogDebug("TrackValidator") << "\n====================================================" << "\n"
                             << "Analyzing new event" << "\n"
                             << "====================================================\n" << "\n";





  edm::ESHandle<ParametersDefinerForTP> parametersDefinerTPHandle;
  setup.get<TrackAssociatorRecord>().get(parametersDefiner,parametersDefinerTPHandle);
  //Since we modify the object, we must clone it
  auto parametersDefinerTP = parametersDefinerTPHandle->clone();

  edm::Handle<TrackingParticleCollection>  TPCollectionHeff ;
  event.getByToken(label_tp_effic,TPCollectionHeff);
  TrackingParticleCollection const & tPCeff = *(TPCollectionHeff.product());

  edm::Handle<TrackingParticleCollection>  TPCollectionHfake ;
  event.getByToken(label_tp_fake,TPCollectionHfake);


  //ADDED BY JARNE
  for(size_t i=0; i<tPCeff.size(); ++i) {
     //size_t iTP = tPCeff[i];
     //TrackingParticleRef tpr(TPCollectionHeff, iTP);
     TrackingParticleRef tpr(TPCollectionHeff,i);
     const TrackingParticle& tp = tPCeff[i];

     //histos_th1f[b+"h_TrackingParticle_charge"]->Fill(tp.charge());   

     int w=0; //counter counting the number of sets of histograms
	  for (unsigned int ww=0;ww<associators.size();ww++){
	    for (unsigned int www=0;www<label.size();www++, w++){ // need to increment w here, since there are many continues in the loop body
	      //
	      //get collections from the event
	      //
	      edm::Handle<View<Track> >  trackCollection;
	      //if(!event.getByToken(labelToken[www], trackCollection)&&ignoremissingtkcollection_)continue;

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
		if(!simtorecoCollectionH.isValid()) cout << "simtorecoCollectionH collection is not valid" << endl;
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
		if(simRecColl.find(tpr) != simRecColl.end()){
		  auto const & rt = simRecColl[tpr];
		  if (rt.size()!=0) {
		    // isRecoMatched = true; // UNUSED
		    const reco::Track *matchedTrackPointer = rt.begin()->first.get();
		    cout << "Jarne:  TrackingParticle #" << i
		    << " with pt,eta,phi: "
		    << tp.pt() << " , "
		    << tp.eta() << " , "
		    << tp.phi() << " , "
		    << " ASSOCIATED with quality:" << rt.begin()->second << endl;
		  }
		}else{
		   cout  
		    << "Jarne:   TrackingParticle #" << i
		    << " with pt,eta,phi: "
		    << tp.pt() << " , "
		    << tp.eta() << " , "
		    << tp.phi() << " , "
		    << " NOT associated to any reco::Track" << endl;
		}
	   }//end loop over www
	  }//end loop over ww
 }//end loop over selected_tPCeff



  //END ADDED BY JARNE


}


void MultiTrackValidatorWithAnalyzer::endJob()
{
}

MultiTrackValidatorWithAnalyzer::~MultiTrackValidatorWithAnalyzer()
{
}

DEFINE_FWK_MODULE(MultiTrackValidatorWithAnalyzer);
