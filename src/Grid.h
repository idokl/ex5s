/*
 * Grid is a graph represents matrix of points in the plane.
 * The matrix size is m*n (m columns and n rows).
 * Its first Point is (0,0). Its last Point is (m-1,n-1).
 */

#ifndef EX1_GRID_H
#define EX1_GRID_H

#include "Node.h"
#include "Graph.h"
#include "Point.h"
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Grid : public Graph<Point> {
private:
    int width;
    int height;
    vector<vector<Point>> vectorOfVerticalVectors;

public:
    //constructor. m is the grid width, n is the grid height.
    Grid(int m, int n, vector<Point> listOfObstacles);

    Grid(int m, int n);

    //return queue of Points in the grid that are next to the Node p (four points at most)
    //the neighbors order is: left, up, right and down. Don't return points that aren't available.
    queue<Node<Point>> getNeighbors(Node<Point> n);

    virtual ~Grid();
};


#endif //EX1_GRID_H
