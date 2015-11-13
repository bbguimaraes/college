#include "include/chartview.h"

#include <algorithm>

ChartView::ChartView(QWidget * parent)
    : QGLWidget(parent), mode(Mode::LINE) {
}

void ChartView::set_data(const std::vector<int> & data) {
    this->data = data;
    this->max = *std::max_element(
        this->data.cbegin(), this->data.cend(),
        [](int x, int y){return abs(x) < abs(y);});
}

/*virtual*/
void ChartView::initializeGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

/*virtual*/
void ChartView::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(-0.5, 0.5, -0.5, 0.5, 0.1, 1000.0);
}

/*virtual*/
void ChartView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-0.5f, -0.5f, -1.5f);
    glScalef(1.0f / this->data.size(), 1.0f / this->max, 1.0f);
    if(this->mode == Mode::BARS)
        this->draw_bars();
    else if(this->mode == Mode::LINE)
        this->draw_line();
}

void ChartView::draw_bars() {
    for(unsigned int i = 0; i < this->data.size(); i++) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2i(i, 0);
        glVertex2i(i + 1, 0);
        glVertex2i(i + 1, this->data[i]);
        glVertex2i(i, this->data[i]);
        glEnd();
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2i(i, 0);
        glVertex2i(i + 1, 0);
        glVertex2i(i + 1, this->data[i]);
        glVertex2i(i, this->data[i]);
        glEnd();
    }
}

void ChartView::draw_line() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for(unsigned int i = 0; i < this->data.size(); i++)
        glVertex2i(i, this->data[i]);
    glEnd();
}
