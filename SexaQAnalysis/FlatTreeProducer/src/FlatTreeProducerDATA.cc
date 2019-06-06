#include "../interface/FlatTreeProducerDATA.h"
#include <typeinfo>

FlatTreeProducerDATA::FlatTreeProducerDATA(edm::ParameterSet const& pset):
  m_lookAtAntiS(pset.getUntrackedParameter<bool>("lookAtAntiS")),
  m_bsTag(pset.getParameter<edm::InputTag>("beamspot")),
  m_offlinePVTag(pset.getParameter<edm::InputTag>("offlinePV")),
  m_sCandsTag(pset.getParameter<edm::InputTag>("sexaqCandidates")),
  m_V0KsTag(pset.getParameter<edm::InputTag>("V0KsCollection")),
  m_V0LTag(pset.getParameter<edm::InputTag>("V0LCollection")),

  m_bsToken    (consumes<reco::BeamSpot>(m_bsTag)),
  m_offlinePVToken    (consumes<vector<reco::Vertex>>(m_offlinePVTag)),
  m_sCandsToken(consumes<vector<reco::VertexCompositeCandidate> >(m_sCandsTag)),
  m_V0KsToken(consumes<vector<reco::VertexCompositeCandidate> >(m_V0KsTag)),
  m_V0LToken(consumes<vector<reco::VertexCompositeCandidate> >(m_V0LTag))
  


{

}


void FlatTreeProducerDATA::beginJob() {

    
        // Initialize when class is created
        edm::Service<TFileService> fs ;
        _tree = fs->make <TTree>("FlatTreeData","tree");

        // Declare tree's branches
        // Event
	_tree->Branch("_S_px",&_S_px);

}

void FlatTreeProducerDATA::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup) {

  Init();
 
  //beamspot
  edm::Handle<reco::BeamSpot> h_bs;

  //primary vertex
  edm::Handle<vector<reco::Vertex>> h_offlinePV;
  iEvent.getByToken(m_offlinePVToken, h_offlinePV);

  //lambdaKshortVertexFilter sexaquark candidates
  edm::Handle<vector<reco::VertexCompositeCandidate> > h_sCands;
  iEvent.getByToken(m_sCandsToken, h_sCands);

  //V0 Kshorts
  edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks;
  iEvent.getByToken(m_V0KsToken, h_V0Ks);

  //V0 Lambdas
  edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L;
  iEvent.getByToken(m_V0LToken, h_V0L);



  //beamspot
  TVector3 beamspot(0,0,0);
  TVector3 beamspotVariance(0,0,0);
  if(h_bs.isValid()){  
	beamspot.SetXYZ(h_bs->x0(),h_bs->y0(),h_bs->z0());
	beamspotVariance.SetXYZ(pow(h_bs->x0Error(),2),pow(h_bs->y0Error(),2),pow(h_bs->z0Error(),2));			
  }

  TVector3 FirstOfflinePV(0.,0.,0.);
  if(h_offlinePV.isValid()){ 
	FirstOfflinePV.SetX(h_offlinePV->at(0).x()); FirstOfflinePV.SetY(h_offlinePV->at(0).y()); FirstOfflinePV.SetZ(h_offlinePV->at(0).z());
	for(size_t i=0; i<h_offlinePV->size(); ++i) {
		reco::Vertex PrimVertex = h_offlinePV->at(i);		
	}
   }



  //loop over the RECO AntiS to plot the kinematics
  if(h_sCands.isValid()){
      for(unsigned int i = 0; i < h_sCands->size(); ++i){//loop all S candidates
	const reco::VertexCompositeCandidate * antiS = &h_sCands->at(i);	
	//the if below is important, if it is -1 you are looking at signal (antiS). If it is +1 you are looking at background (S).
	int chargeAntiProton = 1; //by default look at the background
	if(m_lookAtAntiS) chargeAntiProton = -1;//only when the m_lookAtAntiS flag is enabled look at the antiS, which has a charge of -1 (representing the charge of the proton)
	if(antiS->charge()==chargeAntiProton)FillRECOAntiS(antiS, beamspot, beamspotVariance, iEvent.id().event(), h_offlinePV);
      }
  }

  _tree->Fill();

 } //end of analyzer




