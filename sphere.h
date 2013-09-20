#ifndef SPHERE_H
#define SPHERE_H
#include "color.h"

class Sphere {
  public:
     Color* ambient_color;
     Color* diffuse_color;
     Color* specular_color;
     float power_coefficient;
     float x, y, z, radius;
     Sphere(Color*, Color*, Color*, float, float, float=0, float=0, float=0);
};

#endif