#ifndef LOCKED_PANEL_H
#define LOCKED_PANEL_H

#include <map>
#include <string>

#include <QLabel>

class Transaction;

class LockedPanel : public QLabel {
    public:
        LockedPanel(QWidget * parent = nullptr)
            : QLabel(parent) {this->update_text();}
        void update_text(
            const std::map<std::string, int> & locked =
                std::map<std::string, int>());

    private:
        QString generate_text(const std::map<std::string, int> & locked) const;
};

#endif // LOCKED_PANEL_H
