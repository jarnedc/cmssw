#include "../interface/AnalyzerGEN.h"
#include <typeinfo>

AnalyzerGEN::AnalyzerGEN(edm::ParameterSet const& pset):
  m_lookAtAntiS(pset.getUntrackedParameter<bool>("lookAtAntiS")),
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


void AnalyzerGEN::beginJob() {


     TFileDirectory dir_TrackingEff = m_fs->mkdir("TrackingEff"); 

     TFileDirectory dir_TrackingEff_Global = dir_TrackingEff.mkdir("Global");
 
     histos_th2i["h2_GlobalEfficiencies"] = dir_TrackingEff_Global.make<TH2I>(b+"h2_GlobalEfficiencies", ";#bar{#Lambda}: 0=no daughters RECO, 1=only #bar{p} daug RECO, 2=only #pi^{+} daug RECO, 3=only #bar{p}&#pi^{+} daug RECO, 4=V0-#bar{#Lambda} RECO; Ks: 0=no daughters RECO, 1=only #pi^{+} daug RECO, 2=only #pi^{-} daug RECO, 3=only #pi^{+}&#pi^{-} daug RECO, 4=V0-Ks RECO",5,-0.5,4.5,5,-0.5,4.5);
     histos_th1f["h_deltaRmin_V0Ks_momentumSumKsDaughterTracks"] = dir_TrackingEff_Global.make<TH1F>(b+"h_deltaRmin_V0Ks_momentumSumKsDaughterTracks", ";Daughter #bar{S} #DeltaR(RECO V0-Ks,#vec{p}_{matched RECO track daug1}+#vec{p}_{matched RECO track daug2}); #entries ",1000,0,10);
     histos_th1f["h_deltaRmin_V0AntiL_momentumSumAntiLDaughterTracks"] = dir_TrackingEff_Global.make<TH1F>(b+"h_deltaRmin_V0AntiL_momentumSumAntiLDaughterTracks", ";Daughter #bar{S} min  #DeltaR(RECO V0-#bar{#Lambda},#vec{p}_{matched RECO track daug1}+#vec{p}_{matched RECO track daug2}); #entries ",1000,0,10);

     TFileDirectory dir_TrackingEff_NonAntiS = dir_TrackingEff.mkdir("NonAntiSTracks"); 
     
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

     TFileDirectory dir_TrackingEff_KsAntiS = dir_TrackingEff.mkdir("AntiSKsDaughterTracks"); 
     
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


     TFileDirectory dir_TrackingEff_AntiSAntiLambdaAntiProton = dir_TrackingEff.mkdir("AntiSAntiLambdaAntiProtonTracks"); 
     
     TFileDirectory dir_TrackingEff_AntiSAntiLambdaAntiProton_RECO = dir_TrackingEff_AntiSAntiLambdaAntiProton.mkdir("RECO"); 
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_pt"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_RECO.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_RECO_pt", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_eta"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_RECO.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_RECO_eta", "; SIM track #eta; #entries ",200,-10,10);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_phi"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_RECO.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_RECO_phi", "; SIM track #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_AntiSAntiLAntiProtonDaughterTracks_RECO_vx_vy"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_RECO.make<TH2F>(b+"h2_AntiSAntiLAntiProtonDaughterTracks_RECO_vx_vy", "; SIM track vx (cm); SIM track vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_AntiSAntiLAntiProtonDaughterTracks_RECO_vx_vz"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_RECO.make<TH2F>(b+"h2_AntiSAntiLAntiProtonDaughterTracks_RECO_vx_vz", "; SIM track vx (cm); SIM track vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_lxy"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_RECO.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_RECO_lxy", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_vz"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_RECO.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_RECO_vz", "; SIM track vz(SIM track vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_dxy"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_RECO.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_RECO_dxy", "; SIM track dxy(beamspot) (cm); #entries ",400,-20,20);


     TFileDirectory dir_TrackingEff_AntiSAntiLambdaAntiProton_All = dir_TrackingEff_AntiSAntiLambdaAntiProton.mkdir("All"); 
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_pt"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_All.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_All_pt", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_eta"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_All.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_All_eta", "; SIM track #eta; #entries ",200,-10,10);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_phi"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_All.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_All_phi", "; SIM track #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_AntiSAntiLAntiProtonDaughterTracks_All_vx_vy"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_All.make<TH2F>(b+"h2_AntiSAntiLAntiProtonDaughterTracks_All_vx_vy", "; SIM track vx (cm); SIM track vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_AntiSAntiLAntiProtonDaughterTracks_All_vx_vz"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_All.make<TH2F>(b+"h2_AntiSAntiLAntiProtonDaughterTracks_All_vx_vz", "; SIM track vx (cm); SIM track vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_lxy"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_All.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_All_lxy", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_vz"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_All.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_All_vz", "; SIM track vz(SIM track vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_dxy"] = dir_TrackingEff_AntiSAntiLambdaAntiProton_All.make<TH1F>(b+"h_AntiSAntiLAntiProtonDaughterTracks_All_dxy", "; SIM track dxy(beamspot) (cm); #entries ",400,-20,20);


     TFileDirectory dir_TrackingEff_AntiSAntiLambdaPosPion = dir_TrackingEff.mkdir("AntiSAntiLambdaPosPionTracks"); 
     
     TFileDirectory dir_TrackingEff_AntiSAntiLambdaPosPion_RECO = dir_TrackingEff_AntiSAntiLambdaPosPion.mkdir("RECO"); 
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_pt"] = dir_TrackingEff_AntiSAntiLambdaPosPion_RECO.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_RECO_pt", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_eta"] = dir_TrackingEff_AntiSAntiLambdaPosPion_RECO.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_RECO_eta", "; SIM track #eta; #entries ",200,-10,10);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_phi"] = dir_TrackingEff_AntiSAntiLambdaPosPion_RECO.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_RECO_phi", "; SIM track #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_AntiSAntiLPosPionDaughterTracks_RECO_vx_vy"] = dir_TrackingEff_AntiSAntiLambdaPosPion_RECO.make<TH2F>(b+"h2_AntiSAntiLPosPionDaughterTracks_RECO_vx_vy", "; SIM track vx (cm); SIM track vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_AntiSAntiLPosPionDaughterTracks_RECO_vx_vz"] = dir_TrackingEff_AntiSAntiLambdaPosPion_RECO.make<TH2F>(b+"h2_AntiSAntiLPosPionDaughterTracks_RECO_vx_vz", "; SIM track vx (cm); SIM track vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_lxy"] = dir_TrackingEff_AntiSAntiLambdaPosPion_RECO.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_RECO_lxy", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_vz"] = dir_TrackingEff_AntiSAntiLambdaPosPion_RECO.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_RECO_vz", "; SIM track vz(SIM track vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_dxy"] = dir_TrackingEff_AntiSAntiLambdaPosPion_RECO.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_RECO_dxy", "; SIM track dxy(beamspot) (cm); #entries ",400,-20,20);


     TFileDirectory dir_TrackingEff_AntiSAntiLambdaPosPion_All = dir_TrackingEff_AntiSAntiLambdaPosPion.mkdir("All"); 
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_pt"] = dir_TrackingEff_AntiSAntiLambdaPosPion_All.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_All_pt", "; SIM track pT (GeV); #entries ",200,0,20);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_eta"] = dir_TrackingEff_AntiSAntiLambdaPosPion_All.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_All_eta", "; SIM track #eta; #entries ",200,-10,10);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_phi"] = dir_TrackingEff_AntiSAntiLambdaPosPion_All.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_All_phi", "; SIM track #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_AntiSAntiLPosPionDaughterTracks_All_vx_vy"] = dir_TrackingEff_AntiSAntiLambdaPosPion_All.make<TH2F>(b+"h2_AntiSAntiLPosPionDaughterTracks_All_vx_vy", "; SIM track vx (cm); SIM track vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_AntiSAntiLPosPionDaughterTracks_All_vx_vz"] = dir_TrackingEff_AntiSAntiLambdaPosPion_All.make<TH2F>(b+"h2_AntiSAntiLPosPionDaughterTracks_All_vx_vz", "; SIM track vx (cm); SIM track vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_lxy"] = dir_TrackingEff_AntiSAntiLambdaPosPion_All.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_All_lxy", "; SIM track lxy(beamspot, SIM track vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_vz"] = dir_TrackingEff_AntiSAntiLambdaPosPion_All.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_All_vz", "; SIM track vz(SIM track vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_dxy"] = dir_TrackingEff_AntiSAntiLambdaPosPion_All.make<TH1F>(b+"h_AntiSAntiLPosPionDaughterTracks_All_dxy", "; SIM track dxy(beamspot) (cm); #entries ",400,-20,20);


     TFileDirectory dir_GEN = m_fs->mkdir("GEN"); 

     TFileDirectory dir_GEN_antiS = dir_GEN.mkdir("GEN_antiS"); 
     histos_th1f["h_GEN_nAntiSTotal"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_nAntiSTotal", "; 0 = all #bar{S}/ 1 = interacting #bar{S}/ 2 = #bar{S} with corr grandd; #entries ",20,0,20);
     histos_th1f["h_GEN_nAntiS"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_nAntiS", "; #bar{S}/event; #entries ",20,0,20);
     histos_th1f["h_GEN_nAntiSInteract"] = dir_GEN_antiS.make<TH1F>(b+"h_GEN_nAntiSInteract", "; #bar{S} with 2 daughters/event; #entries ",20,0,20);
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
     histos_th1f["h_GEN_KsAntiS_lxyz"] = dir_GEN_KsAntiS.make<TH1F>(b+"h_GEN_KsAntiS_lxyz", "; KsAntiS lxyz(beamspot, KsAntiS creationvertex) (cm); #entries ",300,0,300);
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
     histos_th1f["h_GEN_AntiLambdaAntiS_lxyz"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_lxyz", "; AntiLambdaAntiS lxyz(beamspot, AntiLambdaAntiS creationvertex) (cm); #entries ",300,0,300);
     histos_th1f["h_GEN_AntiLambdaAntiS_dxy"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_dxy", "; AntiLambdaAntiS dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_GEN_AntiLambdaAntiS_XYpointingAngle"] = dir_GEN_AntiLambdaAntiS.make<TH1F>(b+"h_GEN_AntiLambdaAntiS_XYpointingAngle", "; AntiLambdaAntiS cos[(#vec{l_{xy}}(#bar{#Lambda} daughter).#vec{p_{xy}}(#bar{#Lambda}))/(||#vec{l_{xy}}(#bar{#Lambda daughter})||.||#vec{p_{xy}}(#bar{#Lambda})||)] ; #entries ",20000,-1.5,1.5);

     TFileDirectory dir_GENRECO = m_fs->mkdir("GENRECO"); 

     TFileDirectory dir_GENRECO_KsNonAntiS = dir_GENRECO.mkdir("GENRECO_KsNonAntiS");
     histos_th1f["h_GENRECO_KsNonAntiS_deltaRmin"] = dir_GENRECO_KsNonAntiS.make<TH1F>(b+"h_GENRECO_KsNonAntiS_deltaRmin", "; min #DeltaR(gen Ks, reco Ks); #entries ",1000,0,10);

     TFileDirectory dir_GENRECO_RECO_KsNonAntiS = dir_GENRECO_KsNonAntiS.mkdir("GENRECO_RECO_KsNonAntiS");
     histos_th1f["h_GENRECO_RECO_KsNonAntiS_pt"] = dir_GENRECO_RECO_KsNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsNonAntiS_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_RECO_KsNonAntiS_eta"] = dir_GENRECO_RECO_KsNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsNonAntiS_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_RECO_KsNonAntiS_phi"] = dir_GENRECO_RECO_KsNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsNonAntiS_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_RECO_KsNonAntiS_vx_vy"] = dir_GENRECO_RECO_KsNonAntiS.make<TH2F>(b+"h2_GENRECO_RECO_KsNonAntiS_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_RECO_KsNonAntiS_vx_vz"] = dir_GENRECO_RECO_KsNonAntiS.make<TH2F>(b+"h2_GENRECO_RECO_KsNonAntiS_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_RECO_KsNonAntiS_lxy"] = dir_GENRECO_RECO_KsNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsNonAntiS_lxy", "; Ks lxy(beamspot, Ks vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_RECO_KsNonAntiS_vz"] =  dir_GENRECO_RECO_KsNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsNonAntiS_vz", "; Ks vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_RECO_KsNonAntiS_dxy"] = dir_GENRECO_RECO_KsNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsNonAntiS_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_All_KsNonAntiS = dir_GENRECO_KsNonAntiS.mkdir("GENRECO_All_KsNonAntiS");
     histos_th1f["h_GENRECO_All_KsNonAntiS_pt"] = dir_GENRECO_All_KsNonAntiS.make<TH1F>(b+"h_GENRECO_All_KsNonAntiS_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_All_KsNonAntiS_eta"] = dir_GENRECO_All_KsNonAntiS.make<TH1F>(b+"h_GENRECO_All_KsNonAntiS_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_All_KsNonAntiS_phi"] = dir_GENRECO_All_KsNonAntiS.make<TH1F>(b+"h_GENRECO_All_KsNonAntiS_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_All_KsNonAntiS_vx_vy"] = dir_GENRECO_All_KsNonAntiS.make<TH2F>(b+"h2_GENRECO_All_KsNonAntiS_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_All_KsNonAntiS_vx_vz"] = dir_GENRECO_All_KsNonAntiS.make<TH2F>(b+"h2_GENRECO_All_KsNonAntiS_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_All_KsNonAntiS_lxy"] = dir_GENRECO_All_KsNonAntiS.make<TH1F>(b+"h_GENRECO_All_KsNonAntiS_lxy", "; Ks lxy(beamspot, Ks vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_All_KsNonAntiS_vz"] =  dir_GENRECO_All_KsNonAntiS.make<TH1F>(b+"h_GENRECO_All_KsNonAntiS_vz", "; Ks vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_All_KsNonAntiS_dxy"] = dir_GENRECO_All_KsNonAntiS.make<TH1F>(b+"h_GENRECO_All_KsNonAntiS_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);

     
     TFileDirectory dir_GENRECO_KsAntiS = dir_GENRECO.mkdir("GENRECO_KsAntiS");
     histos_th1f["h_GENRECO_KsAntiS_deltaRmin"] = dir_GENRECO_KsAntiS.make<TH1F>(b+"h_GENRECO_KsAntiS_deltaRmin", "; min #DeltaR(gen Ks, reco Ks); #entries ",1000,0,10);

     TFileDirectory dir_GENRECO_RECO_KsAntiS = dir_GENRECO_KsAntiS.mkdir("GENRECO_RECO_KsAntiS");
     histos_th1f["h_GENRECO_RECO_KsAntiS_pt"] = dir_GENRECO_RECO_KsAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsAntiS_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_RECO_KsAntiS_eta"] = dir_GENRECO_RECO_KsAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsAntiS_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_RECO_KsAntiS_phi"] = dir_GENRECO_RECO_KsAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsAntiS_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_RECO_KsAntiS_vx_vy"] = dir_GENRECO_RECO_KsAntiS.make<TH2F>(b+"h2_GENRECO_RECO_KsAntiS_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_RECO_KsAntiS_vx_vz"] = dir_GENRECO_RECO_KsAntiS.make<TH2F>(b+"h2_GENRECO_RECO_KsAntiS_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_RECO_KsAntiS_lxy"] = dir_GENRECO_RECO_KsAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsAntiS_lxy", "; Ks lxy(beamspot, Ks vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_RECO_KsAntiS_vz"] =  dir_GENRECO_RECO_KsAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsAntiS_vz", "; Ks vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_RECO_KsAntiS_dxy"] = dir_GENRECO_RECO_KsAntiS.make<TH1F>(b+"h_GENRECO_RECO_KsAntiS_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_All_KsAntiS = dir_GENRECO_KsAntiS.mkdir("GENRECO_All_KsAntiS");
     histos_th1f["h_GENRECO_All_KsAntiS_pt"] = dir_GENRECO_All_KsAntiS.make<TH1F>(b+"h_GENRECO_All_KsAntiS_pt", "; Ks pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_All_KsAntiS_eta"] = dir_GENRECO_All_KsAntiS.make<TH1F>(b+"h_GENRECO_All_KsAntiS_eta", "; Ks #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_All_KsAntiS_phi"] = dir_GENRECO_All_KsAntiS.make<TH1F>(b+"h_GENRECO_All_KsAntiS_phi", "; Ks #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_All_KsAntiS_vx_vy"] = dir_GENRECO_All_KsAntiS.make<TH2F>(b+"h2_GENRECO_All_KsAntiS_vx_vy", "; Ks vx (cm); Ks vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_All_KsAntiS_vx_vz"] = dir_GENRECO_All_KsAntiS.make<TH2F>(b+"h2_GENRECO_All_KsAntiS_vx_vz", "; Ks vx (cm); Ks vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_All_KsAntiS_lxy"] = dir_GENRECO_All_KsAntiS.make<TH1F>(b+"h_GENRECO_All_KsAntiS_lxy", "; Ks lxy(beamspot, Ks vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_All_KsAntiS_vz"] =  dir_GENRECO_All_KsAntiS.make<TH1F>(b+"h_GENRECO_All_KsAntiS_vz", "; Ks vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_All_KsAntiS_dxy"] = dir_GENRECO_All_KsAntiS.make<TH1F>(b+"h_GENRECO_All_KsAntiS_dxy", "; Ks dxy(beamspot) (cm); #entries ",400,-20,20);


     TFileDirectory dir_GENRECO_AntiLambdaNonAntiS = dir_GENRECO.mkdir("GENRECO_AntiLambdaNonAntiS");
     histos_th1f["h_GENRECO_AntiLambdaNonAntiS_deltaRmin"] = dir_GENRECO_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_AntiLambdaNonAntiS_deltaRmin", "; min #DeltaR(gen AntiLambda, reco AntiLambda); #entries ",1000,0,10);

     TFileDirectory dir_GENRECO_RECO_AntiLambdaNonAntiS = dir_GENRECO_AntiLambdaNonAntiS.mkdir("GENRECO_RECO_AntiLambdaNonAntiS");
     histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_pt"] = dir_GENRECO_RECO_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaNonAntiS_pt", "; AntiLambda pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_eta"] = dir_GENRECO_RECO_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaNonAntiS_eta", "; AntiLambda #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_phi"] = dir_GENRECO_RECO_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaNonAntiS_phi", "; AntiLambda #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_RECO_AntiLambdaNonAntiS_vx_vy"] = dir_GENRECO_RECO_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GENRECO_RECO_AntiLambdaNonAntiS_vx_vy", "; AntiLambda vx (cm); AntiLambda vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_RECO_AntiLambdaNonAntiS_vx_vz"] = dir_GENRECO_RECO_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GENRECO_RECO_AntiLambdaNonAntiS_vx_vz", "; AntiLambda vx (cm); AntiLambda vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_lxy"] = dir_GENRECO_RECO_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaNonAntiS_lxy", "; AntiLambda lxy(beamspot, AntiLambda vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_vz"] =  dir_GENRECO_RECO_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaNonAntiS_vz", "; AntiLambda vz(AntiLambda vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_dxy"] = dir_GENRECO_RECO_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaNonAntiS_dxy", "; AntiLambda dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_All_AntiLambdaNonAntiS = dir_GENRECO_AntiLambdaNonAntiS.mkdir("GENRECO_All_AntiLambdaNonAntiS");
     histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_pt"] = dir_GENRECO_All_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaNonAntiS_pt", "; AntiLambda pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_eta"] = dir_GENRECO_All_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaNonAntiS_eta", "; AntiLambda #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_phi"] = dir_GENRECO_All_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaNonAntiS_phi", "; AntiLambda #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_All_AntiLambdaNonAntiS_vx_vy"] = dir_GENRECO_All_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GENRECO_All_AntiLambdaNonAntiS_vx_vy", "; AntiLambda vx (cm); AntiLambda vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_All_AntiLambdaNonAntiS_vx_vz"] = dir_GENRECO_All_AntiLambdaNonAntiS.make<TH2F>(b+"h2_GENRECO_All_AntiLambdaNonAntiS_vx_vz", "; AntiLambda vx (cm); AntiLambda vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_lxy"] = dir_GENRECO_All_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaNonAntiS_lxy", "; AntiLambda lxy(beamspot, AntiLambda vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_vz"] =  dir_GENRECO_All_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaNonAntiS_vz", "; AntiLambda vz(AntiLambda vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_dxy"] = dir_GENRECO_All_AntiLambdaNonAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaNonAntiS_dxy", "; AntiLambda dxy(beamspot) (cm); #entries ",400,-20,20);
     

     TFileDirectory dir_GENRECO_AntiLambdaAntiS = dir_GENRECO.mkdir("GENRECO_AntiLambdaAntiS");
     histos_th1f["h_GENRECO_AntiLambdaAntiS_deltaRmin"] = dir_GENRECO_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_AntiLambdaAntiS_deltaRmin", "; min #DeltaR(gen AntiLambda, reco AntiLambda); #entries ",1000,0,10);

     TFileDirectory dir_GENRECO_RECO_AntiLambdaAntiS = dir_GENRECO_AntiLambdaAntiS.mkdir("GENRECO_RECO_AntiLambdaAntiS");
     histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_pt"] = dir_GENRECO_RECO_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaAntiS_pt", "; AntiLambda pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_eta"] = dir_GENRECO_RECO_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaAntiS_eta", "; AntiLambda #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_phi"] = dir_GENRECO_RECO_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaAntiS_phi", "; AntiLambda #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_RECO_AntiLambdaAntiS_vx_vy"] = dir_GENRECO_RECO_AntiLambdaAntiS.make<TH2F>(b+"h2_GENRECO_RECO_AntiLambdaAntiS_vx_vy", "; AntiLambda vx (cm); AntiLambda vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_RECO_AntiLambdaAntiS_vx_vz"] = dir_GENRECO_RECO_AntiLambdaAntiS.make<TH2F>(b+"h2_GENRECO_RECO_AntiLambdaAntiS_vx_vz", "; AntiLambda vx (cm); AntiLambda vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_lxy"] = dir_GENRECO_RECO_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaAntiS_lxy", "; AntiLambda lxy(beamspot, AntiLambda vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_vz"] =  dir_GENRECO_RECO_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaAntiS_vz", "; AntiLambda vz(AntiLambda vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_dxy"] = dir_GENRECO_RECO_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiLambdaAntiS_dxy", "; AntiLambda dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_All_AntiLambdaAntiS = dir_GENRECO_AntiLambdaAntiS.mkdir("GENRECO_All_AntiLambdaAntiS");
     histos_th1f["h_GENRECO_All_AntiLambdaAntiS_pt"] = dir_GENRECO_All_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaAntiS_pt", "; AntiLambda pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_All_AntiLambdaAntiS_eta"] = dir_GENRECO_All_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaAntiS_eta", "; AntiLambda #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_All_AntiLambdaAntiS_phi"] = dir_GENRECO_All_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaAntiS_phi", "; AntiLambda #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_All_AntiLambdaAntiS_vx_vy"] = dir_GENRECO_All_AntiLambdaAntiS.make<TH2F>(b+"h2_GENRECO_All_AntiLambdaAntiS_vx_vy", "; AntiLambda vx (cm); AntiLambda vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_All_AntiLambdaAntiS_vx_vz"] = dir_GENRECO_All_AntiLambdaAntiS.make<TH2F>(b+"h2_GENRECO_All_AntiLambdaAntiS_vx_vz", "; AntiLambda vx (cm); AntiLambda vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_All_AntiLambdaAntiS_lxy"] = dir_GENRECO_All_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaAntiS_lxy", "; AntiLambda lxy(beamspot, AntiLambda vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_All_AntiLambdaAntiS_vz"] =  dir_GENRECO_All_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaAntiS_vz", "; AntiLambda vz(AntiLambda vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_All_AntiLambdaAntiS_dxy"] = dir_GENRECO_All_AntiLambdaAntiS.make<TH1F>(b+"h_GENRECO_All_AntiLambdaAntiS_dxy", "; AntiLambda dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_AntiS = dir_GENRECO.mkdir("GENRECO_AntiS");
     histos_th1f["h_GENRECO_AntiS_deltaRmin"] = dir_GENRECO_AntiS.make<TH1F>(b+"h_GENRECO_AntiS_deltaRmin", "; min #DeltaR(gen AntiS, reco AntiS); #entries ",1000,0,10);

     TFileDirectory dir_GENRECO_RECO_AntiS = dir_GENRECO_AntiS.mkdir("GENRECO_RECO_AntiS");
     histos_th1f["h_GENRECO_RECO_AntiS_pt"] = dir_GENRECO_RECO_AntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiS_pt", "; #bar{S} pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_RECO_AntiS_eta"] = dir_GENRECO_RECO_AntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiS_eta", "; #bar{S} #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_RECO_AntiS_phi"] = dir_GENRECO_RECO_AntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiS_phi", "; #bar{S} #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_RECO_AntiS_vx_vy"] = dir_GENRECO_RECO_AntiS.make<TH2F>(b+"h2_GENRECO_RECO_AntiS_vx_vy", "; #bar{S} vx (cm); #bar{S} vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_RECO_AntiS_vx_vz"] = dir_GENRECO_RECO_AntiS.make<TH2F>(b+"h2_GENRECO_RECO_AntiS_vx_vz", "; #bar{S} vx (cm); #bar{S} vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_RECO_AntiS_lxy"] = dir_GENRECO_RECO_AntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiS_lxy", "; #bar{S} lxy(beamspot, #bar{S} vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_RECO_AntiS_lxy_interactionVertex"] = dir_GENRECO_RECO_AntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiS_lxy_interactionVertex", "; #bar{S} lxy(beamspot, #bar{S} interaction vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_RECO_AntiS_vz"] =  dir_GENRECO_RECO_AntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiS_vz", "; #bar{S} vz(#bar{S} vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_RECO_AntiS_vz_interactionVertex"] =  dir_GENRECO_RECO_AntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiS_vz_interactionVertex", "; #bar{S} vz(#bar{S} decay vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_RECO_AntiS_dxy"] = dir_GENRECO_RECO_AntiS.make<TH1F>(b+"h_GENRECO_RECO_AntiS_dxy", "; #bar{S} dxy(beamspot) (cm); #entries ",400,-20,20);

     TFileDirectory dir_GENRECO_All_AntiS = dir_GENRECO_AntiS.mkdir("GENRECO_All_AntiS");
     histos_th1f["h_GENRECO_All_AntiS_pt"] = dir_GENRECO_All_AntiS.make<TH1F>(b+"h_GENRECO_All_AntiS_pt", "; #bar{S} pT (GeV); #entries ",200,0,20);
     histos_th1f["h_GENRECO_All_AntiS_eta"] = dir_GENRECO_All_AntiS.make<TH1F>(b+"h_GENRECO_All_AntiS_eta", "; #bar{S} #eta; #entries ",200,-10,10);
     histos_th1f["h_GENRECO_All_AntiS_phi"] = dir_GENRECO_All_AntiS.make<TH1F>(b+"h_GENRECO_All_AntiS_phi", "; #bar{S} #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_GENRECO_All_AntiS_vx_vy"] = dir_GENRECO_All_AntiS.make<TH2F>(b+"h2_GENRECO_All_AntiS_vx_vy", "; #bar{S} vx (cm); #bar{S} vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_GENRECO_All_AntiS_vx_vz"] = dir_GENRECO_All_AntiS.make<TH2F>(b+"h2_GENRECO_All_AntiS_vx_vz", "; #bar{S} vx (cm); #bar{S} vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_GENRECO_All_AntiS_lxy"] = dir_GENRECO_All_AntiS.make<TH1F>(b+"h_GENRECO_All_AntiS_lxy", "; #bar{S} lxy(beamspot, #bar{S} vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_All_AntiS_lxy_interactionVertex"] = dir_GENRECO_All_AntiS.make<TH1F>(b+"h_GENRECO_All_AntiS_lxy_interactionVertex", "; #bar{S} lxy(beamspot, #bar{S} decay vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_GENRECO_All_AntiS_vz"] =  dir_GENRECO_All_AntiS.make<TH1F>(b+"h_GENRECO_All_AntiS_vz", "; #bar{S} vz(#bar{S} vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_All_AntiS_vz_interactionVertex"] =  dir_GENRECO_All_AntiS.make<TH1F>(b+"h_GENRECO_All_AntiS_vz_interactionVertex", "; #bar{S} vz(#bar{S} decay vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_GENRECO_All_AntiS_dxy"] = dir_GENRECO_All_AntiS.make<TH1F>(b+"h_GENRECO_All_AntiS_dxy", "; #bar{S} dxy(beamspot) (cm); #entries ",400,-20,20);



     TFileDirectory dir_RECOSignal_BackgroundDiscriminatingCuts = dir_GENRECO_AntiS.mkdir("RECOSignal_BackgroundDiscriminatingCuts");
     histos_th2f["h2_RECOSignal_AntiS_lxy_error_lxy"] = dir_RECOSignal_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOSignal_AntiS_lxy_error_lxy",";lxy(beampsot, RECO #bar{S} interaction vertex); #sigma lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",500,0,50,500,0,5);
     histos_th2f["h2_RECOSignal_AntiS_lxy_deltaRDaughters"] = dir_RECOSignal_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOSignal_AntiS_lxy_deltaRDaughters",";lxy(beampsot, RECO #bar{S} interaction vertex);#DeltaR(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);#Entries",500,0,50,100,0,10);
     histos_th2f["h2_RECOSignal_AntiS_deltaRDaughters_error_lxy"]  = dir_RECOSignal_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOSignal_AntiS_deltaRDaughters_error_lxy",";#DeltaR(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);sigma lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",100,0,10,500,0,5);
     histos_th2f["h2_RECOSignal_AntiS_pKs_pAntiL"]  = dir_RECOSignal_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOSignal_AntiS_pKs_pAntiL",";p Ks daug; p #bar{Lamba} daug;#Entries",100,0,10,100,0,10);
     histos_th2f["h2_RECOSignal_AntiS_lxy_error_lxy_cut_deltaRDaughters"]  = dir_RECOSignal_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOSignal_AntiS_lxy_error_lxy_cut_deltaRDaughters",";lxy(beampsot, RECO #bar{S} interaction vertex);#sigma lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",500,0,50,500,0,5);
     histos_th2f["h2_RECOSignal_AntiS_deltaRDaughters_lxy_cut_error_lxy"]  = dir_RECOSignal_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOSignal_AntiS_deltaRDaughters_lxy_cut_error_lxy",";#DeltaR(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",100,0,10,500,0,50);
     histos_th3f["h2_RECOSignal_AntiS_deltaRDaughters_lxy_mass_cut_error_lxy"]  = dir_RECOSignal_BackgroundDiscriminatingCuts.make<TH3F>(b+"h2_RECOSignal_AntiS_deltaRDaughters_lxy_mass_cut_error_lxy",";#DeltaR(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);lxy(beampsot, RECO #bar{S} interaction vertex) (cm);mass #bar{S} (GeV)",60,0,6,40,0,20,40,0,10);
     histos_th2f["h2_RECOSignal_AntiS_deltaPhiDaughters_lxy_cut_error_lxy"]  = dir_RECOSignal_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOSignal_AntiS_deltaPhiDaughters_lxy_cut_error_lxy",";#Delta#Phi(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",80,-4,4,500,0,50);
     histos_th2f["h2_RECOSignal_AntiS_deltaRDaughters_error_lxy_cut_lxy"]  = dir_RECOSignal_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOSignal_AntiS_deltaRDaughters_error_lxy_cut_lxy",";#DeltaR(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);sigma lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",100,0,10,500,0,5);

/*     TFileDirectory dir_RECOBackground_BackgroundDiscriminatingCuts = dir_GENRECO_AntiS.mkdir("RECOBackground_BackgroundDiscriminatingCuts");
     histos_th2f["h2_RECOBackground_AntiS_lxy_error_lxy"] = dir_RECOBackground_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOBackground_AntiS_lxy_error_lxy",";lxy(beampsot, RECO #bar{S} interaction vertex); #sigma lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",500,0,50,500,0,5);
     histos_th2f["h2_RECOBackground_AntiS_lxy_deltaRDaughters"] = dir_RECOBackground_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOBackground_AntiS_lxy_deltaRDaughters",";lxy(beampsot, RECO #bar{S} interaction vertex);#DeltaR(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);#Entries",500,0,50,100,0,10);
     histos_th2f["h2_RECOBackground_AntiS_deltaRDaughters_error_lxy"]  = dir_RECOBackground_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOBackground_AntiS_deltaRDaughters_error_lxy",";#DeltaR(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);sigma lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",100,0,10,500,0,5);
     histos_th2f["h2_RECOBackground_AntiS_pKs_pAntiL"]  = dir_RECOBackground_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOBackground_AntiS_pKs_pAntiL",";p Ks daug; p #bar{Lamba} daug;#Entries",100,0,10,100,0,10);
     histos_th2f["h2_RECOBackground_AntiS_lxy_error_lxy_cut_deltaRDaughters"]  = dir_RECOBackground_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOBackground_AntiS_lxy_error_lxy_cut_deltaRDaughters",";lxy(beampsot, RECO #bar{S} interaction vertex);#sigma lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",500,0,50,500,0,5);
     histos_th2f["h2_RECOBackground_AntiS_lxy_deltaRDaughters_cut_error_lxy"]  = dir_RECOBackground_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOBackground_AntiS_lxy_deltaRDaughters_cut_error_lxy",";lxy(beampsot, RECO #bar{S} interaction vertex);#DeltaR(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);#Entries",500,0,50,100,0,10);
     histos_th2f["h2_RECOBackground_AntiS_deltaRDaughters_error_lxy_cut_lxy"]  = dir_RECOBackground_BackgroundDiscriminatingCuts.make<TH2F>(b+"h2_RECOBackground_AntiS_deltaRDaughters_error_lxy_cut_lxy",";#DeltaR(#bar{S} Ks daug, #bar{S} #bar{#Lambda} daug);sigma lxy(beampsot, RECO #bar{S} interaction vertex);#Entries",100,0,10,500,0,5);
*/
     TFileDirectory dir_GENRECO_AntiS_accuracy = dir_GENRECO_AntiS.mkdir("GENRECO_AntiS_accuracy");
     histos_th1f["h_RECOAcc_AntiS_pt"] = dir_GENRECO_AntiS_accuracy.make<TH1F>(b+"h_RECOAcc_AntiS_pt", "; #bar{S}: pt_{GEN}-pt_{RECO} ; #entries ",1000,-5,5);
     histos_th1f["h_RECOAcc_AntiS_phi"] = dir_GENRECO_AntiS_accuracy.make<TH1F>(b+"h_RECOAcc_AntiS_phi", "; #bar{S}: #phi_{GEN}-#phi_{RECO}; #entries ",200,-1,1);
     histos_th1f["h_RECOAcc_AntiS_eta"] = dir_GENRECO_AntiS_accuracy.make<TH1F>(b+"h_RECOAcc_AntiS_eta", "; #bar{S}: #eta_{GEN}-#eta_{RECO}; #entries ",200,-1,1);
     histos_th1f["h_RECOAcc_AntiS_mass"] = dir_GENRECO_AntiS_accuracy.make<TH1F>(b+"h_RECOAcc_AntiS_mass", "; #bar{S}: #m_{GEN}-#m_{RECO}; #entries ",200,-1,1);
     histos_th1f["h_RECOAcc_AntiS_lxy_interactionVertex"] = dir_GENRECO_AntiS_accuracy.make<TH1F>(b+"h_RECOAcc_AntiS_lxy_interactionVertex", "; #bar{S}: lxy(beampsot, #bar{S}interaction vertex)_{GEN}-lxy(beampsot, #bar{S}interaction vertex)_{RECO}; #entries ",2000,-100,100);
     histos_th1f["h_RECOAcc_AntiS_vz_interactionVertex"] = dir_GENRECO_AntiS_accuracy.make<TH1F>(b+"h_RECOAcc_AntiS_vz_interactionVertex", "; #bar{S}: vz(beampsot, #bar{S}interaction vertex)_{GEN}-vz(beampsot, #bar{S}interaction vertex)_{RECO}; #entries ",2000,-100,100);
	
     

}

void AnalyzerGEN::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup) {

 
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
	  
	  if(tp.pdgId() == AnalyzerAllSteps::pdgIdAntiS)FillHistosAntiSTracks(tp, beamspot, TPColl,  h_TP, h_trackAssociator, h_generalTracks, h_V0Ks, h_V0L);
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
		if(genParticle->pdgId() == AnalyzerAllSteps::pdgIdAntiS) {genParticleIsAntiS = true;}
		if(genParticleIsAntiS){nAntiSThisEvent++; FillHistosGENAntiS(genParticle, beamspot);}

		//all Ks
		bool genParticleIsKs = false;	
		bool genParticleMotherIsAntiS = false;//Ks as daughter of an antiS	
		if(abs(genParticle->pdgId()) == AnalyzerAllSteps::pdgIdKs)genParticleIsKs =true;
		if(genParticle->numberOfMothers()==1){if(genParticle->mother(0)->pdgId()==AnalyzerAllSteps::pdgIdAntiS){genParticleMotherIsAntiS=true;}};
		if(genParticleIsKs && !genParticleMotherIsAntiS){nKsThisEvent++; FillHistosGENKsNonAntiS(genParticle,beamspot);};
		if(genParticleIsKs && genParticleMotherIsAntiS)FillHistosGENKsAntiS(genParticle,beamspot);

		//all antiLambda
		bool genParticleIsAntiLambda = false;
		if(genParticle->pdgId() == AnalyzerAllSteps::pdgIdAntiLambda)genParticleIsAntiLambda=true;
		if(genParticleIsAntiLambda && !genParticleMotherIsAntiS){nAntiLambdaThisEvent++;FillHistosGENAntiLambdaNonAntiS(genParticle,beamspot);};
		if(genParticleIsAntiLambda && genParticleMotherIsAntiS)FillHistosGENAntiLambdaAntiS(genParticle,beamspot);


		//now start looking at the RECO efficiencies of daughters and antiS, you can do this based on deltaR, becaused these particles are not charged. For all of these you want however that at least they have the correct daughters and for the antiS the correct granddaughters:
		if(genParticle->numberOfDaughters()==2){
			int daughterParticlesTypes = getDaughterParticlesTypes(genParticle);
			//for Ks
			if(genParticleIsKs && !genParticleMotherIsAntiS && daughterParticlesTypes == 1)RecoEvaluationKsNonAntiS(genParticle,h_V0Ks,beamspot);
			if(genParticleIsKs && genParticleMotherIsAntiS && daughterParticlesTypes == 1)RecoEvaluationKsAntiS(genParticle,h_V0Ks,beamspot);
			//for AntiL
			if(genParticleIsAntiLambda && !genParticleMotherIsAntiS && daughterParticlesTypes == 2)RecoEvaluationAntiLambdaNonAntiS(genParticle,h_V0L,beamspot);
			if(genParticleIsAntiLambda && genParticleMotherIsAntiS && daughterParticlesTypes == 2)RecoEvaluationAntiLambdaAntiS(genParticle,h_V0L,beamspot);
			//for antiS
			if(genParticle->daughter(0)->numberOfDaughters()==2 && genParticle->daughter(1)->numberOfDaughters()==2 && daughterParticlesTypes == 3){
				int graddaughters0ParticlesTypes = getDaughterParticlesTypes(genParticle->daughter(0));
				int graddaughters1ParticlesTypes = getDaughterParticlesTypes(genParticle->daughter(1));
				if(genParticleIsAntiS && ((graddaughters0ParticlesTypes == 1 && graddaughters1ParticlesTypes == 2) || (graddaughters1ParticlesTypes == 1 && graddaughters0ParticlesTypes == 2))) RecoEvaluationAntiS(genParticle,h_sCands,beamspot, beamspotVariance);
			}
		}
      }//for(unsigned int i = 0; i < h_genParticles->size(); ++i)
  }//if(h_genParticles.isValid())
  histos_th1f["h_GEN_nAntiS"]->Fill(nAntiSThisEvent);
  histos_th1f["h_GEN_nAntiSInteract"]->Fill(nAntiSInteractThisEvent);
  histos_th1f["h_GEN_nKs"]->Fill(nKsThisEvent);
  histos_th1f["h_GEN_nAntiLambda"]->Fill(nAntiLambdaThisEvent); 


}

