// C++ include
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

// Image writing library
#define STB_IMAGE_WRITE_IMPLEMENTATION // Do not include this line twice in your project!
#include "stb_image_write.h"
#include "utils.h"
#include "scene_elements.h"
#include "sphere.h"
#include "triangle.h"
#include "tri_mesh.h"
#include "plane.h"
#include "closest_intersection.h"
#include "shading.h"
#include "define_scene.h"
#include <math.h>
#include <cmath>
#include "tbb/tbb.h"
#define PI 3.14159265


// Shortcut to avoid Eigen:: and std:: everywhere, DO NOT USE IN .h
using namespace std;
using namespace Eigen;


void part0() {
	std::cout << "Part 0: Writing a grid png image" << std::endl;
	const std::string filename("part0.png");
	Eigen::MatrixXd M(800,800);

	// Draw a grid, each square has a side of e pixels
	const int e = 50;
	const double black = 0;
	const double white = 1;

	for (unsigned wi = 0; wi<M.cols();++wi)
		for (unsigned hi = 0; hi < M.rows(); ++hi)
			M(hi,wi) = (lround(wi / e) % 2) == (lround(hi / e) % 2) ? black : white;

	// Write it in a png image. Note that the alpha channel is reversed to make the white (color = 1) pixels transparent (alhpa = 0)
	write_matrix_to_png(M,M,M,1.0-M.array(),filename);
}

void part1_1() {
	std::cout << "Part 1.1: Simple ray tracer, multiple spheres in general positions." << std::endl;
	const std::string filename("part1_1.png");
	MatrixXd C = MatrixXd::Zero(800,800); // Store the color
	MatrixXd A = MatrixXd::Zero(800,800); // Store the alpha mask

	// The camera is orthographic, pointing in the direction -z and covering the unit square (-1,1) in x and y
	Vector3d origin(-1,1,1);
	Vector3d x_displacement(2.0/C.cols(),0,0);
	Vector3d y_displacement(0,-2.0/C.rows(),0);

	const Vector3d light_position(-1,1,1); // Single light source
	std::vector<Object*> objects; // define objects in scene
	std::vector<Light*> _; //the light variable is not used for this part
	scene_123(objects, _);

	for (unsigned i=0;i<C.cols();i++) {
		for (unsigned j=0;j<C.rows();j++) {
			// Prepare the ray
			Ray ray;
			ray.origin = origin + double(i)*x_displacement + double(j)*y_displacement;
			ray.direction = RowVector3d(0,0,-1);

			// Intersect with the multiple sphere
			Response* obj_response(new Response());
			bool find_hit = closest_intersection(ray,objects,obj_response,0.0);
			double t = obj_response->t;
			int hit_id = obj_response->id;
			Vector3d ray_normal = obj_response->n;

			if (find_hit) {
				Vector3d ray_intersection = ray.origin + t * ray.direction;
				C(i,j) = (light_position-ray_intersection).normalized().transpose() * ray_normal;  // Simple diffuse model
				C(i,j) = max(C(i,j),0.);  // Clamp to zero
				A(i,j) = 1;  // Disable the alpha mask for this pixel
			}
		}
	}
	write_matrix_to_png(C,C,C,A,filename); // Save to png
}


void part1_2() {
	std::cout << "Part 1.2: Simple ray tracer, multiple spheres with ambient and specular lighting." << std::endl;

	const std::string filename("part1_2.png");
	MatrixXd R = MatrixXd::Zero(800,800); // Store the color
	MatrixXd G = MatrixXd::Zero(800,800); // Store the color
	MatrixXd B = MatrixXd::Zero(800,800); // Store the color
	MatrixXd A = MatrixXd::Zero(800,800); // Store the alpha mask

	// The camera is orthographic, pointing in the direction -z and covering the unit square (-1,1) in x and y
	Vector3d origin(-1,1,1);
	Vector3d x_displacement(2.0/R.cols(),0,0);
	Vector3d y_displacement(0,-2.0/R.rows(),0);

	std::vector<Light*> lights;    // multiple light sources
	std::vector<Object*> objects; // define objects in scene
	scene_123(objects, lights);

	for (unsigned i=0;i<R.cols();i++) {
		for (unsigned j=0;j<R.rows();j++) {
			// Prepare the ray
			Ray ray;
			ray.origin = origin + double(i)*x_displacement + double(j)*y_displacement;;
			ray.direction = RowVector3d(0,0,-1);

			Eigen::Vector3d rgb;
			bool find_hit = shading(ray, rgb, objects, lights, 0, 0.0, false, false);
			if (find_hit) {
				R(i,j) = max(rgb(0),0.);  // Clamp to zero
				G(i,j) = max(rgb(1),0.);  // Clamp to zero
				B(i,j) = max(rgb(2),0.);  // Clamp to zero
				A(i,j) = 1;  // Disable the alpha mask for this pixel
			}
	    }
	}
	write_matrix_to_png(R,G,B,A,filename); // Save to png
}


