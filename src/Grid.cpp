#include "Grid.h"

using namespace std;

Grid::Grid(int m, int n) {
    width = m;
    height = n;
    for (int i = 0; i < width; i++) {
        vector<Point> verticalVectorOfPoints = vector<Point>();
        for (int j = 0; j < height; j++) {
            Point p = Point(i, j);
            verticalVectorOfPoints.push_back(p);
        }
        vectorOfVerticalVectors.push_back(verticalVectorOfPoints);
    }
};

Grid::Grid(int m, int n, vector<Point> listOfObstacles) {
    width = m;
    height = n;
    for (int i = 0; i < width; i++) {
        vector<Point> verticalVectorOfPoints = vector<Point>();
        for (int j = 0; j < height; j++) {
            Point p = Point(i, j);
            verticalVectorOfPoints.push_back(p);
        }
        vectorOfVerticalVectors.push_back(verticalVectorOfPoints);
    }
    //TBD - setAvailable(true) for each point in the grid that belongs to listOfObstacles
    while (!listOfObstacles.empty()) {
        Point obstacle = listOfObstacles.back();
        listOfObstacles.pop_back();
        (vectorOfVerticalVectors[obstacle.getX()][obstacle.getY()]).setAvailability(false);
    }
};

queue<Node<Point>> Grid::getNeighbors(Node<Point> n) {
    Point p = n.getValue();
    int x = p.getX();
    int y = p.getY();
    queue<Node<Point>> neighbors = queue<Node<Point>>();
    //if the point isn't in the graph, return null queue
    if ((x < 0) || (y < 0) || (x >= width) || (y >= height))
        return neighbors;
    if ((x > 0) && vectorOfVerticalVectors[x - 1][y].isAvailable())
        neighbors.push(this->vectorOfVerticalVectors[x - 1][y]);
    if ((y + 1 < height) && vectorOfVerticalVectors[x][y + 1].isAvailable())
        neighbors.push(this->vectorOfVerticalVectors[x][y + 1]);
    if ((x + 1 < width) && vectorOfVerticalVectors[x + 1][y].isAvailable())
        neighbors.push(this->vectorOfVerticalVectors[x + 1][y]);
    if ((y > 0) && vectorOfVerticalVectors[x][y - 1].isAvailable())
        neighbors.push(this->vectorOfVerticalVectors[x][y - 1]);
    return neighbors;
}

Grid::~Grid() {
}
