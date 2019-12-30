#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "scene_elements.h"
#include <Eigen/Core>

class Triangle : public Object {
	public:
		Eigen::Vector3d p0;
		Eigen::Vector3d p1;
		Eigen::Vector3d p2;
	// check if the ray intersects the sphere
	bool ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const;
};

Eigen::Vector3d cross_product(Eigen::Vector3d & v1, Eigen::Vector3d & v2);

// implementation of cross product
inline Eigen::Vector3d cross_product(Eigen::Vector3d & v1, Eigen::Vector3d & v2) {
	Eigen::Vector3d cross;
	cross.x() = (v1[1]*v2[2])-(v1[2]*v2[1]);
	cross.y() = (v1[2]*v2[0])-(v1[0]*v2[2]);
	cross.z() = (v1[0]*v2[1])-(v1[1]*v2[0]);
	return cross;
}

#endif
