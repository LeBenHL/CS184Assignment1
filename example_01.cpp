
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

//Ben and Lichen Classes
#include "sphere.h"
#include "point_light.h"
#include "directional_light.h"
#include "three_d_vector.h"

#define PI 3.14159265  // Should be used from mathlib
#define KEY_SPC 32
#define DEFAULT_COEFFICIENT 0
inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;
static vector<PointLight> point_lights;
static vector<DirectionalLight> directional_lights;
static vector<Sphere> spheres;

//****************************************************
// Simple init function
//****************************************************
void initScene(){
}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
  // centers 
  // Note: Need to check for gap
  // bug on inst machines.
}

//****************************************************
// Draw a filled circle.  
//****************************************************


void circle(Sphere* sphere) {
  float centerX = sphere->x;
  float centerY = sphere->y;
  float radius = sphere->radius;
  // Draw inner circle
  glBegin(GL_POINTS);

  // We could eliminate wasted work by only looping over the pixels
  // inside the sphere's radius.  But the example is more clear this
  // way.  In general drawing an object by loopig over the whole
  // screen is wasteful.

  int i,j;  // Pixel indices

  int minI = max(0,(int)floor(centerX-radius));
  int maxI = min(viewport.w-1,(int)ceil(centerX+radius));

  int minJ = max(0,(int)floor(centerY-radius));
  int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));



  for (i=0;i<viewport.w;i++) {
    for (j=0;j<viewport.h;j++) {

      // Location of the center of pixel relative to center of sphere
      float x = (i+0.5-centerX);
      float y = (j+0.5-centerY);

      float dist = sqrt(sqr(x) + sqr(y));

      ThreeDVector pixel_color = ThreeDVector();

      //Ambient Color
      ThreeDVector ambient_light = ThreeDVector(1, 1, 1);
      ThreeDVector* ambient_coefficient = sphere->ambient_coefficient;
      ThreeDVector* ambient_component = ambient_coefficient->vector_multiply(&ambient_light); //NEW OBJECT

      pixel_color.vector_add(ambient_component);

      if (dist<=radius) {
        // This is the front-facing Z coordinate
        float z = sqrt(radius*radius-dist*dist);

        setPixel(i,j, pixel_color.x, pixel_color.y, pixel_color.z);

        // This is amusing, but it assumes negative color values are treated reasonably.
        // setPixel(i,j, x/radius, y/radius, z/radius );
      }


    }
  }


  glEnd();
}
//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
  glLoadIdentity();				        // make sure transformation is "zero'd"


  // Start drawing
  for(vector<Sphere>::iterator i = spheres.begin(); i != spheres.end(); ++i) {
    circle(&(*i));
  }

  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}


void myKeyboardFunc(unsigned char key, int x, int y){
	if(key == KEY_SPC){
		exit(0);
	}
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  ThreeDVector ambient_coefficient = ThreeDVector();
  ThreeDVector diffuse_coefficient = ThreeDVector();
  ThreeDVector specular_coefficient = ThreeDVector();
  float coefficient = DEFAULT_COEFFICIENT;

  for(int i = 1; i < argc; i++){
  	if(string(argv[i]) == "-ka"){
  		if(i + 3 < argc){
        ambient_coefficient.x = atof(argv[i + 1]);
        ambient_coefficient.y = atof(argv[i + 2]);
        ambient_coefficient.z = atof(argv[i + 3]);
        i = i + 3;
  		}else{

  		}
  	}else if(string(argv[i]) == "-kd"){
  		if(i + 3 < argc){
        diffuse_coefficient.x = atof(argv[i + 1]);
        diffuse_coefficient.y = atof(argv[i + 2]);
        diffuse_coefficient.z = atof(argv[i + 3]);
        i = i + 3;
  		}else{

  		}
  	}else if(string(argv[i]) == "-ks"){
  		if(i + 3 < argc){
        specular_coefficient.x = atof(argv[i + 1]);
        specular_coefficient.y = atof(argv[i + 2]);
        specular_coefficient.z = atof(argv[i + 3]);
        i = i + 3;
  		}else{

  		}
  	}else if(string(argv[i]) == "-sp"){
  		if(i + 1 < argc){
        coefficient = atof(argv[i+1]);
        i = i + 1;
  		}else{

  		}
  	}else if(string(argv[i]) == "-ps"){
  		if(i + 6 < argc){
        float x, y, z, r, g, b;
        x = atof(argv[i + 1]);
        y = atof(argv[i + 2]);
        z = atof(argv[i + 3]);
        r = atof(argv[i + 4]);
        g = atof(argv[i + 5]);
        b = atof(argv[i + 6]);
        point_lights.push_back(PointLight(x, y, z, r, g, b));
        i = i + 6;
  		}else{

  		}
  	}else if(string(argv[i]) == "-dl"){
  		if(i + 6 < argc){
        float x, y, z, r, g, b;
        x = atof(argv[i + 1]);
        y = atof(argv[i + 2]);
        z = atof(argv[i + 3]);
        r = atof(argv[i + 4]);
        g = atof(argv[i + 5]);
        b = atof(argv[i + 6]);
        directional_lights.push_back(DirectionalLight(x, y, z, r, g, b));
        i = i + 6;

  		}else{

  		}
  	}else{

  	}
  }

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  spheres.push_back(Sphere(&ambient_coefficient, &diffuse_coefficient, &specular_coefficient, coefficient, min(viewport.w, viewport.h) / 3.0, viewport.w / 2.0, viewport.h / 2.0));

  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene();							// quick function to set up scene

  glutDisplayFunc(myDisplay);				// function to run when its time to draw something
  glutReshapeFunc(myReshape);				// function to run when the window gets resized
  glutKeyboardFunc(myKeyboardFunc);
  glutMainLoop();							// infinite loop that will keep drawing and resizing
  // and whatever else

  return 0;
}








