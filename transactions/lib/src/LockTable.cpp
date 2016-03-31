#include "LockTable.h"

#include <algorithm>

#include "Operation.h"
#include "Transaction.h"

bool LockTable::xlock(const std::string & variable, Transaction * t) {
    if(this->has_xlock(variable, t))
        return true;
    auto it = this->m_locks.find(variable);
    auto locked = it != std::end(this->m_locks);
    if(locked && (it->second.front() != t || it->second.size() != 1))
        return false;
    this->m_locked[variable] = -1;
    if(!locked)
        this->m_locks[variable] = TransactionList(1, t);
    return true;
}

bool LockTable::slock(const std::string & variable, Transaction * t) {
    if(this->has_lock(variable, t))
        return true;
    auto it = this->m_locked.find(variable);
    if(it != std::end(this->m_locked) && it->second == -1)
        return false;
    this->m_locked[variable]++;
    this->m_locks[variable].push_back(t);
    return true;
}

bool LockTable::has_xlock(
        const std::string & variable, Transaction * t) const {
    auto it = this->m_locked.find(variable);
    return it != std::end(this->m_locked)
        && it->second == -1
        && this->m_locks.at(variable).front() == t;
}

bool LockTable::has_slock(
        const std::string & variable, Transaction * t) const {
    auto it = m_locked.find(variable);
    if(it == std::end(this->m_locked) || it->second != -1)
        return false;
    auto ts = this->m_locks.find(variable)->second;
    return std::find(std::begin(ts), std::end(ts), t) != std::end(ts);
}

bool LockTable::has_lock(const std::string & variable, Transaction * t) const {
    auto it = this->m_locks.find(variable);
    if(it == std::end(this->m_locks))
        return false;
    auto ts = it->second;
    return std::find(std::begin(ts), std::end(ts), t) != std::end(ts);
}

void LockTable::xunlock(const std::string & variable) {
    this->m_locked.erase(variable);
    this->m_locks.erase(variable);
    this->wake(variable);
}

void LockTable::sunlock(const std::string & variable, Transaction * t) {
    auto it = this->m_locked.find(variable);
    if(it == std::end(this->m_locked) || it->second == -1)
        return;
    if(it->second == 1) {
        this->m_locked.erase(it);
        this->m_locks.erase(variable);
    } else {
        --it->second;
        this->m_locks.find(variable)->second.remove(t);
    }
    this->wake(variable);
}

void LockTable::wake(const std::string & variable) {
    this->m_sleeping.erase(variable);
}

void LockTable::sleep(const std::string & variable, Transaction * t) {
    this->m_sleeping[variable].push_back(t);
}

bool LockTable::sleeping(const Transaction & t) const {
    return std::any_of(
        std::begin(this->m_sleeping),
        std::end(this->m_sleeping),
        [&t](const VariableMap::value_type & x) {
            return std::find(std::begin(x.second), std::end(x.second), &t)
                != std::end(x.second);
            });
}
