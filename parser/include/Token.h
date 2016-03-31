#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <string>

#include "TokenType.h"

class Token {
    public:
        Token(
            const std::string & text = std::string(),
            TokenType type = TokenType::TK_INVALIDO)
                : m_text(text), m_type(type) {}

        std::string text() const {return this->m_text;}
        TokenType type() const {return this->m_type;}

    private:
        std::string m_text;
        TokenType m_type;

        friend std::ostream & operator<<(
            std::ostream & os, const Token & token);
};

std::ostream & operator<<(std::ostream & os, const Token & token);

#endif // TOKEN_H
