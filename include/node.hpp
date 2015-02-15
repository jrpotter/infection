/**
 * Represents the base class, used as the most atomic unit
 * in the Graph class.
 *
 * Author: Joshua Potter
 * Date: February 14th, 2015
*/
#ifndef NODE_HPP
#define NODE_HPP

#include <map>
#include <vector>
using namespace std;

class Node
{
    public:
        Node();
        Node(int id);
        ~Node();
        Node(const Node& other);
        Node& operator= (const Node& other);
        Node(Node&& other);
        Node& operator= (Node&& other);

        // Build up adjacency list
        void addEdge(int id, Node *node);

    protected:
        int id;
        int index;
        int low_index;
        map<int, Node*> edges;
};

#endif
