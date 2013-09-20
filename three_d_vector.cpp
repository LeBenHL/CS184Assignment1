#include <cmath>
#include "three_d_vector.h"

ThreeDVector::ThreeDVector(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

float ThreeDVector::magnitude(){
	float x = this->x;
	float y = this->y;
	float z = this->z;
	return sqrt(x*x+y*y+z*z);
}

void ThreeDVector::normalize(){
	this->x /= magnitude();
	this->y /= magnitude();
	this->z /= magnitude();
}


float ThreeDVector::dot_product(ThreeDVector* v){
	this->x * v->x + this->y * v->y + this->z * v->z;
}


ThreeDVector* ThreeDVector::scalar_multiply(float k){
	return new ThreeDVector(k*this->x, k*this->y, k*this->z);
}