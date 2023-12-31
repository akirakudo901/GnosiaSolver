#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <set>

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using std::string, std::set;

struct GameRoles {
    enum Role { acFollower, bug, crewMember, doctor, engineer, 
                gnosia, guardianAngel, guardDuty, Role_MAX };
    
    string roleNames[Role_MAX] = { 
        "AC Follower", "Bug", "Crew Member", "Doctor", 
        "Engineer", "Gnosia", "Guardian Angel", "Guard Duty"
        };

    // order has to match between Role and roleNames for getRoleFronNumber to work
    string getRoleFromNumber(int n) {
        if (n < 0 || n >= Role_MAX) return "FOO";
        return roleNames[n];
    }
};

void printDomain(GraphImplementation::VariableVertex *vv) {
    GameRoles gameRoles;
    std::cout << "Printing the roles!" << std::endl;
    for (int n : vv->getDomain())
    {
        std::cout << gameRoles.getRoleFromNumber(n) << " ";
    }
    std::cout << std::endl;
};

int main() 
{
    string name = "newVertex";
    
    set<int> initDomain {0, 10};

    auto vv = std::make_unique<GraphImplementation::VariableVertex>(name, initDomain);

    std::cout << "Name is: " << vv->getName() << "!" << std::endl;

    auto cv = std::make_unique<GraphImplementation::ConstraintVertex>(
        [] (int mainVal, std::initializer_list<GraphImplementation::VariableVertex> varList)
        {
            for (auto variable : varList) {
                auto vDomain = variable.getDomain();
                if (vDomain.find(mainVal) != vDomain.end()) return true;
            }
            return false;
        }
    );

    auto vv2 = std::make_unique<GraphImplementation::VariableVertex>(name, set<int>{0, 1, 2});
    auto vv3 = std::make_unique<GraphImplementation::VariableVertex>("otherNewVertex", set<int>{0, 5, 6});

    for (int dVal : vv->getDomain()) {
        bool constraintIsMet_vv2 = cv->constraintIsMet(dVal, {*vv2}); 
        bool constraintIsMet_vv3 = cv->constraintIsMet(dVal, {*vv3});
        std::cout << "Constraint is" 
                  << ((constraintIsMet_vv2) ? "" : "n't") 
                  <<" met for vv value " << dVal << " comparing against vv2!" << std::endl;  
        std::cout << "Constraint is" 
                  << ((constraintIsMet_vv3) ? "" : "n't") 
                  <<" met for vv value " << dVal << " comparing against vv3!\n" << std::endl;
    }
    
};