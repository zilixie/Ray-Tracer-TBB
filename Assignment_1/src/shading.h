#ifndef SHADING_H
#define SHADING_H
#include "scene_elements.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <algorithm>
#include "closest_intersection.h"

// shading will do:
// first inspect whether the ray intersect any object in vector
//		if not find any intersection, return false without modifying any input variables.
//		if find intersect, return true and modify the input rgb variable. That's it.

bool shading(
	const Ray & ray,
	Eigen::Vector3d & rgb,
	const std::vector<Object*> & objects,
	const std::vector<Light*> & lights,
	const int nth_reflection,
	const double t0,
	const bool & enable_shadow,
	const bool & enable_mirror);

#endif
