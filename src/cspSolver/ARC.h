// Author: Akira Kudo
// Description: Defines a struct ARC holding data of an arc in the CSP Graph.
//  An ARC holds: 1) the main variable, 2) other variables linked to 1, and 
//  3) a constraint which links variables in 1 and 2.

#ifndef ARC_H
#define ARC_H

#include <algorithm>
#include <iostream>
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

        ARC() 
        {
            this->main_var = nullptr;
            this->other_var_list = std::vector<GraphImplementation::VariableVertex*>();
            this->constraint = nullptr;
        };

        std::string generate_unique_string()
        {
            std::string returned = main_var->getName() + "-";
            std::set<std::string> names;
            for (GraphImplementation::VariableVertex* vv : other_var_list)
            {
                names.insert(vv->getName());
            }
            for (std::string name : names)
            {
                returned += name + "-";
            }
            
            return returned += constraint->getName();
        };

        // overloading == so that we can use those in Boost tests
        bool operator==(const ARC& other) const
        {
            if (this->main_var != other.main_var) return false;
            if (this->constraint != other.constraint) return false;
            // elementwise check if we have the same domain
            for (auto elem : this->other_var_list) {
                bool was_found_in_other = false;
                for (auto elem_other : other.other_var_list) {
                    if (elem == elem_other) {
                        was_found_in_other = true;
                        break;
                    }
                }
                if (!was_found_in_other) return false;
            }
            return true;
        }

        // overloading != as well
        bool operator!=(const ARC& other) const
        {
            if (this->main_var != other.main_var) return true;
            if (this->constraint != other.constraint) return true;
            // elementwise check if we have the same domain
            for (auto elem : this->other_var_list) {
                bool was_found_in_other = false;
                for (auto elem_other : other.other_var_list) {
                    if (elem == elem_other) {
                        was_found_in_other = true;
                        break;
                    }
                }
                if (!was_found_in_other) return true;
            }
            return false;
        }
        
        // overloading << so that we can use those in Boost tests
        friend std::ostream& operator<<(std::ostream& os, const ARC& arc)
        {
            os << "Main var: " << std::endl; 
            os << *(arc.main_var) << "\n";
            os << "Other vars: " << std::endl;
            for (auto& elem : arc.other_var_list) os << *(elem) << "\n";
            os << "\n";
            os << "Constraint: " << std::endl;
            os << *(arc.constraint) << "\n";
            return os;
        }
    };
}

#endif