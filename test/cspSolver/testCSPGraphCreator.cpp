// Author: Akira Kudo
// Description: Implements tests for functions contained within CSPGraphCreator.cpp in CSPSolverImplementation.
//  We will ignore testing for now, as I am unsure of how to best test CLI interaction.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(CSPGraphCreator_test_suite, * boost::unit_test::label("CSPGraphCreator"));
    
    BOOST_AUTO_TEST_CASE(cspGraphCreator_no_test) {
        std::cout << "CSPGraphCreator will not be tested as I am unsure of how best to test CLI interaction." << std::endl;
    }

BOOST_AUTO_TEST_SUITE_END();