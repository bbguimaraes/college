#include "Controller.h"

#include <algorithm>
#include <iostream>

#include "Operation.h"
#include "Transaction.h"

Controller::Controller(const OperationList & operations) {
    this->set_operations(operations);
}

const Operation * Controller::current() const {
    return this->done() ? nullptr : *this->m_current;
}

bool Controller::deadlocked(void) const {
    return !this->done() && this->m_current == std::end(this->m_remaining);
}

void Controller::set_operations(const std::list<Operation *> & operations) {
    this->m_operations = operations;
    this->reset();
}

std::list<Operation *>::const_iterator Controller::next() const {
    return std::find_if(
        this->m_remaining.cbegin(),
        this->m_remaining.cend(),
        [this](const Operation * x) {
            return !this->m_locks.sleeping(*x->transaction());
        });
}

void Controller::reset() {
    this->m_locks = LockTable();
    this->m_remaining = this->m_operations;
    this->m_current = this->m_remaining.begin();
}

void Controller::run() {
    while(this->m_current != this->m_remaining.end())
        this->step();
}

void Controller::step() {
    if(this->m_current == this->m_remaining.end())
        return;
    if((*this->m_current)->perform(&this->m_locks))
        this->m_remaining.erase(this->m_current);
    this->m_current = this->next();
}

std::ostream & operator <<(std::ostream & os, const Controller & controller) {
    for(auto x : controller.m_operations)
        os << x->transaction()->name() << " " << x->message() << "\n";
    return os;
}
