#include "gui/OpenDialog.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

OpenDialog::OpenDialog(QWidget * parent) : QDialog(parent) {
    this->init();
    this->m_name_edit->setText("billy");
    this->m_addr_edit->setText("127.0.0.1");
    this->m_is_server_check->setChecked(true);
    connect(
        this->m_ok_button, SIGNAL(clicked()),
        this, SLOT(accept()));
    connect(
        this->m_is_server_check, SIGNAL(stateChanged(int)),
        this, SLOT(set_server_mode()));
}

void OpenDialog::init() {
    QLabel * name_label = new QLabel;
    name_label->setText("Name:");
    this->m_name_edit = new QLineEdit;
    this->m_addr_edit = new QLineEdit;
    this->m_name_edit->setFocus();
    this->m_name_edit->selectAll();
    this->m_addr_edit->hide();
    this->m_is_server_check = new QCheckBox;
    this->m_is_server_check->setText("Server");
    this->m_ok_button = new QPushButton;
    this->m_ok_button->setText("OK");
    QHBoxLayout * button_layout = new QHBoxLayout;
    button_layout->addStretch(1);
    button_layout->addWidget(this->m_ok_button);
    button_layout->addStretch(1);
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(name_label);
    layout->addWidget(this->m_name_edit);
    layout->addWidget(this->m_is_server_check);
    layout->addWidget(this->m_addr_edit);
    layout->addLayout(button_layout);
    this->setLayout(layout);
    this->setMinimumSize(600, this->minimumSize().height());
}

std::string OpenDialog::name() const {
    return this->m_name_edit->text().toStdString();
}

bool OpenDialog::is_server() const {
    return this->m_is_server_check->isChecked();
}

std::string OpenDialog::addr() const {
    return this->m_addr_edit->text().toStdString();
}

void OpenDialog::set_server_mode() {
    this->m_addr_edit->setVisible(!this->m_is_server_check->isChecked());
    this->adjustSize();
}
