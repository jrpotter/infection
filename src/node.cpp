#include "node.hpp"

// Constructors/Destructor
// ==========================================================

Node::Node()
{
    Node(0);
}

Node::Node(int id)
     :id(id)
     ,leader(0)
     ,index(0)
     ,low_index(0)
{}