void FlatTreeProducerDATA::FillRECOAntiS(const reco::VertexCompositeCandidate * RECOAntiS, TVector3 beamspot, TVector3 beamspotVariance, int eventId, edm::Handle<vector<reco::Vertex>> h_offlinePV){
	//calculate some kinematic variables of the for the S:
/*	TVector3 AntiSInteractionVertex(RECOAntiS->vx(),RECOAntiS->vy(),RECOAntiS->vz());
	TVector3 AntiSMomentumVertex(RECOAntiS->px(),RECOAntiS->py(),RECOAntiS->pz());
	double Lxy = AnalyzerAllSteps::lxy(beamspot,AntiSInteractionVertex);
	double error_Lxy = AnalyzerAllSteps::std_dev_lxy(RECOAntiS->vx(),RECOAntiS->vy(), RECOAntiS->vertexCovariance(0,0), RECOAntiS->vertexCovariance(1,1), beamspot.X(), beamspot.Y(), beamspotVariance.X(), beamspotVariance.Y());
	TVector3 AntiSMomentum(RECOAntiS->px(),RECOAntiS->py(),RECOAntiS->pz());
	double dxy = AnalyzerAllSteps::dxy_signed_line_point(AntiSInteractionVertex,AntiSMomentum,beamspot);
	
	double deltaPhiDaughters = reco::deltaPhi(RECOAntiS->daughter(0)->phi(),RECOAntiS->daughter(1)->phi());
	double deltaEtaDaughters = RECOAntiS->daughter(0)->eta()-RECOAntiS->daughter(1)->eta();
	double deltaRDaughters = sqrt(deltaPhiDaughters*deltaPhiDaughters+deltaEtaDaughters*deltaEtaDaughters);
	reco::LeafCandidate::LorentzVector n_(0,0,0,0.939565);
	double massAntiS = (RECOAntiS->p4()-n_).mass();

	//calculate some kinematic variables for the daughters of the antiS
	TVector3 AntiSDaug0Momentum(RECOAntiS->daughter(0)->px(),RECOAntiS->daughter(0)->py(),RECOAntiS->daughter(0)->pz());
	TVector3 AntiSDaug1Momentum(RECOAntiS->daughter(1)->px(),RECOAntiS->daughter(1)->py(),RECOAntiS->daughter(1)->pz());
	double dxy_daughter0 = AnalyzerAllSteps::dxy_signed_line_point(AntiSInteractionVertex, AntiSDaug0Momentum,beamspot);
	double dxy_daughter1 = AnalyzerAllSteps::dxy_signed_line_point(AntiSInteractionVertex, AntiSDaug1Momentum,beamspot);
	
	double dz_daughter0 = AnalyzerAllSteps::dz_line_point(AntiSInteractionVertex, AntiSDaug0Momentum,beamspot);
        double dz_daughter1 = AnalyzerAllSteps::dz_line_point(AntiSInteractionVertex, AntiSDaug1Momentum,beamspot);

	double dxyz_daughter0 = AnalyzerAllSteps::dxyz_signed_line_point(AntiSInteractionVertex, AntiSDaug0Momentum,beamspot);
	double dxyz_daughter1 = AnalyzerAllSteps::dxyz_signed_line_point(AntiSInteractionVertex, AntiSDaug1Momentum,beamspot);

	//collapse the RECO_dxy, RECO_dz, RECO_dxyz variables on one variable
	double relDiff_dxy_daughters = (abs(dxy_daughter0)-abs(dxy_daughter1))/(abs(dxy_daughter0)+abs(dxy_daughter1));
	double relDiff_dz_daughters = (abs(dz_daughter0)-abs(dz_daughter1))/(abs(dz_daughter0)+abs(dz_daughter1));
	double relDiff_dxyz_daughters = (abs(dxyz_daughter0)-abs(dxyz_daughter1))/(abs(dxyz_daughter0)+abs(dxyz_daughter1));

	//calculate dxy and dz for the antiS itself
	double dxy_AntiS = AnalyzerAllSteps::dxy_signed_line_point(AntiSInteractionVertex,AntiSMomentumVertex,beamspot);
	//double dz_AntiS = AnalyzerAllSteps::dz_line_point(AntiSInteractionVertex,AntiSMomentumVertex,beamspot);

	//calculate the dxy of the antiS with respect to the PV which gives the best match:
	double dzAntiSPVmin = 999.;
        TVector3  bestPVdzAntiS;
        for(unsigned int i = 0; i < h_offlinePV->size(); ++i){
                TVector3 PV(h_offlinePV->at(i).x(),h_offlinePV->at(i).y(),h_offlinePV->at(i).z());
		double dzAntiSPV = AnalyzerAllSteps::dz_line_point(AntiSInteractionVertex,AntiSMomentumVertex,PV);
                if(abs(dzAntiSPV) < abs(dzAntiSPVmin)) {dzAntiSPVmin = dzAntiSPV; bestPVdzAntiS = PV;}
        }

        double dxyAntiSPVmin = AnalyzerAllSteps::dxy_signed_line_point(AntiSInteractionVertex,AntiSMomentumVertex,bestPVdzAntiS);

	reco::Candidate::Vector AntiSDaug0MomentumVec(RECOAntiS->daughter(0)->px(),RECOAntiS->daughter(0)->py(),RECOAntiS->daughter(0)->pz());
	reco::Candidate::Vector AntiSDaug1MomentumVec(RECOAntiS->daughter(1)->px(),RECOAntiS->daughter(1)->py(),RECOAntiS->daughter(1)->pz());
	double openingsAngleDaughters = AnalyzerAllSteps::openings_angle(AntiSDaug0MomentumVec,AntiSDaug1MomentumVec);

	//calculate the sign of the dot product between the displacement vector and the dxy vector for both the Ks and the Lambda
	double signLxyDotdxy_daughter0 = AnalyzerAllSteps::sgn(AnalyzerAllSteps::vec_dxy_line_point(AntiSInteractionVertex, AntiSDaug0Momentum,beamspot)*AntiSInteractionVertex);
	double signLxyDotdxy_daughter1 = AnalyzerAllSteps::sgn(AnalyzerAllSteps::vec_dxy_line_point(AntiSInteractionVertex, AntiSDaug1Momentum,beamspot)*AntiSInteractionVertex);
	//calculate the sign of the dot product between the displacement vector and the pt of both the Ks and the Lambda
	double signPtDotdxy_daughter0 = AnalyzerAllSteps::sgn(AnalyzerAllSteps::vec_dxy_line_point(AntiSInteractionVertex, AntiSDaug0Momentum,beamspot)*AntiSDaug0Momentum);
	double signPtDotdxy_daughter1 = AnalyzerAllSteps::sgn(AnalyzerAllSteps::vec_dxy_line_point(AntiSInteractionVertex, AntiSDaug1Momentum,beamspot)*AntiSDaug1Momentum);
*/
 	_S_px.push_back(RECOAntiS->px());	
 	_S_py.push_back(RECOAntiS->py());	
 	_S_pz.push_back(RECOAntiS->pz());	

}

void FlatTreeProducerDATA::endJob()
{
}

void
FlatTreeProducerDATA::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
}

// ------------ method called when ending the processing of a run  ------------
void
FlatTreeProducerDATA::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
FlatTreeProducerDATA::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
FlatTreeProducerDATA::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FlatTreeProducerDATA::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

FlatTreeProducerDATA::~FlatTreeProducerDATA()
{
}


void
FlatTreeProducerDATA::Init()
{
	_S_px.clear();

}

DEFINE_FWK_MODULE(FlatTreeProducerDATA);