void part1_3() {
	std::cout << "Part 1.3: Simple ray tracer, perspective projection." << std::endl;

	const std::string filename("part1_3.png");
	int nx = 800, ny = 800; // num of pixels
	MatrixXd R = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd G = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd B = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd A = MatrixXd::Zero(nx,ny); // Store the alpha mask

	double width = 2.0, height = 2.0;
	double l = -width/2, r = width/2, b = -height/2, t = height/2;
	double d = 3.0;

	Vector3d e(0,0,4), w(0,0,1), u(1,0,0), v(0,1,0);
	std::vector<Light*> lights;  // multiple light sources
	std::vector<Object*> objects; // define objects in scene
	scene_123(objects, lights);

	for (unsigned i=0;i<R.cols();i++) {
		for (unsigned j=0;j<R.rows();j++) {
			double U = l + (i + 0.5)*(r - l)/nx;
			double V = t - (j + 0.5)*(t - b)/ny;
			// Prepare the ray
			Ray ray;
			ray.origin = e;
			ray.direction = -d * w + U * u + V * v;

			Eigen::Vector3d rgb;
			bool find_hit = shading(ray, rgb, objects, lights, 0, 1.0, false, false);
			if (find_hit) {
				R(i,j) = max(rgb(0),0.);  // Clamp to zero
				G(i,j) = max(rgb(1),0.);  // Clamp to zero
				B(i,j) = max(rgb(2),0.);  // Clamp to zero
				A(i,j) = 1;  // Disable the alpha mask for this pixel
			}
		}
	}
	write_matrix_to_png(R,G,B,A,filename); // Save to png
}

void part1_4() {
	std::cout << "Part 1.4: Simple ray tracer, load meshes in off format." << std::endl;

	const std::string filename("part1_4.png");
	int nx = 800, ny = 800; // num of pixels
	MatrixXd R = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd G = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd B = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd A = MatrixXd::Zero(nx,ny); // Store the alpha mask

	double width = 2.0, height = 2.0;
	double l = -width/2, r = width/2, b = -height/2, t = height/2;
	double d = 3.0;

	Vector3d e(18,-2,45);
	//Vector3d w = e.normalized();
	Vector3d w(6,0,15);
	w = w.normalized();
	Vector3d v(0,1,0);
	v = v.normalized();
	Vector3d u = cross_product(v, w);

	// multiple light sources
	std::vector<Light*> lights;
	std::vector< Object*> objects;
	scene_4(objects, lights);

	for (unsigned i=0;i<R.cols();i++) {
		for (unsigned j=0;j<R.rows();j++) {
			//printf("i:%d, j:%d\n", i,j);
			double U = l + (i + 0.5)*(r - l)/nx;
			double V = t - (j + 0.5)*(t - b)/ny;
			// Prepare the ray
			Ray ray;
			ray.origin = e;
			ray.direction = -d * w + U * u + V * v;

			Eigen::Vector3d rgb;
			bool find_hit = shading(ray, rgb, objects, lights, 0, 1.0, false, false);
			if (find_hit) {
				R(i,j) = max(rgb(0),0.);  // Clamp to zero
				G(i,j) = max(rgb(1),0.);  // Clamp to zero
				B(i,j) = max(rgb(2),0.);  // Clamp to zero
				A(i,j) = 1;  // Disable the alpha mask for this pixel
			} else {
				R(i,j) = 1;
				G(i,j) = 1;
				B(i,j) = 1;
				A(i,j) = 1;
			}
		}
	}
	write_matrix_to_png(R,G,B,A,filename); // Save to png
}


