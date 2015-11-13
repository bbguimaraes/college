#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtOpenGL/QGLWidget>

#include <vector>

class ChartView : public QGLWidget {
    public:
        enum class Mode {LINE, BARS};

        ChartView(QWidget * parent = 0);
        virtual ~ChartView() {};
        void set_data(const std::vector<int> & data);
        void set_mode(Mode mode) {this->mode = mode;}

    protected:
        virtual void initializeGL();
        virtual void resizeGL(int w, int h);
        virtual void paintGL();

    private:
        void draw_bars();
        void draw_line();

        std::vector<int> data;
        unsigned int max;
        Mode mode;
};

#endif // CHARTVIEW_H
