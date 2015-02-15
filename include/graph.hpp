/**
 * The following is an adjacency list representation of a graph. It is the main
 * class to link students to one another, and has been generalized so a meta
 * graph (of strongly connected components) can be generated using the same
 * templated class.
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
#include <algorithm>
using namespace std;

#include "sqlite3.h"
#include "node.hpp"
#include "student.hpp"
#include "meta_node.hpp"
#include "index_stack.hpp"


// Graph Declaration
// ==========================================================

template<typename T>
class Graph {

    public:
        Graph();
        ~Graph();
        Graph(Graph& other);
        Graph& operator= (const Graph& other);
        Graph(Graph&& other);
        Graph& operator= (Graph&& other);

        // Add node to graph, indexed by id
        void addNode(int id, Node *node);

        // Add connection to graph, joining nodes by ids
        void connect(int head, int tail);

        // Creates a strongly connected component representation
        // of the current graph, where data is now a list of the
        // nodes collected together
        Graph<MetaNode*> getStrongComponents();

    private:
        map<int, Node*> nodes;

        // The Tarjan Strongly Connected Components algorithm uses a stack to group nodes
        // of a connected equivalence class together. This is the recursive means of maintaining
        // the desired meta nodes
        void tarjanDFS(Node *current, IndexStack<Node*> *indexed, Graph<MetaNode*> *metaGraph);
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

template<typename T>
Graph<T>::Graph(Graph<T>& other)
         :nodes(other.nodes)
{
    // TODO: Complete the copy constructor and assignment operator
    // The copy constructor is unique as we need to create
    // new nodes and maintain all the relationships established
    // so far. 
}

template<typename T>
Graph<T>& Graph<T>::operator= (const Graph<T>& other)
{
    if(this != &other) {
        nodes = other.nodes;
    }

    return *this;
}

template<typename T>
Graph<T>::Graph(Graph<T>&& other)
{

}

template<typename T>
Graph<T>& Graph<T>::operator= (Graph<T>&& other)
{

}


// Graph Building
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


// Graph Strong Components
// ==========================================================

template<typename T>
Graph<MetaNode*> Graph<T>::getStrongComponents()
{
    IndexStack<Node*> indexed;
    Graph<MetaNode*> metaGraph;

    // We continue iterating through nodes until all nodes have been seen
    // This is determined by verifying the indices of the nodes
    for(map<int, Node*>::iterator itr = nodes.begin(); itr != nodes.end(); itr++) {
        Node *current = itr->second;
        if(current->index == 0) {
            tarjanDFS(current, &indexed, &metaGraph);
        }
    }

    // We reset the graph to its original state. Though this is not necessarily
    // required, this will ensure repeated calls remain functional
    for(map<int, Node*>::iterator itr = nodes.begin(); itr != nodes.end(); itr++) {
        Node *current = itr->second;
        current->index = 0;
        current->low_index = 0;
    }

    return metaGraph;
}

// A recursive implementation of Tarjan's Strongly Connected Components Algorithm
// Details can be found here: http://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
template<typename T>
void Graph<T>::tarjanDFS(Node *current, IndexStack<Node*> *indexed, Graph<MetaNode*> *metaGraph)
{
    // This is the recursive base case, as once the index has been set,
    // we can ensure that the node has been seen.
    int index = indexed->push(current);
    current->index = current->low_index = index;

    // Loop through and recursively call tarjanDFS
    // Note the next node must be in the stack if we are to update our
    // low indices since otherwise the node has already been seen and grouped into
    // a strong component
    for(map<int, Node*>::iterator itr = current->edges.begin(); itr != current->edges.end(); itr++) {
        Node *next = itr->second;
        if(next->index == 0) {
            tarjanDFS(next, indexed, metaGraph);
        }
        if(indexed->find(next)) {
            current->low_index = min(next->low_index, current->low_index);
        }
    }

    // Build up strong components
    // Here we generate the meta node, setting the id
    // of the node to be the leader's id of the node
    if(current->index == current->low_index) {
        MetaNode *meta = new MetaNode();
        Node *next;
        do {
            next = indexed->pop();
            meta->addNode(next);
        } while(next->index != current->index);
        metaGraph->addNode(current->index, meta);
    }

}


#endif
