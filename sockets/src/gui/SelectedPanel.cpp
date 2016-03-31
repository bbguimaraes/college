#include "gui/SelectedPanel.h"

#include <QLabel>
#include <QVBoxLayout>

#include "model/Character.h"

SelectedPanel::SelectedPanel(QWidget * parent)
    : QWidget(parent) {
    this->init();
}

void SelectedPanel::init() {
    this->m_name_label = new QLabel;
    this->m_value_label = new QLabel;
    this->clear_fields();
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(this->m_name_label);
    layout->addWidget(this->m_value_label);
    this->setLayout(layout);
}

void SelectedPanel::clear_fields() {
    this->m_name_label->setText("Name:");
    this->m_value_label->setText("Value:");
}

void SelectedPanel::set_selected(const Character * selected) {
    if(!selected)
        this->clear_fields();
    else {
        this->m_name_label->setText(
            ("Name: " + selected->name()).c_str());
        this->m_value_label->setText(
            ("Value: " + std::to_string(selected->value())).c_str());
    }
}
