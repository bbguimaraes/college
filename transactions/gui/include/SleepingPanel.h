#ifndef SLEEPING_PANEL_H
#define SLEEPING_PANEL_H

#include <map>
#include <list>
#include <string>

#include <QLabel>

class Transaction;

class SleepingPanel : public QLabel {
    public:
        SleepingPanel(QWidget * parent = nullptr)
            : QLabel(parent) {this->update_text();}
        void update_text(
            const std::map<std::string, std::list<Transaction *>> & sleeping =
                std::map<std::string, std::list<Transaction *>>());

    private:
        QString generate_text(
            const std::map<std::string, std::list<Transaction *>> & sleeping);
};

#endif // SLEEPING_PANEL_H
