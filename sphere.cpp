#include "sphere.h"

Sphere::Sphere(Color* amibient, Color* diffuse, Color* specular, float coefficient, float r, float _x, float _y, float _z) {
	ambient_color = amibient;
	diffuse_color = diffuse;
	specular_color = specular;
	power_coefficient = coefficient;
	x = _x;
	y = _y;
	z = _z;
	radius = r;
}