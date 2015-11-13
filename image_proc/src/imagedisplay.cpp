#include "include/imagedisplay.h"

#include <QWheelEvent>

ImageDisplay::ImageDisplay(QWidget * parent)
    : QGLWidget(parent), m_zoom(1.0f) {
}

ImageDisplay::~ImageDisplay() {
    glDeleteTextures(1, &this->texture);
}

/*virtual*/
void ImageDisplay::initializeGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &this->texture);
}

/*virtual*/
void ImageDisplay::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(-0.5, 0.5, -0.5, 0.5, 0.1, 10.0);
}

/*virtual*/
void ImageDisplay::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -1.0f);
    glScalef(this->m_zoom, this->m_zoom, this->m_zoom);
    glTranslatef(-0.5f, -0.5f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex2i(0, 0);
    glTexCoord2i(1 , 0);
    glVertex2i(1, 0);
    glTexCoord2i(1, 1);
    glVertex2i(1, 1);
    glTexCoord2i(0, 1);
    glVertex2i(0, 1);
    glEnd();
}

void ImageDisplay::set_image(const QImage & image) {
    this->makeCurrent();
    QImage glImage = QGLWidget::convertToGLFormat(image);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        glImage.width(),
        glImage.height(),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        glImage.bits()
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->image_size = image.size();
    this->m_zoom = 1.0f;
}

void ImageDisplay::wheelEvent(QWheelEvent * e) {
    int steps = e->delta() / 8 / 15;
    if(this->m_zoom + steps * 0.05f > 0)
        this->m_zoom += steps * 0.05f;
    this->update();
    e->accept();
}
