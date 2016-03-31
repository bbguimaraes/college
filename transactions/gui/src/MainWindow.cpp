#include "MainWindow.h"

#include <QFrame>
#include <QCheckBox>
#include <QGroupBox>
#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "Operation.h"
#include "Transaction.h"
#include "TransactionBuilder.h"

#include "CreateEdit.h"
#include "LockedPanel.h"
#include "LocksPanel.h"
#include "OperationPanel.h"
#include "RandomPanel.h"
#include "SleepingPanel.h"

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent) {
    this->init();
    this->m_create_edit->setFocus();
    this->m_step_button->setEnabled(false);
    this->m_suffle_check->setChecked(true);
    connect(
        this->m_create_button, SIGNAL(clicked()),
        this, SLOT(create_transactions()));
    connect(
        this->m_random_button, SIGNAL(clicked()),
        this, SLOT(random_transactions()));
    connect(
        this->m_clear_button, SIGNAL(clicked()),
        this, SLOT(clear_transactions()));
    connect(
        this->m_step_button, SIGNAL(clicked()),
        this, SLOT(step()));
    connect(
        this->m_reset_button, SIGNAL(clicked()),
        this, SLOT(reset()));
}

MainWindow::~MainWindow() {
    for(auto x : this->m_transactions)
        delete x;
}

void MainWindow::init() {
    this->m_create_button = new QPushButton("Create", this);
    this->m_random_button = new QPushButton("Random", this);
    this->m_clear_button = new QPushButton("Clear", this);
    this->m_step_button = new QPushButton("Step", this);
    this->m_reset_button = new QPushButton("Reset", this);
    this->m_suffle_check = new QCheckBox("Shuffle", this);
    this->m_create_edit = new CreateEdit(this);
    this->m_random_panel = new RandomPanel(this);
    this->m_operation_panel = new OperationPanel(this);
    this->m_locked_panel = new LockedPanel(this);
    this->m_locks_panel = new LocksPanel(this);
    this->m_sleeping_panel = new SleepingPanel(this);
    this->m_create_edit->setMaximumHeight(30);
    QFrame * central_frame = new QFrame(this);
    QVBoxLayout * central_layout = new QVBoxLayout(central_frame);
    central_layout->addWidget(this->m_operation_panel);
    central_layout->addStretch(1);
    central_frame->setLayout(central_layout);
    QGroupBox * bottom_top_group = new QGroupBox("File or text", this);
    QHBoxLayout * bottom_top_layout = new QHBoxLayout(bottom_top_group);
    bottom_top_layout->addWidget(this->m_create_edit);
    bottom_top_layout->addWidget(this->m_create_button);
    bottom_top_layout->addWidget(this->m_suffle_check);
    bottom_top_group->setLayout(bottom_top_layout);
    QGroupBox * bottom_bottom_group = new QGroupBox("Random", this);
    QHBoxLayout * bottom_bottom_layout = new QHBoxLayout(bottom_bottom_group);
    bottom_bottom_layout->addWidget(this->m_random_panel);
    bottom_bottom_layout->addWidget(this->m_random_button);
    bottom_bottom_group->setLayout(bottom_bottom_layout);
    QFrame * bottom_frame = new QFrame(this);
    QVBoxLayout * bottom_layout = new QVBoxLayout(bottom_frame);
    bottom_layout->addWidget(bottom_top_group);
    bottom_layout->addWidget(bottom_bottom_group);
    bottom_frame->setLayout(bottom_layout);
    QFrame * right_frame = new QFrame(this);
    right_frame->setMinimumSize(200, right_frame->minimumSize().width());
    QVBoxLayout * right_layout = new QVBoxLayout(right_frame);
    right_layout->addWidget(this->m_locked_panel);
    right_layout->addWidget(this->m_locks_panel);
    right_layout->addWidget(this->m_sleeping_panel);
    right_layout->addStretch(1);
    right_frame->setLayout(right_layout);
    QFrame * left_frame = new QFrame(this);
    QVBoxLayout * left_layout = new QVBoxLayout(left_frame);
    left_layout->addWidget(this->m_step_button);
    left_layout->addWidget(this->m_reset_button);
    left_layout->addWidget(this->m_clear_button);
    left_layout->addStretch(2);
    left_frame->setLayout(left_layout);
    QDockWidget * bottom_dock = new QDockWidget(this);
    QDockWidget * right_dock = new QDockWidget(this);
    QDockWidget * left_dock = new QDockWidget(this);
    bottom_dock->setWindowTitle("Create transactions");
    right_dock->setWindowTitle("Locks");
    left_dock->setWindowTitle("Actions");
    bottom_dock->setWidget(bottom_frame);
    right_dock->setWidget(right_frame);
    left_dock->setWidget(left_frame);
    this->setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    this->addDockWidget(Qt::BottomDockWidgetArea, bottom_dock);
    this->addDockWidget(Qt::RightDockWidgetArea, right_dock);
    this->addDockWidget(Qt::LeftDockWidgetArea, left_dock);
    this->setCentralWidget(central_frame);
    const int FRAME_STYLE = QFrame::Panel | QFrame::Sunken;
    central_frame->setFrameStyle(FRAME_STYLE);
    bottom_frame->setFrameStyle(FRAME_STYLE);
    right_frame->setFrameStyle(FRAME_STYLE);
    left_frame->setFrameStyle(FRAME_STYLE);
    this->m_locked_panel->setFrameStyle(FRAME_STYLE);
    this->m_locks_panel->setFrameStyle(FRAME_STYLE);
    this->m_sleeping_panel->setFrameStyle(FRAME_STYLE);
}

