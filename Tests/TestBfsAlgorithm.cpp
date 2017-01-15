#include "gtest/gtest.h"
#include "../src/BfsAlgorithm.h"

TEST(TestBfsAlgorithm,checkRoutes){
    vector<Point> listOfObstacles = vector<Point>();
    Grid *grid = new Grid(10, 10, listOfObstacles);
    BfsAlgorithm<Point> bfs = BfsAlgorithm<Point>(grid);

    Node<Point> startPoint(Point(0, 0));
    Node<Point> endPoint(Point(5,5));
    stack<Node<Point>> bfsOutput = bfs.navigate(startPoint,endPoint);
    Node<Point> startPoint2(Point(0, 0));
    Node<Point> endPoint2(Point(2,2));
    stack<Node<Point>> bfsOutput2 = bfs.navigate(startPoint2,endPoint2);

    stack<Node<Point>> bfsTest;
    bfsTest.push((Point(5,5)));bfsTest.push((Point(5,4)));bfsTest.push((Point(3,5)));
    bfsTest.push((Point(2,5)));bfsTest.push((Point(1,5)));bfsTest.push((Point(0,5)));
    bfsTest.push((Point(0,4)));bfsTest.push((Point(0,3)));bfsTest.push((Point(0,2)));
    bfsTest.push((Point(0,1)));bfsTest.push((Point(0,0)));

    stack<Node<Point>> bfsTest2;
    bfsTest2.push((Point(2,3)));bfsTest2.push((Point(2,3)));bfsTest2.push((Point(2,3)));
    bfsTest2.push((Point(2,3)));bfsTest2.push((Point(2,3)));


    for(int i=0; i<bfsOutput.size(); i++) {
        EXPECT_EQ(bfsOutput.top().getValue(), bfsTest.top().getValue())
                            << "compare with the correct route";
        bfsOutput.pop();
        bfsTest.pop();
    }
    for(int i=0; i<bfsOutput2.size(); i++) {
        EXPECT_NE(bfsOutput2.top().getValue(), bfsTest2.top().getValue()) << "compare with "
                            "incorrect route";
        bfsOutput2.pop();
        bfsTest2.pop();
    }

    delete grid;
}