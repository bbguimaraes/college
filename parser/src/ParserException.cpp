#include "ParserException.h"

ParserException::ParserException(
        unsigned int line, const std::string & message) {
    std::stringstream ss;
    ss << line << ": " << message;
    this->m_message = ss.str();
}

ParserException::ParserException(
        unsigned int line, TokenType expected, const Token & found) {
    std::stringstream ss;
    ss
        << line << ": expected " << expected.toString()
        << ", found " << found.text()
        << "(" << found.type().toString() << ")";
    this->m_message = ss.str();
}
