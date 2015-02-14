#ifndef META_NODE_HPP
#define META_NODE_HPP

#include "node.hpp"

class MetaNode : public Node
{
    public:
        MetaNode();
        ~MetaNode();

    private:
        vector<Node*> nodes;
};

#endif
