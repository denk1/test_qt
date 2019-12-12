#ifndef CONNECTIONCONTROLTCP_H
#define CONNECTIONCONTROLTCP_H
#include "ITS.h"
#include "Controlling.h"
#include "ConnectionControlInterface.h"
#include "ThreadSafeData.h"
#include  <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


namespace  RAT {

class ConnectionControlTCP : public  ConnectionControlInterface
{
public:
    ConnectionControlTCP(ITS* ptrITS) ;
    ~ConnectionControlTCP();
    void run();
    void sendTelemetry(unsigned char* msg);
private:
    void *get_in_addr(struct sockaddr *sa);
    void set_timer(int t);
    ThreadSafeData mThreadSafeData;
    Controlling mControlling;
    int sockfd;
    int new_fd;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    long int angle;
    const char* PORT = "8001";
    const int BACKLOG = 10;
};

}

#endif // CONNECTIONCONTROLTCP_H
