#include "ray.h"

Ray::Ray(){
    orig = vec3();
	dir = vec3();
}

Ray::Ray(const vec3& origin, const vec3& direction)
{
    orig = origin;
    dir = direction;
}

vec3 Ray::origin() const 
{
    return orig;
}

vec3 Ray::direction() const
{
    return dir;
}

vec3 Ray::at(double t) const {
    return orig + t * dir;
}