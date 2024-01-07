// Author: Akira Kudo
// Description: Implements variable vertices in our CSP graph corresponding to 
//  individual crew members in the game.

#ifndef GRAPHIMPLEMENTATION_VERTICES_VARIABLEVERTEX_H
#define GRAPHIMPLEMENTATION_VERTICES_VARIABLEVERTEX_H

#include <initializer_list>
#include <iostream>
#include <set>
#include <string>

#include "src/graphImplementation/vertices/Vertex.h"

namespace GraphImplementation
{
    class VariableVertex : public Vertex
    {

    private:
        // Name of vertex.
        std::string name;
        // Domains of vertex: are integers, combined accordingly with
        // enumerations which are specific to a problem we are solving.
        std::set<int> domain;

    public:
        VariableVertex(std::string name, std::initializer_list<int> initialDomain);
        VariableVertex(std::string name, std::set<int> initialDomain);
        ~VariableVertex();
        
        // Adds the value to the domain of the vertex, if not there already.
        void addToDomain(int val);
        void addToDomain(std::initializer_list<int> values);
        void addToDomain(std::set<int> values);
        // Removes the value from the domain of the vertex, if there already.
        void removeFromDomain(int val);
        void removeFromDomain(std::initializer_list<int> values);
        void removeFromDomain(std::set<int> values);

        // getters
        using Vertex::getName;
        std::set<int> getDomain() const { return this->domain; };

        // overloading == so that we can use those in Boost tests
        bool operator==(const VariableVertex& other) const
        {
            if (!this->Vertex::operator==(other)) return false;
            // otherwise, elementwise check if we have the same domain
            for (auto elem : this->domain) {
                bool was_found_in_other = false;
                for (auto elem_other : other.getDomain()) {
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
        bool operator!=(const VariableVertex& other) const
        {
            if (this->Vertex::operator!=(other)) return true;
            // otherwise, elementwise check if we have the same domain
            for (auto elem : this->domain) {
                bool was_found_in_other = false;
                for (auto elem_other : other.getDomain()) {
                    if (elem == elem_other) {
                        was_found_in_other = true;
                        break;
                    }
                }
                if (!was_found_in_other) return true;
            }
            return false;
        }

        // overload << operator
        friend std::ostream& operator<<(std::ostream& os, const VariableVertex& vv) {
            os << "NAME: " << vv.Vertex::name << "\n - Domain: { ";
            for (int elem : vv.domain) os << elem << " ";
            os << "}";
            return os;
        };
        
    };
}

#endif