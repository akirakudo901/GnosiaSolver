// Author: Akira Kudo
// Description: Implements tests for the ConstraintVertex class in GraphImplementation.
//  Simple enough (almost a struct built on top of std functions)
//  that we won't need actual tests.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <set>

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using namespace GraphImplementation;

BOOST_AUTO_TEST_SUITE(ConstraintVertex_test_suite, * boost::unit_test::label("ConstraintVertex"));
    
    // return whether the corresponding constraint is met given the variable vertices
    // bool GraphImplementation::ConstraintVertex::constraintIsMet(VariableVertex mainVar, initializer_list<VariableVertex> varList)
    BOOST_AUTO_TEST_SUITE(constraintIsMet);

        // checks if given domains allow the existence of n or less of the checkedDomain value
        // e.g. checkedDomain = werewolf, n = 3: returns false if there has to be more than 
        //      3 werewolves in the mix, true otherwise
        // function<bool(int, initializer_list<VariableVertex>)> GraphImplementation::ConstraintVertex::lesserOrEqualToN(int checkedDomain, int n) 
        BOOST_AUTO_TEST_SUITE(lesserOrEqualToN);

            BOOST_AUTO_TEST_CASE(unary_constraint) {
                // setup: setup variables and constraints
                // we consider a variable vertex which domain consists of {0, 1}:
                // vv1 = VariableVertex("vv1", std::set<int>{0, 1});

                // <= 0 (cannot take the given value)
                ConstraintVertex unary_cv0 = ConstraintVertex("unary_cv0", ConstraintVertex::lesserOrEqualToN(0, 0));
                // <= 1, 2 (can take the given value)
                ConstraintVertex unary_cv1 = ConstraintVertex("unary_cv1", ConstraintVertex::lesserOrEqualToN(0, 1));
                ConstraintVertex unary_cv2 = ConstraintVertex("unary_cv2", ConstraintVertex::lesserOrEqualToN(0, 2));

                // test: check whether constraint is met given vv1
                BOOST_TEST(unary_cv0.constraintIsMet(0, {}) == false); //can't be 0
                BOOST_TEST(unary_cv0.constraintIsMet(1, {}) == true);  //but anything else is ok

                BOOST_TEST(unary_cv1.constraintIsMet(0, {}) == true); //0 is ok as well
                BOOST_TEST(unary_cv1.constraintIsMet(1, {}) == true); //anything else is ok too

                BOOST_TEST(unary_cv2.constraintIsMet(0, {}) == true); //0 is ok as well
                BOOST_TEST(unary_cv2.constraintIsMet(1, {}) == true); //anything else is ok too
            };

            BOOST_AUTO_TEST_CASE(binary_constraint) {
                // setup: set up variables and the constraint
                
                // we consider a variable vertex which domain consists of {0, 1}:
                // vv1 = VariableVertex("vv1", std::set<int>{0, 1});
                VariableVertex vv2 = VariableVertex("vv2", std::set<int>{0, 1, 2});
                VariableVertex vv3 = VariableVertex("vv3", std::set<int>{0});

                // <= 0 (none can take 0)
                ConstraintVertex binary_cv0 = ConstraintVertex("binary_cv0", ConstraintVertex::lesserOrEqualToN(0, 0));
                // <= 1 (only one of the two variables can take 0)
                ConstraintVertex binary_cv1 = ConstraintVertex("binary_cv1", ConstraintVertex::lesserOrEqualToN(0, 1));
                // <= 2 (both of the two variables can take 0)
                ConstraintVertex binary_cv2 = ConstraintVertex("binary_cv2", ConstraintVertex::lesserOrEqualToN(0, 2));

                // test: check whether constraint is met given two variables
                // with cv0 (no variable can take 0)
                // vv2 can take a non-0 value (1 / 2), hence condition is really checking for vv1
                BOOST_TEST(binary_cv0.constraintIsMet(0, {vv2}) == false); //first argument (vv1) can't be 0
                BOOST_TEST(binary_cv0.constraintIsMet(1, {vv2}) == true);  //but can be anything else
                // vv3 can't take any value but 0, so no condition would fulfill a constraint involving vv3
                BOOST_TEST(binary_cv0.constraintIsMet(0, {vv3}) == false); //vv3 violates the constraint so none hold
                BOOST_TEST(binary_cv0.constraintIsMet(1, {vv3}) == false); //vv3 violates the constraint so none hold

                // with cv1 (only one of two variables can take 0)
                // vv2 can take a non-0 value (1 / 2), hence condition is really checking for vv1
                BOOST_TEST(binary_cv1.constraintIsMet(0, {vv2}) == true); //vv1 can be 0
                BOOST_TEST(binary_cv1.constraintIsMet(1, {vv2}) == true); //vv1 can also be anything else
                // vv3 can't take any value but 0, so vv1 has to be something other than 0
                BOOST_TEST(binary_cv1.constraintIsMet(0, {vv3}) == false); //both vv1 and vv3 can't take the value 0
                BOOST_TEST(binary_cv1.constraintIsMet(1, {vv3}) == true); //if only vv3 takes the value 0, it's ok

                // with cv2 (both of two variables can take 0)
                // every combinations of value should be allowed for vv1, against either vv2 or vv3
                BOOST_TEST(binary_cv2.constraintIsMet(0, {vv2}) == true);
                BOOST_TEST(binary_cv2.constraintIsMet(1, {vv2}) == true);
                
                BOOST_TEST(binary_cv2.constraintIsMet(0, {vv3}) == true);
                BOOST_TEST(binary_cv2.constraintIsMet(1, {vv3}) == true);
            };

            BOOST_AUTO_TEST_CASE(quinary_constraint) {
                // setup: set up variables and constraints
                // we consider a variable vertex which domain consists of {0, 1}:
                // vv1 = VariableVertex("vv1", std::set<int>{0, 1});
                VariableVertex vv2 = VariableVertex("vv2", std::set<int>{0, 1, 2});
                VariableVertex vv3 = VariableVertex("vv3", std::set<int>{0});
                VariableVertex vv4 = VariableVertex("vv4", std::set<int>{0});
                VariableVertex vv5 = VariableVertex("vv5", std::set<int>{1});

                // <= 0 (none can take 0)
                ConstraintVertex quinary_cv0 = ConstraintVertex("quinary_cv0", ConstraintVertex::lesserOrEqualToN(0, 0));
                // <= 1 (only one out of five variables can take 0)
                ConstraintVertex quinary_cv1 = ConstraintVertex("quinary_cv1", ConstraintVertex::lesserOrEqualToN(0, 1));
                // <= 2 (two out of five variables can take 0)
                ConstraintVertex quinary_cv2 = ConstraintVertex("quinary_cv2", ConstraintVertex::lesserOrEqualToN(0, 2));
                // <= 3 (three out of five variables can take 0)
                ConstraintVertex quinary_cv3 = ConstraintVertex("quinary_cv3", ConstraintVertex::lesserOrEqualToN(0, 3));
                // <= 4 (four out of five variables can take 0)
                ConstraintVertex quinary_cv4 = ConstraintVertex("quinary_cv4", ConstraintVertex::lesserOrEqualToN(0, 4));
                // <= 5 (five out of five variables can take 0)
                ConstraintVertex quinary_cv5 = ConstraintVertex("quinary_cv5", ConstraintVertex::lesserOrEqualToN(0, 5));

                // test: check which constraint and variable pairs are allowed
                // with cv0 (no variable can take 0) & cv1 (no variable can take 0)
                // vv3, 4 have to take a 0 value, making the condition non-fulfillable for any value of vv1
                BOOST_TEST(quinary_cv0.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == false);
                BOOST_TEST(quinary_cv0.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == false);
                
                BOOST_TEST(quinary_cv1.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == false);
                BOOST_TEST(quinary_cv1.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == false);

                // with cv2 (two out of five variables can take 0)
                // vv3, 4 have to take a 0 value, requiring vv1's value to be checked
                BOOST_TEST(quinary_cv2.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == false); //vv1 can't be 0
                BOOST_TEST(quinary_cv2.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true);  //but anything else

                // with cv3, cv4 and cv5
                // vv1 can take any value!
                BOOST_TEST(quinary_cv3.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true);
                BOOST_TEST(quinary_cv3.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true);
                
                BOOST_TEST(quinary_cv4.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true);
                BOOST_TEST(quinary_cv4.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true);

                BOOST_TEST(quinary_cv5.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true);
                BOOST_TEST(quinary_cv5.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true);
                
            };

        BOOST_AUTO_TEST_SUITE_END();

        // checks if given domains allow the existence of n or more of the checkedDomain value
        // e.g. checkedDomain = werewolf, n = 3: returns false if there has to be less than 
        //      3 werewolves in the mix, true otherwise
        // function<bool(int, initializer_list<VariableVertex>)> GraphImplementation::ConstraintVertex::greaterOrEqualToN(int checkedDomain, int n)
        BOOST_AUTO_TEST_SUITE(greaterOrEqualToN);

            BOOST_AUTO_TEST_CASE(unary_constraint) {
                // setup: setup variables and constraints
                // we consider a variable vertex which domain consists of {0, 1}:
                // vv1 = VariableVertex("vv1", std::set<int>{0, 1});

                // <= 0, 1 (can take 1 or more number of the given value)
                ConstraintVertex unary_cv0 = ConstraintVertex("unary_cv0", ConstraintVertex::greaterOrEqualToN(0, 0));
                ConstraintVertex unary_cv1 = ConstraintVertex("unary_cv1", ConstraintVertex::greaterOrEqualToN(0, 1));
                // <= 2 (have to have 2 or more of the given value, which is impossible)
                ConstraintVertex unary_cv2 = ConstraintVertex("unary_cv2", ConstraintVertex::greaterOrEqualToN(0, 2));

                // test: check whether constraint is met given vv1
                BOOST_TEST(unary_cv0.constraintIsMet(0, {}) == true); //0 is ok
                BOOST_TEST(unary_cv0.constraintIsMet(1, {}) == true); //anything else is fine too

                BOOST_TEST(unary_cv1.constraintIsMet(0, {}) == true);  //0 is ok
                BOOST_TEST(unary_cv1.constraintIsMet(1, {}) == false); //but we need at least one 0

                BOOST_TEST(unary_cv2.constraintIsMet(0, {}) == false); //we need at least two 0
                BOOST_TEST(unary_cv2.constraintIsMet(1, {}) == false); //so this isn't valid either
            };

            BOOST_AUTO_TEST_CASE(binary_constraint) {
                // setup: set up variables and the constraint
                
                // we consider a variable vertex which domain consists of {0, 1}:
                // vv1 = VariableVertex("vv1", std::set<int>{0, 1});
                VariableVertex vv2 = VariableVertex("vv2", std::set<int>{0, 1, 2});
                VariableVertex vv3 = VariableVertex("vv3", std::set<int>{1});

                // <= 0 (we can have no 0s)
                ConstraintVertex binary_cv0 = ConstraintVertex("binary_cv0", ConstraintVertex::greaterOrEqualToN(0, 0));
                // <= 1 (at least one of the two variables has to be able to be 0)
                ConstraintVertex binary_cv1 = ConstraintVertex("binary_cv1", ConstraintVertex::greaterOrEqualToN(0, 1));
                // <= 2 (both of the two variables have to be able to take 0)
                ConstraintVertex binary_cv2 = ConstraintVertex("binary_cv2", ConstraintVertex::greaterOrEqualToN(0, 2));

                // test: check whether constraint is met given two variables
                // with cv0 (we can have no 0s)
                // any combination is really ok
                BOOST_TEST(binary_cv0.constraintIsMet(0, {vv2}) == true);
                BOOST_TEST(binary_cv0.constraintIsMet(1, {vv2}) == true);
                
                BOOST_TEST(binary_cv0.constraintIsMet(0, {vv3}) == true);
                BOOST_TEST(binary_cv0.constraintIsMet(1, {vv3}) == true);

                // with cv1 (at least one of two variables can take 0)
                // vv2 can take 0 as value, hence vv1 doesn't really matter
                BOOST_TEST(binary_cv1.constraintIsMet(0, {vv2}) == true);
                BOOST_TEST(binary_cv1.constraintIsMet(1, {vv2}) == true);
                // vv3 can't take any value but 1, so vv1 has to be 0
                BOOST_TEST(binary_cv1.constraintIsMet(0, {vv3}) == true);  //we have one 0 with vv1
                BOOST_TEST(binary_cv1.constraintIsMet(1, {vv3}) == false); //here, no variable can take the value 0

                // with cv2 (both of two variables have to be able to take 0)
                // vv2 can take 0 as value, hence condition is really checking for vv1
                BOOST_TEST(binary_cv2.constraintIsMet(0, {vv2}) == true);  //we can have two 0s here
                BOOST_TEST(binary_cv2.constraintIsMet(1, {vv2}) == false); //but not here 
                // vv3 can't take 0 as value, so no combination of vv1 and vv3 values fulfill the condition
                BOOST_TEST(binary_cv2.constraintIsMet(0, {vv3}) == false);
                BOOST_TEST(binary_cv2.constraintIsMet(1, {vv3}) == false);
            };

            BOOST_AUTO_TEST_CASE(quinary_constraint) {
                // setup: set up variables and constraints
                // we consider a variable vertex which domain consists of {0, 1}:
                // vv1 = VariableVertex("vv1", std::set<int>{0, 1});
                VariableVertex vv2 = VariableVertex("vv2", std::set<int>{0, 1, 2});
                VariableVertex vv3 = VariableVertex("vv3", std::set<int>{0});
                VariableVertex vv4 = VariableVertex("vv4", std::set<int>{0});
                VariableVertex vv5 = VariableVertex("vv5", std::set<int>{1});

                // <= 0 (we don't need any 0)
                ConstraintVertex quinary_cv0 = ConstraintVertex("quinary_cv0", ConstraintVertex::greaterOrEqualToN(0, 0));
                // <= 1 (one out of five variables have to be able to take 0)
                ConstraintVertex quinary_cv1 = ConstraintVertex("quinary_cv1", ConstraintVertex::greaterOrEqualToN(0, 1));
                // <= 2 (two out of five variables have to be able to take 0)
                ConstraintVertex quinary_cv2 = ConstraintVertex("quinary_cv2", ConstraintVertex::greaterOrEqualToN(0, 2));
                // <= 3 (three out of five variables have to be able to take 0)
                ConstraintVertex quinary_cv3 = ConstraintVertex("quinary_cv3", ConstraintVertex::greaterOrEqualToN(0, 3));
                // <= 4 (four out of five variables have to be able to take 0)
                ConstraintVertex quinary_cv4 = ConstraintVertex("quinary_cv4", ConstraintVertex::greaterOrEqualToN(0, 4));
                // <= 5 (five out of five variables have to be able to take 0)
                ConstraintVertex quinary_cv5 = ConstraintVertex("quinary_cv5", ConstraintVertex::greaterOrEqualToN(0, 5));

                // test: check which constraint and variable pairs are allowed
                // with cv0 (we don't need any 0) ~ cv3 (3 variables have to be able to take 0)
                // vv2, 3 and 4 can take a 0 value, making the condition fulfilled for any value of vv1
                BOOST_TEST(quinary_cv0.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true);
                BOOST_TEST(quinary_cv0.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true);
                
                BOOST_TEST(quinary_cv1.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true);
                BOOST_TEST(quinary_cv1.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true);

                BOOST_TEST(quinary_cv2.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true);
                BOOST_TEST(quinary_cv2.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true);

                BOOST_TEST(quinary_cv3.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true);
                BOOST_TEST(quinary_cv3.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true);
                // with cv4 (4 variables have to be able to take 0)
                // given vv5 can't take a 0 variable, and vv2, 3, 4 can, it depends on the value of vv1
                BOOST_TEST(quinary_cv4.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true); //vv1 has to take 0
                BOOST_TEST(quinary_cv4.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == false); //so this is invalid
                // with cv5 (5 variables have to be able to take 0)
                // we cannot have five 0s at the same time, so no combination is valid
                BOOST_TEST(quinary_cv5.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == false);
                BOOST_TEST(quinary_cv5.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == false);
            };

        BOOST_AUTO_TEST_SUITE_END();

        // checks if given domains allow the existence of exactly n of the checkedDomain value
        // function<bool(int, initializer_list<VariableVertex>)> GraphImplementation::ConstraintVertex::exactlyN(int checkedDomain, int n)
        BOOST_AUTO_TEST_SUITE(exactlyN);

            BOOST_AUTO_TEST_CASE(unary_constraint) {
                // setup: setup variables and constraints
                // we consider a variable vertex which domain consists of {0, 1}:
                // vv1 = VariableVertex("vv1", std::set<int>{0, 1});

                // <= 0 (there has to be zero variables taking the value 0)
                ConstraintVertex unary_cv0 = ConstraintVertex("unary_cv0", ConstraintVertex::exactlyN(0, 0));
                // <= 1 (there has to be one variable taking the value 0)
                ConstraintVertex unary_cv1 = ConstraintVertex("unary_cv1", ConstraintVertex::exactlyN(0, 1));
                // <= 2 (there has to be two variables taking the value 0)
                ConstraintVertex unary_cv2 = ConstraintVertex("unary_cv2", ConstraintVertex::exactlyN(0, 2));

                // test: check whether constraint is met given vv1
                BOOST_TEST(unary_cv0.constraintIsMet(0, {}) == false); //there cannot be a 0
                BOOST_TEST(unary_cv0.constraintIsMet(1, {}) == true); //anything else is fine

                BOOST_TEST(unary_cv1.constraintIsMet(0, {}) == true);  //0 has to be here
                BOOST_TEST(unary_cv1.constraintIsMet(1, {}) == false); //so this isn't valid

                BOOST_TEST(unary_cv2.constraintIsMet(0, {}) == false); //we need two 0
                BOOST_TEST(unary_cv2.constraintIsMet(1, {}) == false); //so this isn't valid either
            };

            BOOST_AUTO_TEST_CASE(binary_constraint) {
                // setup: set up variables and the constraint
                
                // we consider a variable vertex which domain consists of {0, 1}:
                // vv1 = VariableVertex("vv1", std::set<int>{0, 1});
                VariableVertex vv2 = VariableVertex("vv2", std::set<int>{0, 1, 2});
                VariableVertex vv3 = VariableVertex("vv3", std::set<int>{1});

                // <= 0 (we can have no 0s)
                ConstraintVertex binary_cv0 = ConstraintVertex("binary_cv0", ConstraintVertex::exactlyN(0, 0));
                // <= 1 (one of the two variables has to be able to be 0)
                ConstraintVertex binary_cv1 = ConstraintVertex("binary_cv1", ConstraintVertex::exactlyN(0, 1));
                // <= 2 (both of the two variables have to be able to be 0)
                ConstraintVertex binary_cv2 = ConstraintVertex("binary_cv2", ConstraintVertex::exactlyN(0, 2));

                // test: check whether constraint is met given two variables
                // with cv0 (we can have no 0s)
                // any combination involving being one 0 is not allowed
                BOOST_TEST(binary_cv0.constraintIsMet(0, {vv2}) == false); //one 0 exists, so not allowed
                BOOST_TEST(binary_cv0.constraintIsMet(1, {vv2}) == true);  //this is fine
                
                BOOST_TEST(binary_cv0.constraintIsMet(0, {vv3}) == false); //one 0 exists, so not allowed
                BOOST_TEST(binary_cv0.constraintIsMet(1, {vv3}) == true);  //this is fine

                // with cv1 (at least one of two variables can take 0)
                // vv2 can take 0 as value, hence vv1 doesn't really matter
                BOOST_TEST(binary_cv1.constraintIsMet(0, {vv2}) == true);
                BOOST_TEST(binary_cv1.constraintIsMet(1, {vv2}) == true);
                // vv3 can't take any value but 1, so vv1 has to be 0
                BOOST_TEST(binary_cv1.constraintIsMet(0, {vv3}) == true);  //we have one 0 with vv1
                BOOST_TEST(binary_cv1.constraintIsMet(1, {vv3}) == false); //here, no variable can take the value 0

                // with cv2 (both of two variables have to be able to take 0)
                // vv2 can take 0 as value, hence condition is really checking for vv1
                BOOST_TEST(binary_cv2.constraintIsMet(0, {vv2}) == true);  //we can have two 0s here
                BOOST_TEST(binary_cv2.constraintIsMet(1, {vv2}) == false); //but not here 
                // vv3 can't take 0 as value, so no combination of vv1 and vv3 values fulfill the condition
                BOOST_TEST(binary_cv2.constraintIsMet(0, {vv3}) == false);
                BOOST_TEST(binary_cv2.constraintIsMet(1, {vv3}) == false);
            };

            BOOST_AUTO_TEST_CASE(quinary_constraint) {
                // setup: set up variables and constraints
                // we consider a variable vertex which domain consists of {0, 1}:
                // vv1 = VariableVertex("vv1", std::set<int>{0, 1});
                VariableVertex vv2 = VariableVertex("vv2", std::set<int>{0, 1, 2});
                VariableVertex vv3 = VariableVertex("vv3", std::set<int>{0});
                VariableVertex vv4 = VariableVertex("vv4", std::set<int>{0});
                VariableVertex vv5 = VariableVertex("vv5", std::set<int>{1});

                // <= 0 (we can't have any 0)
                ConstraintVertex quinary_cv0 = ConstraintVertex("quinary_cv0", ConstraintVertex::exactlyN(0, 0));
                // <= 1 (exactly one variable has to be able to take 0)
                ConstraintVertex quinary_cv1 = ConstraintVertex("quinary_cv1", ConstraintVertex::exactlyN(0, 1));
                // <= 2 (exactly two variables have to be able to take 0)
                ConstraintVertex quinary_cv2 = ConstraintVertex("quinary_cv2", ConstraintVertex::exactlyN(0, 2));
                // <= 3 (exactly three variables have to be able to take 0)
                ConstraintVertex quinary_cv3 = ConstraintVertex("quinary_cv3", ConstraintVertex::exactlyN(0, 3));
                // <= 4 (exactly four variables have to be able to take 0)
                ConstraintVertex quinary_cv4 = ConstraintVertex("quinary_cv4", ConstraintVertex::exactlyN(0, 4));
                // <= 5 (exactly five variables have to be able to take 0)
                ConstraintVertex quinary_cv5 = ConstraintVertex("quinary_cv5", ConstraintVertex::exactlyN(0, 5));

                // test: check which constraint and variable pairs are allowed
                // with cv0 (we can't have any 0) ~ cv1 (exactly one variable has to be able to take 0)
                // vv3 and 4 have to take a 0 value, making the condition unfulfilled for any value of vv1
                BOOST_TEST(quinary_cv0.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == false);
                BOOST_TEST(quinary_cv0.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == false);
                
                BOOST_TEST(quinary_cv1.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == false);
                BOOST_TEST(quinary_cv1.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == false);
                // with cv2 (exactly two variables have to be able to take 0)
                BOOST_TEST(quinary_cv2.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == false); //vv1 can't be 0
                BOOST_TEST(quinary_cv2.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true);  //but anything else
                // with cv3 (exactly three variables have to be able to take 0)
                BOOST_TEST(quinary_cv3.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true); //vv1 taking a 0 value is ok
                BOOST_TEST(quinary_cv3.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == true); //vv2 can also be the third 0 value instead
                // with cv4 (exactlty four variables have to be able to take 0)
                // given vv5 can't take a 0 variable, and vv2, 3, 4 can, it depends on the value of vv1
                BOOST_TEST(quinary_cv4.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == true);  //vv1 has to take 0
                BOOST_TEST(quinary_cv4.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == false); //so this is invalid
                // with cv5 (exactlty five variables have to be able to take 0)
                // we cannot have five 0s at the same time, so no combination is valid
                BOOST_TEST(quinary_cv5.constraintIsMet(0, {vv2, vv3, vv4, vv5}) == false);
                BOOST_TEST(quinary_cv5.constraintIsMet(1, {vv2, vv3, vv4, vv5}) == false);
            };

        BOOST_AUTO_TEST_SUITE_END();

    BOOST_AUTO_TEST_SUITE_END();
    
BOOST_AUTO_TEST_SUITE_END();