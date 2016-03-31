#include "Parser.h"

#include "Lexer.h"
#include "ParserException.h"

void Parser::check_current(TokenType::Type type) {
    if(this->m_lexer.current().type() != type)
        this->error(type);
    else
        this->m_lexer.next();
}

void Parser::error(TokenType expected) {
    throw ParserException(
        this->m_lexer.line(),
        expected,
        this->m_lexer.current());
}

void Parser::parse(const std::string & input) {
    this->m_lexer.initialize(input);
    return this->val_algoritmo();
}

void Parser::val_algoritmo() {
    this->check_current(TokenType::TK_ALGORITMO);
    this->val_nome();
    this->val_fdec();
    this->val_var();
    this->check_current(TokenType::TK_INICIO);
    this->val_com();
    if(this->m_lexer.current().type() != TokenType::TK_FIM_ALG)
        this->error(TokenType::TK_FIM_ALG);
}

void Parser::val_nome() {
    if(this->m_lexer.current().type() == TokenType::TK_STRING)
        this->m_lexer.next();
}

void Parser::val_fdec() {
    if(this->m_lexer.current().type() == TokenType::TK_FUNCAO) {
        this->m_lexer.next();
        this->val_func();
        this->val_fdec();
    } else if(this->m_lexer.current().type() == TokenType::TK_PROCEDIMENTO) {
        this->m_lexer.next();
        this->val_proc();
        this->val_fdec();
    }
}

void Parser::val_func() {
    this->check_current(TokenType::TK_IDENT);
    this->check_current(TokenType::TK_ABRE_PAR);
    this->val_param_d();
    this->check_current(TokenType::TK_FECHA_PAR);
    this->check_current(TokenType::TK_DOIS_PONTOS);
    this->val_tipo();
    this->val_var();
    this->check_current(TokenType::TK_INICIO);
    this->val_com();
    this->check_current(TokenType::TK_FIMFUNCAO);
}

void Parser::val_proc() {
    this->check_current(TokenType::TK_IDENT);
    this->check_current(TokenType::TK_ABRE_PAR);
    this->val_param_d();
    this->check_current(TokenType::TK_FECHA_PAR);
    this->val_var();
    this->check_current(TokenType::TK_INICIO);
    this->val_com();
    this->check_current(TokenType::TK_FIMPROCEDIMENTO);
}

void Parser::val_param_d() {
    if(this->m_lexer.current().type() != TokenType::TK_IDENT)
        return;
    this->val_dec();
    this->val_param_dl();
}

void Parser::val_param_dl() {
    if(this->m_lexer.current().type() != TokenType::TK_PONTO_E_VIRGULA)
        return;
    this->m_lexer.next();
    this->val_dec();
    this->val_param_dl();
}

void Parser::val_var() {
    if(this->m_lexer.current().type() != TokenType::TK_VAR)
        return;
    this->m_lexer.next();
    this->val_ldec();
}

void Parser::val_ldec() {
    if(this->m_lexer.current().type() != TokenType::TK_IDENT)
        return;
    this->val_dec();
    this->val_ldec();
}

void Parser::val_dec() {
    if(this->m_lexer.current().type() != TokenType::TK_IDENT)
        return;
    this->m_lexer.next();
    this->val_lid();
    this->check_current(TokenType::TK_DOIS_PONTOS);
    this->val_tipo();
}

void Parser::val_lid() {
    if(this->m_lexer.current().type() != TokenType::TK_VIRGULA)
        return;
    this->m_lexer.next();
    this->check_current(TokenType::TK_IDENT);
    this->val_lid();
}

void Parser::val_tipo() {
    this->val_tipo(false);
}

