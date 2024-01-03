// Author: Akira Kudo

#include <initializer_list>
#include <memory>
#include <tuple>
#include <set>
#include <string>
#include <vector>

#include <iostream>

#include "src/cspSolver/CSPGraph.h"
#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using GraphImplementation::ConstraintVertex, GraphImplementation::VariableVertex;

CSPSolverImplementation::CSPGraph::CSPGraph() {

};

CSPSolverImplementation::CSPGraph::~CSPGraph()
{

};

// returns a pointer to a constraint vertex with the given name, or nullptr if it doesn't exist
GraphImplementation::ConstraintVertex* CSPSolverImplementation::CSPGraph::get_constraint(std::string name)
{
    auto name_position = cv_map.find(name);
    if (name_position == cv_map.end()) return nullptr;
    // TODO questionable design choice to make it return a reference, since
    // this ptr becomes invalid as soon as this CSPGraph object goes out of scope
    // Rethink what I need to accomplish with the get_constraint function and recreate
    return &(name_position->second);
};

// returns a pointer to a variable vertex with the given name, or nullptr if it doesn't exist
GraphImplementation::VariableVertex* CSPSolverImplementation::CSPGraph::get_variable(std::string name)
{
    auto name_position = vv_map.find(name);
    if (name_position == vv_map.end()) return nullptr;
    // // TODO questionable design choice to make it return a reference, since
    // // this ptr becomes invalid as soon as this CSPGraph object goes out of scope
    // // Rethink what I need to accomplish with the get_constraint function and recreate
    // else return &(name_position->second);
    return &(name_position->second);
};

// returns whether any vertex of given name is in the graph
bool CSPSolverImplementation::CSPGraph::contains_vertex(std::string name)
{
    return (vv_map.find(name) != vv_map.end() || cv_map.find(name) != cv_map.end());
};

// return if vertices of given names are adjacent
bool CSPSolverImplementation::CSPGraph::adjacent(std::string name1, std::string name2)
{
    // find both vertices searching in cv_map first
    auto adjacent_pair = find_adjacent_vertex_pair_from_name(name1, name2);
    // if we didn't find a pair successfully, exit
    if (!std::get<2>(adjacent_pair)) return false;
    // otherwise we know the names of vertices we are checking for
    VariableVertex& vv = vv_map.at(std::get<0>(adjacent_pair));
    ConstraintVertex& cv = cv_map.at(std::get<1>(adjacent_pair));
    
    return this->Graph::adjacent(vv, cv);
};

// return all constraint neighbors of a variable vertex with given name
std::vector<GraphImplementation::ConstraintVertex*> CSPSolverImplementation::CSPGraph::get_constraint_neighbors(std::string name)
{
    // if there's no variable with given name, return an empty vector
    if (vv_map.find(name) == vv_map.end()) return std::vector<GraphImplementation::ConstraintVertex*>();
    // otherwise, first find the corresponding vertex reference from vv_map
    auto& vv_reference = vv_map.at(name);
    // then obtain neighbors by passing the reference to Graph::neighbors, 
    // then dynamic casting (we know those are ConstraintVertex objects)
    std::vector<ConstraintVertex*> returned;
    for (auto element : this->Graph::neighbors(vv_reference)) 
    {
        returned.push_back(dynamic_cast<ConstraintVertex*>(element));
    }
    return returned;
};

// return all variable neighbors of a constraint vertex with given name
std::vector<GraphImplementation::VariableVertex*> CSPSolverImplementation::CSPGraph::get_variable_neighbors(std::string name)
{
    // if there's no constraint with given name, return an empty vector
    if (cv_map.find(name) == cv_map.end()) return std::vector<GraphImplementation::VariableVertex*>();
    // otherwise, first find the corresponding vertex reference from cv_map
    auto& cv_reference = cv_map.at(name);
    // then obtain neighbors by passing the reference to Graph::neighbors, 
    // then dynamic casting (we know those are VariableVertex objects)
    std::vector<VariableVertex*> returned;
    for (auto element : this->Graph::neighbors(cv_reference)) 
    {
        returned.push_back(dynamic_cast<VariableVertex*>(element));
    }
    return returned;
};
        
// adds a constraint vertex with given name and predicate to the graph
// * Inserting vertex with existing name doesn't do anything,
//   even when the older one was a VariableVertex
void CSPSolverImplementation::CSPGraph::add_constraint(
    std::string name, 
    std::function<bool(int, std::initializer_list<GraphImplementation::VariableVertex>)> pred,
    std::string description)
{
    // first check for any existing vertex with the same name, and if there is, do nothing
    if (contains_vertex(name)) return;
    // otherwise, create the new constraint vertex
    ConstraintVertex new_cv = ConstraintVertex(name, pred, description);
    // copy the resulting object into the cv_map first
    cv_map.emplace(name, new_cv);
    // then access that object's reference, adding it to the Graph to be controlled
    auto& cv_reference = cv_map.at(name);
    this->Graph::add_vertex(cv_reference);
};

