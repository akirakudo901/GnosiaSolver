// Author: Akira Kudo

#include <string>
#include <tuple>
#include <vector>

#include "src/cspSolver/CSPGraphCreator.h"
#include "src/cspSolver/CSPSolver.h"
#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using GraphImplementation::Graph, GraphImplementation::VariableVertex;

CSPSolverImplementation::CSPSolver::CSPSolver() 
{

};

CSPSolverImplementation::CSPSolver::~CSPSolver()
{

};

// save a created CSP graph
// * not implemented yet; have to find a way to serialize 
// constraints first, and I don't think we need that yet
void CSPSolverImplementation::CSPSolver::saveCspGraph(const CSPGraph& graph, std::string saveDir)
{

};

// load a CSP graph
// *not implemented yet; have to find a way to serialize 
// constraints first, and I don't think we need that yet
void CSPSolverImplementation::CSPSolver::loadCspGraph(std::string loadDir)
{
    
};            

// run arc consistency and return all possible answers
std::vector<std::vector<VariableVertex>> CSPSolverImplementation::CSPSolver::arcConsistency(CSPGraph graph)
{
    std::vector<std::vector<VariableVertex>> to_be_returned;

    Frontier frontier = Frontier(Frontier::QueueMode); //queue mode by default for now
    // we initially generate all arc to be checked using getAllToDoArcs
    getAllToDoArcs(frontier, graph);
    // then call arc consistency trampoline, returning results
    return arcConsistency_trampoline(frontier, graph);
};

// run DFS with pruning and return all possible answers
std::vector<std::vector<VariableVertex>> CSPSolverImplementation::CSPSolver::depthFirstSearchWithPruning(CSPGraph graph)
{
    // stub maybe implement another time
    return std::vector<std::vector<VariableVertex>>();
};

// create a CSP graph for a given problem - using a CLI?
CSPSolverImplementation::CSPGraph CSPSolverImplementation::CSPSolver::createCspGraph()
{
    CSPGraphCreator cspGc;
    return cspGc.StartCSPGraphCreator();
};

// ###########################################################################
// PRIVATE FUNCTIONS

// trampoline for starting a call to arc consistency
// enables recursive calls after domain splitting
std::vector<std::vector<GraphImplementation::VariableVertex>>
CSPSolverImplementation::CSPSolver::arcConsistency_trampoline(
    CSPSolverImplementation::Frontier& frontier, 
    CSPSolverImplementation::CSPGraph& graph)
{
    std::vector<std::vector<VariableVertex>> to_be_returned; //holds answers to be returned
    std::tuple<std::vector<VariableVertex>, bool> checked_answer; //result checking for answers
    
    // we repeatedly call singleArcConsistencyStep, until we deplete the frontier
    do {
        // we check if we've reached a determinate answer
        checked_answer = checkAnswer(graph);
        // if we've reached a determinate answer, break out of loop
        if (std::get<1>(checked_answer)) break;
        // otherwise, apply a single step of arc consistency
        singleArcConsistencyStep(graph, frontier);

    } while (!frontier.empty());

    // check we didn't run out of frontier simultaenously to reaching a determinate answer
    checked_answer = checkAnswer(graph);

    // once out of loop, we have either:
    // 1 - reached a determinate solution (unique / non-existent)
    //     in which case we return the result
    if (std::get<1>(checked_answer))
    {
        if (!std::get<0>(checked_answer).empty())
            to_be_returned.push_back(std::get<0>(checked_answer));
        return to_be_returned;
    }
    
    // 2 - reached an indeterminate state where domain splitting is required
    //     in which case we apply domain splitting then recursively call this function
    // when result is indeterminate, var_with... will never be assigned to a nullptr
    VariableVertex* var_with_domain_size_more_than_one;
    for (std::string vv_name : graph.get_all_variable_names())
    {
        // find a variable with domain size greater than 1
        if (graph.get_variable(vv_name)->getDomain().size() > 1)
        {
            var_with_domain_size_more_than_one = graph.get_variable(vv_name);
            break;
        }
    }

    // split the domain of the variable we found
    std::vector<CSPGraph> subgraphs = splitDomain(
        graph,
        var_with_domain_size_more_than_one
        );

    // for each subgraph obtained by splitting
    for (CSPGraph subg : subgraphs)
    {
        // copy the frontier to create a new one
        Frontier new_frontier = frontier;
        
        // add back nodes to check as a result of reducing domain
        // for this arc, only main_var matters
        ARC a; a.main_var = var_with_domain_size_more_than_one;
        a.other_var_list = std::vector<VariableVertex*>();
        a.constraint = nullptr; //nullptr should work equally

        getAllCheckAgainArcs(new_frontier, subg, a);

        // recursively call arc consistency trampoline
        auto subg_solu = arcConsistency_trampoline(new_frontier, subg);
        if (!subg_solu.empty())
        {
            to_be_returned.reserve(to_be_returned.size() + subg_solu.size());
            to_be_returned.insert(to_be_returned.end(), subg_solu.begin(), subg_solu.end());
        }
    }
    // return the result
    return to_be_returned;
};


