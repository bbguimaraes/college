#include "gui/MainWindow.h"

#include <QListWidget>
#include <QVBoxLayout>
#include <QTimer>

#include "gui/GameDisplay.h"
#include "gui/SelectedPanel.h"
#include "model/Character.h"
#include "model/Player.h"

MainWindow::MainWindow(QWidget * parent, GameDisplay * display)
        : QWidget(parent) {
    this->m_display = display;
    this->init();
    this->m_timer = new QTimer(this);
    connect(
        this->m_timer, SIGNAL(timeout()),
        this->m_display, SLOT(update_display()));
}

void MainWindow::init() {
    this->m_selected_panel = new SelectedPanel(this);
    this->m_selected_panel->setMinimumSize(155, 0);
    QFrame * left_panel = new QFrame(this);
    left_panel->setFrameStyle(QFrame::Box | QFrame::Plain);
    this->m_list = new QListWidget(this);
    QVBoxLayout * left_layout = new QVBoxLayout;
    left_layout->addWidget(this->m_selected_panel);
    left_layout->addStretch(1);
    left_layout->addWidget(this->m_list);
    left_panel->setLayout(left_layout);
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(this->m_display, 1);
    layout->addWidget(left_panel);
    this->setLayout(layout);
}

void MainWindow::character_select(const Character * c) {
    this->m_selected_panel->set_selected(c);
}

void MainWindow::character_move(const Character * c, const Point & orig) {
    this->m_list->insertItem(
        this->m_list->count(),
        QString("%1 %2 moved: (%3,%4) -> (%5,%6)")
            .arg(c->owner()->id() == 1 ? "red" : "blue")
            .arg(c->name().c_str())
            .arg(orig.x())
            .arg(orig.y())
            .arg(c->position().x())
            .arg(c->position().y()));
}

void MainWindow::character_attack(const Character * c, const Point & orig) {
    this->m_list->insertItem(
        this->m_list->count(),
        QString("%1 %2 attacked: (%3,%4) -> (%5,%6)")
            .arg(c->owner()->id() == 1 ? "red" : "blue")
            .arg(c->name().c_str())
            .arg(orig.x())
            .arg(orig.y())
            .arg(c->position().x())
            .arg(c->position().y()));
}

/*virtual*/
void MainWindow::closeEvent(QCloseEvent * /*event*/) {
    this->m_display->send_quit_message();
}
