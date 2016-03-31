#ifndef LOCKS_PANEL_H
#define LOCKS_PANEL_H

#include <map>
#include <list>
#include <string>

#include <QLabel>

class Transaction;

class LocksPanel : public QLabel {
    public:
        LocksPanel(QWidget * parent = nullptr)
            : QLabel(parent) {this->update_text();}
        void update_text(
            const std::map<std::string, std::list<Transaction *>> & locks =
                std::map<std::string, std::list<Transaction *>>());

    private:
        QString generate_text(
            const std::map<std::string,
            std::list<Transaction *>> & locks) const;
};

#endif // LOCKS_PANEL_H
