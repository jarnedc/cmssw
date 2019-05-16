#include "../interface/AnalyzerAllSteps.h"
#include <typeinfo>

AnalyzerAllSteps::AnalyzerAllSteps(edm::ParameterSet const& pset):
  m_isData(pset.getUntrackedParameter<bool>("isData")),
  m_bsTag(pset.getParameter<edm::InputTag>("beamspot")),
  m_offlinePVTag(pset.getParameter<edm::InputTag>("offlinePV")),
  m_genParticlesTag_GEN(pset.getParameter<edm::InputTag>("genCollection_GEN")),
  m_genParticlesTag_SIM_GEANT(pset.getParameter<edm::InputTag>("genCollection_SIM_GEANT")),
  m_generalTracksTag(pset.getParameter<edm::InputTag>("generalTracksCollection")),
  m_sCandsTag(pset.getParameter<edm::InputTag>("sexaqCandidates")),
  m_V0KsTag(pset.getParameter<edm::InputTag>("V0KsCollection")),
  m_V0LTag(pset.getParameter<edm::InputTag>("V0LCollection")),
  m_trackAssociatorTag(pset.getParameter<edm::InputTag>("trackAssociators")),
  m_TPTag(pset.getParameter<edm::InputTag>("TrackingParticles")),

  m_bsToken    (consumes<reco::BeamSpot>(m_bsTag)),
  m_offlinePVToken    (consumes<vector<reco::Vertex>>(m_offlinePVTag)),
  m_genParticlesToken_GEN(consumes<vector<reco::GenParticle> >(m_genParticlesTag_GEN)),
  m_genParticlesToken_SIM_GEANT(consumes<vector<reco::GenParticle> >(m_genParticlesTag_SIM_GEANT)),
  //m_generalTracksToken(consumes<vector<reco::Track> >(m_generalTracksTag)),
  m_generalTracksToken(consumes<View<reco::Track> >(m_generalTracksTag)),
  m_sCandsToken(consumes<vector<reco::VertexCompositeCandidate> >(m_sCandsTag)),
  m_V0KsToken(consumes<vector<reco::VertexCompositeCandidate> >(m_V0KsTag)),
  m_V0LToken(consumes<vector<reco::VertexCompositeCandidate> >(m_V0LTag)),
  m_trackAssociatorToken(consumes<reco::TrackToTrackingParticleAssociator> (m_trackAssociatorTag)),
  m_TPToken(consumes<vector<TrackingParticle> >(m_TPTag))
  


{

}


