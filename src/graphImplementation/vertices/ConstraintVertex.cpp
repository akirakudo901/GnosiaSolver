// Author: Akira Kudo

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using std::function, std::vector, GraphImplementation::VariableVertex;

GraphImplementation::ConstraintVertex::ConstraintVertex(
    std::string name, 
    function<bool(int, vector<VariableVertex*>)> pred,
    std::string description)
    : Vertex(name), pred(pred), description(description)
{
    
};


GraphImplementation::ConstraintVertex::ConstraintVertex(
    std::string name, 
    function<bool(int, vector<VariableVertex*>)> pred,
    std::set<int> canBeMainVar,
    std::string description)
    : Vertex(name), pred(pred), description(description), canBeMainVariable(canBeMainVar)
{
    
};

GraphImplementation::ConstraintVertex::~ConstraintVertex()
{

};

// return whether the corresponding constraint is met given the variable vertices
bool GraphImplementation::ConstraintVertex::constraintIsMet(int mainVal, vector<VariableVertex*> varList) const
{
    return this->pred(mainVal, varList);
};

// add new domain to what can be the main variable for this constraint vertex
// functionality for directional edges
void GraphImplementation::ConstraintVertex::allowMainVariable(int newDomain)
{
    this->canBeMainVariable.insert(newDomain);
};

void GraphImplementation::ConstraintVertex::allowMainVariable(std::set<int> newDomains)
{
    for (int d : newDomains) this->canBeMainVariable.insert(d);
};

// remove domain to what can be the main variable for this constraint vertex
// functionality for directional edges
void GraphImplementation::ConstraintVertex::removeMainVariable(int removed)
{
    this->canBeMainVariable.erase(removed);
};

void GraphImplementation::ConstraintVertex::removeMainVariable(std::set<int> removed)
{
    for (int d : removed) this->canBeMainVariable.erase(d);
};

// checks if given domains allow the existence of n or less of the checkedDomain value
// e.g. checkedDomain = werewolf, n = 3: returns false if there has to be more than 
//      3 werewolves in the mix, true otherwise
function<bool(int, vector<VariableVertex*>)> 
GraphImplementation::ConstraintVertex::lesserOrEqualToN(int checkedDomain, int n) 
{
    return [=] (int mainVal, vector<VariableVertex*> varList) {
        // keep count of the number of variables which value has to be checkedDomain
        int hasToBeCheckedDomain = 0;

        // if mainVal is checkedDomain, increment hasToBeCheckedDomain
        if (mainVal == checkedDomain) hasToBeCheckedDomain++;
        
        // otherwise, for every variable which domain is limited to checkedDomain, increment
        for (auto var : varList) {
            auto vDomain = var->getDomain();
            // if domain size is 1 and checkedDomain is in domain, that variable has to be checkedDomain
            if (vDomain.size() == 1 && vDomain.find(checkedDomain) != vDomain.end())
                hasToBeCheckedDomain++;
            // if hastToBeCheckedDomain > n, there cannot n or less checkedDomain
            if (hasToBeCheckedDomain > n) return false;
        }
        // if we've reached this point, the count is smaller or equal to n
        return (hasToBeCheckedDomain <= n);
    };
};

// checks if given domains allow the existence of n or more of the checkedDomain value
// e.g. checkedDomain = werewolf, n = 3: returns false if there has to be less than 
//      3 werewolves in the mix, true otherwise
function<bool(int, vector<VariableVertex*>)> 
GraphImplementation::ConstraintVertex::greaterOrEqualToN(int checkedDomain, int n)
{
    return [=] (int mainVal, vector<VariableVertex*> varList) {
        // keep count of the number of variables which value can be checkedDomain
        int canBeCheckedDomain = 0;

        // if mainVal is checkedDomain, increment canBeCheckedDomain
        if (mainVal == checkedDomain) canBeCheckedDomain++;
        
        // otherwise, for every variable which domain includes checkedDomain, increment
        for (auto var : varList) {
            auto vDomain = var->getDomain();
            // if checkedDomain is in vDomain, increment canBeCheckedDomain
            if (vDomain.find(checkedDomain) != vDomain.end())
                canBeCheckedDomain++;
            // if canBeCheckedDomain >= n, we don't have to check further - return true
            if (canBeCheckedDomain >= n) return true;
        }
        // if we've reached this point, the count is smaller than n, to which we return false
        return (canBeCheckedDomain >= n);

        return false;
    };
};

