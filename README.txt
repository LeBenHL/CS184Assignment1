Names: Ben Le (cs184-bn), Lichen Han (cs184-av)

Platform: Mac OS X

Location: cs184-av, Mac OS X

Extra Credit Features

Cool-Warm Shading: -cws x y z rc gc bc rw gw bw
This will apply cool to warm shading in the x,y,z direction. Parameters rc gc bc represent the cool color and rw gw bw the warm color.
ex) ./as1 -cws 1 0 0 .4 .4 .7 .8 .6 .6

Toon Shading: -tn
This will apply toon shading to a sphere based on its Phong shading
ex) ./as1 -kd .6 .6 .6 -dl 0 1 0 1 1 1 -tn

Save Image: -save str_name
This will save the image as a .bmp file as str_name
ex) ./as1 -kd .1 .8 .8 -dl -1 -.5 -1 1 1 1 -save sample.bmp

Animation: -anim angular_speed r g b
This adds a new direction light beginning at 0,0,-1 with colors r g b that rotates about the z-axis around the sphere at angular_speed (which is in degrees per second).
ex) ./as1 -kd .6 .6 .6 -anim 180 1 1 1 

