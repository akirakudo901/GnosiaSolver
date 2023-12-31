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
    class ConstraintVertex : public Vertex
    {
        private:
            std::function<bool(int, std::initializer_list<VariableVertex>)> pred;

        public:
            ConstraintVertex(std::function<bool(int, std::initializer_list<VariableVertex>)> pred);
            ~ConstraintVertex();
            // checks whether the constraint is met for mainVar given varList.
            bool constraintIsMet(int mainVal, std::initializer_list<VariableVertex> varList);
        
            // Example predicates that can be useful:
            // checks if given domains allow the existence of n or less of the checkedDomain value
            static std::function<bool(int, std::initializer_list<VariableVertex>)> lesserOrEqualToN(int checkedDomain, int n);
            // checks if given domains allow the existence of n or more of the checkedDomain value
            static std::function<bool(int, std::initializer_list<VariableVertex>)> greaterOrEqualToN(int checkedDomain, int n);
            // checks if given domains allow the existence of exactly n of the checkedDomain value
            static std::function<bool(int, std::initializer_list<VariableVertex>)> exactlyN(int checkedDomain, int n);
        
    };
};

#endif