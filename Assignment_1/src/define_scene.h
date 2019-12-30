#ifndef DEFINESCENE_H
#define DEFINESCENE_H

#include <fstream>
#include <vector>
#include <Eigen/Core>
#include <iostream>
#include <string>
#include <unordered_map>
#include <math.h>
#include "utils.h"
#include "scene_elements.h"
#include "sphere.h"
#include "triangle.h"
#include "tri_mesh.h"
#include "plane.h"
#include "read_off.h"
#define PI 3.14159265


inline bool define_materials(std::unordered_map<std::string,Material*> & materials);

inline bool add_sphere(std::vector<Object*> & objects, Eigen::Vector3d c, double r, Material* & m);
inline bool add_mesh(std::vector<Object*> & objects, std::string filename, Eigen::Vector3d move, double scale, Material* & m);
inline bool add_plane(std::vector<Object*> & objects, Eigen::Vector3d &point, Eigen::Vector3d &normal, Material* & m);
inline bool add_light(std::vector<Light*> & lights, Eigen::Vector3d p, double i);

inline bool scene_123(std::vector<Object*> & objects, std::vector<Light*> & lights);
inline bool scene_4(std::vector<Object*> & objects, std::vector<Light*> & lights);
inline bool scene_5(std::vector<Object*> & objects, std::vector<Light*> & lights);
inline bool scene_6(std::vector<Object*> & objects, std::vector<Light*> & lights);

//implementation
inline bool define_materials(
	std::unordered_map<std::string,Material*> & materials) {
	Material* blue_metal(new Material());
	blue_metal->ambient = Eigen::Vector3d(0.02,0.05,0.07);
	blue_metal->diffuse = Eigen::Vector3d(0.2,0.5,0.7);
	blue_metal->specular = Eigen::Vector3d(0.2,0.5,0.7);
	blue_metal->mirror = Eigen::Vector3d(0.2,0.5,0.7);
	blue_metal->exp = 500;

	Material* purple_metal(new Material());
	purple_metal->ambient = Eigen::Vector3d(0.07,0.03,0.08);
	purple_metal->diffuse = Eigen::Vector3d(0.7,0.3,0.8);
	purple_metal->specular = Eigen::Vector3d(0.7,0.3,0.8);
	purple_metal->mirror = Eigen::Vector3d(0.7,0.3,0.8);
	purple_metal->exp = 500;

	Material* green_rubber(new Material());
	green_rubber->ambient = Eigen::Vector3d(0.03,0.07,0.03);
	green_rubber->diffuse = Eigen::Vector3d(0.3,0.7,0.3);
	green_rubber->specular = Eigen::Vector3d(0,0,0);
	green_rubber->mirror = Eigen::Vector3d(0,0,0);
	green_rubber->exp = 10;

	Material* orange_rubber(new Material());
	orange_rubber->ambient = Eigen::Vector3d(0.3,0.15,0.00);
	orange_rubber->diffuse = Eigen::Vector3d(1,0.5,0.0);
	orange_rubber->specular = Eigen::Vector3d(0,0,0);
	orange_rubber->mirror = Eigen::Vector3d(0,0,0);
	orange_rubber->exp = 20;

	Material* grey_mirror(new Material());
	grey_mirror->ambient = Eigen::Vector3d(0.03,0.03,0.03);
	grey_mirror->diffuse = Eigen::Vector3d(0.3,0.3,0.3);
	grey_mirror->specular = Eigen::Vector3d(0.2,0.2,0.2);
	grey_mirror->mirror = Eigen::Vector3d(0.6,0.6,0.6);
	grey_mirror->exp = 10000;

	materials["blue_metal"] = blue_metal;
	materials["purple_metal"] = purple_metal;
	materials["green_rubber"] = green_rubber;
	materials["orange_rubber"] = orange_rubber;
	materials["grey_mirror"] = grey_mirror;

	return true;
}

inline bool add_sphere(
	std::vector<Object*> & objects,
	Eigen::Vector3d c, 
	double r,
	Material* & m) {

	Sphere* s1(new Sphere());
	s1->r = r;
	s1->c = c;

	objects.push_back(s1);
	objects.back()->material = m;

	return true;
}

inline bool add_plane(
	std::vector<Object*> & objects,
	Eigen::Vector3d &point,
	Eigen::Vector3d &normal,
	Material* & m) {

	Plane* plane(new Plane());
	plane->point = point;
	plane->normal = normal;
	objects.push_back(plane);
	objects.back()->material = m;

	return true;
}

