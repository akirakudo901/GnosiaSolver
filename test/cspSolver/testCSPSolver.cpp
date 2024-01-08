// Author: Akira Kudo
// Description: Implements tests for the CSPSolver class in cspSolver.

#define BOOST_TEST_MODULE CSP Solver Testcases
#define BOOST_TEST_DYN_LINK //using shared library variant of Boost.Test
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "src/cspSolver/frontier/Frontier.h"
#include "src/cspSolver/CSPGraph.h"
#include "src/cspSolver/CSPSolver.h"

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using namespace GraphImplementation;
using namespace CSPSolverImplementation;

namespace _unit_test_befriender 
{
    // Static wrappers for private/protected methods
    struct TestBefriender
    {
        static void getAllToDoArcs(CSPSolver& solver, CSPGraph& graph, Frontier& frontier)
        {
            solver.getAllToDoArcs(frontier, graph);
        }

        static void singleArcConsistencyStep(CSPSolver& solver, CSPGraph& graph, Frontier& frontier)
        {
            solver.singleArcConsistencyStep(graph, frontier);
        }

        static std::tuple<std::vector<VariableVertex>, bool> checkAnswer(
            CSPSolver& solver, CSPGraph& graph
            )
        {
            return solver.checkAnswer(graph);
        };

        static std::vector<CSPGraph> splitDomain(CSPSolver& solver, CSPGraph& graph, VariableVertex* vv)
        {
            return solver.splitDomain(graph, vv);
        };

        static void getAllCheckAgainArcs(CSPSolver& solver, Frontier& frontier, CSPGraph& graph, const ARC& arc)
        {
            solver.getAllCheckAgainArcs(frontier, graph, arc);
        }

        static bool compareCSPGraphBasedOnVariablesAndConstraintNames(CSPGraph& graph1, CSPGraph& graph2) 
        {
            // given we can't reliably compare constraint predicates, check: 
            // 1) both CSPGraph hold the same constraints by names
            std::vector<std::string> graph1_constraints = graph1.get_all_constraint_names();
            std::vector<std::string> graph2_constraints = graph2.get_all_constraint_names();
            std::sort(graph1_constraints.begin(), graph1_constraints.end());
            std::sort(graph2_constraints.begin(), graph2_constraints.end());
            if (graph1_constraints != graph2_constraints) return false;
            
            // 2) both CSPGraph hold the same variables checkable with ==
            if (graph1.get_all_variable_names().size() != graph2.get_all_variable_names().size()) 
                return false;

            for (std::string vv_name : graph2.get_all_variable_names()) {
                // check if vv_name in graph1 is a nullptr
                // if it is, vv isn't contained in graph1
                if (graph1.get_variable(vv_name) == nullptr) return false;
                
                VariableVertex vv_graph1 = *graph1.get_variable(vv_name);
                VariableVertex vv_graph2 = *graph2.get_variable(vv_name);
                if (vv_graph1 != vv_graph2) return false;
            }
            // 3) both CSPGraph have the same edges - checkable by adjacency
            for (std::string cv_name : graph2.get_all_constraint_names()) {
                for (std::string vv_name : graph2.get_all_variable_names()) {
                    if (graph2.adjacent(cv_name, vv_name) != graph1.adjacent(cv_name, vv_name))
                        return false;
                }
            }
            // if everything else is ok, return true
            return true;
        }
    };
}


// fixture class for testCSPSolver
struct TestCSPSolver_Fixture
{
    CSPGraph cspg;
    Frontier frontier = Frontier(Frontier::QueueMode);
    CSPSolver solver;
    
    // setup
    TestCSPSolver_Fixture()
    {
        frontier = Frontier(Frontier::QueueMode);
        solver = CSPSolver();

        // let's emulate a single square sudoku
        cspg = CSPGraph();
        cspg.add_constraint("OnlyOne1", ConstraintVertex::exactlyN(1, 1), 
            "We can have only one 1 in the square.");
        cspg.add_constraint("OnlyOne2", ConstraintVertex::exactlyN(2, 1), 
            "We can have only one 2 in the square.");
        cspg.add_constraint("OnlyOne3", ConstraintVertex::exactlyN(3, 1), 
            "We can have only one 3 in the square.");
        cspg.add_constraint("OnlyOne4", ConstraintVertex::exactlyN(4, 1), 
            "We can have only one 4 in the square.");
        cspg.add_constraint("OnlyOne5", ConstraintVertex::exactlyN(5, 1), 
            "We can have only one 5 in the square.");
        cspg.add_constraint("OnlyOne6", ConstraintVertex::exactlyN(6, 1), 
            "We can have only one 6 in the square.");
        cspg.add_constraint("OnlyOne7", ConstraintVertex::exactlyN(7, 1), 
            "We can have only one 7 in the square.");
        cspg.add_constraint("OnlyOne8", ConstraintVertex::exactlyN(8, 1), 
            "We can have only one 8 in the square.");
        cspg.add_constraint("OnlyOne9", ConstraintVertex::exactlyN(9, 1), 
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

    // #########################################################################
    // arcConsistency test is at the bottom in order to use a new test fiture
    // #########################################################################

    // run DFS with pruning and return all possible answers
    // std::vector<std::vector<GraphImplementation::VariableVertex>> depthFirstSearchWithPruning(CSPGraph graph);
    BOOST_AUTO_TEST_SUITE(depthFirstSearchWithPruning);
    // implement another time?
    BOOST_AUTO_TEST_SUITE_END();


    // ###############################################################
    // SPECIAL PRIVATE FUNCTION TESTING

    // populates the given frontier with the set of all arcs to be checked given a CSPGraph
    // used at the beginning when running arc consistency
    // void getAllToDoArcs(Frontier& frontier, const CSPGraph& graph);
    BOOST_AUTO_TEST_SUITE(getAllToDoArcs);

        BOOST_AUTO_TEST_CASE(no_edge) {
            // precondition: check that no edge exist in graph
            for (auto vv_name : cspg.get_all_variable_names()) {
                for (auto cv_name : cspg.get_all_constraint_names()) {
                    BOOST_TEST_REQUIRE(!cspg.adjacent(vv_name, cv_name));
                }
            }
            // also that the frontier is empty
            BOOST_TEST_REQUIRE(frontier.empty());

            // setup: call getAllToDoArcs
            _unit_test_befriender::TestBefriender::getAllToDoArcs(solver, cspg, frontier);

            // test: test that frontier is still empty
            BOOST_TEST(frontier.empty());
        }    

        BOOST_AUTO_TEST_CASE(one_edge) {
            // precondition: check that no edge exist in graph
            for (auto vv_name : cspg.get_all_variable_names()) {
                for (auto cv_name : cspg.get_all_constraint_names()) {
                    BOOST_TEST_REQUIRE(!cspg.adjacent(vv_name, cv_name));
                }
            }
            // also that the frontier is empty
            BOOST_TEST_REQUIRE(frontier.empty());

            // setup: add one edge to the graph, then get all to do arcs
            cspg.add_edge("Square 1", "OnlyOne1");
            _unit_test_befriender::TestBefriender::getAllToDoArcs(solver, cspg, frontier);

            // test: test that frontier now holds one entry, then check what it is
            BOOST_CHECK_EQUAL(frontier.size(), 1);
            ARC expected;
            expected.main_var = cspg.get_variable("Square 1");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>();
            expected.constraint = cspg.get_constraint("OnlyOne1");

            // guard against popping empty frontier
            BOOST_TEST_REQUIRE(!frontier.empty());
            BOOST_CHECK_EQUAL(expected, frontier.pop());
        } 

        BOOST_AUTO_TEST_CASE(six_edges) {
            // precondition: check that no edge exist in graph
            for (auto vv_name : cspg.get_all_variable_names()) {
                for (auto cv_name : cspg.get_all_constraint_names()) {
                    BOOST_TEST_REQUIRE(!cspg.adjacent(vv_name, cv_name));
                }
            }
            // also that the frontier is empty
            BOOST_TEST_REQUIRE(frontier.empty());

            // setup: add three unary & two binary & one ternary constraint edge
            // three unary
            cspg.add_edge("Square 1", "OnlyOne1");
            cspg.add_edge("Square 2", "OnlyOne2");
            cspg.add_edge("Square 3", "OnlyOne3");
            // two binary
            cspg.add_edge("Square 4", "OnlyOne4");
            cspg.add_edge("Square 5", "OnlyOne4");

            cspg.add_edge("Square 5", "OnlyOne5");
            cspg.add_edge("Square 6", "OnlyOne5");
            // one ternary 
            cspg.add_edge("Square 6", "OnlyOne6");
            cspg.add_edge("Square 7", "OnlyOne6");
            cspg.add_edge("Square 8", "OnlyOne6");
            _unit_test_befriender::TestBefriender::getAllToDoArcs(solver, cspg, frontier);

            // test: test that frontier now holds (1*3+2*2+3*1=) 10 entry, then check what it is
            BOOST_CHECK_EQUAL(frontier.size(), 10);
            // first three pops have to be unary
            std::vector<ARC> expected_unary;
            ARC expected;
            bool checkBool;
            // first expected
            expected.main_var = cspg.get_variable("Square 1");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>();
            expected.constraint = cspg.get_constraint("OnlyOne1");
            expected_unary.push_back(expected);
            // second expected
            expected.main_var = cspg.get_variable("Square 2");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>();
            expected.constraint = cspg.get_constraint("OnlyOne2");
            expected_unary.push_back(expected);
            // third expected
            expected.main_var = cspg.get_variable("Square 3");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>();
            expected.constraint = cspg.get_constraint("OnlyOne3");
            expected_unary.push_back(expected);
            
            // guard against popping empty frontier
            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_unary.begin(), expected_unary.end(), frontier.pop()) != expected_unary.end();
            BOOST_TEST(checkBool);
            
            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_unary.begin(), expected_unary.end(), frontier.pop()) != expected_unary.end();
            BOOST_TEST(checkBool);
            
            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_unary.begin(), expected_unary.end(), frontier.pop()) != expected_unary.end();
            BOOST_TEST(checkBool);
            
