#ifndef CLOSEST_INTERSECTION_H
#define CLOSEST_INTERSECTION_H

#include "scene_elements.h"
#include <Eigen/Core>
#include <vector>
#include <algorithm>
#include <limits>

// given a Ray, vector of Objects, return a boolean indicating whether some object is hitted by this ray.
// if some objects are hitted, find the hit with smallest t and its matching obj id, and normal.
// pack these as Response and modify the passed in pointer

bool closest_intersection(
	const Ray & r, 
	const std::vector<Object*> & objs, 
	Response* & obj_response,
	const double t0);  //minimum value that t needs to have

#endif