void Parser::val_tipo(bool simple) {
    switch(this->m_lexer.current().type().type()) {
        case TokenType::TK_INTEIRO:
        case TokenType::TK_REAL:
        case TokenType::TK_LOGICO:
        case TokenType::TK_LITERAL:
            this->m_lexer.next();
            break;
        case TokenType::TK_VETOR:
            if(simple)
                throw ParserException(
                    this->m_lexer.line(),
                    "Expected non-vector type, found "
                        + this->m_lexer.current().type().toString());
            this->m_lexer.next();
            this->val_vdec();
            break;
        default:
            throw ParserException(
                this->m_lexer.line(),
                "Expected a type, found "
                    + this->m_lexer.current().type().toString());
    }
}

void Parser::val_vdec() {
    this->check_current(TokenType::TK_ABRECOLCHETE);
    this->check_current(TokenType::TK_CONST_INT);
    this->check_current(TokenType::TK_PONTOPONTO);
    this->check_current(TokenType::TK_CONST_INT);
    this->val_matriz();
    this->check_current(TokenType::TK_FECHACOLCHETE);
    this->check_current(TokenType::TK_DE);
    this->val_tipo(true);
}

void Parser::val_matriz() {
    if(this->m_lexer.current().type() != TokenType::TK_VIRGULA)
        return;
    this->m_lexer.next();
    this->check_current(TokenType::TK_CONST_INT);
    this->check_current(TokenType::TK_PONTOPONTO);
    this->check_current(TokenType::TK_CONST_INT);
}

void Parser::val_com() {
    TokenType next_token = this->m_lexer.current().type().type();
    if(next_token == TokenType::TK_IDENT) {
        this->val_id();
        this->val_atrib_func();
        this->val_com();
    } else if(next_token == TokenType::TK_SE) {
        this->m_lexer.next();
        this->val_se();
        this->val_com();
    } else if(next_token == TokenType::TK_PARA) {
        this->m_lexer.next();
        this->val_para();
        this->val_com();
    } else if(next_token == TokenType::TK_REPITA) {
        this->m_lexer.next();
        this->val_repita();
        this->val_com();
    } else if(next_token == TokenType::TK_ENQUANTO) {
        this->m_lexer.next();
        this->val_enquanto();
        this->val_com();
    } else if(next_token == TokenType::TK_LEIA) {
        this->m_lexer.next();
        this->val_leia();
        this->val_com();
    } else if(next_token == TokenType::TK_ESCREVA
            || next_token == TokenType::TK_ESCREVAL) {
        this->m_lexer.next();
        this->val_escreva();
        this->val_com();
    } else if(next_token == TokenType::TK_RETORNE) {
        this->m_lexer.next();
        this->val_exp();
    }
}

void Parser::val_atrib_func() {
    if(this->m_lexer.current().type() == TokenType::TK_ATRIB) {
        this->m_lexer.next();
        this->val_exp();
    } else {
        this->check_current(TokenType::TK_ABRE_PAR);
        this->val_ldec();
        this->check_current(TokenType::TK_FECHA_PAR);
    }
}

void Parser::val_se() {
    this->val_exp();
    this->check_current(TokenType::TK_ENTAO);
    this->val_com();
    this->val_senao();
    this->check_current(TokenType::TK_FIMSE);
}

void Parser::val_senao() {
    if(this->m_lexer.current().type() != TokenType::TK_SENAO)
        return;
    this->m_lexer.next();
    this->val_com();
}

void Parser::val_para() {
    this->check_current(TokenType::TK_IDENT);
    this->check_current(TokenType::TK_DE);
    this->val_exp();
    this->check_current(TokenType::TK_ATE);
    this->val_exp();
    this->check_current(TokenType::TK_FACA);
    this->val_com();
    this->check_current(TokenType::TK_FIMPARA);
}

void Parser::val_repita() {
    this->val_com();
    this->check_current(TokenType::TK_ATE);
    this->val_exp();
}

void Parser::val_enquanto() {
    this->val_exp();
    this->check_current(TokenType::TK_FACA);
    this->val_com();
    this->check_current(TokenType::TK_FIMENQUANTO);
}

