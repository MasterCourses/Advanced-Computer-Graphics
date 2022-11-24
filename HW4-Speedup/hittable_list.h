#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }
	
		/*	constructs a hittable list
		*	@src_objects: Vector to initialize from
		*	@start: start index of vector (inclusive)
		*	@end: end index of vector (exclusive)
		*/
		hittable_list(std::vector<shared_ptr<hittable>>& src_objects, int start, int end) {
			for (int i = start; i < end; i++) {
				add(src_objects[i]);
			}
		}

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(
            const Ray& r, double t_min, double t_max, hit_record& rec) const override;
		virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;


    public:
        std::vector<shared_ptr<hittable>> objects;
};


bool hittable_list::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

/*	Constructs a bounding box for a hittable_list.
*	@time0: t0 time interval for moving objects
*	@time1: t1 time interval for moving objects
*	@output_box: output bounding box
*	Returns an AABB.
*/
bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

#endif