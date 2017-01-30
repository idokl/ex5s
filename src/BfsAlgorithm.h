/*
 * This Algorithm maintain graph that contains Nodes and edges.
 * It enables to find the shortest path between every 2 Nodes in the graph.
 * the BFS algorithm (=Breadth-first search) does it by building of tree that the startNode is
 * its root and its neighbors are its sons. the BFS find all the nodes that the distance between
 * them to the startNode is 1, then 2, then 3, etc.
 * When the BFS encounters the endNode, it stop and return stack with all nodes in the shortest
 * path between startNode to endNode.
 */

#ifndef EX1_BFS_H
#define EX1_BFS_H

using namespace std;

#include <map>
#include <queue>
#include <set>
#include <stack>
#include "Node.h"
#include "Grid.h"

template<class T>
class BfsAlgorithm {
private:
    //The graph that the BFS algorithm work on
    Graph<T> *graph;
    //constructor
public:
    BfsAlgorithm(Graph<T> *graph);

    //return stack that maintains the nodes in the shortest path from "start" to "end".
    //(if no path from start to end, return empty stack).
    stack<Node<T>> navigate(Node<T> &start, Node<T> &end);

    ~BfsAlgorithm() {}

};


template<class T>
BfsAlgorithm<T>::BfsAlgorithm(Graph<T> *graph) : graph(graph) {}


template<class T>
stack<Node<T>> BfsAlgorithm<T>::navigate(Node<T> &start, Node<T> &end) {
    //initialization of variables:
    //stack for the shortest path (between the given nodes) that this function will return
    stack<Node<T>> path = stack<Node<T>>();
    bool pathHasBeenFound = false;
    //queue that maintain nodes that are candidates to look for their neighbors
    //(in order to find access to new nodes)
    queue<Node<T>> nodesToLookForNeighbors = queue<Node<T>>();
    //set of nodes that have already been visited in this search
    //(we have already know the shortest path from the start Node to them)
//    set<Node<T>> visitedNodes = set<Node<T>>();
    //map that match to every visitedNode its parent in the tree that the BFS build
    map<Node<T>, Node<T>> parentOfNode = map<Node<T>, Node<T>>();
    //map that match to every visitedNode its distance from the start Node
//    map<Node<T>, int> distanceOfNode = map<Node<T>, int>();
    //(we could implement the function without the "visitedNodes" set and the "distanceOfNode"
    //map, but it might be useful in the future)

    //enter the start Node to the queue and start to build the search tree by it
//    distanceOfNode.insert(pair<Node<T>, int>(start, 0));
    nodesToLookForNeighbors.push(start);

    while (!nodesToLookForNeighbors.empty()) {
        //take out a Node from the queue and check its distance and its neighbors
        Node<T> current = nodesToLookForNeighbors.front();
        nodesToLookForNeighbors.pop();
//        int currentDistance = distanceOfNode.at(current);
        queue<Node<T>> currentNeighbors = graph->getNeighbors(current);
        //for each neighbor: check whether it has been visited.
        //if not, add it to our queue.
        while (!currentNeighbors.empty()) {
            Node<T> neighbor = currentNeighbors.front();
            currentNeighbors.pop();
//            bool theNeighborHasBeenVisited = (visitedNodes.find(neighbor) != visitedNodes.end());
            bool theNeighborHasBeenVisited = (parentOfNode.find(neighbor) != parentOfNode.end());
            if (!theNeighborHasBeenVisited) {
//                visitedNodes.insert(neighbor);
                parentOfNode.insert(pair<Node<T>, Node<T>>(neighbor, current));
//                distanceOfNode.insert(pair<Node<T>, int>(neighbor, currentDistance + 1));
                nodesToLookForNeighbors.push(neighbor);
            }
            //if we found the end Node, enter it to the stack with all its ancestors,
            //(and then break the loop and return this stack)
            if (neighbor == end) {
                pathHasBeenFound = true;
                path.push(neighbor);
                Node<T> previousNodeOfPath = neighbor;
                do {
                    previousNodeOfPath = parentOfNode.at(previousNodeOfPath);
                    path.push(previousNodeOfPath);
                } while (!(previousNodeOfPath == start));
                break;
            }
        }
        if (pathHasBeenFound)
            break;
    }
    return path;
}

#endif