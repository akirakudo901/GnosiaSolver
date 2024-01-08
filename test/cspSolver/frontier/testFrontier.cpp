// Author: Akira Kudo
// Description: Implements tests for the Frontier class under CSPSolverImplementation.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <queue>
#include <vector>

#include "src/cspSolver/ARC.h"
#include "src/cspSolver/frontier/Frontier.h"

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

// define fixture
struct TestFrontier_Fixture
{
    CSPSolverImplementation::Frontier frontier = CSPSolverImplementation::Frontier(CSPSolverImplementation::Frontier::QueueMode);

    GraphImplementation::VariableVertex vv1 = GraphImplementation::VariableVertex("", {});
    GraphImplementation::VariableVertex vv2 = GraphImplementation::VariableVertex("", {});
    GraphImplementation::VariableVertex vv3 = GraphImplementation::VariableVertex("", {});
    GraphImplementation::VariableVertex vv4 = GraphImplementation::VariableVertex("", {});

    GraphImplementation::ConstraintVertex cv1 = GraphImplementation::ConstraintVertex("", GraphImplementation::ConstraintVertex::exactlyN(0, 0));
    GraphImplementation::ConstraintVertex cv2 = GraphImplementation::ConstraintVertex("", GraphImplementation::ConstraintVertex::exactlyN(0, 0));
    GraphImplementation::ConstraintVertex cv3 = GraphImplementation::ConstraintVertex("", GraphImplementation::ConstraintVertex::exactlyN(0, 0));
    GraphImplementation::ConstraintVertex cv4 = GraphImplementation::ConstraintVertex("", GraphImplementation::ConstraintVertex::exactlyN(0, 0));

    CSPSolverImplementation::ARC unary1;
    CSPSolverImplementation::ARC unary2;
    CSPSolverImplementation::ARC non_unary1; 
    CSPSolverImplementation::ARC non_unary2;

    TestFrontier_Fixture() {
        frontier  = CSPSolverImplementation::Frontier(CSPSolverImplementation::Frontier::QueueMode);
        vv1 = GraphImplementation::VariableVertex("vv1", {10, 11, 12});
        vv2 = GraphImplementation::VariableVertex("vv2", {20, 21, 22});
        vv3 = GraphImplementation::VariableVertex("vv3", {30, 31, 32});
        vv4 = GraphImplementation::VariableVertex("vv4", {40, 41, 42});

        cv1 = GraphImplementation::ConstraintVertex("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
        cv2 = GraphImplementation::ConstraintVertex("cv2", GraphImplementation::ConstraintVertex::exactlyN(2, 3));
        cv3 = GraphImplementation::ConstraintVertex("cv3", GraphImplementation::ConstraintVertex::greaterOrEqualToN(3, 2));
        cv4 = GraphImplementation::ConstraintVertex("cv4", GraphImplementation::ConstraintVertex::lesserOrEqualToN(1, 2));

        unary1.main_var = &vv1; unary1.constraint = &cv1;
        unary1.other_var_list = std::vector<GraphImplementation::VariableVertex*>();
        
        unary2.main_var = &vv2; unary2.constraint = &cv2;
        unary2.other_var_list = std::vector<GraphImplementation::VariableVertex*>(); 
        
        non_unary1.main_var = &vv3; non_unary1.constraint = &cv3;
        non_unary1.other_var_list = std::vector<GraphImplementation::VariableVertex*>({&vv1, &vv2}); 
        
        non_unary2.main_var = &vv4; non_unary2.constraint = &cv4;
        non_unary2.other_var_list = std::vector<GraphImplementation::VariableVertex*>({&vv3, &vv4}); 
    };

    ~TestFrontier_Fixture() {};
};


BOOST_FIXTURE_TEST_SUITE(Frontier_test_suite, TestFrontier_Fixture, * boost::unit_test::label("Frontier"));
    
    // check the copy constructor
    BOOST_AUTO_TEST_SUITE(copy_constructor);

        BOOST_AUTO_TEST_CASE(preserves_mode) {
            // setup: create another frontier by copy
            CSPSolverImplementation::Frontier new_f = frontier;
            
            // test: check the new frontier preserves mode
            BOOST_CHECK_EQUAL(frontier.getMode(), new_f.getMode());
        };

        BOOST_AUTO_TEST_CASE(preserves_arcs) {
            // presetup: add some arcs
            frontier.push(unary2);
            frontier.push(non_unary2);
            frontier.push(unary1);
            frontier.push(non_unary1);
            
            // preconditions: check frontier is of size 4
            BOOST_TEST_REQUIRE(frontier.size() == 4);

            // setup: then create another frontier by copy
            CSPSolverImplementation::Frontier new_f = frontier;
            
            // test: check the new frontier preserves arcs and their order
            BOOST_CHECK_EQUAL(new_f.size(), 4);
            BOOST_CHECK_EQUAL(new_f.pop(), unary2);
            BOOST_CHECK_EQUAL(new_f.pop(), unary1);
            BOOST_CHECK_EQUAL(new_f.pop(), non_unary2);
            BOOST_CHECK_EQUAL(new_f.pop(), non_unary1);
        };