void AnalyzerAllSteps::beginJob() {

     TFileDirectory dir_TrackingEff = m_fs->mkdir("TrackingEff"); 

     TFileDirectory dir_TrackingEff_Global = dir_TrackingEff.mkdir("Global");
 
     histos_th2i["h2_GlobalEfficiencies"] = dir_TrackingEff_Global.make<TH2I>(b+"h2_GlobalEfficiencies", ";#bar{#Lambda}: 0=no daughters RECO, 1=#{p} daug RECO, 2=#pi^{+} daug RECO, 3=#bar{p}&#pi^{+} daug RECO, 4=V0-#bar{#Lambda} RECO; Ks: 0=no daughters RECO, 1=#pi^{+} daug RECO, 2=#pi^{-} daug RECO, 3=#pi^{+}&#pi^{-} daug RECO, 4=V0-Ks RECO",5,-0.5,4.5,5,-0.5,4.5);
     histos_th1f["h_deltaR_V0Ks_momentumSumKsDaughterTracks"] = dir_TrackingEff_Global.make<TH1F>(b+"h_deltaR_V0Ks_momentumSumKsDaughterTracks", ";Daughter #bar{S} #DeltaR(RECO V0-Ks,#vec{p_{matched RECO track daug1}}+#vec{p_{matched RECO track daug2}}); #entries ",1000,0,10);
     histos_th1f["h_deltaR_V0AntiL_momentumSumAntiLDaughterTracks"] = dir_TrackingEff_Global.make<TH1F>(b+"h_deltaR_V0AntiL_momentumSumAntiLDaughterTracks", ";Daughter #bar{S} #DeltaR(RECO V0-#bar{#Lambda},#vec{p_{matched RECO track daug1}}+#vec{p_{matched RECO track daug2}}); #entries ",1000,0,10);

     TFileDirectory dir_TrackingEff_NonAntiS = dir_TrackingEff.mkdir("NonAntiS"); 
     
     TFileDirectory dir_TrackingEff_NonAntiS_RECO = dir_TrackingEff_NonAntiS.mkdir("RECO"); 
     histos_th1f["h_NonAntiSTrack_RECO_pt"] = dir_TrackingEff_NonAntiS_RECO.make<TH1F>(b+"h_NonAntiSTrack_RECO_pt", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_NonAntiSTrack_RECO_eta"] = dir_TrackingEff_NonAntiS_RECO.make<TH1F>(b+"h_NonAntiSTrack_RECO_eta", "; SIM track #eta; #entries ",200,-10,10);
     histos_th1f["h_NonAntiSTrack_RECO_phi"] = dir_TrackingEff_NonAntiS_RECO.make<TH1F>(b+"h_NonAntiSTrack_RECO_phi", "; SIM track #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_NonAntiSTrack_RECO_vx_vy"] = dir_TrackingEff_NonAntiS_RECO.make<TH2F>(b+"h2_NonAntiSTrack_RECO_vx_vy", "; SIM track vx (cm); SIM track vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_NonAntiSTrack_RECO_vx_vz"] = dir_TrackingEff_NonAntiS_RECO.make<TH2F>(b+"h2_NonAntiSTrack_RECO_vx_vz", "; SIM track vx (cm); SIM track vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_NonAntiSTrack_RECO_lxy"] = dir_TrackingEff_NonAntiS_RECO.make<TH1F>(b+"h_NonAntiSTrack_RECO_lxy", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_NonAntiSTrack_RECO_vz"] = dir_TrackingEff_NonAntiS_RECO.make<TH1F>(b+"h_NonAntiSTrack_RECO_vz", "; SIM track vz(SIM track vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_NonAntiSTrack_RECO_dxy"] = dir_TrackingEff_NonAntiS_RECO.make<TH1F>(b+"h_NonAntiSTrack_RECO_dxy", "; SIM track dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_NonAntiSTrack_RECO_pt_cut_eta_Lxy"] = dir_TrackingEff_NonAntiS_RECO.make<TH1F>(b+"h_NonAntiSTrack_RECO_pt_cut_eta_Lxy", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_NonAntiSTrack_RECO_lxy_cut_pt_eta"] = dir_TrackingEff_NonAntiS_RECO.make<TH1F>(b+"h_NonAntiSTrack_RECO_lxy_cut_pt_eta", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_NonAntiSTrack_RECO_eta_cut_pt_lxy"] = dir_TrackingEff_NonAntiS_RECO.make<TH1F>(b+"h_NonAntiSTrack_RECO_eta_cut_pt_lxy", "; SIM track #eta; #entries ",200,-10,10);


     TFileDirectory dir_TrackingEff_NonAntiS_All = dir_TrackingEff_NonAntiS.mkdir("All"); 
     histos_th1f["h_NonAntiSTrack_All_pt"] = dir_TrackingEff_NonAntiS_All.make<TH1F>(b+"h_NonAntiSTrack_All_pt", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_NonAntiSTrack_All_eta"] = dir_TrackingEff_NonAntiS_All.make<TH1F>(b+"h_NonAntiSTrack_All_eta", "; SIM track #eta; #entries ",200,-10,10);
     histos_th1f["h_NonAntiSTrack_All_phi"] = dir_TrackingEff_NonAntiS_All.make<TH1F>(b+"h_NonAntiSTrack_All_phi", "; SIM track #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_NonAntiSTrack_All_vx_vy"] = dir_TrackingEff_NonAntiS_All.make<TH2F>(b+"h2_NonAntiSTrack_All_vx_vy", "; SIM track vx (cm); SIM track vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_NonAntiSTrack_All_vx_vz"] = dir_TrackingEff_NonAntiS_All.make<TH2F>(b+"h2_NonAntiSTrack_All_vx_vz", "; SIM track vx (cm); SIM track vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_NonAntiSTrack_All_lxy"] = dir_TrackingEff_NonAntiS_All.make<TH1F>(b+"h_NonAntiSTrack_All_lxy", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_NonAntiSTrack_All_vz"] = dir_TrackingEff_NonAntiS_All.make<TH1F>(b+"h_NonAntiSTrack_All_vz", "; SIM track vz(SIM track vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_NonAntiSTrack_All_dxy"] = dir_TrackingEff_NonAntiS_All.make<TH1F>(b+"h_NonAntiSTrack_All_dxy", "; SIM track dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_NonAntiSTrack_All_pt_cut_eta_Lxy"] = dir_TrackingEff_NonAntiS_All.make<TH1F>(b+"h_NonAntiSTrack_All_pt_cut_eta_Lxy", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_NonAntiSTrack_All_lxy_cut_pt_eta"] = dir_TrackingEff_NonAntiS_All.make<TH1F>(b+"h_NonAntiSTrack_All_lxy_cut_pt_eta", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_NonAntiSTrack_All_eta_cut_pt_lxy"] = dir_TrackingEff_NonAntiS_All.make<TH1F>(b+"h_NonAntiSTrack_All_eta_cut_pt_lxy", "; SIM track #eta; #entries ",200,-10,10);

     TFileDirectory dir_TrackingEff_KsAntiS = dir_TrackingEff.mkdir("KsAntiS"); 
     
     TFileDirectory dir_TrackingEff_KsAntiS_RECO = dir_TrackingEff_KsAntiS.mkdir("RECO"); 
     histos_th1f["h_AntiSKsDaughterTracks_RECO_pt"] = dir_TrackingEff_KsAntiS_RECO.make<TH1F>(b+"h_AntiSKsDaughterTracks_RECO_pt", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_AntiSKsDaughterTracks_RECO_eta"] = dir_TrackingEff_KsAntiS_RECO.make<TH1F>(b+"h_AntiSKsDaughterTracks_RECO_eta", "; SIM track #eta; #entries ",200,-10,10);
     histos_th1f["h_AntiSKsDaughterTracks_RECO_phi"] = dir_TrackingEff_KsAntiS_RECO.make<TH1F>(b+"h_AntiSKsDaughterTracks_RECO_phi", "; SIM track #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_AntiSKsDaughterTracks_RECO_vx_vy"] = dir_TrackingEff_KsAntiS_RECO.make<TH2F>(b+"h2_AntiSKsDaughterTracks_RECO_vx_vy", "; SIM track vx (cm); SIM track vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_AntiSKsDaughterTracks_RECO_vx_vz"] = dir_TrackingEff_KsAntiS_RECO.make<TH2F>(b+"h2_AntiSKsDaughterTracks_RECO_vx_vz", "; SIM track vx (cm); SIM track vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_AntiSKsDaughterTracks_RECO_lxy"] = dir_TrackingEff_KsAntiS_RECO.make<TH1F>(b+"h_AntiSKsDaughterTracks_RECO_lxy", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_AntiSKsDaughterTracks_RECO_vz"] = dir_TrackingEff_KsAntiS_RECO.make<TH1F>(b+"h_AntiSKsDaughterTracks_RECO_vz", "; SIM track vz(SIM track vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_AntiSKsDaughterTracks_RECO_dxy"] = dir_TrackingEff_KsAntiS_RECO.make<TH1F>(b+"h_AntiSKsDaughterTracks_RECO_dxy", "; SIM track dxy(beamspot) (cm); #entries ",400,-20,20);


     TFileDirectory dir_TrackingEff_KsAntiS_All = dir_TrackingEff_KsAntiS.mkdir("All"); 
     histos_th1f["h_AntiSKsDaughterTracks_All_pt"] = dir_TrackingEff_KsAntiS_All.make<TH1F>(b+"h_AntiSKsDaughterTracks_All_pt", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_AntiSKsDaughterTracks_All_eta"] = dir_TrackingEff_KsAntiS_All.make<TH1F>(b+"h_AntiSKsDaughterTracks_All_eta", "; SIM track #eta; #entries ",200,-10,10);
     histos_th1f["h_AntiSKsDaughterTracks_All_phi"] = dir_TrackingEff_KsAntiS_All.make<TH1F>(b+"h_AntiSKsDaughterTracks_All_phi", "; SIM track #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_AntiSKsDaughterTracks_All_vx_vy"] = dir_TrackingEff_KsAntiS_All.make<TH2F>(b+"h2_AntiSKsDaughterTracks_All_vx_vy", "; SIM track vx (cm); SIM track vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_AntiSKsDaughterTracks_All_vx_vz"] = dir_TrackingEff_KsAntiS_All.make<TH2F>(b+"h2_AntiSKsDaughterTracks_All_vx_vz", "; SIM track vx (cm); SIM track vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_AntiSKsDaughterTracks_All_lxy"] = dir_TrackingEff_KsAntiS_All.make<TH1F>(b+"h_AntiSKsDaughterTracks_All_lxy", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_AntiSKsDaughterTracks_All_vz"] = dir_TrackingEff_KsAntiS_All.make<TH1F>(b+"h_AntiSKsDaughterTracks_All_vz", "; SIM track vz(SIM track vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_AntiSKsDaughterTracks_All_dxy"] = dir_TrackingEff_KsAntiS_All.make<TH1F>(b+"h_AntiSKsDaughterTracks_All_dxy", "; SIM track dxy(beamspot) (cm); #entries ",400,-20,20);


     TFileDirectory dir_GEN = m_fs->mkdir("GEN"); 

     TFileDirectory dir_GEN_antiS = dir_GEN.mkdir("GEN_antiS"); 
     histos_th1f["h_GEN_nAntiSTotal"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_nAntiSTotal", "; 0 = all #bar{S}/ 1 = interacting #bar{S}/ 2 = #bar{S} with corr grandd; #entries ",20,0,20);
     histos_th1f["h_GEN_nAntiS"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_nAntiS", "; #bar{S}/event; #entries ",20,0,20);
     histos_th1f["h_GEN_nAntiSInteract"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_nAntiSInteract", "; #bar{S} which decay/event; #entries ",20,0,20);
     histos_th1f["h_GEN_AntiS_pt"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_AntiS_pt", "; #bar{S} pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GEN_AntiS_eta"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_AntiS_eta", "; #bar{S} #eta; #entries ",200,-10,10);
     histos_th1f["h_GEN_AntiS_phi"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_AntiS_phi", "; #bar{S} #phi (rad); #entries ",200,-4,4);
     histos_th1f["h_GEN_AntiS_deltaR_daughters"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_AntiS_deltaR_daughters", "; #Delta R(Ks,#bar{#Lambda}); #entries ",200,0,20);
     histos_th1f["h_GEN_AntiS_deltaPhi_daughters"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_AntiS_deltaPhi_daughters", "; #Delta #Phi(Ks,#bar{#Lambda}); #entries ",200,-4,4);
     histos_th2f["h2_GEN_AntiS_deltaPhi_daughters_lxy_interactionVertex"] = dir_GEN_antiS.make<TH2F>(b+"h2_GEN_AntiS_deltaPhi_daughters_lxy_interactionVertex", "; #Delta #Phi(Ks,#bar{#Lambda}); Lxy(beampsot, antiS interaction vertex) ",200,-4,4,200,0,200);
     histos_th1f["h_GEN_AntiS_AnglePAntiSPKs"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_AntiS_AnglePAntiSPKs", "; angle(#vec(p_{Ks}),#vec(p_{S})); #entries ",8000,-4,4);
     histos_th2f["h2_GEN_AntiS_PAntiS_AnglePAntiSPKs"] = dir_GEN_antiS.make<TH2F>(b+"h2_GEN_AntiS_PAntiS_AnglePAntiSPKs", "; p #bar{S}; angle(#vec(p_{Ks}),#vec(p_{#bar{S}})); #entries ",500,0,50,800,-4,4);
     histos_th1f["h_GEN_AntiS_AnglePAntiSPAntiLambda"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_AntiS_AnglePAntiSPAntiLambda", "; angle(#vec(p_{#bar{#Lambda}}),#vec(p_{S})); #entries ",8000,-4,4);
     histos_th2f["h2_GEN_AntiS_PAntiS_AnglePAntiSPAntiLambda"] = dir_GEN_antiS.make<TH2F>(b+"h2_GEN_AntiS_PAntiS_AnglePAntiSPAntiLambda", "; p #bar{S}; angle(#vec(p_{#bar{#Lambda}}),#vec(p_{#bar{S}})); #entries ",500,0,50,800,-4,4);
     histos_th2f["h2_GEN_AntiS_PointingAngleKs_PointingAngleAntiLambda"] = dir_GEN_antiS.make<TH2F>(b+"h2_GEN_AntiS_PointingAngleKs_PointingAngleAntiLambda", "; cos[(#vec{l_{xy}}(Ks daughter).#vec{p_{xy}}(Ks))/(||#vec{l_{xy}}(Ks daughter)||.||#vec{p_{xy}}(Ks)||)]; cos[(#vec{l_{xy}}(#bar{#Lambda daughter}).#vec{p_{xy}}(#bar{#Lambda}))/(||#vec{l_{xy}}(#bar{#Lambda daughter})||.||#vec{p_{xy}}(#bar{#Lambda})||)]; #entries ",1000,0,1,1000,0,1);

     TFileDirectory dir_GEN_KsNonAntiS = dir_GEN.mkdir("GEN_KsNonAntiS");
     histos_th1f["h_GEN_nKs"] = dir_GEN_KsNonAntiS.make<TH1F>(b+"h_GEN_nKs", "; #Ks/event; #entries ",20,0,20); 
     histos_th1f["h_GEN_KsNonAntiS_pt"] = dir_GEN_KsNonAntiS.make<TH1F>(b+"h_GEN_KsNonAntiS_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GEN_KsNonAntiS_eta"] = dir_GEN_KsNonAntiS.make<TH1F>(b+"h_GEN_KsNonAntiS_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_GEN_KsNonAntiS_phi"] = dir_GEN_KsNonAntiS.make<TH1F>(b+"h_GEN_KsNonAntiS_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GEN_KsNonAntiS_vx_vy"] = dir_GEN_KsNonAntiS.make<TH2F>(b+"h2_GEN_KsNonAntiS_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GEN_KsNonAntiS_vx_vz"] = dir_GEN_KsNonAntiS.make<TH2F>(b+"h2_GEN_KsNonAntiS_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GEN_KsNonAntiS_lxy"] = dir_GEN_KsNonAntiS.make<TH1F>(b+"h_GEN_KsNonAntiS_lxy", "; Ks lxy(beamspot, Ks creationvertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GEN_KsNonAntiS_vz"] = dir_GEN_KsNonAntiS.make<TH1F>(b+"h_GEN_KsNonAntiS_vz", "; Ks vz(Ks creationvertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GEN_KsNonAntiS_dxy"] = dir_GEN_KsNonAntiS.make<TH1F>(b+"h_GEN_KsNonAntiS_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_GEN_KsNonAntiS_XYpointingAngle"] = dir_GEN_KsNonAntiS.make<TH1F>(b+"h_GEN_KsNonAntiS_XYpointingAngle", "; cos[(#vec{l_{xy}}(Ks daughter).#vec{p_{xy}}(Ks))/(||#vec{l_{xy}}(Ks daughter)||.||#vec{p_{xy}}(Ks)||)]; #entries ",20000,-1.5,1.5);

     TFileDirectory dir_GEN_AntiLambdaNonAntiS = dir_GEN.mkdir("GEN_AntiLambdaNonAntiS");
     histos_th1f["h_GEN_nAntiLambda"] = dir_GEN_AntiLambdaNonAntiS.make<TH1F>(b+"h_GEN_nAntiLambda", "; # #bar{#Lambda}/event; #entries ",20,0,20); 
     histos_th1f["h_GEN_AntiLambdaNonAntiS_pt"] = dir_GEN_AntiLambdaNonAntiS.make<TH1F>(b+"h_GEN_AntiLambdaNonAntiS_pt", "; #bar{#Lambda} pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GEN_AntiLambdaNonAntiS_eta"] = dir_GEN_AntiLambdaNonAntiS.make<TH1F>(b+"h_GEN_AntiLambdaNonAntiS_eta", "; #bar{#Lambda} #eta; #entries ",200,-10,10);
     histos_th1f["h_GEN_AntiLambdaNonAntiS_phi"] = dir_GEN_AntiLambdaNonAntiS.make<TH1F>(b+"h_GEN_AntiLambdaNonAntiS_phi", "; #bar{#Lambda} #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GEN_AntiLambdaNonAntiS_vx_vy"] = dir_GEN_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GEN_AntiLambdaNonAntiS_vx_vy", "; #bar{#Lambda} vx (cm); #bar{#Lambda} vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GEN_AntiLambdaNonAntiS_vx_vz"] = dir_GEN_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GEN_AntiLambdaNonAntiS_vx_vz", "; #bar{#Lambda} vx (cm); #bar{#Lambda} vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GEN_AntiLambdaNonAntiS_lxy"] = dir_GEN_AntiLambdaNonAntiS.make<TH1F>(b+"h_GEN_AntiLambdaNonAntiS_lxy", "; #bar{#Lambda} lxy(beamspot, #bar{#Lambda} creationvertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GEN_AntiLambdaNonAntiS_vz"] = dir_GEN_AntiLambdaNonAntiS.make<TH1F>(b+"h_GEN_AntiLambdaNonAntiS_vz", "; #bar{#Lambda} vz(#bar{#Lambda} creationvertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GEN_AntiLambdaNonAntiS_dxy"] = dir_GEN_AntiLambdaNonAntiS.make<TH1F>(b+"h_GEN_AntiLambdaNonAntiS_dxy", "; #bar{#Lambda} dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_GEN_AntiLambdaNonAntiS_XYpointingAngle"] = dir_GEN_AntiLambdaNonAntiS.make<TH1F>(b+"h_GEN_AntiLambdaNonAntiS_XYpointingAngle", "; AntiLambdaNonAntiS cos[(#vec{l_{xy}}(#bar{#Lambda daughter}).#vec{p_{xy}}(#bar{#Lambda}))/(||#vec{l_{xy}}(#bar{#Lambda daughter})||.||#vec{p_{xy}}(#bar{#Lambda})||)]; #entries ",20000,-1.5,1.5);

     TFileDirectory dir_GEN_KsAntiS = dir_GEN.mkdir("GEN_KsAntiS");
     histos_th1f["h_GEN_KsAntiS_pdgId"] = dir_GEN_KsAntiS.make<TH1F>(b+"h_GEN_KsAntiS_pdgId", "; KsAntiS pdgId ; #entries ",10000,-5000,5000);
     histos_th1f["h_GEN_KsAntiS_pt"] = dir_GEN_KsAntiS.make<TH1F>(b+"h_GEN_KsAntiS_pt", "; KsAntiS pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GEN_KsAntiS_eta"] = dir_GEN_KsAntiS.make<TH1F>(b+"h_GEN_KsAntiS_eta", "; KsAntiS #eta; #entries ",200,-10,10);
     histos_th1f["h_GEN_KsAntiS_phi"] = dir_GEN_KsAntiS.make<TH1F>(b+"h_GEN_KsAntiS_phi", "; KsAntiS #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GEN_KsAntiS_vx_vy"] = dir_GEN_KsAntiS.make<TH2F>(b+"h2_GEN_KsAntiS_vx_vy", "; KsAntiS vx (cm); KsAntiS vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GEN_KsAntiS_vx_vz"] = dir_GEN_KsAntiS.make<TH2F>(b+"h2_GEN_KsAntiS_vx_vz", "; KsAntiS vx (cm); KsAntiS vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GEN_KsAntiS_lxy"] = dir_GEN_KsAntiS.make<TH1F>(b+"h_GEN_KsAntiS_lxy", "; KsAntiS lxy(beamspot, KsAntiS creationvertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GEN_KsAntiS_vz"] = dir_GEN_KsAntiS.make<TH1F>(b+"h_GEN_KsAntiS_vz", "; KsAntiS vz(KsAntiS creationvertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GEN_KsAntiS_dxy"] = dir_GEN_KsAntiS.make<TH1F>(b+"h_GEN_KsAntiS_dxy", "; KsAntiS dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_GEN_KsAntiS_XYpointingAngle"] = dir_GEN_KsAntiS.make<TH1F>(b+"h_GEN_KsAntiS_XYpointingAngle", "; cos[(#vec{l_{xy}}(Ks daughter).#vec_{xy}{p}(Ks))/(||#vec{l_{xy}}(Ks daughter)||.||#vec{p_{xy}}(Ks)||)] ; #entries ",20000,-1.5,1.5);

     TFileDirectory dir_GEN_AntiLambdaAntiS = dir_GEN.mkdir("GEN_AntiLambdaAntiS");
     histos_th1f["h_GEN_AntiLambdaAntiS_pdgId"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_pdgId", "; #bar{#Lambda} pdgID; #entries ",10000,-5000,5000);
     histos_th1f["h_GEN_AntiLambdaAntiS_pt"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_pt", "; #bar{#Lambda} pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GEN_AntiLambdaAntiS_eta"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_eta", "; #bar{#Lambda} #eta; #entries ",200,-10,10);
     histos_th1f["h_GEN_AntiLambdaAntiS_phi"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_phi", "; #bar{#Lambda} #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GEN_AntiLambdaAntiS_vx_vy"] = dir_GEN_AntiLambdaAntiS.make<TH2F>(b+"h2_GEN_AntiLambdaAntiS_vx_vy", "; AntiLambdaAntiS vx (cm); AntiLambdaAntiS vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GEN_AntiLambdaAntiS_vx_vz"] = dir_GEN_AntiLambdaAntiS.make<TH2F>(b+"h2_GEN_AntiLambdaAntiS_vx_vz", "; AntiLambdaAntiS vx (cm); AntiLambdaAntiS vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GEN_AntiLambdaAntiS_lxy"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_lxy", "; AntiLambdaAntiS lxy(beamspot, AntiLambdaAntiS creationvertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GEN_AntiLambdaAntiS_vz"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_vz", "; AntiLambdaAntiS vz(AntiLambdaAntiS creationvertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GEN_AntiLambdaAntiS_dxy"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_dxy", "; AntiLambdaAntiS dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_GEN_AntiLambdaAntiS_XYpointingAngle"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_XYpointingAngle", "; AntiLambdaAntiS cos[(#vec{l_{xy}}(#bar{#Lambda} daughter).#vec{p_{xy}}(#bar{#Lambda}))/(||#vec{l_{xy}}(#bar{#Lambda daughter})||.||#vec{p_{xy}}(#bar{#Lambda})||)] ; #entries ",20000,-1.5,1.5);

     TFileDirectory dir_RECO = m_fs->mkdir("RECO"); 

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
     histos_th1f["h_RECO_Ks_deltaRMin_GEN_Ks"] = dir_RECO_Ks.make<TH1F>(b+"h_RECO_Ks_deltaRMin_GEN_Ks", "; #DeltaR_{min} (Ks gen, Ks reco); #entries ",1000,0,10);

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
     histos_th1f["h_RECO_AntiS_m_lxy_cut"] = dir_RECO_AntiS.make<TH1F>(b+"h_RECO_AntiS_m_lxy_cut", "; #bar{S} m (GeV); #entries ",200,0,20);

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


     histos_th2f["h2_RECO_AntiS_m_lxy_and_deltaR_cut_vs_deltaR_best_GEN_match"] = dir_RECO_AntiS_mass_deltaR_cut.make<TH2F>(b+"h2_RECO_AntiS_m_lxy_and_deltaR_cut_vs_deltaR_best_GEN_match", "; #bar{S} m (GeV); best #Delta R (GEN #bar{S}, RECO #bar{S}) ",200,0,20,100,0,10);
     histos_th2f["h2_RECO_AntiS_m_lxy_and_error_lxy_and_deltaR_cut_vs_deltaR_best_GEN_match"] = dir_RECO_AntiS_mass_deltaR_cut.make<TH2F>(b+"h2_RECO_AntiS_m_lxy_and_error_lxy_and_deltaR_cut_vs_deltaR_best_GEN_match", "; #bar{S} m (GeV); best #Delta R (GEN #bar{S}, RECO #bar{S}) ",200,0,20,100,0,10);

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


     TFileDirectory dir_GENRECO_KsNonAntiS = dir_RECO.mkdir("GENRECO_KsNonAntiS");
     histos_th1f["h_GENRECO_KsNonAntiS_deltaR"] = dir_GENRECO_KsNonAntiS.make<TH1F>(b+"h_GENRECO_KsNonAntiS_deltaR", "; #DeltaR(gen Ks, reco Ks); #entries ",1000,0,10);

     TFileDirectory dir_GENRECO_Matched_KsNonAntiS = dir_GENRECO_KsNonAntiS.mkdir("GENRECO_Matched_KsNonAntiS");
     histos_th1f["h_GENRECO_Matched_KsNonAntiS_pt"] = dir_GENRECO_Matched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsNonAntiS_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_Matched_KsNonAntiS_eta"] = dir_GENRECO_Matched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsNonAntiS_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_Matched_KsNonAntiS_phi"] = dir_GENRECO_Matched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsNonAntiS_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_Matched_KsNonAntiS_vx_vy"] = dir_GENRECO_Matched_KsNonAntiS.make<TH2F>(b+"h2_GENRECO_Matched_KsNonAntiS_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_Matched_KsNonAntiS_vx_vz"] = dir_GENRECO_Matched_KsNonAntiS.make<TH2F>(b+"h2_GENRECO_Matched_KsNonAntiS_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_Matched_KsNonAntiS_lxy"] = dir_GENRECO_Matched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsNonAntiS_lxy", "; Ks lxy(beamspot, Ks vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_Matched_KsNonAntiS_vz"] =  dir_GENRECO_Matched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsNonAntiS_vz", "; Ks vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_Matched_KsNonAntiS_dxy"] = dir_GENRECO_Matched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsNonAntiS_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_NotMatched_KsNonAntiS = dir_GENRECO_KsNonAntiS.mkdir("GENRECO_NotMatched_KsNonAntiS");
     histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_pt"] = dir_GENRECO_NotMatched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsNonAntiS_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_eta"] = dir_GENRECO_NotMatched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsNonAntiS_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_phi"] = dir_GENRECO_NotMatched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsNonAntiS_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_NotMatched_KsNonAntiS_vx_vy"] = dir_GENRECO_NotMatched_KsNonAntiS.make<TH2F>(b+"h2_GENRECO_NotMatched_KsNonAntiS_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_NotMatched_KsNonAntiS_vx_vz"] = dir_GENRECO_NotMatched_KsNonAntiS.make<TH2F>(b+"h2_GENRECO_NotMatched_KsNonAntiS_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_lxy"] = dir_GENRECO_NotMatched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsNonAntiS_lxy", "; Ks lxy(beamspot, Ks vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_vz"] =  dir_GENRECO_NotMatched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsNonAntiS_vz", "; Ks vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_dxy"] = dir_GENRECO_NotMatched_KsNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsNonAntiS_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);

     
     TFileDirectory dir_GENRECO_KsAntiS = dir_RECO.mkdir("GENRECO_KsAntiS");
     histos_th1f["h_GENRECO_KsAntiS_deltaR"] = dir_GENRECO_KsAntiS.make<TH1F>(b+"h_GENRECO_KsAntiS_deltaR", "; #DeltaR(gen Ks, reco Ks); #entries ",1000,0,10);

     TFileDirectory dir_GENRECO_Matched_KsAntiS = dir_GENRECO_KsAntiS.mkdir("GENRECO_Matched_KsAntiS");
     histos_th1f["h_GENRECO_Matched_KsAntiS_pt"] = dir_GENRECO_Matched_KsAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsAntiS_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_Matched_KsAntiS_eta"] = dir_GENRECO_Matched_KsAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsAntiS_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_Matched_KsAntiS_phi"] = dir_GENRECO_Matched_KsAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsAntiS_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_Matched_KsAntiS_vx_vy"] = dir_GENRECO_Matched_KsAntiS.make<TH2F>(b+"h2_GENRECO_Matched_KsAntiS_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_Matched_KsAntiS_vx_vz"] = dir_GENRECO_Matched_KsAntiS.make<TH2F>(b+"h2_GENRECO_Matched_KsAntiS_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_Matched_KsAntiS_lxy"] = dir_GENRECO_Matched_KsAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsAntiS_lxy", "; Ks lxy(beamspot, Ks vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_Matched_KsAntiS_vz"] =  dir_GENRECO_Matched_KsAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsAntiS_vz", "; Ks vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_Matched_KsAntiS_dxy"] = dir_GENRECO_Matched_KsAntiS.make<TH1F>(b+"h_GENRECO_Matched_KsAntiS_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_NotMatched_KsAntiS = dir_GENRECO_KsAntiS.mkdir("GENRECO_NotMatched_KsAntiS");
     histos_th1f["h_GENRECO_NotMatched_KsAntiS_pt"] = dir_GENRECO_NotMatched_KsAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsAntiS_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_NotMatched_KsAntiS_eta"] = dir_GENRECO_NotMatched_KsAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsAntiS_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_NotMatched_KsAntiS_phi"] = dir_GENRECO_NotMatched_KsAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsAntiS_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_NotMatched_KsAntiS_vx_vy"] = dir_GENRECO_NotMatched_KsAntiS.make<TH2F>(b+"h2_GENRECO_NotMatched_KsAntiS_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_NotMatched_KsAntiS_vx_vz"] = dir_GENRECO_NotMatched_KsAntiS.make<TH2F>(b+"h2_GENRECO_NotMatched_KsAntiS_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_NotMatched_KsAntiS_lxy"] = dir_GENRECO_NotMatched_KsAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsAntiS_lxy", "; Ks lxy(beamspot, Ks vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_NotMatched_KsAntiS_vz"] =  dir_GENRECO_NotMatched_KsAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsAntiS_vz", "; Ks vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_NotMatched_KsAntiS_dxy"] = dir_GENRECO_NotMatched_KsAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_KsAntiS_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);


     TFileDirectory dir_GENRECO_AntiLambdaNonAntiS = dir_RECO.mkdir("GENRECO_AntiLambdaNonAntiS");
     histos_th1f["h_GENRECO_AntiLambdaNonAntiS_deltaR"] = dir_GENRECO_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_AntiLambdaNonAntiS_deltaR", "; #DeltaR(gen AntiLambda, reco AntiLambda); #entries ",1000,0,10);

     TFileDirectory dir_GENRECO_Matched_AntiLambdaNonAntiS = dir_GENRECO_AntiLambdaNonAntiS.mkdir("GENRECO_Matched_AntiLambdaNonAntiS");
     histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_pt"] = dir_GENRECO_Matched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaNonAntiS_pt", "; AntiLambda pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_eta"] = dir_GENRECO_Matched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaNonAntiS_eta", "; AntiLambda #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_phi"] = dir_GENRECO_Matched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaNonAntiS_phi", "; AntiLambda #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_Matched_AntiLambdaNonAntiS_vx_vy"] = dir_GENRECO_Matched_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GENRECO_Matched_AntiLambdaNonAntiS_vx_vy", "; AntiLambda vx (cm); AntiLambda vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_Matched_AntiLambdaNonAntiS_vx_vz"] = dir_GENRECO_Matched_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GENRECO_Matched_AntiLambdaNonAntiS_vx_vz", "; AntiLambda vx (cm); AntiLambda vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_lxy"] = dir_GENRECO_Matched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaNonAntiS_lxy", "; AntiLambda lxy(beamspot, AntiLambda vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_vz"] =  dir_GENRECO_Matched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaNonAntiS_vz", "; AntiLambda vz(AntiLambda vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_dxy"] = dir_GENRECO_Matched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaNonAntiS_dxy", "; AntiLambda dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_NotMatched_AntiLambdaNonAntiS = dir_GENRECO_AntiLambdaNonAntiS.mkdir("GENRECO_NotMatched_AntiLambdaNonAntiS");
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_pt"] = dir_GENRECO_NotMatched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaNonAntiS_pt", "; AntiLambda pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_eta"] = dir_GENRECO_NotMatched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaNonAntiS_eta", "; AntiLambda #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_phi"] = dir_GENRECO_NotMatched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaNonAntiS_phi", "; AntiLambda #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_NotMatched_AntiLambdaNonAntiS_vx_vy"] = dir_GENRECO_NotMatched_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GENRECO_NotMatched_AntiLambdaNonAntiS_vx_vy", "; AntiLambda vx (cm); AntiLambda vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_NotMatched_AntiLambdaNonAntiS_vx_vz"] = dir_GENRECO_NotMatched_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GENRECO_NotMatched_AntiLambdaNonAntiS_vx_vz", "; AntiLambda vx (cm); AntiLambda vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_lxy"] = dir_GENRECO_NotMatched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaNonAntiS_lxy", "; AntiLambda lxy(beamspot, AntiLambda vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_vz"] =  dir_GENRECO_NotMatched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaNonAntiS_vz", "; AntiLambda vz(AntiLambda vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_dxy"] = dir_GENRECO_NotMatched_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaNonAntiS_dxy", "; AntiLambda dxy(beamspot) (cm); #entries ",400,-20,20);
     

     TFileDirectory dir_GENRECO_AntiLambdaAntiS = dir_RECO.mkdir("GENRECO_AntiLambdaAntiS");
     histos_th1f["h_GENRECO_AntiLambdaAntiS_deltaR"] = dir_GENRECO_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_AntiLambdaAntiS_deltaR", "; #DeltaR(gen AntiLambda, reco AntiLambda); #entries ",1000,0,10);

     TFileDirectory dir_GENRECO_Matched_AntiLambdaAntiS = dir_GENRECO_AntiLambdaAntiS.mkdir("GENRECO_Matched_AntiLambdaAntiS");
     histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_pt"] = dir_GENRECO_Matched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaAntiS_pt", "; AntiLambda pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_eta"] = dir_GENRECO_Matched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaAntiS_eta", "; AntiLambda #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_phi"] = dir_GENRECO_Matched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaAntiS_phi", "; AntiLambda #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_Matched_AntiLambdaAntiS_vx_vy"] = dir_GENRECO_Matched_AntiLambdaAntiS.make<TH2F>(b+"h2_GENRECO_Matched_AntiLambdaAntiS_vx_vy", "; AntiLambda vx (cm); AntiLambda vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_Matched_AntiLambdaAntiS_vx_vz"] = dir_GENRECO_Matched_AntiLambdaAntiS.make<TH2F>(b+"h2_GENRECO_Matched_AntiLambdaAntiS_vx_vz", "; AntiLambda vx (cm); AntiLambda vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_lxy"] = dir_GENRECO_Matched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaAntiS_lxy", "; AntiLambda lxy(beamspot, AntiLambda vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_vz"] =  dir_GENRECO_Matched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaAntiS_vz", "; AntiLambda vz(AntiLambda vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_dxy"] = dir_GENRECO_Matched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_Matched_AntiLambdaAntiS_dxy", "; AntiLambda dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_NotMatched_AntiLambdaAntiS = dir_GENRECO_AntiLambdaAntiS.mkdir("GENRECO_NotMatched_AntiLambdaAntiS");
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_pt"] = dir_GENRECO_NotMatched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaAntiS_pt", "; AntiLambda pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_eta"] = dir_GENRECO_NotMatched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaAntiS_eta", "; AntiLambda #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_phi"] = dir_GENRECO_NotMatched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaAntiS_phi", "; AntiLambda #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_NotMatched_AntiLambdaAntiS_vx_vy"] = dir_GENRECO_NotMatched_AntiLambdaAntiS.make<TH2F>(b+"h2_GENRECO_NotMatched_AntiLambdaAntiS_vx_vy", "; AntiLambda vx (cm); AntiLambda vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_NotMatched_AntiLambdaAntiS_vx_vz"] = dir_GENRECO_NotMatched_AntiLambdaAntiS.make<TH2F>(b+"h2_GENRECO_NotMatched_AntiLambdaAntiS_vx_vz", "; AntiLambda vx (cm); AntiLambda vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_lxy"] = dir_GENRECO_NotMatched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaAntiS_lxy", "; AntiLambda lxy(beamspot, AntiLambda vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_vz"] =  dir_GENRECO_NotMatched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaAntiS_vz", "; AntiLambda vz(AntiLambda vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_dxy"] = dir_GENRECO_NotMatched_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_NotMatched_AntiLambdaAntiS_dxy", "; AntiLambda dxy(beamspot) (cm); #entries ",400,-20,20);
     
     

}

void AnalyzerAllSteps::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup) {

 
  //beamspot
  edm::Handle<reco::BeamSpot> h_bs;
  //iEvent.getByToken(m_bsToken, h_bs);

  //primary vertex
  edm::Handle<vector<reco::Vertex>> h_offlinePV;
  iEvent.getByToken(m_offlinePVToken, h_offlinePV);

  //SIM particles: normal Gen particles or PlusGEANT
  edm::Handle<vector<reco::GenParticle>> h_genParticles;
  //iEvent.getByToken(m_genParticlesToken_GEN, h_genParticles);
  iEvent.getByToken(m_genParticlesToken_SIM_GEANT, h_genParticles);

  //General tracks particles
  //edm::Handle<vector<reco::Track>> h_generalTracks;
  edm::Handle<View<reco::Track>> h_generalTracks;
  iEvent.getByToken(m_generalTracksToken, h_generalTracks);

  //lambdaKshortVertexFilter sexaquark candidates
  edm::Handle<vector<reco::VertexCompositeCandidate> > h_sCands;
  iEvent.getByToken(m_sCandsToken, h_sCands);

  //V0 Kshorts
  edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks;
  iEvent.getByToken(m_V0KsToken, h_V0Ks);

  //V0 Lambdas
  edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L;
  iEvent.getByToken(m_V0LToken, h_V0L);

  //trackingparticle collection
  edm::Handle<TrackingParticleCollection>  h_TP ;
  iEvent.getByToken(m_TPToken,h_TP);
  TrackingParticleCollection const & TPColl = *(h_TP.product());
  //track associator on hits
  edm::Handle< reco::TrackToTrackingParticleAssociator>  h_trackAssociator;
  iEvent.getByToken(m_trackAssociatorToken, h_trackAssociator);


  //beamspot
  TVector3 beamspot(0,0,0);
  TVector3 beamspotVariance(0,0,0);
  if(h_bs.isValid()){  
	beamspot.SetXYZ(h_bs->x0(),h_bs->y0(),h_bs->z0());
	beamspotVariance.SetXYZ(pow(h_bs->x0Error(),2),pow(h_bs->y0Error(),2),pow(h_bs->z0Error(),2));			
  }

  TVector3 FirstOfflinePV(0.,0.,0.);
  if(h_offlinePV.isValid()){ FirstOfflinePV.SetX(h_offlinePV->at(0).x()); FirstOfflinePV.SetY(h_offlinePV->at(0).y()); FirstOfflinePV.SetZ(h_offlinePV->at(0).z());}


  //evaluate tracking performance
  if(h_generalTracks.isValid() && h_TP.isValid() && h_trackAssociator.isValid()){


	for(size_t i=0; i<TPColl.size(); ++i) {
	  //first investigate whether this tp is a dauhgter of an AntiS. You need this because these granddaughter tracks you want to investigate separately from the rest of the tracks. 
	  const TrackingParticle& tp = TPColl[i];
	  bool tpIsGrandDaughterAntiS = isTpGrandDaughterAntiS(TPColl, tp);
	  //std::cout << "---------------------------" << std::endl;
	  //std::cout << "tp pdgId: " << tp.pdgId() << std::endl;
	  //std::cout << "tp is a granddaughter of an antiS " << tpIsGrandDaughterAntiS << std::endl;
	  bool matchingTrackFound = false;
	  //const reco::Track *matchedTrackPointer = nullptr;
	  TrackingParticleRef tpr(h_TP,i);
	  edm::Handle<reco::TrackToTrackingParticleAssociator> theAssociator;
	  reco::SimToRecoCollection simRecCollL;
	  reco::SimToRecoCollection const * simRecCollP=nullptr;
	  simRecCollL = std::move(h_trackAssociator->associateSimToReco(h_generalTracks,h_TP));
	  simRecCollP = &simRecCollL;
          reco::SimToRecoCollection const & simRecColl = *simRecCollP;

	  if(simRecColl.find(tpr) != simRecColl.end()){
		  auto const & rt = simRecColl[tpr];
		  if (rt.size()!=0) {
		    // isRecoMatched = true; // UNUSED
		    //matchedTrackPointer = rt.begin()->first.get();
		    matchingTrackFound = true;
	  	    //std::cout << "Track matching result: " << matchingTrackFound << " ,for a GEN track with a pt of " << tpr->pt() << " and a RECO matched track with a pt of " << matchedTrackPointer->pt() << std::endl;	
		  }
	  }else{
		    //std::cout << "no matching track found" << std::endl;
		    matchingTrackFound = false;
	  }
        
	
	  //now fill some catagory of plots: set of histograms for all charged tp (except for the granddaughters from the antiS) which got reconstructed and a set of histograms for all tp (excpet granddaughters from the antiS) --> get eff easily by dividing histos
	  if(!tpIsGrandDaughterAntiS && tp.charge()!=0){
		if(matchingTrackFound)FillHistosNonAntiSTracksRECO(tp, beamspot);
		FillHistosNonAntiSTracksAll(tp , beamspot);
       	  }
	  //now in the above two catagories you are missing all the tracks from the antiS granddaughters, so when you encounter an antiS you should still have to fill four it's 4 potential granddaughters the histograms and also one where you count how many antiS have four granddaughters reconstructed
	  
	  if(tp.pdgId() == pdgIdAntiS)FillHistosAntiSTracks(tp, beamspot, TPColl,  h_TP, h_trackAssociator, h_generalTracks, h_V0Ks, h_V0L);
	}
  }

  int nAntiSThisEvent = 0;
  int nAntiSInteractThisEvent = 0;
  int nKsThisEvent = 0;
  int nAntiLambdaThisEvent = 0;
  if(h_genParticles.isValid()){
      for(unsigned int i = 0; i < h_genParticles->size(); ++i){//loop all genparticlesPlusGEANT
		const reco::Candidate * genParticle = &h_genParticles->at(i);
		//all antiS
		bool genParticleIsAntiS = false;
		if(genParticle->pdgId() == pdgIdAntiS) {genParticleIsAntiS = true;}
		if(genParticleIsAntiS){nAntiSThisEvent++; FillHistosGENAntiS(genParticle, beamspot);}

		//all Ks
		bool genParticleIsKs = false;	
		bool genParticleMotherIsAntiS = false;//Ks as daughter of an antiS	
		if(abs(genParticle->pdgId()) == pdgIdKs)genParticleIsKs =true;
		if(genParticle->numberOfMothers()==1){if(genParticle->mother(0)->pdgId()==pdgIdAntiS){genParticleMotherIsAntiS=true;}};
		if(genParticleIsKs && !genParticleMotherIsAntiS){nKsThisEvent++; FillHistosGENKsNonAntiS(genParticle,beamspot);};
		if(genParticleIsKs && genParticleMotherIsAntiS)FillHistosGENKsAntiS(genParticle,beamspot);

		//all antiLambda
		bool genParticleIsAntiLambda = false;
		if(genParticle->pdgId() == pdgIdAntiLambda)genParticleIsAntiLambda=true;
		if(genParticleIsAntiLambda && !genParticleMotherIsAntiS){nAntiLambdaThisEvent++;FillHistosGENAntiLambdaNonAntiS(genParticle,beamspot);};
		if(genParticleIsAntiLambda && genParticleMotherIsAntiS)FillHistosGENAntiLambdaAntiS(genParticle,beamspot);

		//check the granddaughters of the AntiS
		if(genParticleIsAntiS && genParticle->numberOfDaughters()==2){nAntiSInteractThisEvent++;FillHistoAllGranddaughtersCorrect(genParticle,h_V0Ks,h_V0L,iEvent.id().event());}

		//now start looking at the RECO efficiencies of daughters and antiS, you can do this based on deltaR, becaused these particles are not charged
		if(genParticleIsKs && !genParticleMotherIsAntiS)RecoEvaluationKsNonAntiS(genParticle,h_V0Ks,beamspot);
		if(genParticleIsKs && genParticleMotherIsAntiS)RecoEvaluationKsAntiS(genParticle,h_V0Ks,beamspot);

		if(genParticleIsAntiLambda && !genParticleMotherIsAntiS)RecoEvaluationAntiLambdaNonAntiS(genParticle,h_V0L,beamspot);
		if(genParticleIsAntiLambda && genParticleMotherIsAntiS)RecoEvaluationAntiLambdaAntiS(genParticle,h_V0L,beamspot);

		//if(genParticleIsAntiS) RecoEvaluationAntiS(genParticle,h_sCands,beamspot);
		
      }//for(unsigned int i = 0; i < h_genParticles->size(); ++i)
  }//if(h_genParticles.isValid())
  histos_th1f["h_GEN_nAntiS"]->Fill(nAntiSThisEvent);
  histos_th1f["h_GEN_nAntiSInteract"]->Fill(nAntiSInteractThisEvent);
  histos_th1f["h_GEN_nKs"]->Fill(nKsThisEvent);
  histos_th1f["h_GEN_nAntiLambda"]->Fill(nAntiLambdaThisEvent);



  //loop over the RECO Ks to plot the kinematics
  if(h_V0Ks.isValid()){
      for(unsigned int i = 0; i < h_V0Ks->size(); ++i){//loop all Ks candidates
	const reco::VertexCompositeCandidate * Ks = &h_V0Ks->at(i);	
	FillHistosRECOKs(Ks,beamspot,h_genParticles);
      }
  }

  //loop over the RECO AntiLambda to plot the kinematics
  if(h_V0L.isValid()){
      for(unsigned int i = 0; i < h_V0L->size(); ++i){//loop all Ks candidates
	const reco::VertexCompositeCandidate * L = &h_V0L->at(i);	
	FillHistosRECOLambda(L,beamspot,h_genParticles);
      }
  }

  //loop over the RECO AntiS to plot the kinematics
  if(h_sCands.isValid()){
      for(unsigned int i = 0; i < h_sCands->size(); ++i){//loop all Ks candidates
	const reco::VertexCompositeCandidate * antiS = &h_sCands->at(i);	
	FillHistosRECOAntiS(antiS, beamspot, beamspotVariance, h_genParticles, iEvent.id().event());
      }
  }



 } //end of analyzer

bool AnalyzerAllSteps::isTpGrandDaughterAntiS(TrackingParticleCollection const & TPColl, const TrackingParticle& tp){
 bool tpIsGrandDaughterAntiS = false;
 if(abs(tp.pdgId()) == 211 || tp.pdgId() == - 2212){//found a charged pion or antiproton

	double granddaughterVx = tp.vx();double granddaughterVy = tp.vy();double granddaughterVz = tp.vz();

	for(size_t j=0; j<TPColl.size(); ++j) {//now find the daughters which have a decay vertex = the production vertex of the granddaughters
		if(tpIsGrandDaughterAntiS)continue;//can skip it was already found in a previous loop
     		const TrackingParticle& tp_daughter = TPColl[j];

		if(abs(tp_daughter.pdgId()) == 310 || tp_daughter.pdgId() == -3122){//daughter has to be a Ks or Lambda

			tv_iterator tp_daughter_firstDecayVertex = tp_daughter.decayVertices_begin();
			double daughterdecayVx = (**tp_daughter_firstDecayVertex).position().X(); double daughterdecayVy = (**tp_daughter_firstDecayVertex).position().Y(); double daughterdecayVz = (**tp_daughter_firstDecayVertex).position().Z();	

		 	if(granddaughterVx == daughterdecayVx && granddaughterVy == daughterdecayVy && granddaughterVz == daughterdecayVz){//daughter decay vertex has to match the granddaughter creation vertex
				for(size_t k=0; k<TPColl.size(); ++k) {//loop to find the antiS
					if(tpIsGrandDaughterAntiS)continue;//can skip it was already found in a previous loop
					const TrackingParticle& tp_S = TPColl[k];
					if(tp_S.pdgId() == -1020000020){//found the S
						tv_iterator tp_S_firstDecayVertex = tp_S.decayVertices_begin();
						double SdecayVx = (**tp_S_firstDecayVertex).position().X(); double SdecayVy = (**tp_S_firstDecayVertex).position().Y();double SdecayVz = (**tp_S_firstDecayVertex).position().Z();
						if(tp_daughter.vx() == SdecayVx && tp_daughter.vy() == SdecayVy && tp_daughter.vz() == SdecayVz){
							tpIsGrandDaughterAntiS = true;
						}
					}//end if antiS
				}//end loop over the tp to find the antiS
			}//end check if granddaughter vertex matches the the daughter decay vertex	
		}//end check for pdgId daughter
	}//end loop over tp to find daughters
   }//end check of granddaughter pdgId
 
 return tpIsGrandDaughterAntiS;

}


void AnalyzerAllSteps::FillHistosNonAntiSTracksRECO(const TrackingParticle& tp, TVector3 beamspot){
	TVector3 tpCreationVertex(tp.vx(),tp.vy(),tp.vz());
	double Lxy = lxy(beamspot,tpCreationVertex);
	TVector3 tpMomentum(tp.px(),tp.py(),tp.pz());
	double dxy = dxy_signed_line_point(tpCreationVertex,tpMomentum,beamspot);
	histos_th1f["h_NonAntiSTrack_RECO_pt"]->Fill(tp.pt());	
	histos_th1f["h_NonAntiSTrack_RECO_eta"]->Fill(tp.eta());	
	histos_th1f["h_NonAntiSTrack_RECO_phi"]->Fill(tp.phi());	
	histos_th2f["h2_NonAntiSTrack_RECO_vx_vy"]->Fill(tp.vx(),tp.vy());
	histos_th2f["h2_NonAntiSTrack_RECO_vx_vz"]->Fill(tp.vx(),tp.vz());
	histos_th1f["h_NonAntiSTrack_RECO_lxy"]->Fill(Lxy);	
	histos_th1f["h_NonAntiSTrack_RECO_vz"]->Fill(tp.vz());	
	histos_th1f["h_NonAntiSTrack_RECO_dxy"]->Fill(dxy);

	if(abs(tp.eta()) < 2.5 && Lxy < 3.5) histos_th1f["h_NonAntiSTrack_RECO_pt_cut_eta_Lxy"]->Fill(tp.pt());
	if(tp.pt() > 0.9 && abs(tp.eta()) < 2.5) histos_th1f["h_NonAntiSTrack_RECO_lxy_cut_pt_eta"]->Fill(Lxy);
	if(tp.pt() > 0.9 && Lxy < 3.5) histos_th1f["h_NonAntiSTrack_RECO_eta_cut_pt_lxy"]->Fill(tp.eta());
}

void AnalyzerAllSteps::FillHistosNonAntiSTracksAll(const TrackingParticle& tp, TVector3 beamspot){
	TVector3 tpCreationVertex(tp.vx(),tp.vy(),tp.vz());
	double Lxy = lxy(beamspot,tpCreationVertex);
	TVector3 tpMomentum(tp.px(),tp.py(),tp.pz());
	double dxy = dxy_signed_line_point(tpCreationVertex,tpMomentum,beamspot);
	histos_th1f["h_NonAntiSTrack_All_pt"]->Fill(tp.pt());	
	histos_th1f["h_NonAntiSTrack_All_eta"]->Fill(tp.eta());	
	histos_th1f["h_NonAntiSTrack_All_phi"]->Fill(tp.phi());	
	histos_th2f["h2_NonAntiSTrack_All_vx_vy"]->Fill(tp.vx(),tp.vy());
	histos_th2f["h2_NonAntiSTrack_All_vx_vz"]->Fill(tp.vx(),tp.vz());
	histos_th1f["h_NonAntiSTrack_All_lxy"]->Fill(Lxy);	
	histos_th1f["h_NonAntiSTrack_All_vz"]->Fill(tp.vz());	
	histos_th1f["h_NonAntiSTrack_All_dxy"]->Fill(dxy);

	if(abs(tp.eta()) < 2.5 && Lxy < 3.5) histos_th1f["h_NonAntiSTrack_All_pt_cut_eta_Lxy"]->Fill(tp.pt());
	if(tp.pt() > 0.9 && abs(tp.eta()) < 2.5) histos_th1f["h_NonAntiSTrack_All_lxy_cut_pt_eta"]->Fill(Lxy);
	if(tp.pt() > 0.9 && Lxy < 3.5) histos_th1f["h_NonAntiSTrack_All_eta_cut_pt_lxy"]->Fill(tp.eta());
	

}

void AnalyzerAllSteps::FillHistosAntiSTracks(const TrackingParticle& tp, TVector3 beamspot, TrackingParticleCollection const & TPColl, edm::Handle<TrackingParticleCollection> h_TP, edm::Handle< reco::TrackToTrackingParticleAssociator> h_trackAssociator, edm::Handle<View<reco::Track>> h_generalTracks, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L){
	//now start from this tp and go down in gen particles to check if you find daughter and granddaughters
	vector<bool> granddaughterTrackMatched;
	granddaughterTrackMatched.push_back(false);granddaughterTrackMatched.push_back(false);granddaughterTrackMatched.push_back(false);granddaughterTrackMatched.push_back(false);
	const reco::Track *matchedTrackPointerKsPosPion = nullptr;
	const reco::Track *matchedTrackPointerKsNegPion = nullptr;
	const reco::Track *matchedTrackPointerAntiLPosPion = nullptr;
	const reco::Track *matchedTrackPointerAntiLNegProton = nullptr;
	
	//get antiS decay vertex
	tv_iterator tp_firstDecayVertex = tp.decayVertices_begin();
	double tpdecayVx = (**tp_firstDecayVertex).position().X(); double tpdecayVy = (**tp_firstDecayVertex).position().Y(); double tpdecayVz = (**tp_firstDecayVertex).position().Z();
	std::cout << "-----------------" << std::endl;
	std::cout << "1. found an antiS" << std::endl;
	//now loop over the TP and try to find daughters
	for(size_t j=0; j<TPColl.size(); ++j) {
		const TrackingParticle& tp_daughter = TPColl[j];
		if(abs(tp_daughter.pdgId()) == 310 || tp_daughter.pdgId() == -3122){//daughter has to be a Ks or Lambda
			
			double daughterVx = tp_daughter.vx(); double daughterVy = tp_daughter.vy(); double daughterVz = tp_daughter.vz();	
			tv_iterator tp_daughter_firstDecayVertex = tp_daughter.decayVertices_begin();
			double tp_daughter_decayVx = (**tp_daughter_firstDecayVertex).position().X(); double tp_daughter_decayVy = (**tp_daughter_firstDecayVertex).position().Y(); double tp_daughter_decayVz = (**tp_daughter_firstDecayVertex).position().Z();

			if( daughterVx == tpdecayVx && daughterVy == tpdecayVy && daughterVz == tpdecayVz){//daughter vertex has to match the mother decay vertex
				std::cout << "2. found daughter of the antiS" << std::endl;
				for(size_t k=0; k<TPColl.size(); ++k) {//loop to find the granddaughters
					const TrackingParticle& tp_granddaughter = TPColl[k];

					if(tp_granddaughter.vx() == tp_daughter_decayVx && tp_granddaughter.vy() == tp_daughter_decayVy && tp_granddaughter.vz() == tp_daughter_decayVz){
						if(tp_granddaughter.pdgId()==pdgIdAntiProton || tp_granddaughter.pdgId()==pdgIdPosPion ||tp_granddaughter.pdgId()==pdgIdNegPion ){//found a tp that is a granddaughter of the antiS
							 std::cout << "3. found granddaughter of the antiS" << std::endl;
							  //now check if you have matching track to this granddaughter
													
							  const reco::Track *matchedTrackPointer = nullptr;
							  TrackingParticleRef tpr(h_TP,k);
							  edm::Handle<reco::TrackToTrackingParticleAssociator> theAssociator;
							  reco::SimToRecoCollection simRecCollL;
							  reco::SimToRecoCollection const * simRecCollP=nullptr;
							  simRecCollL = std::move(h_trackAssociator->associateSimToReco(h_generalTracks,h_TP));
							  simRecCollP = &simRecCollL;
							  reco::SimToRecoCollection const & simRecColl = *simRecCollP;

							  if(simRecColl.find(tpr) != simRecColl.end()){
								  auto const & rt = simRecColl[tpr];
								  if (rt.size()!=0) {
									    
									    std::cout << "4. found matching track" << std::endl;
									    matchedTrackPointer = rt.begin()->first.get();
									    //matchingTrackFound = true;
									    if(abs(tp_daughter.pdgId()) == 310 && tp_granddaughter.pdgId()==pdgIdPosPion){granddaughterTrackMatched[0]=true;matchedTrackPointerKsPosPion= matchedTrackPointer; FillHistosAntiSKsDaughterTracksRECO(tp_granddaughter,beamspot);}
									    else if(abs(tp_daughter.pdgId()) == 310 && tp_granddaughter.pdgId()==pdgIdNegPion){granddaughterTrackMatched[1]=true;matchedTrackPointerKsNegPion=matchedTrackPointer;FillHistosAntiSKsDaughterTracksAll(tp_granddaughter,beamspot);}
									    else if(tp_daughter.pdgId() == -3122 && tp_granddaughter.pdgId()==pdgIdAntiProton){granddaughterTrackMatched[2]=true;matchedTrackPointerAntiLPosPion=matchedTrackPointer;}
									    else if(tp_daughter.pdgId() == -3122 && tp_granddaughter.pdgId()==pdgIdPosPion){granddaughterTrackMatched[3]=true;matchedTrackPointerAntiLNegProton=matchedTrackPointer;}
								  }
							  }
							  /*}else{
								    	    matchingTrackFound = false;
							  }*/
						}
					}
				}
			}	
		}	
    	}
  std::cout << "Ks, pi+ " << granddaughterTrackMatched[0] << std::endl; 
  std::cout << "Ks, pi- " << granddaughterTrackMatched[1] << std::endl; 
  std::cout << " L, p-  " << granddaughterTrackMatched[2] << std::endl; 
  std::cout << " L, pi+ " << granddaughterTrackMatched[3] << std::endl; 
  
  FillMajorEfficiencyPlot(granddaughterTrackMatched, matchedTrackPointerKsPosPion,matchedTrackPointerKsNegPion,matchedTrackPointerAntiLPosPion,matchedTrackPointerAntiLNegProton,h_V0Ks,h_V0L);

}

void AnalyzerAllSteps::FillHistosAntiSKsDaughterTracksRECO(const TrackingParticle& tp, TVector3 beamspot){
	TVector3 tpCreationVertex(tp.vx(),tp.vy(),tp.vz());
	double Lxy = lxy(beamspot,tpCreationVertex);
	TVector3 tpMomentum(tp.px(),tp.py(),tp.pz());
	double dxy = dxy_signed_line_point(tpCreationVertex,tpMomentum,beamspot);
	histos_th1f["h_AntiSKsDaughterTracks_RECO_pt"]->Fill(tp.pt());	
	histos_th1f["h_AntiSKsDaughterTracks_RECO_eta"]->Fill(tp.eta());	
	histos_th1f["h_AntiSKsDaughterTracks_RECO_phi"]->Fill(tp.phi());	
	histos_th2f["h2_AntiSKsDaughterTracks_RECO_vx_vy"]->Fill(tp.vx(),tp.vy());
	histos_th2f["h2_AntiSKsDaughterTracks_RECO_vx_vz"]->Fill(tp.vx(),tp.vz());
	histos_th1f["h_AntiSKsDaughterTracks_RECO_lxy"]->Fill(Lxy);	
	histos_th1f["h_AntiSKsDaughterTracks_RECO_vz"]->Fill(tp.vz());	
	histos_th1f["h_AntiSKsDaughterTracks_RECO_dxy"]->Fill(dxy);
	
}
void AnalyzerAllSteps::FillHistosAntiSKsDaughterTracksAll(const TrackingParticle& tp, TVector3 beamspot){
	TVector3 tpCreationVertex(tp.vx(),tp.vy(),tp.vz());
	double Lxy = lxy(beamspot,tpCreationVertex);
	TVector3 tpMomentum(tp.px(),tp.py(),tp.pz());
	double dxy = dxy_signed_line_point(tpCreationVertex,tpMomentum,beamspot);
	histos_th1f["h_AntiSKsDaughterTracks_All_pt"]->Fill(tp.pt());	
	histos_th1f["h_AntiSKsDaughterTracks_All_eta"]->Fill(tp.eta());	
	histos_th1f["h_AntiSKsDaughterTracks_All_phi"]->Fill(tp.phi());	
	histos_th2f["h2_AntiSKsDaughterTracks_All_vx_vy"]->Fill(tp.vx(),tp.vy());
	histos_th2f["h2_AntiSKsDaughterTracks_All_vx_vz"]->Fill(tp.vx(),tp.vz());
	histos_th1f["h_AntiSKsDaughterTracks_All_lxy"]->Fill(Lxy);	
	histos_th1f["h_AntiSKsDaughterTracks_All_vz"]->Fill(tp.vz());	
	histos_th1f["h_AntiSKsDaughterTracks_All_dxy"]->Fill(dxy);
	
}
void AnalyzerAllSteps::FillMajorEfficiencyPlot(std::vector<bool>granddaughterTrackMatched, const reco::Track *matchedTrackPointerKsPosPion,const reco::Track *matchedTrackPointerKsNegPion,const reco::Track *matchedTrackPointerAntiLPosPion,const reco::Track *matchedTrackPointerAntiLNegProton, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L){
	//if the Ks daughters were found calculate the sum of the daughters momenta and try to find a reco Ks which matches this in deltaR
	bool matchingV0KsFound = false;
	if(matchedTrackPointerKsPosPion && matchedTrackPointerKsNegPion){
		  math::XYZVector sumV0Momenta = matchedTrackPointerKsPosPion->momentum() + matchedTrackPointerKsNegPion->momentum();
		  double deltaRmin = 999;
		  if(h_V0Ks.isValid()){
		      for(unsigned int i = 0; i < h_V0Ks->size(); ++i){//loop all Ks candidates
			const reco::VertexCompositeCandidate * V0 = &h_V0Ks->at(i);	
			double deltaPhi = reco::deltaPhi(sumV0Momenta.phi(),V0->phi());
			double deltaEta = sumV0Momenta.eta()-V0->eta();
			double deltaR = sqrt(deltaPhi*deltaPhi+deltaEta*deltaEta);
			histos_th1f["h_deltaR_V0Ks_momentumSumKsDaughterTracks"]->Fill(deltaR);
			if(deltaR < deltaRmin) deltaR = deltaRmin;
		      }
		      matchingV0KsFound = true;
		  }
	}

	//if the AntiL daughters were found calculate the sum of the daughters momenta and try to find a reco AntiL which matches this in deltaR
	bool matchingV0AntiLFound = false;
	if(matchedTrackPointerAntiLPosPion && matchedTrackPointerAntiLNegProton){
		  math::XYZVector sumV0Momenta = matchedTrackPointerAntiLPosPion->momentum() + matchedTrackPointerAntiLNegProton->momentum();
		  double deltaRmin = 999;
		  if(h_V0L.isValid()){
		      for(unsigned int i = 0; i < h_V0L->size(); ++i){//loop all Ks candidates
			const reco::VertexCompositeCandidate * V0 = &h_V0L->at(i);	
			double deltaPhi = reco::deltaPhi(sumV0Momenta.phi(),V0->phi());
			double deltaEta = sumV0Momenta.eta()-V0->eta();
			double deltaR = sqrt(deltaPhi*deltaPhi+deltaEta*deltaEta);
			histos_th1f["h_deltaR_V0AntiL_momentumSumAntiLDaughterTracks"]->Fill(deltaR);
			if(deltaR < deltaRmin) deltaR = deltaRmin;
		      }
		      matchingV0AntiLFound = true;
		  }
	}

	if(!matchingV0KsFound && !matchingV0AntiLFound){//fill the lower 4*4 part of the matrix
		if(!granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,0.);
		else if(granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,1.);
		else if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,2.);
		else if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,3.);

		else if(!granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,0.);
		else if(granddaughterTrackMatched[0]  && !granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,1.);
		else if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,2.);
		else if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,3.);

		else if(!granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,0.);
		else if(granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,1.);
		else if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,2.);
		else if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,3.);

		else if(!granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,0.);
		else if(granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,1.);
		else if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,2.);
		else if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,3.);
	}
	else if(matchingV0KsFound && !matchingV0AntiLFound){//fill the top row of the matrix
		if( !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,4.);
		else if(granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,4.);
		else if(!granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,4.);
		else if(granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,4.);
	}
	else if(!matchingV0KsFound && matchingV0AntiLFound){//fill the right column of the matrix
		if( !granddaughterTrackMatched[0] && !granddaughterTrackMatched[1])histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,0.);
		else if(granddaughterTrackMatched[0] && !granddaughterTrackMatched[1])histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,1.);
		else if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1])histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,2.);
		else if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1])histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,3.);

	}
	else{//this means you found both a RECO V0-Ks and RECO V0-AntiL
		histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,4.);
	}

	

}

