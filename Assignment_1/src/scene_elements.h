#ifndef SCENE_ELEMENTS_H
#define SCENE_ELEMENTS_H

#include <memory>
#include <Eigen/Core>
#include "material.h"

//A Ray has origin (eye) and direction (d)
class Ray {
	public:
		Eigen::Vector3d origin;
		Eigen::Vector3d direction;
};

//Object is a class to be implemented by Sphere, Triangle, TriMesh and Plane.
class Object {
	public:
		Material* material;
	virtual bool ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const = 0;
};

//Light has point and intensity
class Light {
	public:
		Eigen::Vector3d p;
		double intensity;
	void reverse_light(const Eigen::Vector3d & query, Eigen::Vector3d & direction, double & distance) const;
};

//Consider a scene as a sandbox, given a ray, the scene will response with:
//id: index of obj that was hit by ray.
//t: parameterized distance
//n: normal of the hit point.
class Response {
	public:
		int id;
		double t;
		Eigen::Vector3d n;
};

//light reverse implemented here
inline void Light::reverse_light(const Eigen::Vector3d & query, Eigen::Vector3d & direction, double & distance) const {
	Eigen::Vector3d hit_point_to_light = this->p - query;
	direction = hit_point_to_light.normalized();
	distance = hit_point_to_light.norm();
}

#endif
