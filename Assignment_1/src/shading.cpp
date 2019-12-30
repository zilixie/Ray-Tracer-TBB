#include "shading.h"
#include "closest_intersection.h"


bool shading(
		const Ray & ray,
		Eigen::Vector3d & rgb,
		const std::vector<Object*> & objs,
		const std::vector<Light*> & lights,
		const int nth_reflection,
		const double t0,
		const bool & enable_shadow,
		const bool & enable_mirror) 
{

	Response* obj_response(new Response());

	if (closest_intersection(ray,objs,obj_response,t0)) {

		int obj_id = obj_response->id;
		double t = obj_response->t;
		Eigen::Vector3d n = obj_response->n;


		Eigen::Vector3d a = objs[obj_id]->material->ambient;
		Eigen::Vector3d d = objs[obj_id]->material->diffuse;
		Eigen::Vector3d s = objs[obj_id]->material->specular;
		Eigen::Vector3d color = a;
		double exp = objs[obj_id]->material->exp;

		double t_max;
		Eigen::Vector3d p = ray.origin + t * ray.direction;
		Eigen::Vector3d l; //direction point to light

		for (int i=0; i<lights.size(); i++) {
			Eigen::Vector3d intensity = Eigen::Vector3d(1,1,1) * lights[i]->intensity;
			lights[i]->reverse_light(p, l, t_max); //computes the inverse direction from p to light.
			Eigen::Vector3d v = (-1) * (ray.direction).normalized();
			Eigen::Vector3d h = (v + l).normalized();

			Ray ray_p2light;
			ray_p2light.origin = p;
			ray_p2light.direction = l;


			Eigen::Vector3d diffuse = fmax(0, n.dot(l)) * (intensity.array() * d.array()).matrix();
			Eigen::Vector3d specular = pow(fmax(0, n.dot(h)), exp) * (intensity.array() * s.array()).matrix();


			Response* obj_response_to_hit_point(new Response());
			bool check1 = closest_intersection(ray_p2light, objs, obj_response_to_hit_point, 0.000001);
			double t_hit_point = obj_response_to_hit_point->t;

			bool check2 = t_hit_point < t_max;  //means something is in the middle between light and point.
			bool is_shadow = check1 && check2;
			if (is_shadow && enable_shadow) { color = color + Eigen::Vector3d(0,0,0); }
			else { color = color + diffuse + specular; }
		}

		// assign the color that considered shading and shadow to rgb.
		rgb = color;
		if (nth_reflection < 3 && enable_mirror) {
			Eigen::Vector3d km = objs[obj_id]->material->mirror;
			Eigen::Vector3d in = ray.direction/(ray.direction).norm();
			Eigen::Vector3d out = in - 2*(in.dot(n))*n;

			Ray reflect_ray;
			reflect_ray.origin = ray.origin + t * ray.direction;
			reflect_ray.direction = out.normalized();

			Eigen::Vector3d rgb2 (0,0,0);
			shading(reflect_ray, rgb2, objs, lights, nth_reflection + 1, t0/1000000, enable_shadow, enable_mirror);
			rgb = rgb + (rgb2.array() * km.array()).matrix();
		}
		return true;
	}
	return false;

}
