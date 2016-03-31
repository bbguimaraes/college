#ifndef LEXER_EXCEPTION_H
#define LEXER_EXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

class LexerException : public std::exception {
    std::string m_message;

    public:
        LexerException(const std::string & message) : m_message(message) {}
        LexerException(unsigned int line, const std::string & message);

        virtual ~LexerException() throw() {}
        virtual const char * what() const throw()
            {return this->m_message.c_str();}
};

#endif // LEXER_EXCEPTION_H
