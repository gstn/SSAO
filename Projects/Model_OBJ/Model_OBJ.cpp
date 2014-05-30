/*
 *
 * Demonstrates how to load and display an Wavefront OBJ file.
 * Using triangles and normals as static object. No texture mapping.
 *
 * OBJ files must be triangulated!!!
 * Non triangulated objects wont work!
 * You can use Blender to triangulate
 *
 */

#include "Model_OBJ.hpp"

using namespace std;

Model_OBJ::Model_OBJ()
{
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedPoints = 0;
	this->TotalPoints = 0;
	this->TotalTriangles = 0;
	this->TotalNormalPoints = 0;
	this->numFaces = 0;
	this->numNormals = 0;
	this->numUVs = 0;
	this->numVertices = 0;
	this->TotalUVs = 0;
}

vec3 Model_OBJ::calculateNormal( vec3 coord1, vec3 coord2, vec3 coord3 )
{
   /* calculate Vector1 and Vector2 */
  
   vec3 va, vb, vr;
   //float val;

   va = coord1 - coord2;
   vb = coord1 - coord3;

   /* cross product */
   vr.x = va.y * vb.z - vb.y * va.z;
   vr.y = va.z * vb.x - vb.z * va.y;
   vr.z = va.x * vb.y - vb.x * va.y;

   
   /* normalization factor 
   val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );

	float norm[3];
	norm[0] = vr[0]/val;
	norm[1] = vr[1]/val;
	norm[2] = vr[2]/val;*/


   return vr.Normalized();
}

void Model_OBJ::Init(char* filename)
{
	string line;
	ifstream objFile (filename);
	while(!objFile.eof())
    {
        // 5
        string line;
        getline(objFile, line);
        string type = line.substr(0,2);
 
        // 6
        if(type.compare("v ") == 0)
            this->numPositions++;
        else if(type.compare("vt") == 0)
            this->numUVs++;
        else if(type.compare("vn") == 0)
			this->numNormals++;
        else if(type.compare("f ") == 0)
			this->numFaces++;
    }

	this->numVertices = this->numFaces * 3;
	/*
	std::cout << "Positions : " << this->numPositions << std::endl;
	std::cout << "Vertices : " << this->numVertices << std::endl;
	std::cout << "UVs : " << this->numUVs << std::endl;
	std::cout << "Normals : " << this->numNormals << std::endl;
	std::cout << "Faces : " << this->numFaces << std::endl;
	*/

	this->numFaces *= 9;

	this->numNormals *= 3;
	this->numUVs *= 2;
    objFile.close();
}

