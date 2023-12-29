// Author: Akira Kudo

#include <string>
#include <tuple>
#include <vector>

#include "src/CSPSolver/CSPSolver.h"

using GraphImplementation::Graph, GraphImplementation::VariableVertex;

CSPSolver::CSPSolver::CSPSolver() {

};

CSPSolver::CSPSolver::~CSPSolver()
{

};

// create a CSP graph for a given problem - using a CLI?
void CSPSolver::CSPSolver::createCspGraph()
{

};

// save a created CSP graph
void CSPSolver::CSPSolver::saveCspGraph(Graph graph, std::string saveDir)
{

};

// load a CSP graph
void CSPSolver::CSPSolver::loadCspGraph(std::string loadDir)
{

};

// run arc consistency and return the answer
std::vector<VariableVertex> CSPSolver::CSPSolver::arcConsistency(Graph graph)
{

};

// run DFS with pruning and return the answer
std::vector<VariableVertex> CSPSolver::CSPSolver::depthFirstSearchWithPruning(Graph graph)
{

};

// ###########################################################################
// PRIVATE FUNCTIONS

// run a single step of arc consistency on given Graph and list of arcs arranged in given Frontier;
// using differing Frontiers might change the runtime & efficiency of the process
template <typename Frontier>
std::tuple<Graph, Frontier> CSPSolver::CSPSolver::singleArcConsistencyStep(Graph graph, Frontier frontier)
{

};