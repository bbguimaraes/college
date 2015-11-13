#include "include/mainwindow.h"

#include <fstream>

#include <QFileDialog>
#include <QMenuBar>
#include <QPushButton>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QDialog>
#include <QMessageBox>

#include "include/imagedisplay.h"
#include "include/histogram.h"
#include "include/chartview.h"
#include "include/meanfilter.h"
#include "include/settingsdialog.h"
#include "include/weightdialog.h"

MainWindow::MainWindow(QWidget * parent) :
        QMainWindow(parent), original_image(0), displayed_image(0) {
    this->init();
    this->close_image();
    this->displayed = this->image_display;
    connect(this->image_button, SIGNAL(clicked()), this, SLOT(show_image()));
    connect(
        this->histogram_button, SIGNAL(clicked()),
        this, SLOT(show_histogram()));
}

MainWindow::~MainWindow() {
    if(this->last_path.size() > 0) {
        std::ofstream writer("data.dat");
        writer << this->last_path;
    }
    this->release_images();
    delete this->weight_dialog;
}

void MainWindow::init() {
    this->image_button = new QPushButton("Image");
    this->histogram_button = new QPushButton("Histogram");
    this->image_button->resize(20, 40);
    this->histogram_button->resize(20, 40);
    QHBoxLayout * button_layout = new QHBoxLayout;
    button_layout->addWidget(this->image_button);
    button_layout->addWidget(this->histogram_button);
    this->image_display = new ImageDisplay;
    this->histograms_view = new QWidget(this);
    this->histograms_view->hide();
    QVBoxLayout * histogram_layout = new QVBoxLayout(this->histograms_view);
    this->histograms.resize(4, 0);
    for(int i = 0; i < 4; ++i) {
        this->histograms[i] = new ChartView();
        this->histograms[i]->set_mode(this->CHART_DEFAULT_MODE);
        histogram_layout->addWidget(this->histograms[i]);
    }
    QWidget * central_widget = new QWidget(this);
    setCentralWidget(central_widget);
    QVBoxLayout * main_layout = new QVBoxLayout;
    main_layout->addLayout(button_layout);
    main_layout->addWidget(this->image_display, 2);
    main_layout->addWidget(this->histograms_view, 2);
    central_widget->setLayout(main_layout);
    this->weight_dialog = new WeightDialog;
    this->weight_dialog->setModal(true);
    this->weight_dialog->setWindowTitle("Filter weights");
    this->settings_dialog =
        new SettingsDialog(nullptr, this->CHART_DEFAULT_MODE);
    this->settings_dialog->setWindowTitle("Settings");
    QMenu * file_menu = menuBar()->addMenu("&File");
    file_menu->addAction(
        "&Open", this, SLOT(open_image_dialog()), QKeySequence("Ctrl+O"));
    file_menu->addAction(
        "&Revert", this, SLOT(revert_image()), QKeySequence("Ctrl+R"));
    file_menu->addAction(
        "&Open &last", this, SLOT(open_last()), QKeySequence("Ctrl+L"));
    file_menu->addAction(
        "&Close", this, SLOT(close_image()), QKeySequence("Ctrl+W"));
    file_menu->addAction(
        "&Settings", this, SLOT(show_settings_dialog()),
        QKeySequence("Ctrl+S"));
    QMenu * histogram_menu = menuBar()->addMenu("&Histogram");
    histogram_menu->addAction(
        "&Equalize", this, SLOT(equalize_image()), QKeySequence("Ctrl+E"));
    QMenu * linear_menu = menuBar()->addMenu("&Linear");
    linear_menu->addAction(
        "&Mean", this, SLOT(mean_filter()), QKeySequence("Ctrl+F,Ctrl+M"));
    linear_menu->addAction(
        "&Weighted", this,
        SLOT(weighted_filter()), QKeySequence("Ctrl+F,Ctrl+W"));
    linear_menu->addAction(
        "&Edit filter", this->weight_dialog, SLOT(exec()),
        QKeySequence("Ctrl+F,Ctrl+E"));
    QMenu * view_menu = menuBar()->addMenu("&View");
    view_menu->addAction("&Zoom 1:1", this, SLOT(reset_zoom()));
}

void MainWindow::open_image(const QImage & image) {
    this->release_images();
    this->original_image = new QImage(image);
    this->load_image(this->original_image);
    this->resize(this->original_image->size());
}

void MainWindow::load_image(QImage * image) {
    this->displayed_image = image;
    auto new_histograms = Histogram::create_all(*image);
    unsigned int i = 0;
    for(; i < new_histograms.size() && i < this->histograms.size(); ++i) {
        ChartView * cv = this->histograms[i];
        cv->set_data(new_histograms[i].get_data());
        cv->update();
        cv->show();
    }
    for(; i < this->histograms.size(); ++i)
        this->histograms[i]->hide();
    this->image_display->set_image(*image);
    this->image_display->update();
}

void MainWindow::release_images() {
    delete this->original_image;
    if(this->displayed_image != this->original_image)
        delete this->displayed_image;
    this->original_image = nullptr;
    this->displayed_image = nullptr;
}

void MainWindow::open_image_dialog() {
    auto filename = QFileDialog::getOpenFileName(this, "Choose an image file");
    if(filename == "")
        return;
    this->open_image(QImage(filename));
    this->last_path = filename.toStdString();
}

void MainWindow::open_last() {
    std::getline(std::ifstream("data.dat"), this->last_path);
    if(this->last_path.empty())
        return;
    this->open_image(QImage(this->last_path.c_str()));
}

void MainWindow::revert_image() {
    if(this->original_image == nullptr
            || this->displayed_image == this->original_image)
        return;
    delete this->displayed_image;
    this->load_image(this->original_image);
}

void MainWindow::close_image() {
    this->release_images();
    auto new_image = new QImage(600, 400, QImage::Format_ARGB32);
    this->load_image(new_image);
    this->resize(new_image->size());
}

void MainWindow::equalize_image() {
    if(this->displayed_image == nullptr)
        return;
    auto equalized =
        new QImage(Histogram::equalize_image(*this->displayed_image));
    if(this->displayed_image != this->original_image)
        delete this->displayed_image;
    this->load_image(equalized);
}

void MainWindow::mean_filter() {
    MeanFilter filter(3);
    this->apply_filter(&filter);
}

void MainWindow::weighted_filter() {
    WeightedFilter filter(this->weight_dialog->weights());
    this->apply_filter(&filter);
}

void MainWindow::apply_filter(Filter * filter) {
    auto new_image = new QImage(filter->apply(*this->displayed_image));
    if(this->displayed_image != this->original_image)
        delete this->displayed_image;
    this->load_image(new_image);
}

void MainWindow::reset_zoom() {
    this->image_display->set_zoom(1.0f);
    this->image_display->update();
}

void MainWindow::show_image() {
    this->displayed->hide();
    this->displayed = this->image_display;
    this->displayed->show();
}

void MainWindow::show_histogram() {
    this->displayed->hide();
    this->displayed = this->histograms_view;
    this->displayed->show();
}

void MainWindow::show_settings_dialog() {
    this->settings_dialog->exec();
    for(auto v : this->histograms) {
        v->set_mode(this->settings_dialog->mode());
        v->update();
    }
}
