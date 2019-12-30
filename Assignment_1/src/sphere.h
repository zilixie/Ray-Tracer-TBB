#ifndef SPHERE_H
#define SPHERE_H

#include <Eigen/Core>
#include "scene_elements.h"

class Sphere : public Object {
	public:
		double r;
		Eigen::Vector3d c;
	// check if the ray intersects the sphere
	bool ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const;
};

#endif
