// Author: Akira Kudo
// Description: A command line user interface implemented using the standard library.
//  Allows the user to specify any graphs by interacting through the CLI.

#ifndef CSPGRAPHCREATOR_H
#define CSPGRAPHCREATOR_H

#include <iostream>
#include <string>

#include "src/cspSolver/CSPGraph.h"
#include "src/graphImplementation/Graph.h"

namespace CSPSolverImplementation 
{
    struct CSPGraphCreator {
        // Value-Defintions of the different String values
        enum StringValue 
        { evAddConstraint, evAddVariable, evAddEdge, evRemoveVertex, evRemoveEdge, evExit };

        //this maps string inputs to enumeration StringValue
        std::unordered_map<std::string, StringValue> stringToEnum = {
            {"AC", evAddConstraint}, {"ADDCONSTRAINT", evAddConstraint},
            {"AV", evAddVariable}, {"ADDVARIABLE", evAddVariable},
            {"AE", evAddEdge}, {"ADDEDGE", evAddEdge},
            {"RV", evRemoveVertex}, {"REMOVEVERTEX", evRemoveVertex}, 
            {"RE", evRemoveEdge}, {"REMOVEEDGE", evRemoveEdge},
            {"E", evExit}, {"EXIT", evExit}, {"END", evExit}
        };

        bool keepGoing;
        CSPGraph returnedGraph;

        // calling this method initiates graph creation in the CLI
        CSPGraph StartCSPGraphCreator();

        // specifies how CLI inputs should be handled
        void process(std::string const & line);
    };
}
#endif