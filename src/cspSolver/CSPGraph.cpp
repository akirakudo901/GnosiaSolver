// Author: Akira Kudo

#include <initializer_list>
#include <string>
#include <vector>

#include "src/cspSolver/CSPGraph.h"
#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

CSPSolverImplementation::CSPGraph::CSPGraph() {

};

CSPSolverImplementation::CSPGraph::~CSPGraph()
{

};

// returns a pointer to a constraint vertex with the given name, or nullptr if it doesn't exist
GraphImplementation::ConstraintVertex* CSPSolverImplementation::CSPGraph::get_constraint(std::string name)
{
    // stub
    return nullptr;
};

// returns a pointer to a variable vertex with the given name, or nullptr if it doesn't exist
GraphImplementation::VariableVertex* CSPSolverImplementation::CSPGraph::get_variable(std::string name)
{
    // stub
    return nullptr;
};

// returns whether any vertex of given name is in the graph
bool CSPSolverImplementation::CSPGraph::contains_vertex(std::string name)
{
    // stub
    return false;
};

// return if vertices of given names are adjacent
bool CSPSolverImplementation::CSPGraph::adjacent(std::string name1, std::string name2)
{
    //stub
    return false;
};

// return all constraint neighbors of a variable vertex with given name
std::vector<GraphImplementation::ConstraintVertex*> CSPSolverImplementation::CSPGraph::get_constraint_neighbors(std::string name)
{
    // stub
    return std::vector<GraphImplementation::ConstraintVertex*>();
};

// return all variable neighbors of a constraint vertex with given name
std::vector<GraphImplementation::VariableVertex*> CSPSolverImplementation::CSPGraph::get_variable_neighbors(std::string name)
{
    // stub
    return std::vector<GraphImplementation::VariableVertex*>();
};
        
// adds a constraint vertex with given name and predicate to the graph
// * Inserting vertex with existing name doesn't do anything,
//   even when the older one was a VariableVertex
void CSPSolverImplementation::CSPGraph::add_constraint(std::string name, std::function<bool(int, std::initializer_list<GraphImplementation::VariableVertex>)> pred)
{
    // stub
};

// adds a variable vertex with given name and domain to the graph
// * Inserting vertex with existing name doesn't do anything,
//   even when the older one was a ConstraintVertex        
void CSPSolverImplementation::CSPGraph::add_variable(std::string name, std::initializer_list<int> domain) {
    // stub
};

// removes vertex with given name if there, as well as edges connected to it
void CSPSolverImplementation::CSPGraph::remove_vertex(std::string name) {
    // stub
};

// adds edge between given variable vertex and constraint vertex identified by name, if not there
void CSPSolverImplementation::CSPGraph::add_edge(std::string vv_name, std::string cv_name) {
    // stub
};

// removes edge between given variable vertex and constraint vertex identified by name, if there
void CSPSolverImplementation::CSPGraph::remove_edge(std::string vv_name, std::string cv_name) {
    // stub
}; 