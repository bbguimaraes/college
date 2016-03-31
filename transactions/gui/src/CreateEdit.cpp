#include "CreateEdit.h"

#include <fstream>
#include <sstream>

#include <QFileInfo>

#include "Transaction.h"
#include "TransactionBuilder.h"

std::vector<Transaction *> CreateEdit::transactions() const {
    auto text = this->text();
    if(text.isEmpty())
        return std::vector<Transaction *>();
    if(QFileInfo(text).exists()) {
        auto is = std::ifstream(text.toStdString());
        return this->read_transactions(&is);
    } else {
        auto is = std::istringstream(text.toStdString());
        return this->read_transactions(&is);
    }
}

std::vector<Transaction *>
CreateEdit::read_transactions(std::istream * is) const {
    std::vector<Transaction *> ret;
    if(!*is)
        return ret;
    *is >> ret;
    return ret;
}
