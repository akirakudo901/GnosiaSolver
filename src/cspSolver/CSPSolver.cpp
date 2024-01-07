// Author: Akira Kudo

#include <string>
#include <tuple>
#include <vector>

#include "src/cspSolver/CSPGraphCreator.h"
#include "src/cspSolver/CSPSolver.h"
#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using GraphImplementation::Graph, GraphImplementation::VariableVertex;

CSPSolverImplementation::CSPSolver::CSPSolver() 
{

};

CSPSolverImplementation::CSPSolver::~CSPSolver()
{

};

// save a created CSP graph
// * not implemented yet; have to find a way to serialize 
// constraints first, and I don't think we need that yet
void CSPSolverImplementation::CSPSolver::saveCspGraph(const CSPGraph& graph, std::string saveDir)
{

};

// load a CSP graph
// *not implemented yet; have to find a way to serialize 
// constraints first, and I don't think we need that yet
void CSPSolverImplementation::CSPSolver::loadCspGraph(std::string loadDir)
{
    
};            

// run arc consistency and return all possible answers
std::vector<std::vector<VariableVertex>> CSPSolverImplementation::CSPSolver::arcConsistency(CSPGraph graph)
{
    // we initially generate all arc checks by:
    //  - iterating through all constraint vertex adjacency list
    //  - for every constraint existing, add a pair of Vertex, otherVertices, Constraint to toDoArc
    // toDoArc will hold pairs of VariableVertex to every other VariableVertex concerned,
    //  as well as the Constrait vertex
    // at each point, one such pair is extracted
    // we then iterate on the domain of the main variable, checking if constraint is met
    //  relative to all the other variables
    // for every constraint that aren't met, we remove the domain, then add the relative arcs 
    //  back to the 

    // stub
    return std::vector<std::vector<VariableVertex>>();
};

// run DFS with pruning and return all possible answers
std::vector<std::vector<VariableVertex>> CSPSolverImplementation::CSPSolver::depthFirstSearchWithPruning(CSPGraph graph)
{
    // stub maybe implement another time
    return std::vector<std::vector<VariableVertex>>();
};

// create a CSP graph for a given problem - using a CLI?
CSPSolverImplementation::CSPGraph CSPSolverImplementation::CSPSolver::createCspGraph()
{
    CSPGraphCreator cspGc;
    return cspGc.StartCSPGraphCreator();
};

// ###########################################################################
// PRIVATE FUNCTIONS

// populates the given frontier with the set of all arcs to be checked given a CSPGraph
// used at the beginning when running arc consistency
void CSPSolverImplementation::CSPSolver::getAllToDoArcs(
    CSPSolverImplementation::Frontier& frontier, 
    const CSPSolverImplementation::CSPGraph& graph) 
{
    // for every constraint vertex in the graph:
    // generate an arc choosing each varaible vertex in their adjacency list, 
    // and the remaining in the other variable list

    // add them to the frontier
};


// populates the given frontier with the set of all arcs to be checked, given 
// we just checked a certain ARC and reduced the domain of the main variable
void CSPSolverImplementation::CSPSolver::getAllCheckAgainArcs(Frontier& frontier, const CSPGraph& graph, const ARC& arc)
{
    // iterate over all neighbors of mainVar in arc
    // for each such constraint neighbor C, iterate over its variable neighbors V
    // create an arc containing 
    //  1) V unequal to mainVar as main variable
    //  2) remaining Vs as var_list
    //  3) C as constraint
    // add it to the frontier
};

// run one arc consistency step on CSPGraph and arcs in Frontier;
// using differing Frontiers might change the runtime & efficiency of the process
void CSPSolverImplementation::CSPSolver::singleArcConsistencyStep(CSPGraph& graph, Frontier& frontier)
{
    // pop one object from frontier
    // for each domain in the main var, check if constraint is met given other vars
    // if the constraint isn't consistent, remove that domain from the mix
    // add any arc we need to double check:
    //  if the arc checked now was <X,Y>, then add back every arc <Z,X> where Z != Y.
    // return resulting CSPGraph and Frontier
};

// checks the existence of a unique solution, returned as vector of Variables with unique domain
// also return a boolean of "true" if we have a unique / no answer, false if indeterminate
std::tuple<std::vector<GraphImplementation::VariableVertex>, bool>
CSPSolverImplementation::CSPSolver::checkAnswer(const CSPSolverImplementation::CSPGraph& graph)
{
    // for every variable in graph, check domain size
    // if any is 0, return an empty vector
    // if any is >= 2, return the set of possible values & false bool
    // if all variable have unique domain value, return answer & true bool
    return std::tuple<std::vector<GraphImplementation::VariableVertex>, bool> (
        std::vector<GraphImplementation::VariableVertex>(), false
    );
};


// splits domain of specific variable and returns all generated graphs
std::vector<CSPSolverImplementation::CSPGraph> 
CSPSolverImplementation::CSPSolver::splitDomain(const CSPSolverImplementation::CSPGraph& graph, 
                                                GraphImplementation::VariableVertex* vv) 
{
    // find domain of given vv
    // 1 - if domain is empty: return empty vector
    // 2 - if domain has more than one value:
    // for elements in given vv:
    //  a) copy graph
    //  b) remove vv (by name)
    //  c) add back vv but with only one value as domain
    //  d) add that to returned list of graphs
    return std::vector<CSPSolverImplementation::CSPGraph>();
};