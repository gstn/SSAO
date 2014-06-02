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
#include "../../EsgiGL/Common/matrix.h"

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


typedef struct 
{
	vec3 position;
	float orientation;
	float rotationSpeed;
}Transform;


/***************************************************************************
  OBJ Loading
 ***************************************************************************/

class Model_OBJ
{
  public:
	Model_OBJ();
    vec3 calculateNormal(vec3 coord1,vec3 coord2,vec3 coord3 );
	Transform transform;

	void Init(char* filename);
    int Load(char *filename);	// Loads the model
	void Draw(GLuint programObject);					// Draws the model on the screen
	void Release();				// Release the model
	void Process(float elapsedTime);
	float* normals;							// Stores the normals
    float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	float* verticesBuffer;	
	float* normalsBuffer;
	float* uvsBuffer;
	unsigned int* facesBuffer;
	unsigned int* normalsIndexesBuffer;
	unsigned int* uvsIndexesBuffer;
	std::vector<float> vertices;
	std::vector<unsigned short> indicesTri;
	std::vector<float> normalsBuf;

	int numPositions;
	int numVertices;
	int numNormals;
	int numFaces;
	int numUVs;
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles
	long TotalNormalPoints;
	long TotalPoints;
	long TotalTriangles;
	long TotalUVs;
	
};




#endif // MODEL_OBJ_HPP_INCLUDED