int Model_OBJ::Load(char* filename)
{
	this->Init(filename);
	string line;
	ifstream objFile (filename);
	if (objFile.is_open())													// If obj file is open, continue
	{
		objFile.seekg (0, ios::end);										// Go to end of the file,
		long fileSize = objFile.tellg();									// get file size
		objFile.seekg (0, ios::beg);										// we'll use this to register memory for our 3d model
		std::cout << fileSize << std::endl;
		vertexBuffer = (float*) malloc (fileSize);							// Allocate memory for the verteces
		Faces_Triangles = (float*) malloc(fileSize * 3 *sizeof(float));			// Allocate memory for the triangles
		normals  = (float*) malloc(fileSize*sizeof(float));			// Allocate memory for the normals
		
		normalsBuffer = (float*) malloc(this->numNormals*sizeof(float));
		uvsBuffer = (float*) malloc(this->numUVs*sizeof(float));
		verticesBuffer = (float*) malloc(this->numVertices*sizeof(float));
		facesBuffer = (unsigned int*) malloc(this->numFaces*sizeof(unsigned int));
		normalsIndexesBuffer = (unsigned int*) malloc(this->numFaces*sizeof(unsigned int));


		int triangle_index = 0;												// Set triangle index to zero
		int normal_index = 0;												// Set normal index to zero

		while (! objFile.eof() )											// Start reading file data
		{
			getline (objFile,line);											// Get line from file

			if (line.c_str()[0] == 'v' && line.c_str()[1] == ' ')										// The first character is a v: on this line is a vertex stored.
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
				line[1] = ' ';
				sscanf(line.c_str(),"%f %f %f ",							// Read floats from the line: v X Y Z
					&vertexBuffer[TotalConnectedPoints],
					&vertexBuffer[TotalConnectedPoints+1],
					&vertexBuffer[TotalConnectedPoints+2]);
				//std::cout << "v (" << vertexBuffer[TotalConnectedPoints+0] << "," << vertexBuffer[TotalConnectedPoints+1] << "," << vertexBuffer[TotalConnectedPoints+2] << ")" << std::endl;
				sscanf(line.c_str(),"%f %f %f ",							// Read floats from the line: v X Y Z
					&verticesBuffer[TotalPoints],
					&verticesBuffer[TotalPoints+1],
					&verticesBuffer[TotalPoints+2]);
				//std::cout << "v (" << verticesBuffer[TotalPoints+0] << "," << verticesBuffer[TotalPoints+1] << "," << verticesBuffer[TotalPoints+2] << ")" << std::endl;
				
				TotalConnectedPoints += POINTS_PER_VERTEX;					// Add 3 to the total connected points
				TotalPoints += POINTS_PER_VERTEX;					// Add 3 to the total connected points
			}

			if (line.c_str()[0] == 'v' && line.c_str()[1] == 'n')										// The first character is a v: on this line is a vertex stored.
			{
				//line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
				//line[1] = ' ';
				sscanf(line.c_str(),"vn %f %f %f ",							// Read floats from the line: v X Y Z
					&normalsBuffer[TotalNormalPoints],
					&normalsBuffer[TotalNormalPoints+1],
					&normalsBuffer[TotalNormalPoints+2]);
				//std::cout << "vn (" << normalsBuffer[TotalNormalPoints+0] << "," << normalsBuffer[TotalNormalPoints+1] << "," << normalsBuffer[TotalNormalPoints+2] << ")" << std::endl;
				
				//vertexBuffer[TotalConnectedPoints] *= 0.5f;
				//vertexBuffer[TotalConnectedPoints+1] *= 0.5f;
				//vertexBuffer[TotalConnectedPoints+2] *= 0.5f;
				TotalNormalPoints += POINTS_PER_VERTEX;					// Add 3 to the total connected points
			}

			if (line.c_str()[0] == 'v' && line.c_str()[1] == 't')										// The first character is a v: on this line is a vertex stored.
			{
				//line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
				//line[1] = ' ';
				int vertexNumber[4] = { 0, 0, 0 };
				sscanf(line.c_str(),"vt %f %f ",							// Read floats from the line: v X Y Z
					&uvsBuffer[TotalUVs],
					&uvsBuffer[TotalUVs+1]);
				//std::cout << "vt (" << uvsBuffer[TotalUVs+0] << "," << uvsBuffer[TotalUVs+1] << ")" << std::endl;
				
				//vertexBuffer[TotalConnectedPoints] *= 0.5f;
				//vertexBuffer[TotalConnectedPoints+1] *= 0.5f;
				//vertexBuffer[TotalConnectedPoints+2] *= 0.5f;
				TotalUVs += 2;					// Add 3 to the total connected points
			}

			if (line.c_str()[0] == 'f')										// The first character is an 'f': on this line is a point stored
			{

				int indices[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
				if(sscanf(line.c_str(),"f %i//%i %i//%i %i//%i ",								// Read integers from the line:  f 1 2 3
					&indices[0],										// First point of our triangle. This is an
					&indices[6],
					&indices[1],										// pointer to our vertexBuffer list
					&indices[7],
					&indices[2],
					&indices[8]) <= 1)
				{
					if(sscanf(line.c_str(),"f %i/%i/%i %i/%i/%i %i/%i/%i ",								// Read integers from the line:  f 1 2 3
						&indices[0],							// First point of our triangle. This is an
						&indices[3],
						&indices[6],
						&indices[1],
						&indices[4],
						&indices[7],
						&indices[2],
						&indices[5],
						&indices[8]) <= 1)
					{
						sscanf(line.c_str(),"f %i %i %i ",								// Read integers from the line:  f 1 2 3
						&indices[0],										// First point of our triangle. This is an
						&indices[3],										// pointer to our vertexBuffer list
						&indices[6] );
					}
				}
				for(int i = 0; i < 9 ; i++)
				{
					indices[i]-=1;
				}

				facesBuffer[TotalTriangles + 0] = indices[0];										// First point of our triangle. This is an
				facesBuffer[TotalTriangles + 1] = indices[1];										// pointer to our vertexBuffer list
				facesBuffer[TotalTriangles + 2] = indices[2];
            
				//std::cout << "f (" << facesBuffer[TotalTriangles+0] << "," << facesBuffer[TotalTriangles+1] << "," << facesBuffer[TotalTriangles+2] << ")" << std::endl;
				//std::cout << "ft (" << facesBuffer[TotalTriangles+3] << "," << facesBuffer[TotalTriangles+4] << "," << facesBuffer[TotalTriangles+5] << ")" << std::endl;
				//std::cout << "fn (" << facesBuffer[TotalTriangles+6] << "," << facesBuffer[TotalTriangles+7] << "," << facesBuffer[TotalTriangles+8] << ")" << std::endl;
				TotalTriangles += 3;
			}


			if (line.c_str()[0] == 'f')										// The first character is an 'f': on this line is a point stored
			{
		    	//line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
				//char* l = new char[line.size()+1];
				//memcpy(l, line.c_str(), line.size()+1);
				//char * pch;
				//pch = strtok(l, " ");
				int vertexNumber[4] = { 0, 0, 0 };
				
                /*sscanf(line.c_str(),"%i%i%i",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],										// First point of our triangle. This is an
					&vertexNumber[1],										// pointer to our vertexBuffer list
					&vertexNumber[2] );	*/									// each point represents an X,Y,Z.
				
				if(sscanf(line.c_str(),"f %i//%*i %i//%*i %i//%*i ",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],										// First point of our triangle. This is an
					&vertexNumber[1],										// pointer to our vertexBuffer list
					&vertexNumber[2] ) <= 1)
				{
					if(sscanf(line.c_str(),"f %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i ",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],										// First point of our triangle. This is an
					&vertexNumber[1],										// pointer to our vertexBuffer list
					&vertexNumber[2] ) <= 1)
					{
						sscanf(line.c_str(),"f %i %i %i ",								// Read integers from the line:  f 1 2 3
						&vertexNumber[0],										// First point of our triangle. This is an
						&vertexNumber[1],										// pointer to our vertexBuffer list
						&vertexNumber[2] );
					}
				}

				vertexNumber[0] -= 1;										// OBJ file starts counting from 1
				vertexNumber[1] -= 1;										// OBJ file starts counting from 1
				vertexNumber[2] -= 1;										// OBJ file starts counting from 1


				//********************************************************************
				//* Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z).
				//* The vertexBuffer contains all verteces
				//* The triangles will be created using the verteces we read previously
				///

				int tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					Faces_Triangles[triangle_index + tCounter   ] = vertexBuffer[3*vertexNumber[i] ];
					Faces_Triangles[triangle_index + tCounter +1 ] = vertexBuffer[3*vertexNumber[i]+1 ];
					Faces_Triangles[triangle_index + tCounter +2 ] = vertexBuffer[3*vertexNumber[i]+2 ];
					//std::cout << "(" << Faces_Triangles[triangle_index + tCounter   ] << "," << Faces_Triangles[triangle_index + tCounter + 1] << "," << Faces_Triangles[triangle_index + tCounter + 2 ] << ")" << std::endl; 
					tCounter += POINTS_PER_VERTEX;
				}

				//*********************************************************************
				// * Calculate all normals, used for lighting
				//
				vec3 coord1(Faces_Triangles[triangle_index], Faces_Triangles[triangle_index+1],Faces_Triangles[triangle_index+2]);
				vec3 coord2(Faces_Triangles[triangle_index+3],Faces_Triangles[triangle_index+4],Faces_Triangles[triangle_index+5]);
				vec3 coord3(Faces_Triangles[triangle_index+6],Faces_Triangles[triangle_index+7],Faces_Triangles[triangle_index+8]);
				vec3 norm = this->calculateNormal( coord1, coord2, coord3 );

				tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					normals[normal_index + tCounter ] = norm.x;
					normals[normal_index + tCounter +1] = norm.y;
					normals[normal_index + tCounter +2] = norm.z;

					tCounter += POINTS_PER_VERTEX;
				}

				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;
			}
		}
		objFile.close();														// Close OBJ file
	}
	else
	{
		cout << "Unable to open file";
	}
	return 0;
}

