#include <gtest/gtest.h>
#include "../src/Point.h"
#include "../src/Graph.h"

class TestPoint: public ::testing::Test {
public:
    Point p1,p2,p3,p4;
    TestPoint() : p1(1,2) ,p2(1,2), p3(0,2), p4(1,0){};
};

//our tests:
TEST_F(TestPoint, SanityChecks) {
    // !=
    ASSERT_NE(p1,p3);
    ASSERT_NE(p1,p4);
    // ==
    ASSERT_EQ(p1,p1);
    ASSERT_EQ(p1,p2);
}

TEST_F(TestPoint, OrderingRelationsChecks) {
    // < asymmetry
    ASSERT_NE((p1<p3),(p3<p1));
    ASSERT_NE((p1<p4),(p4<p1));
    // < not reflexive
    ASSERT_EQ((p2<p1), false)
                   << "ERROR: These points are suuposed to be: p2 == p1 == Point(1,2)";
}

TEST_F(TestPoint, printing) {
    /*
    according to   http://stackoverflow.com/questions/31815230/
     how-to-test-input-and-output-overloaded-operator-in-c-gtest
     */
    // << (operator overloading of printing to stream)

    ostringstream output;
    output << p1;
    ASSERT_EQ(output.str(),"(1,2)");
}

TEST_F(TestPoint, isAvailable) {
    Point p = Point(0,0);
    //I assume that default Point has to be available
    ASSERT_EQ(p.isAvailable(), true) << "[not implemented (so far)]";
    p.setAvailability(false);
    ASSERT_EQ(p.isAvailable(), false) << "[not implemented (so far)]";
    p.setAvailability(true);
    ASSERT_EQ(p.isAvailable(), true) << "[not implemented (so far)]";
}
