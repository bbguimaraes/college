#ifndef RENDERING_H
#define RENDERING_H

#include <GL/gl.h>

class Camera;
class GLUquadric;
class Mass;
class SpringMassSystem;

class Rendering {
    Rendering() = delete;
     public:
        static void set_camera(const Camera & c);
        static void draw_grid(unsigned int size);
        static void draw_circle(unsigned int slices);
        static void draw_axis(GLUquadric * q, unsigned int slices);
        static void draw_axes(GLUquadric * q, unsigned int slices);
        static void draw_springs_non_textured(const SpringMassSystem * system);
        static void draw_springs_textured(SpringMassSystem * system);
        static void draw_masses(
            const SpringMassSystem * system,
            GLenum mode,
            GLUquadric * q,
            const Mass * selected);
        static void draw_number(unsigned int n);
        static void draw_hud(unsigned int fps);
};

#endif // RENDERING_H
