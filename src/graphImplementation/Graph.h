// Author: Akira Kudo
// Description: A program that implements a graph object used to create the
// CSP graphs.

// Found Boost Graph Library online, but 
// Reddit [https://www.reddit.com/r/cpp/comments/vyt4t5/boostgraph_user_survey/]
// seems to suggest that for easy tasks, it doesn't hurt to write your own code.
// So I will do just that.

#ifndef GRAPHIMPLEMENTATION_GRAPH_H
#define GRAPHIMPLEMENTATION_GRAPH_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "src/graphImplementation/edges/Edge.h"
#include "src/graphImplementation/vertices/Vertex.h"

namespace GraphImplementation
{
    class Graph 
    {

    protected:
        //use adjacency list:
        // faster than adjacency matrix & edge list in checking adjacency 
        //-> best in adding "ToCheck" arcs for arc consistency
        
        // we will implement this using unordered_map (hash map) of vertex pointers
        // to their corresponding vector
        // I want quick access to a "Vertex" object's registered neighbor nodes
        // as uniquely determined by its memory address 
        std::unordered_map<Vertex*, std::vector<Vertex*>> adjList;

        // returns all edges of this graph as edge list into ostream
        std::ostream& return_edge_list_in_ostream(std::ostream& os) const;
        
    public:
        Graph();
        ~Graph();

        // returns whether vertex x is in the graph
        bool contains_vertex(Vertex& x);
        // return if vertices x and y are adjacent
        bool adjacent(Vertex& x, Vertex& y);
        // return all neighbors of vertex x
        std::vector<Vertex*> neighbors(Vertex& x);
        // adds vertex x to the graph
        void add_vertex(Vertex& x);
        // removes vertex x if there, as well as edges connected to x
        void remove_vertex(Vertex& x);
        // adds edge z from vertex x to y if not there
        void add_edge(Vertex &x, Vertex &y, Edge z);
        // removes edge from x to y if there
        void remove_edge(Vertex &x, Vertex &y);

        // overload << operator
        friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
            // to the stream, pass every vertices contained in this Graph
            os << "#######################\n";
            os << "All contained vertices.\n";
            os << "#######################\n";
            for (auto elem : g.adjList) os << " - " << elem.first->getName() << "\n";
            os << "\n";
            // then all the edges from the edge list
            g.return_edge_list_in_ostream(os);
            return os;
        };
    };
}

#endif