// populates the given frontier with the set of all arcs to be checked given a CSPGraph
// used at the beginning when running arc consistency
void CSPSolverImplementation::CSPSolver::getAllToDoArcs(
    CSPSolverImplementation::Frontier& frontier, 
    CSPSolverImplementation::CSPGraph& graph) 
{
    ARC new_arc;
    // for every constraint vertex in the graph:
    for (auto cv_name: graph.get_all_constraint_names()) 
    {
        // generate an arc choosing each varaible vertex in their adjacency list
        auto variable_neighbors = graph.get_variable_neighbors(cv_name);
        int neighbor_size = variable_neighbors.size();
        
        // pick each neighbor as main variable
        for (int i=0; i < neighbor_size; i++) 
        {
            new_arc.main_var = variable_neighbors[i];
            new_arc.other_var_list = std::vector<VariableVertex*>();
            
            // add all other neighbors into the list
            for (int j=0; j < neighbor_size; j++) 
            {
                if (i != j) new_arc.other_var_list.push_back(variable_neighbors[j]);
            }
            new_arc.constraint = graph.get_constraint(cv_name);
            
            // add it to the frontier
            frontier.push(new_arc);
        } 
    }
};


// populates the given frontier with the set of all arcs to be checked, given 
// we just checked a certain ARC and reduced the domain of the main variable
void CSPSolverImplementation::CSPSolver::getAllCheckAgainArcs(Frontier& frontier, CSPGraph& graph, const ARC& arc)
{
    ARC add_back_arc;
    // iterate over all neighbors of mainVar in arc
    for (auto C : graph.get_constraint_neighbors(arc.main_var->getName())) 
    {
        // for each such constraint neighbor C, ignore the one given as part of arc
        if (C == arc.constraint) continue;
        // for other neighbors C, iterate over its variable neighbors V
        auto variable_neighbors_of_C = graph.get_variable_neighbors(C->getName());
        
        for (auto V : variable_neighbors_of_C)
        {
            if (V == arc.main_var) continue;
            // create an arc containing
            //  1) V unequal to mainVar as main variable            
            add_back_arc.main_var = V;
            //  2) remaining Vs as var_list
            add_back_arc.other_var_list = std::vector<VariableVertex*>();
            for (auto other_variable : variable_neighbors_of_C) 
            {
                if (other_variable != V) 
                    add_back_arc.other_var_list.push_back(other_variable);
            }
            //  3) C as constraint
            add_back_arc.constraint = C;
            // add it to the frontier
            frontier.push(add_back_arc);
        }
    }
};

// run one arc consistency step on CSPGraph and arcs in Frontier;
// using differing Frontiers might change the runtime & efficiency of the process
void CSPSolverImplementation::CSPSolver::singleArcConsistencyStep(CSPGraph& graph, Frontier& frontier)
{
    bool reduced_at_least_one_domain_value = false;
    
    // if frontier is empty, do nothing
    if (frontier.empty()) return;
    
    // pop one object from frontier
    ARC next_arc = frontier.pop();


    // for each domain in the main var, check if constraint is met given other vars
    for (int dom_val : next_arc.main_var->getDomain())
    {
        // if the constraint isn't consistent:
        if (!next_arc.constraint->constraintIsMet(dom_val, next_arc.other_var_list))
        {
            reduced_at_least_one_domain_value = true;
            // remove that domain from the mix
            next_arc.main_var->removeFromDomain(dom_val);
        }
    }

    // if domain was reduced, add any arc we need to double check
    if (reduced_at_least_one_domain_value)
        getAllCheckAgainArcs(frontier, graph, next_arc);
};

// checks the existence of a unique solution, returned as vector of Variables with unique domain
// also return a boolean of "true" if we have a unique / no answer, false if indeterminate
std::tuple<std::vector<GraphImplementation::VariableVertex>, bool>
CSPSolverImplementation::CSPSolver::checkAnswer(CSPSolverImplementation::CSPGraph& graph)
{
    std::vector<VariableVertex> returned;
    bool determinate_result = true;

    // for every variable in graph, check domain size
    for (std::string vv_name : graph.get_all_variable_names())
    {
        VariableVertex* vv = graph.get_variable(vv_name);
        // if any is 0, return an empty vector
        if (vv->getDomain().size() == 0)
        {
            return std::tuple<std::vector<VariableVertex>, bool>({
                std::vector<VariableVertex>(), true
                });
        // if any is >= 2, return the set of possible values & false bool
        } else if (vv->getDomain().size() >= 2)
        {
            determinate_result = false;
        }
        // add a copy of the variable vertex we just saw into returned vector
        returned.push_back(*vv);
    }
    // return result
    // determinate_result == false if even one variable has domain size >= 2 
    return std::tuple<std::vector<GraphImplementation::VariableVertex>, bool> (
        returned, determinate_result
    );
};


// splits domain of specific variable and returns all generated graphs
std::vector<CSPSolverImplementation::CSPGraph> 
CSPSolverImplementation::CSPSolver::splitDomain(const CSPSolverImplementation::CSPGraph& graph, 
                                                GraphImplementation::VariableVertex* vv) 
{
    std::vector<CSPGraph> returned;

    // if domain of given vv is empty, return empty vector
    if (vv->getDomain().empty()) return std::vector<CSPGraph>();
    // otherwise, if domain has more than one value
    // for domain values in given vv:
    for (int dom_val : vv->getDomain())
    {
        //  a) copy graph
        CSPGraph new_graph = graph;
        //  b) remove all value but dom_val from new_graph vv's domain
        VariableVertex* vv_in_new_graph = new_graph.get_variable(vv->getName());
        for (int dom_val_to_remove : vv->getDomain())
        {
            if (dom_val_to_remove != dom_val) 
                vv_in_new_graph->removeFromDomain(dom_val_to_remove);
        }
        //  c) add new_graph to returned list of graphs
        returned.push_back(new_graph);
    }

    return returned;
};