
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
static vector<PointLight*> point_lights;
static vector<DirectionalLight*> directional_lights;
static vector<Sphere*> spheres;

static ThreeDVector* cool_color;
static ThreeDVector* warm_color;
static ThreeDVector* cw_direction;


static bool cw_shade = false;
static bool toon_shade = false;

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

ThreeDVector* calculateDiffuse(Light* l, ThreeDVector* direction, ThreeDVector* diffuse, ThreeDVector* normal){
  //cout << "Light: " << l->red << ", " << l->green << ", " << l->blue << endl;
  //cout << "Direction: " << direction->x << ", " << direction->y << ", " << direction->z << endl;
  //cout << "Diffuse: " << diffuse->x << ", " << diffuse->y << ", " << diffuse->z << endl;
  //cout << "Normal: " << normal->x << ", " << normal->y << ", " << normal->z << endl;
  float dot_product = direction->dot_product(normal);
  //cout << "DOT PRODUCT: " << dot_product << endl;
  ThreeDVector light = ThreeDVector(l->red, l->green, l->blue);
  //cout << "Light1: " << light.x << ", " << light.y << ", " << light.z << endl;
  light.scalar_multiply(max(dot_product, float(0)));
  //cout << "Light1: " << light.x << ", " << light.y << ", " << light.z << endl;
  //cout << endl;
  return diffuse->vector_multiply(&light);
}

ThreeDVector* calculateSpecular(Light* l, ThreeDVector* reflect, ThreeDVector* specular, ThreeDVector* view, float power_coefficient){
  //cout << "Light: " << l->red << ", " << l->green << ", " << l->blue << endl;
  //cout << "Direction: " << direction->x << ", " << direction->y << ", " << direction->z << endl;
  //cout << "Diffuse: " << diffuse->x << ", " << diffuse->y << ", " << diffuse->z << endl;
  //cout << "Normal: " << normal->x << ", " << normal->y << ", " << normal->z << endl;
  float dot_product = view->dot_product(reflect);
  //cout << "DOT PRODUCT: " << dot_product << endl;
  ThreeDVector light = ThreeDVector(l->red, l->green, l->blue);
  //cout << "Light1: " << light.x << ", " << light.y << ", " << light.z << endl;
  light.scalar_multiply(pow(max(dot_product, float(0)), power_coefficient));
  //cout << "Light1: " << light.x << ", " << light.y << ", " << light.z << endl;
  //cout << endl;
  return specular->vector_multiply(&light);
}

float toonify(float color_intensity) {
  if (color_intensity >= 0 and color_intensity < .125) {
    return .0625;
  } else if (color_intensity >= .125 and color_intensity < .375) {
    return 0.25;
  } else if (color_intensity >= .375 and color_intensity < .625) {
    return 0.5;
  } else if (color_intensity >= .625 and color_intensity < .875) {
    return 0.75;
  } else if (color_intensity >= .875 and color_intensity <= 1) {
    return .9375;
  } else {
    return color_intensity;
  }
}

//****************************************************
// Draw a filled circle.  
//****************************************************


