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


G4HadFinalState* G4SQNeutronAnnih::ApplyYourself(
                 const G4HadProjectile& aTrack, G4Nucleus& targetNucleus)
{
  theParticleChange.Clear();
  const G4HadProjectile* aParticle = &aTrack;
  G4double ekin = aParticle->GetKineticEnergy();

  G4int A = targetNucleus.GetA_asInt();
  G4int Z = targetNucleus.GetZ_asInt();
  G4int N = A - Z;


  //G4double m_n = G4Neutron::Neutron()->GetPDGMass();
  //G4double m_S = aParticle->GetDefinition()->GetPDGMass();
  //G4double m_S_plus_n = m_n + m_S;
  G4double m_K0S = G4KaonZeroShort::KaonZeroShort()->GetPDGMass();
  G4double m_L  = G4AntiLambda::AntiLambda()->GetPDGMass();


  G4double plab = aParticle->GetTotalMomentum();

//  const G4Nucleus* aNucleus = &targetNucleus;
//  if (verboseLevel > 1)
    G4cout << "Incident particle p (GeV), total Energy (GeV), particle name ="
           << plab/GeV << "  "
           << aParticle->GetTotalEnergy()/GeV << "  "
           << aParticle->GetDefinition()->GetParticleName() << G4endl;
   //  G4cout << "targetNucleus p(GeV), total Energy (GeV), particle name = "
   //	   << aNucleus->GetTotalMomentum()/GeV << "  "
   //        << aNucleus->GetTotalEnergy()/GeV << "  "
   //        << aNucleus->GetDefinition()->GetParticleName() << G4endl;

  // Scattered particle referred to axis of incident particle
  const G4ParticleDefinition* theParticle = aParticle->GetDefinition();

  G4int projPDG = theParticle->GetPDGEncoding();
  if (verboseLevel > 1)
    G4cout << "G4SQNeutronAnnih for " << theParticle->GetParticleName()
           << " PDGcode= " << projPDG << " on nucleus Z= " << Z
           << " A= " << A << " N= " << N
           << G4endl;

  G4LorentzVector lv1 = aParticle->Get4Momentum();
  G4LorentzVector lv0(0.,0.,0.,G4Neutron::Neutron()->GetPDGMass());

  //const G4Nucleus* aNucleus = &targetNucleus;
  G4double BENeutronInNucleus = 0;
  if(A != 0)BENeutronInNucleus =  G4NucleiProperties::GetBindingEnergy( A, Z)/(A);
  G4cout << "BE of nucleon in the nucleus (GeV): " << BENeutronInNucleus/GeV << G4endl;
  G4LorentzVector lvBE(0,0,0,BENeutronInNucleus/GeV);
  G4LorentzVector lv  = lv0 + lv1 - lvBE;
//  G4ThreeVector bst = lv.boostVector();
//  lv1.boost(-bst);
//  lv0.boost(-bst);

  // kinematiacally impossible ?
  G4double etot = lv0.e() + lv1.e();
  if(etot < theK0S->GetPDGMass() + theAntiL->GetPDGMass()) {
    theParticleChange.SetEnergyChange(ekin);
    theParticleChange.SetMomentumChange(aTrack.Get4Momentum().vect().unit());
    return &theParticleChange;
  }
/*
  //Jarne: calculate parameters for the S+n (= S_n) system - begin
  //assume the S momentum is transfered to the S+n  system, then the momenta are the same

  //get the spatial component
  G4ThreeVector p3_S_n = lv1.vect();
  G4ThreeVector bst = lv1.boostVector();
  
  //G4double E_S = pow(p3_S_n*p3_S_n+m_S*m_S,0.5);
  //G4cout << "total energy of the incident particle (GeV): " << E_S/GeV << G4endl; 
  //G4double E_S_n = E_S + m_n;
  //G4cout << "total energy of the incident particle + n system (GeV): " << E_S_n/GeV << G4endl;
  //G4double M_S_n = pow(E_S_n*E_S_n-p3_S_n*p3_S_n,0.5);

  //Jarne: calculate parameters for the S+n (= S_n) system - end


  //Jarne: K0S 4 momentum in the star frame - begin
  //This is a 2 body decay (http://www.helsinki.fi/~www_sefo/phenomenology/Schlippe_relativistic_kinematics.pdf)
  
  G4double E_K0S_star = (m_S_plus_n*m_S_plus_n+m_K0S*m_K0S-m_L*m_L)/(2*m_S_plus_n);
  G4cout << "Total energy of the Ks in the * frame (GeV): " << E_K0S_star/GeV << G4endl;
  G4double p_K0S_star = pow(E_K0S_star*E_K0S_star - m_K0S*m_K0S,0.5);
  G4cout << "Total momentum of the Ks in the * frame (GeV): " << p_K0S_star/GeV << G4endl;
    

  //Jarne: K0S 4 momentum in the star frame - end

  //Jarne: L 4 momentum in the star frame - begin
  //This is a 2 body decay (http://www.helsinki.fi/~www_sefo/phenomenology/Schlippe_relativistic_kinematics.pdf)

  G4double E_L_star = (m_S_plus_n*m_S_plus_n+m_L*m_L-m_K0S*m_K0S)/(2*m_S_plus_n);
  G4cout << "Total energy of the L in the * frame (GeV): " << E_L_star/GeV << G4endl;
  //G4double p_L_star = pow(E_L_star*E_L_star - m_L*m_L,0.5);


  //Jarne: K0S 4 momentum in the star frame - end

  //Jarne: find a direction for the K0s and the L in the S_n system - begin

  G4double theta_KS0_star = TMath::ACos(2*G4UniformRand()-1);
  G4double phi_KS0_star = 2.*TMath::Pi()*G4UniformRand();

  //Jarne: find a direction for the K0s and the L in the S_n system - end

  //Jarne: fill the KS0 and L 4 vector for the S_n frame - begin

  G4double p_K0S_star_x = p_K0S_star*TMath::Sin(theta_KS0_star)*TMath::Cos(phi_KS0_star);
  G4double p_K0S_star_y = p_K0S_star*TMath::Sin(theta_KS0_star)*TMath::Sin(phi_KS0_star);
  G4double p_K0S_star_z = p_K0S_star*TMath::Cos(theta_KS0_star);

  G4LorentzVector nlvK0S_star(p_K0S_star_x, p_K0S_star_y, p_K0S_star_z, E_K0S_star);
  G4LorentzVector nlvAntiL_star(-p_K0S_star_x, -p_K0S_star_y, -p_K0S_star_z, E_L_star);

  //Jarne: fill the KS0 and L 4 vector for the S_n frame - end

  //Jarne: now boost the nlvK0S_star and nlvAntiL_star from the S_n frame to the detector frame (moving with respect to eachother with p3_S_n) - begin

  G4LorentzVector nlvK0S = nlvK0S_star.boost(bst);
  G4LorentzVector nlvAntiL = nlvAntiL_star.boost(bst);
  G4cout << "Ks in the lab frame p(GeV), E (GeV): " << pow(nlvK0S.px()*nlvK0S.px()+nlvK0S.py()*nlvK0S.py()+nlvK0S.pz()*nlvK0S.pz(),0.5)/GeV << "  "  << nlvK0S.e()/GeV << G4endl;
  G4cout << "L in the lab frame p(GeV), E (GeV): " << pow(nlvAntiL.px()*nlvAntiL.px()+nlvAntiL.py()*nlvAntiL.py()+nlvAntiL.pz()*nlvAntiL.pz(),0.5)/GeV << "  "  << nlvAntiL.e()/GeV << G4endl;
  //Jarne: now boost the nlvK0S_star and nlvAntiL_star from the S_n frame to the detector frame (moving with respect to eachother with p3_S_n) - end

  float newIonMass = targetNucleus.AtomicMass(A-1,Z)*931.5*MeV; // -G4Neutron::Neutron()->GetPDGMass();
  G4LorentzVector nlvIon(0,0,0,newIonMass);

  // temporary; violates all basic laws of nature
 // G4LorentzVector nlvK0S(lv1.x()/2,lv1.y()/2,lv1.z()/2,
 //                        sqrt(pow(lv1.x()/2,2)+pow(lv1.y()/2,2)+pow(lv1.z()/2,2)+pow(498*MeV,2)));
 // G4LorentzVector nlvAntiL(lv1.x()/2,lv1.y()/2,lv1.z()/2,
 //                        sqrt(pow(lv1.x()/2,2)+pow(lv1.y()/2,2)+pow(lv1.z()/2,2)+pow(1116*MeV,2)));

//  nlv0.boost(bst);
//  nlv1.boost(bst);
*/
  float newIonMass = targetNucleus.AtomicMass(A-1,Z)*931.5*MeV; // -G4Neutron::Neutron()->GetPDGMass();
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
//if(A != 1)G4DynamicParticle * aSec3 = new G4DynamicParticle(G4IonTable::GetIonTable()->GetIon(Z,A-1), nlvIon);
  theParticleChange.AddSecondary(aSec3);
  //G4cout << "erec= " <<erec << " Esec= " << aSec->GetKineticEnergy() << G4endl;  

  // return as is; we don't care about what happens to the nucleus
  return &theParticleChange;

}

