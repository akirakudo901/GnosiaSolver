// Author: Akira Kudo
// Description: Implements tests for the Vertex class in GraphImplementation.
//  Abstract so we can't really test it.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(Vertex_test_suite, * boost::unit_test::label("Vertex"));

    BOOST_AUTO_TEST_CASE(vertex_no_test) {
        std::cout << "Test any methods indirectly through subclasses of abstract Vertex!" << std::endl;
    }

BOOST_AUTO_TEST_SUITE_END();