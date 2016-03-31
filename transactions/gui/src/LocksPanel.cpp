#include "LocksPanel.h"

#include <sstream>

#include "Transaction.h"

void LocksPanel::update_text(
        const std::map<std::string, std::list<Transaction *>> & locks) {
    this->setText(this->generate_text(locks));
}

QString LocksPanel::generate_text(
        const std::map<std::string, std::list<Transaction *>> & locks) const {
    std::stringstream ss;
    ss << "Locks:";
    for(auto x : locks) {
        ss << "\n" << x.first << ":";
        for(auto y : x.second)
            ss << " " << y->name();
    }
    return ss.str().c_str();
}
