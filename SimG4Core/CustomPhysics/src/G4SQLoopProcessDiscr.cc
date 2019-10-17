#include "G4SQLoopProcessDiscr.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VParticleChange.hh"


G4SQLoopProcessDiscr::G4SQLoopProcessDiscr(const G4String& name, G4ProcessType type)
  : G4VDiscreteProcess(name, type)
{
  fParticleChange = new G4ParticleChange();
  fParticleChange->ClearDebugFlag();
}


G4SQLoopProcessDiscr::~G4SQLoopProcessDiscr()
{
  delete fParticleChange;
}


G4VParticleChange* G4SQLoopProcessDiscr::PostStepDoIt(const G4Track& track, const G4Step& step)
{
    G4Track * mytr = const_cast<G4Track *>(&track);
    mytr->SetPosition(posini);
  
//  std::cout << "G4SQLoopProcessDiscr::PostStepDoIt track momentumdirection eta: " << mytr->GetMomentumDirection().eta() <<   " position " <<  mytr->GetPosition()/centimeter << " globaltime: " << mytr->GetGlobalTime()/ns <<  std::endl;
//  std::cout << "G4SQLoopProcessDiscr::PostStepDoIt fParticleChange GetPostion  " << fParticleChange->GetPosition()->x() << "," << fParticleChange->GetPosition()->y() << "," << fParticleChange->GetPosition()->z() << " MomentumDirection " << fParticleChange->GetMomentumDirection()->eta() << "," << " GlobalTime " << fParticleChange->GetGlobalTime() << std::endl;
  if(mytr->GetGlobalTime()/ns>4990) std::cout << "going to loose the particle because the GlobalTime is getting close to 5000" << std::endl;

  fParticleChange->Clear();
  fParticleChange->Initialize(track);

  //adding secondary antiS
  fParticleChange->SetNumberOfSecondaries(1); 
  G4DynamicParticle* replacementParticle= new G4DynamicParticle(G4AntiSQ::AntiSQ(), track.GetMomentumDirection(), track.GetKineticEnergy());
  fParticleChange->AddSecondary(replacementParticle,globaltimeini);

  //killing original AntiS
//  std::cout << "G4SQLoopProcessDiscr::PostStepDoIt: KILLING THE OLD PARTICLE AND CREATING A NEW ONE, eta" << mytr->GetMomentumDirection().eta() << std::endl;
  fParticleChange->ProposeTrackStatus(fStopAndKill);

  return fParticleChange;
}


G4double G4SQLoopProcessDiscr::PostStepGetPhysicalInteractionLength(const G4Track& track, G4double previousStepSize, G4ForceCondition* condition)
{
  *condition = NotForced; 
  G4double intLength = DBL_MAX; //by default the interaction length is super large, only when outside tracker make it 0 to be sure it will do the reset to the original position
  G4Track * mytr = const_cast<G4Track *>(&track);
//  if ((mytr->GetPosition().rho()>100*centimeter || // require to be "outside" the tracker
//       fabs(mytr->GetPosition().z())>300*centimeter) ){
  if(sqrt(pow(mytr->GetPosition().rho(),2))>2.5*centimeter){//have to use a spherical border for the sensitive part, because if I put a different limit in different directions the interaction rate along the pathlength will not be uniform due to this. I use the requiremnt that the particle should come back to the origin when lxyz = 292 cm, because that is the furthest point in lxyz that modules are located from the beamspot
       intLength = 0.0;//0 interaction length means particle will directly interact.
  } 
  return intLength;
}

G4double G4SQLoopProcessDiscr::GetMeanFreePath(const G4Track&,G4double,
					  G4ForceCondition*)
{
  return DBL_MAX;
} 


void G4SQLoopProcessDiscr::StartTracking(G4Track * aTrack)
{
  posini = aTrack->GetPosition();
  globaltimeini = aTrack->GetGlobalTime();

}