void AnalyzerAllSteps::FillHistosGENAntiS(const reco::Candidate  * GENantiS, TVector3 beamspot){

	TVector3 momentumGENantiS(GENantiS->px(),GENantiS->py(),GENantiS->pz());
	histos_th1f["h_GEN_nAntiSTotal"]->Fill(0);	
	histos_th1f["h_GEN_AntiS_pt"]->Fill(GENantiS->pt());	
	histos_th1f["h_GEN_AntiS_eta"]->Fill(GENantiS->eta());	
	histos_th1f["h_GEN_AntiS_phi"]->Fill(GENantiS->phi());
		
	if(GENantiS->numberOfDaughters()==2){
		TVector3 momentumKs(GENantiS->daughter(0)->px(),GENantiS->daughter(0)->py(),GENantiS->daughter(0)->pz());
		TVector3 momentumAntiLambda(GENantiS->daughter(1)->px(),GENantiS->daughter(1)->py(),GENantiS->daughter(1)->pz());
		if(abs(GENantiS->daughter(1)->pdgId())==pdgIdKs){momentumKs.SetX(GENantiS->daughter(1)->px());momentumKs.SetY(GENantiS->daughter(1)->py());momentumKs.SetZ(GENantiS->daughter(1)->pz());}
		if(GENantiS->daughter(0)->pdgId()==pdgIdAntiLambda){momentumAntiLambda.SetX(GENantiS->daughter(0)->px());momentumAntiLambda.SetY(GENantiS->daughter(0)->py());momentumAntiLambda.SetZ(GENantiS->daughter(0)->pz());}

		double deltaPhiDaughters = reco::deltaPhi(GENantiS->daughter(0)->phi(),GENantiS->daughter(1)->phi());
		double deltaEtaDaughters = GENantiS->daughter(0)->eta()-GENantiS->daughter(1)->eta();
		double deltaRDaughters = sqrt(deltaPhiDaughters*deltaPhiDaughters+deltaEtaDaughters*deltaEtaDaughters);
		TVector3 interactionVertexAntiS(GENantiS->daughter(0)->vx(),GENantiS->daughter(0)->vy(),GENantiS->daughter(0)->vz());
		double lxyInteractionVertexAntiS = lxy(beamspot, interactionVertexAntiS);
		histos_th1f["h_GEN_AntiS_deltaR_daughters"]->Fill(deltaRDaughters);
		histos_th1f["h_GEN_AntiS_deltaPhi_daughters"]->Fill(deltaPhiDaughters);
		histos_th2f["h2_GEN_AntiS_deltaPhi_daughters_lxy_interactionVertex"]->Fill(deltaPhiDaughters,lxyInteractionVertexAntiS);
		histos_th1f["h_GEN_AntiS_AnglePAntiSPKs"]->Fill(TMath::ACos(CosOpeningsAngle(momentumKs,momentumGENantiS)));
		histos_th2f["h2_GEN_AntiS_PAntiS_AnglePAntiSPKs"]->Fill(GENantiS->p(),TMath::ACos(CosOpeningsAngle(momentumKs,momentumGENantiS)));
		histos_th1f["h_GEN_AntiS_AnglePAntiSPAntiLambda"]->Fill(TMath::ACos(CosOpeningsAngle(momentumAntiLambda,momentumGENantiS)));
		histos_th2f["h2_GEN_AntiS_PAntiS_AnglePAntiSPAntiLambda"]->Fill(GENantiS->p(),TMath::ACos(CosOpeningsAngle(momentumAntiLambda,momentumGENantiS)));
		histos_th2f["h2_GEN_AntiS_PointingAngleKs_PointingAngleAntiLambda"]->Fill(XYpointingAngle(GENantiS->daughter(0),beamspot),XYpointingAngle(GENantiS->daughter(1),beamspot));
	   	
	}	

}


