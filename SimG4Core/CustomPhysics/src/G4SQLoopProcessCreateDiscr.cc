#include "G4SQLoopProcessCreateDiscr.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VParticleChange.hh"
#include "G4Geantino.hh"

G4SQLoopProcessCreateDiscr::G4SQLoopProcessCreateDiscr(const G4String& name, G4ProcessType type)
  : G4VDiscreteProcess(name, type)
{
  fParticleChange = new G4ParticleChange();
}


G4SQLoopProcessCreateDiscr::~G4SQLoopProcessCreateDiscr()
{
  delete fParticleChange;
}


G4VParticleChange* G4SQLoopProcessCreateDiscr::PostStepDoIt(const G4Track& track, const G4Step& step)
{
  G4Track * mytr = const_cast<G4Track *>(&track);
 // G4ThreeVector origin(0.,0.,0.);
  mytr->SetPosition(track.GetPosition());

//  mytr->SetGlobalTime(globaltimeini); //resetting the GlobalTime, because there is a limit of 500 for particles which travel too long
//  mytr->SetLocalTime(localtimeini); //resetting the LocalTime, because there is a limit of 500 for particles which travel too long
//  mytr->SetProperTime(propertimeini);
  
  std::cout << "G4SQLoopProcessCreateDiscr::PostStepDoIt:   momentumdirection: " << mytr->GetMomentumDirection() << " momentumdirection eta: " << mytr->GetMomentumDirection().eta() <<   " position new " <<  mytr->GetPosition()/centimeter << " globaltime: " << mytr->GetGlobalTime()/ns <<  std::endl;
  if(mytr->GetGlobalTime()/ns>49990) std::cout << "G4SQLoopProcessCreateDiscr::PostStepDoIt: going to loose the particle because the GlobalTime is getting close to 50000" << std::endl;
  fParticleChange->Clear();
  fParticleChange->Initialize(track);
  fParticleChange->SetNumberOfSecondaries(2+1); //create 99 secondaries. Together with the original antiS there will then be 100 identical Sexaquarks travelling
  G4DynamicParticle* replacementParticle= new G4DynamicParticle(G4AntiSQ::AntiSQ(), track.GetMomentumDirection(), track.GetKineticEnergy());
  std::cout << "G4SQLoopProcessCreateDiscr::PostStepDoIt: 1 replacementParticle TotalEnergy: "<< replacementParticle->GetTotalEnergy()/GeV << std::endl;

  for(int i = 0; i < 2; i++){
   fParticleChange->AddSecondary(replacementParticle);
  }

  //Add the neutrino going in the opposite direction to conserve energy
  G4double NeutrinoKinEnergy = (2-1)*replacementParticle->GetTotalEnergy(); //also add the Sexaq mass, because all energy goes into kinetic energy for the neutrino
  G4DynamicParticle* neutrinoParticle= new G4DynamicParticle(G4Geantino::GeantinoDefinition(), -track.GetMomentumDirection(), -NeutrinoKinEnergy);
  std::cout << "G4SQLoopProcessCreateDiscr::PostStepDoIt: neutrinoParticle TotalEnergy: "<< neutrinoParticle->GetTotalEnergy()/GeV  << " Kinetic Energy: " << neutrinoParticle->GetKineticEnergy()/GeV << std::endl;
  std::cout << "G4SQLoopProcessCreateDiscr::PostStepDoIt: deltaE = " << track.GetTotalEnergy()-(2*replacementParticle->GetTotalEnergy()+neutrinoParticle->GetTotalEnergy());
  fParticleChange->AddSecondary(neutrinoParticle);
  

  fParticleChange->ProposeTrackStatus(fStopAndKill);
  //fParticleChange->ProposeWeight(track.GetWeight());
  return fParticleChange;
}


G4double G4SQLoopProcessCreateDiscr::PostStepGetPhysicalInteractionLength(const G4Track& track, G4double previousStepSize, G4ForceCondition* condition)
{
  *condition = NotForced; 
  G4double intLength = DBL_MAX; 
  G4Track * mytr = const_cast<G4Track *>(&track);
  if (mytr->GetPosition().rho()>1.8*centimeter && track.GetParentID() == 0){  // when the particle reaches almost the first material let them multiply, but only the one with a parentId = 0 should multiply, because this is the original Sexaq
       //nreset < 100) {
       intLength = 0.0;//0 interaction length means particle will directly interact.
       ++nreset;
       std::cout << "particle at rho > 1.8cm, so would like to create 100 now. The parentID of the particle is: " << track.GetParentID() << std::endl;
  } 
 // std::cout << "G4SQLoopProcessCreateDiscr::PostStepGetPhysicalInteractionLength return: " << intLength << std::endl;
  return intLength;
}

G4double G4SQLoopProcessCreateDiscr::GetMeanFreePath(const G4Track&,G4double,
					  G4ForceCondition*)
{
  return DBL_MAX;
} 


void G4SQLoopProcessCreateDiscr::StartTracking(G4Track * aTrack)
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
