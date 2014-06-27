#include "../../EsgiGL/EsgiGL.h"
#include "../../EsgiGL/EsgiShader.h"
#include "../../EsgiGL/Common/EsgiTGA.h"
#include "../../EsgiGL/Common/matrix.h"
#include "ssaoUtils.hpp"

#include <stdlib.h>
#include "Model_OBJ.hpp"
#include "SceneLoader.hpp"
#include "Camera.hpp"
#include "../../ESGIGL/Libs/GLUT/glut.h"

#define WIDTH 1024.0f
#define HEIGHT 768.0f

struct Light {
    vec3 position;
    vec3 intensities; //a.k.a. the color of the light
};

/***************************************************************************
 * Program code
 ***************************************************************************/

Model_OBJ obj;
SceneLoader * loader;
Camera camera;
Mesh * quad;

EsgiShader shader;
EsgiShader blurShader;
EsgiShader quadShader;
EsgiShader ssaoShader;
EsgiShader blendShader;

mat4 projectionMatrix;
mat4 projectionMatrixP;
mat4 viewMatrix;
Light gLight;

//render controls
bool blend = false;

float g_rotation;
glutWindow win;
double a=0;
int MatSpec [4] = {1,1,1,1};
int LightPos[4] = {0,0,3,1};

unsigned int FBO;
unsigned int renderTexture, ssaoTexture1, ssaoTexture2, depthTexture;

int blurIntensity = 0;


unsigned int createTexture(int w,int h,bool isDepth = false)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, isDepth ? GL_DEPTH_COMPONENT : GL_RGBA8, w, h, 0, isDepth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	int i = glGetError();
	if(i != 0) {
		std::cout << "Error happened while loading the texture: " << i << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

void Draw()
{
	//Render to color and depth buffer--------------
	glClearColor(0.95f, 0.95f, 0.95f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	GLuint programObject = shader.GetProgram();
	glUseProgram(programObject);

	//View Matrix
	viewMatrix.Identity();
	viewMatrix.T.set(0.f, -0.5f, -2.f, 1.f);
	//camera.rotate(vec3(0.f, 0.f, 0.8f));
	viewMatrix = camera.getViewMatrix();

	//Projection Matrix
	//projectionMatrix = esgiOrtho(0.f, WIDTH, 0.f, HEIGHT, 0.f, 1.f);	
	projectionMatrixP = esgiPerspective(45.f, 4.f/3.f, 0.1f, 500.f);

	//ModelView Matrix
	// tourne autour de l'axe Y du monde
	mat4 world = esgiRotateY(obj.transform.orientation*0);
	world.T.set(0.f, 0.f, 0.f, 1.f);

	mat4 modelMatrix;// = esgiMultiplyMatrix(viewMatrix, world);
	modelMatrix.Identity();
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);

	glUniform3fv(glGetUniformLocation(programObject, "u_LightPosition"), 1, &gLight.position.x);
	glUniform3fv(glGetUniformLocation(programObject, "u_LightIntensities"), 1, &gLight.intensities.x);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loader->draw(programObject);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	loader->draw(programObject);

	//ssao
	glDisable(GL_DEPTH_TEST);
	programObject = ssaoShader.GetProgram();
	glUseProgram(programObject);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTexture1, 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	
	quad->draw(programObject);

	//Blur
	programObject = blurShader.GetProgram();
	glUseProgram(programObject);

	glActiveTexture(GL_TEXTURE0);
	bool ping = true;
	
	for (int i = 0; i < blurIntensity; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glActiveTexture(GL_TEXTURE0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ping ? ssaoTexture2 : ssaoTexture1, 0);
		glBindTexture(GL_TEXTURE_2D, ping ? ssaoTexture1 : ssaoTexture2);

		glUniform1i(glGetUniformLocation(programObject, "isVertical"), (int)ping);

		quad->draw(programObject);

		ping = !ping;
	}

	//blend
	if(blend) {
		programObject = blendShader.GetProgram();
		glUseProgram(programObject);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ping ? ssaoTexture1 : ssaoTexture2);
	
		quad->draw(programObject);

	} else {
		//Render to quad----------------------------------
		programObject = quadShader.GetProgram();
		glUseProgram(programObject);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ping ? ssaoTexture1 : ssaoTexture2);

		quad->draw(programObject);
	}
}