void AnalyzerAllSteps::FillHistosGENKsNonAntiS(const reco::Candidate  * GENKsNonAntiS, TVector3 beamspot){
	TVector3 KsCreationVertex(GENKsNonAntiS->vx(),GENKsNonAntiS->vy(),GENKsNonAntiS->vz());
	
	double Lxy = lxy(beamspot,KsCreationVertex);
	TVector3 KsMomentum(GENKsNonAntiS->px(),GENKsNonAntiS->py(),GENKsNonAntiS->pz());
	double dxy = dxy_signed_line_point(KsCreationVertex,KsMomentum,beamspot);
	double xypointingAngle = XYpointingAngle(GENKsNonAntiS,beamspot);
	histos_th1f["h_GEN_KsNonAntiS_pt"]->Fill(GENKsNonAntiS->pt());	
	histos_th1f["h_GEN_KsNonAntiS_eta"]->Fill(GENKsNonAntiS->eta());	
	histos_th1f["h_GEN_KsNonAntiS_phi"]->Fill(GENKsNonAntiS->phi());	
	histos_th2f["h2_GEN_KsNonAntiS_vx_vy"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vy());
	histos_th2f["h2_GEN_KsNonAntiS_vx_vz"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vz());
	histos_th1f["h_GEN_KsNonAntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_GEN_KsNonAntiS_vz"]->Fill(GENKsNonAntiS->vz());	
	histos_th1f["h_GEN_KsNonAntiS_dxy"]->Fill(dxy);	
	if(GENKsNonAntiS->numberOfDaughters() == 2)histos_th1f["h_GEN_KsNonAntiS_XYpointingAngle"]->Fill(xypointingAngle);	
}

