#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <set>
#include <vector>

#include "src/cspSolver/ARC.h"
#include "src/cspSolver/frontier/Frontier.h"
#include "src/cspSolver/CSPGraphCreator.h"
#include "src/cspSolver/CSPGraph.h"
#include "src/cspSolver/CSPSolver.h"

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"

using std::string, std::set;
using namespace CSPSolverImplementation;
using namespace GraphImplementation;

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

int old_main1() 
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
    CSPGraph graph = CSPGraph();
    CSPSolver solver = CSPSolver();

    int GNOSIA_NUMBER = 3;

    enum Role 
    { acFollower, bug, crewMember, doctor, engineer, 
      gnosia, guardianAngel, guardDuty, Role_MAX };

    std::string characters[15] = {
        "Chipie", "Comet", "Gina", "Jonas", "Kukrushka",
        "Otome", "Raqio", "Remnan", "Setsu", "ShaMing",
        "Shigemichi", "SQ", "Stella", "You", "Yuriko"
    };

    std::set<int> full_domain 
    { acFollower, bug, crewMember, doctor, engineer, 
      gnosia, guardianAngel, guardDuty, Role_MAX};

    // add variables
    for (std::string vv_name : characters) 
    {
        graph.add_variable(vv_name, full_domain);
    }

    // add constraints
    // RULE BASED
    std::string rules[15] = {
        "NoMoreThanNGnosia", "AtLeastOneGnosia", 
        "GnosiaLessThanHalfRemainingCrews", "OneOrLessEngineer", 
        "OneOrLessDoctor", "OneOrLessGuardianAngel", 
        "OneOrLessBug", "OneOrLessACFollower"
    };
    // between 1 and GNOSIA_NUMBER Gnosias
    graph.add_constraint("NoMoreThanNGnosia", ConstraintVertex::lesserOrEqualToN(Role::gnosia, GNOSIA_NUMBER));
    graph.add_constraint("AtLeastOneGnosia", ConstraintVertex::greaterOrEqualToN(Role::gnosia, 1));
    // at least less then (remaining variable number) / 2 gnosias
    graph.add_constraint("GnosiaLessThanHalfRemainingCrews", ConstraintVertex::lesserOrEqualToN(Role::gnosia, 15/2));
    // only one / less of: engineer, doctor, guardian angel, bug, AC follower
    graph.add_constraint("OneOrLessEngineer",      ConstraintVertex::lesserOrEqualToN(Role::engineer, 1));
    graph.add_constraint("OneOrLessDoctor",        ConstraintVertex::lesserOrEqualToN(Role::doctor, 1));
    graph.add_constraint("OneOrLessGuardianAngel", ConstraintVertex::lesserOrEqualToN(Role::guardianAngel, 1));
    graph.add_constraint("OneOrLessBug",           ConstraintVertex::lesserOrEqualToN(Role::bug, 1));
    graph.add_constraint("OneOrLessACFollower",    ConstraintVertex::lesserOrEqualToN(Role::acFollower, 1));
    // either have two or zero guard duties <- hard to implement
    
    for (std::string rule : rules) {
        for (auto vv_name : graph.get_all_variable_names()) {
            graph.add_edge(vv_name, rule);
        }
    }

    // TURN BASED: at each turn, we can update info and determine accordingly
    // disappeared members cannot be Gnosias
    graph.add_constraint("DisappearingMembersAreNotGnosias", ConstraintVertex::exactlyN(Role::gnosia, 0));
    // if there are no engineers, disappeared members cannot be bugs
    graph.add_constraint("IfNoEngDisappearingMembersAreNotBugs", ConstraintVertex::exactlyN(Role::bug, 0));
    // lying members are either Gnosias, a bug or an AC Follower
    // that is, not in any of (Crew Member, Doctor, Engineer, Guardian Angel, Guard Duty)
    graph.add_constraint("LyingMembersAreNotCrewMember", ConstraintVertex::exactlyN(Role::crewMember, 0));
    graph.add_constraint("LyingMembersAreNotDoctor", ConstraintVertex::exactlyN(Role::doctor, 0));
    graph.add_constraint("LyingMembersAreNotEngineer", ConstraintVertex::exactlyN(Role::engineer, 0));
    graph.add_constraint("LyingMembersAreNotGuardianAngel", ConstraintVertex::exactlyN(Role::guardianAngel, 0));
    graph.add_constraint("LyingMembersAreNotGuardDuty", ConstraintVertex::exactlyN(Role::guardDuty, 0));
    // if a member accuses another, the two cannot be in the same camp; that is,
    //   they belong in two different groups from: 
    //   > (Crew Member, Doctor, Engineer, Guardian Angel, Guard Duty)
    //   > (AC Follower, Gnosia)
    //   > (Bug) 
    // hard to implement, later

    // also, if a true doctor calls out somebody, they are a gnosia
    graph.add_constraint("GnosiaIfCalledOutByTrueDoctor", ConstraintVertex::exactlyN(Role::gnosia, 1));
    // if a true engineer calls out somebody, they are a gnosia
    graph.add_constraint("GnosiaIfCalledOutByTrueEngineer", ConstraintVertex::exactlyN(Role::gnosia, 1));
    
    // circumstantial, mainly for "You"
    graph.add_constraint("IsACFollower",    ConstraintVertex::exactlyN(Role::acFollower,    1));
    graph.add_constraint("IsBug",           ConstraintVertex::exactlyN(Role::bug,           1));
    graph.add_constraint("IsCrewMember",    ConstraintVertex::exactlyN(Role::crewMember,    1));
    graph.add_constraint("IsDoctor",        ConstraintVertex::exactlyN(Role::doctor,        1));
    graph.add_constraint("IsEngineer",      ConstraintVertex::exactlyN(Role::engineer,      1));
    graph.add_constraint("IsGnosia",        ConstraintVertex::exactlyN(Role::gnosia,        1));
    graph.add_constraint("IsGuardDuty",     ConstraintVertex::exactlyN(Role::guardDuty,     1));
    graph.add_constraint("IsGuardianAngel", ConstraintVertex::exactlyN(Role::guardianAngel, 1));
    
    graph.add_edge("You", "IsCrewMember");


    auto answers = solver.arcConsistency(graph);

    for (auto answer : answers)
    {
        std::cout << "Answer:" << std::endl;
        for (auto var : answer)
        {
            std::cout << var << " ";
            std::cout << std::endl;
        }
    }
}