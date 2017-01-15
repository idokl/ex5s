/*
 * This class represents set of nodes (vertices) and edges (edge is a connection
 * between pair of Nodes. the edges define the neighbors of every Node).
*/

#ifndef EX1_GRAPH_H
#define EX1_GRAPH_H

#include <iostream>
#include <queue>
#include "Node.h"

using namespace std;

template<class T>
class Graph {
public:
    //return queue that maintain all the Points that have a direct access to the Node p
    virtual queue<Node<T>> getNeighbors(Node<T> n)=0;

    virtual ~Graph() {}
};


#endif //EX1_GRAPH_H