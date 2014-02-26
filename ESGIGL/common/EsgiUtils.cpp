// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

// --- Description -----------------------------------------------------------
//

// --- Includes --------------------------------------------------------------

#include "EsgiUtils.h"
#include "../EsgiGL.h"
#include "matrix.h"

// --- Functions -------------------------------------------------------------

#define ESGI_ATTRIB_POSITION	0

static GLuint basicVertexShader = 0;
static GLuint basicFragmentShader = 0;
static GLuint basicProgramObject = 0;

///
// Cree un shader object, charge le code source du shader et le compile
//
static GLuint LoadShader(GLenum type, const char *shaderSrc)
{
	// Cree le shader object
	GLuint shader = glCreateShader(type);
	if (shader == 0) {
		return 0;
	}

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile le shader
	glCompileShader(shader);

	return shader;
}

//
// Initialise les shader & program object
//
bool esgiUtilsSetupShaders()
{	
	char vShaderStr[] =  
		"attribute vec4 a_Position;			\n"
		"uniform float u_PointSize;			\n"
		"uniform mat4 u_ModelViewMatrix;	\n"
		"uniform mat4 u_ProjectionMatrix;	\n"
		"varying vec4 v_VertexColor;		\n"
		"void main()						\n"
		"{									\n"
		"   v_VertexColor = a_Position;		\n"
		"   gl_PointSize = u_PointSize;		\n"
		"   gl_Position = u_ProjectionMatrix * u_ModelViewMatrix * a_Position;	\n"
		"}									\n";

	char fShaderStr[] =  
		"#ifdef GL_ES						\n"
		"varying lowp vec4 v_VertexColor;	\n"
		"#else								\n"
		"varying vec4 v_VertexColor;		\n"
		"#endif								\n"
		"void main()						\n"
		"{									\n"		
		"  gl_FragColor = v_VertexColor;	\n"
		"}									\n";

	GLint linked;

	// charge les vertex/fragment shaders
	basicVertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
	basicFragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

	// Cree le program object
	basicProgramObject = glCreateProgram();

	if (basicProgramObject == 0)
		return 0;

	glAttachShader(basicProgramObject, basicVertexShader);
	glAttachShader(basicProgramObject, basicFragmentShader);

	glBindAttribLocation(basicProgramObject, ESGI_ATTRIB_POSITION, "a_Position");

	// Liage des shaders dans le programme
	glLinkProgram(basicProgramObject);

	return true;
}

//
// Libere la memoire occupee par les shader programs et le shader object
//
void esgiUtilsCleanShaders()
{
	if (basicFragmentShader)
		glDeleteShader(basicFragmentShader);
	if (basicVertexShader)
		glDeleteShader(basicVertexShader);
	if (basicProgramObject)
		glDeleteProgram(basicProgramObject);
}

void esgiUtilsDrawAxes(const mat4 &modelViewMatrix, const mat4 &projection, float size)
{
	static float axes[] = { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 
							0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 
							0.f, 0.f, 0.f, 0.f, 0.f, 1.f };

	glUseProgram(basicProgramObject);
	GLint pointsizeUniform = glGetUniformLocation(basicProgramObject, "u_PointSize");
	glUniform1f(pointsizeUniform, size);
	GLint modelviewUniform = glGetUniformLocation(basicProgramObject, "u_ModelViewMatrix");
	glUniformMatrix4fv(modelviewUniform, 1, 0, &modelViewMatrix.I.x);
	GLint projectionUniform = glGetUniformLocation(basicProgramObject, "u_ProjectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, 0, &projection.I.x);

	glEnableVertexAttribArray(ESGI_ATTRIB_POSITION);
	glVertexAttribPointer(ESGI_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, axes);
	glDrawArrays(GL_LINES, 0, 6);
	glDisableVertexAttribArray(ESGI_ATTRIB_POSITION);
	
	glUseProgram(0);
}

