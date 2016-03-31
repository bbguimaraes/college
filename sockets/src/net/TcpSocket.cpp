#include "net/TcpSocket.h"

extern "C" {
    #include <fcntl.h>
}

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

#include "net/SocketException.h"

/*static*/
std::string TcpSocket::error() {
    return std::string(strerror(errno));
}

TcpSocket::TcpSocket() {
    if((this->m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw SocketException(std::string("TcpSocket(): ") + strerror(errno));
}

TcpSocket::~TcpSocket() {
    try {
        if(this->m_socket > 0)
            close();
        for(auto s : this->m_connections)
            s->close();
    }
    catch(SocketException ex) {
        std::cerr << ex.what() << "\n";
    }
}

void TcpSocket::set_blocking(bool blocking) {
    int status;
    if((status = fcntl(this->m_socket, F_GETFL, nullptr)) == -1)
        throw SocketException(
            std::string("TcpSocket::fcntl(F_GETFL): ") + strerror(errno));
    if(blocking)
        status &= ~O_NONBLOCK;
    else
        status |= O_NONBLOCK;
    if(fcntl(this->m_socket, F_SETFL, status) != 0)
        throw SocketException(
            std::string("TcpSocket::fcntl(F_SETFL, O_NONBLOCK): ")
                + strerror(errno));
}

void TcpSocket::close() {
    if(::close(this->m_socket) < 0)
        throw SocketException(
            std::string("TcpSocket::close: ") + strerror(errno));
    this->m_socket = -1;
}

void TcpSocket::bind(int port) {
    memset(static_cast<void *>(&m_address), 0, sizeof(sockaddr_in));
    m_address.sin_family = AF_INET;
    m_address.sin_port = htons(port);
    m_address.sin_addr.s_addr = INADDR_ANY;
    int result = 1;
    result = ::setsockopt(
        this->m_socket, SOL_SOCKET, SO_REUSEADDR, &result, sizeof(int));
    if(result < 0)
        throw SocketException(
            "TcpSocket::bind: setsockopt: " + std::string(strerror(errno)));
    result = ::bind(
        this->m_socket,
        reinterpret_cast<sockaddr *>(&m_address),
        sizeof(sockaddr_in));
    if(result < 0)
        throw SocketException(
            "TcpSocket::bind: " + std::string(strerror(errno)));
}

void TcpSocket::listen(int n) {
    if(::listen(this->m_socket, n) < 0)
        throw SocketException(
            std::string("TcpSocket::listen: ") + strerror(errno));
}

TcpSocket * TcpSocket::accept() {
    socklen_t len = sizeof(sockaddr_in);
    sockaddr_in addr;
    memset(static_cast<void *>(&addr), 0, sizeof(addr));
    int fd = ::accept(
        this->m_socket, reinterpret_cast<sockaddr *>(&addr), &len);
    if(fd < 0)
        throw SocketException(
            std::string("TcpSocket::accept: ") + strerror(errno));
    TcpSocket * ret = new TcpSocket(fd);
    ret->m_address = addr;
    m_connections.push_back(ret);
    return ret;
}

void TcpSocket::connect(const std::string & host, int port) {
    sockaddr_in addr;
    memset(static_cast<void *>(&addr), 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    int result = ::connect(
        this->m_socket,
        reinterpret_cast<sockaddr *>(&addr),
        sizeof(sockaddr_in));
    if(result < 0)
        throw SocketException(
            std::string("TcpSocket::connect(")
                + "\"" + host + "\""
                + ", " + std::to_string(port) + "): "
                + strerror(errno));
}

void TcpSocket::send(const void * data, int size) {
    if(::send(this->m_socket, data, size, 0) < 0)
        throw SocketException(
            std::string("TcpSocket::send: ") + strerror(errno));
}

bool TcpSocket::recv(void * data, int size) {
    int ret = ::recv(this->m_socket, data, size, 0);
    if(ret > 0) {
        return true;
    }
    if(errno == EAGAIN || errno == EWOULDBLOCK)
        return false;
    throw SocketException(std::string("TcpSocket::recv: ") + strerror(errno));
}
