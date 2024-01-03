// Author: Akira Kudo
// Description: Implements the "abstract" Vertex class for the CSP graph, which is passed 
//  to different methods. VariableVertex and ConstraintVertex for example inherit from this.
//  Saw here
//  [https://stackoverflow.com/questions/13989454/inheritance-from-empty-base-class-in-c] 
//  that the goal can be achieved by creating an empty class with pure virtual destructor.

#ifndef GRAPHIMPLEMENTATION_VERTICES_VERTEX_H
#define GRAPHIMPLEMENTATION_VERTICES_VERTEX_H

#include <iostream>
#include <string>

namespace GraphImplementation 
{
    class Vertex 
    {
    protected:
        std::string name;
    public:
        Vertex(std::string name) : name(name) {};
        virtual ~Vertex() = 0; // declare pure virtual destructor
        std::string getName() const { return this->name; };

        // overload operator<<
        friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
            os << "NAME: " << v.name;
            return os;
        };
    };

};

#endif