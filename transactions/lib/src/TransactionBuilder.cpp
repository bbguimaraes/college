#include "TransactionBuilder.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>

#include "Operation.h"
#include "Transaction.h"

TransactionBuilder &
TransactionBuilder::add_read(const std::string & variable) {
    this->m_transaction->m_operations.push_back(
        new Read(m_transaction, variable));
    return *this;
}

TransactionBuilder &
TransactionBuilder::add_write(const std::string & variable) {
    this->m_transaction->m_operations.push_back(
        new Write(m_transaction, variable));
    return *this;
}

TransactionBuilder & TransactionBuilder::add_commit() {
    this->m_transaction->set_ending(new Commit(m_transaction));
    return *this;
}

TransactionBuilder & TransactionBuilder::add_abort() {
    this->m_transaction->set_ending(new Abort(m_transaction));
    return *this;
}

TransactionBuilder &
TransactionBuilder::random(unsigned int nvars, unsigned int noperations) {
    if(nvars == 0 || noperations == 0)
        return *this;
    std::list<Accessor *> operations;
    for(unsigned int i = 0; i < noperations; ++i) {
        char var = 'a' + (std::rand() % nvars);
        unsigned int rw = std::rand() % 2;
        if(rw)
            this->add_read(std::string(1, var));
        else
            this->add_write(std::string(1, var));
    }
    this->m_transaction->set_operations(operations);
    if(std::rand() % 2)
        this->add_commit();
    else
        this->add_abort();
    return *this;
}

/*static*/
std::list<Operation *> TransactionBuilder::operation_list(
        const std::vector<Transaction *> & transactions) {
    std::list<Operation *> ret;
    for(auto t : transactions) {
        std::copy(
            t->operations()->cbegin(),
            t->operations()->cend(),
            std::back_inserter(ret));
        ret.push_back(t->ending());
    }
    return ret;
}

/*static*/
std::list<Operation *> TransactionBuilder::shuffled_operation_list(
        const std::vector<Transaction *> & transactions) {
    std::vector<
        std::pair<
            const Transaction *,
            std::list<Accessor *>::const_iterator>> merge;
    for(auto x : transactions)
        if(!x->operations()->empty())
            merge.push_back(std::make_pair(x, x->operations()->cbegin()));
    std::list<Operation *> ret;
    while(!merge.empty()) {
        auto it = merge.begin() + std::rand() % merge.size();
        if(it->second != it->first->operations()->cend())
            ret.push_back(*it->second++);
        else {
            ret.push_back(it->first->ending());
            merge.erase(it);
        }
    }
    return ret;
}

/*static*/
bool TransactionBuilder::parse_operation(
        const std::string & s,
        std::string * name,
        std::string * op,
        std::string * var) {
    size_t p = s.find('.');
    if(p == std::string::npos || p == 0 || p == s.length() - 1)
        return false;
    static const std::string ALLOWED = "carw";
    if(ALLOWED.find(s[p + 1]) == std::string::npos)
        return false;
    if((s[p + 1] == 'r' || s[p + 1] == 'w') && p == s.length() - 2)
        return false;
    *name = s.substr(0, p);
    *op = std::string(1, s[p + 1]);
    *var = s.substr(p + 2);
    return true;
}

std::istream & operator>>(
        std::istream & is, std::vector<Transaction *> & value) {
    std::string word, name, operation, variable;
    while(is >> word) {
        if(!TransactionBuilder::parse_operation(
                word, &name, &operation, &variable)) {
            is.setstate(std::ios_base::failbit);
            for(auto x : value)
                delete x;
            value.clear();
            return is;
        }
        auto name_cmp = [&name](const Transaction * x)
            {return x->name() == name;};
        auto it = std::find_if(std::begin(value), std::end(value), name_cmp);
        if(it == std::end(value))
            it = value.insert(it, new Transaction(name));
        auto t = *it;
        if(operation == "c")
            t->set_ending(new Commit(t));
        else if(operation == "a")
            t->set_ending(new Abort(t));
        else if(operation == "r")
            t->operations()->push_back(new Read(t, variable));
        else if(operation == "w")
            t->operations()->push_back(new Write(t, variable));
        else
            assert(false);
    }
    if(is.rdstate() == (std::ios_base::eofbit | std::ios_base::failbit))
        is.clear(std::ios_base::eofbit);
    return is;
}