            // remaining sevens can come really in any order
            std::vector<ARC> expected_non_unary;
            // first expected
            expected.main_var = cspg.get_variable("Square 4");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>(
                { cspg.get_variable("Square 5") }
                );
            expected.constraint = cspg.get_constraint("OnlyOne4");
            expected_non_unary.push_back(expected);
            // second expected
            expected.main_var = cspg.get_variable("Square 5");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>(
                { cspg.get_variable("Square 4") }
                );
            expected.constraint = cspg.get_constraint("OnlyOne4");
            expected_non_unary.push_back(expected);
            // third expected
            expected.main_var = cspg.get_variable("Square 5");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>(
                { cspg.get_variable("Square 6") }
                );
            expected.constraint = cspg.get_constraint("OnlyOne5");
            expected_non_unary.push_back(expected);
            // fourth expected
            expected.main_var = cspg.get_variable("Square 6");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>(
                { cspg.get_variable("Square 5") }
                );
            expected.constraint = cspg.get_constraint("OnlyOne5");
            expected_non_unary.push_back(expected);
            // fifth expected
            expected.main_var = cspg.get_variable("Square 6");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>(
                { cspg.get_variable("Square 7"), cspg.get_variable("Square 8") }
                );
            expected.constraint = cspg.get_constraint("OnlyOne6");
            expected_non_unary.push_back(expected);
            // sixth expected
            expected.main_var = cspg.get_variable("Square 7");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>(
                { cspg.get_variable("Square 6"), cspg.get_variable("Square 8") }
                );
            expected.constraint = cspg.get_constraint("OnlyOne6");
            expected_non_unary.push_back(expected);
             // seventh expected
            expected.main_var = cspg.get_variable("Square 8");
            expected.other_var_list = std::vector<GraphImplementation::VariableVertex*>(
                { cspg.get_variable("Square 6"), cspg.get_variable("Square 7") }
                );
            expected.constraint = cspg.get_constraint("OnlyOne6");
            expected_non_unary.push_back(expected);

            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_non_unary.begin(), expected_non_unary.end(), frontier.pop()) != expected_non_unary.end();
            BOOST_TEST(checkBool);
            
            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_non_unary.begin(), expected_non_unary.end(), frontier.pop()) != expected_non_unary.end();
            BOOST_TEST(checkBool);
            
            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_non_unary.begin(), expected_non_unary.end(), frontier.pop()) != expected_non_unary.end();
            BOOST_TEST(checkBool);
            
            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_non_unary.begin(), expected_non_unary.end(), frontier.pop()) != expected_non_unary.end();
            BOOST_TEST(checkBool);
            
            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_non_unary.begin(), expected_non_unary.end(), frontier.pop()) != expected_non_unary.end();
            BOOST_TEST(checkBool);
            
            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_non_unary.begin(), expected_non_unary.end(), frontier.pop()) != expected_non_unary.end();
            BOOST_TEST(checkBool);
            
            BOOST_TEST_REQUIRE(!frontier.empty());
            checkBool = std::find(expected_non_unary.begin(), expected_non_unary.end(), frontier.pop()) != expected_non_unary.end();
            BOOST_TEST(checkBool);
            
            BOOST_TEST(frontier.empty());
        }  

    BOOST_AUTO_TEST_SUITE_END();

    // populates the given frontier with the set of all arcs to be checked, given 
    // we just checked a certain ARC and reduced the domain of the main variable
    // void CSPSolverImplementation::CSPSolver::getAllCheckAgainArcs(Frontier& frontier, const CSPGraph& graph, const ARC& arc)
    BOOST_AUTO_TEST_SUITE(getAllCheckAgainArcs);

        // if there's no other neighbors, no arcs should be added to the frontier
        // this is true even if the arc we just checked involved another variable
        BOOST_AUTO_TEST_CASE(main_var_has_no_other_neighbors) {
            // presetup: add one edge to the graph that will be passed as arc
            cspg.add_edge("Square 1", "OnlyOne1");
            cspg.add_edge("Square 2", "OnlyOne1");
            // create an arc based on this edge
            ARC passed_arc;
            passed_arc.main_var = cspg.get_variable("Square 1");
            passed_arc.other_var_list = std::vector<VariableVertex*>({
                cspg.get_variable("Square 2")
            });
            passed_arc.constraint = cspg.get_constraint("OnlyOne1");

            // precondition: check that frontier is empty
            BOOST_TEST_REQUIRE(frontier.empty());

            // setup: call getAllCheckAgainArcs
            _unit_test_befriender::TestBefriender::getAllCheckAgainArcs(solver, frontier, cspg, passed_arc);

            // test: check frontier is still empty (since no neighbors to 'Square 1' exist)
            BOOST_TEST(frontier.empty());
            // this verifies that the arc "OnlyOne1"<Square2, Square1> didn't get added, as intended
        }
        
        // if there's a neighbor:
        //  1) neighbors
        BOOST_AUTO_TEST_CASE(main_var_has_one_other_neighbor) {
            // presetup: add some edges to the graph that will be passed as arc
            cspg.add_edge("Square 1", "OnlyOne1");
            // this arc adds another neighbor to 'Square 1'
            cspg.add_edge("Square 1", "OnlyOne2");
            cspg.add_edge("Square 2", "OnlyOne2");
            cspg.add_edge("Square 3", "OnlyOne2");
            // create arcs based on edge
            ARC passed_arc;
            passed_arc.main_var = cspg.get_variable("Square 1");
            passed_arc.other_var_list = std::vector<VariableVertex*>();
            passed_arc.constraint = cspg.get_constraint("OnlyOne1");
            // these are arcs expected to be generated into the frontier 
            ARC exp_arc_2_13;
            exp_arc_2_13.main_var = cspg.get_variable("Square 2");
            exp_arc_2_13.other_var_list = std::vector<VariableVertex*>(
                {cspg.get_variable("Square 1"), cspg.get_variable("Square 3")}
            );
            exp_arc_2_13.constraint = cspg.get_constraint("OnlyOne2");
            
            ARC exp_arc_3_12;
            exp_arc_3_12.main_var = cspg.get_variable("Square 3");
            exp_arc_3_12.other_var_list = std::vector<VariableVertex*>(
                {cspg.get_variable("Square 1"), cspg.get_variable("Square 2")}
            );
            exp_arc_3_12.constraint = cspg.get_constraint("OnlyOne2");

            // precondition: check that frontier is empty
            BOOST_TEST_REQUIRE(frontier.empty());

            // setup: call getAllCheckAgainArcs
            _unit_test_befriender::TestBefriender::getAllCheckAgainArcs(solver, frontier, cspg, passed_arc);

            // test: check frontier now contains the expected arc, and those only
            // first check size
            BOOST_CHECK_EQUAL(frontier.size(), 2);
            // then check its content
            std::vector<ARC> expected_arcs;
            expected_arcs.push_back(exp_arc_2_13); expected_arcs.push_back(exp_arc_3_12);
            
            std::vector<ARC> actual_arcs;
            while (!frontier.empty()) actual_arcs.push_back(frontier.pop());
            
            bool arc_is_found;
            for (auto exp_arc : expected_arcs) 
            { 
                arc_is_found = std::find( actual_arcs.begin(), actual_arcs.end(), exp_arc ) != actual_arcs.end();
                BOOST_TEST(arc_is_found);
            }
            // this also confirms that arc "OnlyOne2"<Square 1, other> isn't added back correctly
        }

        BOOST_AUTO_TEST_CASE(main_var_has_two_other_neighbors) {
            // presetup: add some edges to the graph that will be passed as arc
            cspg.add_edge("Square 1", "OnlyOne1");
            // this arc adds another neighbor to 'Square 1'
            cspg.add_edge("Square 1", "OnlyOne2");
            cspg.add_edge("Square 2", "OnlyOne2");
            // this arc further adds another neighbor to 'Square 1'
            cspg.add_edge("Square 1", "OnlyOne3");
            cspg.add_edge("Square 3", "OnlyOne3");
            // create arcs based on edge
            ARC passed_arc;
            passed_arc.main_var = cspg.get_variable("Square 1");
            passed_arc.other_var_list = std::vector<VariableVertex*>();
            passed_arc.constraint = cspg.get_constraint("OnlyOne1");
            // these are arcs expected to be generated into the frontier 
            ARC exp_arc_2_1;
            exp_arc_2_1.main_var = cspg.get_variable("Square 2");
            exp_arc_2_1.other_var_list = std::vector<VariableVertex*>(
                {cspg.get_variable("Square 1")}
            );
            exp_arc_2_1.constraint = cspg.get_constraint("OnlyOne2");
            
            ARC exp_arc_3_1;
            exp_arc_3_1.main_var = cspg.get_variable("Square 3");
            exp_arc_3_1.other_var_list = std::vector<VariableVertex*>(
                {cspg.get_variable("Square 1")}
            );
            exp_arc_3_1.constraint = cspg.get_constraint("OnlyOne3");

            // precondition: check that frontier is empty
            BOOST_TEST_REQUIRE(frontier.empty());

            // setup: call getAllCheckAgainArcs
            _unit_test_befriender::TestBefriender::getAllCheckAgainArcs(solver, frontier, cspg, passed_arc);

            // test: check frontier now contains the expected arc, and those only
            // first check size
            BOOST_CHECK_EQUAL(frontier.size(), 2);
            // then check its content
            std::vector<ARC> expected_arcs;
            expected_arcs.push_back(exp_arc_2_1); expected_arcs.push_back(exp_arc_3_1);
            
            std::vector<ARC> actual_arcs;
            while (!frontier.empty()) actual_arcs.push_back(frontier.pop());
            
            bool arc_is_found;
            for (auto exp_arc : expected_arcs) 
            { 
                arc_is_found = std::find( actual_arcs.begin(), actual_arcs.end(), exp_arc ) != actual_arcs.end();
                BOOST_TEST(arc_is_found);
            }
        }

        BOOST_AUTO_TEST_CASE(arc_already_there_does_not_get_added_twice) {
            // presetup: add some edge to the graph that will be passed as arc
            cspg.add_edge("Square 1", "OnlyOne1");
            // this arc adds another neighbor to 'Square 1'
            cspg.add_edge("Square 1", "OnlyOne2");
            cspg.add_edge("Square 2", "OnlyOne2");
            // create arcs based on edge
            ARC passed_arc;
            passed_arc.main_var = cspg.get_variable("Square 1");
            passed_arc.other_var_list = std::vector<VariableVertex*>();
            passed_arc.constraint = cspg.get_constraint("OnlyOne1");
            // these are arcs expected to be generated into the frontier 
            ARC exp_arc_2_1;
            exp_arc_2_1.main_var = cspg.get_variable("Square 2");
            exp_arc_2_1.other_var_list = std::vector<VariableVertex*>(
                {cspg.get_variable("Square 1")}
            );
            exp_arc_2_1.constraint = cspg.get_constraint("OnlyOne2");
            // but then we add the arc already by generating it once using getAllCheckAgainArcs
            _unit_test_befriender::TestBefriender::getAllCheckAgainArcs(solver, frontier, cspg, passed_arc);

            // precondition: check that frontier has size 1
            BOOST_REQUIRE_EQUAL(frontier.size(), 1);

            // setup: call getAllCheckAgainArcs again
            _unit_test_befriender::TestBefriender::getAllCheckAgainArcs(solver, frontier, cspg, passed_arc);

            // test: check frontier still contains one arc
            // first check size
            BOOST_CHECK_EQUAL(frontier.size(), 1);
            // then check its content
            ARC actual_arc = frontier.pop();
            BOOST_TEST(actual_arc == exp_arc_2_1);
        }

        BOOST_AUTO_TEST_CASE(irrelated_arcs_are_not_added_to_frontier) {
            // presetup: add some edge to the graph that will be passed as arc
            cspg.add_edge("Square 1", "OnlyOne1");
            // these arc adds another neighbor to 'Square 1'
            cspg.add_edge("Square 1", "OnlyOne2");
            cspg.add_edge("Square 2", "OnlyOne2");
            // these arcs DO NOT add neighbors to 'Square 1'
            cspg.add_edge("Square 3", "OnlyOne3");
            cspg.add_edge("Square 4", "OnlyOne3");
            cspg.add_edge("Square 5", "OnlyOne3");

            // create arcs based on edge
            ARC passed_arc;
            passed_arc.main_var = cspg.get_variable("Square 1");
            passed_arc.other_var_list = std::vector<VariableVertex*>();
            passed_arc.constraint = cspg.get_constraint("OnlyOne1");
            // these are arcs expected to be generated into the frontier 
            ARC exp_arc_2_1;
            exp_arc_2_1.main_var = cspg.get_variable("Square 2");
            exp_arc_2_1.other_var_list = std::vector<VariableVertex*>(
                {cspg.get_variable("Square 1")}
            );
            exp_arc_2_1.constraint = cspg.get_constraint("OnlyOne2");
            
            // precondition: check that frontier is empty
            BOOST_TEST_REQUIRE(frontier.empty());

            // setup: call getAllCheckAgainArcs
            _unit_test_befriender::TestBefriender::getAllCheckAgainArcs(solver, frontier, cspg, passed_arc);

            // test: check frontier contains only the correct arc
            // first check size
            BOOST_CHECK_EQUAL(frontier.size(), 1);
            // then check its content

            // guard against popping empty frontier
            BOOST_TEST_REQUIRE(!frontier.empty());
            
            ARC actual_arc = frontier.pop();
            BOOST_TEST(actual_arc == exp_arc_2_1);
            // this indicates that none of the other possible arcs 
            // (e.g. "OnlyOne3<Square 3, Square 4 & Square 5>) are added to the frontier
        }

    BOOST_AUTO_TEST_SUITE_END();

    
    // run one arc consistency step on CSPGraph and arcs in Frontier;
    // using differing Frontiers might change the runtime & efficiency of the process
    // void singleArcConsistencyStep(CSPGraph& graph, Frontier& frontier);
    BOOST_AUTO_TEST_SUITE(singleArcConsistencyStep);

        // when frontier is empty, no change happens to frontier / graph
        BOOST_AUTO_TEST_CASE(empty_frontier_no_change) {
            // setup: setup graph to some extent
            // we add all possible edges
            for (auto vv_name : cspg.get_all_variable_names()) {
                for (auto cv_name : cspg.get_all_constraint_names()) {
                    cspg.add_edge(vv_name, cv_name);
                }
            }
            // then remove some edges randomly
            cspg.remove_edge("Square 1", "OnlyOne3");
            cspg.remove_edge("Square 5", "OnlyOne7");
            cspg.remove_edge("Square 8", "OnlyOne1");
            // also check frontier is empty
            BOOST_TEST_REQUIRE(frontier.empty());

            // test: check that running singleArcConsistencyStep yields no change
            CSPGraph expectedCSPG = cspg;
            Frontier expectedFrontier = frontier;
            bool _2_cspg_are_identical = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                cspg, expectedCSPG
            );
            BOOST_TEST(_2_cspg_are_identical);
            _unit_test_befriender::TestBefriender::singleArcConsistencyStep(solver, cspg, frontier);
            // compare based on a self-defined function for testing
            bool cspg_are_identical = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                cspg, expectedCSPG
            );
            BOOST_TEST(cspg_are_identical);
            // also check that frontier is still empty
            BOOST_TEST(frontier.empty());
        }

        BOOST_AUTO_TEST_CASE(frontier_size_one_with_domain_reduction) {
            // setup: setup graph to some extent
            // set variable "Square 9" so that its only possible domain is 1
            cspg.remove_vertex("Square 9");
            BOOST_TEST_REQUIRE(!cspg.contains_vertex("Square 9"));
            cspg.add_variable("Square 9", {1});
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 9"));

            // connect all variables to the edge "OnlyOne1"
            for (auto vv_name : cspg.get_all_variable_names()) {
                cspg.add_edge(vv_name, "OnlyOne1");
                BOOST_TEST_REQUIRE(cspg.adjacent(vv_name, "OnlyOne1"));
            }
            // add a couple edges connected to variable "Square 1"
            cspg.add_edge("Square 1", "OnlyOne2"); //binary
            cspg.add_edge("Square 2", "OnlyOne2");
            // and some edges not connected to variable "Square 1"
            cspg.add_edge("Square 3", "OnlyOne3");
            cspg.add_edge("Square 4", "OnlyOne3");

            // also insert one arc in the frontier
            // this arc, when checked, will remove value 1 from "Square 1" domain
            ARC arc1; 
            arc1.main_var = cspg.get_variable("Square 1");
            arc1.other_var_list = std::vector<VariableVertex*>({
                cspg.get_variable("Square 2"), cspg.get_variable("Square 3"), 
                cspg.get_variable("Square 4"), cspg.get_variable("Square 5"), 
                cspg.get_variable("Square 6"), cspg.get_variable("Square 7"),
                cspg.get_variable("Square 8"), cspg.get_variable("Square 9")
            });
            arc1.constraint = cspg.get_constraint("OnlyOne1");
            frontier.push(arc1);
            BOOST_REQUIRE_EQUAL(frontier.size(), 1);

            // then run the function
            CSPGraph expectedCSPG = cspg;
            _unit_test_befriender::TestBefriender::singleArcConsistencyStep(solver, cspg, frontier);

            // test: check that running singleArcConsistencyStep will:
            //  1 - remove arc1 from frontier
            //  2 - add arc "OnlyOne2"<Square 2, Square 1> to do the frontier but nothing else
            //  3 - remove value 1 from "Square 1" domain in cspg, but not change anything else
            
            // check frontier: size 1, but doesn't have arc1
            BOOST_REQUIRE_EQUAL(frontier.size(), 1);
            ARC arc_found_in_frontier = frontier.pop();
            BOOST_TEST(arc_found_in_frontier != arc1);

            ARC expected_arc; 
            expected_arc.main_var = cspg.get_variable("Square 2");
            expected_arc.other_var_list = std::vector<VariableVertex*>({cspg.get_variable("Square 1")});
            expected_arc.constraint = cspg.get_constraint("OnlyOne2");

            BOOST_CHECK_EQUAL(expected_arc, arc_found_in_frontier);

            // check that "Square 1" has no more value 1 in its domain
            std::set<int> expected_dom {2,3,4,5,6,7,8,9};
            
            // guard against when "Square 1" is a nullptr
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 1"));
            
            auto actual_dom = cspg.get_variable("Square 1")->getDomain();
            BOOST_CHECK_EQUAL_COLLECTIONS(expected_dom.begin(), expected_dom.end(), actual_dom.begin(), actual_dom.end());
            // remove the value 1 from the expected graph's domain, then compare based on a self-defined function for testing
            
            // guard against when "Square 1" is a nullptr
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 1"));
            
            expectedCSPG.get_variable("Square 1")->removeFromDomain(1);
            bool cspg_are_identical = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                cspg, expectedCSPG
            );
            BOOST_TEST(cspg_are_identical);
        }
        
        BOOST_AUTO_TEST_CASE(frontier_size_one_no_domain_reduction) {
            // setup: setup graph to some extent
            // connect all variables to the edge "OnlyOne1"
            for (auto vv_name : cspg.get_all_variable_names()) {
                cspg.add_edge(vv_name, "OnlyOne1");
                BOOST_TEST_REQUIRE(cspg.adjacent(vv_name, "OnlyOne1"));
            }
            // add a couple edges connected to variable "Square 1"
            cspg.add_edge("Square 1", "OnlyOne2"); //binary
            cspg.add_edge("Square 2", "OnlyOne2");
            // and some edges not connected to variable "Square 1"
            cspg.add_edge("Square 3", "OnlyOne3");
            cspg.add_edge("Square 4", "OnlyOne3");

            // also insert one arc in the frontier
            // this arc, when checked, won't do anything (no domain value should be inconsistent)
            ARC arc1; 
            arc1.main_var = cspg.get_variable("Square 1");
            arc1.other_var_list = std::vector<VariableVertex*>({
                cspg.get_variable("Square 2"), cspg.get_variable("Square 3"), 
                cspg.get_variable("Square 4"), cspg.get_variable("Square 5"), 
                cspg.get_variable("Square 6"), cspg.get_variable("Square 7"),
                cspg.get_variable("Square 8"), cspg.get_variable("Square 9")
            });
            arc1.constraint = cspg.get_constraint("OnlyOne1");
            frontier.push(arc1);
            BOOST_REQUIRE_EQUAL(frontier.size(), 1);

            // then run the function
            CSPGraph expectedCSPG = cspg;
            _unit_test_befriender::TestBefriender::singleArcConsistencyStep(solver, cspg, frontier);

            // test: check that running singleArcConsistencyStep will:
            //  1 - remove arc1 from frontier
            //  2 - not add back any arcs, given no domain reduction
            //  3 - will leave the graph otherwise in the exact same state
            
            // check frontier is empty
            BOOST_REQUIRE_EQUAL(frontier.size(), 0);

            // check that the two CSPGraph are equal, based on a self-defined function for testing
            bool cspg_are_identical = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                cspg, expectedCSPG
            );
            BOOST_TEST(cspg_are_identical);
        }

        BOOST_AUTO_TEST_CASE(frontier_size_two_with_domain_reduction) {
            // setup: setup graph to some extent
            // set variable "Square 9" so that its only possible domain is 9
            cspg.remove_vertex("Square 9");
            BOOST_TEST_REQUIRE(!cspg.contains_vertex("Square 9"));
            cspg.add_variable("Square 9", {9});
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 9"));

            // connect all variables to the edge "OnlyOne9"
            for (auto vv_name : cspg.get_all_variable_names()) {
                cspg.add_edge(vv_name, "OnlyOne9");
                BOOST_TEST_REQUIRE(cspg.adjacent(vv_name, "OnlyOne9"));
            }
            // add a couple edges connected to variable "Square 1"
            cspg.add_edge("Square 1", "OnlyOne2"); //binary
            cspg.add_edge("Square 2", "OnlyOne2");
            // and some edges not connected to variable "Square 1"
            cspg.add_edge("Square 3", "OnlyOne3");
            cspg.add_edge("Square 4", "OnlyOne3");

            // also insert one arc in the frontier
            // this arc, when checked, will remove value 9 from "Square 1" domain
            ARC arc1; 
            arc1.main_var = cspg.get_variable("Square 1");
            arc1.other_var_list = std::vector<VariableVertex*>({
                cspg.get_variable("Square 2"), cspg.get_variable("Square 3"), 
                cspg.get_variable("Square 4"), cspg.get_variable("Square 5"), 
                cspg.get_variable("Square 6"), cspg.get_variable("Square 7"),
                cspg.get_variable("Square 8"), cspg.get_variable("Square 9")
            });
            arc1.constraint = cspg.get_constraint("OnlyOne9");
            frontier.push(arc1);
            // this arc will be there as the second arc that won't be popped here
            ARC arc2; 
            arc2.main_var = cspg.get_variable("Square 3");
            arc2.other_var_list = std::vector<VariableVertex*>({ cspg.get_variable("Square 4") });
            arc2.constraint = cspg.get_constraint("OnlyOne3");
            frontier.push(arc2);
            // check frontier size
            BOOST_REQUIRE_EQUAL(frontier.size(), 2);

            // then run the function
            CSPGraph expectedCSPG = cspg;
            _unit_test_befriender::TestBefriender::singleArcConsistencyStep(solver, cspg, frontier);

            // test: check that running singleArcConsistencyStep will:
            //  1 - remove arc1 from frontier, leaving arc2 still in it
            //  2 - add arc "OnlyOne2"<Square 2, Square 1> to do the frontier, right after arc2 (as both are non-unary)
            //  3 - remove value 9 from "Square 1" domain in cspg, but not change anything else
            
            // check frontier: size 2, doesn't have arc1
            BOOST_REQUIRE_EQUAL(frontier.size(), 2);
            ARC arc_found_in_frontier = frontier.pop();
            BOOST_TEST(arc_found_in_frontier != arc1);

            // check that arc_found_in_frontier is arc2, which was right behind arc1
            BOOST_CHECK_EQUAL(arc_found_in_frontier, arc2);

            // also check that the arc behind arc2 is an expected one generated
            ARC expected_arc; 
            expected_arc.main_var = cspg.get_variable("Square 2");
            expected_arc.other_var_list = std::vector<VariableVertex*>({cspg.get_variable("Square 1")});
            expected_arc.constraint = cspg.get_constraint("OnlyOne2");

            arc_found_in_frontier = frontier.pop();
            BOOST_TEST(arc_found_in_frontier != arc1);
            BOOST_TEST(arc_found_in_frontier != arc2);
            BOOST_CHECK_EQUAL(arc_found_in_frontier, expected_arc);
                        
            // check that "Square 1" has no more value 9 in its domain
            std::set<int> expected_dom {1,2,3,4,5,6,7,8};
            
            // guard against when "Square 1" is a nullptr
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 1"));

            auto actual_dom = cspg.get_variable("Square 1")->getDomain();
            BOOST_CHECK_EQUAL_COLLECTIONS(expected_dom.begin(), expected_dom.end(), 
                                          actual_dom.begin(), actual_dom.end());
            
            // remove the value 9 from the expected graph's domain
            // then compare based on a self-defined function for testing
            
            // guard against when "Square 1" is a nullptr
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 1"));

            expectedCSPG.get_variable("Square 1")->removeFromDomain(9);
            bool cspg_are_identical = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                cspg, expectedCSPG
            );
            BOOST_TEST(cspg_are_identical);
        }        

    BOOST_AUTO_TEST_SUITE_END();
    
    // checks the existence of a unique solution, returned as vector of Variables with unique domain
    // also return a boolean of "true" if we have a unique / no answer, false if indeterminate
    // std::tuple<std::vector<GraphImplementation::VariableVertex>, bool> 
    // checkAnswer(const CSPGraph& graph);
    BOOST_AUTO_TEST_SUITE(checkAnswer);
        
        // when we check for answers, the current state is diagnosed as one of:
        // 1) no solution, 2) one solution, 3) many possible solutions.
        BOOST_AUTO_TEST_CASE(no_solution) {
            // setup: setup graph such that there's no solution
            // that is, there is at least one variable with empty domain

            // first check Square 9 exists, and remove it
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 9"));
            cspg.remove_vertex("Square 9");

            // then add back a Square 9 variable with no domain
            cspg.add_variable("Square 9", {});

            // also add a couple edges
            cspg.add_edge("Square 1", "OnlyOne3");
            cspg.add_edge("Square 9", "OnlyOne9");

            
            // test: run checkAnswer to see that we have no solutions
            auto actual_result = _unit_test_befriender::TestBefriender::checkAnswer(solver, cspg);
            // first entry of actual_result should be an empty vector (since no answers)
            auto expected_vector = std::vector<VariableVertex>();
            auto actual_vector = std::get<0>(actual_result);
            BOOST_CHECK_EQUAL_COLLECTIONS(expected_vector.begin(), expected_vector.end(), 
                                            actual_vector.begin(),   actual_vector.end());
            // second entry should be true, indicating we have a definite result
            BOOST_TEST(std::get<1>(actual_result));
        }

        BOOST_AUTO_TEST_CASE(unique_solution) {
            // setup: setup graph such that there's a unique solution
            // set each square's value to a unique integer
            std::vector<std::string> variable_names = { "Square 1", "Square 2", "Square 3",
                                                        "Square 4", "Square 5", "Square 6",
                                                        "Square 7", "Square 8", "Square 9" };
            for (int i=0; i < 9; i++) {
                auto name = variable_names[i];
                cspg.remove_vertex(name);
                cspg.add_variable(name, {i+1});
            }

            // also add a couple edges
            cspg.add_edge("Square 1", "OnlyOne3");
            cspg.add_edge("Square 9", "OnlyOne9");
            
            // test: run checkAnswer to see that we have a unique solution
            auto actual_result = _unit_test_befriender::TestBefriender::checkAnswer(solver, cspg);
            // check first entry of actual_result
            std::vector<VariableVertex> expected_answer {
                VariableVertex("Square 1", {1}), VariableVertex("Square 2", {2}),
                VariableVertex("Square 3", {3}), VariableVertex("Square 4", {4}),
                VariableVertex("Square 5", {5}), VariableVertex("Square 6", {6}),
                VariableVertex("Square 7", {7}), VariableVertex("Square 8", {8}),
                VariableVertex("Square 9", {9})
            };
            auto actual_answer = std::get<0>(actual_result);
            auto sort_varaible_by_name_order = [] (VariableVertex f, VariableVertex s) 
                                               { return (f.getName() < s.getName()); };
            std::sort( expected_answer.begin(), expected_answer.end(), sort_varaible_by_name_order);
            std::sort(   actual_answer.begin(),   actual_answer.end(), sort_varaible_by_name_order);
                       
            BOOST_CHECK_EQUAL_COLLECTIONS(
                expected_answer.begin(), expected_answer.end(), 
                  actual_answer.begin(),   actual_answer.end()
            );
            // second entry should be true, indicating we have a definite result
            BOOST_TEST(std::get<1>(actual_result));
        }

        BOOST_AUTO_TEST_CASE(indeterminate) {
            // let the default graph state (every variable have max domain) be used
            // also add a couple edges
            cspg.add_edge("Square 1", "OnlyOne3");
            cspg.add_edge("Square 9", "OnlyOne9");

            // test: run checkAnswer to see that we have an indeterminate solution
            auto actual_result = _unit_test_befriender::TestBefriender::checkAnswer(solver, cspg);
            // check first entry of actual_result
            std::vector<VariableVertex> expected_answer {
                VariableVertex("Square 1", {1,2,3,4,5,6,7,8,9}), 
                VariableVertex("Square 2", {1,2,3,4,5,6,7,8,9}), 
                VariableVertex("Square 3", {1,2,3,4,5,6,7,8,9}), 
                VariableVertex("Square 4", {1,2,3,4,5,6,7,8,9}), 
                VariableVertex("Square 5", {1,2,3,4,5,6,7,8,9}), 
                VariableVertex("Square 6", {1,2,3,4,5,6,7,8,9}), 
                VariableVertex("Square 7", {1,2,3,4,5,6,7,8,9}), 
                VariableVertex("Square 8", {1,2,3,4,5,6,7,8,9}), 
                VariableVertex("Square 9", {1,2,3,4,5,6,7,8,9}), 
            };
            auto actual_answer = std::get<0>(actual_result);
            auto sort_varaible_by_name_order = [] (VariableVertex f, VariableVertex s) 
                                               { return (f.getName() < s.getName()); };
            std::sort( expected_answer.begin(), expected_answer.end(), sort_varaible_by_name_order);
            std::sort(   actual_answer.begin(),   actual_answer.end(), sort_varaible_by_name_order);
                       
            BOOST_CHECK_EQUAL_COLLECTIONS(
                expected_answer.begin(), expected_answer.end(), 
                  actual_answer.begin(),   actual_answer.end()
            );
            // second entry should be false, indicating result is again indefinite
            BOOST_TEST(!std::get<1>(actual_result));
        }

    BOOST_AUTO_TEST_SUITE_END();
    
    // splits domain of specific variable and returns all generated graphs
    // std::vector<CSPGraph> splitDomain(const CSPGraph& graph, VariableVertex* vv);
    BOOST_AUTO_TEST_SUITE(splitDomain);
    
        BOOST_AUTO_TEST_CASE(split_a_variable_that_has_empty_domain) {
            // setup: set up graph with one variable that has empty domain
            // first check Square 9 exists, and remove it
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 9"));
            cspg.remove_vertex("Square 9");

            // then add back a Square 9 variable with no domain
            cspg.add_variable("Square 9", {});

            // also add a couple edges to check that splitting domains preserves edges
            cspg.add_edge("Square 1", "OnlyOne3");
            cspg.add_edge("Square 9", "OnlyOne9");

            // test: check that running splitDomain yields no result (empty vector of CSPGraph)
            auto actual_result = _unit_test_befriender::TestBefriender::splitDomain(solver, cspg, cspg.get_variable("Square 9"));
            BOOST_TEST(actual_result.empty());
        }

        BOOST_AUTO_TEST_CASE(split_a_variable_that_has_one_domain) {
            // setup: set up graph with one variable that has unique domain
            // first check Square 9 exists, and remove it
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 9"));
            cspg.remove_vertex("Square 9");

            // then add back a Square 9 variable with a single domain 9
            cspg.add_variable("Square 9", {9});

            // also add a couple edges to check that splitting domains preserves edges
            cspg.add_edge("Square 1", "OnlyOne3");
            cspg.add_edge("Square 9", "OnlyOne9");

            // test: check that running splitDomain yields a single result (identical graph)
            auto actual_result = _unit_test_befriender::TestBefriender::splitDomain(solver, cspg, cspg.get_variable("Square 9"));
            // guard against empty vector
            BOOST_TEST_REQUIRE(!actual_result.empty());
            // we use our specifically defined function
            bool vectors_are_equal = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                cspg, actual_result[0]
            );
            BOOST_TEST(vectors_are_equal);
        }

        BOOST_AUTO_TEST_CASE(split_a_variable_that_has_two_domain) {
            // setup: set up graph with one variable that has domain of size two
            // first check Square 9 exists, and remove it
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 9"));
            cspg.remove_vertex("Square 9");

            // then add back a Square 9 variable with two values 1, 9
            cspg.add_variable("Square 9", {1, 9});

            // also add a couple edges to check that splitting domains preserves edges
            cspg.add_edge("Square 1", "OnlyOne3");
            cspg.add_edge("Square 9", "OnlyOne9");

            // test: check that running splitDomain yields two results where Square 9 has value 1 and 9
            auto actual_result = _unit_test_befriender::TestBefriender::splitDomain(solver, cspg, cspg.get_variable("Square 9"));
            // guard against vector that aren't size 2
            BOOST_REQUIRE_EQUAL(actual_result.size(), 2);
            auto result_graph1 = actual_result[0]; auto result_graph2 = actual_result[1];
           
            auto exp_graph_dom_1 = cspg; exp_graph_dom_1.get_variable("Square 9")->removeFromDomain(9);
            auto exp_graph_dom_9 = cspg; exp_graph_dom_9.get_variable("Square 9")->removeFromDomain(1);
            bool graph1_is_as_expected; bool graph2_is_as_expected;
            // we use our specifically defined function
            auto graph1_domain = result_graph1.get_variable("Square 9")->getDomain();
            
            // if graph1's Square 9 holds 1 as domain value
            if (graph1_domain.find(1) != graph1_domain.end()) {
                // check that graph1 is identical to a version where Square 9 only holds 1
                graph1_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_1, result_graph1
                );
                // in this case, graph2's Square 9 should only hold 9
                graph2_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_9, result_graph2
                );

            // else if graph1's Square 9 holds 9 as domain value
            } else if (graph1_domain.find(9) != graph1_domain.end()) {
                // check that graph1 is identical to a version where Square 9 only holds 9
                graph1_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_9, result_graph1
                );
                // in this case, graph2's Square 9 should only hold 1
                graph2_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_1, result_graph2
                );
            }

            // finally check for equality
            BOOST_TEST((graph1_is_as_expected && graph2_is_as_expected));
        }

        BOOST_AUTO_TEST_CASE(split_a_variable_that_has_three_domain) {
            // setup: set up graph with one variable that has domain of size three
            // first check Square 9 exists, and remove it
            BOOST_TEST_REQUIRE(cspg.contains_vertex("Square 9"));
            cspg.remove_vertex("Square 9");

            // then add back a Square 9 variable with three values 1,5,9
            cspg.add_variable("Square 9", {1,5,9});

            // also add a couple edges to check that splitting domains preserves edges
            cspg.add_edge("Square 1", "OnlyOne3");
            cspg.add_edge("Square 9", "OnlyOne9");

            // test: check that running splitDomain yields three results where Square 9 has value 1, 5 and 9 each
            auto actual_result = _unit_test_befriender::TestBefriender::splitDomain(solver, cspg, cspg.get_variable("Square 9"));
            // guard against vector of non-size 3
            BOOST_REQUIRE_EQUAL(actual_result.size(), 3);
            auto result_graph1 = actual_result[0]; auto result_graph2 = actual_result[1]; auto result_graph3 = actual_result[2];
           
            auto exp_graph_dom_1 = cspg; exp_graph_dom_1.get_variable("Square 9")->removeFromDomain(5);
            exp_graph_dom_1.get_variable("Square 9")->removeFromDomain(9);
            auto exp_graph_dom_5 = cspg; exp_graph_dom_5.get_variable("Square 9")->removeFromDomain(1);
            exp_graph_dom_5.get_variable("Square 9")->removeFromDomain(9);
            auto exp_graph_dom_9 = cspg; exp_graph_dom_9.get_variable("Square 9")->removeFromDomain(1);
            exp_graph_dom_9.get_variable("Square 9")->removeFromDomain(5);

            bool graph1_is_as_expected; bool graph2_is_as_expected; bool graph3_is_as_expected;
            // we use our specifically defined function
            auto graph1_domain = result_graph1.get_variable("Square 9")->getDomain();
            if (graph1_domain.find(1) != graph1_domain.end()) {
                graph1_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_1, result_graph1
                );
            } else if (graph1_domain.find(5) != graph1_domain.end()) {
                graph1_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_5, result_graph1
                );
            } else if (graph1_domain.find(9) != graph1_domain.end()) {
                graph1_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_9, result_graph1
                );
            }
            // also check graph2
            auto graph2_domain = result_graph2.get_variable("Square 9")->getDomain();
            if (graph2_domain.find(1) != graph2_domain.end()) {
                graph2_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_1, result_graph2
                );
            } else if (graph2_domain.find(5) != graph2_domain.end()) {
                graph2_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_5, result_graph2
                );
            } else if (graph2_domain.find(9) != graph2_domain.end()) {
                graph1_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_9, result_graph2
                );
            }
            // also check graph3
            auto graph3_domain = result_graph3.get_variable("Square 9")->getDomain();
            if (graph3_domain.find(1) != graph3_domain.end()) {
                graph3_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_1, result_graph3
                );
            } else if (graph3_domain.find(5) != graph3_domain.end()) {
                graph3_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_5, result_graph3
                );
            } else if (graph3_domain.find(9) != graph3_domain.end()) {
                graph3_is_as_expected = _unit_test_befriender::TestBefriender::compareCSPGraphBasedOnVariablesAndConstraintNames(
                    exp_graph_dom_9, result_graph3
                );
            }
            // finally check for equality
            BOOST_TEST((graph1_is_as_expected && graph2_is_as_expected && graph3_is_as_expected));
        }

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();

