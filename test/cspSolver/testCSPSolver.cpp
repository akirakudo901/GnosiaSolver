// Author: Akira Kudo
// Description: Implements tests for the CSPSolver class in cspSolver.

#define BOOST_TEST_MODULE CSP Solver Testcases
#define BOOST_TEST_DYN_LINK //using shared library variant of Boost.Test
#include <boost/test/unit_test.hpp>

#include "src/CSPSolver/CSPSolver.h"

// fixture class for testCSPSolver
struct F 
{
    // setup
    F()
    {

    };

    // teardown
    ~F()
    {

    };
};

BOOST_FIXTURE_TEST_SUITE(CSPSolver_test_suite, F, * boost::unit_test::label("CSPSolver"));

    // create a CSP graph for a given problem - using a CLI?
    // void createCspGraph();
    BOOST_AUTO_TEST_SUITE(createCspGraph);

    BOOST_AUTO_TEST_SUITE_END();

    // save a created CSP graph
    // void saveCspGraph(GraphImplementation::Graph graph, std::string saveDir);
    BOOST_AUTO_TEST_SUITE(saveCspGraph);

    BOOST_AUTO_TEST_SUITE_END();

    // load a CSP graph
    // void loadCspGraph(std::string loadDir);
    BOOST_AUTO_TEST_SUITE(loadCspGraph);

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