// Author: Akira Kudo

#include <initializer_list>
#include <string>
#include <set>

#include "src/graphImplementation/vertices/VariableVertex.h"

GraphImplementation::VariableVertex::VariableVertex(std::string name, std::initializer_list<int> initialDomain) 
    : Vertex(name), domain(std::set<int>(initialDomain)) 
{};

GraphImplementation::VariableVertex::VariableVertex(std::string name, std::set<int> initialDomain) 
    : Vertex(name), domain(initialDomain) 
{};

GraphImplementation::VariableVertex::~VariableVertex()
{
    
};

// adds a value to this vertex's domain
void GraphImplementation::VariableVertex::addToDomain(int val) {
    this->domain.insert(val);
}

void GraphImplementation::VariableVertex::addToDomain(std::set<int> values) {
    for (int val : values) this->domain.insert(val);
}

void GraphImplementation::VariableVertex::addToDomain(std::initializer_list<int> values) {
    for (int val : values) this->domain.insert(val);
}

// removes a value from this vertex's domain
void GraphImplementation::VariableVertex::removeFromDomain(int val) {
    this->domain.erase(val);
}

void GraphImplementation::VariableVertex::removeFromDomain(std::set<int> values) {
    for (int val : values) this->domain.erase(val);
}

void GraphImplementation::VariableVertex::removeFromDomain(std::initializer_list<int> values) {
    for (int val : values) this->domain.erase(val);
}