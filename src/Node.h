/*
 * This class represents vertex in a graph.
 * The node maintains a value (that is type is T).
 * the operators of the Node depends on the operators of its value.
*/

#ifndef EX1_NODE_H
#define EX1_NODE_H

#include <iostream>
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

using namespace std;

template<class T>
class Node;

template<class T>
class Node {
    T value;
public:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & value;
    }

    Node();
    Node(T value);

    //nodes comparison according to their values
    virtual bool operator==(const Node<T> &other) const;

    //definition of ordering relation between Nodes in order to enable
    //data structures (like Map) to maintain them efficiently.
    //(the exact ordering relation, that we defined, doesn't matter)
    virtual bool operator<(const Node<T> &other) const;

    //return the value is maintained in the node
    T getValue();
};

template<class T>
Node<T>::Node(T value) : value(value) {}

template<class T>
bool Node<T>::operator==(const Node<T> &other) const {
    return (this->value == other.value);
}

template<class T>
bool Node<T>::operator<(const Node<T> &other) const {
    return (this->value < other.value);
}

template<class T>
T Node<T>::getValue() {
    return this->value;
}
template<class T>

Node<T>::Node() {

}


#endif
