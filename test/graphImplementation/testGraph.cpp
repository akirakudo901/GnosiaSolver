// Author: Akira Kudo
// Description: Implements tests for the Graph class in GraphImplementation.

#define BOOST_TEST_MODULE Graph Implementation Testcases
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <initializer_list>
#include <set>
#include <string>

#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"
#include "src/graphImplementation/vertices/Vertex.h"

using namespace GraphImplementation;

// defining fixture
struct F {
    std::set<int> initDomain = std::set<int>({0, 1, 2});
    
    VariableVertex *vv1;
    VariableVertex *vv2;
    ConstraintVertex *cv1;
    Graph *g;

    F() 
    :  g( new Graph() )
    {
        std::string name1 = "vv1";
        std::string name2 = "vv2";

        vv1 = new VariableVertex(name1, initDomain);
        vv2 = new VariableVertex(name2, initDomain);
        cv1 = new ConstraintVertex(
            [] (VariableVertex mainVar, std::initializer_list<VariableVertex> varList) {
                for (auto element : varList) if (element.getName() == mainVar.getName()) return true;
                return false;
            });        
    };
    
    ~F() {
        delete g;
        g = nullptr;

        delete vv1;
        vv1 = nullptr;
        delete vv2;
        vv2 = nullptr;
        delete cv1;
        cv1 = nullptr;
    };
};

BOOST_FIXTURE_TEST_SUITE(Graph_test_suite, F, * boost::unit_test::label("Graph"));

    // function returns whether vertex x is in the graph
    // bool contains_vertex(Vertex *x)
    BOOST_AUTO_TEST_CASE(test_contains_vertex) {
        
        // initial check: check vertices aren't in graph
        BOOST_TEST(g->contains_vertex(vv1) == false);
        BOOST_TEST(g->contains_vertex(vv2) == false);
        BOOST_TEST(g->contains_vertex(cv1) == false);
        
        // setup: add vertex to graph
        g->add_vertex(vv1);
        g->add_vertex(cv1);
        
        // test:
        // these should be included
        BOOST_TEST(g->contains_vertex(vv1) == true);
        BOOST_TEST(g->contains_vertex(cv1) == true);
        // this shouldn't be included
        BOOST_TEST(g->contains_vertex(vv2) == false);
    };

    // returns if vertices x and y are adjacent
    // bool adjacent(Vertex *x, Vertex *y);
    // // return all neighbors of vertex x
    // std::vector<Vertex*> neighbors(Vertex *x);
    // // adds vertex x to the graph
    // void add_vertex(Vertex *x);
    // // removes vertex x if there
    // void remove_vertex(Vertex *x);
    // // adds edge z from vertex x to y if not there
    // void add_edge(Vertex *x, Vertex *y, Edge z);
    // // removes edge from x to y if there
    // void remove_edge(Vertex *x, Vertex *y); 

BOOST_AUTO_TEST_SUITE_END();