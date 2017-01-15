#include <gtest/gtest.h>
#include "../src/Point.h"
#include "../src/Graph.h"
#include "../src/Grid.h"

class TestGrid: public ::testing::Test {
protected:
    int width;
    int height;
    Grid grid1;
    Graph<Point>* graph1;


    TestGrid() : width(2), height(3), grid1(width,height){
        grid1 = Grid(width,height);
        graph1 = new Grid(width,height);
    }

    virtual void TearDown() {
        delete graph1;
    }
 };


TEST_F(TestGrid, SanityChecks) {
    Point originPoint = Point(0,0);
    //Grid(2,3) shouldn't contain Point(2,1).
    Point invalidPoint = Point(2,1);
    Node<Point> n11(Point(0,1)), n12(Point(1,0));
    queue<Node<Point>> neighborsOfOriginPoint;
    ASSERT_EQ(grid1.getNeighbors(invalidPoint), neighborsOfOriginPoint)
                                << "ERROR: invalid point has neighbors";
    neighborsOfOriginPoint.push(n11);
    neighborsOfOriginPoint.push(n12);
    ASSERT_EQ(grid1.getNeighbors(originPoint), neighborsOfOriginPoint)
                                << "ERROR: neighbors of (0,0) aren't correct";
    ASSERT_EQ((*graph1).getNeighbors(originPoint), neighborsOfOriginPoint)
                                << "ERROR: neighbors of (0,0) in graph1 aren't correct";

    Point p2 = Point(0,1);
    Node<Point> n21(Point(0,2)), n22(Point(1,1)), n23(Point(0,0));
    queue<Node<Point>> neighborsOfP2;
    neighborsOfP2.push(n21);
    neighborsOfP2.push(n22);
    neighborsOfP2.push(n23);
    ASSERT_EQ(grid1.getNeighbors(p2), neighborsOfP2)
                                << "ERROR: neighbors of (0,1) aren't correct";


    Point p3 = Point(1,2);
    Node<Point> n31(Point(0,2)), n32(Point(1,1));
    queue<Node<Point>> neighborsOfP3;
    neighborsOfP3.push(n31);
    neighborsOfP3.push(n32);
    ASSERT_EQ(grid1.getNeighbors(p3), neighborsOfP3)
                        << "ERROR: neighbors of (1,2) aren't correct";
}

TEST_F(TestGrid, getNeighborsMethod) {
    //empty vector (so far)
    vector<Point> vectorOfObstaclesPoints;
    Grid checkedGrid = Grid(2,3,vectorOfObstaclesPoints);

    for (int i = -1; i < 4; i++) {
        for (int j = -1; j < 4; j++) {
            Point p = Point(i,j);
            ASSERT_EQ(checkedGrid.getNeighbors(p), grid1.getNeighbors(p))
             << "ERROR: the neighbors of the point in grid1 and in the checked grid aren't equal";
        }
    }

    Point p0_0 = Point(0,0);
    Point p0_1 = Point(0,1);
    Point p0_2 = Point(0,2);
    Point p1_0 = Point(1,0);
    Point p1_1 = Point(1,1);

    vectorOfObstaclesPoints.push_back(p0_0);
    checkedGrid = Grid(2,3,vectorOfObstaclesPoints);
    Node<Point> n1_1(p1_1);
    queue<Node<Point>> neighborsOfP1_0;
    neighborsOfP1_0.push(n1_1);
    ASSERT_EQ(checkedGrid.getNeighbors(p1_0), neighborsOfP1_0);

    //create "checkedGrid" with "vectorOfObstaclesPoints" = {(0,0),(0,1),(0,2)}
    vectorOfObstaclesPoints.push_back(p0_1);
    vectorOfObstaclesPoints.push_back(p0_2);
    checkedGrid = Grid(2,3,vectorOfObstaclesPoints);
    //make sure the neighbors' lists of all points in "checkedGrid" are different from
    //the neighbors' lists of the points in "grid1" that doesn't contain obstacles.
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            Point p = Point(i,j);
            ASSERT_NE(checkedGrid.getNeighbors(p), grid1.getNeighbors(p))
              << "ERROR: the neighbors of the point in grid1 and in the checked grid are equal";
        }
    }
}
