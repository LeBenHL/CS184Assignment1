#ifndef SPHERE_H
#define SPHERE_H
#include "three_d_vector.h"

class Sphere {
  public:
     ThreeDVector* ambient_coefficient;
     ThreeDVector* diffuse_coefficient;
     ThreeDVector* specular_coefficient;
     float power_coefficient;
     float x, y, z, radius;
     Sphere(ThreeDVector*, ThreeDVector*, ThreeDVector*, float, float, float=0, float=0, float=0);
};

#endif