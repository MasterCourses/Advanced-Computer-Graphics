#ifndef OBJECT_H
#define OBJECT_H

#include "main.h"
#include "ray.h"
#include <vector>

#include "hittable_list.h"

#include "TriangleMesh.h"

using namespace std;
using std::find;

// Light light;

class Material
{
    public:
        char obj;
        vec3 color;
        float Ka, Kd, Ks;
        int exp;
        float Reflect;

        void setMaterial(vec3 color, float Ka, float Kd, float Ks, int exp, float Reflect)
        {
            this->color = color;
            this->Ka = Ka;
            this->Kd = Kd;
            this->Ks = Ks;
            this->exp = exp;
            this->Reflect = Reflect;
        }
};


#endif
