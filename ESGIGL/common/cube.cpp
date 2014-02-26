// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------

#include "cube.h"
#include "../EsgiGL.h"

// --- Globales --------------------------------------------------------------

unsigned int Cube::verticesVBO = 0;
unsigned int Cube::normalesVBO = 0;
unsigned int Cube::indicesVBO = 0;

unsigned int Cube::vertexCount = 0;
unsigned int Cube::indexCount = 0;
float* Cube::vertices = 0;
float* Cube::normales = 0;
short* Cube::indices = 0;

// cube basique

// 8 vertices
float basic_cube_vertices[] =
{
	-1, -1, +1,
	+1, -1, +1,
	+1, +1, +1,
	-1, +1, +1,
	-1, -1, -1,
	+1, -1, -1,
	+1, +1, -1,
	-1, +1, -1
};

// 0.55735 ~= sqrt( 1.f / 3.f )
// Il faut necessairement que la longueur d'une normale soit (approx. ici) egale a 1.0
float basic_cube_normales[] = 
{
	-0.55735f, -0.55735f, +0.55735f,
	+0.55735f, -0.55735f, +0.55735f,
	+0.55735f, +0.55735f, +0.55735f,
	-0.55735f, +0.55735f, +0.55735f,
	-0.55735f, -0.55735f, -0.55735f,
	+0.55735f, -0.55735f, -0.55735f,
	+0.55735f, +0.55735f, -0.55735f,
	-0.55735f, +0.55735f, -0.55735f
};

// 6 vertices par face (2 triangles)
short basic_cube_indices[] =
{
	0, 1, 2, // avant
	2, 3, 0,
	7, 6, 5, // arriere
	5, 4, 7,
	4, 5, 1, // bas
	1, 0, 4,
	3, 2, 6, // haut
	6, 7, 3,
	4, 0, 3, // gauche
	3, 7, 4,
	1, 5, 6, // droite
	6, 2, 1
};

// cube complexe, les triangles sont definis explicitement
// (on utilise tout de même des indices pour conserver un code homogène)

// 24 vertices
float complex_cube_vertices[] =
{
	-1, -1, +1, // avant, 0 à 3
	+1, -1, +1,
	+1, +1, +1,
	-1, +1, +1,
	-1, +1, +1, // haut, 4 à 7
	+1, +1, +1,
	+1, +1, -1,
	-1, +1, -1,
	-1, -1, -1, // arriere, 8 à 11
	+1, -1, -1,
	+1, +1, -1,
	-1, +1, -1,
	-1, -1, -1, // bas, 12 à 15
	+1, -1, -1,
	+1, -1, +1,
	-1, -1, +1,
	-1, -1, -1, // gauche, 16 à 19
	-1, -1, +1,
	-1, +1, +1,
	-1, +1, -1,
	+1, -1, +1, // droit, 20 à 23
	+1, -1, -1,
	+1, +1, -1,
	+1, +1, +1
};

float complex_cube_normales[] =
{
	0, 0, +1,	// avant
	0, 0, +1,
	0, 0, +1,
	0, 0, +1,

	0, +1, 0,	// haut
	0, +1, 0,
	0, +1, 0,
	0, +1, 0,

	0, 0, -1,	// arriere
	0, 0, -1,
	0, 0, -1,
	0, 0, -1,

	0, -1, 0,	// bas
	0, -1, 0,
	0, -1, 0,
	0, -1, 0,

	+1, 0, 0,	// gauche
	+1, 0, 0,
	+1, 0, 0,
	+1, 0, 0,

	-1, 0, 0,	// droite
	-1, 0, 0,
	-1, 0, 0,
	-1, 0, 0
};

// 6 vertices par face (2 triangles)
short complex_cube_indices[] =
{
	0, 1, 2, // avant
	0, 2, 3,
	4, 5, 6, // haut
	6, 7, 4,
	8, 9, 10, // arriere
	10, 11, 8,
	12, 13, 14, // bas
	14, 15, 12,
	16, 17, 18, // gauche
	18, 19, 16,
	20, 21, 22, // droite
	22, 23, 20
};

// --- Fonctions -------------------------------------------------------------

void Cube::InitialiseGlobals(eCubeMode mode)
{
	if (mode == eCube_Simple)
	{
		vertices = basic_cube_vertices;
		normales = basic_cube_normales;
		indices =  basic_cube_indices;
		vertexCount = 8;
	}
	else
	{
		vertices = complex_cube_vertices;
		normales = complex_cube_normales;
		indices = complex_cube_indices;
		vertexCount = 24;
	}
	indexCount = 3*2*6;
	
	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(GLfloat)*vertexCount, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &normalesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalesVBO);
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(GLfloat)*vertexCount, normales, GL_STATIC_DRAW);

	glGenBuffers(1, &indicesVBO);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*indexCount, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::Clean()
{
	glDeleteBuffers(1, &indicesVBO);
	glDeleteBuffers(1, &normalesVBO);
	glDeleteBuffers(1, &verticesVBO);
}

void Cube::Process(float elapsedTime)
{
	m_Orientation += m_AutoRotation * elapsedTime;
	m_Position += m_Velocity * elapsedTime;
}

void Cube::Render(GLuint programObject)
{
	GLint position_attribute = glGetAttribLocation(programObject, "a_Position");

	if (position_attribute != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
		glEnableVertexAttribArray(position_attribute);
		glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
#ifndef ESGI_GLES_20
	else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
	}
#endif

	GLint normal_attribute = glGetAttribLocation(programObject, "a_Normal");
	if (normal_attribute != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, normalesVBO);
		glEnableVertexAttribArray(normal_attribute);
		glVertexAttribPointer(normal_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (position_attribute != -1)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);	
		//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numIndices);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, indices);
	}
	if (normal_attribute != -1)
		glDisableVertexAttribArray(normal_attribute);
	if (position_attribute != -1)
		glDisableVertexAttribArray(position_attribute);
#ifndef ESGI_GLES_20
	else
		glDisableClientState(GL_VERTEX_ARRAY);
#endif

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