struct TestCSPSolver_ArcConsistency_Fixture
{
    CSPGraph sudoku_graph;
    Frontier frontier = Frontier(Frontier::QueueMode);
    CSPSolver solver;
    
    TestCSPSolver_ArcConsistency_Fixture()
    {
        frontier = Frontier(Frontier::QueueMode);
        solver = CSPSolver();

        // let's emulate sudoku
        sudoku_graph = CSPGraph();

        // add variables
        // each variable is of the name:
        //  - "Square " + 'y position' + 'x position'
        // e.g. right-top-most square is "Square 1-9", below is "Square 2-9", etc.
        auto full_domain = std::set<int>({1,2,3,4,5,6,7,8,9});
        for (int i=1; i < 10; i++) {
            for (int j=1; j < 10; j++) {
                std::string vv_name = "Square " + std::to_string(i) + "-" + std::to_string(j);
                sudoku_graph.add_variable(vv_name, full_domain);
            }
        }

        // add constraints
        // each constraint is of the name:
        //  - "OnlyOne" + i + 'type of constraint' + 'position'
        // e.g. constraint on 3 in right-most column: "OnlyOne3-col-9"
        // others would be "OnlyOne5-row-4", "OnlyOne8-squ-3".
        for (int check_num=1; check_num < 10; check_num++) {
            for (int pos_num=1; pos_num < 10; pos_num++) {
                for (std::string type : {"row", "col", "squ"}) {
                    std::string cv_name = ("OnlyOne" + std::to_string(check_num) + "-" + 
                                            type + "-" + std::to_string(pos_num));
                    sudoku_graph.add_constraint(cv_name, ConstraintVertex::exactlyN(check_num, 1), 
                                                ("We can have only one " + std::to_string(check_num) + 
                                                    " in " + type + " " + std::to_string(pos_num) + "."));

                    // 'Square x-y'
                    if (type == "row") {
                        for (int col_num=1; col_num < 10; col_num++) {
                            std::string vv_name = "Square " + std::to_string(pos_num) + 
                                                    "-" + std::to_string(col_num); 
                            sudoku_graph.add_edge(vv_name, cv_name);
                        }
                    } else if (type == "col") {
                        for (int row_num=1; row_num < 10; row_num++) {
                            std::string vv_name = "Square " + std::to_string(row_num) + 
                                                    "-" + std::to_string(pos_num); 
                            sudoku_graph.add_edge(vv_name, cv_name);
                        }
                    } else if (type == "squ") {
                        /*
                        given the "pos_num" th square, which "Square" variables does this correspond to?
                        first, within the 3x3 grid composed by the 9 squares, for square pos_num:
                            1) row number is Floor((pos_num - 1) / 3) + 1.
                            2) column number is (pos_num - 1) % 3 + 1.
                        then, for each such square, we have 9 internal "Square" belonging to it.
                        For those squares determined uniquely by (x, y):
                            - x corresponds to (column number - 1)*3 + 1 ~ (column number) * 3
                            e.g. square 1 -> x = 1~3, square 2 -> x = 4~6
                            - y corresponds to (row number - 1)*3 + 1 ~ (row number)*3
                            e.g. square 1 -> y = 1~3, square 4 -> y = 4~6
                        
                        1: 1-1~3-3  2: 1-4~3-6  3: 1-7~3-9
                        4: 4-1~6-3  5: 4-4~6-6  6: 4-7~6-9
                        7: 7-1~9-3  8: 7-4~9-6  9: 7-7~9-9
                        */ 
                        int row_number = (pos_num - 1) / 3 + 1; //integer division rounds towards 0
                        int col_number = (pos_num - 1) % 3 + 1;
                        for (int x=(col_number - 1) * 3 + 1; x <= col_number * 3; x++) {
                            for (int y=(row_number - 1) * 3 + 1; y <= row_number * 3; y++) {
                                std::string vv_name = "Square " + std::to_string(y) + 
                                                    "-" + std::to_string(x);
                                sudoku_graph.add_edge(vv_name, cv_name);
                            } 
                        }
                    }
                }
            }
        }

        // also check that the problem is well setup
        for (int x=1; x < 10; x++) {
            for (int y=1; y < 10; y++)
            {
                // test existence of all variables
                BOOST_TEST_REQUIRE(sudoku_graph.contains_vertex("Square " + 
                    std::to_string(y) + "-" + std::to_string(x)));
                
                // test existence of all constraints
                for (std::string type : {"col", "row", "squ"})
                {
                    BOOST_TEST(sudoku_graph.contains_vertex("OnlyOne" + 
                            std::to_string(y) + "-" + type + "-" + 
                            std::to_string(x)));
                }
            }
        }
        // check the number of constraints & variables
        // (9*9=) 81 variables
        BOOST_REQUIRE_EQUAL(sudoku_graph.get_all_variable_names().size(), 81);
        // (9*9*3=) 243 constraints
        BOOST_REQUIRE_EQUAL(sudoku_graph.get_all_constraint_names().size(), 243);
        // unsure of how to check for exact edges that exist other than brute force, and am lazy :P
        // we expect (243*9=) 2187 arcs to be checked
        Frontier check_num_arc_f = Frontier(Frontier::QueueMode);
        _unit_test_befriender::TestBefriender::getAllToDoArcs(solver, sudoku_graph, check_num_arc_f);
        BOOST_TEST_REQUIRE(check_num_arc_f.size(), 243*9);
    };

