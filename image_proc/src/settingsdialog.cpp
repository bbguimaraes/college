#include "include/settingsdialog.h"

#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget * parent, ChartView::Mode mode)
        : QDialog(parent), m_mode(mode) {
    init();
    this->chart_line_radio->toggle();
    connect(
        this->ok_button, SIGNAL(clicked()),
        this, SLOT(ok_button_clicked()));
    connect(
        this->cancel_button, SIGNAL(clicked()),
        this, SLOT(reject()));
}

void SettingsDialog::init() {
    this->chart_line_radio = new QRadioButton("Lines");
    this->chart_bars_radio = new QRadioButton("Bars");
    auto group_layout = new QVBoxLayout;
    group_layout->addWidget(this->chart_line_radio);
    group_layout->addWidget(this->chart_bars_radio);
    this->chart_mode_group = new QGroupBox(this);
    this->chart_mode_group->setLayout(group_layout);
    this->ok_button = new QPushButton("OK");
    this->cancel_button = new QPushButton("Cancel");
    auto button_layout = new QHBoxLayout;
    button_layout->addWidget(this->ok_button);
    button_layout->addWidget(this->cancel_button);
    auto layout = new QVBoxLayout;
    layout->addWidget(this->chart_mode_group);
    layout->addLayout(button_layout);
    setLayout(layout);
}

void SettingsDialog::ok_button_clicked() {
    if(this->chart_line_radio->isChecked())
        this->m_mode = ChartView::Mode::LINE;
    else if(this->chart_bars_radio->isChecked())
        this->m_mode = ChartView::Mode::BARS;
    this->accept();
}
