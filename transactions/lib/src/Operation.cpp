#include "Operation.h"

#include <sstream>

#include "LockTable.h"
#include "Transaction.h"

/*virtual*/
Abort::operator std::string() const {
    std::stringstream ss;
    ss
        << "Abort[" << this << "]: transaction("
        << this->transaction()->name() << ")";
    return ss.str();
}

/*virtual*/
bool Abort::perform(LockTable * locks) {
    for(auto x : *this->transaction()->operations())
        x->release(locks);
    return true;
}

/*virtual*/
Commit::operator std::string() const {
    std::stringstream ss;
    ss
        << "Commit[" << this << "]: transaction("
        << this->transaction()->name() << ")";
    return ss.str();
}

/*virtual*/
bool Commit::perform(LockTable * locks) {
    for(auto x : *this->transaction()->operations())
        x->release(locks);
    return true;
}

/*virtual*/
Read::operator std::string() const {
    std::stringstream ss;
    ss
        << "Read[" << this << "]: "
        << "transaction(" << this->transaction() << "), "
        << "variable(" << this->variable() << ")";
    return ss.str();
}

/*virtual*/
bool Read::perform(LockTable * locks) {
    if(locks->slock(this->variable(), this->transaction()))
        return true;
    locks->sleep(this->variable(), this->transaction());
    return false;
}

/*virtual*/
void Read::release(LockTable * locks) {
    locks->sunlock(this->variable(), this->transaction());
}

/*virtual*/
Write::operator std::string() const {
    std::stringstream ss;
    ss
        << "Write[" << this << "]: " <<
        "transaction(" << this->transaction()->name() << "), " <<
        "variable(" << this->variable() << ")";
    return ss.str();
}

/*virtual*/
bool Write::perform(LockTable * locks) {
    if(locks->xlock(this->variable(), this->transaction()))
        return true;
    locks->sleep(this->variable(), this->transaction());
    return false;
}

/*virtual*/
void Write::release(LockTable * locks) {
    locks->xunlock(this->variable());
}
