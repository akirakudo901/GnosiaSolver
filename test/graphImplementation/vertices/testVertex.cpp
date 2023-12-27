// Author: Akira Kudo
// Description: Implements tests for the Vertex class in GraphImplementation.
//  Simple enough that we won't need actual tests.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(Vertex_test_suite, * boost::unit_test::label("Vertex"));

    BOOST_AUTO_TEST_CASE(vertex_no_test) {
        std::cout << "Vertex has currenctly no test as it is a simple class." << std::endl;
    }

BOOST_AUTO_TEST_SUITE_END();