void Parser::val_leia() {
    this->check_current(TokenType::TK_ABRE_PAR);
    this->check_current(TokenType::TK_IDENT);
    this->check_current(TokenType::TK_FECHA_PAR);
}

void Parser::val_escreva() {
    this->check_current(TokenType::TK_ABRE_PAR);
    this->val_exp();
    this->check_current(TokenType::TK_FECHA_PAR);
}

void Parser::val_exp() {
    this->val_exp_ou();
}

void Parser::val_exp_ou() {
    this->val_exp_e();
    this->val_ou();
}

void Parser::val_ou() {
    if(this->m_lexer.current().type() != TokenType::TK_OU)
        return;
    this->m_lexer.next();
    this->val_exp_ou();
}

void Parser::val_exp_e() {
    this->val_exp_rel();
    this->val_e();
}

void Parser::val_e() {
    if(this->m_lexer.current().type() != TokenType::TK_E)
        return;
    this->m_lexer.next();
    this->val_exp_e();
}

void Parser::val_exp_rel() {
    this->val_exp_mais();
    this->val_rel();
}

void Parser::val_rel() {
    TokenType type = this->m_lexer.current().type();
    if(type != TokenType::TK_IGUAL
            && type != TokenType::TK_DIFERENTE
            && type != TokenType::TK_MAIOR
            && type != TokenType::TK_MENOR
            && type != TokenType::TK_MAIORIGUAL
            && type != TokenType::TK_MENORIGUAL)
        return;
    this->m_lexer.next();
    this->val_exp_rel();
}

void Parser::val_exp_mais() {
    this->val_exp_mul();
    this->val_mais();
}

void Parser::val_mais() {
    TokenType type = this->m_lexer.current().type();
    if(type != TokenType::TK_MAIS && type != TokenType::TK_MENOS)
        return;
    this->m_lexer.next();
    this->val_exp_mais();
}

void Parser::val_exp_mul() {
    this->val_exp_pot();
    this->val_mul();
}

void Parser::val_mul() {
    TokenType type = this->m_lexer.current().type();
    if(type != TokenType::TK_MULT
            && type != TokenType::TK_DIVIDE
            && type != TokenType::TK_DIVINT
            && type != TokenType::TK_RESTO)
        return;
    this->m_lexer.next();
    this->val_exp_mul();
}

void Parser::val_exp_pot() {
    this->val_t();
    this->val_pot();
}

void Parser::val_pot() {
    if(this->m_lexer.current().type() != TokenType::TK_POTENCIACAO)
        return;
    this->m_lexer.next();
    this->val_exp_pot();
}

void Parser::val_t() {
    TokenType type = this->m_lexer.current().type();
    if(type == TokenType::TK_IDENT)
        val_id();
    else if(type == TokenType::TK_CONST_INT
            || type == TokenType::TK_CONST_REAL
            || type == TokenType::TK_VERDADEIRO
            || type == TokenType::TK_FALSO
            || type == TokenType::TK_STRING)
        this->m_lexer.next();
    else if(type == TokenType::TK_MENOS) {
        this->m_lexer.next();
        this->val_t();
    } else if(type == TokenType::TK_ABRE_PAR) {
        this->m_lexer.next();
        this->val_exp();
        this->check_current(TokenType::TK_FECHA_PAR);
    } else
        throw ParserException(
            this->m_lexer.line(),
            "Expected something to continue expression, found "
                + this->m_lexer.current().type().toString());
}

void Parser::val_id() {
    this->check_current(TokenType::TK_IDENT);
    this->val_subs();
}

void Parser::val_subs() {
    if(this->m_lexer.current().type() != TokenType::TK_ABRECOLCHETE)
        return;
    this->m_lexer.next();
    this->val_exp();
    this->val_subs_mat();
    this->check_current(TokenType::TK_FECHACOLCHETE);
}

void Parser::val_subs_mat() {
    if(this->m_lexer.current().type() != TokenType::TK_VIRGULA)
        return;
    this->m_lexer.next();
    this->val_exp();
}
