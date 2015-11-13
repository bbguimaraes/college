#ifndef IMAGEDISPLAY_H
#define IMAGEDISPLAY_H

#include <QGLWidget>

class QWheelEvent;

class ImageDisplay : public QGLWidget {
    public:
        ImageDisplay(QWidget * parent = 0);
        virtual ~ImageDisplay();
        void set_image(const QImage & image);
        void set_zoom(float zoom) {m_zoom = zoom;}
        float zoom() const {return m_zoom;}

    protected:
        virtual void wheelEvent(QWheelEvent * e);

    private:
        virtual void initializeGL();
        virtual void resizeGL(int w, int h);
        virtual void paintGL();

        GLuint texture;
        QSize image_size;
        float m_zoom;
};

#endif // IMAGEDISPLAY_H