// adds a variable vertex with given name and domain to the graph
// * Inserting vertex with existing name doesn't do anything,
//   even when the older one was a ConstraintVertex        
void CSPSolverImplementation::CSPGraph::add_variable(std::string name, std::initializer_list<int> domain) {
    this->add_variable(name, std::set<int>(domain));
};

void CSPSolverImplementation::CSPGraph::add_variable(std::string name, std::set<int> domain)
{
    // first check for any existing vertex with the same name, and if there is, do nothing
    if (contains_vertex(name)) return;
    // otherwise, create the new variable vertex
    VariableVertex new_vv = VariableVertex(name, std::move(domain));
    // copy the resulting object into the vv_map first
    vv_map.emplace(name, new_vv);
    // then access that object's reference, adding it to the Graph to be controlled
    auto& vv_reference = vv_map.at(name);
    this->Graph::add_vertex(vv_reference);
};

// removes vertex with given name if there, as well as edges connected to it
void CSPSolverImplementation::CSPGraph::remove_vertex(std::string name) {
    // first find vertex with given name
    // search through vv_map first
    auto position_in_vv_map = vv_map.find(name);
    
    // if not found
    if (position_in_vv_map == vv_map.end()) {
        // search in cv_map next
        auto position_in_cv_map = cv_map.find(name);
        // if not found in cv_map either, skip
        if (position_in_cv_map == cv_map.end()) return;

        // otherwise, remove our ConstraintVertex of interest from Graph
        auto& cv_to_remove = position_in_cv_map->second;
        this->Graph::remove_vertex(cv_to_remove);
        // then from cv_map
        cv_map.erase(name);
    } else { //otherwise, we've found the vertex in vv_map
        // remove our VariableVertex of interest from Graph
        auto& vv_to_remove = position_in_vv_map->second;
        this->Graph::remove_vertex(vv_to_remove);
        // then from vv_map
        vv_map.erase(name);
    }
};

// adds edge between given variable vertex and constraint vertex identified by name, if not there
void CSPSolverImplementation::CSPGraph::add_edge(std::string vv_name, std::string cv_name) {
    // we first obtain if this is a pair of constraint and vertex
    // vertices are adjacent as such in a CSP Graph
    auto adjacent_pair = find_adjacent_vertex_pair_from_name(vv_name, cv_name);
    // if it isn't a valid pair or we didn't find vertices, do nothing
    if (!std::get<2>(adjacent_pair)) return;

    // otherwise
    VariableVertex& vv = vv_map.at(std::get<0>(adjacent_pair));
    ConstraintVertex& cv = cv_map.at(std::get<1>(adjacent_pair));
    // add edge using Graph functionality
    this->Graph::add_edge(vv, cv, {});
};

// removes edge between given variable vertex and constraint vertex identified by name, if there
void CSPSolverImplementation::CSPGraph::remove_edge(std::string vv_name, std::string cv_name) {
    // first check if given names are a valid vertex-constraint pair
    auto adjacent_pair = find_adjacent_vertex_pair_from_name(vv_name, cv_name);
    // if it isn't a valid pair or we didn't find vertices, do nothing
    if (!std::get<2>(adjacent_pair)) return;

    // otherwise
    VariableVertex& vv = vv_map.at(std::get<0>(adjacent_pair));
    ConstraintVertex& cv = cv_map.at(std::get<1>(adjacent_pair));
    // remove edge using Graph functionality
    this->Graph::remove_edge(vv, cv);
}; 


// ###################
// PRIVATE FUNCTIONS
// given two names assumed adjacent vertices, return a tuple:
// <name of variable, name of constraint, if such pair was found>
std::tuple<std::string, std::string, bool> 
CSPSolverImplementation::CSPGraph::find_adjacent_vertex_pair_from_name(std::string name1, std::string name2)
{
    std::string vv_name = "";
    std::string cv_name = "";
    bool found_valid_pair = true; 
    // first search for name1
    // if we find it inside vv_map:
    if (vv_map.find(name1) != vv_map.end()) {
        vv_name = name1;
        
        // in CSP graph, variable vertices are only adjacent to constraints
        if (cv_map.find(name2) == cv_map.end()) {
            found_valid_pair = false;
        } else {
            cv_name = name2;
        }

    } else if (cv_map.find(name1) != cv_map.end()) {
        cv_name = name1;
        
        // in CSP graph, constraint vertices are only adjacent to variables
        if (vv_map.find(name2) == vv_map.end()) {
            found_valid_pair = false;
        } else {
            vv_name = name2;
        }
    } else { //at this point, name1 isn't in graph - so found !found_valid_pair
        found_valid_pair = false;
    }

    return std::tuple<std::string, std::string, bool> { vv_name, cv_name, found_valid_pair };
};

// NON-MEMBER FUNCTION FOR PRINGINT CSPGraph
// friend ostream& operator<<(ostream& os, TestClass const & tc) {
//     return os << "I'm a friend of the class, msg=" << tc.msg << endl;
// }