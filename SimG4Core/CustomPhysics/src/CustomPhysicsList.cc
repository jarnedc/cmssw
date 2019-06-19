#include "SimG4Core/CustomPhysics/interface/CustomPhysicsList.h"
#include "SimG4Core/CustomPhysics/interface/CustomParticleFactory.h"
#include "SimG4Core/CustomPhysics/interface/DummyChargeFlipProcess.h"
#include "SimG4Core/CustomPhysics/interface/G4ProcessHelper.hh"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "G4Decay.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4ProcessManager.hh"

#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4IonConstructor.hh"

#include "SimG4Core/CustomPhysics/interface/FullModelHadronicProcess.hh"
#include "SimG4Core/CustomPhysics/interface/ToyModelHadronicProcess.hh"

#include "SimG4Core/CustomPhysics/src/G4QGSPSIMPBuilder.hh"
#include "SimG4Core/CustomPhysics/src/G4SIMPInelasticProcess.hh"
#include "SimG4Core/CustomPhysics/src/G4SQInelasticProcess.hh"
#include "SimG4Core/CustomPhysics/src/G4SQLoopProcess.hh"
#include "SimG4Core/CustomPhysics/src/G4SQLoopProcessDiscr.hh"
#include "SimG4Core/CustomPhysics/src/G4SQNeutronAnnih.hh"
#include "SimG4Core/CustomPhysics/src/G4SQInelasticCrossSection.hh"

using namespace CLHEP;
 

CustomPhysicsList::CustomPhysicsList(std::string name, const edm::ParameterSet & p)  
  :  G4VPhysicsConstructor(name) 
{  
  myConfig = p;
  edm::FileInPath fp = p.getParameter<edm::FileInPath>("particlesDef");
  particleDefFilePath = fp.fullPath();
  edm::LogInfo("CustomPhysics")<<"Path for custom particle definition file: "
			       <<particleDefFilePath;
  myHelper = 0;  
}

CustomPhysicsList::~CustomPhysicsList() {
  delete myHelper;
}
 
void CustomPhysicsList::ConstructParticle(){
  CustomParticleFactory::loadCustomParticles(particleDefFilePath);     
}
 
void CustomPhysicsList::ConstructProcess() {
  addCustomPhysics();
}
 
