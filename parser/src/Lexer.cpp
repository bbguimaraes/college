#include "Lexer.h"

#include <algorithm>
#include <cassert>
#include <string>

#include "LexerException.h"

/*static*/
const std::map<std::string, TokenType> Lexer::KEYWORDS{
    {"algoritmo",       TokenType::TK_ALGORITMO},
    {"ate",             TokenType::TK_ATE},
    {"de",              TokenType::TK_DE},
    {"entao",           TokenType::TK_ENTAO},
    {"enquanto",        TokenType::TK_ENQUANTO},
    {"faca",            TokenType::TK_FACA},
    {"fimalgoritmo",    TokenType::TK_FIM_ALG},
    {"fimenquanto",     TokenType::TK_FIMENQUANTO},
    {"fimfuncao",       TokenType::TK_FIMFUNCAO},
    {"fimpara",         TokenType::TK_FIMPARA},
    {"fimprocedimento", TokenType::TK_FIMPROCEDIMENTO},
    {"fimse",           TokenType::TK_FIMSE},
    {"funcao",          TokenType::TK_FUNCAO},
    {"inicio",          TokenType::TK_INICIO},
    {"inteiro",         TokenType::TK_INTEIRO},
    {"literal",         TokenType::TK_LITERAL},
    {"logico",          TokenType::TK_LOGICO},
    {"para",            TokenType::TK_PARA},
    {"real",            TokenType::TK_REAL},
    {"repita",          TokenType::TK_REPITA},
    {"retorne",         TokenType::TK_RETORNE},
    {"se",              TokenType::TK_SE},
    {"senao",           TokenType::TK_SENAO},
    {"var",             TokenType::TK_VAR},
    {"vetor",           TokenType::TK_VETOR},
    {"escreva",         TokenType::TK_ESCREVA},
    {"escreval",        TokenType::TK_ESCREVAL},
    {"leia",            TokenType::TK_LEIA},
    {"verdadeiro",      TokenType::TK_VERDADEIRO},
    {"falso",           TokenType::TK_FALSO},
    {"mod",             TokenType::TK_DIVINT},
    {"div",             TokenType::TK_DIVIDE},
    {"e",               TokenType::TK_E},
    {"ou",              TokenType::TK_OU},
};

/*static*/
const std::map<char, unsigned int> Lexer::SINGLE_CHAR{
    {'<',  5},
    {'>',  9},
    {'=', 11},
    {'-', 12},
    {'+', 13},
    {'*', 14},
    {'/', 15},
    {':', 17},
    {'"', 18},
    {',', 20},
    {'(', 21},
    {')', 22},
    {'[', 23},
    {']', 24},
    {'^', 25},
    {'.', 26},
    {';', 28},
};

/*static*/
const std::map<unsigned int, TokenType> Lexer::END_STATES{
    {6,  TokenType::TK_ATRIB},
    {7,  TokenType::TK_MENORIGUAL},
    {8,  TokenType::TK_DIFERENTE},
    {10, TokenType::TK_MAIORIGUAL},
    {11, TokenType::TK_IGUAL},
    {12, TokenType::TK_MENOS},
    {13, TokenType::TK_MAIS},
    {14, TokenType::TK_MULT},
    {17, TokenType::TK_DOIS_PONTOS},
    {19, TokenType::TK_STRING},
    {20, TokenType::TK_VIRGULA},
    {21, TokenType::TK_ABRE_PAR},
    {22, TokenType::TK_FECHA_PAR},
    {23, TokenType::TK_ABRECOLCHETE},
    {24, TokenType::TK_FECHACOLCHETE},
    {25, TokenType::TK_POTENCIACAO},
    {27, TokenType::TK_PONTOPONTO},
    {28, TokenType::TK_PONTO_E_VIRGULA},
};

void Lexer::initialize(const std::string & input) {
    this->m_input = input;
    this->m_position = 0;
    this->next();
}

