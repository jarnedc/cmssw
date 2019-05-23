#ifndef AnalyzerRECO_h
#define AnalyzerRECO_h
 
#include "AnalyzerAllSteps.h"
using namespace edm;
using namespace std; 
class AnalyzerRECO : public edm::EDAnalyzer
 {
  public:
    explicit AnalyzerRECO(edm::ParameterSet const& cfg);
    virtual void beginJob();
    virtual void analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup);
    virtual void endJob();
    virtual ~AnalyzerRECO();
    double openings_angle(reco::Candidate::Vector momentum1, reco::Candidate::Vector momentum2);
    double deltaR(double phi1, double eta1, double phi2, double eta2);
    double lxy(TVector3 v1, TVector3 v2);
    double lxyz(TVector3 v1, TVector3 v2);
    TVector3 PCA_line_point(TVector3 Point_line, TVector3 Vector_along_line, TVector3 Point);
    double dxy_signed_line_point(TVector3 Point_line, TVector3 Vector_along_line, TVector3 Point);
    double std_dev_lxy(double vx, double vy, double vx_var, double vy_var, double bx_x, double bx_y, double bx_x_var, double bx_y_var);
    double XYpointingAngle(const reco::Candidate  * particle,TVector3 beamspot);
    double CosOpeningsAngle(TVector3 vec1, TVector3 vec2);

    bool isTpGrandDaughterAntiS(TrackingParticleCollection const & TPColl, const TrackingParticle& tp);

    int getDaughterParticlesTypes(const reco::Candidate * genParticle);

    void FillHistosPV(reco::Vertex PrimVertex, TVector3 beamspot);
    void FillHistosRECOKs(const reco::VertexCompositeCandidate * RECOKs, TVector3 beamspot);
    void FillHistosRECOLambda(const reco::VertexCompositeCandidate * RECOAntiLambda, TVector3 beamspot);
    void FillHistosRECOAntiS(const reco::VertexCompositeCandidate * RECOAntiS, TVector3 beamspot,TVector3 beamspotVariance, int eventId);


  private:
    //---- configurable parameters --------
    bool m_lookAtAntiS;
    int m_nEvent, m_nRun, m_nLumi;
    
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
   
 
    int verbose=1;
    
    TString a = ""; //"WjetsMC" "ZeroBias" "MET" "MinBiasMC" "SingleMuon"
    TString b = a + "";
    
    //--------- Histogram Declaration --------------------//
    std::map<TString, TH1F *> histos_th1f;
    std::map<TString, TH2I *> histos_th2i;
    std::map<TString, TH2F *> histos_th2f;
    std::map<TString, TH3F *> histos_th3f;
    std::map<TString, TEfficiency *> histos_teff;
    std::map<TString, TProfile *> histos_TProfile;

     };

#endif

