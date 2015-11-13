#ifndef WEIGHT_DIALOG_H
#define WEIGHT_DIALOG_H

#include <vector>

#include <QDialog>

class QGridLayout;
class QPushButton;

class WeightDialog : public QDialog {
    Q_OBJECT

    public:
        WeightDialog(QWidget * parent = 0);
        std::vector<std::vector<float>> weights() const {return m_weights;}

    private slots:
        void number_text_changed(const QString & text);
        void ok_button_clicked();

    private:
        void init();
        void update_size();
        std::vector<std::vector<float>> generate_weights();
        unsigned int m_size;
        std::vector<std::vector<float>> m_weights;
        QGridLayout * grid_layout;
        QPushButton * ok_button;
        QPushButton * cancel_button;
};

#endif // WEIGHT_DIALOG_H
