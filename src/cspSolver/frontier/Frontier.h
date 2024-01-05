// Author: Akira Kudo
// Description: Implements a frontier object acting as container for 
//  arc consistency to control the order in which arcs are checked.

#ifndef FRONTIER_H
#define FRONTIER_H

#include <queue>
#include <vector>

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

#include "src/cspSolver/CSPGraph.h"
#include "src/cspSolver/ARC.h"

namespace CSPSolverImplementation
{
    class Frontier 
    {
    public:
        enum FrontierMode { QueueMode }; // could add PriorityQueueMode

        Frontier(FrontierMode mode);
        ~Frontier();
        // push an arc that is an ARC struct to frontier
        void push(CSPSolverImplementation::ARC arc);
        // pop an arc that is an ARC struct from the frontier
        // REQUIRES that this frontier isn't empty - or does undefined behavior!
        CSPSolverImplementation::ARC pop();
        // mostly simple getter
        size_t size() const { return (this->unaryFrontier.size() + this->nonUnaryFrontier.size()); }
        bool empty() const { return (this->unaryFrontier.empty() && this->nonUnaryFrontier.empty()); }
    
    private:
        FrontierMode mode;
        // stores all unary constraints
        std::queue<CSPSolverImplementation::ARC> unaryFrontier;
        // stores all non-unary constraints
        std::queue<CSPSolverImplementation::ARC> nonUnaryFrontier;
    };
}

#endif