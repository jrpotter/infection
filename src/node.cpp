#include "node.hpp"

// Constructors/Destructor
// ==========================================================

Node::Node()
     :edges({})
     ,index(0)
     ,low_index(0)
{

}

Node::~Node()
{

}

void Node::addEdge(int id, Node *node)
{
    edges.insert(std::pair<int, Node*>(id, node));
}


