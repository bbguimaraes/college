#include "TokenType.h"

std::string TokenType::toString() const {
    static const std::string LOOKUP[] = {
        "TK_INVALIDO",
        "TK_ALGORITMO",
        "TK_VAR",
        "TK_INICIO",
        "TK_FIM_ALG",
        "TK_PARA",
        "TK_DE",
        "TK_ATE",
        "TK_FACA",
        "TK_FIMPARA",
        "TK_REPITA",
        "TK_ENQUANTO",
        "TK_FIMENQUANTO",
        "TK_SE",
        "TK_ENTAO",
        "TK_SENAO",
        "TK_FIMSE",
        "TK_ESCREVA",
        "TK_LEIA",
        "TK_INTEIRO",
        "TK_REAL",
        "TK_COMENT",
        "TK_STRING",
        "TK_CONST_INT",
        "TK_IDENT",
        "TK_DOIS_PONTOS",
        "TK_VIRGULA",
        "TK_ATRIB",
        "TK_MULT",
        "TK_MAIS",
        "TK_MENOS",
        "TK_DIVIDE",
        "TK_DIVINT",
        "TK_RESTO",
        "TK_ABRE_PAR",
        "TK_FECHA_PAR",
        "TK_MENOR",
        "TK_MAIOR",
        "TK_E",
        "TK_OU",
        "TK_IGUAL",
        "TK_MENORIGUAL",
        "TK_MAIORIGUAL",
        "TK_DIFERENTE",
        "TK_NOVA_LINHA",
        "TK_LITERAL",
        "TK_VETOR",
        "TK_ABRECOLCHETE",
        "TK_PONTOPONTO",
        "TK_FECHACOLCHETE",
        "TK_LOGICO",
        "TK_POTENCIACAO",
        "TK_CONST_REAL",
        "TK_ESCREVAL",
        "TK_FALSO",
        "TK_VERDADEIRO",
        "TK_FUNCAO",
        "TK_FIMFUNCAO",
        "TK_PROCEDIMENTO",
        "TK_FIMPROCEDIMENTO",
        "TK_PONTO_E_VIRGULA",
        "TK_RETORNE",
        "TK_NAO",
        "TK_PASSO",
    };
    return LOOKUP[m_type];
}

bool operator ==(TokenType type1, TokenType::Type type2) {
   return type1.type() == type2;
}

bool operator !=(TokenType type1, TokenType::Type type2) {
    return type1.type() != type2;
}

std::ostream & operator <<(std::ostream & os, TokenType type) {
    return os << type.toString();
}
