#include "sphere.h"

Sphere::Sphere(ThreeDVector* ambient, ThreeDVector* diffuse, ThreeDVector* specular, float coefficient, float r, float _x, float _y, float _z) {
	ambient_coefficient = ambient;
	diffuse_coefficient = diffuse;
	specular_coefficient = specular;
	power_coefficient = coefficient;
	x = _x;
	y = _y;
	z = _z;
	radius = r;
}