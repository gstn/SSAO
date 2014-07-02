#include "../../EsgiGL/EsgiGL.h"
#include "../../EsgiGL/EsgiShader.h"
#include "../../EsgiGL/Common/EsgiTGA.h"
#include "../../EsgiGL/Common/matrix.h"
#include "ssaoUtils.hpp"

#include <stdlib.h>
#include "SceneLoader.hpp"
#include "Camera.hpp"
#include "../../ESGIGL/Libs/GLUT/glut.h"

#define WIDTH 1024.0f
#define HEIGHT 600.0f
#define NEAR 0.1f
#define FAR 500.f

struct Light {
    vec3 position;
    vec3 intensities; //a.k.a. the color of the light
};

/***************************************************************************
 * Program code
 ***************************************************************************/

SceneLoader * loader;
Camera camera;
Mesh * quad;

EsgiShader shader;
EsgiShader blurShader;
EsgiShader quadShader;
EsgiShader ssao2DShader;
EsgiShader ssaoShader;
EsgiShader blendShader;
EsgiShader textureShader;

mat4 viewMatrix;
Light gLight;

float elapsedTime;

unsigned int FBO;
unsigned int renderTexture, ssaoTexture1, ssaoTexture2, depthTexture, normalTexture, positionTexture, sceneTexture, noiseTexture;

//render controls
enum Render {SSAO, PHONG, DEPTH, NORMALS, TEXTURE, POSITIONS, NOISE};
int render = SSAO;
bool blend = true;
bool renderSSAO = false;
bool ssao2D = true;
int blurIntensity = 0;
float ssaoRadius = 10.0f;

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

	//Projection matrix
	mat4 projectionMatrix = camera.getProjectionMatrix();
	//View Matrix
	viewMatrix.Identity();
	viewMatrix.T.set(0.f, -0.5f, -2.f, 1.f);
	//camera.rotate(vec3(0.f, 0.f, 0.8f));
	viewMatrix = camera.getViewMatrix();

	//ModelView Matrix
	// tourne autour de l'axe Y du monde
	mat4 world;
	world.T.set(0.f, 0.f, 0.f, 1.f);

	mat4 modelMatrix;
	modelMatrix.Identity();
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrix.I.x);
	glUniform3fv(glGetUniformLocation(programObject, "u_LightPosition"), 1, &gLight.position.x);
	glUniform3fv(glGetUniformLocation(programObject, "u_LightIntensities"), 1, &gLight.intensities.x);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	GLenum mrt[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, mrt);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, positionTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	loader->draw(programObject);

	glDrawBuffer(GL_BACK);

	if (render == SSAO) {
		//ssao
		glDisable(GL_DEPTH_TEST);

		//ssao 2d
		if(ssao2D) {
			programObject = ssao2DShader.GetProgram();
			glUseProgram(programObject);

			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTexture1, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depthTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, noiseTexture);
			glUniform1f(glGetUniformLocation(programObject, "u_radius"), ssaoRadius);
		}
		//ssao 3d
		else {
			programObject = ssaoShader.GetProgram();
			glUseProgram(programObject);

			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTexture1, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depthTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalTexture);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, positionTexture);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, noiseTexture);

			glUniform1f(glGetUniformLocation(programObject, "u_radius"), ssaoRadius);
			glUniformMatrix4fv(glGetUniformLocation(programObject, "u_persMatrix"), 1, 0, &projectionMatrix.I.x);
		}

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
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		programObject = textureShader.GetProgram();
		glUseProgram(programObject);
		glActiveTexture(GL_TEXTURE0);

		if (render == DEPTH) {
			glBindTexture(GL_TEXTURE_2D, depthTexture);
		} else if (render == PHONG) {
			glBindTexture(GL_TEXTURE_2D, renderTexture);
		} else if (render == TEXTURE) {
			glBindTexture(GL_TEXTURE_2D, sceneTexture);
		} else if (render == NORMALS) {
			glBindTexture(GL_TEXTURE_2D, normalTexture);
		} else if (render == POSITIONS) {
			glBindTexture(GL_TEXTURE_2D, positionTexture);
		} else if(render == NOISE) {
			glBindTexture(GL_TEXTURE_2D, noiseTexture);
		}

		quad->draw(programObject);
	}
}

