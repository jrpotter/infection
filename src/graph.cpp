#include "graph.hpp"

// Constructors/Destructor
// ==========================================================

Graph::Graph()
      :nodes({})
{}

Graph::~Graph()
{
    for(unsigned int i = 0; i < nodes.size(); i++) {
        delete nodes[i];
    }
}

Graph::Graph(Graph& other)
      :nodes(other.nodes)
{
    // TODO: Complete the copy constructor and assignment operator
    // The copy constructor is unique as we need to create
    // new nodes and maintain all the relationships established
    // so far. 
}

Graph& Graph::operator= (const Graph& other)
{
    if(this != &other) {
        nodes = other.nodes;
    }

    return *this;
}

Graph::Graph(Graph&& other)
      :nodes(other.nodes)
{

}

Graph& Graph::operator= (Graph&& other)
{
    if(this != &other) {
        nodes = other.nodes;
    }

    return *this;
}


// Graph Building
// ==========================================================

void Graph::addNode(int id, Node *node)
{
    nodes.insert(pair<int, Node*>(id, node)); 
}

void Graph::connect(int head, int tail) 
{
    Node* start = nodes[head];
    start->addEdge(tail, nodes[tail]);
}

// We reset the graph to its original state. Though this is not necessarily
// required, this will ensure repeated calls remain functional
void Graph::reset()
{
    for(eItr itr = nodes.begin(); itr != nodes.end(); itr++) {
        Node *current = itr->second;
        current->index = 0;
        current->low_index = 0;
    }
}


// Graph Strong Components
// ==========================================================

Graph* Graph::getStrongComponents()
{
    IndexStack<Node*> indexed;
    Graph *metaGraph = new Graph();

    // We continue iterating through nodes until all nodes have been seen
    // This is determined by verifying the indices of the nodes
    for(eItr itr = nodes.begin(); itr != nodes.end(); itr++) {
        Node *current = itr->second;
        if(current->index == 0) {
            tarjanDFS(current, &indexed, metaGraph);
        }
    }

    reset();

    return metaGraph;
}

// A recursive implementation of Tarjan's Strongly Connected Components Algorithm
// Details can be found here: http://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
vector<Node*> Graph::tarjanDFS(Node *current, IndexStack<Node*> *indexed, Graph *metaGraph)
{
    // This is the recursive base case, as once the index has been set,
    // we can ensure that the node has been seen.
    int index = indexed->push(current);
    current->index = current->low_index = index;

    // When creating the meta nodes, we do not necessarily know what the meta nodes will
    // be linked to in the meta graph. To resolve this, when creating a strong component
    // we set all nodes in the component to have a common leader. We can then loop
    // through the nodes that have been seen but do not belong to the current component;
    // its leader must be one of the meta nodes the current meta node points to.
    vector<Node*> metaEdges;

    // Loop through and recursively call tarjanDFS
    for(eItr itr = current->edges.begin(); itr != current->edges.end(); itr++) {
        Node *next = itr->second;
        if(next->index == 0) {
            vector<Node*> tmp = tarjanDFS(next, indexed, metaGraph);
            metaEdges.insert(metaEdges.begin(), tmp.begin(), tmp.end());
        }

        // Found potential leader of the component
        if(indexed->find(next)) {
            current->low_index = min(next->low_index, current->low_index);

        // Otherwise found another connection; should try to make connection
        // The seen index has already been grouped into a different component
        } else {
            metaEdges.push_back(next);
        }

    }

    // Build up strong components
    // Since we are not at the end of the current strong component, we can just
    // keep track of which nodes we've seen outside of the current component.
    // This will allow us to know which meta node we have to connect to later.
    if(current->index != current->low_index) {

        return metaEdges;

    // If we have reached the bottom, then we need to build the meta node in
    // question and then connect the graph according to the metaEdges. Note
    // this should always work since the meta graph is acyclic, and any nodes
    // outside of this one will have been accessed when traversing the components.
    } else {

        Node *meta = new Node(current->id);

        // Here we generate the meta node, setting the id of the node to be the
        // leader's id of the node. We can now look at the 
        Node *next;
        do {
            next = indexed->pop();
            next->leader = current->id;
            meta->subNodes.push_back(next);
        } while(next->index != current->index);

        // Now we build the connections
        metaGraph->addNode(current->index, meta);
        for(int i = 0; i < metaEdges.size(); i++) {
            metaGraph->connect(current->index, metaEdges[i]->leader);
        }
    }

    // Empty! Completed a connected component
    return vector<Node*>();
}

