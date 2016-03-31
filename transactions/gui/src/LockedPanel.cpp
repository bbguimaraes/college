#include "LockedPanel.h"

#include <sstream>

#include "Transaction.h"

void LockedPanel::update_text(const std::map<std::string, int> & locked) {
    this->setText(this->generate_text(locked));
}

QString LockedPanel::generate_text(
        const std::map<std::string, int> & locked) const {
    std::stringstream ss;
    ss << "Locked:";
    for(auto x : locked) {
        ss << "\n" << x.first << ": ";
        if(x.second == -1)
            ss << "w";
        else
            ss << "r" << x.second;
    }
    return ss.str().c_str();
}
