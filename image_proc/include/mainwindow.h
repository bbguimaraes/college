#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <vector>

#include <QMainWindow>

#include "include/chartview.h"

class ImageDisplay;
class QPushButton;
class SettingsDialog;
class WeightDialog;

class Filter;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget * parent = nullptr);
        virtual ~MainWindow();

    public slots:
        void open_image_dialog();
        void open_last();
        void revert_image();
        void close_image();

        void equalize_image();
        void mean_filter();
        void weighted_filter();

        void reset_zoom();

        void show_image();
        void show_histogram();
        void show_settings_dialog();

    private:
        void init();
        void open_image(const QImage & image);
        void load_image(QImage * image);
        void release_images();
        void apply_filter(Filter * filter);

        static const ChartView::Mode CHART_DEFAULT_MODE
            = ChartView::Mode::LINE;
        std::string last_path;
        QWidget * displayed;
        ImageDisplay * image_display;
        QImage * original_image;
        QImage * displayed_image;
        QWidget * histograms_view;
        SettingsDialog * settings_dialog;
        WeightDialog * weight_dialog;
        std::vector<ChartView *> histograms;

        QPushButton * image_button;
        QPushButton * histogram_button;
};

#endif // MAINWINDOW_H
