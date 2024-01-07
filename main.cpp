#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <set>
#include <vector>

#include "src/cspSolver/ARC.h"
#include "src/cspSolver/frontier/Frontier.h"
#include "src/cspSolver/CSPGraphCreator.h"

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

int old_main() 
{
    string name = "newVertex";
    
    set<int> initDomain {0, 10};

    auto vv = std::make_unique<GraphImplementation::VariableVertex>(name, initDomain);

    std::cout << "Name is: " << vv->getName() << "!" << std::endl;

    auto cv = std::make_unique<GraphImplementation::ConstraintVertex>(
        "",
        [] (int mainVal, std::vector<GraphImplementation::VariableVertex> varList)
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

    return 0;
    
};

int old_main2() 
{
    auto vv = GraphImplementation::VariableVertex("newVariableVertex", {0, 10});

    std::cout << "PRINTING VARIABLE VERTEX: \n" << vv << std::endl;

    auto cv = GraphImplementation::ConstraintVertex(
        "newConstraintVertex", 
        GraphImplementation::ConstraintVertex::exactlyN(0, 1),
        "Checks if there can be exactly one of type 0 domain value.");
    
    std::cout << "PRINTING CONSTRAINT VERTEX: \n" << cv << std::endl;

    auto graph = GraphImplementation::Graph();
    graph.add_vertex(vv);
    graph.add_vertex(cv);
    graph.add_edge(cv, vv, {});

    std::cout << "PRINTING GRAPH: \n" << graph << std::endl;

    auto cspGraph = CSPSolverImplementation::CSPGraph();
    cspGraph.add_constraint(
        "ExactlyOneZero", 
        GraphImplementation::ConstraintVertex::exactlyN(0, 1),
        "Constraints the number of 0s possible to one."
    );
    cspGraph.add_variable("Variable1", {0, 99, 1023});
    cspGraph.add_edge("Variable1", "ExactlyOneZero");

    std::cout << "PRINTING CSP GRAPH: \n" << cspGraph << std::endl;

    return 0;
};

int main() {
    // CSPSolverImplementation::CSPGraphCreator cspGc;
    // cspGc.StartCSPGraphCreator();
    CSPSolverImplementation::ARC arc;
    auto vv = GraphImplementation::VariableVertex("newVariableVertex", {0, 10});
    auto cv = GraphImplementation::ConstraintVertex(
        "newConstraintVertex", 
        GraphImplementation::ConstraintVertex::exactlyN(0, 1),
        "Checks if there can be exactly one of type 0 domain value.");

    arc.main_var = &vv;
    arc.other_var_list = std::vector<GraphImplementation::VariableVertex*>({});
    arc.constraint = &cv;

    CSPSolverImplementation::Frontier f = CSPSolverImplementation::Frontier(CSPSolverImplementation::Frontier::QueueMode);
    std::cout << "Is empty: " << f.empty() << std::endl;
    std::cout << "Size: " << f.size() << std::endl;
    f = CSPSolverImplementation::Frontier(CSPSolverImplementation::Frontier::QueueMode);
    std::cout << "Is empty: " << f.empty() << std::endl;
    std::cout << "Size: " << f.size() << std::endl;
    f.push(arc);
    std::cout << "Is empty: " << f.empty() << std::endl;
    std::cout << "Size: " << f.size() << std::endl;
    std::cout << "Currently in frontier: " << f.pop() << std::endl;
}