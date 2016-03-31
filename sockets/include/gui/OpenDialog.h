#ifndef OPEN_DIALOG_H
#define OPEN_DIALOG_H

#include <string>

#include <QDialog>

class QCheckBox;
class QLineEdit;
class QPushButton;

class OpenDialog : public QDialog {
    Q_OBJECT

    public:
        OpenDialog(QWidget * parent = nullptr);

        std::string name() const;
        bool is_server() const;
        std::string addr() const;

    public slots:
        void set_server_mode();

    private:
        void init();
        QLineEdit * m_name_edit;
        QCheckBox * m_is_server_check;
        QLineEdit * m_addr_edit;
        QPushButton * m_ok_button;
};

#endif // OPEN_DIALOG_H
