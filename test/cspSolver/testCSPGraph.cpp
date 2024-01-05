// Author: Akira Kudo
// Description: Implements tests for functions contained within CSPGraph.cpp in CSPSolverImplementation.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include "src/cspSolver/CSPGraph.h"
#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using GraphImplementation::ConstraintVertex, GraphImplementation::VariableVertex; 

// defining fixture
struct TestCSPGraph_Fixture {
    CSPSolverImplementation::CSPGraph g;
    
    TestCSPGraph_Fixture() {
        g = CSPSolverImplementation::CSPGraph();
    };
    
    ~TestCSPGraph_Fixture() {};
};

BOOST_FIXTURE_TEST_SUITE(CSPGraph_test_suite, TestCSPGraph_Fixture, * boost::unit_test::label("CSPGraph"));

    // returns a pointer to a constraint vertex with the given name, or nullptr if it doesn't exist
    // GraphImplementation::ConstraintVertex* get_constraint(std::string name);
    BOOST_AUTO_TEST_SUITE(get_constraint);

        BOOST_AUTO_TEST_CASE(constraint_with_name_exists) {
            // setup: add constraint
            // this constraint only returns true if there can be one 1 in given domains
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(1, 1));

            // test: get constraint and check if it is the right one
            auto obtained_constraint = g.get_constraint("cv");
            // guard against null pointers returned by obtained_constraint
            BOOST_TEST_REQUIRE(obtained_constraint != nullptr);
            BOOST_CHECK_EQUAL(obtained_constraint->getName(), "cv");
            // this should return false, since there cannot be any 1 in given domains
            BOOST_TEST(obtained_constraint->constraintIsMet(0, {}) == false);
            // this should return true, since there is one 1 in given domains
            BOOST_TEST(obtained_constraint->constraintIsMet(1, {}) == true);
            // this should return false, since there cannot be any 1 in given domains
            BOOST_TEST(obtained_constraint->constraintIsMet(2, {}) == false);
        }

        BOOST_AUTO_TEST_CASE(constraint_with_name_does_not_exist) {
            // setup: add constraint
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(1, 1));

            // test: get constraint that doesn't exist and check it returns nullptr
            BOOST_TEST(g.get_constraint("non-existent") == nullptr);
        }

    BOOST_AUTO_TEST_SUITE_END();

    // returns a pointer to a variable vertex with the given name, or nullptr if it doesn't exist
    // GraphImplementation::VariableVertex* get_variable(std::string name);
    BOOST_AUTO_TEST_SUITE(get_variable);

        BOOST_AUTO_TEST_CASE(variable_with_name_exists) {
            // setup: add variable
            auto vv_domain = {0, 1};
            g.add_variable("vv", vv_domain);

            // test: get variable and check if it is the right one
            auto obtained_variable = g.get_variable("vv");
            // guard against null pointers
            BOOST_TEST_REQUIRE(obtained_variable != nullptr);

            // check name
            BOOST_CHECK_EQUAL(obtained_variable->getName(), "vv");
            // also check domain
            auto expected_domain = std::set<int>(vv_domain);
            auto actual_domain = obtained_variable->getDomain();
            BOOST_CHECK_EQUAL_COLLECTIONS(expected_domain.begin(), expected_domain.end(), actual_domain.begin(), actual_domain.end());
        }

        BOOST_AUTO_TEST_CASE(variable_with_name_does_not_exist) {
            // setup: add variable
            g.add_variable("vv", {0, 1});

            // test: get variable that doesn't exist and check it returns nullptr
            BOOST_TEST(g.get_variable("non-existent") == nullptr);
        }

    BOOST_AUTO_TEST_SUITE_END();

    // returns whether any vertex of given name is in the graph
    // bool contains_vertex(std::string name); 
    BOOST_AUTO_TEST_SUITE(contains_vertex);

        BOOST_AUTO_TEST_CASE(variable_vertex) {
            // precondition: variable vertex doesn't exist
            BOOST_TEST_REQUIRE(g.contains_vertex("vv") == false);

            // setup: add the vertex
            g.add_variable("vv", {0, 1, 2});

            // test: check if the variable is contained now
            BOOST_TEST_REQUIRE(g.contains_vertex("vv") == true);
        }

        BOOST_AUTO_TEST_CASE(constraint_vertex) {
            // precondition: variable vertex doesn't exist
            BOOST_TEST_REQUIRE(g.contains_vertex("cv") == false);

            // setup: add the vertex
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(1, 1));

            // test: check if the variable is contained now
            BOOST_TEST_REQUIRE(g.contains_vertex("cv") == true);
        }

    BOOST_AUTO_TEST_SUITE_END();

    // return if vertices of given names are adjacent
    // bool adjacent(std::string name1, std::string name2);    
    BOOST_AUTO_TEST_SUITE(adjacent);

        BOOST_AUTO_TEST_CASE(both_given_name_exist) {
            // pre-setup: add vertices
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(0, 0));
            g.add_variable("vv", {0, 1, 2});

            // precondition: check they aren't adjacent by default
            BOOST_TEST_REQUIRE(g.adjacent("cv", "vv") == false);
            BOOST_TEST_REQUIRE(g.adjacent("vv", "cv") == false);
            
            // setup: add edge
            g.add_edge("vv", "cv");

            // test: check they are adjacent now
            BOOST_TEST(g.adjacent("cv", "vv") == true);
            BOOST_TEST(g.adjacent("vv", "cv") == true);
        }

        BOOST_AUTO_TEST_CASE(given_cv_non_existent) {
            // setup: add one of the vertex
            g.add_variable("vv", {0, 1, 2});

            // test: check they aren't adjacent since one doesn't exist
            BOOST_TEST(g.adjacent("cv", "vv") == false);
            BOOST_TEST(g.adjacent("vv", "cv") == false);
        }

        BOOST_AUTO_TEST_CASE(given_vv_non_existent) {
            // setup: add one of the vertex
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(0, 0));

            // test: check they aren't adjacent since one doesn't exist
            BOOST_TEST(g.adjacent("cv", "vv") == false);
            BOOST_TEST(g.adjacent("vv", "cv") == false);
        }

        BOOST_AUTO_TEST_CASE(both_names_non_existent) {
            // test: check given named vertices aren't adjacent (since they don't exist)
            BOOST_TEST(g.adjacent("cv", "vv") == false);
            BOOST_TEST(g.adjacent("vv", "cv") == false);
        }

    BOOST_AUTO_TEST_SUITE_END();

    // return all constraint neighbors of a variable vertex with given name
    // std::vector<GraphImplementation::ConstraintVertex*> get_constraint_neighbors(std::string name);
    BOOST_AUTO_TEST_SUITE(get_constraint_neighbors);

        BOOST_AUTO_TEST_CASE(given_name_is_not_in_graph) {
            // setup: add some vertices and edges
            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 0));
            g.add_constraint("cv2", GraphImplementation::ConstraintVertex::lesserOrEqualToN(0, 0));
            g.add_variable("vv1", {0, 1, 2});
            g.add_variable("vv2", {2});
            // vv1 and vv2 are linked by cv1; vv2 is linked to cv2
            g.add_edge("vv1", "cv1");
            g.add_edge("vv2", "cv1");
            g.add_edge("vv2", "cv2");

            // test: check that neighbors of a non-existent variable vertex is an empty vector
            auto actual_vector = g.get_constraint_neighbors("non-existent");
            auto expected_vector = std::vector<ConstraintVertex*>();

            BOOST_CHECK_EQUAL_COLLECTIONS(actual_vector.begin(), actual_vector.end(), expected_vector.begin(), expected_vector.begin());
        }

        BOOST_AUTO_TEST_CASE(given_name_has_no_neighbors) {
            // setup: add some vertices and edges
            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 0));
            g.add_variable("vv1", {0, 1, 2});
            
            // test: vertex have no neighbors
            auto actual_vector = g.get_constraint_neighbors("vv1");
            auto expected_vector = std::vector<ConstraintVertex*>();

            BOOST_CHECK_EQUAL_COLLECTIONS(actual_vector.begin(), actual_vector.end(), expected_vector.begin(), expected_vector.begin());
        }

        BOOST_AUTO_TEST_CASE(given_name_has_one_neighbor) {
            // pre-setup: add some vertices
            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_variable("vv1", {0, 1, 2});
            
            // precondition: variable vertex has no neighbors
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv1").size(), 0);
            
            // setup: add some edges
            g.add_edge("vv1", "cv1");
            
            // test: check that variable vertex has one correct neighbor now
            auto actual_vector = g.get_constraint_neighbors("vv1");
            
            // guard against empty vertices being returned
            BOOST_TEST_REQUIRE(actual_vector.size() != 0);
            
            auto actual_neighbor = actual_vector[0];
            // we first check the size of neighbor 
            BOOST_CHECK_EQUAL(actual_vector.size(), 1);
            // we also check that this neighbor is the expected one
            // first, guard against null pointers
            BOOST_TEST_REQUIRE(g.get_constraint("cv1") != nullptr);
            
            BOOST_CHECK_EQUAL(actual_neighbor, g.get_constraint("cv1"));
        }

        BOOST_AUTO_TEST_CASE(given_name_has_two_neighbors) {
            // pre-setup: add some vertices
            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_constraint("cv2", GraphImplementation::ConstraintVertex::exactlyN(99, 1));
            g.add_variable("vv1", {1, 99});
            
            // precondition: variable vertex has no neighbors
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv1").size(), 0);
            
            // setup: add some edges
            g.add_edge("vv1", "cv1");
            g.add_edge("vv1", "cv2");
            
            // test: check that variable vertex has two correct neighbors now
            auto actual_vector = g.get_constraint_neighbors("vv1");
            auto checked_ptr = std::vector<ConstraintVertex*> {
                g.get_constraint("cv1"), g.get_constraint("cv2")
                };
            // we first check the size of neighbor 
            BOOST_CHECK_EQUAL(actual_vector.size(), 2);
            // we then check if the vector holds vertices with the right pointers
            for (auto cv_ptr : checked_ptr)
            {
                BOOST_CHECK(
                    (
                        std::find(actual_vector.begin(), actual_vector.end(), cv_ptr) 
                        != actual_vector.end()
                    )
                == true);
            }
        }

    BOOST_AUTO_TEST_SUITE_END();

    // return all variable neighbors of a constraint vertex with given name
    // std::vector<GraphImplementation::VariableVertex*> get_variable_neighbors(std::string name);
    BOOST_AUTO_TEST_SUITE(get_variable_neighbors);

        BOOST_AUTO_TEST_CASE(given_name_is_not_in_graph) {
            // setup: add some vertices and edges
            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 0));
            g.add_constraint("cv2", GraphImplementation::ConstraintVertex::lesserOrEqualToN(0, 0));
            g.add_variable("vv1", {0, 1, 2});
            g.add_variable("vv2", {2});
            // vv1 and vv2 are linked by cv1; vv2 is linked to cv2
            g.add_edge("vv1", "cv1");
            g.add_edge("vv2", "cv1");
            g.add_edge("vv2", "cv2");

            // test: check that neighbors of a non-existent variable vertex is an empty vector
            auto actual_vector = g.get_variable_neighbors("non-existent");
            auto expected_vector = std::vector<VariableVertex*>();

            BOOST_CHECK_EQUAL_COLLECTIONS(actual_vector.begin(), actual_vector.end(), expected_vector.begin(), expected_vector.begin());
        }

        BOOST_AUTO_TEST_CASE(given_name_has_no_neighbors) {
            // setup: add some vertices and edges
            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 0));
            g.add_variable("vv1", {0, 1, 2});
            
            // test: vertex have no neighbors
            auto actual_vector = g.get_variable_neighbors("cv1");
            auto expected_vector = std::vector<VariableVertex*>();

            BOOST_CHECK_EQUAL_COLLECTIONS(actual_vector.begin(), actual_vector.end(), expected_vector.begin(), expected_vector.begin());
        }

        BOOST_AUTO_TEST_CASE(given_name_has_one_neighbor) {
            // pre-setup: add some vertices
            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_variable("vv1", {0, 1, 2});
            
            // precondition: constraint vertex has no neighbors
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv1").size(), 0);
            
            // setup: add some edges
            g.add_edge("vv1", "cv1");
            
            // test: check that constraint vertex has one correct neighbor now
            auto actual_vector = g.get_variable_neighbors("cv1");

            // guard against empty vertices being returned
            BOOST_TEST_REQUIRE(actual_vector.size() != 0);

            auto actual_neighbor = actual_vector[0];
            // we first check the size of neighbor
            BOOST_CHECK_EQUAL(actual_vector.size(), 1);
            // we also check that this neighbor is the expected one
            // first, guard against null pointers
            BOOST_TEST_REQUIRE(g.get_variable("vv1") != nullptr);
            
            BOOST_CHECK_EQUAL(actual_neighbor, g.get_variable("vv1"));
        }

        BOOST_AUTO_TEST_CASE(given_name_has_two_neighbors) {
            // pre-setup: add some vertices
            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_variable("vv1", {1, 99});
            g.add_variable("vv2", {0, 1, 2});
            
            // precondition: constraint vertex has no neighbors
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv1").size(), 0);
            
            // setup: add some edges
            g.add_edge("vv1", "cv1");
            g.add_edge("vv2", "cv1");

            // test: check that constraint vertex has two correct neighbors now
            auto actual_vector = g.get_variable_neighbors("cv1");
            auto checked_ptr = std::vector<VariableVertex*> {
                g.get_variable("vv1"), g.get_variable("vv2")
                };
            // we first check the size of neighbor 
            BOOST_CHECK_EQUAL(actual_vector.size(), 2);
            // we then check if the vector holds vertices with the right pointers
            for (auto vv_ptr : checked_ptr)
            {
                BOOST_CHECK(
                    (
                        std::find(actual_vector.begin(), actual_vector.end(), vv_ptr) 
                        != actual_vector.end()
                    )
                == true);
            }            
        }

    BOOST_AUTO_TEST_SUITE_END();


    // adds a constraint vertex with given name and predicate to the graph
    // void add_constraint(std::string name, std::function<bool(int, std::initializer_list<GraphImplementation::VariableVertex>)> pred);
    // * Inserting vertex with existing name doesn't do anything, even when the older one was a VariableVertex
    BOOST_AUTO_TEST_SUITE(add_constraint);

        BOOST_AUTO_TEST_CASE(add_normally) {
            // precondition: constraint isn't there yet
            BOOST_TEST_REQUIRE(g.get_constraint("cv") == nullptr);

            // setup: add a constraint
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            
            // test: check that the constraint is indeed in the graph now
            auto obtained_constraint = g.get_constraint("cv");
            // guard against null pointers
            BOOST_TEST_REQUIRE(obtained_constraint != nullptr);
            // first check that added vertex has the right name
            BOOST_CHECK_EQUAL(obtained_constraint->getName(), "cv");
            // also check if it has the correct constraint
            // this should return true, since there is one 0 in given domains
            BOOST_TEST(obtained_constraint->constraintIsMet(0, {}) == true);
            // this should return false, since there cannot be any 1 in given domains
            BOOST_TEST(obtained_constraint->constraintIsMet(1, {}) == false);
            // this should return false, since there cannot be any 1 in given domains
            BOOST_TEST(obtained_constraint->constraintIsMet(2, {}) == false);
        }

        BOOST_AUTO_TEST_CASE(add_with_name_equal_to_other_constraint) {
            // pre-setup: add a constraint
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::greaterOrEqualToN(0, 1));

            // precondition: some constraint is already there
            auto existing_constraint = g.get_constraint("cv");
            // guard against null pointers
            BOOST_TEST_REQUIRE(existing_constraint != nullptr);
            // check vertex name
            BOOST_REQUIRE_EQUAL(existing_constraint->getName(), "cv");
            // also check if it has the correct constraint
            // this should return true, since there is one 0 in given domains
            BOOST_TEST_REQUIRE(existing_constraint->constraintIsMet(0, {}) == true);
            // this should return false, since there aren't any 1 in given domains
            BOOST_TEST_REQUIRE(existing_constraint->constraintIsMet(1, {}) == false);
            // this should return false, since there aren't any 1 in given domains
            BOOST_TEST_REQUIRE(existing_constraint->constraintIsMet(2, {}) == false);

            // setup: add a new constraint with different predicate but the same name
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(2, 1));

            // test: check that the old constraint was preserved, and the new one not inserted
            auto new_constraint = g.get_constraint("cv");
            // guard against null pointers
            BOOST_TEST_REQUIRE(new_constraint != nullptr);
            // check vertex name
            BOOST_CHECK_EQUAL(new_constraint->getName(), "cv");
            // also check if it has the older constraint
            // this should return true, since there is one 0 in given domains
            BOOST_TEST(new_constraint->constraintIsMet(0, {}) == true);
            // this should return false, since there aren't any 1 in given domains
            BOOST_TEST(new_constraint->constraintIsMet(1, {}) == false);
            // this should return false, since there aren't any 1 in given domains
            BOOST_TEST(new_constraint->constraintIsMet(2, {}) == false);
        }

        BOOST_AUTO_TEST_CASE(add_with_name_equal_to_other_variable) {
            // pre-setup: add a variable
            g.add_variable("cv", {});

            // precondition: check there is no constraint with the given name
            BOOST_TEST_REQUIRE(g.get_constraint("cv") == nullptr);
            BOOST_TEST_REQUIRE(g.get_variable("cv") != nullptr);

            // setup: add a new constraint with the same name
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(2, 1));

            // test: check that to avoid name collision, the new constraint wasn't inserted
            //  while the old variable was preserved
            BOOST_TEST_REQUIRE(g.get_constraint("cv") == nullptr);
            BOOST_TEST_REQUIRE(g.get_variable("cv") != nullptr);
        }

    BOOST_AUTO_TEST_SUITE_END();

    
    // adds a variable vertex with given name and domain to the graph
    // * Inserting vertex with existing name doesn't do anything,
    //   even when the older one was a ConstraintVertex
    // void add_variable(std::string name, std::initializer_list<int> domain);
    BOOST_AUTO_TEST_SUITE(add_variable);

        BOOST_AUTO_TEST_CASE(add_normally) {
            std::string name = "vv";
            auto vv_domain = {0, 1};
            // precondition: variable isn't there yet
            BOOST_TEST_REQUIRE(g.get_variable(name) == nullptr);

            // setup: add a variable
            g.add_variable(name, vv_domain);
            
            // test: check that the variable is indeed in the graph now
            auto obtained_variable = g.get_variable(name);
            // guard against null pointers
            BOOST_TEST_REQUIRE(obtained_variable != nullptr);
            // first check that added vertex has the right name
            BOOST_CHECK_EQUAL(obtained_variable->getName(), name);
            // also check if it has the correct domain
            auto actual_domain = obtained_variable->getDomain();
            auto expected_domain = std::set<int>(vv_domain);

            BOOST_CHECK_EQUAL_COLLECTIONS(actual_domain.begin(), actual_domain.end(), 
                                          expected_domain.begin(), expected_domain.end());
        }

        BOOST_AUTO_TEST_CASE(add_with_name_equal_to_other_variable) {
            std::string name = "vv";
            auto vv_domain = {0, 1};
            // pre-setup: add a variable
            g.add_variable(name, vv_domain);

            // precondition: some variable is already there
            auto existing_variable = g.get_variable(name);
            // guard against null pointers
            BOOST_TEST_REQUIRE(existing_variable != nullptr);
            // check vertex name
            BOOST_REQUIRE_EQUAL(existing_variable->getName(), name);
            // also check if it has the correct domain
            auto actual_domain = existing_variable->getDomain();
            auto expected_domain = std::set<int>(vv_domain);

            BOOST_CHECK_EQUAL_COLLECTIONS(actual_domain.begin(), actual_domain.end(), 
                                          expected_domain.begin(), expected_domain.end());

            // setup: add a new variable with different domain but the same name
            g.add_variable(name, {99, 100});

            // test: check that the old variable was preserved, and the new one not inserted
            auto new_variable = g.get_variable(name);
            // guard against null pointers
            BOOST_TEST_REQUIRE(new_variable != nullptr);
            // check vertex name
            BOOST_CHECK_EQUAL(new_variable->getName(), name);
            // also check if it has the older domain
            actual_domain = new_variable->getDomain();
            expected_domain = std::set<int>(vv_domain);
            BOOST_CHECK_EQUAL_COLLECTIONS(actual_domain.begin(), actual_domain.end(), 
                                          expected_domain.begin(), expected_domain.end());
        }

        BOOST_AUTO_TEST_CASE(add_with_name_equal_to_other_constraint) {
            std::string name = "vv";
            // pre-setup: add a constraint
            g.add_constraint(name, GraphImplementation::ConstraintVertex::exactlyN(0, 1));

            // precondition: check there is no variable with the given name
            BOOST_TEST_REQUIRE(g.get_variable(name) == nullptr);
            BOOST_TEST_REQUIRE(g.get_constraint(name) != nullptr);

            // setup: add a new variable with the same name
            g.add_variable(name, {0, 1, 2});

            // test: check that to avoid name collision, the new variable wasn't inserted
            //  while the old constraint was preserved
            BOOST_TEST_REQUIRE(g.get_variable(name) == nullptr);
            BOOST_TEST_REQUIRE(g.get_constraint(name) != nullptr);
        }

    BOOST_AUTO_TEST_SUITE_END();

    // removes vertex with given name if there, as well as edges connected to it
    // void remove_vertex(std::string name);    
    BOOST_AUTO_TEST_SUITE(remove_vertex);

        BOOST_AUTO_TEST_CASE(remove_non_existent_vertex) {
            // pre-setup: add vertices and edges
            g.add_variable("vv1", {0, 1, 2});
            g.add_variable("vv2", {10, 11, 12});

            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_constraint("cv2", GraphImplementation::ConstraintVertex::greaterOrEqualToN(0, 2));

            g.add_edge("vv1", "cv1");
            g.add_edge("vv2", "cv1");
            g.add_edge("vv2", "cv2");

            // precondition: check that what was added indeed exists
            BOOST_TEST_REQUIRE(g.get_variable("vv1") != nullptr);
            BOOST_TEST_REQUIRE(g.get_variable("vv2") != nullptr);

            BOOST_TEST_REQUIRE(g.get_constraint("cv1") != nullptr);
            BOOST_TEST_REQUIRE(g.get_constraint("cv2") != nullptr);

            BOOST_TEST_REQUIRE(g.adjacent("vv1", "cv1") == true);
            BOOST_TEST_REQUIRE(g.adjacent("vv1", "cv2") == false);
            BOOST_TEST_REQUIRE(g.adjacent("vv2", "cv1") == true);
            BOOST_TEST_REQUIRE(g.adjacent("vv2", "cv2") == true);
            // test about the internal storage as well
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv1").size(), 1);
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv2").size(), 2);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv1").size(), 2);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv2").size(), 1);
            
            // setup: remove a vertex that doesn't exist
            g.remove_vertex("non-existent");

            // test: check that what existed still exists
            BOOST_TEST(g.get_variable("vv1") != nullptr);
            BOOST_TEST(g.get_variable("vv2") != nullptr);

            BOOST_TEST(g.get_constraint("cv1") != nullptr);
            BOOST_TEST(g.get_constraint("cv2") != nullptr);

            BOOST_TEST(g.adjacent("vv1", "cv1") == true);
            BOOST_TEST(g.adjacent("vv1", "cv2") == false);
            BOOST_TEST(g.adjacent("vv2", "cv1") == true);
            BOOST_TEST(g.adjacent("vv2", "cv2") == true);
            // test about the internal storage as well
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv1").size(), 1);
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv2").size(), 2);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv1").size(), 2);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv2").size(), 1);
        }

        BOOST_AUTO_TEST_CASE(remove_existent_Variable_vertex) {
            // pre-setup: add vertices and edges
            g.add_variable("vv1", {0, 1, 2});
            g.add_variable("vv2", {10, 11, 12});

            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_constraint("cv2", GraphImplementation::ConstraintVertex::greaterOrEqualToN(0, 2));

            g.add_edge("vv1", "cv1");
            g.add_edge("vv2", "cv1");
            g.add_edge("vv2", "cv2");

            // precondition: check that what was added indeed exists
            BOOST_TEST_REQUIRE(g.get_variable("vv1") != nullptr);
            BOOST_TEST_REQUIRE(g.get_variable("vv2") != nullptr);

            BOOST_TEST_REQUIRE(g.get_constraint("cv1") != nullptr);
            BOOST_TEST_REQUIRE(g.get_constraint("cv2") != nullptr);

            BOOST_TEST_REQUIRE(g.adjacent("vv1", "cv1") == true);
            BOOST_TEST_REQUIRE(g.adjacent("vv1", "cv2") == false);
            BOOST_TEST_REQUIRE(g.adjacent("vv2", "cv1") == true);
            BOOST_TEST_REQUIRE(g.adjacent("vv2", "cv2") == true);
            // test about the internal storage as well
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv1").size(), 1);
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv2").size(), 2);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv1").size(), 2);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv2").size(), 1);
            
            // setup: remove a vertex that exists
            g.remove_vertex("vv1");

            // test: check that it has been removed, as well as the appropriate edges
            BOOST_TEST(g.get_variable("vv1") == nullptr);
            BOOST_TEST(g.get_variable("vv2") != nullptr);

            BOOST_TEST(g.get_constraint("cv1") != nullptr);
            BOOST_TEST(g.get_constraint("cv2") != nullptr);

            BOOST_TEST(g.adjacent("vv2", "cv1") == true);
            BOOST_TEST(g.adjacent("vv2", "cv2") == true);
            // test about the internal storage as well
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv1").size(), 0);
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv2").size(), 2);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv1").size(), 1);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv2").size(), 1);
        }

        BOOST_AUTO_TEST_CASE(remove_existent_constraint_vertex) {
            // pre-setup: add vertices and edges
            g.add_variable("vv1", {0, 1, 2});
            g.add_variable("vv2", {10, 11, 12});

            g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_constraint("cv2", GraphImplementation::ConstraintVertex::greaterOrEqualToN(0, 2));

            g.add_edge("vv1", "cv1");
            g.add_edge("vv2", "cv1");
            g.add_edge("vv2", "cv2");

            // precondition: check that what was added indeed exists
            BOOST_TEST_REQUIRE(g.get_variable("vv1") != nullptr);
            BOOST_TEST_REQUIRE(g.get_variable("vv2") != nullptr);

            BOOST_TEST_REQUIRE(g.get_constraint("cv1") != nullptr);
            BOOST_TEST_REQUIRE(g.get_constraint("cv2") != nullptr);

            BOOST_TEST_REQUIRE(g.adjacent("vv1", "cv1") == true);
            BOOST_TEST_REQUIRE(g.adjacent("vv1", "cv2") == false);
            BOOST_TEST_REQUIRE(g.adjacent("vv2", "cv1") == true);
            BOOST_TEST_REQUIRE(g.adjacent("vv2", "cv2") == true);
            // test about the internal storage as well
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv1").size(), 1);
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv2").size(), 2);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv1").size(), 2);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv2").size(), 1);
            
            // setup: remove a vertex that exists
            g.remove_vertex("cv1");

            // test: check that it has been removed, as well as the appropriate edges
            BOOST_TEST(g.get_variable("vv1") != nullptr);
            BOOST_TEST(g.get_variable("vv2") != nullptr);

            BOOST_TEST(g.get_constraint("cv1") == nullptr);
            BOOST_TEST(g.get_constraint("cv2") != nullptr);

            BOOST_TEST(g.adjacent("vv1", "cv2") == false);
            BOOST_TEST(g.adjacent("vv2", "cv2") == true);
            // test about the internal storage as well
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv1").size(), 0);
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv2").size(), 1);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv1").size(), 0);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv2").size(), 1);
        }

    BOOST_AUTO_TEST_SUITE_END();

    // adds edge between given variable vertex and constraint vertex identified by name, if not there
    // void add_edge(std::string vv_name, std::string cv_name);  
    BOOST_AUTO_TEST_SUITE(add_edge);

        BOOST_AUTO_TEST_CASE(both_ends_exist_but_are_not_adjacent) {
            // pre-setup: add some vertices
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_variable("vv", {0, 1});

            // precondition: check the two aren't adjacent
            BOOST_TEST_REQUIRE(g.adjacent("cv", "vv") == false);
            // also test neighbors
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv").size(), 0);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv").size(), 0);
            
            // setup: add an edge between the vertices
            g.add_edge("vv", "cv");

            // test: check that they are adjacent now
            BOOST_TEST(g.adjacent("cv", "vv") == true);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 1);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 1);
        }

        BOOST_AUTO_TEST_CASE(both_ends_exist_and_are_adjacent) {
            // pre-setup: add some vertices and an edge between them
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_variable("vv", {0, 1});

            g.add_edge("vv", "cv");

            // precondition: check vertices are adjacent
            BOOST_TEST_REQUIRE(g.adjacent("cv", "vv") == true);
            // also test neighbors
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv").size(), 1);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv").size(), 1);

            // setup: add the edge one more time
            g.add_edge("vv", "cv");

            // test: check the vertices are still adjacent
            BOOST_TEST(g.adjacent("cv", "vv") == true);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 1);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 1);
        }

        BOOST_AUTO_TEST_CASE(one_end_or_both_do_not_exist) {
            // pre-setup: add some vertices
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_variable("vv", {0, 1});

            // precondition: check none of the combinations are adjacent
            BOOST_TEST_REQUIRE(g.adjacent("cv", "vv") == false);
            // also test neighbors
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv").size(), 0);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv").size(), 0);
            
            // setup 1: add an edge where the constraint vertex isn't in the graph
            g.add_edge("vv", "non-existent_cv");

            // test 1: check there are no edges or vertices added to graph
            BOOST_TEST(g.get_constraint("non-existent_cv") == nullptr);
            BOOST_TEST(g.adjacent("cv", "vv") == false);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 0);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 0);

            // setup 2: add an edge where the variable vertex isn't in the graph
            g.add_edge("non-existent_vv", "cv");

            // test 2: check there are no edges or vertices added to graph
            BOOST_TEST(g.get_variable("non-existent_vv") == nullptr);
            BOOST_TEST(g.adjacent("cv", "vv") == false);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 0);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 0);

            // setup 3: add an edge where both ends aren't in the graph
            g.add_edge("non-existent_vv", "non-existent_cv");

            // test 3: check there are no edges or vertices added to graph
            BOOST_TEST(g.get_variable("non-existent_vv") == nullptr);
            BOOST_TEST(g.get_constraint("non-existent_cv") == nullptr);
            BOOST_TEST(g.adjacent("cv", "vv") == false);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 0);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 0);
        }

    BOOST_AUTO_TEST_SUITE_END();

    // removes edge between given variable vertex and constraint vertex identified by name, if there
    // void remove_edge(std::string vv_name, std::string cv_name);
    BOOST_AUTO_TEST_SUITE(remove_edge);

        BOOST_AUTO_TEST_CASE(both_ends_exist_and_are_adjacent) {
            // pre-setup: add some vertices and edge between them
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_variable("vv", {0, 1});

            g.add_edge("vv", "cv");

            // precondition: check vertices are adjacent
            BOOST_TEST_REQUIRE(g.adjacent("cv", "vv") == true);
            // also test neighbors
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv").size(), 1);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv").size(), 1);
            
            // setup: remove edge
            g.remove_edge("vv", "cv");

            // test: check the vertices are no longer adjacent to each other
            BOOST_TEST(g.adjacent("cv", "vv") == false);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 0);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 0);

        }

        BOOST_AUTO_TEST_CASE(both_ends_exist_but_are_not_adjacent) {
            // pre-setup: add some vertices
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_variable("vv", {0, 1});

            // precondition: check vertices aren't adjacent
            BOOST_TEST_REQUIRE(g.adjacent("cv", "vv") == false);
            // also test neighbors
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv").size(), 0);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv").size(), 0);
            
            // setup: remove edge
            g.remove_edge("vv", "cv");

            // test: check the vertices are still not adjacent to each other
            BOOST_TEST(g.adjacent("cv", "vv") == false);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 0);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 0);
        }

        BOOST_AUTO_TEST_CASE(one_end_or_both_do_not_exist) {
            // pre-setup: add some vertices and an edge between them
            g.add_constraint("cv", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
            g.add_variable("vv", {0, 1});

            g.add_edge("vv", "cv");

            // precondition: vertices are adjacent to each other
            BOOST_TEST_REQUIRE(g.adjacent("cv", "vv") == true);
            // also test neighbors
            BOOST_REQUIRE_EQUAL(g.get_constraint_neighbors("vv").size(), 1);
            BOOST_REQUIRE_EQUAL(g.get_variable_neighbors("cv").size(), 1);

            // setup 1: remove an edge where the constraint vertex isn't in the graph
            g.remove_edge("vv", "non-existent_cv");
            
            // test 1: check there are no edges or vertices removed from graph
            BOOST_TEST(g.get_constraint("non-existent_cv") == nullptr);
            BOOST_TEST(g.adjacent("cv", "vv") == true);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 1);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 1);

            // setup 2: remove an edge where the variable vertex isn't in the graph
            g.add_edge("non-existent_vv", "cv");

            // test 2: check there are no edges or vertices removed from graph
            BOOST_TEST(g.get_variable("non-existent_vv") == nullptr);
            BOOST_TEST(g.adjacent("cv", "vv") == true);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 1);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 1);

            // setup 3: remove an edge where both ends aren't in the graph
            g.add_edge("non-existent_vv", "non-existent_cv");

            // test 3: check there are no edges or vertices removed from graph
            BOOST_TEST(g.get_variable("non-existent_vv") == nullptr);
            BOOST_TEST(g.get_constraint("non-existent_cv") == nullptr);
            BOOST_TEST(g.adjacent("cv", "vv") == true);
            // also test neighbors
            BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv").size(), 1);
            BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv").size(), 1);
        }

    BOOST_AUTO_TEST_SUITE_END();

    // attempt to emulate real world possible use cases to catch potential further errors
    BOOST_AUTO_TEST_CASE(real_world_use_case) {
        /*
        We proceed as follows:
        1) Create many variables and constraint vertices:
          - vv1, vv2, vv3, vv4, vv5, rmvv1, rmvv2 | cv1, cv2, cv3, cv4, rmcv1, rmcv2, rmcv3
        2) Add some edges
          - vv1: cv1, cv2, cv3
          - vv2: cv1, cv4
          - vv3: cv1, cv2, rmcv1, rmcv2
          - vv4: cv1, rmcv1, rmcv2
          - vv5: cv1, cv4, rmcv2
          - rmvv1: cv1, rmcv2
          - rmvv2: cv1, cv2, cv3, cv4, rmcv1, rmcv2
        3) Remove some variables and constraints
          - remove rmvv1, rmcv2, rmcv3, rmcv1, rmvv2
        4) Remove some edges
          - remove vv1-cv1, vv3-cv2, vv5-cv4
        */
        // first, add everything
        g.add_variable("vv1", {0, 1, 2});
        g.add_constraint("cv1", GraphImplementation::ConstraintVertex::exactlyN(0, 1));
        g.add_variable("vv2", {1, 2});
        g.add_constraint("cv2", GraphImplementation::ConstraintVertex::greaterOrEqualToN(0, 1));
        g.add_variable("vv3", {2});
        g.add_constraint("cv3", GraphImplementation::ConstraintVertex::lesserOrEqualToN(0, 1));
        g.add_variable("vv4", {0, 1, 2});
        g.add_constraint("cv4", GraphImplementation::ConstraintVertex::exactlyN(0, 99));
        g.add_variable("vv5", {99});
        g.add_constraint("rmcv1", GraphImplementation::ConstraintVertex::exactlyN(21, 1));
        g.add_variable("rmvv1", {100, 1, 2});
        g.add_constraint("rmcv2", GraphImplementation::ConstraintVertex::lesserOrEqualToN(0, 20));
        g.add_variable("rmvv2", {0, 21, 2});
        g.add_constraint("rmcv3", GraphImplementation::ConstraintVertex::greaterOrEqualToN(11, 3));

        // check if we have everything we want
        BOOST_TEST(g.get_variable("vv1") != nullptr);
        BOOST_TEST(g.get_variable("vv2") != nullptr);
        BOOST_TEST(g.get_variable("vv3") != nullptr);
        BOOST_TEST(g.get_variable("vv4") != nullptr);
        BOOST_TEST(g.get_variable("vv5") != nullptr);
        BOOST_TEST(g.get_variable("rmvv1") != nullptr);
        BOOST_TEST(g.get_variable("rmvv2") != nullptr);
        // the following have the right name but wrong function call (no such constraints)
        BOOST_TEST(g.get_constraint("vv1") == nullptr);
        BOOST_TEST(g.get_constraint("vv2") == nullptr);
        BOOST_TEST(g.get_constraint("vv3") == nullptr);
        BOOST_TEST(g.get_constraint("vv4") == nullptr);
        BOOST_TEST(g.get_constraint("vv5") == nullptr);
        BOOST_TEST(g.get_constraint("rmvv1") == nullptr);
        BOOST_TEST(g.get_constraint("rmvv2") == nullptr);
        
        BOOST_TEST(g.get_constraint("cv1") != nullptr);
        BOOST_TEST(g.get_constraint("cv2") != nullptr);
        BOOST_TEST(g.get_constraint("cv3") != nullptr);
        BOOST_TEST(g.get_constraint("cv4") != nullptr);
        BOOST_TEST(g.get_constraint("rmcv1") != nullptr);
        BOOST_TEST(g.get_constraint("rmcv2") != nullptr);
        BOOST_TEST(g.get_constraint("rmcv3") != nullptr);
        // again, right name but wrong category (no such variables)
        BOOST_TEST(g.get_variable("cv1") == nullptr);
        BOOST_TEST(g.get_variable("cv2") == nullptr);
        BOOST_TEST(g.get_variable("cv3") == nullptr);
        BOOST_TEST(g.get_variable("cv4") == nullptr);
        BOOST_TEST(g.get_variable("rmcv1") == nullptr);
        BOOST_TEST(g.get_variable("rmcv2") == nullptr);
        BOOST_TEST(g.get_variable("rmcv3") == nullptr);

        // none have their neighbors
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv1").size(), 0);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv2").size(), 0);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv3").size(), 0);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv4").size(), 0);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv5").size(), 0);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("rmvv1").size(), 0);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("rmvv2").size(), 0);
        
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv1").size(), 0);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv2").size(), 0);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv3").size(), 0);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv4").size(), 0);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("rmcv1").size(), 0);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("rmcv2").size(), 0);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("rmcv3").size(), 0);

        // we then add edges
        // - vv1: cv1, cv2, cv3
        for (auto n : {"cv1", "cv2", "cv3"}) g.add_edge("vv1", n);
        // - vv2: cv1, cv4
        for (auto n : {"cv1", "cv4"}) g.add_edge("vv2", n);
        // - vv3: cv1, cv2, rmcv1, rmcv2
        for (auto n : {"cv1", "cv2", "rmcv1", "rmcv2"}) g.add_edge("vv3", n);
        // - vv4: cv1, rmcv1, rmcv2
        for (auto n : {"cv1", "rmcv1", "rmcv2"}) g.add_edge("vv4", n);
        // - vv5: cv1, cv4, rmcv2
        for (auto n : {"cv1", "cv4", "rmcv2"}) g.add_edge("vv5", n);
        // - rmvv1: cv1, rmcv2
        for (auto n : {"cv1", "rmcv2"}) g.add_edge("rmvv1", n);
        // - rmvv2: cv1, cv2, cv3, cv4, rmcv1, rmcv2
        for (auto n : {"cv1", "cv2", "cv3", "cv4", "rmcv1", "rmcv2"}) g.add_edge("rmvv2", n);

        // then check the number of neighbors
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv1").size(), 3);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv2").size(), 2);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv3").size(), 4);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv4").size(), 3);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv5").size(), 3);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("rmvv1").size(), 2);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("rmvv2").size(), 6);
        
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv1").size(), 7);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv2").size(), 3);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv3").size(), 2);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv4").size(), 3);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("rmcv1").size(), 3);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("rmcv2").size(), 5);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("rmcv3").size(), 0);

        // finally, remove variables and constraints
        // - remove rmvv1, rmcv2, rmcv3, rmcv1, rmvv2
        for (auto elem : {"rmvv1", "rmcv2", "rmcv3", "rmcv1", "rmvv2"}) g.remove_vertex(elem);

        // then check the number of neighbors again
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv1").size(), 3);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv2").size(), 2);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv3").size(), 2);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv4").size(), 1);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv5").size(), 2);
        
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv1").size(), 5);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv2").size(), 2);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv3").size(), 1);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv4").size(), 2);
        
        // finally remove some edges
        //  - remove vv1-cv1, vv3-cv2, vv5-cv4
        g.remove_edge("vv1", "cv1");
        g.remove_edge("vv3", "cv2");
        g.remove_edge("vv5", "cv4");

        // finally check the number of neighbors once last time
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv1").size(), 2);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv2").size(), 2);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv3").size(), 1);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv4").size(), 1);
        BOOST_CHECK_EQUAL(g.get_constraint_neighbors("vv5").size(), 1);
        
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv1").size(), 4);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv2").size(), 1);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv3").size(), 1);
        BOOST_CHECK_EQUAL(g.get_variable_neighbors("cv4").size(), 1);
    }

BOOST_AUTO_TEST_SUITE_END();