        BOOST_AUTO_TEST_CASE(preserves_which_arc_is_in_frontier) {
            // presetup: add some arcs
            frontier.push(unary2);
            frontier.push(non_unary2);
            frontier.push(unary1);
            frontier.push(non_unary1);
            
            // preconditions: check frontier is of size 4, and create a copy
            BOOST_TEST_REQUIRE(frontier.size() == 4);
            CSPSolverImplementation::Frontier new_f = frontier;
            BOOST_TEST_REQUIRE(new_f.size() == 4);

            // setup: then add identical arcs to the new one
            new_f.push(unary1);
            new_f.push(unary2);
            new_f.push(non_unary1);
            new_f.push(non_unary2);

            // test: check the new frontier didn't add the four later arcs
            BOOST_CHECK_EQUAL(new_f.size(), 4);
            BOOST_CHECK_EQUAL(new_f.pop(), unary2);
            BOOST_CHECK_EQUAL(new_f.pop(), unary1);
            BOOST_CHECK_EQUAL(new_f.pop(), non_unary2);
            BOOST_CHECK_EQUAL(new_f.pop(), non_unary1);
        };

    BOOST_AUTO_TEST_SUITE_END();

    // push an arc that is an ARC struct to frontier
    // void push(CSPSolverImplementation::ARC arc);
    BOOST_AUTO_TEST_SUITE(push);

        BOOST_AUTO_TEST_CASE(push_1_unary) {
            // preconditions: check nothing is in the frontier
            BOOST_TEST_REQUIRE(frontier.size() == 0);
            // setup: push one unary arc
            frontier.push(unary1);
            // test: test the pushed one is in now
            BOOST_CHECK_EQUAL(frontier.size(), 1);
            BOOST_CHECK_EQUAL(frontier.pop(), unary1);
        };

        BOOST_AUTO_TEST_CASE(push_1_non_unary) {
            // preconditions: check nothing is in the frontier
            BOOST_TEST_REQUIRE(frontier.empty());
            // setup: push one non-unary arc
            frontier.push(non_unary1); 
            // test: test the pushed one is in now
            BOOST_CHECK_EQUAL(frontier.size(), 1);
            BOOST_CHECK_EQUAL(frontier.pop(), non_unary1);
        };

        BOOST_AUTO_TEST_CASE(push_2_unary_2_non_unary) {
            // preconditions: check nothing is in the frontier
            BOOST_TEST_REQUIRE(frontier.empty());
            // setup: push two unary & two non-unary arcs
            frontier.push(unary1); 
            frontier.push(unary2); 
            frontier.push(non_unary1); 
            frontier.push(non_unary2); 
            // test: test the pushed ones are in now
            BOOST_CHECK_EQUAL(frontier.size(), 4);
            BOOST_CHECK_EQUAL(frontier.pop(), unary1);
            BOOST_CHECK_EQUAL(frontier.pop(), unary2);
            BOOST_CHECK_EQUAL(frontier.pop(), non_unary1);
            BOOST_CHECK_EQUAL(frontier.pop(), non_unary2);
        };

        BOOST_AUTO_TEST_CASE(push_2_unary_2_non_unary_with_duplicate) {
            // preconditions: check nothing is in the frontier
            BOOST_TEST_REQUIRE(frontier.empty());
            // setup: push two unary & two non-unary arcs
            frontier.push(unary1); 
            frontier.push(unary2); 
            frontier.push(non_unary1); 
            frontier.push(non_unary2); 
            // test there are four entries in the frontier
            BOOST_CHECK_EQUAL(frontier.size(), 4);

            // test: then push the same arcs one more time
            frontier.push(unary1); 
            frontier.push(unary2); 
            frontier.push(non_unary1); 
            frontier.push(non_unary2); 
            // test the pushed ones did not duplicate, but are in now
            BOOST_CHECK_EQUAL(frontier.size(), 4);
            BOOST_CHECK_EQUAL(frontier.pop(), unary1);
            BOOST_CHECK_EQUAL(frontier.pop(), unary2);
            BOOST_CHECK_EQUAL(frontier.pop(), non_unary1);
            BOOST_CHECK_EQUAL(frontier.pop(), non_unary2);
        };

