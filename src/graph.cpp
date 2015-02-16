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
    nodes[id] = node;
}

void Graph::connect(int head, int tail) 
{
    Node* start = nodes[head];
    start->edges[tail] = nodes[tail];
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
    // Variables to faithfully follow Tarjan's algorithm
    IndexStack<Node*> indexed;
    Graph *metaGraph = new Graph();

    // We continue iterating through nodes until all nodes have been seen
    // This is determined by verifying the indices of the nodes
    for(eItr itr = nodes.begin(); itr != nodes.end(); itr++) {
        Node *current = itr->second;
        if(current->index == 0) {
            tarjanDFS(current, metaGraph, &indexed);
        }
    }

    // We can now loop through all the nodes of a meta node, and identify
    // which meta node each edge points to via the leader of each node
    vector<int> test;
    for(eItr itr = nodes.begin(); itr != nodes.end(); itr++) {
        Node *fst = itr->second;
        for(eItr jtr = fst->edges.begin(); jtr != fst->edges.end(); jtr++) {
            Node *snd = jtr->second;
            if(fst->leader != snd->leader) {
                metaGraph->connect(fst->leader, jtr->second->leader);
            }
        }
    }

    // Revert to original state
    reset();

    return metaGraph;
}

// A recursive implementation of Tarjan's Strongly Connected Components Algorithm
// Details can be found here: http://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
void Graph::tarjanDFS(Node *current, Graph *metaGraph, IndexStack<Node*> *indexed)
{
    // This is the recursive base case, as once the index has been set,
    // we can ensure that the node has been seen.
    int index = indexed->push(current);
    current->index = current->low_index = index;

    // Loop through and recursively call tarjanDFS
    for(eItr itr = current->edges.begin(); itr != current->edges.end(); itr++) {
        Node *next = itr->second;
        if(next->index == 0) {
            tarjanDFS(next, metaGraph, indexed);
        }

        // Found potential leader of the component
        if(indexed->find(next)) {
            current->low_index = min(next->low_index, current->low_index);
        }
    }

    // If we have reached the bottom, then we need to build the meta node in
    // question and then connect the graph according to the metaEdges. Note
    // this should always work since the meta graph is acyclic, and any nodes
    // outside of this one will have been accessed when traversing the components.
    if(current->index == current->low_index) {
        Node *next;
        Node *meta = new Node(current->id);
        do {
            next = indexed->pop();
            next->leader = current->id;
            meta->subNodes.push_back(next);
        } while(next->index != current->index);
        metaGraph->addNode(current->id, meta);
    }

}

