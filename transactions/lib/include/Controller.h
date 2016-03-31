#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <list>

#include "LockTable.h"

class Transaction;
class Operation;

class Controller {
    typedef std::list<Operation *> OperationList;

    public:
        Controller(const OperationList & operations = OperationList());

        const OperationList * operations() const {return &this->m_operations;}
        const OperationList * remaining() const {return &this->m_remaining;}
        const LockTable * locks() const {return &this->m_locks;}
        const Operation * current() const;
        bool done() const {return this->m_remaining.empty();}
        bool deadlocked() const;

        void set_operations(const OperationList & operations);

        void run();
        void step();
        void reset();

    private:
        OperationList::const_iterator next(void) const;

        OperationList m_operations;
        OperationList m_remaining;
        OperationList::const_iterator m_current;
        LockTable m_locks;

        friend std::ostream & operator <<(
            std::ostream & os, const Controller & controller);
};

std::ostream & operator <<(std::ostream & os, const Controller & controller);

#endif // CONTROLLER_H
