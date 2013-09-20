#include "color.h"

class Sphere {
  public:
     Color* ambient_color;
     Color* diffuse_color;
     Color* specular_color;
     float power_coefficient;
     Sphere(Color*, Color*, Color*, float);
};