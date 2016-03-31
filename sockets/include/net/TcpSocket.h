#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

extern "C" {
    #include <arpa/inet.h>
    #include <unistd.h>
}

#include <string>
#include <list>

class TcpSocket {
    public:
        static std::string error();

        TcpSocket();
        ~TcpSocket();

        void close();
        void bind(int port);
        void listen(int n = 5);
        /** The socket returned is destroyed when this socket is destroyed. */
        TcpSocket * accept();
        void connect(const std::string & host, int port);
        void set_blocking(bool blocking);

        void send(const void * data, int size);
        bool recv(void * data, int size);

    private:
        TcpSocket(int fd) : m_socket(fd) {}
        TcpSocket(TcpSocket &) = delete;
        void operator =(TcpSocket &) = delete;

        int m_socket;
        sockaddr_in m_address;
        std::list<TcpSocket *> m_connections;
};

#endif // TCP_SOCKET_H
