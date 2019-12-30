#include "tri_mesh.h"
#include "scene_elements.h"
#include "closest_intersection.h"

bool TriMesh::ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const {
	Response* obj_response(new Response());

	//consider triangle mesh as object vector.
	if (closest_intersection(ray,this->meshes,obj_response, t0)) {
		n = obj_response->n;
		t = obj_response->t;
		return true;
	}

	//still need normal and t for comparing other objects
	n = obj_response->n;
	t = obj_response->t;
	return false;
}
