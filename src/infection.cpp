#include "infection.hpp"

// Constructors/Destructor
// ==========================================================

Infection::Infection(Graph *network, Graph *metaNetwork)
          :network(network)
          ,metaNetwork(metaNetwork)
{}

Infection::~Infection()
{}


// KA Infection Methods
// ==========================================================

// The following returns the node that allows the infection to get closest
// to the desired result. This may or may not be exactly the limit
Student* Infection::limit_infection(int limit)
{
    // The best starting node as of now
    int bestTotal = 0;
    Node *bestFit = nullptr;

    // We want to iterate through the nodes that can affect the least amount 
    // of people first, then work our way closer to the limit. We solve this
    // problem dynamically, building up the node count using the provided
    // index field.
    queue<Node*> nodes = topologicalSort(metaNetwork); 
    while(!nodes.empty() && bestTotal <= limit) {
        Node *front = nodes.front(); 

        // Add all indices of the other nodes, and see if closer to the limit
        front->index = front->subNodes.size();
        for(eItr itr = front->edges.begin(); itr != front->edges.end(); itr++) {
            front->index += itr->second->index;
        }

        // Pick the best total so far
        if(abs(limit-front->index) < abs(limit-bestTotal)) {
            bestFit = front;
            bestTotal = front->index;
        }

        nodes.pop();
    }

    if(bestFit != nullptr) {
        cout << "Student ID: " << bestFit->id << " infects " << bestTotal << endl;
    }

    return static_cast<Student*>(bestFit);
}

// The following changes the actual nodes in the graph to infect as many
// users as possible from the given user. We find the meta node corresponding 
// to the passed user's id, find all connected components, changing the 
// desired version while iterating
void Infection::total_infection(Student *user)
{
    // I wasn't really in the mood to write a recursive function for
    // this as well, so instead I'm using a queue. Note there isn't
    // any chance for a loop since the meta network is acyclic
    queue<Node*> metaNodes;
    metaNodes.push(metaNetwork->nodes[user->leader]);

    cout << "(";
    while(!metaNodes.empty()) {

        Node *meta = metaNodes.front();
        for(eItr itr = meta->edges.begin(); itr != meta->edges.end(); itr++) {
            if(itr->second != nullptr) {
                metaNodes.push(itr->second);
            }
        }

        for(int i = 0; i < meta->subNodes.size(); i++) {
            Student *student = static_cast<Student*>(meta->subNodes[i]);
            student->version = user->version;
            cout << student->id;
            if(metaNodes.size() != 1 || i != meta->subNodes.size() - 1) {
                cout << ", ";
            }
        }

        metaNodes.pop();
    }
    cout << ")" << endl;
}


// Convenience Methods
// ==========================================================

// We use a topological sort to properly go through each node in the order
// of least damage to most damage (in terms of the number of people infected).
queue<Node*> Infection::topologicalSort(Graph *graph)
{
    queue<Node*> nodes;

    for(eItr itr = graph->nodes.begin(); itr != graph->nodes.end(); itr++) {
        Node *next = itr->second;
        if(next->index == 0) {
            topologicalDFS(next, &nodes);
        }
    }

    graph->reset();

    return nodes;
}

// Recursive wrapper
void Infection::topologicalDFS(Node *current, queue<Node*> *nodes)
{
    current->index = 1;

    for(eItr itr = current->edges.begin(); itr != current->edges.end(); itr++) {
        Node *next = itr->second;
        if(next->index == 0) {
            topologicalDFS(next, nodes);
        }
    }

    nodes->push(current);
}

