// Author: Akira Kudo
// Description: Implements the "abstract" Vertex class for the CSP graph, which is passed 
//  to different methods. VariableVertex and ConstraintVertex for example inherit from this.
//  Saw here
//  [https://stackoverflow.com/questions/13989454/inheritance-from-empty-base-class-in-c] 
//  that the goal can be achieved by creating an empty class with pure virtual destructor.

#ifndef GRAPHIMPLEMENTATION_VERTICES_VERTEX_H
#define GRAPHIMPLEMENTATION_VERTICES_VERTEX_H

namespace GraphImplementation 
{
    class Vertex 
    {
    public:
        virtual ~Vertex() = 0; // declare pure virtual destructor
    };

};

#endif