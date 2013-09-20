#include "sphere.h"

Sphere::Sphere(Color* amibient, Color* diffuse, Color* specular, float coefficient) {
	ambient_color = amibient;
	diffuse_color = diffuse;
	specular_color = specular;
	power_coefficient = coefficient;
}