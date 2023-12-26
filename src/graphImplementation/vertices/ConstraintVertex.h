// Author: Akira Kudo
// Description: Implements constraint vertices in our CSP graph corresponding to 
//  the constraints put on domains of variables.

#ifndef GRAPHIMPLEMENTATION_VERTICES_CONSTRAINTVERTEX_H
#define GRAPHIMPLEMENTATION_VERTICES_CONSTRAINTVERTEX_H

#include <functional>
#include <initializer_list>

#include "src/graphImplementation/vertices/VariableVertex.h"
#include "src/graphImplementation/vertices/Vertex.h"

namespace GraphImplementation
{
    class ConstraintVertex : Vertex
    {
        private:
            std::function<bool(VariableVertex, std::initializer_list<VariableVertex>)> pred;

        public:
            ConstraintVertex(std::function<bool(VariableVertex, std::initializer_list<VariableVertex>)> pred);
            ~ConstraintVertex();
            // Checks whether the constraint is met for mainVar given varList.
            bool constraintIsMet(VariableVertex mainVar, std::initializer_list<VariableVertex> varList);
    };
};

#endif