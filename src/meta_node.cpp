#include "meta_node.hpp"

// Constructors/Destructor
// ==========================================================

MetaNode::MetaNode()
         :nodes({})
{}

MetaNode::~MetaNode()
{}

MetaNode::MetaNode(const MetaNode& other)
         :nodes(other.nodes)
{}

MetaNode& MetaNode::operator= (const MetaNode& other)
{
    if(this != &other) {
        nodes = other.nodes;
    }

    return *this;
}

MetaNode::MetaNode(MetaNode&& other)
         :nodes(other.nodes)
{
    other.nodes.clear();
}
       
MetaNode& MetaNode::operator= (MetaNode&& other)
{
    if(this != &other) {
        nodes = other.nodes;
        other.nodes.clear();
    }

    return *this;
}


// Collection Methods
// ==========================================================

void MetaNode::addNode(Node *node)
{
    nodes.push_back(node);
}

