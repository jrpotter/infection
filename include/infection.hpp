/**
 * The following implements the requested two functions as specified by the
 * Khan Academy problem specification. It requires the two networks as 
 * demonstrated in "main.cpp".
 *
 * Author: Joshua Potter
 * Date: February 14th, 2015
*/
#ifndef INFECTION_HPP
#define INFECTION_HPP

#include <queue>
#include <cstdlib>
using namespace std;

#include "student.hpp"

class Infection
{
    public:
        Infection(Graph *network, Graph *metaNetwork);
        ~Infection();

        // Here we look through the meta graph and try to set it up so
        // that we get as close as possible to the desired limit of 
        // infected users
        Student* limit_infection(int limit);

        // Total infection begins at the node specified and "infects" all
        // connected nodes. We ensure that all nodes connected to the @user
        // will be given the same version. Note this is not necessarily
        // the latest version; merely the user's version
        void total_infection(Student *user);

    private:
        Graph *network;
        Graph *metaNetwork;

        // For use in limit_infection optimization
        queue<Node*> topologicalSort(Graph *graph);
        void topologicalDFS(Node *current, queue<Node*> *nodes);
};

#endif
