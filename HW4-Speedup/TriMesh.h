#ifndef TRIMESH_H
#define TRIMESH_H

#include <iostream>
#include <string>
#include <vector>
#include "hittable_list.h"
#include "TriangleMesh.h"
#include "object.h"

using namespace std;
using std::find;

int countIndex = 0;
int countIndex2 = 0;
vec3 f;



class TriMesh{
    private:
        int numVertices;
		int numFaces;
        vector<vec3> faces; //trianles index
    public:
		vector<int> mIndex;
    	vector<vec3> vertices;
        vector<vec3> normals;
       
    void loadVertices(vec3 v1, vec3 v2, vec3 v3, vec3 n, int index){
		
		int tmp;
		vector<vec3> tmpVert;
		tmpVert.push_back(v1);
		tmpVert.push_back(v2);
		tmpVert.push_back(v3);

		for (int i = 0; i < 3; i++)
		{
			countIndex ++ ;
			auto it = find(vertices.begin(), vertices.end(), tmpVert[i]);


			if( it != vertices.end()) // vertice exist in vector, so get index
			{            
				int index = it - vertices.begin();
				tmp = index;
			
			}else{
				vertices.push_back(tmpVert[i]); // add new triangle vertices     
				tmp = vertices.size() - 1;   // exist index            
			}
			
			// For mesh : add triangle index to faces
			switch (countIndex)
			{
				case 1:
					f[VX] = tmp;
					break;
				case 2:
					f[VY] = tmp;
					break;
				case 3:
					f[VZ] = tmp;
					faces.push_back(f);    
					countIndex = 0;
					break;
				default:
					break;
			}
		}
              
		normals.push_back(n);
		mIndex.push_back(index);
        numVertices = vertices.size();
		numFaces = faces.size();
    } 

    /*	Generates the triangular mesh
	*	returns a hittable_list of triangles
	*/
	hittable_list generateTriangles() {
		// faces stores a vector of 3 vertices
		// vertices stores a vector of 3 doubles
		hittable_list trianglesList;
		for (int i = 0; i < numFaces; i++) {
			vec3 face = faces[i];
			int index1 = face[0];
			int index2 = face[1];
			int index3 = face[2];
	
			vec3 v1 = vertices[index1];
			vec3 v2 = vertices[index2];
			vec3 v3 = vertices[index3];

			trianglesList.add(make_shared<TriangleMesh>(v1, v2, v3, normals[i], index1, index2, index3, mIndex[i]));
  
		}

		return trianglesList;
	}

};

#endif
