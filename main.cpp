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
    
    set<int> initDomain {0, 1, 2, 3, 4, 5, 6, 7};

    auto vv = std::make_unique<GraphImplementation::VariableVertex>(name, initDomain);

    std::cout << "Name is: " << vv->getName() << "!" << std::endl;

    auto cv = std::make_unique<GraphImplementation::ConstraintVertex>(
        [] (GraphImplementation::VariableVertex mainVar, 
            std::initializer_list<GraphImplementation::VariableVertex> varList)
        {
            for (auto element : varList) if (element.getName() == mainVar.getName()) return true;
            return false;
        }
    );

    auto vv2 = std::make_unique<GraphImplementation::VariableVertex>(name, initDomain);
    auto vv3 = std::make_unique<GraphImplementation::VariableVertex>("otherNewVertex", initDomain);

    std::cout << "Constraint is met between vv and vv2: " << cv->constraintIsMet(*vv, {*vv2}) << " " << std::endl;
    std::cout << "Constraint is met between vv and vv3: " << cv->constraintIsMet(*vv, {*vv3}) << " ";
};