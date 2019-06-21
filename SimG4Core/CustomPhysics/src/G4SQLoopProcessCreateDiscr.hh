#ifndef G4SQLoopProcessCreateDiscr_h
#define G4SQLoopProcessCreateDiscr_h 1

#include "G4VDiscreteProcess.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4ParticleChange.hh"
#include "G4ParticleChangeForTransport.hh"
#include "G4SQ.hh"
#include "G4AntiSQ.hh"
#include "G4NeutrinoMu.hh"
class G4Step;
class G4ParticleDefinition;


class G4SQLoopProcessCreateDiscr : public G4VDiscreteProcess {

  public:

    G4SQLoopProcessCreateDiscr(const G4String& name = "SQLooper",
                          G4ProcessType type = fUserDefined);
    virtual ~G4SQLoopProcessCreateDiscr();


  public:

    virtual G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);
    virtual G4double PostStepGetPhysicalInteractionLength(const G4Track& track, G4double previousStepSize, G4ForceCondition* condition);
    virtual G4double GetMeanFreePath(const G4Track&, G4double,G4ForceCondition*);
    void  SetTimeLimit(G4double);
    virtual void StartTracking(G4Track * aTrack);					
  private:

    G4SQLoopProcessCreateDiscr(G4SQLoopProcessCreateDiscr &);
    G4SQLoopProcessCreateDiscr & operator=(const G4SQLoopProcessCreateDiscr &right);

  protected:

    //G4ParticleChangeForTransport* fParticleChange;
    G4ParticleChange* fParticleChange;
  
  private:
 
    G4int nreset;
    G4ThreeVector posini;
    G4double globaltimeini;
    G4double localtimeini;
    G4double propertimeini;
   

};


#endif
