#include "ConnectionControlTcp.h"


namespace  RAT
{

double get_double_value(char* b)
{
    long int angle = *(reinterpret_cast<long int*>(b));
    int loword =ntohl(static_cast<int>(angle >> 32));
    int hiword = ntohl(static_cast<int>(angle));
    long int i64 = ((static_cast<long int>(hiword)) << 32) | loword;

    double a = (double)ntohl(angle);
    printf("hiword=%d\n", hiword);
    printf("loword=%d\n", loword);
    printf("i64=%ld\n", i64);
    double recieved_double = *(reinterpret_cast<double*>(&i64));
    printf("double=%lf\n", recieved_double);
    while (angle) {
    if (angle & (uint64_t)1)
        printf("1");
    else
            printf("0");

    angle >>= (uint64_t)1;
    }
    printf("\n");
    return recieved_double;
}

int recvtimeout(int s, char *buf, int len, int timeout)
{
    fd_set fds;
    int n;
    struct timeval tv;
    // set up the file descriptor set
    FD_ZERO(&fds);
    FD_SET(s, &fds);
    // set up the struct timeval for the timeout
    tv.tv_sec = 0;
    tv.tv_usec = timeout;
    // wait until timeout or data received
    n = select(s+1, &fds, NULL, NULL, &tv);
    if (n == 0) return -2; // timeout!
    if (n == -1) return -1; // error
    // data must be here, so do a normal recv()
    return recv(s, buf, len, 0);
}

void *ConnectionControlTCP::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in*)sa)->sin_addr);
}


ConnectionControlTCP::ConnectionControlTCP(ITS* ptrITS) : ConnectionControlInterface (ptrITS),
                                                                                  mControlling(ptrITS, &mThreadSafeData)
{
    mThreadSafeData.setControlling(&mControlling);
    memset(&hints, 0, sizeof (hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags =AI_PASSIVE;

    if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) == -1) {
        fprintf(stderr,  "getaddrinfo: %s\n", gai_strerror(rv));
        throw  std::exception();
     }

    for(p = servinfo; p != NULL;  p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int))  == -1) {
            perror("setsockopt");
            exit(1);
        }


        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }
    freeaddrinfo(servinfo);

    if(p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(2);
    }

    if(listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(3);
    }

    if(listen(sockfd, BACKLOG) ==-1) {
        perror("listen");
        exit(4);
    }

    printf("server: waiting for connection...\n");

}

ConnectionControlTCP::~ConnectionControlTCP()
{
    if(mThread.joinable())
        mThread.join();
}

void ConnectionControlTCP::run()
{

    TypeDefSend fSend = std::bind(&ConnectionControlTCP::sendTelemetry, this, std::placeholders::_1);
    mControlling.setSenderTelemetry(&fSend);
    mControlling.start();

    while(1) {
        sin_size = sizeof( their_addr);
        printf("%d\n", sockfd);
        printf("sin_size=%d\n", sin_size);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        printf("new_fd=%d\n", new_fd);
        printf("size of int=%ld\n", sizeof(int));
        printf("size of long int=%ld\n", sizeof(long int));
        printf("test!!!!!!!!!!!!!!!!!!!!!!\n");
        printf("size of double=%ld", sizeof(double));
        if (new_fd == -1) {
            perror("accept");
        //    continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);
        int bytes_read;
        char buff[16], buff1[8], buff2[8];
        while((bytes_read = recv(new_fd, buff, sizeof (buff), 0)) != 0) {
        //while((bytes_read = recvtimeout(new_fd, buff, sizeof (buff), 10)) != -1) {
            if(bytes_read == -2) {
                memset(buff, 0, sizeof (buff));
            }
            memcpy(buff1, buff, 8);
            memcpy(buff2, buff+8, 8);
            mControlling.setSteeringValueFromConn(get_double_value(buff1));
            mControlling.setMovingValueFromConn(get_double_value(buff2));
            printf("bytes_read=%d\n", bytes_read);
        }
        new_fd = 0;
    }
}

void ConnectionControlTCP::sendTelemetry(unsigned char* msg)
{
    if(new_fd > 0)
        send(new_fd, msg, sizeof (unsigned char[18]), 0);
}

void ConnectionControlTCP::set_timer(int t)
{

}


}
