// Author: Akira Kudo
// Description: A program that implements a CSP solver object by creating a 
//  CSP graph and performing arc consistency & DFS with pruning.

// functionalities I want:
// - create a CSP problem
// - save / load a CSP problem
// - solve the CSP problem using arc consistency, while returning:
//   -> any answers found, or that there is no answer
//   -> further steps of arc consistency required (domain splitting)
//   -> switching to DFS from there
//   & optionally:
//   -> a log of the reasoning employed at each step
// - solve the CSP problem using DFS with pruning, while returning:
//   -> any anwers found, or that there is no answer

#ifndef CSPSOLVER_H
#define CSPSOLVER_H

#include <string>
#include <tuple>
#include <vector>

#include "src/cspSolver/CSPGraph.h"
#include "src/cspSolver/frontier/Frontier.h"
#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

// friending from test class so that private functions are testable
namespace _unit_test_befriender
{
    // Forward declaration for befriending 
    struct TestBefriender;
}

namespace CSPSolverImplementation 
{

    class CSPSolver 
    {
        private:
            CSPGraph cspGraph;
            // befriend TestBefriender to allow testing of private functions
            friend struct _unit_test_befriender::TestBefriender;
            
            // populates the given frontier with the set of all arcs to be checked given a CSPGraph
            // used at the beginning when running arc consistency
            void getAllToDoArcs(Frontier& frontier, const CSPGraph& graph);
            // populates the given frontier with the set of all arcs to be checked, given 
            // we just checked a certain ARC and reduced the domain of the main variable
            void getAllCheckAgainArcs(Frontier& frontier, const CSPGraph& graph, const ARC& arc);
            // run one arc consistency step on CSPGraph and arcs in Frontier;
            // using differing Frontiers might change the runtime & efficiency of the process
            void singleArcConsistencyStep(CSPGraph& graph, Frontier& frontier);
            // checks the existence of a unique solution, returned as vector of Variables with unique domain
            // also return a boolean of "true" if we have a unique / no answer, false if indeterminate
            std::tuple<std::vector<GraphImplementation::VariableVertex>, bool> 
            checkAnswer(const CSPGraph& graph);
            // splits domain of specific variable and returns all generated graphs
            std::vector<CSPGraph> splitDomain(const CSPGraph& graph, GraphImplementation::VariableVertex* vv);

        public:
            CSPSolver();
            ~CSPSolver();

            // save a created CSP graph
            // *not implemented yet; have to find a way to serialize 
            // constraints first, and I don't think we need that yet
            void saveCspGraph(const CSPGraph& graph, std::string saveDir);
            // load a CSP graph
            // *not implemented yet; have to find a way to serialize 
            // constraints first, and I don't think we need that yet
            void loadCspGraph(std::string loadDir);

            // run arc consistency and return all possible answers
            std::vector<std::vector<GraphImplementation::VariableVertex>> arcConsistency(CSPGraph graph);
            
            // run DFS with pruning and return all possible answers
            std::vector<std::vector<GraphImplementation::VariableVertex>> depthFirstSearchWithPruning(CSPGraph graph);

            // creates and returns a CSPGraph
            static CSPGraph createCspGraph();
    };

};

#endif