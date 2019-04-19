/* \class CandViewRefSelector
 * 
 * Candidate Selector based on a configurable cut.
 * Reads a edm::View<Candidate> as input
 * and saves a OwnVector of clones.
 * Usage:
 * 
 * module selectedCands = CandViewSelector {
 *   InputTag src = myCollection
 *   string cut = "pt > 15.0"
 * };
 *
 * \author: Luca Lista, INFN
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/Candidate/interface/Candidate.h"

typedef SingleObjectSelector<
          reco::CandidateView,
  StringCutObjectSelector<reco::Candidate, true>
       > CandViewSelector;

DEFINE_FWK_MODULE(CandViewSelector);
