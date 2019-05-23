#include "../interface/AnalyzerRECO.h"
#include <typeinfo>

AnalyzerRECO::AnalyzerRECO(edm::ParameterSet const& pset):
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


void AnalyzerRECO::beginJob() {

     TFileDirectory dir_RECO = m_fs->mkdir("RECO"); 

     TFileDirectory dir_PV = m_fs->mkdir("PV");
     histos_th1f["h_PV_vz"] = dir_PV.make<TH1F>(b+"h_PV_vz", "; vz (cm); #entries ",600,-300,300);

     TFileDirectory dir_RECO_Ks = dir_RECO.mkdir("RECO_Ks");
     histos_th1f["h_RECO_Ks_pt"] = dir_RECO_Ks.make<TH1F>(b+"h_RECO_Ks_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_RECO_Ks_eta"] = dir_RECO_Ks.make<TH1F>(b+"h_RECO_Ks_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_RECO_Ks_phi"] = dir_RECO_Ks.make<TH1F>(b+"h_RECO_Ks_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_RECO_Ks_vx_vy"] = dir_RECO_Ks.make<TH2F>(b+"h2_RECO_Ks_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_RECO_Ks_vx_vz"] = dir_RECO_Ks.make<TH2F>(b+"h2_RECO_Ks_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_RECO_Ks_lxy"] = dir_RECO_Ks.make<TH1F>(b+"h_RECO_Ks_lxy", "; Ks lxy(beamspot, Ks vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_RECO_Ks_vz"] = dir_RECO_Ks.make<TH1F>(b+"h_RECO_Ks_vz", "; Ks vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_RECO_Ks_dxy"] = dir_RECO_Ks.make<TH1F>(b+"h_RECO_Ks_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_RECO_Ks_m"] = dir_RECO_Ks.make<TH1F>(b+"h_RECO_Ks_m", "; Ks m (GeV); #entries ",4000,0,4);

     TFileDirectory dir_RECO_Lambda = dir_RECO.mkdir("RECO_Lambda");
     histos_th1f["h_RECO_Lambda_pt"] = dir_RECO_Lambda.make<TH1F>(b+"h_RECO_Lambda_pt", "; #bar{#Lambda} or #Lambda pT (GeV); #entries ",200,0,20);
     histos_th1f["h_RECO_Lambda_eta"] = dir_RECO_Lambda.make<TH1F>(b+"h_RECO_Lambda_eta", "; #bar{#Lambda} or #Lambda #eta; #entries ",200,-10,10);
     histos_th1f["h_RECO_Lambda_phi"] = dir_RECO_Lambda.make<TH1F>(b+"h_RECO_Lambda_phi", "; #bar{#Lambda} or #Lambda #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_RECO_Lambda_vx_vy"] = dir_RECO_Lambda.make<TH2F>(b+"h2_RECO_Lambda_vx_vy", "; #bar{#Lambda} or #Lambda vx (cm); #bar{#Lambda} vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_RECO_Lambda_vx_vz"] = dir_RECO_Lambda.make<TH2F>(b+"h2_RECO_Lambda_vx_vz", "; #bar{#Lambda} or #Lambda vx (cm); #bar{#Lambda} vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_RECO_Lambda_lxy"] = dir_RECO_Lambda.make<TH1F>(b+"h_RECO_Lambda_lxy", "; #bar{#Lambda} or #Lambda lxy(beamspot, #bar{#Lambda} vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_RECO_Lambda_vz"] = dir_RECO_Lambda.make<TH1F>(b+"h_RECO_Lambda_vz", "; #bar{#Lambda} or #Lambda vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_RECO_Lambda_dxy"] = dir_RECO_Lambda.make<TH1F>(b+"h_RECO_Lambda_dxy", "; #bar{#Lambda} or #Lambda dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_RECO_Lambda_m"] = dir_RECO_Lambda.make<TH1F>(b+"h_RECO_Lambda_m", "; #bar{#Lambda} or #Lambda m (GeV); #entries ",4000,0,4);
     histos_th1f["h_RECO_Lambda_deltaRMin_GEN_Lambda"] = dir_RECO_Lambda.make<TH1F>(b+"h_RECO_Lambda_deltaRMin_GEN_Lambda", "; #DeltaR_{min} (#bar{#Lambda} or or #Lambda GEN, #bar{#Lambda} or #Lambda RECO); #entries ",1000,0,10);

     TFileDirectory dir_RECO_AntiS = dir_RECO.mkdir("RECO_AntiS");
     TFileDirectory dir_RECO_AntiS_Kinematics = dir_RECO_AntiS.mkdir("RECO_AntiS_Kinematics");
     histos_th1f["h_RECO_AntiS_pt"] = dir_RECO_AntiS_Kinematics.make<TH1F>(b+"h_RECO_AntiS_pt", "; #bar{S} pT (GeV); #entries ",200,0,20);
     histos_th1f["h_RECO_AntiS_eta"] = dir_RECO_AntiS_Kinematics.make<TH1F>(b+"h_RECO_AntiS_eta", "; #bar{S} #eta; #entries ",200,-10,10);
     histos_th1f["h_RECO_AntiS_phi"] = dir_RECO_AntiS_Kinematics.make<TH1F>(b+"h_RECO_AntiS_phi", "; #bar{S} #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_RECO_AntiS_vx_vy"] = dir_RECO_AntiS_Kinematics.make<TH2F>(b+"h2_RECO_AntiS_vx_vy", "; #bar{S} vx (cm); #bar{S} vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_RECO_AntiS_vx_vz"] = dir_RECO_AntiS_Kinematics.make<TH2F>(b+"h2_RECO_AntiS_vx_vz", "; #bar{S} vx (cm); #bar{S} vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_RECO_AntiS_vz"] = dir_RECO_AntiS_Kinematics.make<TH1F>(b+"h_RECO_AntiS_vz", "; #bar{S} vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_RECO_AntiS_dxy"] = dir_RECO_AntiS_Kinematics.make<TH1F>(b+"h_RECO_AntiS_dxy", "; #bar{S} dxy(beamspot) (cm); #entries ",400,-20,20);
    
     TFileDirectory dir_RECO_AntiS_cut_variables_distributions = dir_RECO_AntiS.mkdir("RECO_AntiS_cut_variables_distributions");
     histos_th1f["h_RECO_AntiS_lxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_lxy", "; #bar{S} lxy(beamspot, #bar{S} vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_RECO_AntiS_error_lxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_error_lxy", "; #bar{S} error lxy(beamspot, #bar{S} vertex) (cm); #entries ",200,0,10);
     histos_th1f["h_RECO_AntiS_deltaRDaughters"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_deltaRDaughters", "; #bar{S} #DeltaR (Ks,#bar{#Lambda}); #entries ",100,0,10);
     histos_th1f["h_RECO_AntiS_deltaPhiDaughters"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_deltaPhiDaughters", "; #bar{S} #Delta#Phi (Ks,#bar{#Lambda}); #entries ",100,-5,5);
     histos_th2f["h2_RECO_AntiS_deltaR_duaghters_lxy_cut_error_lxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH2F>(b+"h2_RECO_AntiS_deltaR_duaghters_lxy_cut_error_lxy", ";#bar{S} #Delta#R (Ks,#bar{#Lambda});lxy(beamspot,RECO #bar{S} interaction vertex); #entries ",100,0,10,500,0,50);
     histos_th3f["h3_RECO_AntiS_deltaR_duaghters_lxy_mass_cut_error_lxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH3F>(b+"h3_RECO_AntiS_deltaR_duaghters_lxy_mass_cut_error_lxy", ";#bar{S} #Delta#R (Ks,#bar{#Lambda});lxy(beamspot,RECO #bar{S} interaction vertex) (cm); mass #bar{S} (GeV)",60,0,6,40,0,20,40,0,10);
     histos_th2f["h2_RECO_AntiS_deltaPhi_duaghters_lxy_cut_error_lxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH2F>(b+"h2_RECO_AntiS_deltaPhi_duaghters_lxy_cut_error_lxy", ";#bar{S} #Delta#phi (Ks,#bar{#Lambda});lxy(beamspot,RECO #bar{S} interaction vertex); #entries ",80,-4,4,500,0,50);

     TFileDirectory dir_RECO_AntiS_deltaRDaughters = dir_RECO_AntiS.mkdir("RECO_AntiS_deltaRDaughters");
     histos_th1f["h_RECO_AntiS_deltaRDaughters_Lxy_cut"] = dir_RECO_AntiS_deltaRDaughters.make<TH1F>(b+"h_RECO_AntiS_deltaRDaughters_Lxy_cut", "; #bar{S} #DeltaR (Ks,#bar{#Lambda}); #entries ",100,0,10);
     histos_th1f["h_RECO_AntiS_deltaRDaughters_Lxy_and_error_Lxy_cut"] = dir_RECO_AntiS_deltaRDaughters.make<TH1F>(b+"h_RECO_AntiS_deltaRDaughters_Lxy_and_error_Lxy_cut", "; #bar{S} #DeltaR (Ks,#bar{#Lambda}); #entries ",100,0,10);
     histos_th2f["h2_RECO_AntiS_deltaRDaughters_lxy_error_lxy_cut"] = dir_RECO_AntiS_deltaRDaughters.make<TH2F>(b+"h2_RECO_AntiS_deltaRDaughters_lxy_error_lxy_cut", "; #bar{S} #DeltaR (Ks,#bar{#Lambda}); lxy(beamspot, #bar{S} interaction vertex) (cm)",100,0,10,2000,0,200);

     TFileDirectory dir_RECO_AntiS_deltaPhiDaughters = dir_RECO_AntiS.mkdir("RECO_AntiS_deltaPhiDaughters");
     histos_th1f["h_RECO_AntiS_deltaPhiDaughters_Lxy_cut"] = dir_RECO_AntiS_deltaPhiDaughters.make<TH1F>(b+"h_RECO_AntiS_deltaPhiDaughters_Lxy_cut", "; #bar{S} #Delta#Phi (Ks,#bar{#Lambda}); #entries ",100,-5,5);
     histos_th1f["h_RECO_AntiS_deltaPhiDaughters_Lxy_and_error_Lxy_cut"] = dir_RECO_AntiS_deltaPhiDaughters.make<TH1F>(b+"h_RECO_AntiS_deltaPhiDaughters_Lxy_and_error_Lxy_cut", "; #bar{S} #Delta#Phi (Ks,#bar{#Lambda}); #entries ",100,-5,5);
     histos_th2f["h2_RECO_AntiS_deltaPhiDaughters_lxy_error_lxy_cut"] = dir_RECO_AntiS_deltaPhiDaughters.make<TH2F>(b+"h2_RECO_AntiS_deltaPhiDaughters_lxy_error_lxy_cut", "; #bar{S} #Delta#Phi (Ks,#bar{#Lambda}); lxy(beamspot, #bar{S} interaction vertex) (cm)",100,-5,5,2000,0,200);

     TFileDirectory dir_RECO_AntiS_mass = dir_RECO_AntiS.mkdir("RECO_AntiS_mass");
     histos_th1f["h_RECO_AntiS_m"] = dir_RECO_AntiS_mass.make<TH1F>(b+"h_RECO_AntiS_m", "; #bar{S} m (GeV); #entries ",200,0,20);

     TFileDirectory dir_RECO_AntiS_mass_deltaR_cut = dir_RECO_AntiS.mkdir("RECO_AntiS_mass_deltaR_cut");
     histos_th1f["h_RECO_AntiS_m_deltaR_cut"] = dir_RECO_AntiS_mass_deltaR_cut.make<TH1F>(b+"h_RECO_AntiS_m_deltaR_cut", "; #bar{S} m (GeV); #entries ",200,0,20);
     histos_th1f["h_RECO_AntiS_m_lxy_and_deltaR_cut"] = dir_RECO_AntiS_mass_deltaR_cut.make<TH1F>(b+"h_RECO_AntiS_m_lxy_and_deltaR_cut", "; #bar{S} m (GeV); #entries ",200,0,20);
     histos_th2f["h2_RECO_AntiS_m_lxy_and_deltaR_cut_iEvent"] = dir_RECO_AntiS_mass_deltaR_cut.make<TH2F>(b+"h2_RECO_AntiS_m_lxy_and_deltaR_cut_iEvent", "; #bar{S} m (GeV); eventId ",200,0,20, 10000, 0, 10000);
     histos_th2f["h2_RECO_AntiS_error_lxy_m_deltaR_and_lxy_cut"] = dir_RECO_AntiS_mass_deltaR_cut.make<TH2F>(b+"h2_RECO_AntiS_error_lxy_m_deltaR_and_lxy_cut", "; error lxy (cm) ; #bar{S} m (GeV) ",500,0,10, 400, -20, 20);
     histos_th1f["h_RECO_AntiS_m_lxy_and_error_lxy_and_deltaR_cut"] = dir_RECO_AntiS_mass_deltaR_cut.make<TH1F>(b+"h_RECO_AntiS_m_lxy_and_error_lxy_and_deltaR_cut", "; #bar{S} m (GeV); #entries ",200,0,20);


     TFileDirectory dir_RECO_AntiS_kinematics_cut_deltaR_lxy_error_lxy = dir_RECO_AntiS.mkdir("RECO_AntiS_kinematics_cut_deltaR_lxy_error_lxy");
     histos_th1f["h_RECO_AntiS_pt_cut_deltaR_lxy_error_lxy"] = dir_RECO_AntiS_kinematics_cut_deltaR_lxy_error_lxy.make<TH1F>(b+"h_RECO_AntiS_pt_cut_deltaR_lxy_error_lxy", "; #bar{S} pT (GeV); #entries ",200,0,20);
     histos_th1f["h_RECO_AntiS_eta_cut_deltaR_lxy_error_lxy"] = dir_RECO_AntiS_kinematics_cut_deltaR_lxy_error_lxy.make<TH1F>(b+"h_RECO_AntiS_eta_cut_deltaR_lxy_error_lxy", "; #bar{S} #eta; #entries ",200,-10,10);
     histos_th1f["h_RECO_AntiS_phi_cut_deltaR_lxy_error_lxy"] = dir_RECO_AntiS_kinematics_cut_deltaR_lxy_error_lxy.make<TH1F>(b+"h_RECO_AntiS_phi_cut_deltaR_lxy_error_lxy", "; #bar{S} #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_RECO_AntiS_vx_vy_cut_deltaR_lxy_error_lxy"] = dir_RECO_AntiS_kinematics_cut_deltaR_lxy_error_lxy.make<TH2F>(b+"h2_RECO_AntiS_vx_vy_cut_deltaR_lxy_error_lxy", "; #bar{S} vx (cm); #bar{S} vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_RECO_AntiS_vx_vz_cut_deltaR_lxy_error_lxy"] = dir_RECO_AntiS_kinematics_cut_deltaR_lxy_error_lxy.make<TH2F>(b+"h2_RECO_AntiS_vx_vz_cut_deltaR_lxy_error_lxy", "; #bar{S} vx (cm); #bar{S} vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_RECO_AntiS_lxy_cut_deltaR_lxy_error_lxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_lxy_cut_deltaR_lxy_error_lxy", "; #bar{S} lxy(beamspot, #bar{S} vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_RECO_AntiS_vz_cut_deltaR_lxy_error_lxy"] = dir_RECO_AntiS_kinematics_cut_deltaR_lxy_error_lxy.make<TH1F>(b+"h_RECO_AntiS_vz_cut_deltaR_lxy_error_lxy", "; #bar{S} vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_RECO_AntiS_dxy_cut_deltaR_lxy_error_lxy"] = dir_RECO_AntiS_kinematics_cut_deltaR_lxy_error_lxy.make<TH1F>(b+"h_RECO_AntiS_dxy_cut_deltaR_lxy_error_lxy", "; #bar{S} dxy(beamspot) (cm); #entries ",400,-20,20);



     TFileDirectory dir_RECO_AntiS_mass_deltaPhi_cut = dir_RECO_AntiS.mkdir("RECO_AntiS_mass_deltaPhi_cut");
     histos_th1f["h_RECO_AntiS_m_deltaPhi_cut"] = dir_RECO_AntiS_mass_deltaPhi_cut.make<TH1F>(b+"h_RECO_AntiS_m_deltaPhi_cut", "; #bar{S} m (GeV); #entries ",200,0,20);
     histos_th1f["h_RECO_AntiS_m_lxy_and_deltaPhi_cut"] = dir_RECO_AntiS_mass_deltaPhi_cut.make<TH1F>(b+"h_RECO_AntiS_m_lxy_and_deltaPhi_cut", "; #bar{S} m (GeV); #entries ",200,0,20);
     histos_th1f["h_RECO_AntiS_m_lxy_and_error_lxy_and_deltaPhi_cut"] = dir_RECO_AntiS_mass_deltaPhi_cut.make<TH1F>(b+"h_RECO_AntiS_m_lxy_and_error_lxy_and_deltaPhi_cut", "; #bar{S} m (GeV); #entries ",200,0,20);
     TFileDirectory dir_RECO_AntiS_error_lxy = dir_RECO_AntiS.mkdir("RECO_AntiS_error_lxy");
     histos_th1f["h_RECO_AntiS_error_lxy_cut_lxy_and_deltaR_and_mass"] = dir_RECO_AntiS_error_lxy.make<TH1F>(b+"h_RECO_AntiS_error_lxy_cut_lxy_and_deltaR_and_mass", "; error lxy(cm); #entries ",1000,0,10);
     histos_th2f["h2_RECO_AntiS_lxy_error_lxy_cut_lxy_and_deltaR_and_mass"] = dir_RECO_AntiS_error_lxy.make<TH2F>(b+"h2_RECO_AntiS_lxy_error_lxy_cut_lxy_and_deltaR_and_mass", "; lxy (cm); error lxy(cm); #entries ",100,0,10, 500,0,5);

     TFileDirectory dir_RECO_AntiS_special = dir_RECO_AntiS.mkdir("RECO_AntiS_special");
     histos_th1f["h_RECO_AntiS_m_matchedDaughtersFound"] = dir_RECO_AntiS_special.make<TH1F>(b+"h_RECO_AntiS_m_matchedDaughtersFound", "; #bar{S} m (GeV); #entries ",200,0,20);
     histos_th2f["h2_RECO_AntiS_m_matchedDaughtersFound_eventId"] = dir_RECO_AntiS_special.make<TH2F>(b+"h2_RECO_AntiS_m_matchedDaughtersFound_eventId", "; #bar{S} m (GeV); eventId ",200,0,20,10000,0,10000);


    

}

void AnalyzerRECO::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup) {

 
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
		FillHistosPV(PrimVertex, beamspot);
	}
   }
  

  //loop over the RECO Ks to plot the kinematics
  if(h_V0Ks.isValid()){
      for(unsigned int i = 0; i < h_V0Ks->size(); ++i){//loop all Ks candidates
	const reco::VertexCompositeCandidate * Ks = &h_V0Ks->at(i);	
	FillHistosRECOKs(Ks,beamspot);
      }
  }

  //loop over the RECO AntiLambda to plot the kinematics
  if(h_V0L.isValid()){
      for(unsigned int i = 0; i < h_V0L->size(); ++i){//loop all Ks candidates
	const reco::VertexCompositeCandidate * L = &h_V0L->at(i);	
	FillHistosRECOLambda(L,beamspot);
      }
  }


  //loop over the RECO AntiS to plot the kinematics
  if(h_sCands.isValid()){
      for(unsigned int i = 0; i < h_sCands->size(); ++i){//loop all S candidates
	const reco::VertexCompositeCandidate * antiS = &h_sCands->at(i);	
	//the if below is important, if it is -1 you are looking at signal (antiS). If it is +1 you are looking at background (S).
	int chargeAntiProton = 1; //by default look at the background
	if(m_lookAtAntiS) chargeAntiProton = -1;//only when the m_lookAtAntiS flag is enabled look at the antiS, which has a charge of -1 (representing the charge of the proton)
	if(antiS->charge()==chargeAntiProton)FillHistosRECOAntiS(antiS, beamspot, beamspotVariance, iEvent.id().event());
      }
  }



 } //end of analyzer

void AnalyzerRECO::FillHistosPV(reco::Vertex PrimVertex, TVector3 beamspot){
	histos_th1f["h_PV_vz"]->Fill(PrimVertex.z());
}


void AnalyzerRECO::FillHistosRECOKs(const reco::VertexCompositeCandidate * RECOKs, TVector3 beamspot){
	TVector3 KsCreationVertex(RECOKs->vx(),RECOKs->vy(),RECOKs->vz());
	double Lxy = AnalyzerAllSteps::lxy(beamspot,KsCreationVertex);
	TVector3 KsMomentum(RECOKs->px(),RECOKs->py(),RECOKs->pz());
	double dxy = AnalyzerAllSteps::dxy_signed_line_point(KsCreationVertex,KsMomentum,beamspot);
	histos_th1f["h_RECO_Ks_pt"]->Fill(RECOKs->pt());	
	histos_th1f["h_RECO_Ks_eta"]->Fill(RECOKs->eta());	
	histos_th1f["h_RECO_Ks_phi"]->Fill(RECOKs->phi());	
	histos_th2f["h2_RECO_Ks_vx_vy"]->Fill(RECOKs->vx(),RECOKs->vy());
	histos_th2f["h2_RECO_Ks_vx_vz"]->Fill(RECOKs->vx(),RECOKs->vz());
	histos_th1f["h_RECO_Ks_lxy"]->Fill(Lxy);	
	histos_th1f["h_RECO_Ks_vz"]->Fill(RECOKs->vz());	
	histos_th1f["h_RECO_Ks_dxy"]->Fill(dxy);
	histos_th1f["h_RECO_Ks_m"]->Fill(RECOKs->mass());

}

void AnalyzerRECO::FillHistosRECOLambda(const reco::VertexCompositeCandidate * RECOLambda, TVector3 beamspot){
	TVector3 LambdaCreationVertex(RECOLambda->vx(),RECOLambda->vy(),RECOLambda->vz());
	double Lxy = AnalyzerAllSteps::lxy(beamspot,LambdaCreationVertex);
	TVector3 LambdaMomentum(RECOLambda->px(),RECOLambda->py(),RECOLambda->pz());
	double dxy = AnalyzerAllSteps::dxy_signed_line_point(LambdaCreationVertex,LambdaMomentum,beamspot);
	histos_th1f["h_RECO_Lambda_pt"]->Fill(RECOLambda->pt());	
	histos_th1f["h_RECO_Lambda_eta"]->Fill(RECOLambda->eta());	
	histos_th1f["h_RECO_Lambda_phi"]->Fill(RECOLambda->phi());	
	histos_th2f["h2_RECO_Lambda_vx_vy"]->Fill(RECOLambda->vx(),RECOLambda->vy());
	histos_th2f["h2_RECO_Lambda_vx_vz"]->Fill(RECOLambda->vx(),RECOLambda->vz());
	histos_th1f["h_RECO_Lambda_lxy"]->Fill(Lxy);	
	histos_th1f["h_RECO_Lambda_vz"]->Fill(RECOLambda->vz());	
	histos_th1f["h_RECO_Lambda_dxy"]->Fill(dxy);	
	histos_th1f["h_RECO_Lambda_m"]->Fill(RECOLambda->mass());	


}

void AnalyzerRECO::FillHistosRECOAntiS(const reco::VertexCompositeCandidate * RECOAntiS, TVector3 beamspot, TVector3 beamspotVariance, int eventId){

	TVector3 AntiSCreationVertex(RECOAntiS->vx(),RECOAntiS->vy(),RECOAntiS->vz());
	double Lxy = AnalyzerAllSteps::lxy(beamspot,AntiSCreationVertex);
	double error_Lxy = AnalyzerAllSteps::std_dev_lxy(RECOAntiS->vx(),RECOAntiS->vy(), RECOAntiS->vertexCovariance(0,0), RECOAntiS->vertexCovariance(1,1), beamspot.X(), beamspot.Y(), beamspotVariance.X(), beamspotVariance.Y());
	TVector3 AntiSMomentum(RECOAntiS->px(),RECOAntiS->py(),RECOAntiS->pz());
	double dxy = AnalyzerAllSteps::dxy_signed_line_point(AntiSCreationVertex,AntiSMomentum,beamspot);
	
	double deltaPhiDaughters = reco::deltaPhi(RECOAntiS->daughter(0)->phi(),RECOAntiS->daughter(1)->phi());
	double deltaEtaDaughters = RECOAntiS->daughter(0)->eta()-RECOAntiS->daughter(1)->eta();
	double deltaRDaughters = sqrt(deltaPhiDaughters*deltaPhiDaughters+deltaEtaDaughters*deltaEtaDaughters);
	reco::LeafCandidate::LorentzVector n_(0,0,0,0.939565);
	double massAntiS = (RECOAntiS->p4()-n_).mass();
	histos_th1f["h_RECO_AntiS_pt"]->Fill(RECOAntiS->pt());	
	histos_th1f["h_RECO_AntiS_eta"]->Fill(RECOAntiS->eta());	
	histos_th1f["h_RECO_AntiS_phi"]->Fill(RECOAntiS->phi());	
	histos_th2f["h2_RECO_AntiS_vx_vy"]->Fill(RECOAntiS->vx(),RECOAntiS->vy());
	histos_th2f["h2_RECO_AntiS_vx_vz"]->Fill(RECOAntiS->vx(),RECOAntiS->vz());
	histos_th1f["h_RECO_AntiS_vz"]->Fill(RECOAntiS->vz());	
	histos_th1f["h_RECO_AntiS_dxy"]->Fill(dxy);	

	//the stuff we cut on:
	histos_th1f["h_RECO_AntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_RECO_AntiS_error_lxy"]->Fill(error_Lxy);
	histos_th1f["h_RECO_AntiS_deltaRDaughters"]->Fill(deltaRDaughters);
	histos_th1f["h_RECO_AntiS_deltaPhiDaughters"]->Fill(deltaPhiDaughters);
	if(error_Lxy < 0.1)histos_th2f["h2_RECO_AntiS_deltaR_duaghters_lxy_cut_error_lxy"]->Fill(deltaRDaughters,Lxy);
	if(error_Lxy < 0.1)histos_th3f["h3_RECO_AntiS_deltaR_duaghters_lxy_mass_cut_error_lxy"]->Fill(deltaRDaughters,Lxy,RECOAntiS->mass());
	if(error_Lxy < 0.1)histos_th2f["h2_RECO_AntiS_deltaPhi_duaghters_lxy_cut_error_lxy"]->Fill(deltaPhiDaughters,Lxy);
	//is the deltaR distribution good for background cut?	
	if(Lxy > 1.9)histos_th1f["h_RECO_AntiS_deltaRDaughters_Lxy_cut"]->Fill(deltaRDaughters);
	if(Lxy > 1.9 && error_Lxy < 0.1)histos_th1f["h_RECO_AntiS_deltaRDaughters_Lxy_and_error_Lxy_cut"]->Fill(deltaRDaughters);
	if(error_Lxy < 0.1)histos_th2f["h2_RECO_AntiS_deltaRDaughters_lxy_error_lxy_cut"]->Fill(deltaRDaughters, Lxy);
	//is the deltaPhi distribution better for background cut?
	if(Lxy > 1.9)histos_th1f["h_RECO_AntiS_deltaPhiDaughters_Lxy_cut"]->Fill(deltaPhiDaughters);
	if(Lxy > 1.9 && error_Lxy < 0.1)histos_th1f["h_RECO_AntiS_deltaPhiDaughters_Lxy_and_error_Lxy_cut"]->Fill(deltaPhiDaughters);
	if(error_Lxy < 0.1)histos_th2f["h2_RECO_AntiS_deltaPhiDaughters_lxy_error_lxy_cut"]->Fill(deltaPhiDaughters, Lxy);
	//plot the antiS mass with different cuts: 
	histos_th1f["h_RECO_AntiS_m"]->Fill(massAntiS);
	if(deltaRDaughters < 6 && deltaRDaughters > 1)histos_th1f["h_RECO_AntiS_m_deltaR_cut"]->Fill(massAntiS);	
	if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9)histos_th1f["h_RECO_AntiS_m_lxy_and_deltaR_cut"]->Fill(massAntiS);	
	if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9)histos_th2f["h2_RECO_AntiS_m_lxy_and_deltaR_cut_iEvent"]->Fill(massAntiS, eventId);
	if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9)histos_th2f["h2_RECO_AntiS_error_lxy_m_deltaR_and_lxy_cut"]->Fill(error_Lxy, massAntiS);	
	if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9 && error_Lxy < 0.1)histos_th1f["h_RECO_AntiS_m_lxy_and_error_lxy_and_deltaR_cut"]->Fill(massAntiS);	
	//with this cut, which should be the one you want to apply on data plot the kinematics of the antiS which survive
	if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9 && error_Lxy < 0.1){
	
		histos_th1f["h_RECO_AntiS_pt_cut_deltaR_lxy_error_lxy"]->Fill(RECOAntiS->pt());	
		histos_th1f["h_RECO_AntiS_eta_cut_deltaR_lxy_error_lxy"]->Fill(RECOAntiS->eta());	
		histos_th1f["h_RECO_AntiS_phi_cut_deltaR_lxy_error_lxy"]->Fill(RECOAntiS->phi());	
		histos_th2f["h2_RECO_AntiS_vx_vy_cut_deltaR_lxy_error_lxy"]->Fill(RECOAntiS->vx(),RECOAntiS->vy());
		histos_th2f["h2_RECO_AntiS_vx_vz_cut_deltaR_lxy_error_lxy"]->Fill(RECOAntiS->vx(),RECOAntiS->vz());
		histos_th1f["h_RECO_AntiS_lxy_cut_deltaR_lxy_error_lxy"]->Fill(Lxy);	
		histos_th1f["h_RECO_AntiS_vz_cut_deltaR_lxy_error_lxy"]->Fill(RECOAntiS->vz());	
		histos_th1f["h_RECO_AntiS_dxy_cut_deltaR_lxy_error_lxy"]->Fill(dxy);	

	}
		
	if(abs(deltaPhiDaughters) < 2.5 && abs(deltaPhiDaughters) > 1)histos_th1f["h_RECO_AntiS_m_deltaPhi_cut"]->Fill(massAntiS);	
	if(abs(deltaPhiDaughters) < 2.5 && abs(deltaPhiDaughters) > 1 && Lxy > 1.9)histos_th1f["h_RECO_AntiS_m_lxy_and_deltaPhi_cut"]->Fill(massAntiS);	
	if(abs(deltaPhiDaughters) < 2.5 && abs(deltaPhiDaughters) > 1 && Lxy > 1.9 && error_Lxy < 0.1)histos_th1f["h_RECO_AntiS_m_lxy_and_error_lxy_and_deltaPhi_cut"]->Fill(massAntiS);

	if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9 && massAntiS > 1.5 && massAntiS < 2.1)histos_th1f["h_RECO_AntiS_error_lxy_cut_lxy_and_deltaR_and_mass"]->Fill(error_Lxy);	
	if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9 && massAntiS > 1.5 && massAntiS < 2.1)histos_th2f["h2_RECO_AntiS_lxy_error_lxy_cut_lxy_and_deltaR_and_mass"]->Fill(Lxy,error_Lxy);	
		


}
void AnalyzerRECO::endJob()
{
}

AnalyzerRECO::~AnalyzerRECO()
{
}


DEFINE_FWK_MODULE(AnalyzerRECO);
