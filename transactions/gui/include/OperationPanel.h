#ifndef OPERATION_PANEL_H
#define OPERATION_PANEL_H

#include <list>
#include <map>

#include <QFrame>

class QVBoxLayout;
class QLabel;

class Operation;

class OperationPanel : public QFrame {
    public:
        OperationPanel(QWidget * parent = nullptr);

        void set_operations(const std::list<Operation *> & operations);
        void set_current(const Operation * op, bool current_sleeping = false);

    private:
        void init();
        void create_list(const std::list<Operation *> & operations);
        void create_list_line(
            std::list<Operation *>::const_iterator * it,
            unsigned int n);
        void create_lines(const std::list<Operation *> & operations);
        QLabel * create_label(const Operation * op);
        void clear_operations();

        std::map<const Operation *, QLabel *> m_list_labels;
        std::map<const Operation *, QLabel *> m_line_labels;
        QVBoxLayout * m_list_layout;
        QVBoxLayout * m_line_layout;
        QLabel * m_list_current;
        QLabel * m_line_current;
        QPalette m_default_palette;
        QPalette m_selected_palette;
        QPalette m_done_palette;
        QPalette m_sleeping_palette;
};

#endif // OPERATION_PANEL_H
