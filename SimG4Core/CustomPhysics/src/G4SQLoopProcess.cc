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
  // check of reset needed
  if ((mytr->GetPosition().rho()>100*centimeter || // require to be "outside" the tracker
       fabs(mytr->GetPosition().z())>300*centimeter) &&
      nreset < 100) { // don't reset too many times
    std::cout << "*** RESET " << nreset%100 + 1 << " ! from " << mytr->GetPosition()
              << " to " << posini << std::endl;
    std::cout << "*** KINE " << mytr->GetKineticEnergy() << " " << mytr->GetVelocity() << std::endl;
    ++nreset;
    mytr->SetPosition(posini);
    mytr->SetGlobalTime(globaltimeini); //resetting the GlobalTime, because there is a limit of 500 for particles which travel too long
    mytr->SetLocalTime(localtimeini); //resetting the LocalTime, because there is a limit of 500 for particles which travel too long
    mytr->SetProperTime(propertimeini);
  }
  // return current state of the track
 
  if(mytr->GetPosition() == posini) std::cout << "the track is back at it's posini, does it interact here?" << std::endl;
  
  std::cout << "*** in between RESETs to initpos KIN, vel, position " << mytr->GetKineticEnergy() << " " << mytr->GetVelocity() << " " << mytr->GetPosition()/centimeter << std::endl;
  std::cout << "*** in between RESETs to initpos GlobalTime, LocalTime, ProperTime, TrackStatus, TrackLength " << mytr->GetGlobalTime()/nanosecond << " " << mytr->GetLocalTime()/nanosecond << " " << mytr->GetProperTime()/nanosecond << " " << mytr->GetTrackStatus() << " " << mytr->GetTrackLength()/centimeter << std::endl;
  fParticleChange->Clear();
  fParticleChange->Initialize(track);
  fParticleChange->ProposeWeight(track.GetWeight());
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
  std::cout << "*** STARTTRACK "
            << aTrack->GetMomentum().rho()/GeV << " "
            << aTrack->GetMomentum().eta() << " "
            << aTrack->GetPosition().rho()/centimeter << " " 
            << aTrack->GetPosition().z()/centimeter << std::endl;
  posini = aTrack->GetPosition();
  globaltimeini = aTrack->GetGlobalTime();
  localtimeini = aTrack->GetLocalTime();
  propertimeini = aTrack->GetProperTime();
  
  nreset = 0;
}


