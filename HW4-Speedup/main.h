#ifndef MAIN_H
#define MAIN_H

#include "algebra3.h"
#include "imageIO.h"
#include "object.h"

class Light
{
	public:
		Light() {}
		

        vec3 getIi(){
            return Ii;
        }
        Material getMaterial(){
            return material;
        }
		vec3 getPosition() {
			return pos;
		}

		
    public: 
        vec3 pos;
        vec3 P, Ii; // hit point, ambient
        Material material;
};

class Camera
{
    public:
        vec3 pos;
        vec3 d;
        float angle;
};

class Viewport
{
    public:
        vec3 center;
        vec3 dir;
        vec3 up_dir;
        int width, height;
        vec3 hori, vert;
};

#endif

