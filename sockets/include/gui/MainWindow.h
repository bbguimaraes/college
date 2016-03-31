#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>

class QListWidget;
class QTimer;
class GameDisplay;
class SelectedPanel;
class Character;
class Point;

class MainWindow : public QWidget {
    Q_OBJECT

    public:
        MainWindow(QWidget * parent, GameDisplay * display);

        QTimer * timer() const {return this->m_timer;}

    public slots:
        void character_select(const Character * c);
        void character_move(const Character * c, const Point & orig);
        void character_attack(const Character * c, const Point & orig);

    protected:
        virtual void closeEvent(QCloseEvent * event);

    private:
        void init();

        GameDisplay * m_display;
        SelectedPanel * m_selected_panel;
        QListWidget * m_list;
        QTimer * m_timer;
};

#endif // MAIN_WINDOW_H
