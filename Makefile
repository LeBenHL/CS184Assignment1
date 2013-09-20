CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU
endif
	
RM = /bin/rm -f 
all: main 
main: example_01.o color.o sphere.o point_light.o directional_light.o
	$(CC) $(CFLAGS) -o as1 example_01.o color.o sphere.o point_light.o directional_light.o $(LDFLAGS) 
example_01.o: example_01.cpp color.h sphere.h
	$(CC) $(CFLAGS) -c example_01.cpp -o example_01.o
color.o: color.h color.cpp
	$(CC) $(CFLAGS) -c color.cpp -o color.o
sphere.o: sphere.h sphere.cpp color.h
	$(CC) $(CFLAGS) -c sphere.cpp -o sphere.o
point_light.o: point_light.h point_light.cpp light.h
	$(CC) $(CFLAGS) -c point_light.cpp -o point_light.o
directional_light.o: directional_light.h directional_light.cpp light.h
	$(CC) $(CFLAGS) -c directional_light.cpp -o directional_light.o
clean: 
	$(RM) *.o as1
 


