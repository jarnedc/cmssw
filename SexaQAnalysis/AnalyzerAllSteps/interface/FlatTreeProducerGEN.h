#ifndef FlatTreeProducerGEN_h
#define FlatTreeProducerGEN_h
 
#include "AnalyzerAllSteps.h"
using namespace edm;
using namespace std; 
class FlatTreeProducerGEN : public edm::EDAnalyzer
 {
  public:
    explicit FlatTreeProducerGEN(edm::ParameterSet const& cfg);
    virtual ~FlatTreeProducerGEN();
    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    void FillBranchesGENAntiS(const reco::Candidate  * genParticle, TVector3 beamspot, TVector3 beamspotVariance, vector<vector<float>> v_antiS_momenta_and_itt);

  private:
    int nTotalGENS=0;
    int nTotalUniqueGenS=0;
    int nTotalGiving2DaughtersGENS=0;
    int nTotalGivingCorrectDaughtersAnd4GrandDaughtersGENS=0;
    int nTotalCorrectGENS=0;
    bool m_lookAtAntiS;
    bool m_runningOnData; 

    virtual void beginJob();
    virtual void analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup);
    virtual void endJob();
    
    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

    void Init();

    edm::Service<TFileService> m_fs;
 
    edm::InputTag m_bsTag;
    edm::InputTag m_genParticlesTag_GEN;
    edm::InputTag m_genParticlesTag_SIM_GEANT;


    edm::EDGetTokenT<reco::BeamSpot> m_bsToken;
    edm::EDGetTokenT<vector<reco::GenParticle>> m_genParticlesToken_GEN; 
    edm::EDGetTokenT<vector<reco::GenParticle>> m_genParticlesToken_SIM_GEANT; 
   
    TTree* _tree;   

    //definition of variables which should go to tree
    std::vector<float> _S_n_loops;
    std::vector<float> _S_charge;
    std::vector<float> _S_lxy_interaction_vertex, _S_lxyz_interaction_vertex,  _S_error_lxy_interaction_vertex,_S_mass,_S_chi2_ndof;
    std::vector<float> _S_daughters_deltaphi,_S_daughters_deltaeta,_S_daughters_openingsangle,_S_Ks_openingsangle,_S_Lambda_openingsangle,_S_daughters_DeltaR,_S_eta,_Ks_eta,_Lambda_eta;
    std::vector<float> _S_dxy,_Ks_dxy,_Lambda_dxy;
    std::vector<float> _S_dxy_over_lxy,_Ks_dxy_over_lxy,_Lambda_dxy_over_lxy;
    std::vector<float> _S_dz,_Ks_dz,_Lambda_dz,_S_dz_min,_Ks_dz_min,_Lambda_dz_min;
    std::vector<float> _S_pt,_Ks_pt,_Lambda_pt;
    std::vector<float> _S_pz,_Ks_pz,_Lambda_pz;
    std::vector<float> _S_vz_interaction_vertex;
    std::vector<float> _S_vx,_S_vy,_S_vz;


     };

#endif

