#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <list>

class Accessor;
class Ending;

class Transaction {
    public:
        Transaction(
            const std::string & name = std::string(),
            const std::list<Accessor *> & operations = std::list<Accessor *>(),
            const Ending * ending = nullptr);
        Transaction(const Transaction & other) = delete;
        Transaction & operator =(const Transaction & other) = delete;
        ~Transaction();

        void set_name(const std::string & name) {this->m_name = name;}
        void set_operations(const std::list<Accessor *> & operations);
        void set_ending(const Ending * ending);

        std::string name() const {return this->m_name;}
        const std::list<Accessor *> * operations() const
            {return &this->m_operations;}
        std::list<Accessor *> * operations() {return &this->m_operations;}
        Ending * ending() const {return this->m_ending;}

        void shuffle();

    private:
        void release_operations();

        std::string m_name;
        std::list<Accessor *> m_operations;
        Ending * m_ending;

        friend class TransactionBuilder;
        friend std::ostream & operator <<(std::ostream & os, Transaction & t);
};

std::ostream & operator <<(std::ostream & os, Transaction & t);

#endif // TRANSACTION_H
