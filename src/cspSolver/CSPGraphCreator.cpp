// Author: Akira Kudo

#include <iostream>
#include <string>

#include "src/cspSolver/CSPGraphCreator.h"
#include "src/graphImplementation/Graph.h"

namespace CSPSolverImplementation 
{
    // calling this method initiates graph creation in the CLI
    GraphImplementation::Graph StartCSPGraphCreator() 
    {
        GraphImplementation::Graph returnedGraph = GraphImplementation::Graph();

        std::cout << "Initiating graph creation.\n";

        for (std::string line; std::cout << "APP > " && std::getline(std::cin, line); )
        {
            if (!line.empty()) { process(line, returnedGraph); }
        }

        std::cout << "Goodbye.\n";
        
        return returnedGraph;
    };

    // specifies how CLI inputs should be handled
    void process(std::string const & line, GraphImplementation::Graph& g)
    {
        /*
        What do you want to do?

        - add a new vertex: name, domain
        - add a new edge between two vertices (by name)
        - remove a vertex by name
        - remove an edge between two vertices
        */
    };
}