#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <map>

#include "Token.h"

class Lexer {
    public:
        Lexer() : m_position(0), m_line(1) {}

        Token current() const {return this->m_current;}
        Token next();
        unsigned int position() const {return this->m_position;}
        unsigned int line() const {return this->m_line;}
        bool eof() const;
        void initialize(const std::string & input);

    private:
        Token val_keyword(unsigned int start);
        Token get_token(unsigned int start, TokenType type);
        unsigned int next_char_state(unsigned int state);

        static const std::map<std::string, TokenType> KEYWORDS;
        static const std::map<char, unsigned int> SINGLE_CHAR;
        static const std::map<unsigned int, TokenType> END_STATES;

        std::string m_input;
        unsigned int m_position;
        unsigned int m_line;
        Token m_current;
};

#endif // LEXER_H
