#include <cmath>
#include "three_d_vector.h"
#include <iostream>
using namespace std;

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
	//cout << "BEFORE: " << this->x << ", " << this->y << ", " << this->z << endl;
	float mag = magnitude();
	this->x /= mag;
	this->y /= mag;
	this->z /= mag;
	//cout << "AFTER: " << this->x << ", " << this->y << ", " << this->z << endl;
}


float ThreeDVector::dot_product(ThreeDVector* v){
	return this->x * v->x + this->y * v->y + this->z * v->z;
}


void ThreeDVector::scalar_multiply(float k){
	this->x *= k;
	this->y *= k;
	this->z *= k;
}

void ThreeDVector::vector_add(ThreeDVector* v){
	this->x += v->x;
	this->y += v->y;
	this->z += v->z;
}

ThreeDVector* ThreeDVector::vector_subtract(ThreeDVector* v){
	return new ThreeDVector(this->x - v->x, this->y - v->y, this->z - v->z);
}

ThreeDVector* ThreeDVector::vector_multiply(ThreeDVector* v){
	return new ThreeDVector(v->x * this->x, v->y * this->y, v->z * this->z);
}

ThreeDVector* ThreeDVector::clone(){
	return new ThreeDVector(this->x, this->y, this->z);
}