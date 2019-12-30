#ifndef MATERIAL_H
#define MATERIAL_H
#include <Eigen/Core>

class Material {
	public:
		// Ambient, Diffuse, Specular, Mirror Color
  		Eigen::Vector3d ambient;
  		Eigen::Vector3d diffuse;
  		Eigen::Vector3d specular;
  		Eigen::Vector3d mirror;
  		double exp; // Phong exponent
};
#endif
