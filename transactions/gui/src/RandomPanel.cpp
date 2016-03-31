#include "RandomPanel.h"

#include <QHBoxLayout>
#include <QLabel>

RandomPanel::RandomPanel(QWidget * parent)
    : QWidget(parent) {
    this->init();
    this->m_transaction_spin->setValue(5);
    this->m_variable_spin->setValue(10);
    this->m_min_operation_spin->setValue(5);
    this->m_max_operation_spin->setValue(10);
}

void RandomPanel::init() {
    this->m_transaction_spin = new QSpinBox(this);
    this->m_variable_spin = new QSpinBox(this);
    this->m_min_operation_spin = new QSpinBox(this);
    this->m_max_operation_spin = new QSpinBox(this);
    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("Trans.:", this));
    layout->addWidget(this->m_transaction_spin);
    layout->addWidget(new QLabel("Var.:", this));
    layout->addWidget(this->m_variable_spin);
    layout->addWidget(new QLabel("Min. op.:", this));
    layout->addWidget(this->m_min_operation_spin);
    layout->addWidget(new QLabel("Max. op.:", this));
    layout->addWidget(this->m_max_operation_spin);
    setLayout(layout);
}
