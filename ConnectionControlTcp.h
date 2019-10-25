#ifndef CONNECTIONCONTROLTCP_H
#define CONNECTIONCONTROLTCP_H

#include <sys/socket.h>
#include <arpa/inet.h>

#define SA struct sockaddr

class ConnectionControlTcp
{
public:
    ConnectionControlTcp();
    virtual ~ConnectionControlTcp();
private:
    void init_server();
    void connectionLoop();
    int mClientFd;
    int mSocketFd;
    struct sockaddr_in mServerAddress;
    struct sockaddr_in mClientAddress;
    unsigned short port;
    bool mListenFlag;
    char mClientIP[INET_ADDRSTRLEN + 1];
};

#endif // CONNECTIONCONTROLTCP_H
