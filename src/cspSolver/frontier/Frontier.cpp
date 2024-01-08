// Author: Akira Kudo

#include <queue>

#include "src/cspSolver/ARC.h"
#include "src/cspSolver/frontier/Frontier.h"

CSPSolverImplementation::Frontier::Frontier(CSPSolverImplementation::Frontier::FrontierMode mode)
{
    this->mode = mode;
    this->unaryFrontier = std::queue<CSPSolverImplementation::ARC>();
    this->nonUnaryFrontier = std::queue<CSPSolverImplementation::ARC>();
    this->arc_is_in_frontier = std::unordered_map<std::string, bool>();
};

CSPSolverImplementation::Frontier::~Frontier()
{

};

// push an arc that is an ARC struct to frontier
void CSPSolverImplementation::Frontier::push(CSPSolverImplementation::ARC arc) 
{
    // first check whether we need to insert

    // get the unique name of the arc 
    // TODO: generate_unique_string is O(n) OR MORE, UNSURE OF EFFICIENCY
    std::string arc_name = arc.generate_unique_string();
    // if arc_name is not in map, add a 'true' entry to map 
    if (arc_is_in_frontier.find(arc_name) == arc_is_in_frontier.end())
    {
        arc_is_in_frontier.insert({arc_name, true});
    }
    // if arc_name is in there and indicates false (arc not in frontier),
    // set existence to true
    else if (!arc_is_in_frontier.at(arc_name))
    {
        arc_is_in_frontier.emplace(arc_name, true);
    // otherwise arc is already in map - skip
    }
    else return;


    // then deal with insertion
    switch (this->mode) {
        // if mode is QueueMode
        case QueueMode:
        {
            // check the size of variable list in arc
            // if it is none, we have a unary constraint
            if (arc.other_var_list.empty())
                unaryFrontier.push(arc);
            // otherwise, we have a non-unary constraint
            else 
                nonUnaryFrontier.push(arc);
            break;
        }
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
        {
            // if we still have unary constraints, return that first
            if (!unaryFrontier.empty()) {
                returned = unaryFrontier.front();
                unaryFrontier.pop();
            // otherwise, we return a non-unary constraint
            } else {
                returned = nonUnaryFrontier.front();
                nonUnaryFrontier.pop();
            }
            break;
        }
        default:
            return returned;
    }

    // finally track the fact that we just removed 'returned' from arc_is_in_frontier
    std::string removed_arc_name = returned.generate_unique_string();
    arc_is_in_frontier.erase(removed_arc_name);
    arc_is_in_frontier.emplace(removed_arc_name, false);
    return returned;
    // might implement PriorityQueueMode as well
};