// Author: Akira Kudo
// Description: Implements variable vertices in our CSP graph corresponding to 
//  individual crew members in the game.

#ifndef GRAPHIMPLEMENTATION_VERTICES_VARIABLEVERTEX_H
#define GRAPHIMPLEMENTATION_VERTICES_VARIABLEVERTEX_H

#include <string>
#include <set>

#include "src/graphImplementation/vertices/Vertex.h"

namespace GraphImplementation
{
    class VariableVertex : public Vertex
    {

    private:
        // Name of vertex.
        std::string name;
        // Domains of vertex: are integers, combined accordingly with
        // enumerations which are specific to a problem we are solving.
        std::set<int> domain;

    public:
        VariableVertex(std::string name, std::set<int> initialDomain);
        ~VariableVertex();
        
        // Adds the value to the domain of the vertex, if not there already.
        void addToDomain(int val);
        // Removes the value from the domain of the vertex, if there already.
        void removeFromDomain(int val);

        // getters
        
        std::string getName() { return this->name; };
        std::set<int> getDomain() { return this->domain; };
        
    };
}

#endif