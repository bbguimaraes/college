#ifndef CREATE_EDIT_H
#define CREATE_EDIT_H

#include <vector>

#include <QLineEdit>

class Transaction;

class CreateEdit : public QLineEdit {
    public:
        CreateEdit(QWidget * parent = nullptr) : QLineEdit(parent) {}
        std::vector<Transaction *> transactions() const;

    private:
        std::vector<Transaction *> read_transactions(std::istream * is) const;
};

#endif // CREATE_EDIT_H
