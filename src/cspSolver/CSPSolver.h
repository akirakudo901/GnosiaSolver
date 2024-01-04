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

#include "src/cspSolver/CSPGraph.h"
#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

namespace CSPSolverImplementation 
{

    class CSPSolver 
    {
        private:
            GraphImplementation::Graph cspGraph;
            // run a single step of arc consistency on given Graph and list of arcs arranged in given Frontier;
            // using differing Frontiers might change the runtime & efficiency of the process
            template <typename Frontier>
            std::tuple<GraphImplementation::Graph, Frontier> singleArcConsistencyStep(GraphImplementation::Graph graph, Frontier frontier);

        public:
            CSPSolver();
            ~CSPSolver();

            // save a created CSP graph
            // *not implemented yet; have to find a way to serialize 
            // constraints first, and I don't think we need that yet
            void saveCspGraph(GraphImplementation::Graph graph, std::string saveDir);
            // load a CSP graph
            // *not implemented yet; have to find a way to serialize 
            // constraints first, and I don't think we need that yet
            void loadCspGraph(std::string loadDir);

            // run arc consistency and return the answer
            std::vector<GraphImplementation::VariableVertex> arcConsistency(GraphImplementation::Graph graph);
            
            // run DFS with pruning and return the answer
            std::vector<GraphImplementation::VariableVertex> depthFirstSearchWithPruning(GraphImplementation::Graph graph);

            // creates and returns a CSPGraph
            static CSPGraph createCspGraph();
    };

};

#endif