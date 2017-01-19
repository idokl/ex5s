#ifndef EX1_VERSION_1_1_POINT_H
#define EX1_VERSION_1_1_POINT_H

#include "Node.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "easylogging++.h"


class Point {
private:
    int x;
    int y;
    bool isPassable;
public:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & x;
        ar & y;
    }

    Point();

    Point(int x, int y);

    //return the x coordinate of this Point
    const int getX() const;

    //return the y coordinate of this Point
    const int getY() const;

    //return true if this Point is passable.
    // false otherwise (=if there is an obstacle in this Point).
    bool isAvailable();

    //if this Point is passable (there isn't obstacle) return true. false otherwise.
    void setAvailability(bool isAvailable);

    //oprator overloading for printing point in the form (x,y)
    friend ostream &operator<<(ostream &os, const Point &point);

    //definition of ordering relation between Points in order to enable
    //data structures (like Map) to maintain them efficiently.
    //(the exact ordering relation, that we defined, doesn't matter)
    bool operator<(const Point &other) const;

    //points comparison according to their coordinates
    bool operator==(const Point &other) const;

    //points comparison according to their coordinates
    bool operator!=(const Point &other) const;
};

#endif //EX1_VERSION_1_1_POINT_H
