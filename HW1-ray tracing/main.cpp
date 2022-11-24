//
//  main.cpp
//  CG_HW1
//
//  Created by NUZEROVI on 2021/3/8.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "algebra3.h"
#include "main.h"
#include "imageIO.h"

using namespace std;

Camera camera;
Viewport viewport;
Sphere sphere;
Triangle triangle;
Ray ray;
ColorImage image;
Pixel black = {0, 0, 0};
Pixel white = {255, 255, 255};
   
bool readFile()
{
    string txt;
    int count = 0;
    float Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz;
    ifstream inFile("hw1_input.txt");
    if(!inFile) return false;
    while(inFile >> txt)
    {
        switch (txt[0])
        {
            case 'E':
                inFile >> camera.pos[VX] >> camera.pos[VY] >> camera.pos[VZ];
                break;
            case 'O':
                for (int i = 0; i < 4; i++)
                    inFile >>  viewport.corner[i][VX] >>  viewport.corner[i][VY] >>  viewport.corner[i][VZ];
                break;
            case 'R':
                inFile >> viewport.width >> viewport.height;
                break;
            case 'S':
                inFile >> sphere.center[VX] >> sphere.center[VY] >> sphere.center[VZ];
                inFile >> sphere.radius;              
                break;
            case 'T':
                for (int i = 0; i < 3; i++)
                        inFile >>  triangle.vertices[3*count + i][VX] >>  triangle.vertices[3*count + i][VY] >>  triangle.vertices[3*count + i][VZ];
                count ++;                           
                break;
            default:
                break;
        }
    }
    return true;
}

bool hitSphere(const vec3 & center, float radius,const Ray& r)
{
    vec3 oc = r.origin() - center;
    float a = r.direction() * r.direction();
    float b = 2.0 * oc * r.direction();
    float c = oc * oc -radius * radius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant>0);
}

// Using Möller–Trumbore Algorithm
bool hitTriangle(int triNums, const Ray& r)
{
    vec3 a = triangle.vertices[3 * triNums];
    vec3 b = triangle.vertices[3 * triNums + 1];
    vec3 c = triangle.vertices[3 * triNums + 2];

    vec3 n = (b-a) ^ (c-a);

    mat3 A = (mat3(-(r.direction()), b-a, c-a)).transpose();
    
    vec3 S = r.origin() - a;

    vec3 s1 = r.direction() ^ (c-a);
    vec3 s2 = S ^ (b-a);
    float coeff = 1.0 / (s1 * (b-a));
    float t = coeff * (s2 * (c-a));
    float b1 = coeff * (s1 * S);
    float b2 = coeff * (s2 * r.direction());

    if (t>=0 && b1 >=0 && b2 >=0 && (b1+b2) <=1)
        return true;

    return false;
}

Pixel set_color(const Ray& r)
{
   
    for (int i = 0; i < 2; i++)
    {
        if(hitTriangle(i, r)){
            return white;
        }
    }
    if(hitSphere(sphere.center, sphere.radius, r)){
        return white;
    }
    return black;
}

int main() {
    if (readFile()) // Get txt info 
    { 
        // Setting output_image initial w,h        
        image.init(viewport.width, viewport.height);

        // Setting viewport center
        vec3 UL = viewport.corner[0];
        vec3 UR = viewport.corner[1];
        vec3 LL = viewport.corner[2];

        // Setting ray orig
        ray.orig = camera.pos; 

        for (int j = 0; j < viewport.height; ++j)
        {
            for (int i = 0; i < viewport.width; ++i)
            {
                float u = (float) i / viewport.width;
                float v = (float) j / viewport.height;

                vec3 pointOnScreen = UL + (UR- UL) * u + (LL - UL) * v;
                Ray r(ray.orig, (pointOnScreen - ray.orig).normalize()); // ray (orig, dir)
                image.writePixel(i, j, set_color(r));                        
            }

        }   
       
    }
    
    char out_file[] = "HW1.ppm";
    image.outputPPM(out_file);
    
    return 0;
}


/*
bool hitTriangle_old(int triNums, const Ray& r)
{
    vec3 a = triangle.vertices[3 * triNums];
    vec3 b = triangle.vertices[3 * triNums + 1];
    vec3 c = triangle.vertices[3 * triNums + 2];
    vec3 n = (b-a) ^ (c-a);

    mat3 A = (mat3(c-b, a-b, -(r.direction())).transpose()).inverse();
    
    vec3 B = r.origin() - b;

    float s1 = A[0] * B;
    float s2 = A[1] * B;
    float t = A[2] * B;

    if (t <= 0)
        return false;
    if ((s1 < 0) || (s1 > 1))
        return false;
    if ((s2 < 0) || (s2 > 1))
        return false;
    if (s1+s2 > 1) 
        return false;
    if (n * r.direction() == 0) // if ray hit normal
        return false;

    return true;
}
*/
