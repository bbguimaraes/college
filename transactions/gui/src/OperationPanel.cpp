#include "OperationPanel.h"

#include <cmath>

#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>

#include "Operation.h"
#include "Transaction.h"

OperationPanel::OperationPanel(QWidget * parent)
        : QFrame(parent), m_list_current(nullptr), m_line_current(nullptr) {
    this->init();
    this->m_selected_palette.setColor(QPalette::Foreground, Qt::red);
    this->m_done_palette.setColor(QPalette::Foreground, Qt::darkGreen);
    this->m_sleeping_palette.setColor(QPalette::Foreground, Qt::darkYellow);
}

void OperationPanel::init() {
    this->m_list_layout = new QVBoxLayout;
    this->m_line_layout= new QVBoxLayout;
    QGroupBox * list_group = new QGroupBox("Operations", this);
    list_group->setLayout(m_list_layout);
    QGroupBox * line_group = new QGroupBox("Transactions", this);
    line_group->setLayout(m_line_layout);
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(list_group);
    layout->addStretch(1);
    layout->addWidget(line_group);
    this->setLayout(layout);
}

void OperationPanel::clear_operations() {
    this->m_list_current = nullptr;
    this->m_line_current = nullptr;
    QLayoutItem * item;
    while((item = this->m_list_layout->takeAt(0))) {
        auto item_layout = static_cast<QHBoxLayout *>(item->layout());
        QLayoutItem * item2;
        while((item2 = item_layout->takeAt(0))) {
            delete item2->widget();
            delete item2;
        }
        delete item;
    }
    while((item = this->m_line_layout->takeAt(0))) {
        auto item_layout = static_cast<QHBoxLayout *>(item->layout());
        QLayoutItem * item2;
        while((item2 = item_layout->takeAt(0))) {
            delete item2->widget();
            delete item2;
        }
        delete item;
    }
}

void OperationPanel::set_operations(const std::list<Operation *> & operations) {
    this->clear_operations();
    this->create_list(operations);
    this->create_lines(operations);
}

void OperationPanel::create_list(const std::list<Operation *> & operations) {
    const unsigned int COUNT = operations.size();
    const unsigned int SIDE = std::sqrt(COUNT);
    auto it = std::begin(operations);
    for(unsigned int j = 0; j < SIDE; ++j)
        this->create_list_line(&it, SIDE);
    unsigned int rest = COUNT - SIDE * SIDE;
    if(rest)
        this->create_list_line(&it, rest);
}

void OperationPanel::create_list_line(
        std::list<Operation *>::const_iterator * it,
        unsigned int n) {
    auto line = new QHBoxLayout;
    this->m_list_layout->addLayout(line);
    for(unsigned int i = 0; i < n; ++i, ++*it) {
        auto label = this->create_label(**it);
        line->addWidget(label);
        this->m_list_labels[**it] = label;
    }
}

void OperationPanel::create_lines(const std::list<Operation *> & operations) {
    std::map<const Transaction *, QHBoxLayout *> lines;
    for(auto x : operations) {
        auto line = lines[x->transaction()];
        if(!line) {
            line = new QHBoxLayout;
            lines[x->transaction()] = line;
            this->m_line_layout->addLayout(line);
            line->addWidget(
                new QLabel(
                    QString("%1:").arg(x->transaction()->name().c_str())));
        }
        auto label = this->create_label(x);
        line->addWidget(label, 1);
        this->m_line_labels[x] = label;
    }
}

QLabel * OperationPanel::create_label(const Operation * op) {
    auto label = new QLabel(
        QString("%1.%2").arg(
            op->transaction()->name().c_str(),
            op->message().c_str()),
        this);
    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    return label;
}

void OperationPanel::set_current(const Operation * op, bool current_sleeping) {
    auto palette = current_sleeping
        ? this->m_sleeping_palette
        : this->m_done_palette;
    if(this->m_list_current)
        this->m_list_current->setPalette(palette);
    if(this->m_line_current)
        this->m_line_current->setPalette(palette);
    if(!op)
        return;
    this->m_list_current = this->m_list_labels[op];
    this->m_list_current->setPalette(this->m_selected_palette);
    this->m_line_current = this->m_line_labels[op];
    this->m_line_current->setPalette(this->m_selected_palette);
}
