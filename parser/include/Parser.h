#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "Lexer.h"

class Parser {
    public:
        Parser() {}

        void parse(const std::string & input);

    private:
        void check_current(TokenType::Type type);
        void error(TokenType expected);

        void val_algoritmo();
        void val_nome();

        void val_fdec();
        void val_func();
        void val_proc();
        void val_param_d();
        void val_param_dl();

        void val_com();

        void val_var();
        void val_ldec();
        void val_dec();
        void val_lid();
        void val_tipo();
        void val_tipo(bool simple);
        void val_vdec();
        void val_matriz();
        void val_atrib_func();

        void val_se();
        void val_senao();
        void val_para();
        void val_repita();
        void val_enquanto();
        void val_funcao();
        void val_procedimento();

        void val_leia();
        void val_escreva();

        void val_exp();
        void val_exp_ou();
        void val_ou();
        void val_exp_e();
        void val_e();
        void val_exp_rel();
        void val_rel();
        void val_exp_mais();
        void val_mais();
        void val_exp_mul();
        void val_mul();
        void val_exp_pot();
        void val_pot();
        void val_t();
        void val_id();
        void val_subs();
        void val_subs_mat();

        Lexer m_lexer;
};

#endif // PARSER_H
