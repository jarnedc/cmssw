#include "G4SQLoopProcessDiscr.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VParticleChange.hh"


G4SQLoopProcessDiscr::G4SQLoopProcessDiscr(const G4String& name, G4ProcessType type)
  : G4VDiscreteProcess(name, type)
{
  fParticleChange = new G4ParticleChange();
}


G4SQLoopProcessDiscr::~G4SQLoopProcessDiscr()
{
  delete fParticleChange;
}


G4VParticleChange* G4SQLoopProcessDiscr::PostStepDoIt(const G4Track& track, const G4Step& step)
{
    G4Track * mytr = const_cast<G4Track *>(&track);
    mytr->SetPosition(posini);
//    mytr->SetGlobalTime(globaltimeini); //resetting the GlobalTime, because there is a limit of 500 for particles which travel too long
//    mytr->SetLocalTime(localtimeini); //resetting the LocalTime, because there is a limit of 500 for particles which travel too long
//    mytr->SetProperTime(propertimeini);
  
  std::cout << "G4SQLoopProcessDiscr::PostStepDoIt killing particle at   momentumdirection: " << mytr->GetMomentumDirection() << " momentumdirection eta: " << mytr->GetMomentumDirection().eta() <<   " position new " <<  mytr->GetPosition()/centimeter << " globaltime: " << mytr->GetGlobalTime()/ns <<  std::endl;
  std::cout << "G4SQLoopProcessDiscr::PostStepDoIt fParticleChange GetPostion  " << fParticleChange->GetPosition()->x() << "," << fParticleChange->GetPosition()->y() << "," << fParticleChange->GetPosition()->z() << " MomentumDirection " << fParticleChange->GetMomentumDirection()->x() << "," << fParticleChange->GetMomentumDirection()->y() << "," << fParticleChange->GetMomentumDirection()->z() << "," << " GlobalTime " << fParticleChange->GetGlobalTime() << std::endl;
  if(mytr->GetGlobalTime()/ns>4990) std::cout << "going to loose the particle because the GlobalTime is getting close to 5000" << std::endl;

  fParticleChange->Clear();
  fParticleChange->Initialize(track);

  //adding secondary antiS
  fParticleChange->SetNumberOfSecondaries(1); 
  G4DynamicParticle* replacementParticle= new G4DynamicParticle(G4AntiSQ::AntiSQ(), track.GetMomentumDirection(), track.GetKineticEnergy());
  std::cout << "G4SQLoopProcessDiscr::PostStepDoIt: 1 replacementParticle TotalEnergy: "<< replacementParticle->GetTotalEnergy()/GeV << std::endl;
  fParticleChange->AddSecondary(replacementParticle);

  //fParticleChange->AddSecondary(replacementParticle,globaltimeini);
  //fParticleChange->ProposeGlobalTime(globaltimeini);
  //fParticleChange->ProposeLocalTime(localtimeini);
  //fParticleChange->ProposeProperTime(propertimeini);

  //killing original AntiS
  fParticleChange->ProposeTrackStatus(fStopAndKill);
  //fParticleChange->ProposeWeight(track.GetWeight());
  return fParticleChange;
}


G4double G4SQLoopProcessDiscr::PostStepGetPhysicalInteractionLength(const G4Track& track, G4double previousStepSize, G4ForceCondition* condition)
{
  *condition = NotForced; 
  G4double intLength = DBL_MAX; //by default the interaction length is super large, only when outside tracker make it 0 to be sure it will do the reset to the original position
  G4Track * mytr = const_cast<G4Track *>(&track);
  if ((mytr->GetPosition().rho()>100*centimeter || // require to be "outside" the tracker
       fabs(mytr->GetPosition().z())>300*centimeter) ){
       //nreset < 100) {
       intLength = 0.0;//0 interaction length means particle will directly interact.
       ++nreset;
  } 
 // std::cout << "G4SQLoopProcessDiscr::PostStepGetPhysicalInteractionLength return: " << intLength << std::endl;
  return intLength;
}

G4double G4SQLoopProcessDiscr::GetMeanFreePath(const G4Track&,G4double,
					  G4ForceCondition*)
{
  return DBL_MAX;
} 


void G4SQLoopProcessDiscr::StartTracking(G4Track * aTrack)
{
 // std::cout << "*** STARTTRACK: momentum rho, momentum eta, rho, z "
 //           << aTrack->GetMomentum().rho()/GeV << " "
 //           << aTrack->GetMomentum().eta() << " "
 //           << aTrack->GetPosition().rho()/centimeter << " "
 //           << aTrack->GetPosition().z()/centimeter << std::endl;
  posini = aTrack->GetPosition();
  globaltimeini = aTrack->GetGlobalTime();
  localtimeini = aTrack->GetLocalTime();
  propertimeini = aTrack->GetProperTime();

  nreset = 0;
}
