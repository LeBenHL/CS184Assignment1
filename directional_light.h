#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "light.h"

class DirectionalLight: public Light {
  public:
  	DirectionalLight(float, float, float, float, float, float);
};

#endif