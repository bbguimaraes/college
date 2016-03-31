#include "SleepingPanel.h"

#include <sstream>

#include "Transaction.h"

void SleepingPanel::update_text(
        const std::map<std::string, std::list<Transaction*>> & sleeping) {
    this->setText(this->generate_text(sleeping));
}

QString SleepingPanel::generate_text(
        const std::map<std::string, std::list<Transaction*>> & sleeping) {
    std::stringstream ss;
    ss << "Sleeping:";
    for(auto x : sleeping) {
        ss << "\n" << x.first << ":";
        for(auto y : x.second)
            ss << " " << y->name();
    }
    return ss.str().c_str();
}
