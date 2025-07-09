// Author: Akira Kudo

#include "src/cspSolver/CSPGraph.h"
#include "src/gnosiaSolver/GnosiaSolver.h"
#include "src/graphImplementation/vertices/ConstraintVertex.h"

GnosiaSolverImplementation::GnosiaSolver::GnosiaSolver()
{
    this->gnosia_graph = CSPSolverImplementation::CSPGraph();
    // setup all constraints specified by the rules
}

// #####################
// PRIVATE FUNCTIONS
// sets up the rules of Gnosia as specified by constraints
void GnosiaSolverImplementation::GnosiaSolver::rule_setup()
{
    // RULE BASED
    std::string rules[15] = {
        "NoMoreThanNGnosia", "AtLeastOneGnosia", 
        "GnosiaLessThanHalfRemainingCrews", "OneOrLessEngineer", 
        "OneOrLessDoctor", "OneOrLessGuardianAngel", 
        "OneOrLessBug", "OneOrLessACFollower"
    };
    // between 1 and GNOSIA_NUMBER Gnosias
    gnosia_graph.add_constraint("NoMoreThanNGnosia", GraphImplementation::ConstraintVertex::lesserOrEqualToN(GnosiaSolverImplementation::GnosiaSolver::Roles::Role::gnosia, GNOSIA_NUMBER));
    gnosia_graph.add_constraint("AtLeastOneGnosia", GraphImplementation::ConstraintVertex::greaterOrEqualToN(GnosiaSolverImplementation::GnosiaSolver::Roles::Role::gnosia, 1));
    // no more than (remaining variable number) / 2 gnosias
    gnosia_graph.add_constraint("GnosiaLessThanHalfRemainingCrews", GraphImplementation::ConstraintVertex::lesserOrEqualToN(GnosiaSolverImplementation::GnosiaSolver::Roles::Role::gnosia, 15/2));
    // only one / less of: engineer, doctor, guardian angel, bug, AC follower
    gnosia_graph.add_constraint("OneOrLessEngineer",      GraphImplementation::ConstraintVertex::lesserOrEqualToN(GnosiaSolverImplementation::GnosiaSolver::Roles::Role::engineer, 1));
    gnosia_graph.add_constraint("OneOrLessDoctor",        GraphImplementation::ConstraintVertex::lesserOrEqualToN(GnosiaSolverImplementation::GnosiaSolver::Roles::Role::doctor, 1));
    gnosia_graph.add_constraint("OneOrLessGuardianAngel", GraphImplementation::ConstraintVertex::lesserOrEqualToN(GnosiaSolverImplementation::GnosiaSolver::Roles::Role::guardianAngel, 1));
    gnosia_graph.add_constraint("OneOrLessBug",           GraphImplementation::ConstraintVertex::lesserOrEqualToN(GnosiaSolverImplementation::GnosiaSolver::Roles::Role::bug, 1));
    gnosia_graph.add_constraint("OneOrLessACFollower",    GraphImplementation::ConstraintVertex::lesserOrEqualToN(GnosiaSolverImplementation::GnosiaSolver::Roles::Role::acFollower, 1));
    // either have two or zero guard duties <- hard to implement

    // disappeared members cannot be Gnosias
    // if there are no engineers, disappeared members cannot be bugs
    // lying members are either Gnosias, a bug or an AC Follower
    // if a member accuses another, the two cannot be in the same camp; that is,
    //   they belong in two different groups from: 
    //   > (Crew Member, Doctor, Engineer, Guardian Angel, Guard Duty)
    //   > (AC Follower, Gnosia)
    //   > (Bug) 
    //    <- In other words, we cannot have two people from the same group.
    
    for (std::string rule : rules) {
        for (auto vv_name : gnosia_graph.get_all_variable_names()) {
            gnosia_graph.add_edge(vv_name, rule);
        }
    }   
};