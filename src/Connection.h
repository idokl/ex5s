//
// Created by ohad on 12/01/17.
//

#ifndef EX5_CONNECTION_H
#define EX5_CONNECTION_H

#include <vector>
#include "pthread.h"
#include "Socket.h"

using namespace std;
class Connection {

private:
    int socketDescriptor;

    vector<int> vectorOfClientsDescriptor;

public:
    //Connection(int socketDescriptor, struct sockaddr_in client_sin, unsigned int *addr_len);

    Connection(int socketDescriptor);
    void makeConnect(void *);

    vector<int> getVectorOfClientsDescriptor();
    struct threadsStruct{
        void* socket;
        int socketDescriptor;
    };
};


#endif //EX5_CONNECTION_H