void Model_OBJ::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
}

void Model_OBJ::Process(float elapsedTime)
{
	this->transform.orientation += this->transform.rotationSpeed * elapsedTime;
	this->transform.orientation = fmodf(this->transform.orientation, 360.f);
}

void Model_OBJ::Draw( GLuint programObject)
{
 	/*glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
 	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3,GL_FLOAT,	0,Faces_Triangles);				// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays*/

	GLint position_attribute = 0;
	GLint normal_attribute = 0;
			unsigned int toto[36] = {	0,6,4,
								0,2,6,
								0,3,2,
								0,1,3,
								2,7,6,
								2,3,7,
								4,6,7,
								4,7,5,
								0,4,5,
								0,5,1,
								1,5,7,
								1,7,3};
	

	position_attribute = glGetAttribLocation(programObject, "a_Position");

	//glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_TRUE, 0, Faces_Triangles);
	glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_TRUE, 0, verticesBuffer);
	glEnableVertexAttribArray(position_attribute);

	normal_attribute = glGetAttribLocation(programObject, "a_Normal");
	glVertexAttribPointer(normal_attribute, 3, GL_FLOAT, GL_FALSE, 0, normalsBuffer);
	glEnableVertexAttribArray(normal_attribute);
	//glNormalPointer(GL_FLOAT, 0, normals);
	//glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);
	glDrawElements(GL_TRIANGLES,this->TotalTriangles, GL_UNSIGNED_SHORT, this->facesBuffer);

	glDisableVertexAttribArray(position_attribute);
	glDisableVertexAttribArray(normal_attribute);
}

