// Author: Akira Kudo

#include <queue>

#include "src/cspSolver/ARC.h"
#include "src/cspSolver/frontier/Frontier.h"

CSPSolverImplementation::Frontier::Frontier(CSPSolverImplementation::Frontier::FrontierMode mode)
{
    this->mode = mode;
    this->unaryFrontier = std::queue<CSPSolverImplementation::ARC>();
    this->nonUnaryFrontier = std::queue<CSPSolverImplementation::ARC>();
};

CSPSolverImplementation::Frontier::~Frontier()
{

};

// push an arc that is an ARC struct to frontier
void CSPSolverImplementation::Frontier::push(CSPSolverImplementation::ARC arc) 
{
    switch (this->mode) {
        // if mode is QueueMode
        case QueueMode:
            // check the size of variable list in arc
            // if it is none, we have a unary constraint
            if (arc.other_var_list.empty())
                unaryFrontier.push(arc);
            // otherwise, we have a non-unary constraint
            else 
                nonUnaryFrontier.push(arc);
            break;
    }
    // might implement PriorityQueueMode as well
};

// pop an arc that is an ARC struct from the frontier
// REQUIRES that this frontier isn't empty - or does undefined behavior!
CSPSolverImplementation::ARC CSPSolverImplementation::Frontier::pop() 
{
    ARC returned;
    switch (this->mode) {
        // if mode is QueueMode
        case QueueMode:
            // if we still have unary constraints, return that first
            if (!unaryFrontier.empty()) {
                returned = unaryFrontier.front();
                unaryFrontier.pop();
                return returned;
            // otherwise, we return a non-unary constraint
            } else {
                returned = nonUnaryFrontier.front();
                nonUnaryFrontier.pop();
                return returned;
            }
        default:
            return returned;
    }
    // might implement PriorityQueueMode as well
};