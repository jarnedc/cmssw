#ifndef FlatTreeProducerDATA_h
#define FlatTreeProducerDATA_h
 
#include "../../AnalyzerAllSteps/interface/AnalyzerAllSteps.h"
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
    std::vector<double> _S_px,_S_py,_S_pz;


     };

#endif