void AnalyzerAllSteps::FillHistosGENAntiLambdaNonAntiS(const reco::Candidate  * GENAntiLambdaNonAntiS, TVector3 beamspot){
	TVector3 AntiLambdaCreationVertex(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vy(),GENAntiLambdaNonAntiS->vz());
	double Lxy = lxy(beamspot,AntiLambdaCreationVertex);
	TVector3 AntiLambdaMomentum(GENAntiLambdaNonAntiS->px(),GENAntiLambdaNonAntiS->py(),GENAntiLambdaNonAntiS->pz());
	double dxy = dxy_signed_line_point(AntiLambdaCreationVertex,AntiLambdaMomentum,beamspot);
	double xypointingAngle = XYpointingAngle(GENAntiLambdaNonAntiS,beamspot);

	histos_th1f["h_GEN_AntiLambdaNonAntiS_pt"]->Fill(GENAntiLambdaNonAntiS->pt());	
	histos_th1f["h_GEN_AntiLambdaNonAntiS_eta"]->Fill(GENAntiLambdaNonAntiS->eta());	
	histos_th1f["h_GEN_AntiLambdaNonAntiS_phi"]->Fill(GENAntiLambdaNonAntiS->phi());	
	histos_th2f["h2_GEN_AntiLambdaNonAntiS_vx_vy"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vy());
	histos_th2f["h2_GEN_AntiLambdaNonAntiS_vx_vz"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vz());
	histos_th1f["h_GEN_AntiLambdaNonAntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_GEN_AntiLambdaNonAntiS_vz"]->Fill(GENAntiLambdaNonAntiS->vz());	
	histos_th1f["h_GEN_AntiLambdaNonAntiS_dxy"]->Fill(dxy);	
	if(GENAntiLambdaNonAntiS->numberOfDaughters() == 2)histos_th1f["h_GEN_AntiLambdaNonAntiS_XYpointingAngle"]->Fill(xypointingAngle);	
}


