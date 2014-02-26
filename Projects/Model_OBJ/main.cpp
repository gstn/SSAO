#include "../../EsgiGL/EsgiGL.h"
#include "../../EsgiGL/EsgiShader.h"
#include "../../EsgiGL/Common/EsgiTGA.h"
#include "../../EsgiGL/Common/matrix.h"

#include <stdlib.h>
#include "Model_OBJ.hpp"

/***************************************************************************
 * Program code
 ***************************************************************************/

Model_OBJ obj;
EsgiShader shader;
float g_rotation;
glutWindow win;
double a=0;
int MatSpec [4] = {1,1,1,1};
int LightPos[4] = {1,0,13,1};

GLint position_attribute = 0;
GLint normale_attribute = 0;

void Draw()
{
	glClearColor( 0.0f, 0.1f, 0.0f, 0.5f );
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glEnable(GL_DEPTH_TEST); 	// Active le test de profondeur
	glEnable(GL_LIGHTING); 	// Active l'éclairage
	glEnable(GL_LIGHT0);

	bool useShader = true;
	if(!useShader){
		// efface le color buffer
		
	
		obj.Draw();
	}
	else
	{
		GLuint programObject = shader.GetProgram();
		glUseProgram(programObject);
		position_attribute = glGetAttribLocation(programObject, "a_Position");

		glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_TRUE, 0, obj.Faces_Triangles);
		glEnableVertexAttribArray(position_attribute);

		glDrawArrays(GL_TRIANGLES, 0, obj.TotalConnectedTriangles);

		glDisableVertexAttribArray(position_attribute);
	}

}

bool Setup()
{
	// charge les vertex/fragment shaders
	shader.LoadVertexShader("basic.vert");
	shader.LoadFragmentShader("basic.frag");

	shader.Create();

	obj.Load("obj/suzanne.obj");

	return true;
}
void initialize ()
{
    glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat) win.width / win.height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
    glMatrixMode(GL_MODELVIEW);
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.1f, 0.0f, 0.5f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Update(float elapsedTime)
{
	//sprite.Process(elapsedTime);
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
