// -*- C++ -*-
//
// Package:    V0Producer
// Class:      V0Fitter
//
/**\class V0Fitter V0Fitter.cc RecoVertex/V0Producer/src/V0Fitter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Brian Drell
//         Created:  Fri May 18 22:57:40 CEST 2007
//
//

#include "V0Fitter.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/TSCBLBuilderNoMaterial.h"
#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>
#include <typeinfo>
#include <memory>
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include <fstream>

// pdg mass constants
namespace {
   const double piMass = 0.13957018;
   const double piMassSquared = piMass*piMass;
   const double protonMass = 0.938272046;
   const double protonMassSquared = protonMass*protonMass;
   const double kShortMass = 0.497614;
   const double lambdaMass = 1.115683;
}

typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
typedef ROOT::Math::SVector<double, 3> SVector3;

V0Fitter::V0Fitter(const edm::ParameterSet& theParameters, edm::ConsumesCollector && iC)
{
   token_beamSpot = iC.consumes<reco::BeamSpot>(theParameters.getParameter<edm::InputTag>("beamSpot"));
   useVertex_ = theParameters.getParameter<bool>("useVertex");
   token_vertices = iC.consumes<std::vector<reco::Vertex>>(theParameters.getParameter<edm::InputTag>("vertices")); 
   m_genParticlesToken_SIM_GEANT = iC.consumes<std::vector<reco::GenParticle> >(theParameters.getParameter<edm::InputTag>("genCollection_SIM_GEANT"));
	
   token_tracks = iC.consumes<reco::TrackCollection>(theParameters.getParameter<edm::InputTag>("trackRecoAlgorithm"));
   vertexFitter_ = theParameters.getParameter<bool>("vertexFitter");
   useRefTracks_ = theParameters.getParameter<bool>("useRefTracks");
   
   // whether to reconstruct KShorts
   doKShorts_ = theParameters.getParameter<bool>("doKShorts");
   // whether to reconstruct Lambdas
   doLambdas_ = theParameters.getParameter<bool>("doLambdas");

   // cuts on initial track selection
   tkChi2Cut_ = theParameters.getParameter<double>("tkChi2Cut");
   tkNHitsCut_ = theParameters.getParameter<int>("tkNHitsCut");
   tkPtCut_ = theParameters.getParameter<double>("tkPtCut");
   tkIPSigXYCut_ = theParameters.getParameter<double>("tkIPSigXYCut");
   tkIPSigZCut_ = theParameters.getParameter<double>("tkIPSigZCut");
   
   // cuts on vertex
   vtxChi2Cut_ = theParameters.getParameter<double>("vtxChi2Cut");
   vtxDecaySigXYZCut_ = theParameters.getParameter<double>("vtxDecaySigXYZCut");
   vtxDecaySigXYCut_ = theParameters.getParameter<double>("vtxDecaySigXYCut");
   // miscellaneous cuts
   tkDCACut_ = theParameters.getParameter<double>("tkDCACut");
   mPiPiCut_ = theParameters.getParameter<double>("mPiPiCut");
   innerHitPosCut_ = theParameters.getParameter<double>("innerHitPosCut");
   cosThetaXYCut_ = theParameters.getParameter<double>("cosThetaXYCut");
   cosThetaXYZCut_ = theParameters.getParameter<double>("cosThetaXYZCut");
   // cuts on the V0 candidate mass
   kShortMassCut_ = theParameters.getParameter<double>("kShortMassCut");
   lambdaMassCut_ = theParameters.getParameter<double>("lambdaMassCut");
}

TVector3 V0Fitter::PCA_line_point(TVector3 Point_line, TVector3 Vector_along_line, TVector3 Point){
   //first move the vector along the line to the starting point of Point_line
   double normalise = sqrt(Vector_along_line.X()*Vector_along_line.X()+Vector_along_line.Y()*Vector_along_line.Y()+Vector_along_line.Z()*Vector_along_line.Z());
   TVector3 n(Vector_along_line.X()/normalise,Vector_along_line.Y()/normalise,Vector_along_line.Z()/normalise);
   TVector3 a = Point_line;
   TVector3 p = Point;

   //see https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line (Vector formulation)
   TVector3 vector_PCA = (a-p)-((a-p)*n)*n;
   return vector_PCA ;
}

double V0Fitter::dxy_signed_line_point(TVector3 Point_line_in, TVector3 Vector_along_line_in, TVector3 Point_in){

  //looking at XY, so put the Z component to 0 first
  TVector3 Point_line(Point_line_in.X(),Point_line_in.Y(),0.);
  TVector3 Vector_along_line(Vector_along_line_in.X(), Vector_along_line_in.Y(),0.);
  TVector3 Point(Point_in.X(), Point_in.Y(), 0.);

  TVector3 shortest_distance = PCA_line_point(Point_line,  Vector_along_line, Point);
  double dxy_signed_line_point = sqrt(shortest_distance.X()*shortest_distance.X()+shortest_distance.Y()*shortest_distance.Y());

  TVector3 displacement = Point_line - Point;
  if(displacement*Vector_along_line<0)dxy_signed_line_point = -dxy_signed_line_point;

  return dxy_signed_line_point;
}

// method containing the algorithm for vertex reconstruction
void V0Fitter::fitAll(const edm::Event& iEvent, const edm::EventSetup& iSetup,
   reco::VertexCompositeCandidateCollection & theKshorts, reco::VertexCompositeCandidateCollection & theLambdas)
{
   using std::vector;

   edm::Handle<reco::TrackCollection> theTrackHandle;
   iEvent.getByToken(token_tracks, theTrackHandle);
   if (!theTrackHandle->size()) return;
   const reco::TrackCollection* theTrackCollection = theTrackHandle.product();   

   edm::Handle<reco::BeamSpot> theBeamSpotHandle;
   iEvent.getByToken(token_beamSpot, theBeamSpotHandle);
   const reco::BeamSpot* theBeamSpot = theBeamSpotHandle.product();
   math::XYZPoint referencePos(theBeamSpot->position());
 
   //load the GEN particlesPlusGEANT to find examine the reconstruction of antiS daughters
   edm::Handle<vector<reco::GenParticle>> h_genParticles;
   iEvent.getByToken(m_genParticlesToken_SIM_GEANT, h_genParticles);

   //loop over the GEN particles and push back a vector which contains math::XYZVector with the momenta of the Ks and Lambda
   std::vector<math::XYZVector> momentaKsAntiS;
   std::vector<TVector3> decayVertexKsAntiS;
   std::vector<double>phiKsAntiS;
   std::vector<double>etaKsAntiS;
   std::vector<double>dxyKsAntiS;

   std::vector<math::XYZVector> momentaAntiLambdaAntiS;
   std::vector<TVector3> decayVertexAntiLambdaAntiS;
   std::vector<double>phiAntiLambdaAntiS;
   std::vector<double>etaAntiLambdaAntiS;
   std::vector<double>dxyAntiLambdaAntiS;

   if(h_genParticles.isValid()){
      for(unsigned int i = 0; i < h_genParticles->size(); ++i){//loop all genparticlesPlusGEANT
	const reco::Candidate * genParticle = &h_genParticles->at(i);
	//all antiS with 2 daughters
	if(genParticle->pdgId() == -1020000020 && genParticle->numberOfDaughters() == 2 ) {
		TVector3 beamspot(referencePos.X(), referencePos.Y(), referencePos.Z());

		momentaKsAntiS.push_back(genParticle->daughter(0)->momentum());
		momentaAntiLambdaAntiS.push_back(genParticle->daughter(1)->momentum());
		
		phiKsAntiS.push_back(genParticle->daughter(0)->phi());		
		etaKsAntiS.push_back(genParticle->daughter(0)->eta());		
	
		phiAntiLambdaAntiS.push_back(genParticle->daughter(1)->phi());		
		etaAntiLambdaAntiS.push_back(genParticle->daughter(1)->eta());		

		TVector3 CreationVertexKsAntiS(genParticle->daughter(0)->vx(),genParticle->daughter(0)->vy(),genParticle->daughter(0)->vz());
		TVector3 MomentumKsAntiS(genParticle->daughter(0)->px(),genParticle->daughter(0)->py(),genParticle->daughter(0)->pz());
		dxyKsAntiS.push_back(dxy_signed_line_point(CreationVertexKsAntiS, MomentumKsAntiS, beamspot));

		TVector3 CreationVertexAntiLambdaAntiS(genParticle->daughter(1)->vx(),genParticle->daughter(1)->vy(),genParticle->daughter(1)->vz());
		TVector3 MomentumAntiLambdaAntiS(genParticle->daughter(1)->px(),genParticle->daughter(1)->py(),genParticle->daughter(1)->pz());
		dxyAntiLambdaAntiS.push_back(dxy_signed_line_point(CreationVertexAntiLambdaAntiS, MomentumAntiLambdaAntiS, beamspot));

		TVector3 vertex;
		if(genParticle->daughter(0)->numberOfDaughters()>0){
			vertex.SetX(genParticle->daughter(0)->daughter(0)->vx());
			vertex.SetY(genParticle->daughter(0)->daughter(0)->vy());
			vertex.SetZ(genParticle->daughter(0)->daughter(0)->vz());
			decayVertexKsAntiS.push_back(vertex);
			
		}
		else{
			vertex.SetX(999);
			vertex.SetY(999);
			vertex.SetZ(999);
			decayVertexKsAntiS.push_back(vertex);
		
		}
		if(genParticle->daughter(1)->numberOfDaughters()>0){
			vertex.SetX(genParticle->daughter(1)->daughter(0)->vx());
			vertex.SetY(genParticle->daughter(1)->daughter(0)->vy());
			vertex.SetZ(genParticle->daughter(1)->daughter(0)->vz());
			decayVertexAntiLambdaAntiS.push_back(vertex);
		}
		else{
			vertex.SetX(999);
			vertex.SetY(999);
			vertex.SetZ(999);
			decayVertexAntiLambdaAntiS.push_back(vertex);
		
		}
        }
      }
    }

 
   reco::Vertex referenceVtx;
   if (useVertex_) {
      edm::Handle<std::vector<reco::Vertex>> vertices;
      iEvent.getByToken(token_vertices, vertices);
      referenceVtx = vertices->at(0);
      referencePos = referenceVtx.position();
   }


   edm::ESHandle<MagneticField> theMagneticFieldHandle;
   iSetup.get<IdealMagneticFieldRecord>().get(theMagneticFieldHandle);
   const MagneticField* theMagneticField = theMagneticFieldHandle.product();

   std::vector<reco::TrackRef> theTrackRefs;
   std::vector<reco::TransientTrack> theTransTracks;

   // fill vectors of TransientTracks and TrackRefs after applying preselection cuts
   for (reco::TrackCollection::const_iterator iTk = theTrackCollection->begin(); iTk != theTrackCollection->end(); ++iTk) {
      const reco::Track* tmpTrack = &(*iTk);
      double ipsigXY = std::abs(tmpTrack->dxy(*theBeamSpot)/tmpTrack->dxyError());
      if (useVertex_) ipsigXY = std::abs(tmpTrack->dxy(referencePos)/tmpTrack->dxyError());
      double ipsigZ = std::abs(tmpTrack->dz(referencePos)/tmpTrack->dzError());
      if (tmpTrack->normalizedChi2() < tkChi2Cut_ && tmpTrack->numberOfValidHits() >= tkNHitsCut_ &&
          tmpTrack->pt() > tkPtCut_ && ipsigXY > tkIPSigXYCut_ && ipsigZ > tkIPSigZCut_) {
         reco::TrackRef tmpRef(theTrackHandle, std::distance(theTrackCollection->begin(), iTk));
         theTrackRefs.push_back(std::move(tmpRef));
         reco::TransientTrack tmpTransient(*tmpRef, theMagneticField);
         theTransTracks.push_back(std::move(tmpTransient));
      }
   }
   // good tracks have now been selected for vertexing

   // loop over tracks and vertex good charged track pairs
   for (unsigned int trdx1 = 0; trdx1 < theTrackRefs.size(); ++trdx1) {
   for (unsigned int trdx2 = trdx1 + 1; trdx2 < theTrackRefs.size(); ++trdx2) {

      reco::TrackRef positiveTrackRef;
      reco::TrackRef negativeTrackRef;
      reco::TransientTrack* posTransTkPtr = nullptr;
      reco::TransientTrack* negTransTkPtr = nullptr;

      if (theTrackRefs[trdx1]->charge() < 0. && theTrackRefs[trdx2]->charge() > 0.) {
         negativeTrackRef = theTrackRefs[trdx1];
         positiveTrackRef = theTrackRefs[trdx2];
         negTransTkPtr = &theTransTracks[trdx1];
         posTransTkPtr = &theTransTracks[trdx2];
      } else if (theTrackRefs[trdx1]->charge() > 0. && theTrackRefs[trdx2]->charge() < 0.) {
         negativeTrackRef = theTrackRefs[trdx2];
         positiveTrackRef = theTrackRefs[trdx1];
         negTransTkPtr = &theTransTracks[trdx2];
         posTransTkPtr = &theTransTracks[trdx1];
      } else {
         continue;
      }

      //calculate the sum of the track momenta. This gives an initial guess on the direction of the V0. Check if this direction matches the direction of any of the daughters of the antiS, like this you know that this V0 comes from the antiS...
      math::XYZVector SumMomentaTrackPair = theTrackRefs[trdx1]->momentum() + theTrackRefs[trdx2]->momentum();

      double deltaRKsmin = 999;
      int indexMatchedKs = -1;
      for(unsigned int i = 0 ; i < momentaKsAntiS.size(); i++){
	double deltaPhi = reco::deltaPhi(SumMomentaTrackPair.phi(),momentaKsAntiS[i].phi());
	double deltaEta = SumMomentaTrackPair.eta()-momentaKsAntiS[i].eta();
	double deltaR = sqrt(deltaPhi*deltaPhi+deltaEta*deltaEta);
	if(deltaR < deltaRKsmin){ deltaRKsmin = deltaR; indexMatchedKs = i;}
      }

      bool tracksAreFromAntiSKs = false;
      if(deltaRKsmin<0.03) tracksAreFromAntiSKs = true;

      double deltaRAntiLambdamin = 999;
      int indexMatchedAntiLambda = -1;
      for(unsigned int i = 0 ; i < momentaAntiLambdaAntiS.size(); i++){
	double deltaPhi = reco::deltaPhi(SumMomentaTrackPair.phi(),momentaAntiLambdaAntiS[i].phi());
	double deltaEta = SumMomentaTrackPair.eta()-momentaAntiLambdaAntiS[i].eta();
	double deltaR = sqrt(deltaPhi*deltaPhi+deltaEta*deltaEta);
	if(deltaR < deltaRAntiLambdamin){  deltaRAntiLambdamin = deltaR; indexMatchedAntiLambda = i;}
      }


      bool tracksAreFromAntiSAntiLambda = false;
      if(deltaRAntiLambdamin<0.03) tracksAreFromAntiSAntiLambda = true;

      if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda) std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
      if(tracksAreFromAntiSKs)std::cout << "found 2 tracks which sum of momenta is pointing within deltaR < 0.03 of a Ks from the antiS, deltaR: " << deltaRKsmin << std::endl;
      if(tracksAreFromAntiSAntiLambda)std::cout << "found 2 tracks which sum of momenta is pointing within deltaR < 0.03 of a AntiL from the AntiS, deltaR: " << deltaRAntiLambdamin << std::endl;
      std::ofstream outfile;
      outfile.open("statisticsV0Cuts.txt", std::ios_base::app);

      if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda) outfile << "------- \n";
      // measure distance between tracks at their closest approach
      if (!posTransTkPtr->impactPointTSCP().isValid() || !negTransTkPtr->impactPointTSCP().isValid()){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){ outfile << "AAA \n";};continue;}
      FreeTrajectoryState const & posState = posTransTkPtr->impactPointTSCP().theState();
      FreeTrajectoryState const & negState = negTransTkPtr->impactPointTSCP().theState();
      ClosestApproachInRPhi cApp;
      cApp.calculate(posState, negState);
      
      if (!cApp.status()){ if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "BBB";};continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "1 passed ClosestApproachInRPhi valid" << std::endl;}
      float dca = std::abs(cApp.distance());

      //print the R-PHI closest approach, because often goes wrong here
      if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda) std::cout << "closest approach in R-Phi: "<< dca << std::endl;
      //print some info on the GEN KS and AntiLambda to see if there is a correlation between their parameters and large R-Phi PCA
      if(tracksAreFromAntiSKs)std::cout << "Ks decays at: " << decayVertexKsAntiS[indexMatchedKs].X() << "," << decayVertexKsAntiS[indexMatchedKs].Y() << "," << decayVertexKsAntiS[indexMatchedKs].Z() << std::endl;
      if(tracksAreFromAntiSKs)std::cout << "Ks phi, eta: " << phiKsAntiS[indexMatchedKs] << "," << etaKsAntiS[indexMatchedKs] << std::endl ;
      if(tracksAreFromAntiSKs)std::cout << "Ks dxy: " << dxyKsAntiS[indexMatchedKs] <<  std::endl ;

      if(tracksAreFromAntiSAntiLambda)std::cout << "AntiLambda decays at: " << decayVertexAntiLambdaAntiS[indexMatchedAntiLambda].X() << "," << decayVertexAntiLambdaAntiS[indexMatchedAntiLambda].Y() << "," << decayVertexAntiLambdaAntiS[indexMatchedAntiLambda].Z() << std::endl;
      if(tracksAreFromAntiSAntiLambda)std::cout << "AntiLambda phi, eta: " << phiAntiLambdaAntiS[indexMatchedAntiLambda] << "," << etaAntiLambdaAntiS[indexMatchedAntiLambda] << std::endl ;
      if(tracksAreFromAntiSAntiLambda)std::cout << "AntiLambda dxy: " << dxyAntiLambdaAntiS[indexMatchedAntiLambda] <<  std::endl ;

      //print some parameters of the V0 tracks:
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda)   std::cout << "negTrack: innerposition xyz: " << negativeTrackRef->innerPosition().X() << "," << negativeTrackRef->innerPosition().Y() << "," << negativeTrackRef->innerPosition().Z()  << std::endl;
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda)   std::cout << "posTrack: innerposition xyz: " << positiveTrackRef->innerPosition().X() << "," << positiveTrackRef->innerPosition().Y() << "," << positiveTrackRef->innerPosition().Z()  << std::endl;
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda)   std::cout << "negTrack: innermomentum xyz: " << negativeTrackRef->innerMomentum().X() << "," << negativeTrackRef->innerMomentum().Y() << "," << negativeTrackRef->innerMomentum().Z()  << std::endl;
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda)   std::cout << "posTrack: innermomentum xyz: " << positiveTrackRef->innerMomentum().X() << "," << positiveTrackRef->innerMomentum().Y() << "," << positiveTrackRef->innerMomentum().Z()  << std::endl;


      if (dca > tkDCACut_){ if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "CCC, DCA = " << dca << "\n";};continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "2 passed closest approach cut" << std::endl;}

      // the POCA should at least be in the sensitive volume
      GlobalPoint cxPt = cApp.crossingPoint();
      if (sqrt(cxPt.x()*cxPt.x() + cxPt.y()*cxPt.y()) > 120. || std::abs(cxPt.z()) > 300.) {if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "DDD \n";};continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "3 passed POCA cut" << std::endl;}

      // the tracks should at least point in the same quadrant
      TrajectoryStateClosestToPoint const & posTSCP = posTransTkPtr->trajectoryStateClosestToPoint(cxPt);
      TrajectoryStateClosestToPoint const & negTSCP = negTransTkPtr->trajectoryStateClosestToPoint(cxPt);
      if (!posTSCP.isValid() || !negTSCP.isValid()){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){ outfile << "EEE \n";};continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "4 passed quadrant cut" << std::endl;}
      if (posTSCP.momentum().dot(negTSCP.momentum())  < 0) {if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "FFF \n";};continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "5 passed cut on dot product" << std::endl;}
     
      // calculate mPiPi
      double totalE = sqrt(posTSCP.momentum().mag2() + piMassSquared) + sqrt(negTSCP.momentum().mag2() + piMassSquared);
      double totalESq = totalE*totalE;
      double totalPSq = (posTSCP.momentum() + negTSCP.momentum()).mag2();
      double mass = sqrt(totalESq - totalPSq);
      //when one of the tracks is moving back in the direction of the beamspot this is not know by the track reconstruction because there is no timing for the hits. Therefore the momentum vector of this track will be taken to point outwards. Therefore you could try to take teh track with the lowest momentum (this is likely the one which is going back to the beamspot), revert it's momentum and calculate the mass of the pair again
//      double totalPSq_trial2 = (-posTSCP.momentum() + negTSCP.momentum()).mag2();
//      double mass_trial2 = sqrt(totalESq - totalPSq_trial2);
//
//      double totalPSq_trial3 = (posTSCP.momentum() - negTSCP.momentum()).mag2();
//      double mass_trial3 = sqrt(totalESq - totalPSq_trial3);      
//
//      double totalPSq_trial4 = (-posTSCP.momentum() - negTSCP.momentum()).mag2();
//      double mass_trial4 = sqrt(totalESq - totalPSq_trial4);      
//
//      if (mass > mPiPiCut_ and mass_trial2 > mPiPiCut_ and mass_trial3 > mPiPiCut_ and mass_trial4 > mPiPiCut_){ if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "GGG, inv mass of track pair: " << mass << " alternative masses: " << mass_trial2 << " " << mass_trial3 << " " << mass_trial4 <<  " with momenta " << posTSCP.momentum().mag() << " and " << negTSCP.momentum().mag() << " and dot product between momenta: " << posTSCP.momentum().dot(negTSCP.momentum()) << "\n";};continue;}
//      else if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "Good invariant mass of the tracks: " <<  mass << " alternative mass: " << mass_trial2 << " " << mass_trial3 << " " << mass_trial4 << " with momenta " << posTSCP.momentum().mag() << " and " << negTSCP.momentum().mag() << " and dot product between momenta: " << posTSCP.momentum().dot(negTSCP.momentum()) << "\n";}
//
      if (mass > mPiPiCut_){ if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "GGG, inv mass of track pair: " << mass << " with momenta " << posTSCP.momentum().mag() << " and " << negTSCP.momentum().mag() << " and dot product between momenta: " << posTSCP.momentum().dot(negTSCP.momentum()) << "\n";};continue;}

      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "6 passed inv mass PiPi cut" << std::endl;}

      // Fill the vector of TransientTracks to send to KVF
      std::vector<reco::TransientTrack> transTracks;
      transTracks.reserve(2);
      transTracks.push_back(*posTransTkPtr);
      transTracks.push_back(*negTransTkPtr);

      // create the vertex fitter object and vertex the tracks
      TransientVertex theRecoVertex;
      if (vertexFitter_) {
         KalmanVertexFitter theKalmanFitter(useRefTracks_ == 0 ? false : true);
         theRecoVertex = theKalmanFitter.vertex(transTracks);
      } else if (!vertexFitter_) {
         useRefTracks_ = false;
         AdaptiveVertexFitter theAdaptiveFitter;
         theRecoVertex = theAdaptiveFitter.vertex(transTracks);
      }
      if (!theRecoVertex.isValid()){ if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "HHH \n";};continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "7 passed vertexing" << std::endl;}
     
      reco::Vertex theVtx = theRecoVertex;
      if (theVtx.normalizedChi2() > vtxChi2Cut_){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){ outfile << "III \n";};continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "8 passed vertex Chi2 cut" << std::endl;}
      GlobalPoint vtxPos(theVtx.x(), theVtx.y(), theVtx.z());

      // 2D decay significance
      SMatrixSym3D totalCov = theBeamSpot->rotatedCovariance3D() + theVtx.covariance();
      if (useVertex_) totalCov = referenceVtx.covariance() + theVtx.covariance();
      SVector3 distVecXY(vtxPos.x()-referencePos.x(), vtxPos.y()-referencePos.y(), 0.);
      double distMagXY = ROOT::Math::Mag(distVecXY);
      double sigmaDistMagXY = sqrt(ROOT::Math::Similarity(totalCov, distVecXY)) / distMagXY;
      if (distMagXY/sigmaDistMagXY < vtxDecaySigXYCut_){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "JJJ \n";}; continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "9 passed 2D sign cut" << std::endl;}

      // 3D decay significance
      SVector3 distVecXYZ(vtxPos.x()-referencePos.x(), vtxPos.y()-referencePos.y(), vtxPos.z()-referencePos.z());
      double distMagXYZ = ROOT::Math::Mag(distVecXYZ);
      double sigmaDistMagXYZ = sqrt(ROOT::Math::Similarity(totalCov, distVecXYZ)) / distMagXYZ;
      if (distMagXYZ/sigmaDistMagXYZ < vtxDecaySigXYZCut_){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "KKK \n";}; continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "10 passed 3D sign cut" << std::endl;}

      // make sure the vertex radius is within the inner track hit radius
      if (innerHitPosCut_ > 0. && positiveTrackRef->innerOk()) {
         reco::Vertex::Point posTkHitPos = positiveTrackRef->innerPosition();
         double posTkHitPosD2 =  (posTkHitPos.x()-referencePos.x())*(posTkHitPos.x()-referencePos.x()) +
            (posTkHitPos.y()-referencePos.y())*(posTkHitPos.y()-referencePos.y());
         if (sqrt(posTkHitPosD2) < (distMagXY - sigmaDistMagXY*innerHitPosCut_)){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "LLL \n";}; continue;}
      }
      if (innerHitPosCut_ > 0. && negativeTrackRef->innerOk()) {
         reco::Vertex::Point negTkHitPos = negativeTrackRef->innerPosition();
         double negTkHitPosD2 = (negTkHitPos.x()-referencePos.x())*(negTkHitPos.x()-referencePos.x()) +
            (negTkHitPos.y()-referencePos.y())*(negTkHitPos.y()-referencePos.y());
         if (sqrt(negTkHitPosD2) < (distMagXY - sigmaDistMagXY*innerHitPosCut_)){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "MMM \n";}; continue;}
      }
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "11 passed inner track hit radius cut" << std::endl;}
      
      std::auto_ptr<TrajectoryStateClosestToPoint> trajPlus;
      std::auto_ptr<TrajectoryStateClosestToPoint> trajMins;
      std::vector<reco::TransientTrack> theRefTracks;
      if (theRecoVertex.hasRefittedTracks()) {
         theRefTracks = theRecoVertex.refittedTracks();
      }

      if (useRefTracks_ && theRefTracks.size() > 1) {
         reco::TransientTrack* thePositiveRefTrack = 0;
         reco::TransientTrack* theNegativeRefTrack = 0;
         for (std::vector<reco::TransientTrack>::iterator iTrack = theRefTracks.begin(); iTrack != theRefTracks.end(); ++iTrack) {
            if (iTrack->track().charge() > 0.) {
               thePositiveRefTrack = &*iTrack;
            } else if (iTrack->track().charge() < 0.) {
               theNegativeRefTrack = &*iTrack;
            }
         }
         if (thePositiveRefTrack == 0 || theNegativeRefTrack == 0) continue;
         trajPlus.reset(new TrajectoryStateClosestToPoint(thePositiveRefTrack->trajectoryStateClosestToPoint(vtxPos)));
         trajMins.reset(new TrajectoryStateClosestToPoint(theNegativeRefTrack->trajectoryStateClosestToPoint(vtxPos)));
      } else {
         trajPlus.reset(new TrajectoryStateClosestToPoint(posTransTkPtr->trajectoryStateClosestToPoint(vtxPos)));
         trajMins.reset(new TrajectoryStateClosestToPoint(negTransTkPtr->trajectoryStateClosestToPoint(vtxPos)));
      }

      if (trajPlus.get() == 0 || trajMins.get() == 0 || !trajPlus->isValid() || !trajMins->isValid()){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "NNN \n";}; continue;}

      GlobalVector positiveP(trajPlus->momentum());
      GlobalVector negativeP(trajMins->momentum());
      GlobalVector totalP(positiveP + negativeP);

      // 2D pointing angle
      double dx = theVtx.x()-referencePos.x();
      double dy = theVtx.y()-referencePos.y();
      double px = totalP.x();
      double py = totalP.y();
      double angleXY = (dx*px+dy*py)/(sqrt(dx*dx+dy*dy)*sqrt(px*px+py*py));
      if (angleXY < cosThetaXYCut_){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "OOO \n";};continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "13 passed 2D pointing angle cut" << std::endl;}

      // 3D pointing angle
      double dz = theVtx.z()-referencePos.z();
      double pz = totalP.z();
      double angleXYZ = (dx*px+dy*py+dz*pz)/(sqrt(dx*dx+dy*dy+dz*dz)*sqrt(px*px+py*py+pz*pz));
      if (angleXYZ < cosThetaXYZCut_){if(tracksAreFromAntiSKs || tracksAreFromAntiSAntiLambda){outfile << "PPP \n";};continue;}
      if(tracksAreFromAntiSKs||tracksAreFromAntiSAntiLambda){std::cout << "14 passed 3D pointing angle cut" << std::endl;}

      // calculate total energy of V0 3 ways: assume it's a kShort, a Lambda, or a LambdaBar.
      double piPlusE = sqrt(positiveP.mag2() + piMassSquared);
      double piMinusE = sqrt(negativeP.mag2() + piMassSquared);
      double protonE = sqrt(positiveP.mag2() + protonMassSquared);
      double antiProtonE = sqrt(negativeP.mag2() + protonMassSquared);
      double kShortETot = piPlusE + piMinusE;
      double lambdaEtot = protonE + piMinusE;
      double lambdaBarEtot = antiProtonE + piPlusE;

      // Create momentum 4-vectors for the 3 candidate types
      const reco::Particle::LorentzVector kShortP4(totalP.x(), totalP.y(), totalP.z(), kShortETot);
      const reco::Particle::LorentzVector lambdaP4(totalP.x(), totalP.y(), totalP.z(), lambdaEtot);
      const reco::Particle::LorentzVector lambdaBarP4(totalP.x(), totalP.y(), totalP.z(), lambdaBarEtot);

      reco::Particle::Point vtx(theVtx.x(), theVtx.y(), theVtx.z());
      const reco::Vertex::CovarianceMatrix vtxCov(theVtx.covariance());
      double vtxChi2(theVtx.chi2());
      double vtxNdof(theVtx.ndof());

      // Create the VertexCompositeCandidate object that will be stored in the Event
      reco::VertexCompositeCandidate* theKshort = nullptr;
      reco::VertexCompositeCandidate* theLambda = nullptr;
      reco::VertexCompositeCandidate* theLambdaBar = nullptr;

      if (doKShorts_) {
         theKshort = new reco::VertexCompositeCandidate(0, kShortP4, vtx, vtxCov, vtxChi2, vtxNdof);
      }
      if (doLambdas_) {
         if (positiveP.mag2() > negativeP.mag2()) {
            theLambda = new reco::VertexCompositeCandidate(0, lambdaP4, vtx, vtxCov, vtxChi2, vtxNdof);
         } else {
            theLambdaBar = new reco::VertexCompositeCandidate(0, lambdaBarP4, vtx, vtxCov, vtxChi2, vtxNdof);
         }
      }

      // Create daughter candidates for the VertexCompositeCandidates
      reco::RecoChargedCandidate thePiPlusCand(
         1, reco::Particle::LorentzVector(positiveP.x(), positiveP.y(), positiveP.z(), piPlusE), vtx);
      thePiPlusCand.setTrack(positiveTrackRef);
      
      reco::RecoChargedCandidate thePiMinusCand(
         -1, reco::Particle::LorentzVector(negativeP.x(), negativeP.y(), negativeP.z(), piMinusE), vtx);
      thePiMinusCand.setTrack(negativeTrackRef);
      
      reco::RecoChargedCandidate theProtonCand(
         1, reco::Particle::LorentzVector(positiveP.x(), positiveP.y(), positiveP.z(), protonE), vtx);
      theProtonCand.setTrack(positiveTrackRef);

      reco::RecoChargedCandidate theAntiProtonCand(
         -1, reco::Particle::LorentzVector(negativeP.x(), negativeP.y(), negativeP.z(), antiProtonE), vtx);
      theAntiProtonCand.setTrack(negativeTrackRef);

      AddFourMomenta addp4;
      // Store the daughter Candidates in the VertexCompositeCandidates if they pass mass cuts
      if (doKShorts_) {
         theKshort->addDaughter(thePiPlusCand);
         theKshort->addDaughter(thePiMinusCand);
         theKshort->setPdgId(310);
         addp4.set(*theKshort);
         if (theKshort->mass() < kShortMass + kShortMassCut_ && theKshort->mass() > kShortMass - kShortMassCut_) {
            theKshorts.push_back(std::move(*theKshort));
         }
      }
      if (doLambdas_ && theLambda) {
         theLambda->addDaughter(theProtonCand);
         theLambda->addDaughter(thePiMinusCand);
         theLambda->setPdgId(3122);
         addp4.set( *theLambda );
         if (theLambda->mass() < lambdaMass + lambdaMassCut_ && theLambda->mass() > lambdaMass - lambdaMassCut_) {
            theLambdas.push_back(std::move(*theLambda));
         }
      } else if (doLambdas_ && theLambdaBar) {
         theLambdaBar->addDaughter(theAntiProtonCand);
         theLambdaBar->addDaughter(thePiPlusCand);
         theLambdaBar->setPdgId(-3122);
         addp4.set(*theLambdaBar);
         if (theLambdaBar->mass() < lambdaMass + lambdaMassCut_ && theLambdaBar->mass() > lambdaMass - lambdaMassCut_) {
            theLambdas.push_back(std::move(*theLambdaBar));
         }
      }

      delete theKshort;
      delete theLambda;
      delete theLambdaBar;
      theKshort = theLambda = theLambdaBar = nullptr;

    }
  }
}

