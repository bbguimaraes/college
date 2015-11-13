#include "include/weightdialog.h"

#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>

WeightDialog::WeightDialog(QWidget * parent) :
        QDialog(parent), m_size(3), m_weights(this->generate_weights()) {
    init();
    connect(
        this->ok_button, SIGNAL(clicked()),
        this, SLOT(ok_button_clicked()));
    connect(
        this->cancel_button, SIGNAL(clicked()),
        this, SLOT(reject()));
}

std::vector<std::vector<float>> WeightDialog::generate_weights() {
    return std::vector<std::vector<float>>(
        this->m_size,
        std::vector<float>(
            this->m_size, 1.0f / (float) (this->m_size * this->m_size)));
}

void WeightDialog::init() {
    QLineEdit * number_text = new QLineEdit;
    this->ok_button = new QPushButton("OK");
    this->cancel_button = new QPushButton("Cancel");
    number_text->setText(QString::number(this->m_size));
    this->grid_layout = new QGridLayout;
    update_size();
    QHBoxLayout * button_layout = new QHBoxLayout;
    button_layout->addStretch(2);
    button_layout->addWidget(this->ok_button);
    button_layout->addStretch(2);
    button_layout->addWidget(this->cancel_button);
    button_layout->addStretch(2);
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(number_text);
    layout->addLayout(this->grid_layout);
    layout->addLayout(button_layout);
    setLayout(layout);
    connect(
        number_text, SIGNAL(textChanged(const QString &)),
        this, SLOT(number_text_changed(const QString &)));
}

void WeightDialog::update_size() {
    while(this->grid_layout->count() != 0) {
        QLayoutItem * item = this->grid_layout->itemAt(0);
        delete item->widget();
        this->grid_layout->removeItem(item);
    }
    const QString WEIGHT_TEXT = QString::number(
        1.0f / (this->m_size * this->m_size));
    unsigned int i, j;
    for(i = 0; i < this->m_size; ++i) {
        for(j = 0; j < this->m_size; ++j) {
            QLineEdit * line_edit = new QLineEdit;
            line_edit->setText(WEIGHT_TEXT);
            this->grid_layout->addWidget(line_edit, i, j);
        }
    }
    this->adjustSize();
}

void WeightDialog::number_text_changed(const QString & text) {
    bool ok;
    unsigned int temp = text.toUInt(&ok);
    if(!ok)
        return;
    this->m_size = temp;
    this->update_size();
}

void WeightDialog::ok_button_clicked() {
    if(this->m_size % 2 != 1) {
        QMessageBox::critical(this, "Invalid value", "Size must be odd");
        return;
    }
    std::vector<std::vector<float>> new_weights(
        this->m_size, std::vector<float>(this->m_size));
    for(unsigned int i = 0; i < this->m_size; ++i)
        for(unsigned int j = 0; j < this->m_size; ++j) {
            QLayoutItem * item = this->grid_layout->itemAtPosition(i, j);
            QLineEdit * line_edit = dynamic_cast<QLineEdit *>(item->widget());
            bool ok;
            float temp = line_edit->text().toFloat(&ok);
            if(!ok) {
                QMessageBox::critical(
                    this, "Invalid value",
                    "\"" + line_edit->text() + "\" on " + QString::number(i)
                        + ", " + QString::number(j));
                return;
            }
            new_weights[i][j] = temp;
        }
    this->m_weights = new_weights;
    this->accept();
}
