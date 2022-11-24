#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "hittable.h"
#include "algebra3.h"

class TriangleMesh : public hittable {
	public:
		TriangleMesh(vec3 v1u, vec3 v2u, vec3 v3u, vec3 ni, int v1ii, int v2ii, int v3ii, int mii)
		: v1(v1u), v2(v2u), v3(v3u), n(ni), v1i(v1ii), v2i(v2ii), v3i(v3ii), mi(mii) {};

		virtual bool hit(
            const Ray& r, double t_min, double t_max, hit_record& rec) const override;
		virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

		vec3 getV1() const{
			return v1;
		}
		vec3 getV2() const{
			return v2;
		}
		vec3 getV3() const{
			return v3;
		}

		vec3 getN() const{
			return n;
		}
	private:
		vec3 v1;
		vec3 v2;
		vec3 v3;
		vec3 n;
		int v1i;
		int v2i;
		int v3i;
		int mi;
};


bool TriangleMesh::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
	double epsilon = 1e-5;
	vec3 edge1 = v2 - v1;
	vec3 edge2 = v3 - v1;
	vec3 s1 = r.direction() ^ edge2;
	double a = edge1 * s1;
	if (a > -epsilon && a < epsilon) {
		// ray is parallel to triangle
		return false;
	}

	double coeff = 1.0/a;
	vec3 s = r.origin() - v1;
	double b1 = coeff * (s * s1);
	if (b1 < 0) {
		return false;
	}

	vec3 s2 = s ^ edge1;
	double b2 = coeff * (r.direction() * s2);
	if (b2 < 0 || b1 + b2 > 1) {
		return false;
	}

	double t = coeff * (edge2 * s2);
	if (t < 0 || t < t_min || t_max < t) {
		return false;
	}

	if (n * r.direction() > 0)
	{
	   return false;
	}

	if (t > epsilon) {
		rec.t = t;
    	rec.p = r.at(rec.t);
    	rec.n = n;
		rec.v1i = v1i;
		rec.v2i = v2i;
		rec.v3i = v3i;
		rec.mi = mi;
		return true;
	}

	return false;
}

/*	Constructs a bounding box for a triangle.
*	@time0: t0 time interval for moving objects
*	@time1: t1 time interval for moving objects
*	@output_box: output bounding box
*	Returns an AABB.
*/


bool TriangleMesh::bounding_box(double time0, double time1, aabb& output_box) const {
    
    vec3 v1 = TriangleMesh::getV1();
    vec3 v2 = TriangleMesh::getV2();
    vec3 v3 = TriangleMesh::getV3();
	double minx = fmin(v1[VX],fmin(v2[VX],v3[VX]));
	double miny = fmin(v1[VY],fmin(v2[VY],v3[VY]));
	double minz = fmin(v1[VZ],fmin(v2[VZ],v3[VZ]));

	double maxx = fmax(v1[VX],fmax(v2[VX],v3[VX]));
	double maxy = fmax(v1[VY],fmax(v2[VY],v3[VY]));
	double maxz = fmax(v1[VZ],fmax(v2[VZ],v3[VZ]));
	double eps = 1e-5;
	
	vec3 min = vec3(minx-eps,miny-eps,minz-eps);
	vec3 max = vec3(maxx+eps,maxy+eps,maxz+eps);
	//std::cout << "min = " << min << std::endl;
	//std::cout << "max = " << max << std::endl;
	output_box = aabb(min,max);
	return true;
}

#endif
