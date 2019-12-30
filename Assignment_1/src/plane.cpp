#include <iostream>
#include "plane.h"
#include "scene_elements.h"
#include <cmath>

bool Plane::ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const {
	double t1 = 0;
	Eigen::Vector3d e = ray.origin;
	Eigen::Vector3d d = ray.direction;
	double dp_ray_normal = d.dot(this->normal);
	bool perpendicular = std::fabs(dp_ray_normal) < 0.0001;

	if (!perpendicular) {
		double dp2 = (point - e).dot(this->normal);
		t1 = (dp2/dp_ray_normal);
		if (t1 >= t0 && t1 < t) {
			t = t1;
			n = this->normal;
			return true;
		}
	}
	return false;
}