bool Setup()
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	viewMatrix.Identity();
	viewMatrix.T.set(0.f, -1.5f, 1.f, 1.f);
	camera.setViewMatrix(viewMatrix);
	camera.setProjectionMatrix(esgiPerspective(45.f, WIDTH/HEIGHT, NEAR, FAR));
	vec4 frustum = esgiPerspectiveFrustum(45.f, WIDTH/HEIGHT, NEAR, FAR);

	//init kernel and noise
	initKernel();
	initNoiseTexture();

	//basic diffuse-ambient-spec shader
	shader.LoadVertexShader("basic.vert");
	shader.LoadFragmentShader("basic.frag");
	shader.Create();
	
	//blur shader
	blurShader.LoadVertexShader("basic.vert");
	blurShader.LoadFragmentShader("blur4.frag");
	blurShader.Create();

	//quadShader
	quadShader.LoadVertexShader("basic.vert");
	quadShader.LoadFragmentShader("quadRender.frag");
	quadShader.Create();
	
	//ssao2D shader
	ssao2DShader.LoadVertexShader("basic.vert");
	ssao2DShader.LoadFragmentShader("ssao2D.frag");
	ssao2DShader.Create();

	//ssao shader
	ssaoShader.LoadVertexShader("basic.vert");
	ssaoShader.LoadFragmentShader("ssao.frag");
	ssaoShader.Create();

	//blend shader
	blendShader.LoadVertexShader("basic.vert");
	blendShader.LoadFragmentShader("blend.frag");
	blendShader.Create();

	//texture shader
	textureShader.LoadVertexShader("basic.vert");
	textureShader.LoadFragmentShader("renderTexture.frag");
	textureShader.Create();
	
	//load the scene
	loader = new SceneLoader("obj/courtyard/courtyard.obj");
	//loader = new SceneLoader("obj/suzanne/suzanne.obj");

	gLight.position = vec3(0.0,-3.0,-9.0);
	gLight.intensities = vec3(0.8, 0.8, 0.8);

	renderTexture = createTexture(WIDTH, HEIGHT);
	ssaoTexture1 = createTexture(WIDTH, HEIGHT);
	ssaoTexture2 = createTexture(WIDTH, HEIGHT);
	depthTexture = createTexture(WIDTH, HEIGHT, true);
	normalTexture = createTexture(WIDTH, HEIGHT);
	//positionTexture = createTexture(WIDTH, HEIGHT);
	noiseTexture = createTexture(4, 4, false);

	glGenTextures(1, &positionTexture);
	glBindTexture(GL_TEXTURE_2D, positionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4, 4, 0, GL_BGRA, GL_UNSIGNED_BYTE, &noiseTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	
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
		tmp.position = vec3(1.0f,-1.0,0.0);
		tmp.u=1;
		tmp.v=0;
		vertices.push_back(tmp);
		//4.
		tmp.position = vec3(1.0f,1.0,0.0);
		tmp.u=1;
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

	world.Identity();
	viewMatrix.Identity();
	viewMatrix.T.set(0.f, 0.f, 0.f, -1.f);
	modelMatrix = esgiMultiplyMatrix(viewMatrix, world);
	mat4 projectionMatrixP = esgiOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

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
	
	programObject = ssao2DShader.GetProgram();
	glUseProgram(programObject);
	glUniform1i(glGetUniformLocation(programObject, "u_depthTexture"), 0);
	glUniform3f(glGetUniformLocation(programObject, "pixelSize"), 1.0 / WIDTH, 1.0 / HEIGHT, 0);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);
	glUniform1i(glGetUniformLocation(programObject, "u_kernelSize"), kernelSize);
	glUniform3fv(glGetUniformLocation(programObject, "u_kernel"), kernelSize, kernel);
	
	programObject = ssaoShader.GetProgram();
	glUseProgram(programObject);
	glUniform1i(glGetUniformLocation(programObject, "u_depthTexture"), 0);
	glUniform1i(glGetUniformLocation(programObject, "u_normalTexture"), 1);
	glUniform1i(glGetUniformLocation(programObject, "u_positionTexture"), 2);
	glUniform1i(glGetUniformLocation(programObject, "u_noiseTexture"), 3);
	glUniform3f(glGetUniformLocation(programObject, "pixelSize"), 1.0 / WIDTH, 1.0 / HEIGHT, 0);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);
	glUniform1f(glGetAttribLocation(programObject, "u_screenW"), WIDTH);
	glUniform1f(glGetAttribLocation(programObject, "u_screenH"), HEIGHT);
	glUniform1i(glGetUniformLocation(programObject, "u_kernelSize"), kernelSize);
	glUniform3fv(glGetUniformLocation(programObject, "u_kernel"), kernelSize, kernel);
	glUniform1f(glGetUniformLocation(programObject, "u_near"), NEAR);
	glUniform1f(glGetUniformLocation(programObject, "u_far"), FAR);
	glUniform4f(glGetUniformLocation(programObject, "u_frustum"), frustum.x, frustum.y, frustum.z, frustum.w);

	programObject = blendShader.GetProgram();
	glUseProgram(programObject);
	glUniform1i(glGetUniformLocation(programObject, "u_renderTexture"), 0);
	glUniform1i(glGetUniformLocation(programObject, "u_ssaoTexture"), 1);
	glUniform3f(glGetUniformLocation(programObject, "pixelSize"), 1.0 / WIDTH, 1.0 / HEIGHT, 0);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);

	programObject = textureShader.GetProgram();
	glUseProgram(programObject);
	glUniform1i(glGetUniformLocation(programObject, "u_texture"), 0);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ModelMatrix"), 1, 0, &modelMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ViewMatrix"), 1, 0, &viewMatrix.I.x);
	glUniformMatrix4fv(glGetUniformLocation(programObject, "u_ProjectionMatrix"), 1, 0, &projectionMatrixP.I.x);

	std::cout << "_______________________________\n";
	std::cout << "Instructions\n\n";
	std::cout << "navigation:\n";
	std::cout << "----------\n";
	std::cout << "  z\n";
	std::cout << "q s d: move\n\n";
	std::cout << "p: move up\n";
	std::cout << "m: move down\n";
	std::cout << "right-click: toggle camera pitch/yaw mode\n";
	std::cout << "mouse move: camera pitch/yaw (if enabled)\n\n";
	std::cout << "render:\n";
	std::cout << "------\n";
	std::cout << "b: toogle blend phong and ssao (only in SSAO render mode)\n";
	std::cout << "S: SSAO rendering\n";
	std::cout << "P: Phong rendering\n";
	std::cout << "D: fragment depth\n";
	std::cout << "N: fragment normal\n";
	std::cout << "V: fragment position\n";
	std::cout << "W: toggle SSAO/SSAO2D\n";
	std::cout << "T: next scene texture\n";
	
	return true;
}