void CustomPhysicsList::addCustomPhysics(){

  edm::LogInfo("CustomPhysics") << " CustomPhysicsList: adding CustomPhysics processes "
				<< "for the list of particles: \n";
  aParticleIterator->reset();

  while((*aParticleIterator)()) {
    G4ParticleDefinition* particle = aParticleIterator->value();

    if (particle->GetParticleType() == "simp") {

std::cout << "=== SL === " 
          << particle->GetParticleType() << " "
          << particle->GetParticleName() << std::endl;

      G4ProcessManager* pmanager = particle->GetProcessManager();
      if(pmanager) {
        G4SIMPInelasticProcess * simpInelPr = new G4SIMPInelasticProcess();
        G4QGSPSIMPBuilder * theQGSPSIMPB = new G4QGSPSIMPBuilder(false); // false -> no QuasiElastic (not adapted for SIMP - crashes)
        theQGSPSIMPB->SetMinEnergy(12.0*MeV); // normally this is GeV, but that
                 // leads to crashes with the massive SIMPs
	theQGSPSIMPB->Build(simpInelPr);
        pmanager->AddDiscreteProcess(simpInelPr);
      }
      else  edm::LogInfo("CustomPhysics") << "   No pmanager";

    } else if (particle->GetParticleName() == "sexaq" || particle->GetParticleName() == "anti_sexaq") {

std::cout << "=-= SL sexaq FTW! =-= " 
          << particle->GetParticleType() << " "
          << particle->GetParticleName() << std::endl;

      G4ProcessManager* pmanager = particle->GetProcessManager();
      if(pmanager) {
        G4SQInelasticProcess * sqInelPr = new G4SQInelasticProcess();
	G4SQNeutronAnnih * sqModel = new G4SQNeutronAnnih();
	sqInelPr->RegisterMe(sqModel);
	G4SQInelasticCrossSection * sqInelXS = new G4SQInelasticCrossSection();
	sqInelPr->AddDataSet(sqInelXS);
        pmanager->AddDiscreteProcess(sqInelPr);

	G4SQLoopProcess * sqLoopPr = new G4SQLoopProcess();
	pmanager->AddContinuousProcess(sqLoopPr);
	G4SQLoopProcessDiscr * sqLoopPrDiscr = new G4SQLoopProcessDiscr();
	pmanager->AddDiscreteProcess(sqLoopPrDiscr);
      }
      else  edm::LogInfo("CustomPhysics") << "   No pmanager";

    } else {

std::cout << "=-= SL =-= " 
          << particle->GetParticleType() << " "
          << particle->GetParticleName() << std::endl;

    if(CustomParticleFactory::isCustomParticle(particle)) {
      CustomParticle* cp = dynamic_cast<CustomParticle*>(particle);
      edm::LogInfo("CustomPhysics") << particle->GetParticleName()
				    <<"  PDGcode= "<<particle->GetPDGEncoding()
				    << "  Mass= "
				    <<particle->GetPDGMass()/GeV  <<" GeV.";
      if(cp->GetCloud()!=0) {
	edm::LogInfo("CustomPhysics") << particle->GetParticleName()
				      <<" CloudMass= "
				      <<cp->GetCloud()->GetPDGMass()/GeV
				      <<" GeV; SpectatorMass= "
				      << cp->GetSpectator()->GetPDGMass()/GeV
				      <<" GeV.";
      }

      G4ProcessManager* pmanager = particle->GetProcessManager();
      if(pmanager) {
	if(particle->GetPDGCharge() != 0.0) {
	  pmanager->AddProcess(new G4hMultipleScattering,-1, 1, 1);
	  pmanager->AddProcess(new G4hIonisation,        -1, 2, 2);
	}
	if(cp != 0) {
	  if(particle->GetParticleType()=="rhadron" || 
	     particle->GetParticleType()=="mesonino" || 
	     particle->GetParticleType() == "sbaryon"){
	    if(!myHelper) myHelper = new G4ProcessHelper(myConfig);
	    pmanager->AddDiscreteProcess(new FullModelHadronicProcess(myHelper));
	  }
	}
      }
      else  edm::LogInfo("CustomPhysics") << "   No pmanager";

    }
   }
  }
}


void CustomPhysicsList::setupRHadronPhycis(G4ParticleDefinition* particle)
{
  //    LogDebug("CustomPhysics")<<"Configuring rHadron: "
  //	<<cp->

  CustomParticle* cp = dynamic_cast<CustomParticle*>(particle);
  if(cp->GetCloud()!=0) {
    LogDebug("CustomPhysics")
      <<"Cloud mass is "
      <<cp->GetCloud()->GetPDGMass()/GeV
      <<" GeV. Spectator mass is "
      <<static_cast<CustomParticle*>(particle)->GetSpectator()->GetPDGMass()/GeV
      <<" GeV.";
  }
  G4ProcessManager* pmanager = particle->GetProcessManager();
  if(pmanager){
    if(!myHelper) myHelper = new G4ProcessHelper(myConfig);
    if(particle->GetPDGCharge()/eplus != 0){
      pmanager->AddProcess(new G4hMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,        -1, 2, 2);
    }
    pmanager->AddDiscreteProcess(new FullModelHadronicProcess(myHelper)); //GHEISHA
  }
  else      LogDebug("CustomPhysics") << "   No pmanager";
}
					       
void CustomPhysicsList::setupSUSYPhycis(G4ParticleDefinition* particle)
{
  G4ProcessManager* pmanager = particle->GetProcessManager();
  if(pmanager){
    if(particle->GetPDGCharge()/eplus != 0){
      pmanager->AddProcess(new G4hMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,        -1, 2, 2);
    }
    pmanager->AddProcess(new G4Decay, 1, -1, 3);
  }
  else      LogDebug("CustomPhysics") << "   No pmanager";
}
