/**
 * The following class represents a node of an adjaceny list graph.
 * Though I expect this to only be used for the student and j_student
 * table, I wanted to create something generic enough for extendibility
 * reasons.
 *
 * Author: Joshua Potter
 * Date: February 14, 2015
*/
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#include "sqlite3.h"
#include "node.hpp"
#include "meta_node.hpp"

template<typename T>
class Graph {

    public:
        Graph();
        ~Graph();

        // Creates a strongly connected component representation
        // of the current graph, where data is now a list of the
        // nodes collected together
        Graph<MetaNode*> getStrongComponents();

        // Add node to graph, indexed by id
        void addNode(int id, Node *node);

        // Add connection to graph, joining nodes by ids
        void connect(int head, int tail);

    private:
        map<int, Node*> nodes;
};


// Constructors/Destructor
// ==========================================================

template<typename T>
Graph<T>::Graph()
        :nodes({})
{

}

template<typename T>
Graph<T>::~Graph()
{
    for(unsigned int i = 0; i < nodes.size(); i++) {
        delete nodes[i];
    }
}


// Graph Definitions
// ==========================================================

template<typename T>
void Graph<T>::addNode(int id, Node *node)
{
    nodes.insert(pair<int, Node*>(id, node)); 
}

template<typename T>
void Graph<T>::connect(int head, int tail) 
{
    Node* start = nodes[head];
    start->addEdge(tail, nodes[tail]);
}

template<typename T>
Graph<MetaNode*> Graph<T>::getStrongComponents()
{

    return {};
}

#endif
