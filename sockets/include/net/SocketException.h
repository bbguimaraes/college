#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H

#include <exception>
#include <string>

class SocketException : public std::exception {
    public:
        SocketException(const std::string & message) : m_message(message) {}
        virtual ~SocketException() throw() {}
        virtual const char * what() const throw() {return m_message.c_str();}

    protected:
        std::string m_message;
};

#endif // SOCKET_EXCEPTION_H
