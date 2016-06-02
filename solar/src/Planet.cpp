#include <Planet.h>

#include <string>

/*static*/
const float Planet::AU_TO_KM = 149597870.7f;

Planet::Planet(
    std::string name, float distance, float speed, float radius,
    OpenGLTexture * texture, bool star) :
        m_name(name),
        m_distance(distance),
        m_speed(speed),
        m_angle(0.0f),
        m_radius(radius),
        m_texture(texture),
        m_star(star) {}

void Planet::update(float time) {
    m_angle += time * m_speed;
}

void Planet::draw(GLUquadric * quadric, unsigned int slices) const {
    // Store matrix.
    glPushMatrix();

    // Translate and rotate to position.
    glRotatef(m_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -m_distance);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    // Draw sphere.
    gluSphere(quadric, m_radius, slices, slices);

    // Restore matrix.
    glPopMatrix();
}

std::ostream & operator<<(std::ostream & os, const Planet & p) {
    return os << "Planet[" << &p << "]: " <<
        "name(" << p.name() << ")"<<
        ", distance(" << p.distance() << ")" <<
        ", speed(" << p.speed() << ")" <<
        ", angle(" << p.angle() << ")" <<
        ", radius(" << p.radius() << ")" <<
        ", texture(" << p.texture() << ")" <<
        ", star(" << p.star() << ")";
}
