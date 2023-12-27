// Author: Akira Kudo
// Description: Implements tests for the VariableVertex class in GraphImplementation.
//  Simple enough (almost a struct built on top of std functions)
//  that we won't need actual tests.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(VariableVertex_test_suite, * boost::unit_test::label("VariableVertex"));
    
    BOOST_AUTO_TEST_CASE(variableVertex_no_test) {
        std::cout << "VariableVertex has currently no test as it is a simple class." << std::endl;
    }

BOOST_AUTO_TEST_SUITE_END();


// BELOW IS CODE I USED TO SEE IF EVERYTHING CAME TOGETHER.
// #include <iostream>
// #include <set>
// #include <string>

// #include "src/graphImplementation/vertices/VariableVertex.h"

// using std::string, std::set;

// struct GameRoles {
//     enum Role { acFollower, bug, crewMember, doctor, engineer, 
//                 gnosia, guardianAngel, guardDuty, Role_MAX };
    
//     string roleNames[Role_MAX] = { 
//         "AC Follower", "Bug", "Crew Member", "Doctor", 
//         "Engineer", "Gnosia", "Guardian Angel", "Guard Duty"
//         };

//     // order has to match between Role and roleNames for getRoleFronNumber to work
//     string getRoleFromNumber(int n) {
//         if (n < 0 || n >= Role_MAX) return "FOO";
//         return roleNames[n];
//     }
// };

// void printDomain(GraphImplementation::VariableVertex *vv) {
//     GameRoles gameRoles;
//     std::cout << "Printing the roles!" << std::endl;
//     for (int n : vv->getDomain())
//     {
//         std::cout << gameRoles.getRoleFromNumber(n) << " ";
//     }
//     std::cout << std::endl;
// };

// int main()
// {
//     string name = "newVertex";
    
//     set<int> initDomain {0, 1, 2, 3, 4, 5, 6, 7};

//     GraphImplementation::VariableVertex *vv = new GraphImplementation::VariableVertex(name, initDomain);
    
//     delete vv;
//     vv = NULL;
// };