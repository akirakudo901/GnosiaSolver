// Author: Akira Kudo
// Description: Implements tests for the VariableVertex class in GraphImplementation.
//  Simple enough (almost a struct built on top of std functions)
//  that we won't need actual tests.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "src/graphImplementation/vertices/VariableVertex.h"

using GraphImplementation::VariableVertex;

BOOST_AUTO_TEST_SUITE(VariableVertex_test_suite, * boost::unit_test::label("VariableVertex"));
    
    BOOST_AUTO_TEST_SUITE(testing_equal_operator);
        
        BOOST_AUTO_TEST_CASE(unequal_name_unequal_domain) {
            // setup: create variables with different names & domain
            VariableVertex vv1 = VariableVertex("vv1", {0, 1, 2});
            VariableVertex vv2 = VariableVertex("vv2", {10, 11, 12});
            // test: check that they yield false for equality comparison
            BOOST_CHECK_EQUAL(vv1 == vv2, false);
        }

        BOOST_AUTO_TEST_CASE(unequal_name_equal_domain) {
            // setup: create variables with different names & same domain
            VariableVertex vv1 = VariableVertex("vv1", {0, 1, 2});
            VariableVertex vv2 = VariableVertex("vv2", {0, 1, 2});
            VariableVertex vv3 = VariableVertex("vv3", {2, 1, 0});
            // test: check that they yield false for equality comparison
            BOOST_CHECK_EQUAL(vv1 == vv2, false);
            BOOST_CHECK_EQUAL(vv1 == vv3, false);
            BOOST_CHECK_EQUAL(vv2 == vv3, false);
        }

        BOOST_AUTO_TEST_CASE(equal_name_unequal_domain) {
            // setup: create variables with same names & different domains
            VariableVertex vv1 = VariableVertex("vv1", {0, 1, 2});
            VariableVertex vv2 = VariableVertex("vv1", {10, 11, 12});
            // test: check that they yield false for equality comparison
            BOOST_CHECK_EQUAL(vv1 == vv2, false);
        }

        BOOST_AUTO_TEST_CASE(equal_name_equal_domain) {
            // setup: create variables with same names & domains
            // different orderings but same element count as same domain
            VariableVertex vv1 = VariableVertex("vv1", {0, 1, 2});
            VariableVertex vv2 = VariableVertex("vv1", {0, 2, 1});
            VariableVertex vv3 = VariableVertex("vv1", {1, 0, 2});
            VariableVertex vv4 = VariableVertex("vv1", {1, 2, 0});
            VariableVertex vv5 = VariableVertex("vv1", {2, 0, 1});
            VariableVertex vv6 = VariableVertex("vv1", {2, 1, 0});
            // test: check that they yield false for equality comparison
            BOOST_CHECK_EQUAL(vv1 == vv1, true); //obviously, equal to yourself
            BOOST_CHECK_EQUAL(vv1 == vv2, true);
            BOOST_CHECK_EQUAL(vv1 == vv3, true);
            BOOST_CHECK_EQUAL(vv1 == vv4, true);
            BOOST_CHECK_EQUAL(vv1 == vv5, true);
            BOOST_CHECK_EQUAL(vv1 == vv6, true);
        }

    BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE(testing_not_equal_operator);
        
        BOOST_AUTO_TEST_CASE(unequal_name_unequal_domain) {
            // setup: create variables with different names & domain
            VariableVertex vv1 = VariableVertex("vv1", {0, 1, 2});
            VariableVertex vv2 = VariableVertex("vv2", {10, 11, 12});
            // test: check that they yield true for inequality comparison
            BOOST_CHECK_EQUAL(vv1 != vv2, true);
        }

        BOOST_AUTO_TEST_CASE(unequal_name_equal_domain) {
            // setup: create variables with different names & same domain
            VariableVertex vv1 = VariableVertex("vv1", {0, 1, 2});
            VariableVertex vv2 = VariableVertex("vv2", {0, 1, 2});
            VariableVertex vv3 = VariableVertex("vv3", {2, 1, 0});
            // test: check that they yield true for inequality comparison
            BOOST_CHECK_EQUAL(vv1 != vv2, true);
            BOOST_CHECK_EQUAL(vv1 != vv3, true);
            BOOST_CHECK_EQUAL(vv2 != vv3, true);
        }

        BOOST_AUTO_TEST_CASE(equal_name_unequal_domain) {
            // setup: create variables with same names & different domains
            VariableVertex vv1 = VariableVertex("vv1", {0, 1, 2});
            VariableVertex vv2 = VariableVertex("vv1", {10, 11, 12});
            // test: check that they yield true for inequality comparison
            BOOST_CHECK_EQUAL(vv1 != vv2, true);
        }

        BOOST_AUTO_TEST_CASE(equal_name_equal_domain) {
            // setup: create variables with same names & domains
            // different orderings but same element count as same domain
            VariableVertex vv1 = VariableVertex("vv1", {0, 1, 2});
            VariableVertex vv2 = VariableVertex("vv1", {0, 2, 1});
            VariableVertex vv3 = VariableVertex("vv1", {1, 0, 2});
            VariableVertex vv4 = VariableVertex("vv1", {1, 2, 0});
            VariableVertex vv5 = VariableVertex("vv1", {2, 0, 1});
            VariableVertex vv6 = VariableVertex("vv1", {2, 1, 0});
            // test: check that they yield true for inequality comparison
            BOOST_CHECK_EQUAL(vv1 != vv1, false); //obviously, equal to yourself
            BOOST_CHECK_EQUAL(vv1 != vv2, false);
            BOOST_CHECK_EQUAL(vv1 != vv3, false);
            BOOST_CHECK_EQUAL(vv1 != vv4, false);
            BOOST_CHECK_EQUAL(vv1 != vv5, false);
            BOOST_CHECK_EQUAL(vv1 != vv6, false);
        }

    BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();