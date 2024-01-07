// Author: Akira Kudo
// Description: Implements constraint vertices in our CSP graph corresponding to 
//  the constraints put on domains of variables.

#ifndef GRAPHIMPLEMENTATION_VERTICES_CONSTRAINTVERTEX_H
#define GRAPHIMPLEMENTATION_VERTICES_CONSTRAINTVERTEX_H

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "src/graphImplementation/vertices/VariableVertex.h"
#include "src/graphImplementation/vertices/Vertex.h"

namespace GraphImplementation
{
    class ConstraintVertex : public Vertex
    {
        private:
            std::function<bool(int, std::vector<VariableVertex*>)> pred;
            std::string name;
            // description aims to make printing of ConstraintVertex clearer;
            // instead of us taking pred apart, we expect you to specify info of what
            // pred does in description in case you want descriptive info when printing
            std::string description;

        public:
            ConstraintVertex(std::string, 
                             std::function<bool(int, std::vector<VariableVertex*>)> pred,
                             std::string description="This is the default description.");
            ~ConstraintVertex();
            // checks whether the constraint is met for mainVar given varList.
            bool constraintIsMet(int mainVal, std::vector<VariableVertex*> varList) const;

            // getters
            using Vertex::getName;
        
            // Example predicates that can be useful:
            // checks if given domains allow the existence of n or less of the checkedDomain value
            static std::function<bool(int, std::vector<VariableVertex*>)> lesserOrEqualToN(int checkedDomain, int n);
            // checks if given domains allow the existence of n or more of the checkedDomain value
            static std::function<bool(int, std::vector<VariableVertex*>)> greaterOrEqualToN(int checkedDomain, int n);
            // checks if given domains allow the existence of exactly n of the checkedDomain value
            static std::function<bool(int, std::vector<VariableVertex*>)> exactlyN(int checkedDomain, int n);

            // overwrite << operator
            friend std::ostream& operator<<(std::ostream& os, const ConstraintVertex& cv) {
                os << "NAME: " << cv.Vertex::name << "\n - Description: " << cv.description;
                return os;
            };
    };
};

#endif