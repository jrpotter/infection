#include "node.hpp"

// Constructors/Destructor
// ==========================================================

Node::Node()
{
    Node(0);
}

Node::Node(int id)
     :id(id)
     ,edges({})
     ,index(0)
     ,low_index(0)
{}

Node::~Node()
{}

Node::Node(const Node& other)
     :id(other.id)
     ,index(other.index)
     ,low_index(other.low_index)
     ,edges(other.edges)
{}

Node& Node::operator= (const Node& other)
{
    if(this != &other) {
        id = other.id;
        index = other.index;
        low_index = other.low_index;
        edges = other.edges;
    }

    return *this;
}

Node::Node(Node&& other)
     :id(other.id)
     ,index(other.index)
     ,low_index(other.low_index)
     ,edges(std::move(other.edges))
{}
       
Node& Node::operator= (Node&& other)
{
    if(this != &other) {
        id = other.id;
        index = other.index;
        low_index = other.low_index;
        edges = std::move(other.edges);
    }

    return *this;
}


// Collection Methods
// ==========================================================

void Node::addEdge(int id, Node *node)
{
    edges.insert(std::pair<int, Node*>(id, node));
}

