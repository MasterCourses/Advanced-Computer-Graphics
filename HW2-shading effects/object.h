#ifndef OBJECT_H
#define OBJECT_H

#include "main.h"
#include <vector>

using namespace std;

Light light;

class Material
{
    public:
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

class Triangle
{
    public:
        vec3 vertices[3];
        Material material;
        vector<Triangle> triangles;
        vec3 N;

    // Using Möller–Trumbore Algorithm
    bool hitTriangle(int i, const Ray& r)
    {
        vec3 a = triangles[i].vertices[0];
        vec3 b = triangles[i].vertices[1];
        vec3 c = triangles[i].vertices[2];

        N = ((b-a) ^ (c-a)).normalize();

        mat3 A = (mat3(-(r.direction()), b-a, c-a)).transpose();
    
        vec3 S = r.origin() - a;
        vec3 s1 = r.direction() ^ (c-a);
        vec3 s2 = S ^ (b-a);
        float coeff = 1.0 / (s1 * (b-a));
        float t = coeff * (s2 * (c-a));
        float b1 = coeff * (s1 * S);
        float b2 = coeff * (s2 * r.direction());

        vec3 col = triangles[i].material.color;

        if (N * r.direction() > 0)
        {
            N = N * -1;   
            N = N.normalize();    
        }

        if (t>=0 && b1 >=0 && b2 >=0 && (b1+b2) <=1)
        {
            light.P = r.at(t);           
            light.Ii = col * 255;      

        }

        return (t>=0 && b1 >=0 && b2 >=0 && (b1+b2) <=1);    
    }
};

class Sphere
{
    public:
        vec3 center;
        float radius;
        Material material;   
        vector<Sphere> spheres;
        vec3 N;

    bool hitSphere(int i, const Ray& r)
    {
        center = spheres[i].center;
        radius = spheres[i].radius;

        vec3 col = spheres[i].material.color;
        vec3 oc = r.origin() - center;
        float a = r.direction() * r.direction();
        float b = 2.0 * oc * r.direction();
        float c = oc * oc - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant > 0)
        {

            float t = ( -b - sqrt(discriminant) ) / 2;
            if (t > 0)
            {
                light.P = r.at(t);
                N = (light.P - center).normalize(); 
                light.Ii = col * 255;   
            }                               
        }   
       
        return (discriminant > 0);
    }    
};

#endif