inline bool add_mesh(
	std::vector<Object*> & objects,
	std::string filename,
	Eigen::Vector3d move,
	double scale,
	Material* & m) {

	Eigen::MatrixXd V;
	Eigen::MatrixXi F;
	TriMesh* mesh(new TriMesh());
	read_off(filename,V,F);
	for(int f = 0;f<F.rows();f++) {
		Triangle* tri(new Triangle());
		tri->p0 = Eigen::Vector3d( V(F(f,0),0)* scale + move[0], V(F(f,0),1)* scale + move[1], V(F(f,0),2)* scale + move[2]);
		tri->p1 = Eigen::Vector3d( V(F(f,1),0)* scale + move[0], V(F(f,1),1)* scale + move[1], V(F(f,1),2)* scale + move[2]);
		tri->p2 = Eigen::Vector3d( V(F(f,2),0)* scale + move[0], V(F(f,2),1)* scale + move[1], V(F(f,2),2)* scale + move[2]);
		mesh->meshes.push_back(tri);
	}
	objects.push_back(mesh);
	objects.back()->material = m;
	return true;
}

inline bool add_light(
	std::vector<Light*> & lights,
	Eigen::Vector3d p,
	double i) {
	Light* l(new Light());
	l->p = p;
	l->intensity = i;
	lights.push_back(l);
	return true;
}

inline bool scene_123(
	std::vector<Object*> & objects,
	std::vector<Light*> & lights) {

	//define lights
	add_light(lights, Eigen::Vector3d(-1,1,1), 1.0);
	add_light(lights, Eigen::Vector3d(-1,-1,1), 1.0);

	// define materials
	std::unordered_map<std::string,Material* > materials;
	define_materials(materials);
	    
	// define objects in scene
	add_sphere(objects, Eigen::Vector3d(0.2,0.3,0), 0.3, materials["green_rubber"]);
	add_sphere(objects, Eigen::Vector3d(0,0,-1.2), 0.4, materials["blue_metal"]);
	add_sphere(objects, Eigen::Vector3d(-0.2,-0.5,0), 0.15, materials["orange_rubber"]);
	add_sphere(objects, Eigen::Vector3d(-0.5,0,0.1), 0.2, materials["purple_metal"]);

	return true;
}

inline bool scene_4(
	std::vector<Object*> & objects,
	std::vector<Light*> & lights) {
	//define lights
	add_light(lights, Eigen::Vector3d(-5,5,10), 1.0);
	add_light(lights, Eigen::Vector3d(-5,-5,10), 1.0);

	// define materials
	std::unordered_map<std::string,Material* > materials;
	define_materials(materials);
	add_mesh(objects, "../data/bunny.off", Eigen::Vector3d(-6,-10,0), 50, materials["blue_metal"]);
	add_mesh(objects, "../data/bumpy_cube.off", Eigen::Vector3d(4,0,0), 1, materials["purple_metal"]);

	return true;
}

inline bool scene_5(
	std::vector<Object*> & objects,
	std::vector<Light*> & lights) {
	//define lights
	add_light(lights, Eigen::Vector3d(-5,5,30), 0.6);
	add_light(lights, Eigen::Vector3d(-5,-5,50), 0.5);
	add_light(lights, Eigen::Vector3d(15,-5,40), 0.3);

	// define materials
	std::unordered_map<std::string,Material*> materials;
	define_materials(materials);

	add_mesh(objects, "../data/bunny.off", Eigen::Vector3d(-6,-10,0), 50, materials["blue_metal"]);
	add_mesh(objects, "../data/bumpy_cube.off", Eigen::Vector3d(4,0,0), 1, materials["purple_metal"]);

	Eigen::Vector3d p(0,0,-30);
	Eigen::Vector3d n(-0.75,0.9,1);
	add_plane(objects, p, n, materials["orange_rubber"]);
	add_sphere(objects, Eigen::Vector3d(-6,6,0), 3.5, materials["green_rubber"]);

	return true;
}

inline bool scene_6(
	std::vector<Object*> & objects,
	std::vector<Light*> & lights) {
	//define lights
	add_light(lights, Eigen::Vector3d(-5,5,10), 1.0);
	add_light(lights, Eigen::Vector3d(-5,-5,10), 1.0);

	// define materials
	std::unordered_map<std::string,Material*> materials;
	define_materials(materials);

	add_mesh(objects, "../data/bunny.off", Eigen::Vector3d(-6,-10,0), 50, materials["blue_metal"]);
	add_mesh(objects, "../data/bumpy_cube.off", Eigen::Vector3d(4,0,0), 1, materials["purple_metal"]);

	Eigen::Vector3d p(-4,-10,0);
	Eigen::Vector3d n(0,1,0);
	add_plane(objects, p, n, materials["grey_mirror"]);
	add_sphere(objects, Eigen::Vector3d(-6,6,0), 3.5, materials["green_rubber"]);

	return true;
}

#endif