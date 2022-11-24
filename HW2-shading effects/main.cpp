//
//  main.cpp
//  CG_HW2
//
//  Created by NUZEROVI on 2021/3/21.
//

#include <iostream>
#include <fstream>

#include "algebra3.h"
#include "main.h"
#include "object.h"

using namespace std;

#define PI 3.1415926535898
#define Tan(th) tan(PI/180*(th))

int Rw, Rh;
vec3 UL, UR, LL;
ColorImage image;
Pixel color;
Pixel black = {0, 0, 0};

Camera camera;
Viewport viewport;
Ray ray;

Material material;
Sphere sphere;
Triangle triangle;

bool readFile()
{
    string txt;
    int exp;
    float Ka, Kd, Ks, Reflect;
    ifstream inFile("hw2_input.txt");
    if(!inFile) return false;
    while(inFile >> txt)
    {
        switch (txt[0])
        {
            case 'E':
                inFile >> camera.pos[VX] >> camera.pos[VY] >> camera.pos[VZ];
                break;
            case 'V':
                inFile >> viewport.dir[VX] >> viewport.dir[VY] >> viewport.dir[VZ];
                inFile >> viewport.up_dir[VX] >> viewport.up_dir[VY] >> viewport.up_dir[VZ];
                break;
            case 'F':
                inFile >> camera.angle;
                break;
            case 'R':
                inFile >> viewport.width >> viewport.height;
                break;
            case 'S':
                inFile >> sphere.center[VX] >> sphere.center[VY] >> sphere.center[VZ];
                inFile >> sphere.radius;  
                sphere.material = material;     
                sphere.spheres.push_back(sphere);       
                break;
            case 'T':               
                for (int i = 0; i < 3; i++)
                        inFile >> triangle.vertices[i][VX] >> triangle.vertices[i][VY] >> triangle.vertices[i][VZ];
                triangle.material = material;
                triangle.triangles.push_back(triangle);                                   
                break;
            case 'M':
                inFile >> material.color[VX] >> material.color[VY] >> material.color[VZ];
                inFile >> Ka >> Kd >> Ks >> exp >> Reflect;
                material.setMaterial(material.color, Ka, Kd, Ks, exp, Reflect);
                break;
            case 'L':
                inFile >> light.pos[VX] >> light.pos[VY] >> light.pos[VZ]; 
                break;
            default:
                break;
        }
    }
    return true;
}

bool isShadow(vec3 P)
{
    Ray r(P, (light.pos - P).normalize());
    for (int i = 0; i < sphere.spheres.size(); i++)
    {               
        if(sphere.hitSphere(i, r)){    
            return true;
        }
    }
    for (int i = 0; i < triangle.triangles.size(); i++)
    {
        if(triangle.hitTriangle(i, r)){
            return true;
        }
    }
    return false;
}

bool isReflect(const Ray& r, vec3 P, vec3 N)
{
    vec3 reflectDir = ((r.dir - 2 * r.dir * N * N)).normalize();
    Ray rf(P, reflectDir);
    
    for (int i = 0; i < sphere.spheres.size(); i++)
    {               
        if(sphere.hitSphere(i, rf)){    
            return true;
        }
    }
    for (int i = 0; i < triangle.triangles.size(); i++)
    {
        if(triangle.hitTriangle(i, rf)){
            return true;
        }
    }
    return false;
}

void phongShading(const Ray& r, vec3 P, vec3 Ii, vec3 N, Material material)
{
    /* 1. Phong Lighting model */
    
    // diffuse
    vec3 L = (light.pos - P).normalize();
    vec3 Id = Ii * MAX(N*L, 0);

    // specular
    vec3 V = (camera.pos - P).normalize();
    vec3 H = (L + V).normalize();
    vec3 Is = Ii * (pow(MAX(N*H, 0.0), material.exp));
    
    // Ambient + Diffuse + Specular
    vec3 I = material.Ka * Ii + material.Kd * Id + material.Ks * Is; 
    color.R  = MIN(I[VX], 255);
    color.G  = MIN(I[VY], 255);
    color.B  = MIN(I[VZ], 255); 
    
    /* 2. Shadow */
    if(isShadow(P))
    {       
        if ( ((light.P - light.pos).length() < (P - light.pos).length()) ){
            color = black; 
        }
    }
    
    /* 3. Reflection */
    if (material.Reflect > 0)
    {   
        static int counter = 0;
        counter ++ ;
        if(isReflect(r, P, N))
        {
            if(counter == 3){
                counter = 0;
                color.R = color.R + material.Reflect * color.R;
                color.G = color.G + material.Reflect * color.G;
                color.B = color.B + material.Reflect * color.B;
            }
           
        }
    }
}


Pixel set_color(const Ray& r)
{
    for (int i = 0; i < sphere.spheres.size(); i++)
    {
        if(sphere.hitSphere(i, r)){   
            phongShading(r, light.P, light.Ii, sphere.N, sphere.material);  
            return color;                       
        }
    }
    for (int i = 0; i < triangle.triangles.size(); i++)
    {
        if(triangle.hitTriangle(i, r)){     
            phongShading(r, light.P, light.Ii, triangle.N, triangle.material); 
            return color;            
        }
    }
    return black;
}

void setCorner()
{
    vec3 hori_dir;
    hori_dir = (viewport.dir ^ viewport.up_dir).normalize();
    viewport.hori = hori_dir * ( Tan(camera.angle/2) * (viewport.dir).length() );
    viewport.vert = viewport.up_dir * ( Tan(camera.angle/2) * (viewport.dir).length() );    
 
    UL = viewport.hori + viewport.vert;
    UR = -viewport.hori + viewport.vert;
    LL = viewport.hori - viewport.vert;
}

int main() {
    if (readFile()) // Get txt info 
    {             
        // Setting output_image initial w,h        
        image.init(viewport.width, viewport.height);

        // Setting ray orig
        ray.orig = camera.pos; 
        viewport.center = viewport.dir - camera.pos;

        // Set viewport corner
        setCorner();

        for (int j = 0; j < viewport.height; ++j)
        {
            for (int i = 0; i < viewport.width; ++i)
            {
                float u = (float) i / viewport.width;
                float v = (float) j / viewport.height;

                vec3 pointOnScreen = UL + (UR- UL) * u + (LL - UL) * v;
                Ray r(ray.orig, (pointOnScreen - ray.orig).normalize());
                image.writePixel(i, j, set_color(r));                   
            }

        }   
    }
    
    char out_file[] = "HW2.ppm";
    image.outputPPM(out_file);
    
    return 0;
}

