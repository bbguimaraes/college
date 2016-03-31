#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <vector>

#include <QMainWindow>

#include "Controller.h"

class QPushButton;
class QCheckBox;

class CreateEdit;
class RandomPanel;
class OperationPanel;
class LockedPanel;
class LocksPanel;
class SleepingPanel;
class Transaction;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget * parent = nullptr);
        ~MainWindow();

    public slots:
        void create_transactions();
        void random_transactions();
        void clear_transactions();
        void step();
        void reset();

    private:
        void init();
        void update_displays(bool last_sleeping = false);
        void highlight_current_operation(bool last_sleeping = false);

        QPushButton * m_create_button;
        QPushButton * m_random_button;
        QPushButton * m_clear_button;
        QPushButton * m_step_button;
        QPushButton * m_reset_button;
        QCheckBox * m_suffle_check;

        CreateEdit * m_create_edit;
        RandomPanel * m_random_panel;
        OperationPanel * m_operation_panel;
        LockedPanel * m_locked_panel;
        LocksPanel * m_locks_panel;
        SleepingPanel * m_sleeping_panel;

        Controller m_controller;
        std::vector<Transaction *> m_transactions;
};

#endif // MAIN_WINDOW_H
