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
     histos_th1f["h_RECO_AntiS_m"] = dir_RECO_AntiS_Kinematics.make<TH1F>(b+"h_RECO_AntiS_m", "; #bar{S} m (GeV); #entries ",200,0,20);
    
     TFileDirectory dir_RECO_AntiS_cut_variables_distributions = dir_RECO_AntiS.mkdir("RECO_AntiS_cut_variables_distributions");
     histos_th1f["h_RECO_AntiS_lxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_lxy", "; #bar{S} lxy(beamspot, #bar{S} vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_RECO_AntiS_error_lxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_error_lxy", "; #bar{S} error lxy(beamspot, #bar{S} vertex) (cm); #entries ",200,0,10);
     histos_th1f["h_RECO_AntiS_deltaRDaughters"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_deltaRDaughters", "; #bar{S} #DeltaR (Ks,#bar{#Lambda}); #entries ",100,0,10);
     histos_th1f["h_RECO_AntiS_deltaPhiDaughters"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_deltaPhiDaughters", "; #bar{S} #Delta#Phi (Ks,#bar{#Lambda}); #entries ",100,-5,5);
     histos_th1f["h_RECO_AntiS_openingsAngleDaughters"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_openingsAngleDaughters", ";S or #bar{S} openings angle (Ks,#Lambda or #bar{#Lambda}); #entries ",80,-4,4);
     histos_th2f["h2_RECO_AntiS_corr_dxy_daughters"] = dir_RECO_AntiS_cut_variables_distributions.make<TH2F>(b+"h2_RECO_AntiS_corr_dxy_daughters", "; dxy Ks; dxy #bar{#Lambda}",100,-10,10,100,-10,10);
     histos_th2f["h2_RECO_AntiS_corr_dz_daughters"] = dir_RECO_AntiS_cut_variables_distributions.make<TH2F>(b+"h2_RECO_AntiS_corr_dz_daughters", "; dz Ks; dz #bar{#Lambda}",200,-100,100,200,-100,100);
     histos_th1f["h_RECO_AntiS_dxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_dxy", ";dxy S or #bar{S}; #entries ",100,-10,10);
     histos_th1f["h_RECO_AntiS_dxy_over_lxy"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_dxy_over_lxy", ";dxy S or #bar{S}/lxy interaction vertex; #entries ",100,-10,10);
     histos_th1f["h_RECO_AntiS_dz"] = dir_RECO_AntiS_cut_variables_distributions.make<TH1F>(b+"h_RECO_AntiS_dz", ";dxy S or #bar{S}; #entries ",100,-10,10);


     TFileDirectory dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass = dir_RECO_AntiS.mkdir("RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass");
     histos_th1f["h_RECO_AntiS_RECO_dxy_antiS_cut_lxy_error_lxy_mass"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass.make<TH1F>(b+"h_RECO_AntiS_RECO_dxy_antiS_cut_lxy_error_lxy_mass",";dxy S or #bar{S};#Entries",100,-10,10);
     histos_th1f["h_RECO_AntiS_RECO_dxy_over_lxy_antiS_cut_lxy_error_lxy_mass"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass.make<TH1F>(b+"h_RECO_AntiS_RECO_dxy_over_lxy_antiS_cut_lxy_error_lxy_mass",";dxy S or #bar{S}/lxy interaction vertex;#Entries",1000,-10,10);
     histos_th2f["h2_RECO_AntiS_RECO_dxy_vs_dxy_over_lxy_antiS_cut_lxy_error_lxy_mass"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass.make<TH2F>(b+"h2_RECO_AntiS_RECO_dxy_vs_dxy_over_lxy_antiS_cut_lxy_error_lxy_mass",";dxy S or #bar{S}/lxy interaction vertex; dxy S or #bar{S};#Entries",200,-10,10,200,-10,10);
     histos_th1f["h_RECO_AntiS_RECO_dz_antiS_cut_lxy_error_lxy_mass"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass.make<TH1F>(b+"h_RECO_AntiS_RECO_dz_antiS_cut_lxy_error_lxy_mass",";dz S or #bar{S};#Entries",100,-10,10);
     histos_th1f["h_RECO_AntiS_RECO_mass_antiS_cut_lxy_error_lxy_mass"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass.make<TH1F>(b+"h_RECO_AntiS_RECO_mass_antiS_cut_lxy_error_lxy_mass",";mass S or #bar{S};#Entries",1000,0,10);
     histos_th1f["h_RECO_AntiS_dxyAntiSPVmin"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass.make<TH1F>(b+"h_RECO_AntiS_dxyAntiSPVmin","; min dxy (#bar{S},PV);#Entries",400,-2,2);
     histos_th1f["h_RECO_AntiS_dzAntiSPVmin"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass.make<TH1F>(b+"h_RECO_AntiS_dzAntiSPVmin",";  dz (#bar{S},PV) for min dxy (#bar{S},PV);#Entries",400,-2,2);



     TFileDirectory dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh = dir_RECO_AntiS.mkdir("RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh");
     histos_th1f["h_RECO_AntiS_corr_dxy_Ks_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_corr_dxy_Ks_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dxy(Ks); #entries ",100,-10,10);
     histos_th1f["h_RECO_AntiS_corr_dz_Ks_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_corr_dz_Ks_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dz(Ks); #entries ",1000,-100,100);
     histos_th1f["h_RECO_AntiS_corr_dxy_AntiL_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_corr_dxy_AntiL_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dxy(#Lambda or #bar{#Lambda});  #entries ",100,-10,10);
     histos_th1f["h_RECO_AntiS_corr_dz_AntiL_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_corr_dz_AntiL_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dz(#Lambda or #bar{#Lambda}); #entries ",1000,-100,100);




     histos_th2f["h2_RECO_AntiS_corr_dxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_corr_dxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dxy(Ks)/lxy(Ks); dxy(#Lambda or #bar{#Lambda})/lxy(#Lambda or #bar{#Lambda}); #entries ",100,-10,10,100,-10,10);
     histos_th2f["h2_RECO_AntiS_corr_dxy_sign_dotProdLxydxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_corr_dxy_sign_dotProdLxydxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh",";|dxy(Ks).|sign(#vec{lxy(Ks)}.#vec{dxy(Ks)});  |dxy(#bar{#Lambda} or #Lambda).|sign(#vec{lxy(#bar{#Lambda} or #Lambda)}.#vec{dxy(#bar{#Lambda} or #Lambda)})  ;#Entries",100,-10,10,100,-10,10);
     histos_th2f["h2_RECO_AntiS_corr_dxy_sign_dotProdptdxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_corr_dxy_sign_dotProdptdxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh",";|dxy(Ks).|sign(#vec{pt(Ks)}.#vec{dxy(Ks)});  |dxy(#bar{#Lambda} or #Lambda).|sign(#vec{pt(#bar{#Lambda} or #Lambda)}.#vec{dxy(#bar{#Lambda} or #Lambda)})  ;#Entries",100,-10,10,100,-10,10);

     histos_th2f["h2_RECO_AntiS_corr_dxy_over_lxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_corr_dxy_over_lxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dxy(Ks)/lxy(Ks); dxy(#Lambda or #bar{#Lambda})/lxy(#Lambda or #bar{#Lambda}); #entries ",100,-10,10,100,-10,10);
     histos_th2f["h2_RECO_AntiS_corr_dz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_corr_dz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dz(Ks); dz(#Lambda or #bar{#Lambda}); #entries ",200,-100,100,200,-100,100);
     histos_th2f["h2_RECO_AntiS_corr_dxyz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_corr_dxyz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dxyz(Ks); dxyz(#Lambda or #bar{#Lambda}); #entries ",200,-100,100,200,-100,100);

     histos_th2f["h2_RECO_AntiS_dxy_Ks_vs_lxy_Ks_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_dxy_Ks_vs_lxy_Ks_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";lxy(Ks); dxy(Ks); #entries ",200,-100,100,200,-10,10);
     histos_th2f["h2_RECO_AntiS_dxy_AntiL_vs_lxy_AntiL_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_dxy_AntiL_vs_lxy_AntiL_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";lxy(#Lambda or #bar{#Lambda}); dxy(#Lambda or #bar{#Lambda}); #entries ",200,-100,100,200,-10,10);

     histos_th2f["h2_RECO_AntiS_dxy_Ks_dz_Ks_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_dxy_Ks_dz_Ks_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dxy(Ks); dz(Ks); #entries ",100,-10,10,200,-100,100);
     histos_th2f["h2_RECO_AntiS_dxy_L_dz_L_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_dxy_L_dz_L_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dxy(#Lambda or #bar{#Lambda}); dz(#Lambda or #bar{#Lambda}); #entries ",100,-10,10,200,-100,100);
     histos_th2f["h2_RECO_AntiS_dxy_Ks_dz_L_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_dxy_Ks_dz_L_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dxy(Ks); dz(#Lambda or #bar{#Lambda}); #entries ",100,-10,10,200,-100,100);
     histos_th2f["h2_RECO_AntiS_dxy_L_dz_Ks_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_dxy_L_dz_Ks_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";dxy(#Lambda or #bar{#Lambda}); dz(Ks); #entries ",100,-10,10,200,-100,100);


     histos_th1f["h_RECO_AntiS_relDiff_RECO_dxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_relDiff_RECO_dxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh",";#frac{|dxy(Ks)|-|dxy(#bar{#Lambda})|}{|dxy(Ks)|+|dxy(#bar{#Lambda})|};#Entries",500,-10,10);
     histos_th1f["h_RECO_AntiS_relDiff_RECO_dz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_relDiff_RECO_dz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh",";#frac{|dz(Ks)|-|dz(#bar{#Lambda})|}{|dz(Ks)|+|dz(#bar{#Lambda})|};#Entries",500,-10,10);
     histos_th1f["h_RECO_AntiS_relDiff_RECO_dxyz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_relDiff_RECO_dxyz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh",";#frac{|dxyz(Ks)|-|dxyz(#bar{#Lambda})|}{|dxyz(Ks)|+|dxyz(#bar{#Lambda})|};#Entries",500,-10,10);
     histos_th1f["h_RECO_AntiS_RECO_vertexNormalizedChi2_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_RECO_vertexNormalizedChi2_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh",";#Chi^{2}/ndof S or #bar{S} interaction vertex;#Entries",300,0,15);
     histos_th2f["h2_RECO_AntiS_deltaPhiDeltaEta_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_deltaPhiDeltaEta_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh",";#Delta #Phi(Ks,#Lambda or #bar{Lambda}); #Delta #eta(Ks,#Lambda or #bar{Lambda});#Entries",80,-4,4,80,-4,4);
     histos_th1f["h_RECO_AntiS_openingsAngleDaughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_openingsAngleDaughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";openings angle(Ks, #Lambda or #bar{#Lambda}); #entries ",80,-4,4);
     histos_th2f["h2_RECO_AntiS_deltaEta_openingsAngleDaughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH2F>(b+"h2_RECO_AntiS_deltaEta_openingsAngleDaughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";|#Delta#eta(Ks, #Lambda or #bar{#Lambda})|;openings angle(Ks, #Lambda or #bar{Lambda}); #entries ",40,0,4,40,0,4);
     histos_th1f["h_RECO_AntiS_deltaRDaughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_deltaRDaughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";#DeltaR(Ks, #Lambda or #bar{#Lambda}); #entries ",100,0,10);
     histos_th1f["h_RECO_AntiS_pt_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_pt_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";pt S or #bar{S}; #entries ",100,0,10);
     histos_th1f["h_RECO_AntiS_Ks_pt_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_Ks_pt_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";pt Ks; #entries ",100,0,10);
     histos_th1f["h_RECO_AntiS_AntiL_pt_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_AntiL_pt_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";pt #Lambda or #bar{Lambda}; #entries ",100,0,10);
     histos_th1f["h_RECO_AntiS_mass_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh.make<TH1F>(b+"h_RECO_AntiS_mass_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";mass S or #bar{S}; #entries ",2000,0,20);


     TFileDirectory dir_RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_daughters= dir_RECO_AntiS.mkdir("RECO_AntiS_cut_variables_distributions_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_daughters");

     TFileDirectory dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters= dir_RECO_AntiS.mkdir("RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters");
     histos_th1f["h_RECO_AntiS_eta_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_eta_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters", "; #bar{S} #eta; #entries ",200,-10,10);
     histos_th1f["h_RECO_AntiS_phi_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_phi_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters", "; #bar{S} #phi (rad); #entries ",200,-4,4);
     histos_th2f["h2_RECO_AntiS_vx_vy_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH2F>(b+"h2_RECO_AntiS_vx_vy_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters", "; #bar{S} vx (cm); #bar{S} vy (cm)",400,-200,200,400,-200,200);
     histos_th2f["h2_RECO_AntiS_vx_vz_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH2F>(b+"h2_RECO_AntiS_vx_vz_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters", "; #bar{S} vx (cm); #bar{S} vz (cm)",400,-200,200,800,-400,400);
     histos_th1f["h_RECO_AntiS_lxy_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_lxy_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters", "; #bar{S} lxy(beamspot, #bar{S} vertex) (cm); #entries ",200,0,200);
     histos_th1f["h_RECO_AntiS_vz_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_vz_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters", "; #bar{S} vz(Ks vertex) (cm); #entries ",600,-300,300);
     histos_th1f["h_RECO_AntiS_dxy_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_dxy_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters", "; #bar{S} dxy(beamspot) (cm); #entries ",400,-20,20);
     histos_th1f["h_RECO_AntiS_deltaR_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_deltaR_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters", "; #DeltaR(Ks, #Lambda or #bar{Lambda}) ; #entries ",100,0,10);
     histos_th1f["h_RECO_AntiS_mass_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_mass_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters", "; S or #bar{S} mass (GeV); #entries ",2000,0,20);
     histos_th1f["h_RECO_AntiS_dxyAntiSPVmin_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_dxyAntiSPVmin_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", "; S or #bar{S} dxy(PV with smallest dz)  (GeV); #entries ",200,-10,10);
     histos_th1f["h_RECO_AntiS_dzAntiSPVmin_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_dzAntiSPVmin_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", "; S or #bar{S} dz(PV with smallest dz)  (GeV); #entries ",200,-10,10);
     histos_th1f["h_RECO_AntiS_dzAntiSPVmin_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_dzAntiSPVmin_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", "; S or #bar{S} dz(PV with smallest dz)  (GeV); #entries ",200,-10,10);
     histos_th1f["h_RECO_AntiS_pz_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"] = dir_RECO_AntiS_kinematics_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters.make<TH1F>(b+"h_RECO_AntiS_pz_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh", ";pz S or #bar{S}; #entries ",200,-50,50);
 
     TFileDirectory dir_RECO_AntiS_cutFlows = dir_RECO_AntiS.mkdir("RECO_AntiS_cutFlows");
     histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"] = dir_RECO_AntiS_cutFlows.make<TH1F>(b+"h_RECO_AntiS_BackgroundCuts_cutFlow", "; 0=all,1=#sigma(lxy)<0.1,2=Lxy>1.9,3=m>0,4=#DeltaR(daug0,daug1)>1,5=#DeltaR(daug0,daug1)<6,6=|dxy_{daug0}|>1 and |dxy_{daug1}|>1,7=|dz_{daug0}|>10 and |dz_{daug1}|>10,8=0<dxy(S or #bar{S})<0.4); #entries ",10,0,10); 
     histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow_cut_lxy_error_lxy_mass"] = dir_RECO_AntiS_cutFlows.make<TH1F>(b+"h_RECO_AntiS_BackgroundCuts_cutFlow_cut_lxy_error_lxy_mass", "; 0=all surviving lxy, #sigma(lxy), #DeltaR, m cut,1=0 && 0<dxy(S or #bar{S})<0.4 ,2= 1 && |dxy_{daug0}|>0.2 and |dxy_{daug1}|>0.2,3= 2 && |dz_{daug0}|>5 and |dz_{daug1}|>5; #entries ",10,0,10); 

     TFileDirectory dir_RECO_AntiS_deltaRInvestigation = dir_RECO_AntiS.mkdir("RECO_AntiS_deltaRInvestigation");
     histos_th1f["h_RECO_AntiS_deltaR_cut_error_lxy"] = dir_RECO_AntiS_deltaRInvestigation.make<TH1F>(b+"h_RECO_AntiS_deltaR_cut_error_lxy", "; #Delta R(Ks, #Lambda or #bar{#Lambda}); #entries ",100,0,10); 
     histos_th1f["h_RECO_AntiS_deltaR_cut_lxy"] = dir_RECO_AntiS_deltaRInvestigation.make<TH1F>(b+"h_RECO_AntiS_deltaR_cut_lxy", "; #Delta R(Ks, #Lambda or #bar{#Lambda}); #entries ",100,0,10); 
     histos_th1f["h_RECO_AntiS_deltaR_cut_mass"] = dir_RECO_AntiS_deltaRInvestigation.make<TH1F>(b+"h_RECO_AntiS_deltaR_cut_mass", "; #Delta R(Ks, #Lambda or #bar{#Lambda}); #entries ",100,0,10); 

    

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
	if(antiS->charge()==chargeAntiProton)FillHistosRECOAntiS(antiS, beamspot, beamspotVariance, iEvent.id().event(), h_offlinePV);
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

void AnalyzerRECO::FillHistosRECOAntiS(const reco::VertexCompositeCandidate * RECOAntiS, TVector3 beamspot, TVector3 beamspotVariance, int eventId, edm::Handle<vector<reco::Vertex>> h_offlinePV){
	//calculate some kinematic variables of the for the S:
	TVector3 AntiSInteractionVertex(RECOAntiS->vx(),RECOAntiS->vy(),RECOAntiS->vz());
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
	double dz_AntiS = AnalyzerAllSteps::dz_line_point(AntiSInteractionVertex,AntiSMomentumVertex,beamspot);

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

	//plot kinematics of *all* the RECO S candidates
	histos_th1f["h_RECO_AntiS_pt"]->Fill(RECOAntiS->pt());	
	histos_th1f["h_RECO_AntiS_eta"]->Fill(RECOAntiS->eta());	
	histos_th1f["h_RECO_AntiS_phi"]->Fill(RECOAntiS->phi());	
	histos_th2f["h2_RECO_AntiS_vx_vy"]->Fill(RECOAntiS->vx(),RECOAntiS->vy());
	histos_th2f["h2_RECO_AntiS_vx_vz"]->Fill(RECOAntiS->vx(),RECOAntiS->vz());
	histos_th1f["h_RECO_AntiS_vz"]->Fill(RECOAntiS->vz());	
	histos_th1f["h_RECO_AntiS_dxy"]->Fill(dxy);	
	histos_th1f["h_RECO_AntiS_m"]->Fill(massAntiS);

	//the stuff we cut on for *all* the RECO S candidates:
	histos_th1f["h_RECO_AntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_RECO_AntiS_error_lxy"]->Fill(error_Lxy);
	histos_th1f["h_RECO_AntiS_deltaRDaughters"]->Fill(deltaRDaughters);
	histos_th1f["h_RECO_AntiS_deltaPhiDaughters"]->Fill(deltaPhiDaughters);
	histos_th1f["h_RECO_AntiS_openingsAngleDaughters"]->Fill(openingsAngleDaughters);
	histos_th2f["h2_RECO_AntiS_corr_dxy_daughters"]->Fill(dxy_daughter0,dxy_daughter1);
	histos_th2f["h2_RECO_AntiS_corr_dz_daughters"]->Fill(dz_daughter0,dz_daughter1);

	//fill a cut flow histogram
	histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"]->Fill(0);	
	if(error_Lxy < 0.1) histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"]->Fill(1);
	if(Lxy>1.9)histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"]->Fill(2);
	if(massAntiS > 0.)histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"]->Fill(3);
	if(deltaRDaughters > 1.)histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"]->Fill(4);
	if(deltaRDaughters < 6.)histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"]->Fill(5);
	if(abs(dxy_daughter0) > 1 && abs(dxy_daughter1) > 1)histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"]->Fill(6);
	if(abs(dz_daughter0) > 10 && abs(dz_daughter1) > 10)histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"]->Fill(7);
	if(dxy_AntiS > 0 && dxy_AntiS < 0.4)histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow"]->Fill(8);

	//try to find out which of the fundamental background cuts get rid of the deltaR=0 peak--> it appears to be the M>0 cut
	if(error_Lxy < 0.1) histos_th1f["h_RECO_AntiS_deltaR_cut_error_lxy"]->Fill(deltaRDaughters);	
	if(Lxy < 1.9) histos_th1f["h_RECO_AntiS_deltaR_cut_lxy"]->Fill(deltaRDaughters);	
	if(massAntiS > 0.) histos_th1f["h_RECO_AntiS_deltaR_cut_mass"]->Fill(deltaRDaughters);

	
	//start investigating background discriminating variables
	//the stuff we cut on for *all* the RECO S candidates:
	histos_th1f["h_RECO_AntiS_lxy"]->Fill(Lxy);	
	histos_th1f["h_RECO_AntiS_error_lxy"]->Fill(error_Lxy);
	histos_th1f["h_RECO_AntiS_deltaRDaughters"]->Fill(deltaRDaughters);
	histos_th1f["h_RECO_AntiS_deltaPhiDaughters"]->Fill(deltaPhiDaughters);
	histos_th1f["h_RECO_AntiS_openingsAngleDaughters"]->Fill(openingsAngleDaughters);
	histos_th2f["h2_RECO_AntiS_corr_dxy_daughters"]->Fill(dxy_daughter0,dxy_daughter1);
	histos_th2f["h2_RECO_AntiS_corr_dz_daughters"]->Fill(dz_daughter0,dz_daughter1);
	histos_th1f["h_RECO_AntiS_dxy"]->Fill(dxy_AntiS);
	histos_th1f["h_RECO_AntiS_dxy_over_lxy"]->Fill(dxy_AntiS/Lxy);
	histos_th1f["h_RECO_AntiS_dz"]->Fill(dz_AntiS);

	//now start applying the background cuts and see what is important, start with the "fundamental" background cuts
	if(	Lxy > AnalyzerAllSteps::MinLxyCut && 
		error_Lxy < AnalyzerAllSteps::MaxErrorLxyCut && 
		massAntiS > AnalyzerAllSteps::MinErrorMassCut){

                histos_th1f["h_RECO_AntiS_RECO_dxy_antiS_cut_lxy_error_lxy_mass"]->Fill(dxy_AntiS);
                histos_th1f["h_RECO_AntiS_RECO_dxy_over_lxy_antiS_cut_lxy_error_lxy_mass"]->Fill(dxy_AntiS/Lxy);
                histos_th2f["h2_RECO_AntiS_RECO_dxy_vs_dxy_over_lxy_antiS_cut_lxy_error_lxy_mass"]->Fill(dxy_AntiS/Lxy,dxy_AntiS);
                histos_th1f["h_RECO_AntiS_RECO_dz_antiS_cut_lxy_error_lxy_mass"]->Fill(dz_AntiS);
                histos_th1f["h_RECO_AntiS_RECO_mass_antiS_cut_lxy_error_lxy_mass"]->Fill(massAntiS);

		histos_th1f["h_RECO_AntiS_dxyAntiSPVmin"]->Fill(dxyAntiSPVmin);
                histos_th1f["h_RECO_AntiS_dzAntiSPVmin"]->Fill(dzAntiSPVmin);

		//fill a cut flow histogram after the "fundamental" background cuts:
		histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow_cut_lxy_error_lxy_mass"]->Fill(0);

		//now cut on the dxy of the S/antiS, with this cut you keep almost all the signal, but you cut strongly on background (background can have large dxy because it are random overlaps). Cut also on the deltaPhi. The m>0 cut cuts away the deltaR peak at 0, buuuuut there is this strange correlation in deltaphi-deltaeta where you have this band at deltaphi = 0 at high deltaeta, which is most likely attributated to the Ks and Lambdas from different vertices. 
		if(	abs(deltaPhiDaughters) > AnalyzerAllSteps::MinAbsDeltaPhiDaughtersCut && 
			abs(deltaPhiDaughters) < AnalyzerAllSteps::MaxAbsDeltaPhiDaughtersCut && 
			dxy_AntiS/Lxy > AnalyzerAllSteps::MinDxyOverLxyCut && 
			dxy_AntiS/Lxy < AnalyzerAllSteps::MaxDxyOverLxyCut && 
			RECOAntiS->vertexNormalizedChi2() < AnalyzerAllSteps::MaxNormChi2Cut && 
			openingsAngleDaughters < AnalyzerAllSteps::MaxOpeningsAngleCut && 
			abs(deltaEtaDaughters) < AnalyzerAllSteps::MaxAbsDeltaEtaDaughCut && 
			(dxy_daughter0 > AnalyzerAllSteps::DxyKsExclusionRangeMaxCut || dxy_daughter0 < AnalyzerAllSteps::DxyKsExclusionRangeMinCut) && 
			RECOAntiS->daughter(1)->pt() > AnalyzerAllSteps::MinLambdaPtCut &&
			abs(dzAntiSPVmin)<AnalyzerAllSteps::dzAntiSPVminCut &&
			abs(RECOAntiS->vz()) > AnalyzerAllSteps::vzAntiSInteractionVertexCut &&
			abs(RECOAntiS->eta())> AnalyzerAllSteps::antiSEtaCut &&
			(dxy_daughter1>AnalyzerAllSteps::DxyAntiLExclusionRangeMaxCut || dxy_daughter1<AnalyzerAllSteps::DxyAntiLExclusionRangeMinCut)){


			//if not all background is gone yet, you can still cut on the dxy and dz of the Ks and Lambda, so investigate how they look:
			histos_th1f["h_RECO_AntiS_corr_dxy_Ks_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxy_daughter0);
			histos_th1f["h_RECO_AntiS_corr_dz_Ks_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dz_daughter0);
			histos_th1f["h_RECO_AntiS_corr_dxy_AntiL_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxy_daughter1);
			histos_th1f["h_RECO_AntiS_corr_dz_AntiL_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dz_daughter1);

			histos_th2f["h2_RECO_AntiS_corr_dxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxy_daughter0,dxy_daughter1);
			histos_th2f["h2_RECO_AntiS_corr_dxy_sign_dotProdLxydxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(abs(dxy_daughter0)*signLxyDotdxy_daughter0,abs(dxy_daughter1)*signLxyDotdxy_daughter1);
			histos_th2f["h2_RECO_AntiS_corr_dxy_sign_dotProdptdxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(abs(dxy_daughter0)*signPtDotdxy_daughter0,abs(dxy_daughter1)*signPtDotdxy_daughter1);
			histos_th2f["h2_RECO_AntiS_corr_dxy_over_lxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxy_daughter0/Lxy,dxy_daughter1/Lxy);
			histos_th2f["h2_RECO_AntiS_corr_dz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dz_daughter0,dz_daughter1);
			histos_th2f["h2_RECO_AntiS_corr_dxyz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxyz_daughter0,dxyz_daughter1);

			histos_th2f["h2_RECO_AntiS_dxy_Ks_vs_lxy_Ks_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(Lxy,dxy_daughter0);
			histos_th2f["h2_RECO_AntiS_dxy_AntiL_vs_lxy_AntiL_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(Lxy,dxy_daughter1);
			
			
			histos_th2f["h2_RECO_AntiS_dxy_Ks_dz_Ks_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxy_daughter0,dz_daughter0);
			histos_th2f["h2_RECO_AntiS_dxy_L_dz_L_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxy_daughter1,dz_daughter1);
			histos_th2f["h2_RECO_AntiS_dxy_Ks_dz_L_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxy_daughter0,dz_daughter1);
			histos_th2f["h2_RECO_AntiS_dxy_L_dz_Ks_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxy_daughter1,dz_daughter0);
			

			histos_th1f["h_RECO_AntiS_relDiff_RECO_dxy_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(relDiff_dxy_daughters);
			histos_th1f["h_RECO_AntiS_relDiff_RECO_dz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(relDiff_dz_daughters);
			histos_th1f["h_RECO_AntiS_relDiff_RECO_dxyz_daughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(relDiff_dxyz_daughters);
			histos_th1f["h_RECO_AntiS_RECO_vertexNormalizedChi2_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(RECOAntiS->vertexNormalizedChi2());
			histos_th2f["h2_RECO_AntiS_deltaPhiDeltaEta_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(deltaPhiDaughters,deltaEtaDaughters);
			histos_th1f["h_RECO_AntiS_openingsAngleDaughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(openingsAngleDaughters);
			histos_th2f["h2_RECO_AntiS_deltaEta_openingsAngleDaughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(abs(deltaEtaDaughters),openingsAngleDaughters);
			histos_th1f["h_RECO_AntiS_deltaRDaughters_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(deltaRDaughters);
			//look at the momenta of what is remaining:			
			histos_th1f["h_RECO_AntiS_pt_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(RECOAntiS->pt());
			histos_th1f["h_RECO_AntiS_Ks_pt_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(RECOAntiS->daughter(0)->pt());
			histos_th1f["h_RECO_AntiS_AntiL_pt_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(RECOAntiS->daughter(1)->pt());

			histos_th1f["h_RECO_AntiS_mass_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(massAntiS);

			
		
			histos_th1f["h_RECO_AntiS_BackgroundCuts_cutFlow_cut_lxy_error_lxy_mass"]->Fill(1);

			histos_th1f["h_RECO_AntiS_eta_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"]->Fill(RECOAntiS->eta());	
			histos_th1f["h_RECO_AntiS_phi_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"]->Fill(RECOAntiS->phi());	
			histos_th2f["h2_RECO_AntiS_vx_vy_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"]->Fill(RECOAntiS->vx(),RECOAntiS->vy());
			histos_th2f["h2_RECO_AntiS_vx_vz_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"]->Fill(RECOAntiS->vx(),RECOAntiS->vz());
			histos_th1f["h_RECO_AntiS_lxy_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"]->Fill(Lxy);	
			histos_th1f["h_RECO_AntiS_vz_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"]->Fill(RECOAntiS->vz());	
			histos_th1f["h_RECO_AntiS_dxy_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"]->Fill(dxy);	
			histos_th1f["h_RECO_AntiS_deltaR_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"]->Fill(deltaRDaughters);	
			histos_th1f["h_RECO_AntiS_mass_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh_dxy_dz_daughters"]->Fill(massAntiS);

			histos_th1f["h_RECO_AntiS_dxyAntiSPVmin_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dxyAntiSPVmin);
                	histos_th1f["h_RECO_AntiS_dzAntiSPVmin_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(dzAntiSPVmin);
			histos_th1f["h_RECO_AntiS_pz_cut_lxy_error_lxy_mass_dxy_over_lxy_deltaphiDaugh_Chi2Vertex_deltaetaDaugh_openingsangleDaugh"]->Fill(RECOAntiS->pz());



		}
	}
}

void AnalyzerRECO::endJob()
{
}

AnalyzerRECO::~AnalyzerRECO()
{
}


DEFINE_FWK_MODULE(AnalyzerRECO);
