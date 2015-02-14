/**
 *
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
        ~Node();

        void addEdge(int id, Node *node);

    private:
        int index;
        int low_index;
        map<int, Node*> edges;
};

#endif
