#include "LexerException.h"

LexerException::LexerException(
        unsigned int line, const std::string & message) {
    std::stringstream ss;
    ss << line << ": " << message;
    this->m_message = ss.str();
}