void AnalyzerAllSteps::FillHistosGENKsAntiS(const reco::Candidate  * GENKsAntiS, TVector3 beamspot){
	histos_th1f["h_GEN_nAntiSTotal"]->Fill(1);	
	TVector3 KsAntiSCreationVertex(GENKsAntiS->vx(),GENKsAntiS->vy(),GENKsAntiS->vz());
	double Lxy = lxy(beamspot,KsAntiSCreationVertex);
	TVector3 KsAntiSMomentum(GENKsAntiS->px(),GENKsAntiS->py(),GENKsAntiS->pz());
	double dxy = dxy_signed_line_point(KsAntiSCreationVertex,KsAntiSMomentum,beamspot);
	double xypointingAngle = XYpointingAngle(GENKsAntiS,beamspot);

	histos_th1f["h_GEN_KsAntiS_pdgId"]->Fill(GENKsAntiS->pdgId());	
	histos_th1f["h_GEN_KsAntiS_pt"]->Fill(GENKsAntiS->pt());	
	histos_th1f["h_GEN_KsAntiS_eta"]->Fill(GENKsAntiS->eta());	
	histos_th1f["h_GEN_KsAntiS_phi"]->Fill(GENKsAntiS->phi());	
	histos_th2f["h2_GEN_KsAntiS_vx_vy"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vy());
	histos_th2f["h2_GEN_KsAntiS_vx_vz"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vz());
	histos_th1f["h_GEN_KsAntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_GEN_KsAntiS_vz"]->Fill(GENKsAntiS->vz());	
	histos_th1f["h_GEN_KsAntiS_dxy"]->Fill(dxy);
	if(GENKsAntiS->numberOfDaughters() == 2)histos_th1f["h_GEN_KsAntiS_XYpointingAngle"]->Fill(xypointingAngle);
		
}

void AnalyzerAllSteps::FillHistosGENAntiLambdaAntiS(const reco::Candidate  * GENAntiLambdaAntiS, TVector3 beamspot){
	TVector3 AntiLambdaAntiSCreationVertex(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vy(),GENAntiLambdaAntiS->vz());
	double Lxy = lxy(beamspot,AntiLambdaAntiSCreationVertex);
	TVector3 AntiLambdaMomentum(GENAntiLambdaAntiS->px(),GENAntiLambdaAntiS->py(),GENAntiLambdaAntiS->pz());
	double dxy = dxy_signed_line_point(AntiLambdaAntiSCreationVertex,AntiLambdaMomentum,beamspot);
	double xypointingAngle = XYpointingAngle(GENAntiLambdaAntiS,beamspot);

	histos_th1f["h_GEN_AntiLambdaAntiS_pdgId"]->Fill(GENAntiLambdaAntiS->pdgId());	
	histos_th1f["h_GEN_AntiLambdaAntiS_pt"]->Fill(GENAntiLambdaAntiS->pt());	
	histos_th1f["h_GEN_AntiLambdaAntiS_eta"]->Fill(GENAntiLambdaAntiS->eta());	
	histos_th1f["h_GEN_AntiLambdaAntiS_phi"]->Fill(GENAntiLambdaAntiS->phi());	
	histos_th2f["h2_GEN_AntiLambdaAntiS_vx_vy"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vy());
	histos_th2f["h2_GEN_AntiLambdaAntiS_vx_vz"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vz());
	histos_th1f["h_GEN_AntiLambdaAntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_GEN_AntiLambdaAntiS_vz"]->Fill(GENAntiLambdaAntiS->vz());	
	histos_th1f["h_GEN_AntiLambdaAntiS_dxy"]->Fill(dxy);
	if(GENAntiLambdaAntiS->numberOfDaughters() == 2)histos_th1f["h_GEN_AntiLambdaAntiS_XYpointingAngle"]->Fill(xypointingAngle);
	
}

