#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include "scene_elements.h"
#include <Eigen/Core>
#include <iostream>

class Plane : public Object {
	public:
		Eigen::Vector3d point;
		Eigen::Vector3d normal;
	bool ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const;
};

#endif
