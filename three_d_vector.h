#ifndef THREEDVECTOR_H
#define THREEDVECTOR_H

class ThreeDVector{
	public:
		float x;
		float y;
		float z;
		ThreeDVector(float, float, float);
		float magnitude();
		void normalize();
		float dot_product(ThreeDVector*);
		ThreeDVector* cross_product(ThreeDVector*);
		ThreeDVector* scalar_multiply(float k);
};

#endif