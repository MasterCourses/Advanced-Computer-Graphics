#ifndef RAY_H  
#define RAY_H  


#include "algebra3.h"
class Ray {
    public:
        Ray();
        Ray(const vec3& origin, const vec3& direction);
        vec3 origin() const;
        vec3 direction() const; 
        vec3 at(double t) const;            

    public:
        vec3 orig;
        vec3 dir;
};

#endif // RAY_H