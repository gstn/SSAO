#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string.h>
#include <SDL/SDL.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <GL/glext.h> 

#include "objloader.h"

float angle=0.0;

int cube;
objloader obj;	//create an instance of the objloader
void init()
{
	glClearColor(0.9,0.9,0.9,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,640.0/480.0,1.0,500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH_POINT_SIZE_RANGE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float col[]={1.0,1.0,1.0,1.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,col);
	float spe[]={1.0,0.0,0.0,1.0};
	glLightfv(GL_LIGHT0,GL_SPECULAR,spe);
	glTranslatef(1.0,0.0,-10.0);
	float pos[]={1.0,1.0,-1,1.0};
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION,pos);

	//cube=obj.load("laser.obj");	//load it
	cube=obj.load("chicken1.obj");	//load it

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	glTranslatef(0.0,-10.0,-100.0);
	glRotatef(angle,0,1,0);
	glCallList(cube);	//and display it
}




int main(int argc, char* argv[])
{

	/*String mode = "cam";

	if(mode == "cam")
	{
		VideoCapture cap(0); // open the video camera no. 0

		if(!cap.isOpened())  // if not success, exit program
		{
			cout << "Cannot open the video file" << endl;
			return -1;
		}

		double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
		double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

		cout << "Frame size : " << dWidth << " x " << dHeight << endl;

		namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

		while(1)
		{
			Mat frame;

			bool bSuccess = cap.read(frame); // read a new frame from video

			if (!bSuccess) //if not success, break loop
			{
				cout << "Cannot read a frame from video file" << endl;
				break;
			}

			imshow("MyVideo", frame); //show the frame in "MyVideo" window

			if(waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			{
				cout << "esc key is pressed by user" << endl;
				break; 
			}
		}
	}
	else
	{

		atexit(OnShutdown);
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);	
		glutInitContextVersion (3, 3);
		glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
		glutInitWindowSize(screen_width, screen_height);
		glutCreateWindow("Tutorial 4 - OpenGL 3.3");
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)	{
			cerr<<"Error: "<<glewGetErrorString(err)<<endl;
		} else {
			if (GLEW_VERSION_3_3)
			{
				cout<<"Driver supports OpenGL 3.3\nDetails:"<<endl;
			}
		}
		cout<<"Using GLEW "<<glewGetString(GLEW_VERSION)<<endl;
		cout<<"Vendor: "<<glGetString (GL_VENDOR)<<endl;
		cout<<"Renderer: "<<glGetString (GL_RENDERER)<<endl;
		cout<<"Version: "<<glGetString (GL_VERSION)<<endl;
		cout<<"GLSL: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<endl;
		InitGL();
		glutDisplayFunc(OnRender);
		glutReshapeFunc(OnResize);
		glutKeyboardFunc(OnKey);
		glutSpecialFunc(OnSpecialKey);
		glutIdleFunc(OnIdle);
		glutMainLoop();

	}
    return 0;*/


	SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Surface* screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE|SDL_OPENGL);
        bool running=true;
        Uint32 start;
        SDL_Event event;
        init();
        while(running)
        {
                start=SDL_GetTicks();
                while(SDL_PollEvent(&event))
                {
                        switch(event.type)
                        {
                                case SDL_QUIT:
                                        running=false;
                                        break;
                        }
                }
                display();
                SDL_GL_SwapBuffers();
                angle+=0.5;
                if(angle>360)
                        angle-=360;
                if(1000/30>(SDL_GetTicks()-start))
                        SDL_Delay(1000/30-(SDL_GetTicks()-start));
        }
        SDL_Quit();
        return 0;      
}