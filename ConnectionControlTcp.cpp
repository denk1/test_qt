#include "ConnectionControlTcp.h"
#include <stdarg.h>
#include <cstdio>
#include <exception>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

static socklen_t len = 0;

inline void thread_printf(const char* __restrict__ format, ... )
{
    va_list args;
    int done;
    va_start(args, format);
    done = vfprintf(stdout, format, args);
    va_end(args);
}

ConnectionControlTcp::ConnectionControlTcp():port(22400), mListenFlag(true)
{

}

ConnectionControlTcp::~ConnectionControlTcp()
{

}

void ConnectionControlTcp::init_server()
{
    mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(mSocketFd == -1)
    {
            const char *err_msg = "socket creation error";
            printf(err_msg);
            throw new std::exception();
    }
    else
    {
        thread_printf("socket bind OK");

    }
    bzero(&mServerAddress, sizeof (mServerAddress));
    mServerAddress.sin_family = AF_INET;
    mServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    mServerAddress.sin_port = htons(port);

    if(bind(mSocketFd, (SA*)&mServerAddress, sizeof(mServerAddress)) != 0)
    {
        printf("socket binding is failed");
        close(mSocketFd);
        throw new std::exception();
    }
    else
    {
        printf("socket binding is OK");
        fflush(stdout);
    }

    if(listen(mSocketFd, 5)!=0)
    {
        printf("socket listen is failed. Try again. Tomorrow for example.");
    }
    else
    {
        thread_printf("server is listening.\nPort: %d\n\n");
    }
}

void ConnectionControlTcp::connectionLoop()
{
    while (mListenFlag)
    {
        // connectionless
        mClientFd = accept(mSocketFd, (SA*)&mClientAddress, &len);
        if(mClientFd == -1 && mListenFlag)
        {
            thread_printf("the attempt of the connection client is failed");
        }
        else if(mClientFd != -1 && mListenFlag)
        {
            inet_ntop(AF_INET, &mClientAddress.sin_addr, mClientIP, INET_ADDRSTRLEN);
            mClientIP[INET_ADDRSTRLEN + 1] = 0x00;
            thread_printf("Server accept the client.\n");
            thread_printf("Client IPv4 address:%s\n\n", mClientAddress);
        }
        else
        {
            break;
        }
    }
}