void AnalyzerGEN::FillHistosNonAntiSTracksRECO(const TrackingParticle& tp, TVector3 beamspot){
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

void AnalyzerGEN::FillHistosNonAntiSTracksAll(const TrackingParticle& tp, TVector3 beamspot){
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

void AnalyzerGEN::FillHistosAntiSTracks(const TrackingParticle& tp, TVector3 beamspot, TrackingParticleCollection const & TPColl, edm::Handle<TrackingParticleCollection> h_TP, edm::Handle< reco::TrackToTrackingParticleAssociator> h_trackAssociator, edm::Handle<View<reco::Track>> h_generalTracks, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L){
	//now start from this tp and go down in gen particles to check if you find daughter and granddaughters
	vector<bool> granddaughterTrackMatched;
	granddaughterTrackMatched.push_back(false);granddaughterTrackMatched.push_back(false);granddaughterTrackMatched.push_back(false);granddaughterTrackMatched.push_back(false);
	const reco::Track *matchedTrackPointerKsPosPion = nullptr;
	const reco::Track *matchedTrackPointerKsNegPion = nullptr;
	const reco::Track *matchedTrackPointerAntiLPosPion = nullptr;
	const reco::Track *matchedTrackPointerAntiLNegProton = nullptr;
	
	int nCorrectGrandDaughters = 0;
	
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
						if(tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdAntiProton || tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdPosPion ||tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdNegPion ){//found a tp that is a granddaughter of the antiS
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
									    if(abs(tp_daughter.pdgId()) == 310 && tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdPosPion){granddaughterTrackMatched[0]=true;matchedTrackPointerKsPosPion= matchedTrackPointer; FillHistosAntiSKsDaughterTracksRECO(tp_granddaughter,beamspot);}
									    else if(abs(tp_daughter.pdgId()) == 310 && tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdNegPion){granddaughterTrackMatched[1]=true;matchedTrackPointerKsNegPion=matchedTrackPointer;FillHistosAntiSKsDaughterTracksRECO(tp_granddaughter,beamspot);}
									    else if(tp_daughter.pdgId() == -3122 && tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdAntiProton){granddaughterTrackMatched[2]=true;matchedTrackPointerAntiLPosPion=matchedTrackPointer; FillHistosAntiSAntiLAntiProtonDaughterTracksRECO(tp_granddaughter,beamspot);}
									    else if(tp_daughter.pdgId() == -3122 && tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdPosPion){granddaughterTrackMatched[3]=true;matchedTrackPointerAntiLNegProton=matchedTrackPointer;FillHistosAntiSAntiLPosPionDaughterTracksRECO(tp_granddaughter,beamspot);}
								  }
							  }
							  if(abs(tp_daughter.pdgId()) == 310 && tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdPosPion){nCorrectGrandDaughters++;FillHistosAntiSKsDaughterTracksAll(tp_granddaughter,beamspot);}
							  else if(abs(tp_daughter.pdgId()) == 310 && tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdNegPion){nCorrectGrandDaughters++;FillHistosAntiSKsDaughterTracksAll(tp_granddaughter,beamspot);}
							  else if(tp_daughter.pdgId() == -3122 && tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdAntiProton){nCorrectGrandDaughters++;FillHistosAntiSAntiLAntiProtonDaughterTracksAll(tp_granddaughter,beamspot);}
							  else if(tp_daughter.pdgId() == -3122 && tp_granddaughter.pdgId()==AnalyzerAllSteps::pdgIdPosPion){nCorrectGrandDaughters++;FillHistosAntiSAntiLPosPionDaughterTracksAll(tp_granddaughter,beamspot);}
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
  
  std::cout << "number of correct (=charged) granddaughters of this antiS: " << nCorrectGrandDaughters << std::endl;
  if(nCorrectGrandDaughters == 4)FillMajorEfficiencyPlot(granddaughterTrackMatched, matchedTrackPointerKsPosPion,matchedTrackPointerKsNegPion,matchedTrackPointerAntiLPosPion,matchedTrackPointerAntiLNegProton,h_V0Ks,h_V0L);

}

void AnalyzerGEN::FillHistosAntiSKsDaughterTracksRECO(const TrackingParticle& tp, TVector3 beamspot){
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
void AnalyzerGEN::FillHistosAntiSKsDaughterTracksAll(const TrackingParticle& tp, TVector3 beamspot){
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

void AnalyzerGEN::FillHistosAntiSAntiLAntiProtonDaughterTracksRECO(const TrackingParticle& tp, TVector3 beamspot){
	TVector3 tpCreationVertex(tp.vx(),tp.vy(),tp.vz());
	double Lxy = lxy(beamspot,tpCreationVertex);
	TVector3 tpMomentum(tp.px(),tp.py(),tp.pz());
	double dxy = dxy_signed_line_point(tpCreationVertex,tpMomentum,beamspot);
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_pt"]->Fill(tp.pt());	
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_eta"]->Fill(tp.eta());	
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_phi"]->Fill(tp.phi());	
	histos_th2f["h2_AntiSAntiLAntiProtonDaughterTracks_RECO_vx_vy"]->Fill(tp.vx(),tp.vy());
	histos_th2f["h2_AntiSAntiLAntiProtonDaughterTracks_RECO_vx_vz"]->Fill(tp.vx(),tp.vz());
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_lxy"]->Fill(Lxy);	
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_vz"]->Fill(tp.vz());	
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_RECO_dxy"]->Fill(dxy);
	
}
void AnalyzerGEN::FillHistosAntiSAntiLAntiProtonDaughterTracksAll(const TrackingParticle& tp, TVector3 beamspot){
	TVector3 tpCreationVertex(tp.vx(),tp.vy(),tp.vz());
	double Lxy = lxy(beamspot,tpCreationVertex);
	TVector3 tpMomentum(tp.px(),tp.py(),tp.pz());
	double dxy = dxy_signed_line_point(tpCreationVertex,tpMomentum,beamspot);
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_pt"]->Fill(tp.pt());	
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_eta"]->Fill(tp.eta());	
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_phi"]->Fill(tp.phi());	
	histos_th2f["h2_AntiSAntiLAntiProtonDaughterTracks_All_vx_vy"]->Fill(tp.vx(),tp.vy());
	histos_th2f["h2_AntiSAntiLAntiProtonDaughterTracks_All_vx_vz"]->Fill(tp.vx(),tp.vz());
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_lxy"]->Fill(Lxy);	
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_vz"]->Fill(tp.vz());	
	histos_th1f["h_AntiSAntiLAntiProtonDaughterTracks_All_dxy"]->Fill(dxy);
	
}

void AnalyzerGEN::FillHistosAntiSAntiLPosPionDaughterTracksRECO(const TrackingParticle& tp, TVector3 beamspot){
	TVector3 tpCreationVertex(tp.vx(),tp.vy(),tp.vz());
	double Lxy = lxy(beamspot,tpCreationVertex);
	TVector3 tpMomentum(tp.px(),tp.py(),tp.pz());
	double dxy = dxy_signed_line_point(tpCreationVertex,tpMomentum,beamspot);
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_pt"]->Fill(tp.pt());	
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_eta"]->Fill(tp.eta());	
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_phi"]->Fill(tp.phi());	
	histos_th2f["h2_AntiSAntiLPosPionDaughterTracks_RECO_vx_vy"]->Fill(tp.vx(),tp.vy());
	histos_th2f["h2_AntiSAntiLPosPionDaughterTracks_RECO_vx_vz"]->Fill(tp.vx(),tp.vz());
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_lxy"]->Fill(Lxy);	
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_vz"]->Fill(tp.vz());	
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_RECO_dxy"]->Fill(dxy);
	
}
void AnalyzerGEN::FillHistosAntiSAntiLPosPionDaughterTracksAll(const TrackingParticle& tp, TVector3 beamspot){
	TVector3 tpCreationVertex(tp.vx(),tp.vy(),tp.vz());
	double Lxy = lxy(beamspot,tpCreationVertex);
	TVector3 tpMomentum(tp.px(),tp.py(),tp.pz());
	double dxy = dxy_signed_line_point(tpCreationVertex,tpMomentum,beamspot);
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_pt"]->Fill(tp.pt());	
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_eta"]->Fill(tp.eta());	
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_phi"]->Fill(tp.phi());	
	histos_th2f["h2_AntiSAntiLPosPionDaughterTracks_All_vx_vy"]->Fill(tp.vx(),tp.vy());
	histos_th2f["h2_AntiSAntiLPosPionDaughterTracks_All_vx_vz"]->Fill(tp.vx(),tp.vz());
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_lxy"]->Fill(Lxy);	
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_vz"]->Fill(tp.vz());	
	histos_th1f["h_AntiSAntiLPosPionDaughterTracks_All_dxy"]->Fill(dxy);
	
}


void AnalyzerGEN::FillMajorEfficiencyPlot(std::vector<bool>granddaughterTrackMatched, const reco::Track *matchedTrackPointerKsPosPion,const reco::Track *matchedTrackPointerKsNegPion,const reco::Track *matchedTrackPointerAntiLPosPion,const reco::Track *matchedTrackPointerAntiLNegProton, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L){
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
			if(deltaR < deltaRmin) deltaRmin = deltaR;
		      }
		      if(deltaRmin<deltaRCutV0RECOKs)matchingV0KsFound = true;
		  }
		  histos_th1f["h_deltaRmin_V0Ks_momentumSumKsDaughterTracks"]->Fill(deltaRmin);
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
			if(deltaR < deltaRmin) deltaRmin = deltaR;
		      }
		      if(deltaRmin<deltaRCutV0RECOLambda)matchingV0AntiLFound = true;
		  }
		histos_th1f["h_deltaRmin_V0AntiL_momentumSumAntiLDaughterTracks"]->Fill(deltaRmin);
	}

	if(!granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,0.);
	if(granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,1.);
	if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,2.);
	if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,3.);

	if(!granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,0.);
	if(granddaughterTrackMatched[0]  && !granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,1.);
	if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,2.);
	if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,3.);

	if(!granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,0.);
	if(granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,1.);
	if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,2.);
	if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && !granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,3.);

	if(!granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,0.);
	if(granddaughterTrackMatched[0] && !granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,1.);
	if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,2.);
	if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1] && granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,3.);

	if(matchingV0KsFound && !matchingV0AntiLFound){//fill the top row of the matrix
		if( !granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(0.,4.);
		if(granddaughterTrackMatched[2] && !granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(1.,4.);
		if(!granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(2.,4.);
		if(granddaughterTrackMatched[2] && granddaughterTrackMatched[3])histos_th2i["h2_GlobalEfficiencies"]->Fill(3.,4.);
	}
	if(!matchingV0KsFound && matchingV0AntiLFound){//fill the right column of the matrix
		if( !granddaughterTrackMatched[0] && !granddaughterTrackMatched[1])histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,0.);
		if(granddaughterTrackMatched[0] && !granddaughterTrackMatched[1])histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,1.);
		if(!granddaughterTrackMatched[0] && granddaughterTrackMatched[1])histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,2.);
		if(granddaughterTrackMatched[0] && granddaughterTrackMatched[1])histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,3.);

	}
	if(matchingV0KsFound && matchingV0AntiLFound){//this means you found both a RECO V0-Ks and RECO V0-AntiL
		histos_th2i["h2_GlobalEfficiencies"]->Fill(4.,4.);
	}

	

}

