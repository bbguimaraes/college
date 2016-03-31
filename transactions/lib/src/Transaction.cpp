#include "Transaction.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "Operation.h"

Transaction::Transaction(
        const std::string & name,
        const std::list<Accessor *> & operations,
        const Ending * ending)
            : m_name(name), m_ending(nullptr) {
    this->set_operations(operations);
    this->set_ending(ending);
}

Transaction::~Transaction() {
    this->release_operations();
    delete this->m_ending;
}

void Transaction::release_operations() {
    std::for_each(
        this->m_operations.cbegin(),
        this->m_operations.cend(),
        [](const Accessor * x){delete x;});
    this->m_operations.clear();
}

void Transaction::set_operations(const std::list<Accessor *> & operations) {
    this->release_operations();
    for(auto x : operations) {
        this->m_operations.push_back(static_cast<Accessor *>(x->clone()));
        this->m_operations.back()->set_transaction(this);
    }
}

void Transaction::set_ending(const Ending * ending) {
    delete this->m_ending;
    if(ending == nullptr)
        this->m_ending = nullptr;
    else {
        this->m_ending = static_cast<Ending *>(ending->clone());
        this->m_ending->set_transaction(this);
    }
}

void Transaction::shuffle() {
    std::vector<Accessor *> shuffled(
        this->m_operations.cbegin(),
        --this->m_operations.cend());
    std::random_shuffle(shuffled.begin(), shuffled.end());
    std::copy(shuffled.cbegin(), shuffled.cend(), this->m_operations.begin());
}

std::ostream & operator <<(std::ostream & os, Transaction & t) {
    os
        << "Transaction[" << &t << "]: "
        << "name(" << t.m_name << "), "
        << "operations{";
    auto it = t.m_operations.cbegin();
    if(it != t.m_operations.end()) {
        os << std::string(**it);
        ++it;
    }
    for(; it != t.m_operations.end(); ++it)
        os << ", " << std::string(**it);
    os << "}, ending(";
    if(t.ending())
        os << std::string(*t.ending());
    else
        os << t.ending();
    os << ")";
    return os;
}