void circle(Sphere* sphere) {
  float centerX = viewport.w / 2.0;
  float centerY = viewport.h / 2.0;
  float radius = min(viewport.w, viewport.h) / 3.0;
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

    //Ambient Color
  ThreeDVector ambient_light = ThreeDVector(1, 1, 1);
  ThreeDVector* ambient_coefficient = sphere->ambient_coefficient;
  ThreeDVector* ambient_component = ambient_coefficient->vector_multiply(&ambient_light); //NEW OBJECT
  ThreeDVector* view_vector = new ThreeDVector(0, 0, 1);


  for (i=0;i<viewport.w;i++) {
    for (j=0;j<viewport.h;j++) {

      // Location of the center of pixel relative to center of sphere
      float x = (i+0.5-centerX);
      float y = (j+0.5-centerY);

      float dist = sqrt(sqr(x) + sqr(y));

      if (dist<=radius) {
        // This is the front-facing Z coordinate
        float z = sqrt(radius*radius-dist*dist);

        //cout << x << ", " << y << ", " << z << endl;
        ThreeDVector currentPoint = ThreeDVector(x, y, z);
        ThreeDVector center = ThreeDVector(0, 0, 0);
        ThreeDVector* normal = currentPoint.vector_subtract(&center);
        normal->normalize();

        ThreeDVector pixel_color = ThreeDVector();

        if (cw_shade) {
          float warmth_constant = (1 + normal->dot_product(cw_direction))/2;
          pixel_color.x = warmth_constant*(warm_color->x) + (1-warmth_constant)*(cool_color->x);
          pixel_color.y = warmth_constant*(warm_color->y) + (1-warmth_constant)*(cool_color->y);
          pixel_color.z = warmth_constant*(warm_color->z) + (1-warmth_constant)*(cool_color->z);

        } else {
          pixel_color.vector_add(ambient_component);

          //Directional Lights
          for (vector<DirectionalLight*>::iterator i = directional_lights.begin(); i != directional_lights.end(); ++i) {
            ThreeDVector* direction = (*i)->direction;
            //cout << "Direction: " << direction->x << ", " << direction->y << ", " << direction->z << endl;
            ThreeDVector* diffuse_component = calculateDiffuse(*i, direction, sphere->diffuse_coefficient, normal); //returns NEW OBJ
            //cout << diffuse_component->x << ", " << diffuse_component->y << ", " << diffuse_component->z << endl;
            pixel_color.vector_add(diffuse_component);
            //cout << pixel_color.x << ", " << pixel_color.y << ", " << pixel_color.z << endl;

            ThreeDVector* direction_clone = direction->clone(); //new obj!
            direction_clone->scalar_multiply(-1);
            ThreeDVector* normal_clone = normal->clone(); //new obj!
            normal_clone->scalar_multiply(2*direction->dot_product(normal));
            direction_clone->vector_add(normal_clone);

            //cout << "Direction: " << direction->x << ", " << direction->y << ", " << direction->z << endl;
            ThreeDVector* specular_component = calculateSpecular(*i, direction_clone, sphere->specular_coefficient, view_vector, sphere->power_coefficient); //returns NEW OBJ
            //cout << diffuse_component->x << ", " << diffuse_component->y << ", " << diffuse_component->z << endl;
            pixel_color.vector_add(specular_component);
            //cout << pixel_color.x << ", " << pixel_color.y << ", " << pixel_color.z << endl;

            delete diffuse_component;
            delete direction_clone;
            delete normal_clone;
            delete specular_component;

          }

          //cout << endl;

          //Point Lights
          for (vector<PointLight*>::iterator i = point_lights.begin(); i != point_lights.end(); ++i) {
            ThreeDVector pointLightPosition = ThreeDVector((*i)->x, (*i)->y, (*i)->z);
            //cout << "DIRECTION1: " << pointLightPosition.x << ", " << pointLightPosition.y << ", " << pointLightPosition.z << endl;
            pointLightPosition.scalar_multiply(radius);
            //cout << "DIRECTION2: " << pointLightPosition.x << ", " << pointLightPosition.y << ", " << pointLightPosition.z << endl;
            ThreeDVector* direction = pointLightPosition.vector_subtract(&currentPoint); // new OBJ
            //cout << "DIRECTION3: " << direction->x << ", " << direction->y << ", " << direction->z << endl;
            direction->normalize();
            ThreeDVector* diffuse_component = calculateDiffuse(*i, direction, sphere->diffuse_coefficient, normal); //returns NEW OBJ
            pixel_color.vector_add(diffuse_component);

            ThreeDVector* direction_clone = direction->clone(); //new obj!
            direction_clone->scalar_multiply(-1);
            ThreeDVector* normal_clone = normal->clone(); //new obj!
            normal_clone->scalar_multiply(2*direction->dot_product(normal));
            direction_clone->vector_add(normal_clone);

            //cout << "Direction: " << direction->x << ", " << direction->y << ", " << direction->z << endl;
            ThreeDVector* specular_component = calculateSpecular(*i, direction_clone, sphere->specular_coefficient, view_vector, sphere->power_coefficient); //returns NEW OBJ
            //cout << diffuse_component->x << ", " << diffuse_component->y << ", " << diffuse_component->z << endl;
            pixel_color.vector_add(specular_component);
            //cout << pixel_color.x << ", " << pixel_color.y << ", " << pixel_color.z << endl;
            
            delete direction_clone;
            delete normal_clone;
            delete specular_component;

            delete direction;
            delete diffuse_component;
          }

        }

        if (toon_shade) {
          setPixel(i,j, toonify(pixel_color.x), toonify(pixel_color.y), toonify(pixel_color.z));
        } else {
          setPixel(i,j, pixel_color.x, pixel_color.y, pixel_color.z);
        }

        // This is amusing, but it assumes negative color values are treated reasonably.
        // setPixel(i,j, x/radius, y/radius, z/radius );
      }


    }
  }
  delete ambient_component;
  delete view_vector;

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
  for(vector<Sphere*>::iterator i = spheres.begin(); i != spheres.end(); ++i) {
    circle(*i);
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
  	}else if(string(argv[i]) == "-pl"){
  		if(i + 6 < argc){
        float x, y, z, r, g, b;
        x = atof(argv[i + 1]);
        y = atof(argv[i + 2]);
        z = atof(argv[i + 3]);
        r = atof(argv[i + 4]);
        g = atof(argv[i + 5]);
        b = atof(argv[i + 6]);
        PointLight* light = new PointLight(x, y, z, r, g, b);
        point_lights.push_back(light);
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
        DirectionalLight* light = new DirectionalLight(x, y, z, r, g, b);
        directional_lights.push_back(light);
        i = i + 6;

  		} else {
  		}
  	} else if (string(argv[i]) == "-cws") {
        if(i + 9 < argc){
          cw_shade = true;
          float x, y, z, rc, gc, bc, rw, gw, bw;
          x = atof(argv[i + 1]);
          y = atof(argv[i + 2]);
          z = atof(argv[i + 3]);
          rc = atof(argv[i + 4]);
          gc = atof(argv[i + 5]);
          bc = atof(argv[i + 6]);
          rw = atof(argv[i + 7]);
          gw = atof(argv[i + 8]);
          bw = atof(argv[i + 9]);
          cool_color = new ThreeDVector(rc, gc, bc);
          warm_color = new ThreeDVector(rw, gw, bw);
          cw_direction = new ThreeDVector(x, y, z);
          cw_direction->normalize();
          cw_direction->scalar_multiply(-1);
          i = i + 9;
        } else {
        }
  	} else if (string(argv[i]) == "-tn") {
      toon_shade = true;
    }
  }

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  Sphere* sphere = new Sphere(&ambient_coefficient, &diffuse_coefficient, &specular_coefficient, coefficient, min(viewport.w, viewport.h) / 3.0, viewport.w / 2.0, viewport.h / 2.0);
  spheres.push_back(sphere);

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








