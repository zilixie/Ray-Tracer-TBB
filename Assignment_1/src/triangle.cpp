#include "triangle.h"
#include "scene_elements.h"
#include <cmath>

bool Triangle::ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const {
	Eigen::Vector3d origin = ray.origin;
	Eigen::Vector3d direction = ray.direction;
	Eigen::Vector3d p0 = this->p0;
	Eigen::Vector3d p1 = this->p1;
	Eigen::Vector3d p2 = this->p2;

	//use cramer's rule
	//3 vectors (a,b,c) (d,e,f) (g,h,i)
	// |a d g|   |beta |
	// |b e h| X |gamma| = ray.direction
	// |c f i|   |tau  |

	double a = (p0 - origin).x();
	double d = (p1 - origin).x();
	double g = (p2 - origin).x();

	double b = (p0 - origin).y();
	double e = (p1 - origin).y();
	double h = (p2 - origin).y();

	double c = (p0 - origin).z();
	double f = (p1 - origin).z();
	double i = (p2 - origin).z();

	double j = direction.x();
	double k = direction.y();
	double l = direction.z();

	double M = a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g);
	double beta = (j * (e * i - h * f) + k * (g * f - d * i) + l * (d * h - e * g))/M;
	double gamma = (i * (a * k - j * b) + h * (j * c - a * l) + g * (b * l - k * c))/M;
	double tau = (-1) * (f * (a * k - j * b) + e * (j * c - a * l) + d * (b * l - k * c))/M;

	Eigen::Vector3d v1 = (p1 - p0);
	Eigen::Vector3d v2 = (p2 - p0);
	Eigen::Vector3d normal = cross_product(v1, v2);

	if ((beta < 0) || (gamma < 0) || (tau < 0)) { return false; }
	if (normal.dot(direction) < 0) {n = normal.normalized();}
	else {n = -normal.normalized();}

	//angle of ray direction and normal
	double cos_theta = abs(direction.dot(n)/(direction.norm()*n.norm()));
	//abs(n.dot(p0 - origin) computes the height from origin to plane form by triangle.
	t = (abs(n.dot(p0 - origin))/cos_theta)/direction.norm();

	if (t < t0) {return false;}
	return true;
}
