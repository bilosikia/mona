#pragma once

#include <socket.h>
#include <netinet/in.h>


namespace mona {
class TcpServer {
public:
    TcpServer(int port);
    ~TcpServer();

    bool start();

private:
    bool initListenFd();
    bool createEpoll();

private:
    int listenFd_;
    int epollFd_;
    sockaddr_in addr_;
};
}

