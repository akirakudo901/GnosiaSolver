// Author: Akira Kudo
// Description: Defines a struct ARC holding data of an arc in the CSP Graph.
//  An ARC holds: 1) the main variable, 2) other variables linked to 1, and 
//  3) a constraint which links variables in 1 and 2.

#ifndef ARC_H
#define ARC_H

#include <iostream>
#include <set>
#include <vector>

#include "src/graphImplementation/vertices/ConstraintVertex.h"
#include "src/graphImplementation/vertices/VariableVertex.h"


namespace CSPSolverImplementation
{
    // a struct indicating an arc to be checked in arc consistency
    struct ARC
    {
        GraphImplementation::VariableVertex* main_var;
        std::vector<GraphImplementation::VariableVertex*> other_var_list; 
        GraphImplementation::ConstraintVertex* constraint;

        // overloading == so that we can use those in Boost tests
        bool operator==(const ARC& other) const
        {
            if (!(this->main_var == other.main_var)) return false;
            if (!(this->constraint == other.constraint)) return false;
            if (!(this->other_var_list.size()== other.other_var_list.size())) return false;
            for (size_t i=0; i < this->other_var_list.size(); i++) {
                if (this->other_var_list[i] != other.other_var_list[i]) return false;
            }
            return true;
        }
        
        // overloading << so that we can use those in Boost tests
        friend std::ostream& operator<<(std::ostream& os, const ARC& arc)
        {
            os << "Main var: " << *(arc.main_var) << "\n";
            os << "Other vars: ";
            for (auto& elem : arc.other_var_list) os << *(elem) << " ";
            os << "\n";
            os << "Constraint: " << *(arc.constraint) << "\n";
            return os;
        }
    };
}

#endif