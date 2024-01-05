// Author: Akira Kudo
// Description: Implements tests for the CSPSolver class in cspSolver.

#define BOOST_TEST_MODULE CSP Solver Testcases
#define BOOST_TEST_DYN_LINK //using shared library variant of Boost.Test
#include <boost/test/unit_test.hpp>

#include <tuple>

#include "src/cspSolver/frontier/Frontier.h"
#include "src/cspSolver/CSPGraph.h"
#include "src/cspSolver/CSPSolver.h"

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using namespace GraphImplementation;

namespace _unit_test_befriender 
{
    // Static wrappers for private/protected methods
    struct TestBefriender
    {
        static void singleArcConsistencyStep(
            CSPSolverImplementation::CSPSolver& solver, 
            CSPSolverImplementation::CSPGraph graph, 
            CSPSolverImplementation::Frontier frontier
            )
        {
            return solver.singleArcConsistencyStep(graph, frontier);
        }
    };
}


// fixture class for testCSPSolver
struct TestCSPSolver_Fixture
{
    CSPSolverImplementation::CSPGraph cspg;
    CSPSolverImplementation::Frontier frontier = CSPSolverImplementation::Frontier(CSPSolverImplementation::Frontier::QueueMode);
    
    // setup
    TestCSPSolver_Fixture()
    {
        frontier = CSPSolverImplementation::Frontier(CSPSolverImplementation::Frontier::QueueMode);

        // let's emulate a single square sudoku
        cspg = CSPSolverImplementation::CSPGraph();
        cspg.add_constraint("OnlyOne1", ConstraintVertex::exactlyN(1, 1), 
            "We can have only one 1 in the square.");
        cspg.add_constraint("OnlyOne2", ConstraintVertex::exactlyN(1, 2), 
            "We can have only one 2 in the square.");
        cspg.add_constraint("OnlyOne3", ConstraintVertex::exactlyN(1, 3), 
            "We can have only one 3 in the square.");
        cspg.add_constraint("OnlyOne4", ConstraintVertex::exactlyN(1, 4), 
            "We can have only one 4 in the square.");
        cspg.add_constraint("OnlyOne5", ConstraintVertex::exactlyN(1, 5), 
            "We can have only one 5 in the square.");
        cspg.add_constraint("OnlyOne6", ConstraintVertex::exactlyN(1, 6), 
            "We can have only one 6 in the square.");
        cspg.add_constraint("OnlyOne7", ConstraintVertex::exactlyN(1, 7), 
            "We can have only one 7 in the square.");
        cspg.add_constraint("OnlyOne8", ConstraintVertex::exactlyN(1, 8), 
            "We can have only one 8 in the square.");
        cspg.add_constraint("OnlyOne9", ConstraintVertex::exactlyN(1, 9), 
            "We can have only one 9 in the square.");
        
        cspg.add_variable("Square 1", {1,2,3,4,5,6,7,8,9});
        cspg.add_variable("Square 2", {1,2,3,4,5,6,7,8,9});
        cspg.add_variable("Square 3", {1,2,3,4,5,6,7,8,9});
        cspg.add_variable("Square 4", {1,2,3,4,5,6,7,8,9});
        cspg.add_variable("Square 5", {1,2,3,4,5,6,7,8,9});
        cspg.add_variable("Square 6", {1,2,3,4,5,6,7,8,9});
        cspg.add_variable("Square 7", {1,2,3,4,5,6,7,8,9});
        cspg.add_variable("Square 8", {1,2,3,4,5,6,7,8,9});
        cspg.add_variable("Square 9", {1,2,3,4,5,6,7,8,9});
    };

    // teardown
    ~TestCSPSolver_Fixture() {};
};

