// Author: Akira Kudo

#include <iostream>
#include <string>
#include <set>

#include "src/graphImplementation/vertices/VariableVertex.h"

GraphImplementation::VariableVertex::VariableVertex(std::string name, std::set<int> initialDomain)
{
    this->name = name;
    this->domain = initialDomain;
};

GraphImplementation::VariableVertex::~VariableVertex()
{
};

// adds a value to this vertex's domain
void GraphImplementation::VariableVertex::addToDomain(int val) {
    this->domain.insert(val);
}


// removes a value from this vertex's domain
void GraphImplementation::VariableVertex::removeFromDomain(int val) {
    this->domain.erase(val);
}