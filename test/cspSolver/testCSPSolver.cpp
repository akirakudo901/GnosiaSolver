// Author: Akira Kudo
// Description: Implements tests for the CSPSolver class in cspSolver.

#define BOOST_TEST_MODULE CSP Solver Testcases
#define BOOST_TEST_DYN_LINK //using shared library variant of Boost.Test
#include <boost/test/unit_test.hpp>

#include "src/cspSolver/CSPGraph.h"
#include "src/cspSolver/CSPSolver.h"

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using namespace GraphImplementation;

// fixture class for testCSPSolver
struct F 
{
    CSPSolverImplementation::CSPGraph cspg;

    ConstraintVertex cv1 = ConstraintVertex("", ConstraintVertex::exactlyN(0, 0));
    ConstraintVertex cv2 = ConstraintVertex("", ConstraintVertex::exactlyN(0, 0));
    ConstraintVertex cv3 = ConstraintVertex("", ConstraintVertex::exactlyN(0, 0));
    ConstraintVertex cv4 = ConstraintVertex("", ConstraintVertex::exactlyN(0, 0));
    ConstraintVertex cv5 = ConstraintVertex("", ConstraintVertex::exactlyN(0, 0));
    ConstraintVertex cv6 = ConstraintVertex("", ConstraintVertex::exactlyN(0, 0));
    ConstraintVertex cv7 = ConstraintVertex("", ConstraintVertex::exactlyN(0, 0));
    ConstraintVertex cv8 = ConstraintVertex("", ConstraintVertex::exactlyN(0, 0));
    ConstraintVertex cv9 = ConstraintVertex("", ConstraintVertex::exactlyN(0, 0));

    VariableVertex vv1 = VariableVertex("", {});
    VariableVertex vv2 = VariableVertex("", {});
    VariableVertex vv3 = VariableVertex("", {});
    VariableVertex vv4 = VariableVertex("", {});
    VariableVertex vv5 = VariableVertex("", {});
    VariableVertex vv6 = VariableVertex("", {});
    VariableVertex vv7 = VariableVertex("", {});
    VariableVertex vv8 = VariableVertex("", {});
    VariableVertex vv9 = VariableVertex("", {});
    
    // setup
    F()
    {
        // let's emulate a single square sudoku
        cspg = CSPSolverImplementation::CSPGraph();
        cv1 = ConstraintVertex("OnlyOne1", ConstraintVertex::exactlyN(1, 1), 
            "We can have only one 1 in the square.");
        cv2 = ConstraintVertex("OnlyOne2", ConstraintVertex::exactlyN(1, 2), 
            "We can have only one 2 in the square.");
        cv3 = ConstraintVertex("OnlyOne3", ConstraintVertex::exactlyN(1, 3), 
            "We can have only one 3 in the square.");
        cv4 = ConstraintVertex("OnlyOne4", ConstraintVertex::exactlyN(1, 4), 
            "We can have only one 4 in the square.");
        cv5 = ConstraintVertex("OnlyOne5", ConstraintVertex::exactlyN(1, 5), 
            "We can have only one 5 in the square.");
        cv6 = ConstraintVertex("OnlyOne6", ConstraintVertex::exactlyN(1, 6), 
            "We can have only one 6 in the square.");
        cv7 = ConstraintVertex("OnlyOne7", ConstraintVertex::exactlyN(1, 7), 
            "We can have only one 7 in the square.");
        cv8 = ConstraintVertex("OnlyOne8", ConstraintVertex::exactlyN(1, 8), 
            "We can have only one 8 in the square.");
        cv9 = ConstraintVertex("OnlyOne9", ConstraintVertex::exactlyN(1, 9), 
            "We can have only one 9 in the square.");
        
        vv1 = VariableVertex("Square 1", {1,2,3,4,5,6,7,8,9});
        vv2 = VariableVertex("Square 2", {1,2,3,4,5,6,7,8,9});
        vv3 = VariableVertex("Square 3", {1,2,3,4,5,6,7,8,9});
        vv4 = VariableVertex("Square 4", {1,2,3,4,5,6,7,8,9});
        vv5 = VariableVertex("Square 5", {1,2,3,4,5,6,7,8,9});
        vv6 = VariableVertex("Square 6", {1,2,3,4,5,6,7,8,9});
        vv7 = VariableVertex("Square 7", {1,2,3,4,5,6,7,8,9});
        vv8 = VariableVertex("Square 8", {1,2,3,4,5,6,7,8,9});
        vv9 = VariableVertex("Square 9", {1,2,3,4,5,6,7,8,9});
    };

    // teardown
    ~F() {};
};

BOOST_FIXTURE_TEST_SUITE(CSPSolver_test_suite, F, * boost::unit_test::label("CSPSolver"));

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

    // run arc consistency and return the answer
    // by default, the frontier deals with unary constraints first, then in random order
    // std::vector<GraphImplementation::VariableVertex> arcConsistency(GraphImplementation::Graph);
    BOOST_AUTO_TEST_SUITE(arcConsistency);
        
        // when frontier is empty, the current state is diagnosed as one of:
        // 1) no solution, 2) one solution, 3) many possible solutions.
        BOOST_AUTO_TEST_CASE(empty_frontier_no_solution) {
            
        }

        BOOST_AUTO_TEST_CASE(empty_frontier_unique_solution) {

        }

        BOOST_AUTO_TEST_CASE(empty_frontier_many_solutions) {

        }

        // frontier can also hold 1, or 2+ arcs
        BOOST_AUTO_TEST_CASE(frontier_size_one) {

        }
        
        BOOST_AUTO_TEST_CASE(frontier_size_two) {

        }

    BOOST_AUTO_TEST_SUITE_END();

    // run DFS with pruning and return the answer
    // std::vector<GraphImplementation::VariableVertex> depthFirstSearchWithPruning(GraphImplementation::Graph);
    BOOST_AUTO_TEST_SUITE(depthFirstSearchWithPruning);

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();