BOOST_FIXTURE_TEST_SUITE(CSPSolver_test_suite, TestCSPSolver_Fixture, * boost::unit_test::label("CSPSolver"));

    // create a CSP graph for a given problem - using a CLI?
    // void createCspGraph();
    BOOST_AUTO_TEST_SUITE(createCspGraph);

        BOOST_AUTO_TEST_CASE(no_need_for_test) {
            // *since this calls a CLI-based function, we don't need
            // a test (I don't think I know how to do it anyway)
        }

    BOOST_AUTO_TEST_SUITE_END();

    // save a created CSP graph
    // void saveCspGraph(GraphImplementation::Graph graph, std::string saveDir);
    BOOST_AUTO_TEST_SUITE(saveCspGraph);

        BOOST_AUTO_TEST_CASE(not_implemented_yet) {
            // *not implemented yet; have to find a way to serialize 
            // constraints first, and I don't think we need that yet
        }

    BOOST_AUTO_TEST_SUITE_END();

    // load a CSP graph
    // void loadCspGraph(std::string loadDir);
    BOOST_AUTO_TEST_SUITE(loadCspGraph);

        BOOST_AUTO_TEST_CASE(not_implemented_yet) {
            // *not implemented yet; have to find a way to serialize 
            // constraints first, and I don't think we need that yet
        }

    BOOST_AUTO_TEST_SUITE_END();

    // run arc consistency and return all possible answers
    // std::vector<std::vector<GraphImplementation::VariableVertex>> arcConsistency(CSPGraph graph);
    BOOST_AUTO_TEST_SUITE(arcConsistency);

    BOOST_AUTO_TEST_SUITE_END();

    // run DFS with pruning and return all possible answers
    // std::vector<std::vector<GraphImplementation::VariableVertex>> depthFirstSearchWithPruning(CSPGraph graph);
    BOOST_AUTO_TEST_SUITE(depthFirstSearchWithPruning);

    BOOST_AUTO_TEST_SUITE_END();


    // ###############################################################
    // SPECIAL PRIVATE FUNCTION TESTING

    // populates the given frontier with the set of all arcs to be checked given a CSPGraph
    // used at the beginning when running arc consistency
    // void getAllToDoArcs(Frontier& frontier, const CSPGraph& graph);
    BOOST_AUTO_TEST_SUITE(getAllToDoArcs);

        BOOST_AUTO_TEST_CASE(many_vertices_no_edge) {
            // precondition: check that no edge exist in graph
            for (auto vv_name : cspg.get_all_variable_names()) {
                for (auto cv_name : cspg.get_all_constraint_names()) {
                    BOOST_TEST_REQUIRE(cspg.adjacent(vv_name, cv_name) == false);
                }
            }
            // also that the frontier is empty
            BOOST_TEST_REQUIRE(frontier.empty() == true);


            // setup:
            // test:
        }    

    BOOST_AUTO_TEST_SUITE_END();

    
    // run one arc consistency step on CSPGraph and arcs in Frontier;
    // using differing Frontiers might change the runtime & efficiency of the process
    // void singleArcConsistencyStep(CSPGraph& graph, Frontier& frontier);
    // BOOST_AUTO_TEST_SUITE(singleArcConsistencyStep);

    //     // when frontier is empty, the current state is diagnosed as one of:
    //     // 1) no solution, 2) one solution, 3) many possible solutions.
    //     BOOST_AUTO_TEST_CASE(empty_frontier_no_solution) {
    //         // setup: setup graph such that there's no solution
    //         // that is, there is at least one variable with empty domain

    //         // first check Square 9 exists, and remove it
    //         BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 9") == true);
    //         cspg.remove_vertex("Square 9");

    //         // then add back a Square 9 variable with no domain
    //         cspg.add_variable("Square 9", {});
            
    //         // test: check that running singleArcConsistencyStep yields no solution
            
    //     }

    //     BOOST_AUTO_TEST_CASE(empty_frontier_unique_solution) {
    //         // presetup:   
    //         // precondition:
    //         // setup:
    //         // test:
    //     }

    //     BOOST_AUTO_TEST_CASE(empty_frontier_many_solutions) {
    //         // presetup:   
    //         // precondition:
    //         // setup:
    //         // test:

    //     }

    //     // frontier can also hold 1, or 2+ arcs
    //     BOOST_AUTO_TEST_CASE(frontier_size_one) {
    //         // presetup:   
    //         // precondition:
    //         // setup:
    //         // test:

    //     }
        
    //     BOOST_AUTO_TEST_CASE(frontier_size_two) {
    //         // presetup:   
    //         // precondition:
    //         // setup:
    //         // test:

    //     }        

    // BOOST_AUTO_TEST_SUITE_END();
    
    // checks the existence of a unique solution, returned as vector of Variables with unique domain
    // also return a boolean of "true" if we have a unique / no answer, false if indeterminate
    // std::tuple<std::vector<GraphImplementation::VariableVertex>, bool> 
    // checkAnswer(const CSPGraph& graph);
    // BOOST_AUTO_TEST_SUITE(checkAnswer);
    
    //     BOOST_AUTO_TEST_CASE() {
    //         // presetup:   
    //         // precondition:
    //         // setup:
    //         // test:
    //     }    

    // BOOST_AUTO_TEST_SUITE_END();
    
    // splits domain of specific variable and returns all generated graphs
    // std::vector<CSPGraph> splitDomain(const CSPGraph& graph, VariableVertex* vv);
    // BOOST_AUTO_TEST_SUITE(splitDomain);
    
    //     BOOST_AUTO_TEST_CASE() {
    //         // presetup:   
    //         // precondition:
    //         // setup:
    //         // test:
    //     }    

    // BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();