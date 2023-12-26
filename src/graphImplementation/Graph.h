// Author: Akira Kudo
// Description: A program that implements a graph object used to create the
// CSP graphs.

// Found Boost Graph Library online, but 
// Reddit [https://www.reddit.com/r/cpp/comments/vyt4t5/boostgraph_user_survey/]
// seems to suggest that for easy tasks, it doesn't hurt to write your own code.
// So I will do just that.

#ifndef GRAPHIMPLEMENTATION_GRAPH_H
#define GRAPHIMPLEMENTATION_GRAPH_H

#include <list>

#include "src/graphImplementation/edges/Edge.h"
#include "src/graphImplementation/vertices/Vertex.h"

namespace GraphImplementation
{
    class Graph 
    {

    private:
        //use adjacency list:
        // faster than adjacency matrix & edge list in checking adjacency 
        //-> useful in adding "ToCheck" arcs for arc consistency
        struct AdjacencyList
        {
            std::set<Vertex*> 
            std::list<Vertex*> *allVertices;
            std::
            
        };
        // actual data holding adjList
        AdjacencyList adjList;
        
    public:
        Graph();
        ~Graph();

        // return if x and y are adjacent
        bool adjacent(Vertex *x, Vertex *y);
        // return all neighbors of x
        std::list<Vertex*> neighbors(Vertex *x);
        // adds vertex x to G
        void add_vertex(Vertex *x);
        // removes vertex x if there
        void remove_vertex(Vertex *x);
        // adds edge z from vertex x to y if not there
        void add_edge(Vertex *x, Vertex *y, Edge z);
        // removes edge from x to y if there
        void remove_edge(Vertex *x, Vertex *y);        
    };
}

#endif