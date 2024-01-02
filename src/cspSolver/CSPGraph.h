// Author: Akira Kudo
// Description: Implements a CSP Graph object which inherits from Graph.
//  Allows the creation of a graph by giving variable names & domains, 
//  and edges by giving varaible names.

#ifndef CSPGRAPH_H
#define CSPGRAPH_H

#include <functional>
#include <initializer_list>
#include <string>

#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

namespace CSPSolverImplementation
{
    class CSPGraph : private GraphImplementation::Graph
    {
    private:
        // maps each variable vertex by their unique names
        // * Inserting vertex with existing name doesn't do anything,
        //   even when the older one was a ConstraintVertex
        std::unordered_map<std::string, GraphImplementation::VariableVertex> vv_map;
        // maps each constraint vertex by their unique names
        // * Inserting vertex with existing name doesn't do anything,
        //   even when the older one was a VariableVertex
        std::unordered_map<std::string, GraphImplementation::ConstraintVertex> cv_map;
        
    public:
        CSPGraph();
        ~CSPGraph();

        // returns a pointer to a constraint vertex with the given name, or nullptr if it doesn't exist
        GraphImplementation::ConstraintVertex* get_constraint(std::string name);
        // returns a pointer to a variable vertex with the given name, or nullptr if it doesn't exist
        GraphImplementation::VariableVertex* get_variable(std::string name);
        // returns whether any vertex of given name is in the graph
        bool contains_vertex(std::string name);
        // return if vertices of given names are adjacent
        bool adjacent(std::string name1, std::string name2);
        // return all constraint neighbors of a variable vertex with given name
        std::vector<GraphImplementation::ConstraintVertex*> get_constraint_neighbors(std::string name);
        // return all variable neighbors of a constraint vertex with given name
        std::vector<GraphImplementation::VariableVertex*> get_variable_neighbors(std::string name);
        // adds a constraint vertex with given name and predicate to the graph
        // * Inserting vertex with existing name doesn't do anything,
        //   even when the older one was a VariableVertex
        void add_constraint(std::string name, std::function<bool(int, std::initializer_list<GraphImplementation::VariableVertex>)> pred);
        // adds a variable vertex with given name and domain to the graph
        // * Inserting vertex with existing name doesn't do anything,
        //   even when the older one was a ConstraintVertex
        void add_variable(std::string name, std::initializer_list<int> domain);
        // removes vertex with given name if there, as well as edges connected to it
        void remove_vertex(std::string name);
        // adds edge between given variable vertex and constraint vertex identified by name, if not there
        void add_edge(std::string vv_name, std::string cv_name);
        // removes edge between given variable vertex and constraint vertex identified by name, if there
        void remove_edge(std::string vv_name, std::string cv_name);      
    };
}

#endif