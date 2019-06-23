#include "G4SQLoopProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VParticleChange.hh"


G4SQLoopProcess::G4SQLoopProcess(const G4String& name, G4ProcessType type)
  : G4VContinuousProcess(name, type)
{
  fParticleChange = new G4ParticleChange();
}


G4SQLoopProcess::~G4SQLoopProcess()
{
  delete fParticleChange;
}


G4VParticleChange* G4SQLoopProcess::AlongStepDoIt(const G4Track& track, const G4Step& step)
{
  G4Track * mytr = const_cast<G4Track *>(&track);

//  mytr->SetGlobalTime(globaltimeini); 
//  mytr->SetLocalTime(localtimeini); 
//  mytr->SetProperTime(propertimeini); 

  std::cout << "G4SQLoopProcess::AlongStepDoIt track GetPostion  " << track.GetPosition()/cm << " MomentumDirection " << track.GetMomentumDirection() << " trackId " << track.GetTrackID() << " parentId: " << track.GetParentID() << " GlobalTime " << track.GetGlobalTime()/ns << " TotalEnergy: " << track.GetTotalEnergy()/GeV << " Velocity " << track.GetVelocity()/m/ns << std::endl;
//  std::cout << "G4SQLoopProcess::AlongStepDoIt fParticleChange GetPostion  " << fParticleChange->GetPosition()->x()/cm << " " << fParticleChange->GetPosition()->y()/cm << " " << fParticleChange->GetPosition()->z()/cm << " " << " MomentumDirection " << fParticleChange->GetMomentumDirection() << " GlobalTime " << fParticleChange->GetGlobalTime() << std::endl;

  if(track.GetPosition()==posini) std::cout << "G4SQLoopProcess::AlongStepDoIt particle is back at posini, where Material is " << track.GetMaterial()  << std::endl;

  fParticleChange->Clear();
  fParticleChange->Initialize(track);
  fParticleChange->ProposeWeight(track.GetWeight());

  //fParticleChange->ProposeGlobalTime(globaltimeini);
  //fParticleChange->ProposeLocalTime(localtimeini);
  //fParticleChange->ProposeProperTime(propertimeini); 

  return fParticleChange;
}


G4double G4SQLoopProcess::AlongStepGetPhysicalInteractionLength(const G4Track& track, G4double previousStepSize, G4double currentMinimumStep, G4double& proposedSafety, G4GPILSelection* selection)
{
  return 1.*centimeter;
}


G4double G4SQLoopProcess::GetContinuousStepLimit(const G4Track& track, G4double , G4double , G4double& )
{ 
  return 1.*centimeter; // seems irrelevant
}


void G4SQLoopProcess::StartTracking(G4Track * aTrack)
{
  posini = aTrack->GetPosition();
  globaltimeini = aTrack->GetGlobalTime();  
  localtimeini = aTrack->GetLocalTime();
  propertimeini = aTrack->GetProperTime();
  nreset = 0;
}


