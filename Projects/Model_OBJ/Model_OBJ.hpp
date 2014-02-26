#ifndef MODEL_OBJ_HPP_INCLUDED
#define MODEL_OBJ_HPP_INCLUDED


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "../../EsgiGL/EsgiGL.h"

#define KEY_ESCAPE 27

using namespace std;




#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9
/************************************************************************
  Window
 ************************************************************************/

typedef struct {
    int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;



/***************************************************************************
  OBJ Loading
 ***************************************************************************/

class Model_OBJ
{
  public:
	Model_OBJ();
    vec3 calculateNormal(vec3 coord1,vec3 coord2,vec3 coord3 );
    int Load(char *filename);	// Loads the model
	void Draw();					// Draws the model on the screen
	void Release();				// Release the model

	float* normals;							// Stores the normals
    float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles

};

#endif // MODEL_OBJ_HPP_INCLUDED
