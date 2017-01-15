#include "Point.h"

Point::Point() : x(0), y(0) {}

Point::Point(int x, int y) : x(x), y(y), isPassable(true) {}

int Point::getX() {
    return x;
}

int Point::getY() {
    return y;
}

bool Point::isAvailable() {
    return isPassable;
}


void Point::setAvailability(bool isAvailable) {
    isPassable = isAvailable;
}

bool Point::operator==(const Point &other) const {
    return (x == other.x) && (y == other.y);
}

bool Point::operator<(const Point &other) const {
    return (x < other.x) ||
           (x == other.x && (y < other.y));
}

ostream &operator<<(ostream &os, Point &point) {
    return os << "(" << point.getX() << "," << point.getY() << ")";
}

bool Point::operator!=(const Point &other) const {
    return !this->operator==(other);
}

