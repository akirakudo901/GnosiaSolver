// Author: Akira Kudo
// Description: Implements tests for the Graph class in GraphImplementation.

#define BOOST_TEST_MODULE Graph Implementation Testcases
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <string>
#include <vector>

#include "src/graphImplementation/edges/Edge.h"
#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"
#include "src/graphImplementation/vertices/Vertex.h"

using namespace GraphImplementation;

// defining fixture
struct TestGraph_Fixture {

    VariableVertex vv1 = VariableVertex("", {});
    VariableVertex vv2 = VariableVertex("", {});
    VariableVertex vv3 = VariableVertex("", {});
    ConstraintVertex cv1 = ConstraintVertex("",
        [] (auto mainVal, auto varList) { return false; }
        );
    Graph g;
    Edge e;

    TestGraph_Fixture() 
    {
        auto initDomain = {0, 1, 2};
    
        g = Graph();
        vv1 = VariableVertex("vv1", initDomain);
        vv2 = VariableVertex("vv2", initDomain);
        vv3 = VariableVertex("vv3", initDomain);
        cv1 = ConstraintVertex( //cv1's constraint itself isn't part of the test
            "",
            [] (int mainVar, std::vector<VariableVertex*> varList) { return false; }
            );
        e = Edge();    
    };
    
    ~TestGraph_Fixture() {};
};

