// Author: Akira Kudo
// Descrption: Implements a "crew member" in the game GNOSIA.
//  Inherits from a VariableVertex and has additional features in disposition.

#include <string>

namespace GnosiaSolverImplementation
{
    struct CrewMembers
    {
        enum CrewMember
        { Chipie, Comet, Gina, Jonas, Kukrushka, 
            Otome, Raqio, Remnan, Setsu, ShaMing, 
            Shigemichi, SQ, Stella, You, Yuriko, 
            CrewMember_MAX};
        
        std::string crew_member_names[CrewMember_MAX] = {
            "Chipie", "Comet", "Gina", "Jonas", "Kukrushka",
            "Otome", "Raqio", "Remnan", "Setsu", "ShaMing",
            "Shigemichi", "SQ", "Stella", "You", "Yuriko"
        };

        // order has to match between strings and enumerations
        std::string get_string_from_enum(int n) {
            if (n < 0 || n >= CrewMember_MAX) return "FOO";
            return crew_member_names[n];
        }

        // order has to match between strings and enumerations
        int get_enum_from_string(std::string role_string) {
            for (int i=0; i <= CrewMember_MAX; i++)
            {
                if (crew_member_names[i] == role_string) return i;
            }
            return -1;
        }
    };
};