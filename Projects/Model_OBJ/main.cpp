#include "../../EsgiGL/EsgiGL.h"
#include "../../EsgiGL/EsgiShader.h"
#include "../../EsgiGL/Common/EsgiTGA.h"
#include "../../EsgiGL/Common/matrix.h"

#include <stdlib.h>
#include "Model_OBJ.hpp"


struct Light {
    vec3 position;
    vec3 intensities; //a.k.a. the color of the light
};

/***************************************************************************
 * Program code
 ***************************************************************************/

Model_OBJ obj;
EsgiShader shader;
mat4 projectionMatrix;
mat4 projectionMatrixP;
mat4 viewMatrix;
Light gLight;

float g_rotation;
glutWindow win;
double a=0;
int MatSpec [4] = {1,1,1,1};
int LightPos[4] = {0,0,3,1};




void Draw()
{
	glClearColor(0.95f, 0.95f, 0.95f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	GLuint programObject = shader.GetProgram();
	glUseProgram(programObject);	

	//view Matrix
	viewMatrix.Identity();
	viewMatrix.T.set(0.f, 0.f, -2.f, 1.f);

	GLint viewUniform = glGetUniformLocation(programObject, "u_ViewMatrix");
	glUniformMatrix4fv(viewUniform, 1, 0, &viewMatrix.I.x);

	//Projection Matrix
	projectionMatrix = esgiOrtho(0.f, 800.f, 0.f, 600.f, 0.f, 1.f);	
	projectionMatrixP = esgiPerspective(45.f, 4.f/3.f, 0.1f, 500.f);

	GLint projectionUniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, 0, &projectionMatrixP.I.x);

	//ModelView Matrix
	// tourne autour de l'axe Y du monde
	mat4 world = esgiRotateY(obj.transform.orientation);
	world.T.set(0.f, 0.f, 0.f, 1.f);

    GLint modelUniform = glGetUniformLocation(programObject, "u_ModelMatrix");
	mat4 modelMatrix = esgiMultiplyMatrix(viewMatrix, world);
	glUniformMatrix4fv(modelUniform, 1, 0, &modelMatrix.I.x);
	
	//Light
	GLint lightPositionUniform = glGetUniformLocation(programObject, "u_LightPosition");
	glUniform3fv(lightPositionUniform, 1, &gLight.position.x);

	GLint lightIntensitiesUniform = glGetUniformLocation(programObject, "u_LightIntensities");
	glUniform3fv(lightIntensitiesUniform, 1, &gLight.intensities.x);

	


	obj.Draw(programObject);


	

}
bool Setup()
{
	// charge les vertex/fragment shaders
	shader.LoadVertexShader("basic.vert");
	shader.LoadFragmentShader("basic.frag");

	shader.Create();

	gLight.position = vec3(0.0,0.0,3.0);
	gLight.intensities = vec3(0.0,1.0,1.0);
	obj.transform.position = vec3(0.f,0.f,0.0f);
	obj.transform.rotationSpeed = 90.f;
	obj.transform.orientation = 0.f;

	//obj.Load("obj/plane.obj");
	//obj.Load("obj/cube.obj");
	//obj.Load("obj/monkey.obj");
	obj.Load("obj/suzanne.obj");
	return true;
}

void Update(float elapsedTime)
{
	obj.Process(elapsedTime);
}
void Clean()
{
	shader.Destroy();
}


int main(int argc, char *argv[])
{
	EsgiGLApplication esgi;
    
	esgi.InitWindowPosition(0, 0);
	esgi.InitWindowSize(800, 600);
	esgi.InitDisplayMode(ESGI_WINDOW_RGBA|ESGI_WINDOW_DOUBLEBUFFER);
	esgi.CreateWindow("[TEST] Model OBJ ", ESGI_WINDOW_CENTERED);
	
    esgi.IdleFunc(&Update);
	esgi.DisplayFunc(&Draw);
    esgi.InitFunc(&Setup);

    
	esgi.CleanFunc(&Clean);
	esgi.MainLoop();
    
    return 0;
}