bool Lexer::eof() const {
    unsigned int i = this->m_position;
    while(i <= this->m_input.size() && (
            this->m_input[i] == '\n'
            || this->m_input[i] == '\t'
            || this->m_input[i] == ' '))
        ++i;
    return i == this->m_input.size();
}

Token Lexer::val_keyword(unsigned int start) {
    std::string token = this->m_input.substr(start, this->m_position - start);
    std::string token_to_lower(token.length(), 0);
    std::transform(
        std::begin(token),
        std::end(token),
        std::begin(token_to_lower),
        tolower);
    auto it = KEYWORDS.find(token_to_lower);
    if(it != std::end(KEYWORDS))
        return this->m_current = Token(token, it->second);
    else
        return this->m_current = Token(token, TokenType::TK_IDENT);
}

Token Lexer::get_token(unsigned int start, TokenType type) {
    return this->m_current = Token(
        this->m_input.substr(start, this->m_position - start),
        type);
}

unsigned int Lexer::next_char_state(unsigned int state) {
    ++this->m_position;
    return state;
}

Token Lexer::next() {
    if(this->m_position == this->m_input.size())
        throw LexerException("EOF");
    const unsigned int start = this->m_position;
    unsigned int state = 0;
    while(true) {
        char c = this->m_input[this->m_position];
        switch(state) {
            case 0:
                if(c == '\n') {
                    ++this->m_line;
                    ++this->m_position;
                    return m_current = next();
                }
                if(c == ' ' || c == '\t' || c == '\r') {
                    ++this->m_position;
                    return m_current = next();
                }
                if(isalpha(c) || c == '_')
                    state = this->next_char_state(1);
                else if(isdigit(c))
                    state = this->next_char_state(2);
                else {
                    auto it = this->SINGLE_CHAR.find(c);
                    if(it != std::end(this->SINGLE_CHAR))
                        state = this->next_char_state(it->second);
                    else
                        throw LexerException(
                            this->m_line,
                            std::string("Unknown symbol: '") + c + "'");
                }
                break;
            case 1:
                if(!std::isalnum(c) && c != '_')
                    return this->val_keyword(start);
                ++this->m_position;
                break;
            case 2:
                if(isdigit(c))
                    ++this->m_position;
                else if(c == '.')
                    state = this->next_char_state(3);
                else
                    return this->get_token(start, TokenType::TK_CONST_INT);
                break;
            case 3:
                if(c == '.') {
                    --this->m_position;
                    return this->get_token(start, TokenType::TK_CONST_INT);
                } else if(std::isdigit(c))
                    state = this->next_char_state(4);
                else
                    throw LexerException(
                        this->m_line,
                        "Expected digit after point (e.g. '1.' is not valid)");
                break;
            case 4:
                if(isdigit(c))
                    ++this->m_position;
                else
                    return this->get_token(start, TokenType::TK_CONST_REAL);
                break;
            case 5:
                if(c == '-')
                    state = this->next_char_state(6);
                else if(c == '=')
                    state = this->next_char_state(7);
                else if(c == '>')
                    state = this->next_char_state(8);
                else
                    return this->get_token(start, TokenType::TK_MENOR);
                break;
            case 9:
                if(c == '=')
                    state = this->next_char_state(10);
                else
                    return this->get_token(start, TokenType::TK_MAIOR);
                break;
            case 15:
                if(c == '/')
                    state = this->next_char_state(16);
                else
                    return this->get_token(start, TokenType::TK_DIVIDE);
                break;
            case 16:
                if(c != '\n')
                    ++this->m_position;
                else {
                    ++this->m_position;
                    return this->m_current = next();
                }
                break;
            case 18:
                if(c != '"')
                    ++this->m_position;
                else
                    state = this->next_char_state(19);
                break;
            case 26:
                if(c == '.')
                    state = this->next_char_state(27);
                else
                    throw LexerException(
                        this->m_line,
                        std::string("Expected a second point, found ") + c);
                break;
            default: {
                auto it = this->END_STATES.find(state);
                if(it != std::end(this->END_STATES))
                    return this->get_token(start, it->second);
                assert(false);
            }
        }
    }
}
