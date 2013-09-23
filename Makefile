OBJS = example_01.o sphere.o point_light.o directional_light.o three_d_vector.o libSOIL.a
CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL -framework CoreFoundation\
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU
endif
	
RM = /bin/rm -f 
all: main 
main: $(OBJS)
	$(CC) $(CFLAGS) -o as1 $(OBJS) $(LDFLAGS) 
example_01.o: example_01.cpp sphere.h point_light.h directional_light.h three_d_vector.h
	$(CC) $(CFLAGS) -c example_01.cpp -o example_01.o
sphere.o: sphere.h sphere.cpp three_d_vector.h
	$(CC) $(CFLAGS) -c sphere.cpp -o sphere.o
point_light.o: point_light.h point_light.cpp light.h
	$(CC) $(CFLAGS) -c point_light.cpp -o point_light.o
directional_light.o: directional_light.h directional_light.cpp light.h three_d_vector.h
	$(CC) $(CFLAGS) -c directional_light.cpp -o directional_light.o
three_d_vector.o: three_d_vector.h three_d_vector.cpp
	$(CC) $(CFLAGS) -c three_d_vector.cpp -o three_d_vector.o
clean: 
	$(RM) *.o as1
 


