#ifndef LOCK_TABLE_H
#define LOCK_TABLE_H

#include <map>
#include <list>
#include <string>

class Transaction;

class LockTable {
    typedef std::list<Transaction *> TransactionList;
    typedef std::map<std::string, int> LockMap;
    typedef std::map<std::string, TransactionList> VariableMap;

    public:
        bool xlock(const std::string & variable, Transaction * t);
        bool slock(const std::string & variable, Transaction * t);
        void xunlock(const std::string & variable);
        void sunlock(const std::string & variable, Transaction * t);
        void sleep(const std::string & variable, Transaction * t);
        bool sleeping(const Transaction & t) const;
        const LockMap * locked() const {return &this->m_locked;}
        const VariableMap * locks() const {return &this->m_locks;}
        const VariableMap * sleeping() const {return &this->m_sleeping;}

    private:
        bool has_xlock(const std::string & variable, Transaction * t) const;
        bool has_slock(const std::string & variable, Transaction * t) const;
        bool has_lock(const std::string & variable, Transaction * t) const;
        void wake(const std::string & variable);

        LockMap m_locked;
        VariableMap m_locks;
        VariableMap m_sleeping;
};

#endif // LOCK_TABLE_H
