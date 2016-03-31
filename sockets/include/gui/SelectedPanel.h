#ifndef SELECTED_PANEL_H
#define SELECTED_PANEL_H

#include <QWidget>

class Character;
class QLabel;

class SelectedPanel : public QWidget {
    Q_OBJECT

    public:
        SelectedPanel(QWidget * parent = 0);

    public slots:
        void set_selected(const Character * selected);

    private:
        void init();
        void clear_fields();

        QLabel * m_name_label;
        QLabel * m_value_label;
};

#endif // SELECTED_PANEL_H

