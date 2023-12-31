// Author: Akira Kudo

#include <functional>
#include <initializer_list>

#include "src/graphImplementation/vertices/ConstraintVertex.h"

using std::function, std::initializer_list;

GraphImplementation::ConstraintVertex::ConstraintVertex(function<bool(int, initializer_list<VariableVertex>)> pred)
{
    this->pred = pred;
};

GraphImplementation::ConstraintVertex::~ConstraintVertex()
{

};

// return whether the corresponding constraint is met given the variable vertices
bool GraphImplementation::ConstraintVertex::constraintIsMet(int mainVal, initializer_list<VariableVertex> varList)
{
    return this->pred(mainVal, varList);
};