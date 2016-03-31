#ifndef PARSER_EXCEPTION_H
#define PRASER_EXCEPTION_H

#include <exception>
#include <string>
#include <sstream>

#include "Token.h"
#include "TokenType.h"

class ParserException : public std::exception {
    std::string m_message;

    public:
        ParserException(unsigned int line, const std::string & message);
        ParserException(
            unsigned int line, TokenType expected, const Token & found);

        virtual ~ParserException() throw() {}
        virtual const char * what() const throw()
            {return this->m_message.c_str();}
};

#endif // PARSER_EXCEPTION_H