void AnalyzerGEN::FillHistosGENAntiS(const reco::Candidate  * GENantiS, TVector3 beamspot){

	TVector3 momentumGENantiS(GENantiS->px(),GENantiS->py(),GENantiS->pz());
	histos_th1f["h_GEN_nAntiSTotal"]->Fill(0);	
	histos_th1f["h_GEN_AntiS_pt"]->Fill(GENantiS->pt());	
	histos_th1f["h_GEN_AntiS_eta"]->Fill(GENantiS->eta());	
	histos_th1f["h_GEN_AntiS_phi"]->Fill(GENantiS->phi());
		
	if(GENantiS->numberOfDaughters()==2){
		TVector3 momentumKs(GENantiS->daughter(0)->px(),GENantiS->daughter(0)->py(),GENantiS->daughter(0)->pz());
		TVector3 momentumAntiLambda(GENantiS->daughter(1)->px(),GENantiS->daughter(1)->py(),GENantiS->daughter(1)->pz());
		if(abs(GENantiS->daughter(1)->pdgId())==AnalyzerAllSteps::pdgIdKs){momentumKs.SetX(GENantiS->daughter(1)->px());momentumKs.SetY(GENantiS->daughter(1)->py());momentumKs.SetZ(GENantiS->daughter(1)->pz());}
		if(GENantiS->daughter(0)->pdgId()==AnalyzerAllSteps::pdgIdAntiLambda){momentumAntiLambda.SetX(GENantiS->daughter(0)->px());momentumAntiLambda.SetY(GENantiS->daughter(0)->py());momentumAntiLambda.SetZ(GENantiS->daughter(0)->pz());}

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


void AnalyzerGEN::FillHistosGENKsNonAntiS(const reco::Candidate  * GENKsNonAntiS, TVector3 beamspot){
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

void AnalyzerGEN::FillHistosGENAntiLambdaNonAntiS(const reco::Candidate  * GENAntiLambdaNonAntiS, TVector3 beamspot){
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


void AnalyzerGEN::FillHistosGENKsAntiS(const reco::Candidate  * GENKsAntiS, TVector3 beamspot){
	histos_th1f["h_GEN_nAntiSTotal"]->Fill(1);	
	TVector3 KsAntiSCreationVertex(GENKsAntiS->vx(),GENKsAntiS->vy(),GENKsAntiS->vz());
	double Lxy = lxy(beamspot,KsAntiSCreationVertex);
	double Lxyz = lxyz(beamspot,KsAntiSCreationVertex);
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
	histos_th1f["h_GEN_KsAntiS_lxyz"]->Fill(Lxyz);	
	histos_th1f["h_GEN_KsAntiS_dxy"]->Fill(dxy);
	if(GENKsAntiS->numberOfDaughters() == 2)histos_th1f["h_GEN_KsAntiS_XYpointingAngle"]->Fill(xypointingAngle);
		
}

void AnalyzerGEN::FillHistosGENAntiLambdaAntiS(const reco::Candidate  * GENAntiLambdaAntiS, TVector3 beamspot){
	TVector3 AntiLambdaAntiSCreationVertex(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vy(),GENAntiLambdaAntiS->vz());
	double Lxy = lxy(beamspot,AntiLambdaAntiSCreationVertex);
	double Lxyz = lxyz(beamspot,AntiLambdaAntiSCreationVertex);
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
	histos_th1f["h_GEN_AntiLambdaAntiS_lxyz"]->Fill(Lxyz);	
	histos_th1f["h_GEN_AntiLambdaAntiS_dxy"]->Fill(dxy);
	if(GENAntiLambdaAntiS->numberOfDaughters() == 2)histos_th1f["h_GEN_AntiLambdaAntiS_XYpointingAngle"]->Fill(xypointingAngle);
	
}





void AnalyzerGEN::RecoEvaluationKsNonAntiS(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks, TVector3 beamspot){
  const reco::Candidate  * GENKsNonAntiS = genParticle; 
  if(h_V0Ks.isValid()){
      double deltaRmin = 999.;
      for(unsigned int i = 0; i < h_V0Ks->size(); ++i){//loop all Ks candidates
	const reco::VertexCompositeCandidate * Ks = &h_V0Ks->at(i);	
	double deltaPhi = reco::deltaPhi(Ks->phi(),genParticle->phi());
	double deltaEta = Ks->eta() - genParticle->eta();
	double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
        if(deltaR < deltaRmin) deltaRmin = deltaR;
       }	
	histos_th1f["h_GENRECO_KsNonAntiS_deltaRmin"]->Fill(deltaRmin);
	TVector3 KsCreationVertex(GENKsNonAntiS->vx(),GENKsNonAntiS->vy(),GENKsNonAntiS->vz());
	double Lxy = lxy(beamspot,KsCreationVertex);
	TVector3 KsMomentum(GENKsNonAntiS->px(),GENKsNonAntiS->py(),GENKsNonAntiS->pz());
	double dxy = dxy_signed_line_point(KsCreationVertex,KsMomentum,beamspot);

	if(deltaRmin<deltaRCutV0RECOKs){//matched
		
		histos_th1f["h_GENRECO_RECO_KsNonAntiS_pt"]->Fill(GENKsNonAntiS->pt());	
		histos_th1f["h_GENRECO_RECO_KsNonAntiS_eta"]->Fill(GENKsNonAntiS->eta());	
		histos_th1f["h_GENRECO_RECO_KsNonAntiS_phi"]->Fill(GENKsNonAntiS->phi());	
		histos_th2f["h2_GENRECO_RECO_KsNonAntiS_vx_vy"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vy());
		histos_th2f["h2_GENRECO_RECO_KsNonAntiS_vx_vz"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vz());
		histos_th1f["h_GENRECO_RECO_KsNonAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_RECO_KsNonAntiS_vz"]->Fill(GENKsNonAntiS->vz());	
		histos_th1f["h_GENRECO_RECO_KsNonAntiS_dxy"]->Fill(dxy);	

	}


	histos_th1f["h_GENRECO_All_KsNonAntiS_pt"]->Fill(GENKsNonAntiS->pt());	
	histos_th1f["h_GENRECO_All_KsNonAntiS_eta"]->Fill(GENKsNonAntiS->eta());	
	histos_th1f["h_GENRECO_All_KsNonAntiS_phi"]->Fill(GENKsNonAntiS->phi());	
	histos_th2f["h2_GENRECO_All_KsNonAntiS_vx_vy"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vy());
	histos_th2f["h2_GENRECO_All_KsNonAntiS_vx_vz"]->Fill(GENKsNonAntiS->vx(),GENKsNonAntiS->vz());
	histos_th1f["h_GENRECO_All_KsNonAntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_GENRECO_All_KsNonAntiS_vz"]->Fill(GENKsNonAntiS->vz());	
	histos_th1f["h_GENRECO_All_KsNonAntiS_dxy"]->Fill(dxy);	


      }	
  
}

void AnalyzerGEN::RecoEvaluationKsAntiS(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0Ks, TVector3 beamspot){
  const reco::Candidate  * GENKsAntiS = genParticle; 
  if(h_V0Ks.isValid()){
      double deltaRmin = 999.;
      for(unsigned int i = 0; i < h_V0Ks->size(); ++i){//loop all Ks candidates
	const reco::VertexCompositeCandidate * Ks = &h_V0Ks->at(i);	
	double deltaPhi = reco::deltaPhi(Ks->phi(),genParticle->phi());
	double deltaEta = Ks->eta() - genParticle->eta();
	double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
        if(deltaR < deltaRmin) deltaRmin = deltaR;
      }
      histos_th1f["h_GENRECO_KsAntiS_deltaRmin"]->Fill(deltaRmin);

	TVector3 KsCreationVertex(GENKsAntiS->vx(),GENKsAntiS->vy(),GENKsAntiS->vz());
	double Lxy = lxy(beamspot,KsCreationVertex);
	TVector3 KsMomentum(GENKsAntiS->px(),GENKsAntiS->py(),GENKsAntiS->pz());
	double dxy = dxy_signed_line_point(KsCreationVertex,KsMomentum,beamspot);

	if(deltaRmin<deltaRCutV0RECOKs){//matched
		
		histos_th1f["h_GENRECO_RECO_KsAntiS_pt"]->Fill(GENKsAntiS->pt());	
		histos_th1f["h_GENRECO_RECO_KsAntiS_eta"]->Fill(GENKsAntiS->eta());	
		histos_th1f["h_GENRECO_RECO_KsAntiS_phi"]->Fill(GENKsAntiS->phi());	
		histos_th2f["h2_GENRECO_RECO_KsAntiS_vx_vy"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vy());
		histos_th2f["h2_GENRECO_RECO_KsAntiS_vx_vz"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vz());
		histos_th1f["h_GENRECO_RECO_KsAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_RECO_KsAntiS_vz"]->Fill(GENKsAntiS->vz());	
		histos_th1f["h_GENRECO_RECO_KsAntiS_dxy"]->Fill(dxy);	

	}


	histos_th1f["h_GENRECO_All_KsAntiS_pt"]->Fill(GENKsAntiS->pt());	
	histos_th1f["h_GENRECO_All_KsAntiS_eta"]->Fill(GENKsAntiS->eta());	
	histos_th1f["h_GENRECO_All_KsAntiS_phi"]->Fill(GENKsAntiS->phi());	
	histos_th2f["h2_GENRECO_All_KsAntiS_vx_vy"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vy());
	histos_th2f["h2_GENRECO_All_KsAntiS_vx_vz"]->Fill(GENKsAntiS->vx(),GENKsAntiS->vz());
	histos_th1f["h_GENRECO_All_KsAntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_GENRECO_All_KsAntiS_vz"]->Fill(GENKsAntiS->vz());	
	histos_th1f["h_GENRECO_All_KsAntiS_dxy"]->Fill(dxy);	


      	
  }
}

void AnalyzerGEN::RecoEvaluationAntiLambdaNonAntiS(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L, TVector3 beamspot){
  const reco::Candidate  * GENAntiLambdaNonAntiS = genParticle; 
  if(h_V0L.isValid()){
      double deltaRmin = 999.;
      for(unsigned int i = 0; i < h_V0L->size(); ++i){//loop all AntiLambda candidates
	const reco::VertexCompositeCandidate * AntiLambda = &h_V0L->at(i);	
	double deltaPhi = reco::deltaPhi(AntiLambda->phi(),genParticle->phi());
	double deltaEta = AntiLambda->eta() - genParticle->eta();
	double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
	if(deltaR < deltaRmin) deltaRmin = deltaR;
      }	
	histos_th1f["h_GENRECO_AntiLambdaNonAntiS_deltaRmin"]->Fill(deltaRmin);

	TVector3 AntiLambdaCreationVertex(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vy(),GENAntiLambdaNonAntiS->vz());
	double Lxy = lxy(beamspot,AntiLambdaCreationVertex);
	TVector3 AntiLambdaMomentum(GENAntiLambdaNonAntiS->px(),GENAntiLambdaNonAntiS->py(),GENAntiLambdaNonAntiS->pz());
	double dxy = dxy_signed_line_point(AntiLambdaCreationVertex,AntiLambdaMomentum,beamspot);

	if(deltaRmin<deltaRCutV0RECOLambda){//matched
		
		histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_pt"]->Fill(GENAntiLambdaNonAntiS->pt());	
		histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_eta"]->Fill(GENAntiLambdaNonAntiS->eta());	
		histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_phi"]->Fill(GENAntiLambdaNonAntiS->phi());	
		histos_th2f["h2_GENRECO_RECO_AntiLambdaNonAntiS_vx_vy"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vy());
		histos_th2f["h2_GENRECO_RECO_AntiLambdaNonAntiS_vx_vz"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vz());
		histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_vz"]->Fill(GENAntiLambdaNonAntiS->vz());	
		histos_th1f["h_GENRECO_RECO_AntiLambdaNonAntiS_dxy"]->Fill(dxy);	

	}


	histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_pt"]->Fill(GENAntiLambdaNonAntiS->pt());	
	histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_eta"]->Fill(GENAntiLambdaNonAntiS->eta());	
	histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_phi"]->Fill(GENAntiLambdaNonAntiS->phi());	
	histos_th2f["h2_GENRECO_All_AntiLambdaNonAntiS_vx_vy"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vy());
	histos_th2f["h2_GENRECO_All_AntiLambdaNonAntiS_vx_vz"]->Fill(GENAntiLambdaNonAntiS->vx(),GENAntiLambdaNonAntiS->vz());
	histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_vz"]->Fill(GENAntiLambdaNonAntiS->vz());	
	histos_th1f["h_GENRECO_All_AntiLambdaNonAntiS_dxy"]->Fill(dxy);	


      	
  }
}

void AnalyzerGEN::RecoEvaluationAntiLambdaAntiS(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_V0L, TVector3 beamspot){
  const reco::Candidate  * GENAntiLambdaAntiS = genParticle; 
  if(h_V0L.isValid()){
	double deltaRmin = 999.;
      for(unsigned int i = 0; i < h_V0L->size(); ++i){//loop all AntiLambda candidates
	const reco::VertexCompositeCandidate * AntiLambda = &h_V0L->at(i);	
	double deltaPhi = reco::deltaPhi(AntiLambda->phi(),genParticle->phi());
	double deltaEta = AntiLambda->eta() - genParticle->eta();
	double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
	if(deltaR < deltaRmin) deltaRmin = deltaR;
      }
	histos_th1f["h_GENRECO_AntiLambdaAntiS_deltaRmin"]->Fill(deltaRmin);

	
	TVector3 AntiLambdaCreationVertex(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vy(),GENAntiLambdaAntiS->vz());
	double Lxy = lxy(beamspot,AntiLambdaCreationVertex);
	TVector3 AntiLambdaMomentum(GENAntiLambdaAntiS->px(),GENAntiLambdaAntiS->py(),GENAntiLambdaAntiS->pz());
	double dxy = dxy_signed_line_point(AntiLambdaCreationVertex,AntiLambdaMomentum,beamspot);

	if(deltaRmin<deltaRCutV0RECOLambda){//matched
		
		histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_pt"]->Fill(GENAntiLambdaAntiS->pt());	
		histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_eta"]->Fill(GENAntiLambdaAntiS->eta());	
		histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_phi"]->Fill(GENAntiLambdaAntiS->phi());	
		histos_th2f["h2_GENRECO_RECO_AntiLambdaAntiS_vx_vy"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vy());
		histos_th2f["h2_GENRECO_RECO_AntiLambdaAntiS_vx_vz"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vz());
		histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_lxy"]->Fill(Lxy);	
		histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_vz"]->Fill(GENAntiLambdaAntiS->vz());	
		histos_th1f["h_GENRECO_RECO_AntiLambdaAntiS_dxy"]->Fill(dxy);	

	}


	histos_th1f["h_GENRECO_All_AntiLambdaAntiS_pt"]->Fill(GENAntiLambdaAntiS->pt());	
	histos_th1f["h_GENRECO_All_AntiLambdaAntiS_eta"]->Fill(GENAntiLambdaAntiS->eta());	
	histos_th1f["h_GENRECO_All_AntiLambdaAntiS_phi"]->Fill(GENAntiLambdaAntiS->phi());	
	histos_th2f["h2_GENRECO_All_AntiLambdaAntiS_vx_vy"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vy());
	histos_th2f["h2_GENRECO_All_AntiLambdaAntiS_vx_vz"]->Fill(GENAntiLambdaAntiS->vx(),GENAntiLambdaAntiS->vz());
	histos_th1f["h_GENRECO_All_AntiLambdaAntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_GENRECO_All_AntiLambdaAntiS_vz"]->Fill(GENAntiLambdaAntiS->vz());	
	histos_th1f["h_GENRECO_All_AntiLambdaAntiS_dxy"]->Fill(dxy);	


  }
}


void AnalyzerGEN::RecoEvaluationAntiS(const reco::Candidate  * genParticle, edm::Handle<vector<reco::VertexCompositeCandidate> > h_sCands, TVector3 beamspot, TVector3 beamspotVariance){
  const reco::Candidate  * GENAntiS = genParticle;   
  
  if(h_sCands.isValid()){
      double deltaRmin = 999.;
      const reco::VertexCompositeCandidate * bestRECOAntiS = nullptr;
      for(unsigned int i = 0; i < h_sCands->size(); ++i){//loop all AntiS RECO candidates
	const reco::VertexCompositeCandidate * AntiS = &h_sCands->at(i);	
	double deltaPhi = reco::deltaPhi(AntiS->phi(),genParticle->phi());
	double deltaEta = AntiS->eta() - genParticle->eta();
	double deltaR = pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
	if(deltaR < deltaRmin){ deltaRmin = deltaR; bestRECOAntiS = AntiS;}
      }
	histos_th1f["h_GENRECO_AntiS_deltaRmin"]->Fill(deltaRmin);

	//calculate some kinmatic variables for the GEN antiS	
	TVector3 GENAntiSCreationVertex(GENAntiS->vx(),GENAntiS->vy(),GENAntiS->vz());
	double GENLxy = lxy(beamspot,GENAntiSCreationVertex);
	TVector3 GENAntiSMomentum(GENAntiS->px(),GENAntiS->py(),GENAntiS->pz());
	double GENdxy = dxy_signed_line_point(GENAntiSCreationVertex,GENAntiSMomentum,beamspot);
	TVector3 GENAntiSInteractionVertex(GENAntiS->daughter(0)->vx(),GENAntiS->daughter(0)->vy(),GENAntiS->daughter(0)->vz());
	double GENLxy_interactionVertex = lxy(beamspot,GENAntiSInteractionVertex);
	
	//calculate some kinematic variables for the RECO AntiS
	TVector3 RECOAntiSInteractionVertex(bestRECOAntiS->vx(),bestRECOAntiS->vy(),bestRECOAntiS->vz());//this is the interaction vertex of the antiS and the neutron. Check in the skimming code if you want to check.
	double RECOLxy_interactionVertex = lxy(beamspot,RECOAntiSInteractionVertex);
	double RECOErrorLxy_interactionVertex = std_dev_lxy(bestRECOAntiS->vx(), bestRECOAntiS->vy(), bestRECOAntiS->vertexCovariance(0,0), bestRECOAntiS->vertexCovariance(1,1), beamspot.X(), beamspot.Y(), beamspotVariance.X(), beamspotVariance.Y());
	double RECODeltaPhiDaughters = reco::deltaPhi(bestRECOAntiS->daughter(0)->phi(),bestRECOAntiS->daughter(1)->phi());
	double RECODeltaEtaDaughters = bestRECOAntiS->daughter(0)->eta()-bestRECOAntiS->daughter(1)->eta();
	double RECODeltaRDaughters = pow(RECODeltaPhiDaughters*RECODeltaPhiDaughters+RECODeltaEtaDaughters*RECODeltaEtaDaughters,0.5);

	if(deltaRmin<deltaRCutRECOAntiS){//matched
		
		histos_th1f["h_GENRECO_RECO_AntiS_pt"]->Fill(GENAntiS->pt());	
		histos_th1f["h_GENRECO_RECO_AntiS_eta"]->Fill(GENAntiS->eta());	
		histos_th1f["h_GENRECO_RECO_AntiS_phi"]->Fill(GENAntiS->phi());	
		histos_th2f["h2_GENRECO_RECO_AntiS_vx_vy"]->Fill(GENAntiS->vx(),GENAntiS->vy());
		histos_th2f["h2_GENRECO_RECO_AntiS_vx_vz"]->Fill(GENAntiS->vx(),GENAntiS->vz());
		histos_th1f["h_GENRECO_RECO_AntiS_lxy"]->Fill(GENLxy);	
		histos_th1f["h_GENRECO_RECO_AntiS_lxy_interactionVertex"]->Fill(GENLxy_interactionVertex);	
		histos_th1f["h_GENRECO_RECO_AntiS_vz"]->Fill(GENAntiS->vz());	
		histos_th1f["h_GENRECO_RECO_AntiS_vz_interactionVertex"]->Fill(GENAntiSInteractionVertex.Z());	
		histos_th1f["h_GENRECO_RECO_AntiS_dxy"]->Fill(GENdxy);	
		
		//so now if you found a RECO antiS compare it to the GEN antiS. Compare parameters such as GEN vs RECO pt, GEN vs RECO interaction vertex, ...
		histos_th1f["h_RECOAcc_AntiS_pt"]->Fill(GENAntiS->pt()-bestRECOAntiS->pt());
		histos_th1f["h_RECOAcc_AntiS_phi"]->Fill(reco::deltaPhi(GENAntiS->phi(),bestRECOAntiS->phi()));
		histos_th1f["h_RECOAcc_AntiS_eta"]->Fill(GENAntiS->eta()-bestRECOAntiS->eta());
		histos_th1f["h_RECOAcc_AntiS_mass"]->Fill(GENAntiS->mass()-bestRECOAntiS->mass());
		histos_th1f["h_RECOAcc_AntiS_lxy_interactionVertex"]->Fill(GENLxy_interactionVertex-RECOLxy_interactionVertex);
		histos_th1f["h_RECOAcc_AntiS_vz_interactionVertex"]->Fill(GENAntiSInteractionVertex.Z()-RECOAntiSInteractionVertex.Z());
		
		//so now you know that these RECO AntiS are really signal, so what you can do is plot the variables which you will cut on to get rid of the background for the signal
		histos_th2f["h2_RECOSignal_AntiS_lxy_error_lxy"]->Fill(RECOLxy_interactionVertex,  RECOErrorLxy_interactionVertex);
		histos_th2f["h2_RECOSignal_AntiS_lxy_deltaRDaughters"]->Fill(RECOLxy_interactionVertex,  RECODeltaRDaughters);
		histos_th2f["h2_RECOSignal_AntiS_deltaRDaughters_error_lxy"]->Fill(RECODeltaRDaughters,  RECOErrorLxy_interactionVertex);
		histos_th2f["h2_RECOSignal_AntiS_pKs_pAntiL"]->Fill(bestRECOAntiS->daughter(0)->p(), bestRECOAntiS->daughter(1)->p());
		if(RECODeltaRDaughters<6 && RECODeltaRDaughters>1 )histos_th2f["h2_RECOSignal_AntiS_lxy_error_lxy_cut_deltaRDaughters"]->Fill(RECOLxy_interactionVertex,  RECOErrorLxy_interactionVertex);
		if(RECOErrorLxy_interactionVertex<0.1)histos_th2f["h2_RECOSignal_AntiS_deltaRDaughters_lxy_cut_error_lxy"]->Fill(RECODeltaRDaughters,RECOLxy_interactionVertex);
		if(RECOErrorLxy_interactionVertex<0.1)histos_th3f["h2_RECOSignal_AntiS_deltaRDaughters_lxy_mass_cut_error_lxy"]->Fill(RECODeltaRDaughters,RECOLxy_interactionVertex,bestRECOAntiS->mass());
		if(RECOErrorLxy_interactionVertex<0.1)histos_th2f["h2_RECOSignal_AntiS_deltaPhiDaughters_lxy_cut_error_lxy"]->Fill(RECODeltaPhiDaughters,RECOLxy_interactionVertex);
		if(RECOLxy_interactionVertex>1.9)histos_th2f["h2_RECOSignal_AntiS_deltaRDaughters_error_lxy_cut_lxy"]->Fill(RECODeltaRDaughters,  RECOErrorLxy_interactionVertex);
	 		
	}
/*	else{//all what false outside this deltaR is background, fill some plots with the background discriminating variable --> you should not look at this, this is not correct. It is not a reference for your background, because if you did not find a RECO antiS for this GEN antiS, which can happen as it might not be reconstructed, but what you do below is plot anyway the kinematic variables of a the best RECOAntiS, which will make these plots look like your background.
		histos_th2f["h2_RECOBackground_AntiS_lxy_error_lxy"]->Fill(RECOLxy_interactionVertex,  RECOErrorLxy_interactionVertex);
		histos_th2f["h2_RECOBackground_AntiS_lxy_deltaRDaughters"]->Fill(RECOLxy_interactionVertex,  RECODeltaRDaughters);
		histos_th2f["h2_RECOBackground_AntiS_deltaRDaughters_error_lxy"]->Fill(RECODeltaRDaughters,  RECOErrorLxy_interactionVertex);
		histos_th2f["h2_RECOBackground_AntiS_pKs_pAntiL"]->Fill(bestRECOAntiS->daughter(0)->p(), bestRECOAntiS->daughter(1)->p());
		if(RECODeltaRDaughters<6 && RECODeltaRDaughters>1 )histos_th2f["h2_RECOBackground_AntiS_lxy_error_lxy_cut_deltaRDaughters"]->Fill(RECOLxy_interactionVertex,  RECOErrorLxy_interactionVertex);
		if(RECOErrorLxy_interactionVertex<0.1)histos_th2f["h2_RECOBackground_AntiS_lxy_deltaRDaughters_cut_error_lxy"]->Fill(RECOLxy_interactionVertex,  RECODeltaRDaughters);
		if(RECOLxy_interactionVertex>1.9)histos_th2f["h2_RECOBackground_AntiS_deltaRDaughters_error_lxy_cut_lxy"]->Fill(RECODeltaRDaughters,  RECOErrorLxy_interactionVertex);

	}
*/

	histos_th1f["h_GENRECO_All_AntiS_pt"]->Fill(GENAntiS->pt());	
	histos_th1f["h_GENRECO_All_AntiS_eta"]->Fill(GENAntiS->eta());	
	histos_th1f["h_GENRECO_All_AntiS_phi"]->Fill(GENAntiS->phi());	
	histos_th2f["h2_GENRECO_All_AntiS_vx_vy"]->Fill(GENAntiS->vx(),GENAntiS->vy());
	histos_th2f["h2_GENRECO_All_AntiS_vx_vz"]->Fill(GENAntiS->vx(),GENAntiS->vz());
	histos_th1f["h_GENRECO_All_AntiS_lxy"]->Fill(GENLxy);	
	histos_th1f["h_GENRECO_All_AntiS_lxy_interactionVertex"]->Fill(GENLxy_interactionVertex);	
	histos_th1f["h_GENRECO_All_AntiS_vz"]->Fill(GENAntiS->vz());	
	histos_th1f["h_GENRECO_All_AntiS_vz_interactionVertex"]->Fill(GENAntiSInteractionVertex.Z());	
	histos_th1f["h_GENRECO_All_AntiS_dxy"]->Fill(GENdxy);	
  }
}

bool AnalyzerGEN::isTpGrandDaughterAntiS(TrackingParticleCollection const & TPColl, const TrackingParticle& tp){
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

int AnalyzerGEN::getDaughterParticlesTypes(const reco::Candidate * genParticle){
	int pdgIdDaug0 = genParticle->daughter(0)->pdgId();
	int pdgIdDaug1 = genParticle->daughter(1)->pdgId();
	int returnCode = -1;
	if(abs(pdgIdDaug0) == AnalyzerAllSteps::pdgIdPosPion && abs(pdgIdDaug1) == AnalyzerAllSteps::pdgIdPosPion)returnCode = 1;//this is the correct decay mode for Ks to be RECO
	else if((pdgIdDaug0 == AnalyzerAllSteps::pdgIdAntiProton && pdgIdDaug1 == AnalyzerAllSteps::pdgIdPosPion) || (pdgIdDaug1 == AnalyzerAllSteps::pdgIdAntiProton && pdgIdDaug0 == AnalyzerAllSteps::pdgIdPosPion))returnCode = 2;//this is the correct decay mode for an antiLambda to get RECO
	else if((pdgIdDaug0 == AnalyzerAllSteps::pdgIdKs && pdgIdDaug1 == AnalyzerAllSteps::pdgIdAntiLambda) ||(pdgIdDaug1 == AnalyzerAllSteps::pdgIdKs && pdgIdDaug0 == AnalyzerAllSteps::pdgIdAntiLambda)) returnCode = 3;//this is the correct decay mode for an antiS

	return returnCode;

}

double AnalyzerGEN::openings_angle(reco::Candidate::Vector momentum1, reco::Candidate::Vector momentum2){
  double opening_angle = TMath::ACos((momentum1.Dot(momentum2))/(pow(momentum1.Mag2()*momentum2.Mag2(),0.5)));
  return opening_angle;
}

double AnalyzerGEN::deltaR(double phi1, double eta1, double phi2, double eta2){
	double deltaPhi = reco::deltaPhi(phi1,phi2);
	double deltaEta = eta1-eta2;
	return pow(deltaPhi*deltaPhi+deltaEta*deltaEta,0.5);
}


double AnalyzerGEN::lxy(TVector3 v1, TVector3 v2){
	double x1 = v1.X();
	double x2 = v2.X();
	double y1 = v1.Y();
	double y2 = v2.Y();
	return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

double AnalyzerGEN::lxyz(TVector3 v1, TVector3 v2){
	double x1 = v1.X();
	double x2 = v2.X();
	double y1 = v1.Y();
	double y2 = v2.Y();
	double z1 = v1.Z();
	double z2 = v2.Z();
	return sqrt(pow(x1-x2,2)+pow(y1-y2,2)+pow(z1-z2,2));
}


TVector3 AnalyzerGEN::PCA_line_point(TVector3 Point_line, TVector3 Vector_along_line, TVector3 Point){
   //first move the vector along the line to the starting point of Point_line
   double normalise = sqrt(Vector_along_line.X()*Vector_along_line.X()+Vector_along_line.Y()*Vector_along_line.Y()+Vector_along_line.Z()*Vector_along_line.Z());
   TVector3 n(Vector_along_line.X()/normalise,Vector_along_line.Y()/normalise,Vector_along_line.Z()/normalise);
   TVector3 a = Point_line;
   TVector3 p = Point;

   //see https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line (Vector formulation)
   TVector3 vector_PCA = (a-p)-((a-p)*n)*n;
   return vector_PCA ;
}

double AnalyzerGEN::dxy_signed_line_point(TVector3 Point_line_in, TVector3 Vector_along_line_in, TVector3 Point_in){

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

double AnalyzerGEN::std_dev_lxy(double vx, double vy, double vx_var, double vy_var, double bx_x, double bx_y, double bx_x_var, double bx_y_var){

        double lxy_std_dev_nominator = pow(vx-bx_x,2)*(vx_var+bx_x_var) + pow(vy-bx_y,2)*(vy_var+bx_y_var);
        double lxy_std_dev_denominator = pow(vx-bx_x,2) + pow(vy-bx_y,2);
        double lxy_b_std_dev = sqrt(lxy_std_dev_nominator/lxy_std_dev_denominator);
        return lxy_b_std_dev;

}

//function to return the cos of the angle between the momentum of the particle and it's displacement vector. This is for a V0 particle, so you need the V0 to decay to get it's interaction vertex
double AnalyzerGEN::XYpointingAngle(const reco::Candidate  * particle, TVector3 beamspot){
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

double AnalyzerGEN::CosOpeningsAngle(TVector3 vec1, TVector3 vec2){

  double nom = vec1.X()*vec2.X()+vec1.Y()*vec2.Y()+vec1.Z()*vec2.Z();
  double denom = sqrt(vec1.X()*vec1.X()+vec1.Y()*vec1.Y()+vec1.Z()*vec1.Z())*sqrt(vec2.X()*vec2.X()+vec2.Y()*vec2.Y()+vec2.Z()*vec2.Z());
  return nom/denom;
	
}

void AnalyzerGEN::endJob()
{
}

AnalyzerGEN::~AnalyzerGEN()
{
}


DEFINE_FWK_MODULE(AnalyzerGEN);
