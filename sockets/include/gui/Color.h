#ifndef COLOR_H
#define COLOR_H

class Color {
    float m_r, m_g, m_b, m_a;

    public:
        Color(float r, float g, float b, float a = 1.0f) :
            m_r(r),
            m_g(g),
            m_b(b),
            m_a(a) {}
        Color() : Color(0.0f, 0.0f, 0.0f, 1.0f) {}
        Color(int r, int g, int b, int a = 255) : Color(
            (r / 255.0f),
            (g / 255.0f),
            (b / 255.0f),
            (a / 255.0f)) {}

        float r() const {return m_r;}
        float g() const {return m_g;}
        float b() const {return m_b;}
        float a() const {return m_a;}
};

#endif // COLOR_H
