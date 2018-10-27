#include <sys/socket.h>
#include <sys/epoll.h>
#include <cstdio>
#include <zconf.h>
#include "tcp_server.h"
#include "../base/log.h"
#include "../base/errno_str.h"

namespace mona {
TcpServer::TcpServer(int port) : listenFd_(-1), epollFd_(0) {
    addr_.sin_family = SOCK_STREAM;
    addr_.sin_port = port;
    addr_.sin_addr.s_addr = 0;
}

bool TcpServer::initListenFd() {
    do {
        listenFd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (listenFd_ == -1) {
            LOG_FATAL << errnoToStr(errno, "create listen socket failed!");
            break;
        }
        if (bind(listenFd_, (sockaddr *) &addr_, sizeof(addr_)) == -1) {
            LOG_FATAL << errnoToStr(errno, "bind socket failed!");
            break;
        }
        if (listen(listenFd_, SOMAXCONN) == -1) {
            LOG_FATAL << errnoToStr(errno, "listen socket failed!");
            break;
        }
        return true;
    } while (0);

    return false;
}

bool TcpServer::start() {
    if (!initListenFd()) {
        return false;
    }
    if (!createEpoll()) {
        return false;
    }

    epoll_event epollEvent = {
            .events  = EPOLLIN,
            .data = (void *) listenFd_
    };
    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, listenFd_, &epollEvent) == -1) {
        LOG_ERROR << errnoToStr(errno, "epoll add failed, ") << listenFd_;
        return false;
    }

    sockaddr peerAddr;
    bzero(&peerAddr, sizeof(peerAddr));
    socklen_t peerAddrLen = 0;
    int peerFd;
    while (peerFd = accept(listenFd_, &peerAddr, &peerAddrLen), peerFd != -1) {

    }
    return false;
}

bool TcpServer::createEpoll() {
    epollFd_ = epoll_create(0);
    if (epollFd_ == -1) {
        LOG_FATAL << errnoToStr(errno, "create epoll failed!");
        return false;
    }
    return true;
}

TcpServer::~TcpServer() {
    close(epollFd_);
    close(listenFd_);
}
}