bool Setup()
{
	//load the scene
	loader = new SceneLoader("obj/courtyard/courtyard.obj");

	mat4 viewMatrix;
	viewMatrix.Identity();
	viewMatrix.T.set(0.f, -1.5f, 1.f, 1.f);
	camera.setViewMatrix(viewMatrix);

	//init kernel and noise
	initKernel();
	initNoiseTexture();

	//basic diffuse-ambient-spec shader
	shader.LoadVertexShader("basic.vert");
	shader.LoadFragmentShader("basic.frag");
	shader.Create();
	
	//blur shader
	blurShader.LoadVertexShader("basic.vert");
	blurShader.LoadFragmentShader("blur.frag");
	blurShader.Create();

	//quadShader
	quadShader.LoadVertexShader("basic.vert");
	quadShader.LoadFragmentShader("quadRender.frag");
	quadShader.Create();

	//ssao shader
	ssaoShader.LoadVertexShader("basic.vert");
	ssaoShader.LoadFragmentShader("ssao.frag");
	ssaoShader.Create();

	//blend shader
	blendShader.LoadVertexShader("basic.vert");
	blendShader.LoadFragmentShader("blend.frag");
	blendShader.Create();
	
	gLight.position = vec3(0.0,-3.0,-9.0);
	gLight.intensities = vec3(0.0,1.0,1.0);
	obj.transform.position = vec3(0.f,0.f,0.0f);
	obj.transform.rotationSpeed = 90.f;
	obj.transform.orientation = 0.f;

	renderTexture = createTexture(WIDTH, HEIGHT);
	ssaoTexture1 = createTexture(WIDTH, HEIGHT);
	ssaoTexture2 = createTexture(WIDTH, HEIGHT);
	depthTexture = createTexture(WIDTH, HEIGHT, true);

	glGenFramebuffers(1,&FBO);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	int i = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(i!=GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer is not OK, status=" << i << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//create the quad here
	{
		std::vector<unsigned short> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		//1.
		tmp.position = vec3(-1.0,1.0,0.0);
		tmp.u=0;
		tmp.v=1;
		vertices.push_back(tmp);
		//2.
		tmp.position = vec3(-1.0,-1.0,0.0);
		tmp.u=0;
		tmp.v=0;
		vertices.push_back(tmp);
		//3.
		tmp.position = vec3(0.0f,-1.0,0.0);
		tmp.u=0.5;
		tmp.v=0;
		vertices.push_back(tmp);
		//4.
		tmp.position = vec3(0.0f,1.0,0.0);
		tmp.u=0.5;
		tmp.v=1;
		vertices.push_back(tmp);
		
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);		
		
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
		quad = new Mesh(&vertices, &indices);
	}

	//init quad position
	mat4 world;
	mat4 modelMatrix;
	mat4 projectionMatrix;

	world.Identity();
	viewMatrix.Identity();
	viewMatrix.T.set(0.f, 0.f, 0.f, -1.f);
	modelMatrix = esgiMultiplyMatrix(viewMatrix, world);
	projectionMatrixP = esgiOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	GLuint programObject = blurShader.GetProgram();
	glUseProgram(programObject);
	glUniform1i(glGetUniformLocation(programObject, "texture"), 0);
	glUniform3f(glGetUniformLocation(programObject, "pixelSize"), 1.0 / WIDTH, 1.0 / HEIGHT, 0);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);

	programObject = quadShader.GetProgram();
	glUseProgram(programObject);
	glUniform1i(glGetUniformLocation(programObject, "texture"), 0);
	glUniform3f(glGetUniformLocation(programObject, "pixelSize"), 1.0 / WIDTH, 1.0 / HEIGHT, 0);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);

	programObject = ssaoShader.GetProgram();
	glUseProgram(programObject);
	glUniform1i(glGetUniformLocation(programObject, "u_depthTexture"), 0);
	glUniform3f(glGetUniformLocation(programObject, "pixelSize"), 1.0 / WIDTH, 1.0 / HEIGHT, 0);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);
	glUniform1i(glGetUniformLocation(programObject, "u_kernelSize"), kernelSize);
	glUniform3fv(glGetUniformLocation(programObject, "u_kernel"), kernelSize, kernel);

	programObject = blendShader.GetProgram();
	glUseProgram(programObject);
	glUniform1i(glGetUniformLocation(programObject, "u_renderTexture"), 0);
	glUniform1i(glGetUniformLocation(programObject, "u_ssaoTexture"), 1);
	glUniform3f(glGetUniformLocation(programObject, "pixelSize"), 1.0 / WIDTH, 1.0 / HEIGHT, 0);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);
	
	//obj.Load("obj/courtyard++.obj");
	return true;
}

