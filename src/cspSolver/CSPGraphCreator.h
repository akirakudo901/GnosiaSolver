// Author: Akira Kudo
// Description: A command line user interface implemented using the standard library.
//  Allows the user to specify any graphs by interacting through the CLI.

#include <iostream>
#include <string>

#include "src/graphImplementation/Graph.h"

namespace CSPSolverImplementation 
{
    // calling this method initiates graph creation in the CLI
    GraphImplementation::Graph StartCSPGraphCreator();

    // specifies how CLI inputs should be handled
    void process(std::string const & line, GraphImplementation::Graph& g);
}