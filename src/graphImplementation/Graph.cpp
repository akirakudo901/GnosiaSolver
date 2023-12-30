// Author: Akira Kudo

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "src/graphImplementation/edges/Edge.h"
#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/Vertex.h"

GraphImplementation::Graph::Graph()
{
    this->adjList = std::unordered_map<Vertex*, std::vector<Vertex*>>();
};

GraphImplementation::Graph::~Graph()
{

};

// returns whether vertex x is in the graph
bool GraphImplementation::Graph::contains_vertex(Vertex& x)
{
    return (adjList.find(&x) != adjList.end());
};

// return if x and y are adjacent
bool GraphImplementation::Graph::adjacent(Vertex& x, Vertex& y)
{
    // if x or y aren't contained in graph, return false
    if (!contains_vertex(x) || !contains_vertex(y)) return false;
    // if x and y point to the same memory address, return false - we don't allow loops in the graph
    if (&x == &y) return false;
    // otherwise
    std::vector<Vertex*> xAdjList = adjList[&x];
    for (auto element : xAdjList)
    {
        if (element == &y) return true;
    }
    return false;
};

// return all neighbors of x
std::vector<GraphImplementation::Vertex*> GraphImplementation::Graph::neighbors(Vertex& x)
{
    // if x isn't in the graph yet, just return an empty vector
    if (!contains_vertex(x))
        return std::vector<Vertex*>();
    else 
        return adjList[&x];
};

// adds vertex x to G
void GraphImplementation::Graph::add_vertex(Vertex& x)
{
    // if x is contained in graph, skip
    if (contains_vertex(x)) return;
    adjList[&x] = std::vector<Vertex*>();
};

// removes vertex x if there, as well as edges connected to x
void GraphImplementation::Graph::remove_vertex(Vertex& x)
{
    // if x isn't contained in this graph, skip
    if (!contains_vertex(x)) return;
    // otherwise get the adjacency list of x
    std::vector<Vertex*> xAdjList = adjList[&x];
    // remove corresponding edges from the other vertex' adjacency list
    for (Vertex* adjVrtx : xAdjList) remove_edge(x, *adjVrtx);
    // remove entry of x from adjList
    adjList.erase(&x);
};

// adds edge z from vertex x to y if not there
void GraphImplementation::Graph::add_edge(Vertex &x, Vertex &y, Edge z)
{
    // if either of x and y aren't in the graph, skip
    if (!contains_vertex(x) || !contains_vertex(y)) return;
    // if x and y are the same vertex, skip
    if (&x == &y) return;
    // if the edge is already there, skip
    if (adjacent(x, y)) return;
    // otherwise, add the edge. Currenctly, we won't use the argument z
    adjList[&x].push_back(&y);
    adjList[&y].push_back(&x);
};

// removes edge from x to y if there
void GraphImplementation::Graph::remove_edge(Vertex& x, Vertex& y)
{
    auto findAndRemoveEntryFromAdjacencyList = [](std::vector<Vertex*> *lst, Vertex *toRemove)
    {
        // find the position of the vertex we are trying to remove in the adjacency list
        auto toErase = std::find(lst->begin(), lst->end(), toRemove);
        // remove it if it is found
        if (toErase != lst->end()) lst->erase(toErase);
    };

    // if either of x and y aren't in the graph, skip
    if (!contains_vertex(x) || !contains_vertex(y)) return;
    // if x and y are the same vertex, skip
    if (&x == &y) return;
    // otherwise get the adjacency list of x and remove the corresponding edge
    findAndRemoveEntryFromAdjacencyList(&adjList[&x], &y);
    // DO THE EXACT SAME FOR y
    findAndRemoveEntryFromAdjacencyList(&adjList[&y], &x);
};