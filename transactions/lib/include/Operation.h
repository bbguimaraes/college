#ifndef OPERATION_H
#define OPERATION_H

#include <string>

class Transaction;
class LockTable;

class Operation {
    Transaction * m_transaction;

    public:
        Operation(Transaction * t = nullptr) : m_transaction(t) {}
        virtual ~Operation() {}

        void set_transaction(Transaction * t) {this->m_transaction = t;}
        const Transaction * transaction() const {return this->m_transaction;}
        Transaction * transaction() {return this->m_transaction;}

        virtual Operation * clone() const = 0;
        virtual operator std::string() const = 0;
        virtual std::string message() const = 0;

        virtual bool perform(LockTable *) = 0;
        virtual void release(LockTable *) {}
};

class Ending : public Operation {
    public:
        Ending() : Operation() {}
        Ending(Transaction * transaction) : Operation(transaction) {}
};

class Accessor : public Operation {
    std::string m_variable;

    public:
        Accessor() : Operation() {}
        Accessor(Transaction * transaction, const std::string & variable)
            : Operation(transaction), m_variable(variable) {}

        virtual void set_variable(const std::string & v)
            {this->m_variable = v;}
        virtual std::string variable(void) const {return m_variable;}
};

class Abort : public Ending {
    public:
        Abort() : Ending() {}
        Abort(Transaction * t) : Ending(t) {}

        virtual Operation * clone() const {return new Abort;}
        virtual operator std::string() const;

        virtual bool perform(LockTable * locks);
        virtual std::string message() const {return "a";}
};

class Commit : public Ending {
    public:
        Commit() : Ending() {}
        Commit(Transaction * t) : Ending(t) {}

        virtual Operation * clone() const {return new Commit;}
        virtual operator std::string() const;

        virtual bool perform(LockTable * locks);
        virtual std::string message() const {return "c";}
};

class Read : public Accessor {
    public:
        Read() : Accessor() {}
        Read(Transaction * transaction, const std::string & variable)
            : Accessor(transaction, variable) {}

        virtual Operation * clone() const
            {return new Read(nullptr, this->variable());}
        virtual operator std::string() const;

        virtual bool perform(LockTable * locks);
        virtual void release(LockTable * locks);
        virtual std::string message() const {return "r" + this->variable();}
};

class Write : public Accessor {
    public:
        Write() : Accessor() {}
        Write(Transaction * transaction, const std::string & variable)
            : Accessor(transaction, variable) {}

        virtual Operation * clone() const
            {return new Write(nullptr, this->variable());}
        virtual operator std::string() const;

        virtual bool perform(LockTable * locks);
        virtual void release(LockTable * locks);
        virtual std::string message() const {return "w" + this->variable();}
};

#endif // OPERATION_H
