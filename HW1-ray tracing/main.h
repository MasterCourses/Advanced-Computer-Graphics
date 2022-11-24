#ifndef MAIN_H
#define MAIN_H

#include "algebra3.h"
#include<vector>

class Camera
{
    public:
        vec3 pos;
        vec3 d;
};

class Triangle
{
    public:
        vec3 vertices[6];
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
            return orig + t*dir;
        }

    public:
        vec3 orig;
        vec3 dir;
};

class Viewport
{
    public:
        int width, height;
        //vec3 u, v;
        //vec3 center;
        vec3 corner[4];
};

class Sphere
{
    public:
        vec3 center;
        float radius;
};



#endif

/*
    public:
        vec3 pos;
        vec3 dir;       //ray direction(normalized)
    */