BOOST_FIXTURE_TEST_SUITE(Graph_test_suite, TestGraph_Fixture, * boost::unit_test::label("Graph"));

    // function returns whether vertex x is in the graph
    // bool contains_vertex(Vertex *x)
    BOOST_AUTO_TEST_SUITE(contains_vertex);
        
        BOOST_AUTO_TEST_CASE(pass_normal) {
            // initial check: check vertices aren't in graph
            BOOST_TEST_REQUIRE(g.contains_vertex(vv1) == false);
            BOOST_TEST_REQUIRE(g.contains_vertex(vv2) == false);
            BOOST_TEST_REQUIRE(g.contains_vertex(cv1) == false);
            
            // setup: add vertex to graph
            g.add_vertex(vv1);
            g.add_vertex(cv1);
            
            // test:
            // these should be included
            BOOST_TEST(g.contains_vertex(vv1) == true);
            BOOST_TEST(g.contains_vertex(cv1) == true);
            // this shouldn't be included
            BOOST_TEST(g.contains_vertex(vv2) == false);
        };
    
    BOOST_AUTO_TEST_SUITE_END();

    // returns if vertices x and y are adjacent
    // bool adjacent(Vertex *x, Vertex *y);
    BOOST_AUTO_TEST_SUITE(adjacent);

        BOOST_AUTO_TEST_CASE(pass_normal) {

            // pre-setup: add vertices to graph
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);
            
            // initial check: vertices aren't adjacent
            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv2) == false);
            BOOST_TEST_REQUIRE(g.adjacent(vv2, vv1) == false);
            
            BOOST_TEST_REQUIRE(g.adjacent(vv1, cv1) == false);
            BOOST_TEST_REQUIRE(g.adjacent(cv1, vv1) == false);
            
            BOOST_TEST_REQUIRE(g.adjacent(vv2, cv1) == false);
            BOOST_TEST_REQUIRE(g.adjacent(cv1, vv2) == false);
            // checking against a vertex not in graph should return false
            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv3) == false);
            BOOST_TEST_REQUIRE(g.adjacent(vv3, vv1) == false);
            
            BOOST_TEST_REQUIRE(g.adjacent(vv2, vv3) == false);
            BOOST_TEST_REQUIRE(g.adjacent(vv3, vv2) == false);
            
            BOOST_TEST_REQUIRE(g.adjacent(cv1, vv3) == false);
            BOOST_TEST_REQUIRE(g.adjacent(vv3, cv1) == false);
            
            // setup: add edges to graph
            g.add_edge(vv1, vv2, e);
            g.add_edge(vv1, cv1, e);
            
            // test: check which vertices are adjacent
            // these should be
            BOOST_TEST(g.adjacent(vv1, vv2) == true);
            BOOST_TEST(g.adjacent(vv2, vv1) == true);
            
            BOOST_TEST(g.adjacent(vv1, cv1) == true);
            BOOST_TEST(g.adjacent(cv1, vv1) == true);
            // these shouldn't
            BOOST_TEST(g.adjacent(vv2, cv1) == false);
            BOOST_TEST(g.adjacent(cv1, vv2) == false);
        };

    BOOST_AUTO_TEST_SUITE_END();

    // return all neighbors of vertex x
    // std::vector<Vertex*> neighbors(Vertex *x);
    BOOST_AUTO_TEST_SUITE(neighbors);

        BOOST_AUTO_TEST_CASE(pass_normal) {
            // pre-setup: add vertices to graph
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);
            
            // initial check: neighbors are empty
            BOOST_REQUIRE_EQUAL(g.neighbors(vv1).size(), 0);
            BOOST_REQUIRE_EQUAL(g.neighbors(vv2).size(), 0);
            BOOST_REQUIRE_EQUAL(g.neighbors(cv1).size(), 0);
            
            // setup: add edges to the graph
            g.add_edge(vv1, vv2, e);
            g.add_edge(vv1, cv1, e);
            
            // test: check for neighbors content
            std::vector<Vertex*> vv1_actual = g.neighbors(vv1);
            std::vector<Vertex*> vv1_expected = std::vector<Vertex*> {&vv2, &cv1};
            std::sort(vv1_actual.begin(),     vv1_actual.end());
            std::sort(vv1_expected.begin(), vv1_expected.end());

            std::vector<Vertex*> vv2_actual = g.neighbors(vv2);
            std::vector<Vertex*> vv2_expected = std::vector<Vertex*> {&vv1};
            std::sort(vv2_actual.begin(),     vv2_actual.end());
            std::sort(vv2_expected.begin(), vv2_expected.end());
            
            std::vector<Vertex*> cv1_actual = g.neighbors(cv1);
            std::vector<Vertex*> cv1_expected = std::vector<Vertex*> {&vv1};
            std::sort(cv1_actual.begin(),     cv1_actual.end());
            std::sort(cv1_expected.begin(), cv1_expected.end());

            BOOST_CHECK_EQUAL_COLLECTIONS(vv1_actual.begin(), vv1_actual.end(), vv1_expected.begin(), vv1_expected.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(vv2_actual.begin(), vv2_actual.end(), vv2_expected.begin(), vv2_expected.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(cv1_actual.begin(), cv1_actual.end(), cv1_expected.begin(), cv1_expected.end());
        };

        BOOST_AUTO_TEST_CASE(pass_not_contained_vertex) {
            // test against non-contained-by-graph vertices
            BOOST_REQUIRE_EQUAL(g.neighbors(vv3).size(), 0);
        };

    BOOST_AUTO_TEST_SUITE_END();
    
    // adds vertex x to the graph
    // void add_vertex(Vertex *x);
    BOOST_AUTO_TEST_SUITE(add_vertex);

        BOOST_AUTO_TEST_CASE(pass_normal) {
            // initial check: vertices aren't in the graph
            BOOST_TEST_REQUIRE(g.contains_vertex(vv1) == false);
            BOOST_TEST_REQUIRE(g.contains_vertex(vv2) == false);
            BOOST_TEST_REQUIRE(g.contains_vertex(cv1) == false);
            
            // setup: add vertices to graph
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            
            // test: check if vertices are indeed in the graph
            // these should be in
            BOOST_TEST(g.contains_vertex(vv1) == true);
            BOOST_TEST(g.contains_vertex(vv2) == true);
            // these shouldn't
            BOOST_TEST(g.contains_vertex(cv1) == false);        
        };
        
        BOOST_AUTO_TEST_CASE(pass_already_contained_vertex) {
            // pre-setup: add vertices to graph
            g.add_vertex(vv1);

            // initial check: some vertices are in the graph
            BOOST_TEST_REQUIRE(g.contains_vertex(vv1) == true);
            BOOST_TEST_REQUIRE(g.contains_vertex(vv2) == false);
            BOOST_TEST_REQUIRE(g.contains_vertex(cv1) == false);
            
            // setup: add vertex one more time
            g.add_vertex(vv1);

            // test: check if vertices are indeed in the graph
            // these should be in
            BOOST_TEST(g.contains_vertex(vv1) == true);
            // these shouldn't
            BOOST_TEST(g.contains_vertex(vv2) == false);
            BOOST_TEST(g.contains_vertex(cv1) == false);              
        }

    BOOST_AUTO_TEST_SUITE_END();

    // removes vertex x if there, as well as edges connected to x
    // void remove_vertex(Vertex *x);
    BOOST_AUTO_TEST_SUITE(remove_vertex);

        BOOST_AUTO_TEST_CASE(pass_normal) {
            // pre-setup: add vertices and edges into the graph
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);

            g.add_edge(vv1, vv2, e);
            g.add_edge(vv1, cv1, e);
            g.add_edge(cv1, vv2, e);

            // initial check: check that the ones added are in the graph
            BOOST_TEST_REQUIRE(g.contains_vertex(vv1) == true);
            BOOST_TEST_REQUIRE(g.contains_vertex(vv2) == true);
            BOOST_TEST_REQUIRE(g.contains_vertex(cv1) == true);

            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv2) == true);
            BOOST_TEST_REQUIRE(g.adjacent(vv1, cv1) == true);
            BOOST_TEST_REQUIRE(g.adjacent(cv1, vv2) == true);
            
            // setup: remove some vertices
            g.remove_vertex(vv1);
            
            // test: check if vertices and edges have been removed
            // these shouldn't be in the graph
            BOOST_TEST(g.contains_vertex(vv1) == false);

            BOOST_TEST(g.adjacent(vv1, vv2) == false);
            BOOST_TEST(g.adjacent(vv1, cv1) == false);

            // these should
            BOOST_TEST(g.contains_vertex(vv2) == true);
            BOOST_TEST(g.contains_vertex(cv1) == true);
            
            BOOST_TEST(g.adjacent(cv1, vv2) == true);

        };

        BOOST_AUTO_TEST_CASE(pass_not_contained_vertex) {
            // pre-setup: add vertices and edges into the graph
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);

            g.add_edge(vv1, vv2, e);
            g.add_edge(vv1, cv1, e);
            g.add_edge(cv1, vv2, e);

            // initial check: check that the ones added are in the graph
            BOOST_TEST_REQUIRE(g.contains_vertex(vv1) == true);
            BOOST_TEST_REQUIRE(g.contains_vertex(vv2) == true);
            BOOST_TEST_REQUIRE(g.contains_vertex(cv1) == true);

            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv2) == true);
            BOOST_TEST_REQUIRE(g.adjacent(vv1, cv1) == true);
            BOOST_TEST_REQUIRE(g.adjacent(cv1, vv2) == true);
            
            // setup: remove a vertex not contained in the graph
            g.remove_vertex(vv3);
            
            // test: check if vertices have been removed
            // none should have been removed
            BOOST_TEST(g.contains_vertex(vv1) == true);
            BOOST_TEST(g.contains_vertex(cv1) == true);
            BOOST_TEST(g.contains_vertex(vv2) == true);

            BOOST_TEST(g.adjacent(vv1, vv2) == true);
            BOOST_TEST(g.adjacent(vv1, cv1) == true);
            BOOST_TEST(g.adjacent(cv1, vv2) == true);
        };

    BOOST_AUTO_TEST_SUITE_END();

    // adds edge z from vertex x to y if not there
    // void add_edge(Vertex *x, Vertex *y, Edge z);
    BOOST_AUTO_TEST_SUITE(add_edge);
        
        BOOST_AUTO_TEST_CASE(pass_normal) {
            // pre-setup: add vertices to graph
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);

            // initial check: no edges are in the graph
            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv2) == false);
            BOOST_TEST_REQUIRE(g.adjacent(vv1, cv1) == false);
            BOOST_TEST_REQUIRE(g.adjacent(cv1, vv2) == false);
            
            // setup: add edges to the graph
            g.add_edge(vv1, vv2, e);
            g.add_edge(vv1, cv1, e);
            
            // test: check that vertices are now adjacent to each other
            // these should be adjacent
            BOOST_TEST(g.adjacent(vv1, vv2) == true);
            BOOST_TEST(g.adjacent(vv1, cv1) == true);
            // these shouldn't
            BOOST_TEST(g.adjacent(cv1, vv2) == false);                
        };
        
        BOOST_AUTO_TEST_CASE(pass_vertex_not_in_graph) {
            // pre-setup: add vertices to graph
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);

            // initial check: no edges are in the graph
            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv2) == false);
            BOOST_TEST_REQUIRE(g.adjacent(vv1, cv1) == false);
            BOOST_TEST_REQUIRE(g.adjacent(cv1, vv2) == false);
            
            // setup: add edges to the graph where one or more vertices aren't in graph
            g.add_edge(vv3, vv2, e);
            g.add_edge(vv1, vv3, e);
            g.add_edge(vv3, vv3, e);
            
            // test: check that vertices are now adjacent to each other
            // none should really be adjacent
            BOOST_TEST(g.adjacent(vv1, vv2) == false);
            BOOST_TEST(g.adjacent(vv1, cv1) == false);
            BOOST_TEST(g.adjacent(cv1, vv2) == false);                
        };

        BOOST_AUTO_TEST_CASE(pass_edge_already_there) {
            // pre-setup: add vertices to graph and an edge
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);

            g.add_edge(vv1, vv2, e);

            // initial check: some edges are in the graph
            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv2) == true);
            BOOST_TEST_REQUIRE(g.adjacent(vv1, cv1) == false);
            BOOST_TEST_REQUIRE(g.adjacent(cv1, vv2) == false);
            
            // setup: add edge that is already in there
            g.add_edge(vv1, vv2, e);
            
            // test: check that vertices are now adjacent to each other
            // these should
            BOOST_TEST(g.adjacent(vv1, vv2) == true);
            // these shouldn't
            BOOST_TEST(g.adjacent(vv1, cv1) == false);
            BOOST_TEST(g.adjacent(cv1, vv2) == false);                
        };

    BOOST_AUTO_TEST_SUITE_END();


    // removes edge from x to y if there
    // void remove_edge(Vertex *x, Vertex *y); 
    BOOST_AUTO_TEST_SUITE(remove_edge);
    
        BOOST_AUTO_TEST_CASE(pass_normal) {
            // pre-setup: add vertices and edges
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);
            
            g.add_edge(vv1, vv2, e);
            g.add_edge(vv1, cv1, e);
            g.add_edge(cv1, vv2, e);

            // initial check: check that each vertices are adjacent to each other
            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv2) == true);
            BOOST_TEST_REQUIRE(g.adjacent(vv1, cv1) == true);
            BOOST_TEST_REQUIRE(g.adjacent(vv2, cv1) == true);
            
            // setup: remove some edge
            g.remove_edge(vv1, vv2);
            g.remove_edge(cv1, vv2);
            
            // test: check that the correct vertices are still adjacent
            // these should be
            BOOST_TEST(g.adjacent(vv1, cv1) == true);
            // these shouldn't
            BOOST_TEST(g.adjacent(vv1, vv2) == false);
            BOOST_TEST(g.adjacent(cv1, vv2) == false);
                
        };

        BOOST_AUTO_TEST_CASE(pass_vertex_not_in_graph) {
            // pre-setup: add vertices and edges
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);
            
            g.add_edge(vv1, vv2, e);
            g.add_edge(vv1, cv1, e);
            g.add_edge(cv1, vv2, e);

            // initial check: check that each vertices are adjacent to each other
            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv2) == true);
            BOOST_TEST_REQUIRE(g.adjacent(vv1, cv1) == true);
            BOOST_TEST_REQUIRE(g.adjacent(vv2, cv1) == true);
            
            // setup: remove edges with non-existing-vertex arguments passed
            g.remove_edge(vv1, vv3);
            g.remove_edge(vv3, vv2);
            g.remove_edge(vv3, vv3);
            
            // test: check that the correct vertices are still adjacent
            // all should still be
            BOOST_TEST(g.adjacent(vv1, cv1) == true);
            BOOST_TEST(g.adjacent(vv1, vv2) == true);
            BOOST_TEST(g.adjacent(cv1, vv2) == true);
        };

        BOOST_AUTO_TEST_CASE(pass_same_vertex) {
            // pre-setup: add vertices and edges
            g.add_vertex(vv1);
            g.add_vertex(vv2);
            g.add_vertex(cv1);
            
            g.add_edge(vv1, vv2, e);
            g.add_edge(vv1, cv1, e);
            g.add_edge(cv1, vv2, e);

            // initial check: check that each vertices are adjacent to each other
            BOOST_TEST_REQUIRE(g.adjacent(vv1, vv2) == true);
            BOOST_TEST_REQUIRE(g.adjacent(vv1, cv1) == true);
            BOOST_TEST_REQUIRE(g.adjacent(vv2, cv1) == true);
            
            // setup: remove edges by passing the same vertex that exist in graph
            g.remove_edge(vv1, vv1);
            g.remove_edge(vv2, vv2);
            g.remove_edge(cv1, cv1);
            
            // test: check that the correct vertices are still adjacent
            // all should still be
            BOOST_TEST(g.adjacent(vv1, cv1) == true);
            BOOST_TEST(g.adjacent(vv1, vv2) == true);
            BOOST_TEST(g.adjacent(cv1, vv2) == true);
        };

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();