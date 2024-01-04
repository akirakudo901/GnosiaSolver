// Author: Akira Kudo
// Description: Implements tests for the Frontier class under CSPSolverImplementation.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

// TODO REMOVE
#include <iostream>

#include <vector>

#include "src/cspSolver/ARC.h"
#include "src/cspSolver/frontier/Frontier.h"

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

// define fixture
struct F
{
    CSPSolverImplementation::Frontier no_way_this_collides_in_name = CSPSolverImplementation::Frontier(CSPSolverImplementation::Frontier::QueueMode);

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

    F() {
        no_way_this_collides_in_name = CSPSolverImplementation::Frontier(CSPSolverImplementation::Frontier::QueueMode);

        vv1 = GraphImplementation::VariableVertex("vv1", {10, 11, 12});
        vv2 = GraphImplementation::VariableVertex("vv2", {20, 21, 22});
        vv3 = GraphImplementation::VariableVertex("vv3", {30, 31, 32});
        vv4 = GraphImplementation::VariableVertex("vv4", {40, 41, 42});

        cv1 = GraphImplementation::ConstraintVertex("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
        cv2 = GraphImplementation::ConstraintVertex("cv2", GraphImplementation::ConstraintVertex::exactlyN(2, 3));
        cv3 = GraphImplementation::ConstraintVertex("cv3", GraphImplementation::ConstraintVertex::greaterOrEqualToN(3, 2));
        cv4 = GraphImplementation::ConstraintVertex("cv4", GraphImplementation::ConstraintVertex::lesserOrEqualToN(1, 2));

        unary1.main_var = &vv1; unary1.constraint = &cv1;
        unary1.other_var_list = std::vector<GraphImplementation::VariableVertex*>({}); 
        
        unary2.main_var = &vv2; unary2.constraint = &cv2;
        unary2.other_var_list = std::vector<GraphImplementation::VariableVertex*>({}); 
        
        non_unary1.main_var = &vv3; non_unary1.constraint = &cv3;
        non_unary1.other_var_list = std::vector<GraphImplementation::VariableVertex*>({&vv1, &vv2}); 
        
        non_unary2.main_var = &vv4; non_unary2.constraint = &cv4;
        non_unary2.other_var_list = std::vector<GraphImplementation::VariableVertex*>({&vv3, &vv4}); 
    };
    ~F() {};
};


BOOST_FIXTURE_TEST_SUITE(Frontier_test_suite, F, * boost::unit_test::label("Frontier"));
    
    // push an arc that is an ARC struct to frontier
    // void push(CSPSolverImplementation::ARC arc);
    BOOST_AUTO_TEST_SUITE(push);

        BOOST_AUTO_TEST_CASE(push_1_unary) {
            // preconditions: check nothing is in the frontier
            BOOST_TEST_REQUIRE(no_way_this_collides_in_name.empty() == true);
            // setup: push one unary arc
            no_way_this_collides_in_name.push(unary1);
            // test: test the pushed one is in now
            BOOST_CHECK_EQUAL(no_way_this_collides_in_name.size(), 1);
            BOOST_CHECK_EQUAL(no_way_this_collides_in_name.pop(), unary1);
        };

        BOOST_AUTO_TEST_CASE(push_1_non_unary) {
            // preconditions: check nothing is in the frontier
            BOOST_TEST_REQUIRE(no_way_this_collides_in_name.empty() == true);
            // setup: push one non-unary arc
            no_way_this_collides_in_name.push(non_unary1); 
            // test: test the pushed one is in now
            BOOST_CHECK_EQUAL(no_way_this_collides_in_name.size(), 1);
            // BOOST_CHECK_EQUAL(no_way_this_collides_in_name.pop(), non_unary1);
        };

        BOOST_AUTO_TEST_CASE(push_2_unary_2_non_unary) {
            // preconditions: check nothing is in the frontier
            BOOST_TEST_REQUIRE(no_way_this_collides_in_name.empty() == true);
            // setup: push two unary & two non-unary arcs
            no_way_this_collides_in_name.push(unary1); 
            no_way_this_collides_in_name.push(unary2); 
            no_way_this_collides_in_name.push(non_unary1); 
            no_way_this_collides_in_name.push(non_unary2); 
            // test: test the pushed ones are in now
            BOOST_CHECK_EQUAL(no_way_this_collides_in_name.size(), 4);
            // BOOST_CHECK_EQUAL(no_way_this_collides_in_name.pop(), unary1);
            // BOOST_CHECK_EQUAL(no_way_this_collides_in_name.pop(), unary2);
            // BOOST_CHECK_EQUAL(no_way_this_collides_in_name.pop(), non_unary1);
            // BOOST_CHECK_EQUAL(no_way_this_collides_in_name.pop(), non_unary2);
        };

    BOOST_AUTO_TEST_SUITE_END();

    // // pop an arc that is an ARC struct from the frontier
    // // REQUIRES that this frontier isn't empty - or does undefined behavior!
    // // CSPSolverImplementation::ARC pop();
    // BOOST_AUTO_TEST_SUITE(pop);
        
    //     // BOOST_AUTO_TEST_CASE() {
    //     //     // presetup:
    //     //     // preconditions:
    //     //     // setup:
    //     //     // test:
    //     // };

    // BOOST_AUTO_TEST_SUITE_END();

    // // int size()
    // BOOST_AUTO_TEST_SUITE(size);
        
    //     // BOOST_AUTO_TEST_CASE() {
    //     //     // presetup:
    //     //     // preconditions:
    //     //     // setup:
    //     //     // test:
    //     // };

    // BOOST_AUTO_TEST_SUITE_END();

    
    // // bool empty()
    // BOOST_AUTO_TEST_SUITE(empty);
        
    //     // BOOST_AUTO_TEST_CASE() {
    //     //     // presetup:
    //     //     // preconditions:
    //     //     // setup:
    //     //     // test:
    //     // };

    // BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();