//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                            s                      *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id$
//
//
// G4 Model: simple SexaQuark neutron annihilation model
//           note: 
//           25-11-2018 Steven Lowette
//
// Modified:
// 07-Jun-06 V.Ivanchenko fix problem of rotation of final state
// 25-Jul-06 V.Ivanchenko add 19 MeV low energy, below which S-wave is sampled
// 12-Jun-12 A.Ribon fix warnings of shadowed variables
//

#include "G4SQNeutronAnnih.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SQ.hh"
#include "Randomize.hh"
//Jarne-begin
#include <math.h>
#include "TMath.h"
#include "G4NucleiProperties.hh"
//Jarne-end

G4SQNeutronAnnih::G4SQNeutronAnnih() : G4HadronicInteraction("SexaQuark-neutron annihilation")
{
  SetMinEnergy( 0.0*GeV );
  SetMaxEnergy( 100.*TeV );

  theSQ    = G4SQ::SQ();
  theK0S   = G4KaonZeroShort::KaonZeroShort();
  theAntiL = G4AntiLambda::AntiLambda();
  theProton = G4Proton::Proton();//proton only used when the particle which the sexaquark hits is a deutereon and the neutron dissapears, so what stays behind is a proton
}


G4SQNeutronAnnih::~G4SQNeutronAnnih()
{}




G4double G4SQNeutronAnnih::momDistr(G4double x_in){

const int n_entries = 44;

G4double CDF_k[n_entries] = {
0,
0.1,
0.2,
0.3,
0.4,
0.5,
0.6,
0.7,
0.8,
0.9,
1.0,
1.1,
1.2,
1.3,
1.4,
1.5,
1.6,
1.7,
1.8,
1.9,
2.0,
2.1,
2.2,
2.3,
2.4,
2.5,
2.6,
2.7,
2.8,
2.9,
3.0,
3.1,
3.2,
3.3,
3.4,
3.5,
3.6,
3.7,
3.8,
3.9,
4.0,
4.1,
4.2,
4.3
};

G4double x[n_entries] = {
0,
0.0021588149,
0.0140404826,
0.0398978742,
0.0867028366,
0.1621454662,
0.2598997166,
0.3711661117,
0.4894576938,
0.6014469022,
0.6965054188,
0.7698421233,
0.8248982674,
0.8653929814,
0.8908522303,
0.9076414554,
0.9180538115,
0.9252072498,
0.9302921992,
0.9344352827,
0.9382781653,
0.9419662882,
0.9455545126,
0.9491421018,
0.9526564342,
0.9560989822,
0.9594942978,
0.962797601,
0.9660114729,
0.9691812282,
0.9722862164,
0.9752459602,
0.9781059363,
0.9808507361,
0.98344928,
0.9859093946,
0.9882226185,
0.990353598,
0.992357339,
0.9941907296,
0.9958567897,
0.9973916332,
0.9987581556,
1
};

  //now interpolate the above points for x_in
  G4double result = 9999;
  for(int i = 0; i < n_entries; i++){
    if(x[i] > x_in) {
        result = (CDF_k[i]-CDF_k[i-1])/(x[i]-x[i-1])*(x_in-x[i-1])+CDF_k[i-1];
        break;
    }
  } 
  //ROOT::Math::Interpolator inter(n_entries, ROOT::Math::Interpolation::kAKIMA);
  //inter.SetData(n_entries,x,CDF_k);
  //result = inter.Eval(x_in);

  return result;
  //return 1;

}





