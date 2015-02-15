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

        // The unique id of the node in the graph
        int id;

        // Represents the parent id of the strongly
        // connected subtree
        int leader;

        // Used during Tarjan's algorithm
        int index, low_index;

        // The actual connections; Note I do not bother making this private
        // since I would need to access the entire map anyways
        map<int, Node*> edges;

        // A node can recursively contain other nodes. This is the vector
        // of those subnodes
        vector<Node*> subNodes;
};

#endif
