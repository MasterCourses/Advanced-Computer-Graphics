//
//  main.cpp
//  CG_HW2
//
//  Created by NUZEROVI on 2021/3/21.
//

#include <iostream>
#include <fstream>
#include <ctime>

#include <cmath>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#include "algebra3.h"
#include "main.h"
#include "object.h"
#include "ray.h"


#include "hittable.h"
#include "hittable_list.h"
#include "util.h"
#include "aabb.h"
#include "bvh.h"
#include "TriMesh.h"


using namespace std;

#define PI 3.1415926535898
#define Tan(th) tan(PI/180*(th))

int Rw, Rh;
vec3 UL, UR, LL;
ColorImage image;
Pixel color;
Pixel black = {0, 0, 0};
Pixel green = {0, 255, 0};
Camera camera;
Viewport viewport;
Ray ray;

Material material;
vector<Light> lights;
vector<TriMesh> meshTriangles;
static TriMesh mesh;
Light light ;
bool meshExists = true;
int v1index = 0;
int v2index = 0;
int v3index = 0;
int mi = 0;

bool readFile()
{
    string txt;
    int exp;
    float Ka, Kd, Ks, Reflect;
    vec3 vec, lightPos, nor;
    vec3 v1, v2, v3;
    int index = -1;
    ifstream inFile("Input_Suzanne.txt");
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
                viewport.width = viewport.width ;
                viewport.height = viewport.height  ;
                break;
            case 'T':                   
                inFile >> v1[VX] >> v1[VY] >> v1[VZ] ;
                inFile >> v2[VX] >> v2[VY] >> v2[VZ] ;
                inFile >> v3[VX] >> v3[VY] >> v3[VZ] ;
                inFile >> nor[VX] >> nor[VY] >> nor[VZ];
                mesh.loadVertices(v1, v2, v3, nor, index);                                                             
                break;
            case 'M':
                inFile >> material.color[VX] >> material.color[VY] >> material.color[VZ];
                inFile >> Ka >> Kd >> Ks >> exp >> Reflect;
                material.setMaterial(material.color, Ka, Kd, Ks, exp, Reflect);
                index++;
                light.Ii = material.color * 255;
                light.material = material;
                lights.push_back(light);            
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

void CalculatePhong(vec3 hitpoint, vec3 n, int mi) { 
    vec3 N = n;
    N = N.normalize();

    for(int index = 0; index < lights.size(); index++)
    {
        if(mi == index){
            /* 1. Phong Lighting model */
            Material mat = lights[index].getMaterial();
            // diffuse
            vec3 L = (lights[index].getPosition() - hitpoint).normalize();
            vec3 Id = lights[index].getIi() * MAX(N*L, 0);
            
            // specular
            vec3 V = (camera.pos - hitpoint).normalize();
            vec3 H = (L + V).normalize();
            vec3 Is = lights[index].getIi() * (pow(MAX(N*H, 0.0), mat.exp));
            
            // Ambient + Diffuse + Specular
            vec3 I = mat.Ka * lights[index].getIi() + mat.Kd * Id + mat.Ks * Is; 
            color.R  = MIN(I[VX], 255);
            color.G  = MIN(I[VY], 255);
            color.B  = MIN(I[VZ], 255); 
        }
    }
	//return color;
}

Pixel raycast(Ray r, const hittable& world) {
	hit_record rec;

	if (world.hit(r,0,infinity,rec)) {
        // Shadows : create a ray from hitpoint to all light sources
        vec3 hitpoint = rec.p;
        for (int i = 0; i < lights.size(); i++) {                
            Ray shadow_ray = Ray(hitpoint, (lights[i].getPosition() - hitpoint).normalize());
            hit_record new_rec;
            if (world.hit(shadow_ray,0,infinity,new_rec)) {
                // color at that point is black
                return black;
            }                 
        }
        if (meshExists) {
            v1index = rec.v1i;
            v2index = rec.v2i;
            v3index = rec.v3i;            
        }  

        CalculatePhong(rec.p, rec.n, rec.mi);

        for (int i=0; i < lights.size(); i++)
        {
            if (lights[i].getMaterial().Reflect > 0){
                static int counter = 0;
                counter ++ ;
                Ray reflect_ray = Ray(hitpoint, (r.direction() - 2 * r.direction() * rec.n * rec.n).normalize());
                hit_record ref_rec;
                if (world.hit(reflect_ray, 0 , infinity, ref_rec)){     
                    if(counter == 3) {
                        counter = 0;
                        color.R = color.R + lights[i].getMaterial().Reflect * color.R;
                        color.G = color.G + lights[i].getMaterial().Reflect * color.G;
                        color.B = color.B + lights[i].getMaterial().Reflect * color.B;
                    }
                 
                }
            }
           
        }
        return color;
        
	}	
    return black;
}

int main() {
    clock_t start, end; // 儲存時間用的變數
    start = clock(); // 計算開始時間

    hittable_list world;

    if (readFile()) // Get txt info 
    {        
        // Mesh    
        world = mesh.generateTriangles();
        
	    // BVH
	    bvh_node bvh = bvh_node(world, 0, infinity, 10); 

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

                vec3 pointOnScreen = UL + (UR - UL) * u + (LL - UL) * v;
                Ray r(ray.orig, (pointOnScreen - ray.orig).normalize());                         
                image.writePixel(i, j, raycast(r, bvh));                   
            }

        }   
    }
    
    char out_file[] = "test_Suzanne.ppm";
    image.outputPPM(out_file);

    end = clock(); // 計算結束時間
    // For macOS output console
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; // 計算實際花費時間
    cout << "程式執行時間:" << cpu_time_used << " s" << endl;
    // For Windows output console
    // printf("%Lf", ((double) (end - start)) / CLOCKS_PER_SEC);
    // printf("\n");
    // printf("%g", ((double) (end - start)) / CLOCKS_PER_SEC);
    return 0;
}

