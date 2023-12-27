// Author: Akira Kudo
// Description: Implements tests for the ConstraintVertex class in GraphImplementation.
//  Simple enough (almost a struct built on top of std functions)
//  that we won't need actual tests.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(ConstraintVertex_test_suite, * boost::unit_test::label("ConstraintVertex"));
    
    BOOST_AUTO_TEST_CASE(constraintVertex_no_test) {
        std::cout << "ConstraintVertex has currently no test as it is a simple class." << std::endl;
    }

BOOST_AUTO_TEST_SUITE_END();