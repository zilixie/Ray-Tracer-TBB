# Ray-Tracer-TBB
## Data Structures
The main idea here is that we consider each principle element that constructs a part of the scene as a individual class, and define those classes in .h files.  

### scene_elements.h
scene_elements.h is where I defined general elements that construct the scene.  

A Ray has origin (eye) and direction (d).   
Both of these two attributes are Eigen vector with 3 doubles.  

  class Ray {  
    public:  
      Eigen::Vector3d origin;  
      Eigen::Vector3d direction;  
  };  
Object is a class to be implemented by Sphere, Triangle, TriMesh and Plane. It has a not implemented method call ray_intersect which takes in a Ray instance as input and returns a bool indicating whether this object is hitted by the given ray. If it is hit, then at meantime it will output t and n, which are respectively the parameterized distance of ray from origin to intersection point and the normal at the intersection point. In addition, Object has a material attribute that specifies the material features (ka, kd, ks, km and phong exp) that this object is using.

  class Object {
    public:
      Material* material;
    virtual bool ray_intersect(const Ray & ray, double & t, Eigen::Vector3d & n, const double t0) const = 0;
  };
Light has point and intensity. reverse_light computes the distance and direction from a given point in 3d to the origin of the light source.

  class Light {
    public:
      Eigen::Vector3d p;
      double intensity;
    void reverse_light(const Eigen::Vector3d & query, Eigen::Vector3d & direction, double & distance) const;
  };
Consider a scene as a sandbox, given a ray, the scene will response with: id: index of obj that was hit by ray. t: parameterized distance n: normal of the hit point.  

  class Response {
    public:
      int id;
      double t;
      Eigen::Vector3d n;
  };
