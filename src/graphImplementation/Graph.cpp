// Author: Akira Kudo

#include <algorithm>
#include <list>

#include "src/graphImplementation/edges/Edge.h"
#include "src/graphImplementation/Graph.h"
#include "src/graphImplementation/vertices/Vertex.h"

GraphImplementation::Graph::Graph()
{
    this->adjList = new std::list<Vertex*>();
};

GraphImplementation::Graph::~Graph()
{
    delete this->adjList;
};

// return if x and y are adjacent
bool GraphImplementation::Graph::adjacent(Vertex *x, Vertex *y)
{
    
    return (std::find(this->adjList.begin(), this->adjList.end(), y) != this->adjList.end())
};

// return all neighbors of x
std::list<GraphImplementation::Vertex*> GraphImplementation::Graph::neighbors(Vertex *x)
{

};

// adds vertex x to G
void GraphImplementation::Graph::add_vertex(Vertex *x)
{

};

// removes vertex x if there
void GraphImplementation::Graph::remove_vertex(Vertex *x)
{

};

// adds edge z from vertex x to y if not there
void GraphImplementation::Graph::add_edge(Vertex *x, Vertex *y, Edge z)
{

};

// removes edge from x to y if there
void GraphImplementation::Graph::remove_edge(Vertex *x, Vertex *y)
{

};