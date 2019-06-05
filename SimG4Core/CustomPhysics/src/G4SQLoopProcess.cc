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
  if (fabs(mytr->GetMomentum().eta())<4 &&  // require not too forward
      (mytr->GetPosition().rho()>100*centimeter || // require to be "outside" the tracker
       fabs(mytr->GetPosition().z())>300*centimeter) &&
      nreset < 100) { // don't reset too many times
    std::cout << "*** RESET " << nreset%100 + 1 << " ! from " << mytr->GetPosition()
              << " to " << posini << std::endl;
    std::cout << "*** KINE " << mytr->GetKineticEnergy() << " " << mytr->GetVelocity() << std::endl;
    ++nreset;
    mytr->SetPosition(posini);
  }
  // return current state of the track
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
  nreset = 0;
}


