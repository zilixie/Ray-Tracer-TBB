#include <Eigen/Geometry>
#include "sphere.h"
#include "scene_elements.h"


bool Sphere::ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const {
	Eigen::Vector3d e = ray.origin;
	Eigen::Vector3d d = ray.direction;
	Eigen::Vector3d c = this->c;
	double r = this->r;

	//k2 is k^2, where k is distance from hit point to the projection of c on ray.
	//(c-e)^2 - ((c-e) dot d)^2 = r^2 - k^2
	double k2 = pow(r, 2) - (c - e).squaredNorm() + pow(((c - e).dot(d.normalized())), 2);
	if (k2 < 0) { return false; }
	double k = sqrt(k2); //get k
	t = (((c - e).dot(d.normalized())) - k)/d.norm(); //t is regarding to the origin norm of d
	Eigen::Vector3d i1 = e + t * d; //hit point

	if (t < t0) { return false; }
	n = (i1 - e - (c - e)).normalized();
	return true;
}