void part1_5() {
	std::cout << "Part 1.5: Simple ray tracer, meshes, sphere and plane with shadow." << std::endl;
	const std::string filename("part1_5.png");
	int nx = 800, ny = 800; // num of pixels
	MatrixXd R = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd G = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd B = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd A = MatrixXd::Zero(nx,ny); // Store the alpha mask

	double width = 2.0, height = 2.0;
	double l = -width/2, r = width/2, b = -height/2, t = height/2;
	double d = 3.0;

	Vector3d e(18,-2,45);

	Vector3d w(6,0,15);
	w = w.normalized();
	Vector3d v(0,1,0);
	v = v.normalized();
	Vector3d u = cross_product(v, w);

	// multiple light sources
	std::vector<Light*> lights;
	std::vector< Object*> objects;
	scene_5(objects, lights);

	for (unsigned i=0;i<R.cols();i++) {
	    for (unsigned j=0;j<R.rows();j++) {
			//printf("i:%d, j:%d\n", i,j);
			double U = l + (i + 0.5)*(r - l)/nx;
			double V = t - (j + 0.5)*(t - b)/ny;
			// Prepare the ray
			Ray ray;
			ray.origin = e;
			ray.direction = -d * w + U * u + V * v;

			Eigen::Vector3d rgb;
			bool find_hit = shading(ray, rgb, objects, lights, 0, 1.0, true, false);
			if (find_hit) {
				R(i,j) = max(rgb(0),0.); // Clamp to zero
				G(i,j) = max(rgb(1),0.);
				//(i,j) = max(rgb(1),0.);  // Clamp to zero
				B(i,j) = max(rgb(2),0.);  // Clamp to zero
				A(i,j) = 1;  // Disable the alpha mask for this pixel
			} else {
				R(i,j) = 0;
				G(i,j) = 0;
				B(i,j) = 0;
				A(i,j) = 1;
			}
		}
	}
	write_matrix_to_png(R,G,B,A,filename); // Save to png
}


void part1_6() {
	std::cout << "Part 1.6: Simple ray tracer, meshes, sphere and plane with shadow and mirror floor." << std::endl;
	const std::string filename("part1_6.png");
	int nx = 800, ny = 800; // num of pixels
	MatrixXd R = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd G = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd B = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd A = MatrixXd::Zero(nx,ny); // Store the alpha mask

	double width = 2.0, height = 2.0;
	double l = -width/2, r = width/2, b = -height/2, t = height/2;
	double d = 3.0;

	Vector3d e(24,-8,60), w(8,0,20), v(0,1,0);
	//Vector3d w = e.normalized();
	w = w.normalized();
	v = v.normalized();
	Vector3d u = cross_product(v, w);

	// multiple light sources
	std::vector<Light*> lights;
	std::vector<Object*> objects;
	scene_6(objects, lights);

	for (unsigned i=0;i<R.cols();i++) {
		for (unsigned j=0;j<R.rows();j++) {
			//printf("i:%d, j:%d\n", i,j);
			double U = l + (i + 0.5)*(r - l)/nx;
			double V = t - (j + 0.5)*(t - b)/ny;
			// Prepare the ray
			Ray ray;
			ray.origin = e;
			ray.direction = -d * w + U * u + V * v;

			Eigen::Vector3d rgb;
			bool find_hit = shading(ray, rgb, objects, lights, 0, 1.0, true, true);
			if (find_hit) {
				//Eigen::Vector3d rgb = shading(ray,objects,lights, hit_id,t,ray_normal,true);
				R(i,j) = max(rgb(0),0.);  // Clamp to zero
				G(i,j) = max(rgb(1),0.);  // Clamp to zero
				B(i,j) = max(rgb(2),0.);  // Clamp to zero
				A(i,j) = 1;  // Disable the alpha mask for this pixel
			} else {
				R(i,j) = 0;
				G(i,j) = 0;
				B(i,j) = 0;
				A(i,j) = 1;
			}
		}
	}
	write_matrix_to_png(R,G,B,A,filename); // Save to png
}


