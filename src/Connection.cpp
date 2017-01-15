//
// Created by ohad on 12/01/17.
//

#include "Socket.h"
#include "Connection.h"
#include "ProgramFlow.h"


void Connection::makeConnect(void * socket) {
    struct sockaddr_in client_sin;
    unsigned int addr_len;
    addr_len = sizeof(client_sin);
    cout << "accept" << endl;
    int descriptor = accept(this->socketDescriptor,
                            (struct sockaddr *) &client_sin, &addr_len);
    threadsStruct *threadsStructToSend;
    threadsStructToSend->socket = socket;
    threadsStructToSend->socketDescriptor = descriptor;
    pthread_t pthread;
    pthread_create(&pthread, NULL, ProgramFlow::threadsRun, (void*)threadsStructToSend);
//    this->vectorOfClientsDescriptor.push_back(descriptor);
    cout << "send" << endl;

}




vector<int> Connection::getVectorOfClientsDescriptor() {
    return this->vectorOfClientsDescriptor;
}

Connection::Connection(int socketDescriptor) {
    this->socketDescriptor = socketDescriptor;

}

