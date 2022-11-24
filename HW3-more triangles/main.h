#ifndef MAIN_H
#define MAIN_H

#include "algebra3.h"
#include "imageIO.h"

class Light
{
    public: 
        vec3 pos;
        vec3 P, Ii; // hit point, ambient
};

class Camera
{
    public:
        vec3 pos;
        vec3 d;
        float angle;
};

class Ray {
    public:
        Ray() {}
        Ray(const vec3& origin, const vec3& direction)
            : orig(origin), dir(direction)
        {}
        vec3 origin() const  { return orig; }
        vec3 direction() const { return dir; }

        vec3 at(double t) const {
            return orig + t * dir;
        }

    public:
        vec3 orig;
        vec3 dir;
};

class Viewport
{
    public:
        vec3 center;
        vec3 dir;
        vec3 up_dir;
        int width, height;
        vec3 hori, vert;
};

#endif