    ~TestCSPSolver_ArcConsistency_Fixture()
    {};

    void print_sudoku(CSPGraph& graph)
    {
        std::vector<VariableVertex*> non_unique_domains;

        for (int y=1; y < 10; y++)
        {
            for (int x=1; x < 10; x++)
            {
                std::string vv_name = "Square " + std::to_string(y) + 
                                        "-" + std::to_string(x);
                auto var_domain = graph.get_variable(vv_name)->getDomain();
                // determine the printed symbol
                std::string symbol;
                if (var_domain.size() == 0) symbol = "x";
                else if (var_domain.size() == 1) symbol = std::to_string(*var_domain.begin());
                else 
                {
                    symbol = " ";
                    non_unique_domains.push_back(graph.get_variable(vv_name));
                }

                std::cout << symbol << "|";
                if (x % 3 == 0) std::cout << "|";
            }
            std::cout << std::endl;
            if (y % 3 == 0) std::cout << "_____________________" << std::endl;
        }
        std::sort(non_unique_domains.begin(), non_unique_domains.end(),
        [] (VariableVertex* vv1, VariableVertex* vv2)
        {
            return vv1->getName() < vv2->getName();
        });
        std::cout << "Variables with undetermined domains:" << std::endl;
        for (auto vv : non_unique_domains) std::cout << *vv << std::endl; 
        std::cout << std::endl;
        std::cout << std::endl;
    };
};

