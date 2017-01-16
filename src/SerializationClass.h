#ifndef EX4_SERIALIZECLASS_H
#define EX4_SERIALIZECLASS_H
#include <string>
using namespace std;

template<class T>

class SerializationClass {

public:

    const string serializationObject(T object);

    const T deSerializationObject(string str, T object);
};


template<class T>
const string SerializationClass<T>::serializationObject(T object) {

    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);
    oa << object;
    s.flush();
    return serial_str;
}

template<class T>
const T SerializationClass<T>::deSerializationObject(string str, T object) {
    boost::iostreams::basic_array_source<char> device(str.c_str(), str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> object;
    return object;
}


#endif //EX4_SERIALIZECLASS_H
