/************************************************************
* File description: TCP implementation.						*
* the class inherit from socket. 							*
* methods of tcp socket type								*
************************************************************/

#include <boost/iostreams/close.hpp>
#include "Tcp.h"

/***********************************************************************
* function name: Tcp												   *
* The Input: Boolean, true - if server, false if client	and port number*
* The output: none										               *
* The Function operation: creating new Tcp. initialize if server and   *
* port_num by the input												   *
***********************************************************************/
Tcp::Tcp(bool isServers, int port_num, string ip) {
    this->descriptorCommunicateClient = 0;
    this->port_number = port_num;
    this->isServer = isServers;
    this->ip_address = ip;
    this->listOfDescriptors = vector<int>();

}

/***********************************************************************
* function name: ~Tcp												   *
* The Input: none													   *
* The output: none										               *
* The Function operation: default destructor					       *
***********************************************************************/
Tcp::~Tcp() {
    unsigned long size = this->listOfDescriptors.size();
    for (unsigned long i = 0; i < size; i++) {
        close(listOfDescriptors.at(i));
    }
}

/***********************************************************************
* function name: initialize											   *
* The Input: none              										   *
* The output: int number representing the return status		           *
* The Function operation: initialize the Socket object by getting	   *
* socket descriptor. bind and accept for servers or connect for clients*
***********************************************************************/
int Tcp::initialize() {
    //getting a socket descriptor and check if legal
    this->socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socketDescriptor < 0) {
        //return an error represent error at this method
        return ERROR_SOCKET;
    }
    //if server
    if (this->isServer) {
        //initialize the struct
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(this->port_number);
        bzero(&(sin.sin_zero), 8);
        //bind
        if (::bind(this->socketDescriptor,
                   (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            //return an error represent error at this method
            return ERROR_BIND;
        }

        //listen
        if (listen(this->socketDescriptor, this->backLog) < 0) {
            //return an error represent error at this method
            return ERROR_LISTEN;
        }
        if (this->descriptorCommunicateClient < 0) {
            //return an error represent error at this method
            return ERROR_ACCEPT;
        }
        //if client
    } else {
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr((this->ip_address).c_str());
        sin.sin_port = htons(this->port_number);
        if (connect(this->socketDescriptor,
                    (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            //return an error represent error at this method
            return ERROR_CONNECT;
        }
    }
    //return correct if there were no problem
    return CORRECT;
}

/***********************************************************************
* function name: sendData											   *
* The Input: string data to send									   *
* The output: int number representing the return status		           *
* The Function operation: sending the required data, using his length  *
* and the socket descroptor											   *
***********************************************************************/
int Tcp::sendData(string data, int descriptorCommunicateClient) {
    int data_len = data.length() + 1;
    const char *datas = data.c_str();
    int sent_bytes = send(this->isServer ? descriptorCommunicateClient
                                         : this->socketDescriptor, datas, data_len, 0);
    if (sent_bytes < 0) {
        //return an error represent error at this method
        return ERROR_SEND;
    }
    //return correct if there were no problem
    return CORRECT;
}

/***********************************************************************
* function name: recive	`											   *
* The Input: none										               *
* The output: int number representing the return status	               *
* The Function operation: getting data from the other socket to,	   *
* enter it to the buffer and print the data							   *
***********************************************************************/
int Tcp::reciveData(char *buffer, int size, int descriptorCommunicateClient) {
    int read_bytes = recv(this->isServer ? descriptorCommunicateClient
                                         : this->socketDescriptor, buffer, size, 0);
    //checking the errors
    if (read_bytes == 0) {
        return CONNECTION_CLOSED;
    } else if (read_bytes < 0) {
        //return an error represent error at this method
        return ERROR_RECIVE;
    }
    //return correct if there were no problem
    return read_bytes;
}

int Tcp::getSocketDescriptor() {
    return this->socketDescriptor;
}

vector<int> Tcp::getListOfDescriptors() {
    return this->listOfDescriptors;
}