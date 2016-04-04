#ifndef GlobalTrigger_ExternalCondition_h
#define GlobalTrigger_ExternalCondition_h

/**
 * \class ExternalCondition
 * 
 * 
 * Description: evaluation of a CondEnergySum condition.
 * 
 * Implementation:
 *    <TODO: enter implementation details>
 *   
 * \author: Vasile Mihai Ghete   - HEPHY Vienna 
 * 
 *
 */

// system include files
#include <iosfwd>
#include <string>

// user include files
//   base classes
#include "L1Trigger/L1TGlobal/interface/ConditionEvaluation.h"

// forward declarations
class GtCondition;
class ExternalTemplate;

namespace l1t {

class L1Candidate;

class GtBoard;

// class declaration
class ExternalCondition : public ConditionEvaluation
{

public:

    /// constructors
    ///     default
    ExternalCondition();

    ///     from base template condition (from event setup usually)
    ExternalCondition(const GtCondition*, const GtBoard*);

    // copy constructor
    ExternalCondition(const ExternalCondition&);

    // destructor
    virtual ~ExternalCondition();

    // assign operator
    ExternalCondition& operator=(const ExternalCondition&);

public:

    /// the core function to check if the condition matches
    const bool evaluateCondition(const int bxEval) const;

    /// print condition
     void print(std::ostream& myCout) const;

public:

    ///   get / set the pointer to a L1GtCondition
    inline const ExternalTemplate* gtExternalTemplate() const {
        return m_gtExternalTemplate;
    }

    void setGtExternalTemplate(const ExternalTemplate*);

    ///   get / set the pointer to uGt GtBoard
    inline const GtBoard* getuGtB() const {
        return m_uGtB;
    }

    void setuGtB(const GtBoard*);

private:

    /// copy function for copy constructor and operator=
    void copy(const ExternalCondition& cp);

private:

    /// pointer to a ExternalTemplate
    const ExternalTemplate* m_gtExternalTemplate;

    /// pointer to uGt GtBoard, to be able to get the trigger objects
    const GtBoard* m_uGtB;

};

}
#endif