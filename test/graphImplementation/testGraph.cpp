// Author: Akira Kudo
// Description: Implements tests for the Graph class in GraphImplementation.

#define BOOST_TEST_MODULE Graph Implementation Testcases
#include <boost/test/included/unit_test.hpp>
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(Graph_test_suite);

    BOOST_AUTO_TEST_CASE(simple_test, * utf::label("l1")) {
        BOOST_CHECK_EQUAL(2+2, 4);
    };

    BOOST_AUTO_TEST_CASE(failing_test, * utf::label("l2")) {
        BOOST_CHECK_EQUAL(2+2, 5);
    };

BOOST_AUTO_TEST_SUITE_END();


// bool contains_vertex(Vertex *x);
// // return if vertices x and y are adjacent
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