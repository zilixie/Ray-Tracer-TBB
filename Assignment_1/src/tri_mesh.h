#ifndef TRI_MESH_H
#define TRI_MESH_H

#include "scene_elements.h"
#include "closest_intersection.h"
#include <Eigen/Core>
#include <vector>

class TriMesh : public Object {
	public:
		std::vector<Object*> meshes;
	bool ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const;
};


#endif

