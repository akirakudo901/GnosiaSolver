// Author: Akira Kudo

#include <string>
#include <tuple>
#include <vector>

#include "src/cspSolver/CSPGraphCreator.h"
#include "src/cspSolver/CSPSolver.h"
#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using GraphImplementation::Graph, GraphImplementation::VariableVertex;

CSPSolverImplementation::CSPSolver::CSPSolver() {

};

CSPSolverImplementation::CSPSolver::~CSPSolver()
{

};

// save a created CSP graph
void CSPSolverImplementation::CSPSolver::saveCspGraph(Graph graph, std::string saveDir)
{

};

// load a CSP graph
void CSPSolverImplementation::CSPSolver::loadCspGraph(std::string loadDir)
{

};

// run arc consistency and return the answer
std::vector<VariableVertex> CSPSolverImplementation::CSPSolver::arcConsistency(Graph graph)
{
    // stub
    return std::vector<VariableVertex>();
};

// run DFS with pruning and return the answer
std::vector<VariableVertex> CSPSolverImplementation::CSPSolver::depthFirstSearchWithPruning(Graph graph)
{
    // stub
    return std::vector<VariableVertex>();
};

// create a CSP graph for a given problem - using a CLI?
CSPSolverImplementation::CSPGraph CSPSolverImplementation::CSPSolver::createCspGraph()
{
    CSPGraphCreator cspGc;
    return cspGc.StartCSPGraphCreator();
};

// ###########################################################################
// PRIVATE FUNCTIONS

// run a single step of arc consistency on given Graph and list of arcs arranged in given Frontier;
// using differing Frontiers might change the runtime & efficiency of the process
template <typename Frontier>
std::tuple<Graph, Frontier> CSPSolverImplementation::CSPSolver::singleArcConsistencyStep(Graph graph, Frontier frontier)
{

};