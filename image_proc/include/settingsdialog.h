#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

#include "include/chartview.h"

class QGroupBox;
class QPushButton;
class QRadioButton;

class SettingsDialog : public QDialog {
    Q_OBJECT

    public:
        SettingsDialog(
            QWidget * parent = nullptr,
            ChartView::Mode mode = ChartView::Mode::LINE);
        ChartView::Mode mode() const {return this->m_mode;}

    private slots:
        void ok_button_clicked();

    private:
        void init();
        QGroupBox * chart_mode_group;
        QRadioButton * chart_line_radio;
        QRadioButton * chart_bars_radio;
        QPushButton * ok_button;
        QPushButton * cancel_button;
        ChartView::Mode m_mode;
};

#endif // SETTINGS_DIALOG_H
