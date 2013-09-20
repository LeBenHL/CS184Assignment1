#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light.h"

class PointLight: public Light {
  public:
  	PointLight(float, float, float, float, float, float);
};

#endif