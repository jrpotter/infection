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
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

#include "sqlite3.h"
#include "node.hpp"
#include "student.hpp"
#include "index_stack.hpp"

typedef map<int, Node*>::iterator eItr;

class Graph
{
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

        // Reverts the nodes indices back to their original
        void reset();

        // Creates a strongly connected component representation
        // of the current graph, where data is now a list of the
        // nodes collected together
        Graph* getStrongComponents();

        // The actual connections; Note I do not bother making this private
        // since I would need to access the entire map anyways
        map<int, Node*> nodes;

    private:
        // The Tarjan Strongly Connected Components algorithm uses a stack to group nodes
        // of a connected equivalence class together. This is the recursive means of maintaining
        // the desired meta nodes
        void tarjanDFS(Node *current, Graph *metaGraph, IndexStack<Node*> *indexed);
};

#endif