        BOOST_AUTO_TEST_CASE(push_2_unary_2_non_unary_after_push_and_pop) {
            // preconditions: check nothing is in the frontier
            BOOST_TEST_REQUIRE(frontier.empty());
            // setup: push two unary & two non-unary arcs
            frontier.push(unary1); 
            frontier.push(unary2); 
            frontier.push(non_unary1); 
            frontier.push(non_unary2); 
            // test there are four entries in the frontier
            BOOST_CHECK_EQUAL(frontier.size(), 4);
            // then pop them
            frontier.pop(); frontier.pop(); frontier.pop(); frontier.pop();
            BOOST_TEST(frontier.empty());

            // test: then push the same arcs one more time
            frontier.push(unary1); 
            frontier.push(unary2); 
            frontier.push(non_unary1); 
            frontier.push(non_unary2);
            // test the pushed ones are in there
            BOOST_REQUIRE_EQUAL(frontier.size(), 4);
            BOOST_CHECK_EQUAL(frontier.pop(), unary1);
            BOOST_CHECK_EQUAL(frontier.pop(), unary2);
            BOOST_CHECK_EQUAL(frontier.pop(), non_unary1);
            BOOST_CHECK_EQUAL(frontier.pop(), non_unary2);
        };

    BOOST_AUTO_TEST_SUITE_END();

    // pop an arc that is an ARC struct from the frontier
    // REQUIRES that this frontier isn't empty - or does undefined behavior!
    // CSPSolverImplementation::ARC pop();
    BOOST_AUTO_TEST_SUITE(pop);
        
        BOOST_AUTO_TEST_CASE(single_unary) {
            // presetup: have one unary arc in frontier
            frontier.push(unary1);
            // preconditions: check frontier isn't empty
            BOOST_REQUIRE_EQUAL(frontier.size(), 1);
            // test: pop and check we get the correct one
            CSPSolverImplementation::ARC popped = frontier.pop();
            BOOST_CHECK_EQUAL(popped, unary1);
        };

        BOOST_AUTO_TEST_CASE(single_non_unary) {
            // presetup: have one non-unary arc in frontier
            frontier.push(non_unary1);
            // preconditions: check frontier isn't empty
            BOOST_REQUIRE_EQUAL(frontier.size(), 1);
            // test: pop and check we get the correct one
            CSPSolverImplementation::ARC popped = frontier.pop();
            BOOST_CHECK_EQUAL(popped, non_unary1);
        };

        BOOST_AUTO_TEST_CASE(two_unary_two_non_unary) {
            // presetup: insert two unary and two non-unary arcs
            frontier.push(non_unary2);
            frontier.push(unary1);
            frontier.push(non_unary1);
            frontier.push(unary2);
            // preconditions: check the number of arcs in the frontier
            BOOST_REQUIRE_EQUAL(frontier.size(), 4);
            // test: pop to check we get unary first, then non-unary
            //  but in order of push between unary / non-unary
            BOOST_CHECK_EQUAL(frontier.pop(), unary1); //first pushed among unary
            BOOST_CHECK_EQUAL(frontier.pop(), unary2);
            BOOST_CHECK_EQUAL(frontier.pop(), non_unary2); //first pushed among non-unary
            BOOST_CHECK_EQUAL(frontier.pop(), non_unary1);
        };

    BOOST_AUTO_TEST_SUITE_END();

    // int size()
    BOOST_AUTO_TEST_SUITE(size);
        
        BOOST_AUTO_TEST_CASE(empty) {
            // preconditions: check frontier is emtpy
            BOOST_TEST_REQUIRE(frontier.empty() == true);
            // test: check there are 0 objects
            BOOST_CHECK_EQUAL(frontier.size(), 0);
        };

        BOOST_AUTO_TEST_CASE(one) {
            // preconditions: check frontier is emtpy
            BOOST_TEST_REQUIRE(frontier.empty() == true);
            // setup: add one arc
            frontier.push(unary2);
            // test: check there are 1 object
            BOOST_CHECK_EQUAL(frontier.size(), 1);
        };

        BOOST_AUTO_TEST_CASE(four) {
            // preconditions: check frontier is emtpy
            BOOST_TEST_REQUIRE(frontier.empty() == true);
            // setup: add four arcs
            frontier.push(unary1);
            frontier.push(unary2);
            frontier.push(non_unary1);
            frontier.push(non_unary2);
            // test: check there are 4 objects
            BOOST_CHECK_EQUAL(frontier.size(), 4);
        };

    BOOST_AUTO_TEST_SUITE_END();

    
    // bool empty()
    BOOST_AUTO_TEST_SUITE(empty);
        
        BOOST_AUTO_TEST_CASE(is_empty) {
            // test: check frontier is empty by default
            BOOST_TEST(frontier.empty() == true);
        };

        BOOST_AUTO_TEST_CASE(is_not_empty_1) {
            // setup: add one arc
            frontier.push(unary1);
            // test: check frontier isn't empty anymore
            BOOST_TEST(frontier.empty() == false);
        };

        BOOST_AUTO_TEST_CASE(is_not_empty_4_then_is_empty) {
            // setup: add four arcs and check we're not empty anymore
            frontier.push(unary1);
            frontier.push(unary2);
            frontier.push(non_unary1);
            frontier.push(non_unary2);
            BOOST_TEST(frontier.empty() == false);
            // then remove everything
            frontier.pop();
            frontier.pop();
            frontier.pop();
            frontier.pop();
            // test: check we're empty again
            BOOST_TEST(frontier.empty() == true);
        };

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();