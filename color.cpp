#include "color.h"
#include <iostream>

using namespace std;


Color::Color(float r, float g, float b) {
	if (r < 0 or r > 1 or g < 0 or g > 1 or b < 0 or b > 1) {
		cerr << "Color Values Out Of Range." << endl;
		exit(1);
	}
	red = r;
	green = g;
	blue = b;
}
