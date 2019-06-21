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
  //G4Track * mytr = const_cast<G4Track *>(&track);
  
  std::cout << "G4SQLoopProcess::AlongStepDoIt GetPostion  " << track.GetPosition()/cm << " MomentumDirection " << track.GetMomentumDirection() << " trackId " << track.GetTrackID() << " parentId: " << track.GetParentID()  << std::endl;
  //std::cout << "*** in between RESETs to initpos GlobalTime, LocalTime, ProperTime, TrackStatus, TrackLength " << mytr->GetGlobalTime()/nanosecond << " " << mytr->GetLocalTime()/nanosecond << " " << mytr->GetProperTime()/nanosecond << " " << mytr->GetTrackStatus() << " " << mytr->GetTrackLength()/centimeter << std::endl;
  //if(track.GetPosition()==posini)std::cout << "*** in between RESETs to initpos Material " << track.GetMaterial()  << std::endl;
  fParticleChange->Clear();
  fParticleChange->Initialize(track);
  fParticleChange->ProposeWeight(track.GetWeight());
  //if(fabs(track.GetMomentumDirection().eta()) > 3){fParticleChange->ProposeTrackStatus(fStopAndKill);std::cout << "killed the particle because eta is too large" << std::endl;}
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
  
  nreset = 0;
}