G4HadFinalState* G4SQNeutronAnnih::ApplyYourself(
                 const G4HadProjectile& aTrack, G4Nucleus& targetNucleus)
{
  theParticleChange.Clear();
  const G4HadProjectile* aParticle = &aTrack;
  G4double ekin = aParticle->GetKineticEnergy();


  G4int A = targetNucleus.GetA_asInt();
  G4int Z = targetNucleus.GetZ_asInt();


  G4double m_K0S = G4KaonZeroShort::KaonZeroShort()->GetPDGMass();
  G4double m_L  = G4AntiLambda::AntiLambda()->GetPDGMass();

  G4cout << "---->   G4SQNeutronAnnih    <-----" <<  G4endl;

  //G4double plab = aParticle->GetTotalMomentum();

//    G4cout << "G4SQNeutronAnnih: Incident particle p (GeV), total Energy (GeV), particle name, eta ="
//           << plab/GeV << "  "
//           << aParticle->GetTotalEnergy()/GeV << "  "
//           << aParticle->GetDefinition()->GetParticleName() << " "
//	   << aParticle->Get4Momentum() << G4endl;

  // Scattered particle referred to axis of incident particle
  //const G4ParticleDefinition* theParticle = aParticle->GetDefinition();

  //G4int projPDG = theParticle->GetPDGEncoding();
//  if (verboseLevel > 1)
//    G4cout << "G4SQNeutronAnnih: for " << theParticle->GetParticleName()
//           << " PDGcode= " << projPDG << " on nucleus Z= " << Z
//           << " A= " << A << " N= " << N
//           << G4endl;

  G4LorentzVector lv1 = aParticle->Get4Momentum();
  G4cout << "The neutron Fermi momentum (mag, x, y, z) " << targetNucleus.GetFermiMomentum().mag()/MeV << " " << targetNucleus.GetFermiMomentum().x()/MeV << " " << targetNucleus.GetFermiMomentum().y()/MeV << " " << targetNucleus.GetFermiMomentum().z()/MeV << std::endl;

  //calculate fermi momentum

  G4double  k_neutron = momDistr(G4UniformRand());
  G4double momentum_neutron = 0.1973*GeV*k_neutron;
  
  G4double theta_neutron = TMath::ACos(2*G4UniformRand()-1);
  G4double phi_neutron = 2.*TMath::Pi()*G4UniformRand();

  G4double p_neutron_x = momentum_neutron*TMath::Sin(theta_neutron)*TMath::Cos(phi_neutron);
  G4double p_neutron_y = momentum_neutron*TMath::Sin(theta_neutron)*TMath::Sin(phi_neutron);
  G4double p_neutron_z = momentum_neutron*TMath::Cos(theta_neutron);


  //G4LorentzVector lv0(targetNucleus.GetFermiMomentum(), sqrt( pow(G4Neutron::Neutron()->GetPDGMass(),2) + targetNucleus.GetFermiMomentum().mag2()  ) );
  G4LorentzVector lv0(p_neutron_x, p_neutron_y, p_neutron_z, sqrt( pow(G4Neutron::Neutron()->GetPDGMass(),2) + momentum_neutron*momentum_neutron  ) );

  //const G4Nucleus* aNucleus = &targetNucleus;
  G4double BENeutronInNucleus = 0;
  if(A != 0)BENeutronInNucleus =  G4NucleiProperties::GetBindingEnergy( A, Z)/(A);

  G4cout << "BE of nucleon in the nucleus (GeV): " << BENeutronInNucleus/GeV << G4endl;

  G4LorentzVector lvBE(0,0,0,BENeutronInNucleus/GeV);
  G4LorentzVector lv  = lv0 + lv1 - lvBE;

  // kinematiacally impossible ?
  G4double etot = lv0.e() + lv1.e() - lvBE.e();
  if(etot < theK0S->GetPDGMass() + theAntiL->GetPDGMass()) {
    theParticleChange.SetEnergyChange(ekin);
    theParticleChange.SetMomentumChange(aTrack.Get4Momentum().vect().unit());
    return &theParticleChange;
  }

  float newIonMass = targetNucleus.AtomicMass(A-1,Z)*931.5*MeV; ;
  G4LorentzVector nlvIon(0,0,0,newIonMass);

  G4double theta_KS0_star = TMath::ACos(2*G4UniformRand()-1);
  G4double phi_KS0_star = 2.*TMath::Pi()*G4UniformRand();

  G4double p_K0S_star_x = TMath::Sin(theta_KS0_star)*TMath::Cos(phi_KS0_star);
  G4double p_K0S_star_y = TMath::Sin(theta_KS0_star)*TMath::Sin(phi_KS0_star);
  G4double p_K0S_star_z = TMath::Cos(theta_KS0_star);

  G4ThreeVector p(p_K0S_star_x,p_K0S_star_y,p_K0S_star_z);
  double m0 = lv.m();
  double m0_2 = m0*m0;
  double m1_2 = m_K0S*m_K0S;
  double m2_2 = m_L*m_L;

   p *= 0.5/m0 * sqrt (    m0_2*m0_2 +   m1_2*m1_2 +   m2_2*m2_2 - 2*m0_2*m1_2 - 2*m0_2*m2_2 - 2*m1_2*m2_2);
  double p2 = p.mag2();

  G4LorentzVector nlvK0S( p, sqrt (p2 + m1_2));
  G4LorentzVector nlvAntiL(-p, sqrt (p2 + m2_2));

  // Boost out of the rest frame.
  nlvK0S.boost (lv.boostVector ());
  nlvAntiL.boost (lv.boostVector ());

  // now move to implement the interaction
  theParticleChange.SetStatusChange(stopAndKill);
  //theParticleChange.SetEnergyChange(ekin); // was 0.0



  G4DynamicParticle * aSec1 = new G4DynamicParticle(theK0S, nlvK0S);
  theParticleChange.AddSecondary(aSec1);
  G4DynamicParticle * aSec2 = new G4DynamicParticle(theAntiL, nlvAntiL);
  theParticleChange.AddSecondary(aSec2);

  const G4ParticleDefinition* theRemainingNucleusDef = theProton;
  if(A != 1) theRemainingNucleusDef = G4IonTable::GetIonTable()->GetIon(Z,A-1);
  G4DynamicParticle * aSec3 = new G4DynamicParticle(theRemainingNucleusDef, nlvIon);  
  theParticleChange.AddSecondary(aSec3);

  // return as is; we don't care about what happens to the nucleus
  return &theParticleChange;

}