void Update(float elapsedTime)
{
	//obj.Process(elapsedTime);
	camera.update();
}

void Clean()
{
	shader.Destroy();
	blurShader.Destroy();
	quadShader.Destroy();
	ssaoShader.Destroy();
	delete quad;
	delete loader;
}

void keyFunc(unsigned char key, int x, int y) {
	//Z
	if (key == 122) {
		camera.moveForward(true);
	}
	//S
	else if (key == 115) {
		camera.moveBackwards(true);
	}
	//Q
	else if (key == 113) {
		camera.moveLeft(true);
	}
	//D
	else if (key == 100) {
		camera.moveRight(true);
	}
	else if (key == 102) {
		camera.freezeRotation();
	}
	else if (key == 'b') {
		blend = !blend;
	}
	else if (key == '+') {
		++blurIntensity;
		std::cout << "Blur level: " << blurIntensity << std::endl;
	}
	else if (key == '-' && blurIntensity != 0) {
		--blurIntensity;
		std::cout << "Blur level: " << blurIntensity << std::endl;
	}
	//space
	else if(key == 32) {
		std::cout << "jump";
		camera.jump();
	}
}

void keyUpFunc(unsigned char key, int x, int y) {
	//Z
	if (key == 122) {
		camera.moveForward(false);
	}
	//S
	else if (key == 115) {
		camera.moveBackwards(false);
	}
	//Q
	else if (key == 113) {
		camera.moveLeft(false);
	}
	//D
	else if (key == 100) {
		camera.moveRight(false);
	}

	if(key == 27) {
		exit(0);
	}
}

void mouseFunc(int x, int y) {

	float yaw = ( x / WIDTH - .5 ) * 3;
	float pitch = ( y / HEIGHT - .5 ) * 3;

	float yawSign = yaw > 0 ? 1.f : -1.f;
	float pitchSign = pitch > 0 ? 1.f : -1.f;

	yaw *= yawSign * yaw;
	pitch *= pitchSign * pitch;
	camera.setRotation(pitch, yaw);
}

int main(int argc, char *argv[])
{
	EsgiGLApplication esgi;
    
	esgi.InitWindowPosition(0, 0);
	esgi.InitWindowSize(WIDTH, HEIGHT);
	esgi.InitDisplayMode(ESGI_WINDOW_RGBA|ESGI_WINDOW_DOUBLEBUFFER);
	esgi.CreateWindow("SSAO | 4A IJV - Augustin GARDETTE - Jérémie FERREIRA", ESGI_WINDOW_CENTERED);
	//esgi.MouseFunction( &mouseFunc );

    esgi.IdleFunc(&Update);
	esgi.DisplayFunc(&Draw);
    esgi.InitFunc(&Setup);

	glutPassiveMotionFunc(&mouseFunc);
	glutKeyboardFunc(&keyFunc);
	glutKeyboardUpFunc(&keyUpFunc);
	glutIgnoreKeyRepeat(1);
    
	esgi.CleanFunc(&Clean);
	esgi.MainLoop();
    
    return 0;
}