void Update(float deltaTime)
{
	elapsedTime = deltaTime;
	camera.update(deltaTime);
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

bool look = false;

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
	else if (key == 'S') {
		render = SSAO;
	}
	else if (key == 'P') {
		render = PHONG;
	}
	else if (key == 'D') {
		render = DEPTH;
	}
	else if (key == 'T') {
		render = TEXTURE;
		sceneTexture = loader->getNextTexture();
	}
	else if (key == 'N') {
		render = NORMALS;
	}
	else if (key == 'V') {
		render = POSITIONS;
	}
	else if (key == 'b') {
		blend = !blend;
	}
	else if (key == 'n') {
		render = NOISE;
	}
	else if (key == 'W') {
		ssao2D = !ssao2D;
	}
	else if (key == '+') {
		++blurIntensity;
		std::cout << "Blur level: " << blurIntensity << std::endl;
	}
	else if (key == '-' && blurIntensity != 0) {
		--blurIntensity;
		std::cout << "Blur level: " << blurIntensity << std::endl;
	}
	else if (key == 'p') {
		camera.moveUp(true);
	}
	else if (key == 'm') {
		camera.moveDown(true);
	}
	//space
	else if(key == 32) {
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
	else if (key == 'p') {
		camera.moveUp(false);
	}
	else if (key == 'm') {
		camera.moveDown(false);
	}
	if(key == 27) {
		exit(0);
	}
}

void mouseFunc(int button, int state, int x, int y) {

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		look = !look;
		camera.freezeRotation();
	}
}

void passiveMotionFunc(int x, int y) {
	if (look) {
		float yaw = ( x / WIDTH - .5 ) * 20;
		float pitch = ( y / HEIGHT - .5 ) * 20;

		float yawSign = yaw > 0 ? 1.f : -1.f;
		float pitchSign = pitch > 0 ? 1.f : -1.f;

		yaw *= yawSign * yaw;
		pitch *= pitchSign * pitch;
		camera.setRotation(pitch, yaw);
	} else {
		ssaoRadius = x / WIDTH * 20;
		//std::cout << ssaoRadius << std::endl;
	}
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

	glutPassiveMotionFunc(&passiveMotionFunc);
	glutKeyboardFunc(&keyFunc);
	glutKeyboardUpFunc(&keyUpFunc);
	glutMouseFunc(&mouseFunc);
	glutIgnoreKeyRepeat(1);
    
	esgi.CleanFunc(&Clean);
	esgi.MainLoop();
    
    return 0;
}
