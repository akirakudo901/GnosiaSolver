// Author: Akira Kudo
// Description: Implements a CSP solver specifically for a setting of the game GNOSIA.
//   Provides a default set up for rules within GNOSIA, and allows you to provide situational
//   information such that it gradually solves the problem.

#include <string>

#include "src/cspSolver/CSPSolver.h"

namespace GnosiaSolverImplementation
{
    class GnosiaSolver : CSPSolverImplementation::CSPSolver
    {
    public:
        struct Roles
        {
            enum Role 
            { acFollower, bug, crewMember, doctor, engineer, 
              gnosia, guardianAngel, guardDuty, Role_MAX };
    
            std::string role_names[Role_MAX] = { 
                "AC Follower", "Bug", "Crew Member", "Doctor", 
                "Engineer", "Gnosia", "Guardian Angel", "Guard Duty"
                };

            // order has to match between strings and enumerations
            std::string get_string_from_enum(int n) {
                if (n < 0 || n >= Role_MAX) return "FOO";
                return role_names[n];
            }

            int get_enum_from_string(std::string role_string) {
                for (int i=0; i <= Role_MAX; i++)
                {
                    if (role_names[i] == role_string) return i;
                }
                return -1;
            }
        };

        GnosiaSolver();

    private:
        CSPSolverImplementation::CSPGraph gnosia_graph;
        
        // sets up the rules of GNosia as specified by constraints
        void GnosiaSolverImplementation::GnosiaSolver::rule_setup();

    };

}