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
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// By JPW, working, but to be cleaned up. @@@
// G.Folger, 29-sept-2006: extend to 1TeV, using a constant above 20GeV
// 22 Dec 2006 - DHW added isotope dependence
// G.Folger, 25-Nov-2009: extend to 100TeV, using a constant above 20GeV
// V.Ivanchenko, 18-Aug-2011: migration to new design and cleanup;
//                            make it applicable for Z>1
//

#include "G4SQInelasticCrossSection.hh"
#include "G4SystemOfUnits.hh"
#include "G4DynamicParticle.hh"
#include "G4SQ.hh"
#include "G4AntiSQ.hh"
#include "G4NistManager.hh"


G4SQInelasticCrossSection::G4SQInelasticCrossSection()
  : G4VCrossSectionDataSet("SQ-neutron")
{
  nist = G4NistManager::Instance();
  theSQ = G4SQ::SQ();
  theAntiSQ = G4AntiSQ::AntiSQ();
}


G4SQInelasticCrossSection::~G4SQInelasticCrossSection()
{}


G4bool G4SQInelasticCrossSection::IsElementApplicable(
                             const G4DynamicParticle* aPart, 
			     G4int Z, const G4Material*)
{
  return ((0 < Z) && 
          (aPart->GetDefinition() == theSQ || 
	   aPart->GetDefinition() == theAntiSQ)
	 );
}


G4double G4SQInelasticCrossSection::GetElementCrossSection(
			     const G4DynamicParticle* aPart, 
			     G4int Z, const G4Material*)
{
  if (aPart->GetDefinition() == theAntiSQ) {
    return GetSQCrossSection(aPart->GetKineticEnergy(), Z);
  } else {
    return 0; // return zero fo particle instead of antiparticle
              // sexaquark interaction with matter expected really tiny
  }
}


G4double G4SQInelasticCrossSection::GetSQCrossSection(
			     G4double kineticEnergy, G4int Z)
{
  if(kineticEnergy <= 0.0) { return 0.0; }
 
  G4double a = nist->GetAtomicMassAmu(Z);
  G4double nOfNeutrons = a - Z;
  G4double crossSection = nOfNeutrons * 1200. * millibarn; //can put it to 1200

  return crossSection;
}

