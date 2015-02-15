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
     ,edges({})
     ,subNodes({})
{}

Node::~Node()
{}

Node::Node(const Node& other)
     :id(other.id)
     ,leader(other.leader)
     ,index(other.index)
     ,low_index(other.low_index)
     ,edges(other.edges)
     ,subNodes({})
{}

Node& Node::operator= (const Node& other)
{
    if(this != &other) {
        id = other.id;
        leader = other.leader;
        index = other.index;
        low_index = other.low_index;
        edges = other.edges;
        subNodes = other.subNodes;
    }

    return *this;
}

Node::Node(Node&& other)
     :id(other.id)
     ,leader(other.leader)
     ,index(other.index)
     ,low_index(other.low_index)
     ,edges(std::move(other.edges))
     ,subNodes(std::move(other.subNodes))
{}
       
Node& Node::operator= (Node&& other)
{
    if(this != &other) {
        id = other.id;
        leader = other.leader;
        index = other.index;
        low_index = other.low_index;
        edges = std::move(other.edges);
        subNodes = std::move(other.subNodes);
    }

    return *this;
}


// Collection Methods
// ==========================================================

void Node::addEdge(int id, Node *node)
{
    edges.insert(std::pair<int, Node*>(id, node));
}

