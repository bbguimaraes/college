#ifndef TRANSACTION_BUILDER_H
#define TRANSACTION_BUILDER_H

#include <iostream>
#include <list>
#include <string>
#include <vector>

class Operation;
class Transaction;

class TransactionBuilder {
    Transaction * m_transaction;

    public:
        TransactionBuilder(Transaction * transaction)
            : m_transaction(transaction) {}
        void set_transaction(Transaction * transaction)
            {this->m_transaction = transaction;}

        TransactionBuilder & add_read(const std::string & variable);
        TransactionBuilder & add_write(const std::string & variable);
        TransactionBuilder & add_commit();
        TransactionBuilder & add_abort();
        TransactionBuilder & random(
            unsigned int nvars, unsigned int noperations);

        static std::list<Operation *> operation_list(
            const std::vector<Transaction *> & transactions);
        static std::list<Operation *> shuffled_operation_list(
            const std::vector<Transaction *> & transactions);
        static bool parse_operation(
            const std::string & s,
            std::string * name,
            std::string * op,
            std::string * var);
};

std::istream & operator>>(
    std::istream & is, std::vector<Transaction *> & value);

#endif // TRANSACTION_BUILDER_H
