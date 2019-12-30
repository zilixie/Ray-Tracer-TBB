#include "closest_intersection.h"
#include "scene_elements.h"

//description in closest_intersection.h
bool closest_intersection(
		const Ray & ray,
		const std::vector< Object*> & objs,
		Response* & obj_response,
		const double t0)
{
	double t1;
	Eigen::Vector3d n1;
	obj_response->t = std::numeric_limits<double>::max(); //initialize as inf
	bool intersect = false;
	for (int i=0; i<objs.size(); i++) {
		if (objs[i]->ray_intersect(ray, t1, n1, t0)) {
			intersect = true;
			if (t1 < obj_response->t) {
				obj_response->t = t1;
				obj_response->id = i;
				obj_response->n = n1;
			}
		}
	}

	if (intersect) { return true; }
	else { return false; }
}