void AnalyzerAllSteps::FillHistoAllGranddaughtersCorrect(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L, int eventId){

	const reco::Candidate * KsAntiS = genParticle->daughter(0);//daughter 0 is the Ks
	const reco::Candidate * AntiLambdaAntiS = genParticle->daughter(1);//daughter 1 is the antiL

	bool KsCorrectDaughters = false;
	bool AntiLambdaCorrectDaughters = false;
	if(KsAntiS->numberOfDaughters() == 2 && AntiLambdaAntiS->numberOfDaughters() == 2){
		if( (KsAntiS->daughter(0)->pdgId() == pdgIdPosPion && KsAntiS->daughter(1)->pdgId() == pdgIdNegPion) || (KsAntiS->daughter(1)->pdgId() == pdgIdPosPion && KsAntiS->daughter(0)->pdgId() == pdgIdNegPion)) KsCorrectDaughters = true;
		if( (AntiLambdaAntiS->daughter(0)->pdgId() == pdgIdPosPion && AntiLambdaAntiS->daughter(1)->pdgId() == pdgIdAntiProton) || (AntiLambdaAntiS->daughter(1)->pdgId() == pdgIdPosPion && AntiLambdaAntiS->daughter(0)->pdgId() == pdgIdAntiProton) ) AntiLambdaCorrectDaughters = true;
		if(KsCorrectDaughters && AntiLambdaCorrectDaughters){histos_th1f["h_GEN_nAntiSTotal"]->Fill(2);}
	}

	//now check by looping over all the RECO Ks and RECO L if this antiS has both daughters RECO. You only need to do this loops if the Ks and Lambda have the proper granddaughters
	bool KsRECOMatched = false;	
	bool AntiLRECOMatched = false;	
	const reco::VertexCompositeCandidate * matchedKs = nullptr;
	const reco::VertexCompositeCandidate * matchedL = nullptr;
	if(KsCorrectDaughters && AntiLambdaCorrectDaughters){
		  //loop over the RECO Ks to check match
		  if(h_V0Ks.isValid()){
		      for(unsigned int i = 0; i < h_V0Ks->size(); ++i){//loop all Ks candidates
			const reco::VertexCompositeCandidate * Ks = &h_V0Ks->at(i);	
		      	double deltaPhi = reco::deltaPhi(Ks->phi(),KsAntiS->phi());
		      	double deltaEta = Ks->eta()-KsAntiS->eta();
			double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
			if(deltaR < 0.03){ KsRECOMatched = true;matchedKs = Ks;}
		      }
		  }

		  //loop over the RECO AntiLambda to check match
		  if(h_V0L.isValid()){
		      for(unsigned int i = 0; i < h_V0L->size(); ++i){//loop all L candidates
			const reco::VertexCompositeCandidate * L = &h_V0L->at(i);	
		      	double deltaPhi = reco::deltaPhi(L->phi(),AntiLambdaAntiS->phi());
		      	double deltaEta = L->eta()-AntiLambdaAntiS->eta();
			double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
			if(deltaR < 0.03){ AntiLRECOMatched = true; matchedL = L;}
		      }
		  }
	}

	if(KsRECOMatched && AntiLRECOMatched){//found for this single antiS both a reconstructed Ks and antiL daughter
		reco::LeafCandidate::LorentzVector n_(0,0,0,0.939565);
		double invMassAntiS = (matchedKs->p4()+matchedL->p4()-n_).mass();
		histos_th1f["h_RECO_AntiS_m_matchedDaughtersFound"]->Fill(invMassAntiS);
		cout << "found for this single GEN antiS both a reconstructed Ks and antiL daughter (based on deltR matching); event info: id " << eventId << "invariant mass from the two RECO daughters: " << invMassAntiS  << endl;
		histos_th2f["h2_RECO_AntiS_m_matchedDaughtersFound_eventId"]->Fill(invMassAntiS, eventId);

	}	


}

void AnalyzerAllSteps::FillHistosRECOKs(const reco::VertexCompositeCandidate * RECOKs, TVector3 beamspot, edm::Handle<vector<reco::GenParticle>> h_genParticles){
	TVector3 KsCreationVertex(RECOKs->vx(),RECOKs->vy(),RECOKs->vz());
	double Lxy = lxy(beamspot,KsCreationVertex);
	TVector3 KsMomentum(RECOKs->px(),RECOKs->py(),RECOKs->pz());
	double dxy = dxy_signed_line_point(KsCreationVertex,KsMomentum,beamspot);
	histos_th1f["h_RECO_Ks_pt"]->Fill(RECOKs->pt());	
	histos_th1f["h_RECO_Ks_eta"]->Fill(RECOKs->eta());	
	histos_th1f["h_RECO_Ks_phi"]->Fill(RECOKs->phi());	
	histos_th2f["h2_RECO_Ks_vx_vy"]->Fill(RECOKs->vx(),RECOKs->vy());
	histos_th2f["h2_RECO_Ks_vx_vz"]->Fill(RECOKs->vx(),RECOKs->vz());
	histos_th1f["h_RECO_Ks_lxy"]->Fill(Lxy);	
	histos_th1f["h_RECO_Ks_vz"]->Fill(RECOKs->vz());	
	histos_th1f["h_RECO_Ks_dxy"]->Fill(dxy);
	histos_th1f["h_RECO_Ks_m"]->Fill(RECOKs->mass());

	double deltaRMin = 999;
	if(h_genParticles.isValid()){
	    for(unsigned int i = 0; i < h_genParticles->size(); ++i){//loop all genparticlesPlusGEANT
		const reco::Candidate * genParticle = &h_genParticles->at(i);
		if(abs(genParticle->pdgId())==pdgIdKs){
			double deltaPhi = reco::deltaPhi(RECOKs->phi(),genParticle->phi());
			double deltaEta = RECOKs->eta()-genParticle->eta();
			double deltaR = sqrt(deltaPhi*deltaPhi+deltaEta*deltaEta);
			if(deltaR < deltaRMin) deltaRMin = deltaR;
		}
	   }
	}
	histos_th1f["h_RECO_Ks_deltaRMin_GEN_Ks"]->Fill(deltaRMin);

	
}

void AnalyzerAllSteps::FillHistosRECOLambda(const reco::VertexCompositeCandidate * RECOLambda, TVector3 beamspot, edm::Handle<vector<reco::GenParticle>> h_genParticles){
	TVector3 LambdaCreationVertex(RECOLambda->vx(),RECOLambda->vy(),RECOLambda->vz());
	double Lxy = lxy(beamspot,LambdaCreationVertex);
	TVector3 LambdaMomentum(RECOLambda->px(),RECOLambda->py(),RECOLambda->pz());
	double dxy = dxy_signed_line_point(LambdaCreationVertex,LambdaMomentum,beamspot);
	histos_th1f["h_RECO_Lambda_pt"]->Fill(RECOLambda->pt());	
	histos_th1f["h_RECO_Lambda_eta"]->Fill(RECOLambda->eta());	
	histos_th1f["h_RECO_Lambda_phi"]->Fill(RECOLambda->phi());	
	histos_th2f["h2_RECO_Lambda_vx_vy"]->Fill(RECOLambda->vx(),RECOLambda->vy());
	histos_th2f["h2_RECO_Lambda_vx_vz"]->Fill(RECOLambda->vx(),RECOLambda->vz());
	histos_th1f["h_RECO_Lambda_lxy"]->Fill(Lxy);	
	histos_th1f["h_RECO_Lambda_vz"]->Fill(RECOLambda->vz());	
	histos_th1f["h_RECO_Lambda_dxy"]->Fill(dxy);	
	histos_th1f["h_RECO_Lambda_m"]->Fill(RECOLambda->mass());	

	double deltaRMin = 999;
	if(h_genParticles.isValid()){
	    for(unsigned int i = 0; i < h_genParticles->size(); ++i){//loop all genparticlesPlusGEANT
		const reco::Candidate * genParticle = &h_genParticles->at(i);
			double deltaPhi = reco::deltaPhi(RECOLambda->phi(),genParticle->phi());
			double deltaEta = RECOLambda->eta()-genParticle->eta();
			double deltaR = sqrt(deltaPhi*deltaPhi+deltaEta*deltaEta);
			if(deltaR < deltaRMin) deltaRMin = deltaR;
	   }
	}
	histos_th1f["h_RECO_Lambda_deltaRMin_GEN_Lambda"]->Fill(deltaRMin);

}

void AnalyzerAllSteps::FillHistosRECOAntiS(const reco::VertexCompositeCandidate * RECOAntiS, TVector3 beamspot, TVector3 beamspotVariance, edm::Handle<vector<reco::GenParticle>> h_genParticles, int eventId){
	TVector3 AntiSCreationVertex(RECOAntiS->vx(),RECOAntiS->vy(),RECOAntiS->vz());
	double Lxy = lxy(beamspot,AntiSCreationVertex);
	double error_Lxy = std_dev_lxy(RECOAntiS->vx(),RECOAntiS->vy(), RECOAntiS->vertexCovariance(0,0), RECOAntiS->vertexCovariance(1,1), beamspot.X(), beamspot.Y(), beamspotVariance.X(), beamspotVariance.Y());
	TVector3 AntiSMomentum(RECOAntiS->px(),RECOAntiS->py(),RECOAntiS->pz());
	double dxy = dxy_signed_line_point(AntiSCreationVertex,AntiSMomentum,beamspot);
	
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
	if(Lxy > 1.9)histos_th1f["h_RECO_AntiS_m_lxy_cut"]->Fill(massAntiS);
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
	//for the RECO AntiS which survives the deltaR and Lxy cut find the closest matching GEN antiS and check what is the deltaR(GEN,RECO)
	double bestDeltaRLxy_and_deltaRDaughtersCut = 999;
        if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9){
		if(h_genParticles.isValid()){
			for(unsigned int i = 0; i < h_genParticles->size(); ++i){//loop all genparticlesPlusGEANT
				const reco::Candidate * genParticle = &h_genParticles->at(i);
				if(genParticle->pdgId() == pdgIdAntiS){	
					double deltaPhi = reco::deltaPhi(genParticle->phi(),RECOAntiS->phi());
					double deltaEta = genParticle->eta()-RECOAntiS->eta();
					double deltaR = sqrt(deltaPhi*deltaPhi+deltaEta*deltaEta);
					if(deltaR<bestDeltaRLxy_and_deltaRDaughtersCut)bestDeltaRLxy_and_deltaRDaughtersCut = deltaR;
				}
			}
		}
		//now fill this bestDeltaR in a 2D histo with the mass to see if the ones with properly RECO mass also have a small deltaR...
		histos_th2f["h2_RECO_AntiS_m_lxy_and_deltaR_cut_vs_deltaR_best_GEN_match"]->Fill(massAntiS,bestDeltaRLxy_and_deltaRDaughtersCut);
	}

	//for the RECO AntiS which survives the deltaR and Lxy cut and error_Lxy find the closest matching GEN antiS and check what is the deltaR(GEN,RECO)
	double bestDeltaRLxy_and_errorLx_and_deltaRDaughtersCut = 999;
        if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9 && error_Lxy < 0.1){
		if(h_genParticles.isValid()){
			for(unsigned int i = 0; i < h_genParticles->size(); ++i){//loop all genparticlesPlusGEANT
				const reco::Candidate * genParticle = &h_genParticles->at(i);
				if(genParticle->pdgId() == pdgIdAntiS){	
					double deltaPhi = reco::deltaPhi(genParticle->phi(),RECOAntiS->phi());
					double deltaEta = genParticle->eta()-RECOAntiS->eta();
					double deltaR = sqrt(deltaPhi*deltaPhi+deltaEta*deltaEta);
					if(deltaR<bestDeltaRLxy_and_errorLx_and_deltaRDaughtersCut)bestDeltaRLxy_and_errorLx_and_deltaRDaughtersCut = deltaR;
				}
			}
		}
		//now fill this bestDeltaR in a 2D histo with the mass to see if the ones with properly RECO mass also have a small deltaR...
		histos_th2f["h2_RECO_AntiS_m_lxy_and_error_lxy_and_deltaR_cut_vs_deltaR_best_GEN_match"]->Fill(massAntiS,bestDeltaRLxy_and_errorLx_and_deltaRDaughtersCut);
	}

		
	if(abs(deltaPhiDaughters) < 2.5 && abs(deltaPhiDaughters) > 1)histos_th1f["h_RECO_AntiS_m_deltaPhi_cut"]->Fill(massAntiS);	
	if(abs(deltaPhiDaughters) < 2.5 && abs(deltaPhiDaughters) > 1 && Lxy > 1.9)histos_th1f["h_RECO_AntiS_m_lxy_and_deltaPhi_cut"]->Fill(massAntiS);	
	if(abs(deltaPhiDaughters) < 2.5 && abs(deltaPhiDaughters) > 1 && Lxy > 1.9 && error_Lxy < 0.1)histos_th1f["h_RECO_AntiS_m_lxy_and_error_lxy_and_deltaPhi_cut"]->Fill(massAntiS);

	if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9 && massAntiS > 1.5 && massAntiS < 2.1)histos_th1f["h_RECO_AntiS_error_lxy_cut_lxy_and_deltaR_and_mass"]->Fill(error_Lxy);	
	if(deltaRDaughters < 6 && deltaRDaughters > 1 && Lxy > 1.9 && massAntiS > 1.5 && massAntiS < 2.1)histos_th2f["h2_RECO_AntiS_lxy_error_lxy_cut_lxy_and_deltaR_and_mass"]->Fill(Lxy,error_Lxy);	
		


}