// checks if given domains allow the existence of exactly n of the checkedDomain value
function<bool(int, vector<VariableVertex*>)> 
GraphImplementation::ConstraintVertex::exactlyN(int checkedDomain, int n)
{
    return [=] (int mainVal, vector<VariableVertex*> varList) {
        // count both the number of variables which value 'can be' / 'have to be' checkedDomain.
        int canBeCheckedDomain = 0;
        int hasToBeCheckedDomain = 0;

        // if mainVal is checkedDomain, increment both canBeCheckedDomain and hasToBeCheckedDomain
        if (mainVal == checkedDomain) {
            canBeCheckedDomain++;
            hasToBeCheckedDomain++;
        }

        // otherwise: 
        // - for every variable which domain includes checkedDomain, increment canBeCheckedDomain
        // - for every variable which domain is limited to checkedDomain, increment hasToBeCheckedDomain
        for (auto var : varList) {
            auto vDomain = var->getDomain();
            // includes checkedDomain
            if (vDomain.find(checkedDomain) != vDomain.end()) {
                canBeCheckedDomain++;
                // on top of that, also has domain limited to be checkedDomain
                if (vDomain.size() == 1) hasToBeCheckedDomain++;
            }

            // after increment, if we have to have more than n of checkedDomains
            // (= hasToBeCheckedDomain > n), we cannot have exactly N
            if (hasToBeCheckedDomain > n) return false;
        }
        // finally return if we could have at least n checkedDomains
        return (hasToBeCheckedDomain <= n && canBeCheckedDomain >= n);

        return false;
    };

};

// checks if given domains allow the existence of the checkedDomain value exactly
// for any number provided in ns 
function<bool(int, vector<VariableVertex*>)> 
GraphImplementation::ConstraintVertex::exactlyN(int checkedDomain, std::set<int> ns)
{
    return [=] (int mainVal, vector<VariableVertex*> varList) {
        // count both the number of variables which value 'can be' / 'have to be' checkedDomain.
        int canBeCheckedDomain = 0;
        int hasToBeCheckedDomain = 0;

        // if mainVal is checkedDomain, increment both canBeCheckedDomain and hasToBeCheckedDomain
        if (mainVal == checkedDomain) {
            canBeCheckedDomain++;
            hasToBeCheckedDomain++;
        }

        // otherwise: 
        // - for every variable which domain includes checkedDomain, increment canBeCheckedDomain
        // - for every variable which domain is limited to checkedDomain, increment hasToBeCheckedDomain
        for (auto var : varList) {
            auto vDomain = var->getDomain();
            // includes checkedDomain
            if (vDomain.find(checkedDomain) != vDomain.end()) {
                canBeCheckedDomain++;
                // on top of that, also has domain limited to be checkedDomain
                if (vDomain.size() == 1) hasToBeCheckedDomain++;
            }

            // after increment, if we have to have checkedDomains more than the max 
            // of ns (= hasToBeCheckedDomain > max(n)), we cannot have exactly N for any N in ns
            //  Also, set.rbegin returns pointer to max of an ordered set
            if (hasToBeCheckedDomain > *ns.rbegin()) return false; 
        }
        // finally, for each value N in ns, check if at least one value N is possible,
        // that is (hasToBeCheckedDomain <= N & canbeCheckedDomain >= N)
        for (int N : ns)
        {
            if (hasToBeCheckedDomain <= N && canBeCheckedDomain >= N) return true;
        }
        return false;
    };

};