void MainWindow::update_displays(bool last_sleeping) {
    this->highlight_current_operation(last_sleeping);
    this->m_locked_panel->update_text(
        *this->m_controller.locks()->locked());
    this->m_locks_panel->update_text(
        *this->m_controller.locks()->locks());
    this->m_sleeping_panel->update_text(
        *this->m_controller.locks()->sleeping());
    if(this->m_controller.done() || this->m_controller.deadlocked())
        this->m_step_button->setEnabled(false);
    else {
        this->m_step_button->setEnabled(true);
        this->m_step_button->setFocus();
    }
}

void MainWindow::highlight_current_operation(bool last_sleeping) {
    this->m_operation_panel->set_current(
        this->m_controller.current(), last_sleeping);
}

void MainWindow::create_transactions() {
    this->clear_transactions();
    if(!this->m_suffle_check->isChecked())
        this->m_controller.set_operations(
            TransactionBuilder::operation_list(
                this->m_create_edit->transactions()));
    else
        this->m_controller.set_operations(
            TransactionBuilder::shuffled_operation_list(
                this->m_create_edit->transactions()));
    this->m_operation_panel->set_operations(*this->m_controller.operations());
    this->update_displays();
}

void MainWindow::random_transactions() {
    this->clear_transactions();
    const unsigned int T_COUNT = this->m_random_panel->ntransactions();
    const unsigned int V_COUNT = this->m_random_panel->nvariables();
    const unsigned int MIN_OP = this->m_random_panel->min_operations();
    const unsigned int MAX_OP = this->m_random_panel->max_operations();
    const unsigned int DIFF = MAX_OP - MIN_OP;
    if(T_COUNT <= 0 || V_COUNT <= 0 || MAX_OP <= 0 || MIN_OP > MAX_OP)
        return;
    for(unsigned int i = 0; i < T_COUNT; ++i) {
        const int rand = MIN_OP + (DIFF >  0 ? random() % DIFF : 0);
        this->m_transactions.push_back(
            new Transaction("t" + std::to_string(i)));
        TransactionBuilder b(this->m_transactions[i]);
        b.random(V_COUNT, rand);
    }
    this->m_controller.set_operations(
        TransactionBuilder::shuffled_operation_list(this->m_transactions));
    this->m_operation_panel->set_operations(*this->m_controller.operations());
    this->update_displays();
}

void MainWindow::clear_transactions() {
    for(auto x : this->m_transactions)
        delete x;
    this->m_transactions.clear();
    this->m_controller.set_operations(std::list<Operation *>());
    this->m_operation_panel->set_operations(*this->m_controller.operations());
    this->update_displays();
}

void MainWindow::step() {
    auto o = this->m_controller.current();
    this->m_controller.step();
    this->update_displays(
        this->m_controller.locks()->sleeping(*(o->transaction())));
}

void MainWindow::reset() {
    this->m_controller.reset();
    this->m_operation_panel->set_operations(*this->m_controller.operations());
    this->update_displays();
}
