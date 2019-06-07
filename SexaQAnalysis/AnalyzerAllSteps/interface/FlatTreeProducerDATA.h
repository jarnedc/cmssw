#ifndef FlatTreeProducerDATA_h
#define FlatTreeProducerDATA_h
 
#include "AnalyzerAllSteps.h"
using namespace edm;
using namespace std; 
class FlatTreeProducerDATA : public edm::EDAnalyzer
 {
  public:
    explicit FlatTreeProducerDATA(edm::ParameterSet const& cfg);
    virtual ~FlatTreeProducerDATA();
    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    void FillRECOAntiS(const reco::VertexCompositeCandidate * RECOAntiS, TVector3 beamspot,TVector3 beamspotVariance, int eventId, edm::Handle<vector<reco::Vertex>> h_offlinePV);

  private:
    bool m_lookAtAntiS;

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
    edm::InputTag m_offlinePVTag;
    edm::InputTag m_sCandsTag;
    edm::InputTag m_V0KsTag;
    edm::InputTag m_V0LTag;


    edm::EDGetTokenT<reco::BeamSpot> m_bsToken;
    edm::EDGetTokenT<vector<reco::Vertex>> m_offlinePVToken;
    edm::EDGetTokenT<vector<reco::VertexCompositeCandidate> > m_sCandsToken;
    edm::EDGetTokenT<vector<reco::VertexCompositeCandidate> > m_V0KsToken;
    edm::EDGetTokenT<vector<reco::VertexCompositeCandidate> > m_V0LToken;
   
    TTree* _tree;   

    //definition of variablew which should go to tree
    std::vector<double> _S_charge;
    std::vector<double> _S_lxy_interaction_vertex, _S_error_lxy_interaction_vertex,_S_mass,_S_chi2_ndof;
    std::vector<double> _S_daughters_deltaphi,_S_daughters_deltaeta,_S_daughters_openingsangle;
    std::vector<double> _S_dxy,_Ks_dxy,_Lambda_dxy;
    std::vector<double> _S_dz,_Ks_dz,_Lambda_dz;
    std::vector<double> _S_pt,_Ks_pt,_Lambda_pt;
    std::vector<double> _S_pz,_Ks_pz,_Lambda_pz;
    std::vector<double> _S_vz_interaction_vertex;
    std::vector<double> _S_vx,_S_vy,_S_vz;


     };

#endif