void AnalyzerAllSteps::RecoEvaluationKsNonAntiS(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks, TVector3 beamspot){
  const reco::Candidate  * GENKsNonAntiS = genParticle; 
  if(h_V0Ks.isValid()){
      for(unsigned int i = 0; i < h_V0Ks->size(); ++i){//loop all Ks candidates
	const reco::VertexCompositeCandidate * Ks = &h_V0Ks->at(i);	
	double deltaPhi = reco::deltaPhi(Ks->phi(),genParticle->phi());
	double deltaEta = Ks->eta() - genParticle->eta();
	double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
	histos_th1f["h_GENRECO_KsNonAntiS_deltaR"]->Fill(deltaR);
	
	TVector3 KsCreationVertex(GENKsNonAntiS->vx(),GENKsNonAntiS->vy(),GENKsNonAntiS->vz());
	double Lxy = lxy(beamspot,KsCreationVertex);
	TVector3 KsMomentum(GENKsNonAntiS->px(),GENKsNonAntiS->py(),GENKsNonAntiS->pz());
	double dxy = dxy_signed_line_point(KsCreationVertex,KsMomentum,beamspot);

	if(deltaR<0.03){//matched
		
		histos_th1f["h_GENRECO_Matched_KsNonAntiS_pt"]->Fill(GENKsNonAntiS->pt());	
		histos_th1f["h_GENRECO_Matched_KsNonAntiS_eta"]->Fill(GENKsNonAntiS->eta());	
		histos_th1f["h_GENRECO_Matched_KsNonAntiS_phi"]->Fill(GENKsNonAntiS->phi());	
		histos_th2f["h2_GENRECO_Matched_KsNonAntiS_vx_vy"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vy());
		histos_th2f["h2_GENRECO_Matched_KsNonAntiS_vx_vz"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vz());
		histos_th1f["h_GENRECO_Matched_KsNonAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_Matched_KsNonAntiS_vz"]->Fill(GENKsNonAntiS->vz());	
		histos_th1f["h_GENRECO_Matched_KsNonAntiS_dxy"]->Fill(dxy);	

	}

	else{//not matched

		histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_pt"]->Fill(GENKsNonAntiS->pt());	
		histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_eta"]->Fill(GENKsNonAntiS->eta());	
		histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_phi"]->Fill(GENKsNonAntiS->phi());	
		histos_th2f["h2_GENRECO_NotMatched_KsNonAntiS_vx_vy"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vy());
		histos_th2f["h2_GENRECO_NotMatched_KsNonAntiS_vx_vz"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vz());
		histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_vz"]->Fill(GENKsNonAntiS->vz());	
		histos_th1f["h_GENRECO_NotMatched_KsNonAntiS_dxy"]->Fill(dxy);	

	}

      }	
  }
}

void AnalyzerAllSteps::RecoEvaluationKsAntiS(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks, TVector3 beamspot){
  const reco::Candidate  * GENKsAntiS = genParticle; 
  if(h_V0Ks.isValid()){
      for(unsigned int i = 0; i < h_V0Ks->size(); ++i){//loop all Ks candidates
	const reco::VertexCompositeCandidate * Ks = &h_V0Ks->at(i);	
	double deltaPhi = reco::deltaPhi(Ks->phi(),genParticle->phi());
	double deltaEta = Ks->eta() - genParticle->eta();
	double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
	histos_th1f["h_GENRECO_KsAntiS_deltaR"]->Fill(deltaR);
	
	TVector3 KsCreationVertex(GENKsAntiS->vx(),GENKsAntiS->vy(),GENKsAntiS->vz());
	double Lxy = lxy(beamspot,KsCreationVertex);
	TVector3 KsMomentum(GENKsAntiS->px(),GENKsAntiS->py(),GENKsAntiS->pz());
	double dxy = dxy_signed_line_point(KsCreationVertex,KsMomentum,beamspot);

	if(deltaR<0.03){//matched
		
		histos_th1f["h_GENRECO_Matched_KsAntiS_pt"]->Fill(GENKsAntiS->pt());	
		histos_th1f["h_GENRECO_Matched_KsAntiS_eta"]->Fill(GENKsAntiS->eta());	
		histos_th1f["h_GENRECO_Matched_KsAntiS_phi"]->Fill(GENKsAntiS->phi());	
		histos_th2f["h2_GENRECO_Matched_KsAntiS_vx_vy"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vy());
		histos_th2f["h2_GENRECO_Matched_KsAntiS_vx_vz"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vz());
		histos_th1f["h_GENRECO_Matched_KsAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_Matched_KsAntiS_vz"]->Fill(GENKsAntiS->vz());	
		histos_th1f["h_GENRECO_Matched_KsAntiS_dxy"]->Fill(dxy);	

	}

	else{//not matched

		histos_th1f["h_GENRECO_NotMatched_KsAntiS_pt"]->Fill(GENKsAntiS->pt());	
		histos_th1f["h_GENRECO_NotMatched_KsAntiS_eta"]->Fill(GENKsAntiS->eta());	
		histos_th1f["h_GENRECO_NotMatched_KsAntiS_phi"]->Fill(GENKsAntiS->phi());	
		histos_th2f["h2_GENRECO_NotMatched_KsAntiS_vx_vy"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vy());
		histos_th2f["h2_GENRECO_NotMatched_KsAntiS_vx_vz"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vz());
		histos_th1f["h_GENRECO_NotMatched_KsAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_NotMatched_KsAntiS_vz"]->Fill(GENKsAntiS->vz());	
		histos_th1f["h_GENRECO_NotMatched_KsAntiS_dxy"]->Fill(dxy);	

	}

      }	
  }
}

void AnalyzerAllSteps::RecoEvaluationAntiLambdaNonAntiS(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L, TVector3 beamspot){
  const reco::Candidate  * GENAntiLambdaNonAntiS = genParticle; 
  if(h_V0L.isValid()){
      for(unsigned int i = 0; i < h_V0L->size(); ++i){//loop all AntiLambda candidates
	const reco::VertexCompositeCandidate * AntiLambda = &h_V0L->at(i);	
	double deltaPhi = reco::deltaPhi(AntiLambda->phi(),genParticle->phi());
	double deltaEta = AntiLambda->eta() - genParticle->eta();
	double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
	histos_th1f["h_GENRECO_AntiLambdaNonAntiS_deltaR"]->Fill(deltaR);
	
	TVector3 AntiLambdaCreationVertex(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vy(),GENAntiLambdaNonAntiS->vz());
	double Lxy = lxy(beamspot,AntiLambdaCreationVertex);
	TVector3 AntiLambdaMomentum(GENAntiLambdaNonAntiS->px(),GENAntiLambdaNonAntiS->py(),GENAntiLambdaNonAntiS->pz());
	double dxy = dxy_signed_line_point(AntiLambdaCreationVertex,AntiLambdaMomentum,beamspot);

	if(deltaR<0.03){//matched
		
		histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_pt"]->Fill(GENAntiLambdaNonAntiS->pt());	
		histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_eta"]->Fill(GENAntiLambdaNonAntiS->eta());	
		histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_phi"]->Fill(GENAntiLambdaNonAntiS->phi());	
		histos_th2f["h2_GENRECO_Matched_AntiLambdaNonAntiS_vx_vy"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vy());
		histos_th2f["h2_GENRECO_Matched_AntiLambdaNonAntiS_vx_vz"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vz());
		histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_vz"]->Fill(GENAntiLambdaNonAntiS->vz());	
		histos_th1f["h_GENRECO_Matched_AntiLambdaNonAntiS_dxy"]->Fill(dxy);	

	}

	else{//not matched

		histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_pt"]->Fill(GENAntiLambdaNonAntiS->pt());	
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_eta"]->Fill(GENAntiLambdaNonAntiS->eta());	
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_phi"]->Fill(GENAntiLambdaNonAntiS->phi());	
		histos_th2f["h2_GENRECO_NotMatched_AntiLambdaNonAntiS_vx_vy"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vy());
		histos_th2f["h2_GENRECO_NotMatched_AntiLambdaNonAntiS_vx_vz"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vz());
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_vz"]->Fill(GENAntiLambdaNonAntiS->vz());	
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaNonAntiS_dxy"]->Fill(dxy);	

	}

      }	
  }
}

void AnalyzerAllSteps::RecoEvaluationAntiLambdaAntiS(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L, TVector3 beamspot){
  const reco::Candidate  * GENAntiLambdaAntiS = genParticle; 
  if(h_V0L.isValid()){
      for(unsigned int i = 0; i < h_V0L->size(); ++i){//loop all AntiLambda candidates
	const reco::VertexCompositeCandidate * AntiLambda = &h_V0L->at(i);	
	double deltaPhi = reco::deltaPhi(AntiLambda->phi(),genParticle->phi());
	double deltaEta = AntiLambda->eta() - genParticle->eta();
	double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
	histos_th1f["h_GENRECO_AntiLambdaAntiS_deltaR"]->Fill(deltaR);
	
	TVector3 AntiLambdaCreationVertex(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vy(),GENAntiLambdaAntiS->vz());
	double Lxy = lxy(beamspot,AntiLambdaCreationVertex);
	TVector3 AntiLambdaMomentum(GENAntiLambdaAntiS->px(),GENAntiLambdaAntiS->py(),GENAntiLambdaAntiS->pz());
	double dxy = dxy_signed_line_point(AntiLambdaCreationVertex,AntiLambdaMomentum,beamspot);

	if(deltaR<0.03){//matched
		
		histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_pt"]->Fill(GENAntiLambdaAntiS->pt());	
		histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_eta"]->Fill(GENAntiLambdaAntiS->eta());	
		histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_phi"]->Fill(GENAntiLambdaAntiS->phi());	
		histos_th2f["h2_GENRECO_Matched_AntiLambdaAntiS_vx_vy"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vy());
		histos_th2f["h2_GENRECO_Matched_AntiLambdaAntiS_vx_vz"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vz());
		histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_vz"]->Fill(GENAntiLambdaAntiS->vz());	
		histos_th1f["h_GENRECO_Matched_AntiLambdaAntiS_dxy"]->Fill(dxy);	

	}

	else{//not matched

		histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_pt"]->Fill(GENAntiLambdaAntiS->pt());	
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_eta"]->Fill(GENAntiLambdaAntiS->eta());	
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_phi"]->Fill(GENAntiLambdaAntiS->phi());	
		histos_th2f["h2_GENRECO_NotMatched_AntiLambdaAntiS_vx_vy"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vy());
		histos_th2f["h2_GENRECO_NotMatched_AntiLambdaAntiS_vx_vz"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vz());
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_vz"]->Fill(GENAntiLambdaAntiS->vz());	
		histos_th1f["h_GENRECO_NotMatched_AntiLambdaAntiS_dxy"]->Fill(dxy);	

	}

      }	
  }
}



double AnalyzerAllSteps::openings_angle(reco::Candidate::Vector momentum1, reco::Candidate::Vector momentum2){
  double opening_angle = TMath::ACos((momentum1.Dot(momentum2))/(pow(momentum1.Mag2()*momentum2.Mag2(),0.5)));
  return opening_angle;
}

double AnalyzerAllSteps::deltaR(double phi1, double eta1, double phi2, double eta2){
	double deltaPhi = reco::deltaPhi(phi1,phi2);
	double deltaEta = eta1-eta2;
	return pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
}


double AnalyzerAllSteps::lxy(TVector3 v1, TVector3 v2){
	double x1 = v1.X();
	double x2 = v2.X();
	double y1 = v1.Y();
	double y2 = v2.Y();
	return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}


TVector3 AnalyzerAllSteps::PCA_line_point(TVector3 Point_line, TVector3 Vector_along_line, TVector3 Point){
   //first move the vector along the line to the starting point of Point_line
   double normalise = sqrt(Vector_along_line.X()*Vector_along_line.X()+Vector_along_line.Y()*Vector_along_line.Y()+Vector_along_line.Z()*Vector_along_line.Z());
   TVector3 n(Vector_along_line.X()/normalise,Vector_along_line.Y()/normalise,Vector_along_line.Z()/normalise);
   TVector3 a = Point_line;
   TVector3 p = Point;

   //see https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line (Vector formulation)
   TVector3 vector_PCA = (a-p)-((a-p)*n)*n;
   return vector_PCA ;
}

double AnalyzerAllSteps::dxy_signed_line_point(TVector3 Point_line_in, TVector3 Vector_along_line_in, TVector3 Point_in){

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

double AnalyzerAllSteps::std_dev_lxy(double vx, double vy, double vx_var, double vy_var, double bx_x, double bx_y, double bx_x_var, double bx_y_var){

        double lxy_std_dev_nominator = pow(vx-bx_x,2)*(vx_var+bx_x_var) + pow(vy-bx_y,2)*(vy_var+bx_y_var);
        double lxy_std_dev_denominator = pow(vx-bx_x,2) + pow(vy-bx_y,2);
        double lxy_b_std_dev = sqrt(lxy_std_dev_nominator/lxy_std_dev_denominator);
        return lxy_b_std_dev;

}

//function to return the cos of the angle between the momentum of the particle and it's displacement vector. This is for a V0 particle, so you need the V0 to decay to get it's interaction vertex
double AnalyzerAllSteps::XYpointingAngle(const reco::Candidate  * particle, TVector3 beamspot){
      double angleXY = -2;
      if(particle->numberOfDaughters() == 2){
	      TVector3 decayVertexParticle(particle->daughter(0)->vx(),particle->daughter(0)->vy(),particle->daughter(0)->vz());	 
	      double dx = decayVertexParticle.X()-beamspot.X();
	      double dy = decayVertexParticle.Y()-beamspot.Y();
	      double px = particle->px();
	      double py = particle->py();
	      angleXY = (dx*px+dy*py)/(sqrt(dx*dx+dy*dy)*sqrt(px*px+py*py));
      }
      return angleXY;
	
}

double AnalyzerAllSteps::CosOpeningsAngle(TVector3 vec1, TVector3 vec2){

  double nom = vec1.X()*vec2.X()+vec1.Y()*vec2.Y()+vec1.Z()*vec2.Z();
  double denom = sqrt(vec1.X()*vec1.X()+vec1.Y()*vec1.Y()+vec1.Z()*vec1.Z())*sqrt(vec2.X()*vec2.X()+vec2.Y()*vec2.Y()+vec2.Z()*vec2.Z());
  return nom/denom;
	
}

void AnalyzerAllSteps::endJob()
{
}

AnalyzerAllSteps::~AnalyzerAllSteps()
{
}


DEFINE_FWK_MODULE(AnalyzerAllSteps);