BOOST_FIXTURE_TEST_SUITE(CSPSolver_test_suite_ArcConsistency, 
                         TestCSPSolver_ArcConsistency_Fixture, 
                         * boost::unit_test::label("CSPSolver"));

    // run arc consistency and return all possible answers
    // std::vector<std::vector<GraphImplementation::VariableVertex>> arcConsistency(CSPGraph graph);
    BOOST_AUTO_TEST_SUITE(arcConsistency, * boost::unit_test::label("arcConsistency"));

        BOOST_AUTO_TEST_CASE(arc_consistency_no_solution) {

            // setup: create a sudoku puzzle that has no solution
            // square 1-1 should take the value 9, but we remove this when initializing

            // first row
            sudoku_graph.get_variable("Square 1-1")->removeFromDomain({9}); // <-removal
            sudoku_graph.get_variable("Square 1-2")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 1-3")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 1-4")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 1-5")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 1-6")->removeFromDomain({1,2,3,4,5,6  ,8,9});
            sudoku_graph.get_variable("Square 1-7")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 1-8")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 1-9")->removeFromDomain({1,2,3,  5,6,7,8,9});
            // second row
            sudoku_graph.get_variable("Square 2-1")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 2-2")->removeFromDomain({});
            sudoku_graph.get_variable("Square 2-3")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 2-4")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 2-5")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 2-6")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 2-7")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 2-8")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 2-9")->removeFromDomain({1,2,3,4,5,6,7,  9});
            // third row
            sudoku_graph.get_variable("Square 3-1")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 3-2")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 3-3")->removeFromDomain({});
            sudoku_graph.get_variable("Square 3-4")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 3-5")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 3-6")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 3-7")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 3-8")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 3-9")->removeFromDomain({1,  3,4,5,6,7,8,9});
            // fourth row
            sudoku_graph.get_variable("Square 4-1")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 4-2")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 4-3")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 4-4")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 4-5")->removeFromDomain({});
            sudoku_graph.get_variable("Square 4-6")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 4-7")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 4-8")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 4-9")->removeFromDomain({1,2,3,4,5,6,  8,9});
            // fifth row
            sudoku_graph.get_variable("Square 5-1")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-2")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-3")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-4")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 5-5")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-6")->removeFromDomain({});
            sudoku_graph.get_variable("Square 5-7")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 5-8")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 5-9")->removeFromDomain({1,2,3,4  ,6,7,8,9});
            // sixth row
            sudoku_graph.get_variable("Square 6-1")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 6-2")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 6-3")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 6-4")->removeFromDomain({});
            sudoku_graph.get_variable("Square 6-5")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 6-6")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 6-7")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 6-8")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 6-9")->removeFromDomain({1,2,  4,5,6,7,8,9});
            // seventh row
            sudoku_graph.get_variable("Square 7-1")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 7-2")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-3")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-4")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-5")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 7-6")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 7-7")->removeFromDomain({});
            sudoku_graph.get_variable("Square 7-8")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-9")->removeFromDomain({1,2,3,4,5,  7,8,9});
            // eith row
            sudoku_graph.get_variable("Square 8-1")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-2")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-3")->removeFromDomain({});
            sudoku_graph.get_variable("Square 8-4")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 8-5")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 8-6")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-7")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-8")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 8-9")->removeFromDomain({1,2,3,4,5,6,7,8  });
            // nineth row
            sudoku_graph.get_variable("Square 9-1")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 9-2")->removeFromDomain({});
            sudoku_graph.get_variable("Square 9-3")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 9-4")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 9-5")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 9-6")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 9-7")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 9-8")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 9-9")->removeFromDomain({  2,3,4,5,6,7,8,9});

            // If interested, we can visualize this
            // std::cout << "PRINTING CSPGRAPH FROM SUDOKU" << std::endl;
            // std::cout << sudoku_graph;

            // call arcConsistency
            auto answers = solver.arcConsistency(sudoku_graph);

            //test: check that we have no answer - an empty solution vector
            BOOST_TEST(answers.empty());
        }
        
        BOOST_AUTO_TEST_CASE(real_use_case_easy_version) {

            // setup: create a sudoku puzzle with the following initialization.
            /*
            x'8'2|1'6'7|5'3'4
            5'x'3|9'2'4|1'6'8
            1'4'x|3'5'8|9'7'2
            _________________
            3'6'8|5'x'9|2'4'7
            2'1'4|7'3'x|8'9'5
            7'5'9|x'4'2|6'1'3
            _________________
            8'3'1|4'9'5|x'2'6
            4'2'x|6'7'1|3'8'9
            6'x'7|2'8'3|4'5'1
            */

            // first row
            sudoku_graph.get_variable("Square 1-1")->removeFromDomain({});
            sudoku_graph.get_variable("Square 1-2")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 1-3")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 1-4")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 1-5")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 1-6")->removeFromDomain({1,2,3,4,5,6  ,8,9});
            sudoku_graph.get_variable("Square 1-7")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 1-8")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 1-9")->removeFromDomain({1,2,3,  5,6,7,8,9});
            // second row
            sudoku_graph.get_variable("Square 2-1")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 2-2")->removeFromDomain({});
            sudoku_graph.get_variable("Square 2-3")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 2-4")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 2-5")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 2-6")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 2-7")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 2-8")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 2-9")->removeFromDomain({1,2,3,4,5,6,7,  9});
            // third row
            sudoku_graph.get_variable("Square 3-1")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 3-2")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 3-3")->removeFromDomain({});
            sudoku_graph.get_variable("Square 3-4")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 3-5")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 3-6")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 3-7")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 3-8")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 3-9")->removeFromDomain({1,  3,4,5,6,7,8,9});
            // fourth row
            sudoku_graph.get_variable("Square 4-1")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 4-2")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 4-3")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 4-4")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 4-5")->removeFromDomain({});
            sudoku_graph.get_variable("Square 4-6")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 4-7")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 4-8")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 4-9")->removeFromDomain({1,2,3,4,5,6,  8,9});
            // fifth row
            sudoku_graph.get_variable("Square 5-1")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-2")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-3")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-4")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 5-5")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-6")->removeFromDomain({});
            sudoku_graph.get_variable("Square 5-7")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 5-8")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 5-9")->removeFromDomain({1,2,3,4  ,6,7,8,9});
            // sixth row
            sudoku_graph.get_variable("Square 6-1")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 6-2")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 6-3")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 6-4")->removeFromDomain({});
            sudoku_graph.get_variable("Square 6-5")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 6-6")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 6-7")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 6-8")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 6-9")->removeFromDomain({1,2,  4,5,6,7,8,9});
            // seventh row
            sudoku_graph.get_variable("Square 7-1")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 7-2")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-3")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-4")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-5")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 7-6")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 7-7")->removeFromDomain({});
            sudoku_graph.get_variable("Square 7-8")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-9")->removeFromDomain({1,2,3,4,5,  7,8,9});
            // eith row
            sudoku_graph.get_variable("Square 8-1")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-2")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-3")->removeFromDomain({});
            sudoku_graph.get_variable("Square 8-4")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 8-5")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 8-6")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-7")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-8")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 8-9")->removeFromDomain({1,2,3,4,5,6,7,8  });
            // nineth row
            sudoku_graph.get_variable("Square 9-1")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 9-2")->removeFromDomain({});
            sudoku_graph.get_variable("Square 9-3")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 9-4")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 9-5")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 9-6")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 9-7")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 9-8")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 9-9")->removeFromDomain({  2,3,4,5,6,7,8,9});

            // If interested, we can visualize this
            // std::cout << "PRINTING CSPGRAPH FROM SUDOKU" << std::endl;
            // std::cout << sudoku_graph;

            // call arcConsistency
            auto answers = solver.arcConsistency(sudoku_graph);

            //guard against empty vector
            BOOST_REQUIRE_EQUAL(answers.size(), 1);
            auto actual_answer = answers[0];

            // test: test that we get the correct answer
            /*
            ANSWER:
            9'8'2|1'6'7|5'3'4
            5'7'3|9'2'4|1'6'8
            1'4'6|3'5'8|9'7'2
            _________________
            3'6'8|5'1'9|2'4'7
            2'1'4|7'3'6|8'9'5
            7'5'9|8'4'2|6'1'3
            _________________
            8'3'1|4'9'5|7'2'6
            4'2'5|6'7'1|3'8'9
            6'9'7|2'8'3|4'5'1
            */
            std::vector<VariableVertex> expected_answer;
            // first row
            expected_answer.push_back(VariableVertex("Square 1-1", {9}));
            expected_answer.push_back(VariableVertex("Square 1-2", {8}));
            expected_answer.push_back(VariableVertex("Square 1-3", {2}));
            expected_answer.push_back(VariableVertex("Square 1-4", {1}));
            expected_answer.push_back(VariableVertex("Square 1-5", {6}));
            expected_answer.push_back(VariableVertex("Square 1-6", {7}));
            expected_answer.push_back(VariableVertex("Square 1-7", {5}));
            expected_answer.push_back(VariableVertex("Square 1-8", {3}));
            expected_answer.push_back(VariableVertex("Square 1-9", {4})); 
            // second row
            expected_answer.push_back(VariableVertex("Square 2-1", {5}));
            expected_answer.push_back(VariableVertex("Square 2-2", {7}));
            expected_answer.push_back(VariableVertex("Square 2-3", {3}));
            expected_answer.push_back(VariableVertex("Square 2-4", {9}));
            expected_answer.push_back(VariableVertex("Square 2-5", {2}));
            expected_answer.push_back(VariableVertex("Square 2-6", {4}));
            expected_answer.push_back(VariableVertex("Square 2-7", {1}));
            expected_answer.push_back(VariableVertex("Square 2-8", {6}));
            expected_answer.push_back(VariableVertex("Square 2-9", {8})); 
            // third row
            expected_answer.push_back(VariableVertex("Square 3-1", {1}));
            expected_answer.push_back(VariableVertex("Square 3-2", {4}));
            expected_answer.push_back(VariableVertex("Square 3-3", {6}));
            expected_answer.push_back(VariableVertex("Square 3-4", {3}));
            expected_answer.push_back(VariableVertex("Square 3-5", {5}));
            expected_answer.push_back(VariableVertex("Square 3-6", {8}));
            expected_answer.push_back(VariableVertex("Square 3-7", {9}));
            expected_answer.push_back(VariableVertex("Square 3-8", {7}));
            expected_answer.push_back(VariableVertex("Square 3-9", {2})); 
            // fourth row
            expected_answer.push_back(VariableVertex("Square 4-1", {3}));
            expected_answer.push_back(VariableVertex("Square 4-2", {6}));
            expected_answer.push_back(VariableVertex("Square 4-3", {8}));
            expected_answer.push_back(VariableVertex("Square 4-4", {5}));
            expected_answer.push_back(VariableVertex("Square 4-5", {1}));
            expected_answer.push_back(VariableVertex("Square 4-6", {9}));
            expected_answer.push_back(VariableVertex("Square 4-7", {2}));
            expected_answer.push_back(VariableVertex("Square 4-8", {4}));
            expected_answer.push_back(VariableVertex("Square 4-9", {7})); 
            // fifth row
            expected_answer.push_back(VariableVertex("Square 5-1", {2}));
            expected_answer.push_back(VariableVertex("Square 5-2", {1}));
            expected_answer.push_back(VariableVertex("Square 5-3", {4}));
            expected_answer.push_back(VariableVertex("Square 5-4", {7}));
            expected_answer.push_back(VariableVertex("Square 5-5", {3}));
            expected_answer.push_back(VariableVertex("Square 5-6", {6}));
            expected_answer.push_back(VariableVertex("Square 5-7", {8}));
            expected_answer.push_back(VariableVertex("Square 5-8", {9}));
            expected_answer.push_back(VariableVertex("Square 5-9", {5})); 
            // sixth row
            expected_answer.push_back(VariableVertex("Square 6-1", {7}));
            expected_answer.push_back(VariableVertex("Square 6-2", {5}));
            expected_answer.push_back(VariableVertex("Square 6-3", {9}));
            expected_answer.push_back(VariableVertex("Square 6-4", {8}));
            expected_answer.push_back(VariableVertex("Square 6-5", {4}));
            expected_answer.push_back(VariableVertex("Square 6-6", {2}));
            expected_answer.push_back(VariableVertex("Square 6-7", {6}));
            expected_answer.push_back(VariableVertex("Square 6-8", {1}));
            expected_answer.push_back(VariableVertex("Square 6-9", {3})); 
            // seventh row
            expected_answer.push_back(VariableVertex("Square 7-1", {8}));
            expected_answer.push_back(VariableVertex("Square 7-2", {3}));
            expected_answer.push_back(VariableVertex("Square 7-3", {1}));
            expected_answer.push_back(VariableVertex("Square 7-4", {4}));
            expected_answer.push_back(VariableVertex("Square 7-5", {9}));
            expected_answer.push_back(VariableVertex("Square 7-6", {5}));
            expected_answer.push_back(VariableVertex("Square 7-7", {7}));
            expected_answer.push_back(VariableVertex("Square 7-8", {2}));
            expected_answer.push_back(VariableVertex("Square 7-9", {6})); 
            // eith row
            expected_answer.push_back(VariableVertex("Square 8-1", {4}));
            expected_answer.push_back(VariableVertex("Square 8-2", {2}));
            expected_answer.push_back(VariableVertex("Square 8-3", {5}));
            expected_answer.push_back(VariableVertex("Square 8-4", {6}));
            expected_answer.push_back(VariableVertex("Square 8-5", {7}));
            expected_answer.push_back(VariableVertex("Square 8-6", {1}));
            expected_answer.push_back(VariableVertex("Square 8-7", {3}));
            expected_answer.push_back(VariableVertex("Square 8-8", {8}));
            expected_answer.push_back(VariableVertex("Square 8-9", {9})); 
            // nineth row
            expected_answer.push_back(VariableVertex("Square 9-1", {6}));
            expected_answer.push_back(VariableVertex("Square 9-2", {9}));
            expected_answer.push_back(VariableVertex("Square 9-3", {7}));
            expected_answer.push_back(VariableVertex("Square 9-4", {2}));
            expected_answer.push_back(VariableVertex("Square 9-5", {8}));
            expected_answer.push_back(VariableVertex("Square 9-6", {3}));
            expected_answer.push_back(VariableVertex("Square 9-7", {4}));
            expected_answer.push_back(VariableVertex("Square 9-8", {5}));
            expected_answer.push_back(VariableVertex("Square 9-9", {1}));
            
            // sort both the answer and expected variables based on order of variable name
            auto first_variable_vertex_is_smaller_by_name = [] 
            (VariableVertex vv1, VariableVertex vv2) 
            {
                return vv1.getName() < vv2.getName();
            };
            std::sort(expected_answer.begin(), expected_answer.end(), 
                      first_variable_vertex_is_smaller_by_name);
            std::sort(actual_answer.begin(), actual_answer.end(), 
                      first_variable_vertex_is_smaller_by_name);
            // finally compare the expected and actual answer
            BOOST_CHECK_EQUAL_COLLECTIONS(expected_answer.begin(), expected_answer.end(),
                                            actual_answer.begin(),   actual_answer.end());
        }

        BOOST_AUTO_TEST_CASE(real_use_case_hard_version) {

            // setup: create a sudoku puzzle with the following initialization.
            /*
            x'x'x|x'6'7|5'3'x
            5'x'x|9'2'x|1'x'x
            x'4'x|x'x'x|x'7'x
            _________________
            x'x'x|x'x'9|2'x'x
            x'1'4|7'3'x|x'x'x
            7'x'9|x'x'x|x'1'x
            _________________
            x'3'x|x'x'x|x'2'6
            4'2'5|x'7'1|x'8'9
            6'x'7|2'8'3|x'5'1
            */
            
            // first row
            sudoku_graph.get_variable("Square 1-5")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 1-6")->removeFromDomain({1,2,3,4,5,6  ,8,9});
            sudoku_graph.get_variable("Square 1-7")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 1-8")->removeFromDomain({1,2,  4,5,6,7,8,9});
            // second row
            sudoku_graph.get_variable("Square 2-1")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 2-4")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 2-5")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 2-7")->removeFromDomain({  2,3,4,5,6,7,8,9});
            // third row
            sudoku_graph.get_variable("Square 3-2")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 3-8")->removeFromDomain({1,2,3,4,5,6,  8,9});
            // fourth row
            sudoku_graph.get_variable("Square 4-6")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 4-7")->removeFromDomain({1,  3,4,5,6,7,8,9});
            // fifth row
            sudoku_graph.get_variable("Square 5-2")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-3")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 5-4")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 5-5")->removeFromDomain({1,2,  4,5,6,7,8,9});
            // sixth row
            sudoku_graph.get_variable("Square 6-1")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 6-3")->removeFromDomain({1,2,3,4,5,6,7,8  });
            sudoku_graph.get_variable("Square 6-8")->removeFromDomain({  2,3,4,5,6,7,8,9});
            // seventh row
            sudoku_graph.get_variable("Square 7-2")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-8")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 7-9")->removeFromDomain({1,2,3,4,5,  7,8,9});
            // eith row
            sudoku_graph.get_variable("Square 8-1")->removeFromDomain({1,2,3,  5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-2")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-3")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 8-5")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 8-6")->removeFromDomain({  2,3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 8-8")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 8-9")->removeFromDomain({1,2,3,4,5,6,7,8  });
            // nineth row
            sudoku_graph.get_variable("Square 9-1")->removeFromDomain({1,2,3,4,5,  7,8,9});
            sudoku_graph.get_variable("Square 9-3")->removeFromDomain({1,2,3,4,5,6,  8,9});
            sudoku_graph.get_variable("Square 9-4")->removeFromDomain({1,  3,4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 9-5")->removeFromDomain({1,2,3,4,5,6,7,  9});
            sudoku_graph.get_variable("Square 9-6")->removeFromDomain({1,2,  4,5,6,7,8,9});
            sudoku_graph.get_variable("Square 9-8")->removeFromDomain({1,2,3,4,  6,7,8,9});
            sudoku_graph.get_variable("Square 9-9")->removeFromDomain({  2,3,4,5,6,7,8,9});

            // If interested, we can visualize this
            // std::cout << "PRINTING CSPGRAPH FROM SUDOKU" << std::endl;
            // std::cout << sudoku_graph;

            // call arcConsistency
            auto answers = solver.arcConsistency(sudoku_graph);

            //guard against empty vector
            BOOST_REQUIRE_EQUAL(answers.size(), 1);
            auto actual_answer = answers[0];

            // test: test that we get the correct answer
            /*
            ANSWER:
            9'8'2|1'6'7|5'3'4
            5'7'3|9'2'4|1'6'8
            1'4'6|3'5'8|9'7'2
            _________________
            3'6'8|5'1'9|2'4'7
            2'1'4|7'3'6|8'9'5
            7'5'9|8'4'2|6'1'3
            _________________
            8'3'1|4'9'5|7'2'6
            4'2'5|6'7'1|3'8'9
            6'9'7|2'8'3|4'5'1
            */
            std::vector<VariableVertex> expected_answer;
            // first row
            expected_answer.push_back(VariableVertex("Square 1-1", {9}));
            expected_answer.push_back(VariableVertex("Square 1-2", {8}));
            expected_answer.push_back(VariableVertex("Square 1-3", {2}));
            expected_answer.push_back(VariableVertex("Square 1-4", {1}));
            expected_answer.push_back(VariableVertex("Square 1-5", {6}));
            expected_answer.push_back(VariableVertex("Square 1-6", {7}));
            expected_answer.push_back(VariableVertex("Square 1-7", {5}));
            expected_answer.push_back(VariableVertex("Square 1-8", {3}));
            expected_answer.push_back(VariableVertex("Square 1-9", {4})); 
            // second row
            expected_answer.push_back(VariableVertex("Square 2-1", {5}));
            expected_answer.push_back(VariableVertex("Square 2-2", {7}));
            expected_answer.push_back(VariableVertex("Square 2-3", {3}));
            expected_answer.push_back(VariableVertex("Square 2-4", {9}));
            expected_answer.push_back(VariableVertex("Square 2-5", {2}));
            expected_answer.push_back(VariableVertex("Square 2-6", {4}));
            expected_answer.push_back(VariableVertex("Square 2-7", {1}));
            expected_answer.push_back(VariableVertex("Square 2-8", {6}));
            expected_answer.push_back(VariableVertex("Square 2-9", {8})); 
            // third row
            expected_answer.push_back(VariableVertex("Square 3-1", {1}));
            expected_answer.push_back(VariableVertex("Square 3-2", {4}));
            expected_answer.push_back(VariableVertex("Square 3-3", {6}));
            expected_answer.push_back(VariableVertex("Square 3-4", {3}));
            expected_answer.push_back(VariableVertex("Square 3-5", {5}));
            expected_answer.push_back(VariableVertex("Square 3-6", {8}));
            expected_answer.push_back(VariableVertex("Square 3-7", {9}));
            expected_answer.push_back(VariableVertex("Square 3-8", {7}));
            expected_answer.push_back(VariableVertex("Square 3-9", {2})); 
            // fourth row
            expected_answer.push_back(VariableVertex("Square 4-1", {3}));
            expected_answer.push_back(VariableVertex("Square 4-2", {6}));
            expected_answer.push_back(VariableVertex("Square 4-3", {8}));
            expected_answer.push_back(VariableVertex("Square 4-4", {5}));
            expected_answer.push_back(VariableVertex("Square 4-5", {1}));
            expected_answer.push_back(VariableVertex("Square 4-6", {9}));
            expected_answer.push_back(VariableVertex("Square 4-7", {2}));
            expected_answer.push_back(VariableVertex("Square 4-8", {4}));
            expected_answer.push_back(VariableVertex("Square 4-9", {7})); 
            // fifth row
            expected_answer.push_back(VariableVertex("Square 5-1", {2}));
            expected_answer.push_back(VariableVertex("Square 5-2", {1}));
            expected_answer.push_back(VariableVertex("Square 5-3", {4}));
            expected_answer.push_back(VariableVertex("Square 5-4", {7}));
            expected_answer.push_back(VariableVertex("Square 5-5", {3}));
            expected_answer.push_back(VariableVertex("Square 5-6", {6}));
            expected_answer.push_back(VariableVertex("Square 5-7", {8}));
            expected_answer.push_back(VariableVertex("Square 5-8", {9}));
            expected_answer.push_back(VariableVertex("Square 5-9", {5})); 
            // sixth row
            expected_answer.push_back(VariableVertex("Square 6-1", {7}));
            expected_answer.push_back(VariableVertex("Square 6-2", {5}));
            expected_answer.push_back(VariableVertex("Square 6-3", {9}));
            expected_answer.push_back(VariableVertex("Square 6-4", {8}));
            expected_answer.push_back(VariableVertex("Square 6-5", {4}));
            expected_answer.push_back(VariableVertex("Square 6-6", {2}));
            expected_answer.push_back(VariableVertex("Square 6-7", {6}));
            expected_answer.push_back(VariableVertex("Square 6-8", {1}));
            expected_answer.push_back(VariableVertex("Square 6-9", {3})); 
            // seventh row
            expected_answer.push_back(VariableVertex("Square 7-1", {8}));
            expected_answer.push_back(VariableVertex("Square 7-2", {3}));
            expected_answer.push_back(VariableVertex("Square 7-3", {1}));
            expected_answer.push_back(VariableVertex("Square 7-4", {4}));
            expected_answer.push_back(VariableVertex("Square 7-5", {9}));
            expected_answer.push_back(VariableVertex("Square 7-6", {5}));
            expected_answer.push_back(VariableVertex("Square 7-7", {7}));
            expected_answer.push_back(VariableVertex("Square 7-8", {2}));
            expected_answer.push_back(VariableVertex("Square 7-9", {6})); 
            // eith row
            expected_answer.push_back(VariableVertex("Square 8-1", {4}));
            expected_answer.push_back(VariableVertex("Square 8-2", {2}));
            expected_answer.push_back(VariableVertex("Square 8-3", {5}));
            expected_answer.push_back(VariableVertex("Square 8-4", {6}));
            expected_answer.push_back(VariableVertex("Square 8-5", {7}));
            expected_answer.push_back(VariableVertex("Square 8-6", {1}));
            expected_answer.push_back(VariableVertex("Square 8-7", {3}));
            expected_answer.push_back(VariableVertex("Square 8-8", {8}));
            expected_answer.push_back(VariableVertex("Square 8-9", {9})); 
            // nineth row
            expected_answer.push_back(VariableVertex("Square 9-1", {6}));
            expected_answer.push_back(VariableVertex("Square 9-2", {9}));
            expected_answer.push_back(VariableVertex("Square 9-3", {7}));
            expected_answer.push_back(VariableVertex("Square 9-4", {2}));
            expected_answer.push_back(VariableVertex("Square 9-5", {8}));
            expected_answer.push_back(VariableVertex("Square 9-6", {3}));
            expected_answer.push_back(VariableVertex("Square 9-7", {4}));
            expected_answer.push_back(VariableVertex("Square 9-8", {5}));
            expected_answer.push_back(VariableVertex("Square 9-9", {1}));
            
            // sort both the answer and expected variables based on order of variable name
            auto first_variable_vertex_is_smaller_by_name = [] 
            (VariableVertex vv1, VariableVertex vv2) 
            {
                return vv1.getName() < vv2.getName();
            };
            std::sort(expected_answer.begin(), expected_answer.end(), 
                      first_variable_vertex_is_smaller_by_name);
            std::sort(actual_answer.begin(), actual_answer.end(), 
                      first_variable_vertex_is_smaller_by_name);
            // finally compare the expected and actual answer
            BOOST_CHECK_EQUAL_COLLECTIONS(expected_answer.begin(), expected_answer.end(),
                                            actual_answer.begin(),   actual_answer.end());
        }

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();