void part1_7() {
	std::cout << "Part 1.7: Parallelized part 6." << std::endl;
	const std::string filename("part1_7.png");
	int nx = 800, ny = 800; // num of pixels
	MatrixXd R = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd G = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd B = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd A = MatrixXd::Zero(nx,ny); // Store the alpha mask

	double width = 2.0, height = 2.0;
	double l = -width/2, r = width/2, b = -height/2, t = height/2;
	double d = 3.0;

	Vector3d e(24,-8,60), w(8,0,20), v(0,1,0);
	//Vector3d w = e.normalized();
	w = w.normalized();
	v = v.normalized();
	Vector3d u = cross_product(v, w);

	// multiple light sources
	std::vector<Light*> lights;
	std::vector<Object*> objects;
	scene_6(objects, lights);

	tbb::parallel_for(0, (int)R.cols(), [&](int i){
		tbb::parallel_for(0, (int)R.rows(), [&](int j) {
			double U = l + (i + 0.5)*(r - l)/nx;
			double V = t - (j + 0.5)*(t - b)/ny;
			// Prepare the ray
			Ray ray;
			ray.origin = e;
			ray.direction = -d * w + U * u + V * v;

			Eigen::Vector3d rgb;
			bool find_hit = shading(ray, rgb, objects, lights, 0, 1.0, true, true);
			if (find_hit) {
				R(i,j) = max(rgb(0),0.);  // Clamp to zero
				G(i,j) = max(rgb(1),0.);  // Clamp to zero
				B(i,j) = max(rgb(2),0.);  // Clamp to zero
				A(i,j) = 1;  // Disable the alpha mask for this pixel
			} else {
				R(i,j) = 0;
				G(i,j) = 0;
				B(i,j) = 0;
				A(i,j) = 1;
			}
		});
	});

	write_matrix_to_png(R,G,B,A,filename); // Save to png
}

void part1_8() {
	//convert -delay 10 -loop 0 $(ls part8-*.png | sort -V) animated.gif
	//rm part8-*.png

	std::cout << "Part 1.8: Animated scene from part 6." << std::endl;

	int nx = 600, ny = 600; // num of pixels
	MatrixXd R = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd G = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd B = MatrixXd::Zero(nx,ny); // Store the color
	MatrixXd A = MatrixXd::Zero(nx,ny); // Store the alpha mask

	double width = 2.0, height = 2.0;
	double l = -width/2, r = width/2, b = -height/2, t = height/2;
	double d = 3.0;

	// multiple light sources
	std::vector<Light*> lights;
	std::vector<Object*> objects;
	scene_6(objects, lights);

	for (unsigned k=0;k<80;k++){
		double param, result_sin, result_cos;
		param = 68.2 + k*5;
		result_sin = sin (param*PI/180);
		result_cos = cos (param*PI/180);
		double z = 64.62 * result_sin;
		double x = 64.62 * result_cos;

		Vector3d e(x,-8,z), w(x,0,z), v(0,1,0);
		//Vector3d w = e.normalized();
		w = w.normalized();
		v = v.normalized();
		Vector3d u = cross_product(v, w);

		char filename[100];
		sprintf(filename, "part8-%d.png", k);

		tbb::parallel_for(0, (int)R.cols(), [&](int i){
			tbb::parallel_for(0, (int)R.rows(), [&](int j) {
				double U = l + (i + 0.5)*(r - l)/nx;
				double V = t - (j + 0.5)*(t - b)/ny;
				// Prepare the ray
				Ray ray;
				ray.origin = e;
				ray.direction = -d * w + U * u + V * v;

				Eigen::Vector3d rgb;
				bool find_hit = shading(ray, rgb, objects, lights, 0, 1.0, true, true);
				if (find_hit) {
					R(i,j) = max(rgb(0),0.);  // Clamp to zero
					G(i,j) = max(rgb(1),0.);  // Clamp to zero
					B(i,j) = max(rgb(2),0.);  // Clamp to zero
					A(i,j) = 1;  // Disable the alpha mask for this pixel
				} else {
					R(i,j) = 0;
					G(i,j) = 0;
					B(i,j) = 0;
					A(i,j) = 1;
				}
			});
		});
		write_matrix_to_png(R,G,B,A,filename); // Save to png
	}
}

int main(int argc, char * argv[])
{
	/*
	//part0();
	if (argc<=1) { part1_6(); }
	else if (std::strcmp(argv[1],"1") == 0) { part1_1(); }
	else if (std::strcmp(argv[1],"2") == 0) { part1_2(); }
	else if (std::strcmp(argv[1],"3") == 0) { part1_3(); }
	else if (std::strcmp(argv[1],"4") == 0) { part1_4(); }
	else if (std::strcmp(argv[1],"5") == 0) { part1_5(); }
	else if (std::strcmp(argv[1],"6") == 0) { part1_6(); }
	else if (std::strcmp(argv[1],"all") == 0) {
		part1_1();
		part1_2();
		part1_3();
		part1_4();
		part1_5();
		part1_6();
	}*/

	part1_1();
	part1_2();
	part1_3();
	part1_4();
	part1_5();
	part1_6();

	part1_7();
	part1_8();


	return 0;
}
