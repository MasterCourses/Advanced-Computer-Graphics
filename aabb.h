#ifndef AABB_H
#define AABB_H

#include "util.h"

class aabb {
    public:
        aabb() {}
        aabb(const vec3& a, const vec3& b) { minimum = a; maximum = b;}

        vec3 min() const {return minimum; }
        vec3 max() const {return maximum; }

		/*	Determines if a ray hits the bounding box
		*	@r: The ray to test
		*	@t_min: the min value of t
		*	@t_max: the max value of t
		*	Returns true if ray intersects, false otherwise.
		*/
        bool hit(const Ray& r, double t_min, double t_max) const {
    		for (int a = 0; a < 3; a++) {
        		auto invD = 1.0f / r.direction()[a];
        		auto t0 = (min()[a] - r.origin()[a]) * invD;
        		auto t1 = (max()[a] - r.origin()[a]) * invD;
        		if (invD < 0.0f)
            		std::swap(t0, t1);
        		t_min = t0 > t_min ? t0 : t_min;
        		t_max = t1 < t_max ? t1 : t_max;
        		if (t_max <= t_min)
            		return false;
    		}
    		return true;
		}

        vec3 minimum;
        vec3 maximum;
};

/*	Creates a AABB that encloses box0 and box1.
*	@box0: The first box
*	@box1: The second box
*	Returns an AABB.
*/
aabb surrounding_box(aabb box0, aabb box1) {
   	vec3 small(fmin(box0.min()[VX], box1.min()[VX]),
               fmin(box0.min()[VY], box1.min()[VY]),
               fmin(box0.min()[VZ], box1.min()[VZ]));

    vec3 big(fmax(box0.max()[VX], box1.max()[VX]),
             fmax(box0.max()[VY], box1.max()[VY]),
             fmax(box0.max()[VZ], box1.max()[VZ]));

    return aabb(small,big);
}

#endif
