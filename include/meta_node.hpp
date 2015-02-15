/**
 * The meta node serves merely as a container of other nodes.
 * It is useful for composing a strongly connected graph and
 * maintaining a count of how many other nodes it represents.
 *
 * Author: Joshua Potter
 * Date: February 14, 2015
*/
#ifndef META_NODE_HPP
#define META_NODE_HPP

#include "node.hpp"

class MetaNode : public Node
{
    public:
        MetaNode();
        ~MetaNode();
        MetaNode(const MetaNode& other);
        MetaNode& operator= (const MetaNode& other);
        MetaNode(MetaNode&& other);
        MetaNode& operator= (MetaNode&& other);

        void addNode(Node *node);

    private:
        vector<Node*> nodes;
};

#endif
