#ifndef COLOR_H
#define COLOR_H
#include "three_d_vector.h"

class Color: public ThreeDVector {
  public:
    Color(float=0, float=0, float=0);
};

#endif