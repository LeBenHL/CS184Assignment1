#ifndef THREEDVECTOR_H
#define THREEDVECTOR_H

class ThreeDVector{
	public:
		float x;
		float y;
		float z;
		ThreeDVector(float=0, float=0, float=0);
		float magnitude();
		void normalize();
		float dot_product(ThreeDVector*);
		void scalar_multiply(float k);
		void vector_add(ThreeDVector*);
		ThreeDVector* vector_subtract(ThreeDVector*);
		ThreeDVector* vector_multiply(ThreeDVector*);
		ThreeDVector* clone();
};

#endif