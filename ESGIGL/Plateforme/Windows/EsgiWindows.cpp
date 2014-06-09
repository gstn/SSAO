// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#if defined(_WIN32) && !defined(ESGI_GLES_20)

// --- Includes --------------------------------------------------------------

#include "../EsgiGL.h"
#include <cstdio>
#include <cstdlib>

// --- Globales --------------------------------------------------------------

// --- Functions -------------------------------------------------------------

// log ---

void ESGI_PRINT(const char *format, ...)
{
	char buffer[512];
	
	// Formatage du message dans une chaine de caractere
	va_list vaParams;
	va_start(vaParams, format);
	vsnprintf(buffer, 512, format, vaParams);
	va_end(vaParams);	
	
	::OutputDebugString(buffer);
	printf("%s", buffer);
}

void ESGI_PRINTSCREEN(int x, int y, const char *format, ...)
{
	char buffer[512];
	
#ifdef ESGI_GLUT
	// Formatage du message dans une chaine de caractere
	va_list vaParams;
	va_start(vaParams, format);
	vsnprintf(buffer, 512, format, vaParams);
	va_end(vaParams);	
	
	//::OutputDebugString(buffer);

	int len;
	glRasterPos2i(x, y);
	len = (int)strlen(buffer);

	for (int index = 0; index < len; ++index)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buffer[index]);
	}
#endif
}

// --- Initialisation --------------------------------------------------------

void EsgiContext::SetContextVersion(unsigned int major, unsigned int minor) { majorVersion = major; minorVersion = minor; }
void EsgiContext::SetContextProfile(unsigned int profile) 
{ 
	profileFlags |= (profile & ESGI_PROFILE_CORE ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB); 
}
void EsgiContext::SetContextFlags(unsigned int flags) 
{ 
	contextFlags |= (flags & ESGI_CONTEXT_FORWARD ? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB : 0);
	contextFlags |= (flags & ESGI_CONTEXT_DEBUG ? WGL_CONTEXT_DEBUG_BIT_ARB : 0);
}

bool EsgiContext::CreateContext()
{
#ifndef ESGI_GLUT
	int pixelFormat = 0;

	device = ::GetDC(hWnd);
	if (!device) {
		printf("Impossible d'acceder au device !\n");
		return false;
	}

	OSVERSIONINFO osVersion = {0};
	osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&osVersion);

	//int count = ::DescribePixelFormat( device, 1, sizeof( PIXELFORMATDESCRIPTOR ), NULL );
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // double buffering obligatoire sous Windows
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cAlphaBits = renderflags & ESGI_WINDOW_RGBA ? 8 : 0;
	pfd.cDepthBits = renderflags & ESGI_WINDOW_DEPTH ? 24 : 0;
	pfd.cStencilBits = renderflags & ESGI_WINDOW_STENCIL ? 8 : 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	
	if (wglChoosePixelFormatARB == NULL)
	{
		pixelFormat = ::ChoosePixelFormat(device, &pfd);
	}
	else
	{
		float fAttributes[] = {0,0};
		UINT numFormats;
		int iAttributes[] = { 
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_DOUBLE_BUFFER_ARB, TRUE,
				WGL_COLOR_BITS_ARB, 24,
				WGL_ALPHA_BITS_ARB, renderflags & ESGI_WINDOW_RGBA ? 8 : 0,
				WGL_DEPTH_BITS_ARB, renderflags & ESGI_WINDOW_DEPTH ? 24 : 0,
				WGL_STENCIL_BITS_ARB, renderflags & ESGI_WINDOW_STENCIL ? 8 : 0,
				WGL_DOUBLE_BUFFER_ARB, renderflags & ESGI_WINDOW_DOUBLEBUFFER ? GL_TRUE : GL_FALSE,
				//WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
				//WGL_SAMPLES_ARB, 4 ,                        // Check For 4x Multisampling
				0, 0};

		BOOL valid = wglChoosePixelFormatARB(device, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
		if (valid == FALSE)
		{
			return false;
		}
	}

	BOOL success = ::SetPixelFormat(device, pixelFormat, &pfd);
	if (success == FALSE)
	{
		printf("Impossible de definir un format de pixel !\n");
		return false;
	}

	context = ::wglCreateContext(device);
	if (!context)
	{
		printf("Impossible de creer le render context !\n");
		return false;
	}	

	::wglMakeCurrent(device, context);

	//
	// EXTENSIONS
	InitOGLExtensions();
	//

	// Test la version maximum supportee
	ESGI_PRINT("Version max d'OpenGL supportee: %s\nVersion max du GLSL supportee: %s\n\n", (char *)glGetString(GL_VERSION), (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// cf. http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
	
	if (wglCreateContextAttribsARB)
	{
		int attribs[9] = { 0 };
		
		// profile
		attribs[0] = WGL_CONTEXT_MAJOR_VERSION_ARB; attribs[1] = majorVersion;
		attribs[2] = WGL_CONTEXT_MINOR_VERSION_ARB; attribs[3] = minorVersion;
		// context
			WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
		attribs[4] = WGL_CONTEXT_FLAGS_ARB; attribs[5] = contextFlags; // WGL_CONTEXT_DEBUG_BIT_ARB et/ou WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	
		// profile
		if (majorVersion >= 3 && minorVersion >= 2) { // bug fix pour les anciens drivers nVidia
			attribs[6] = WGL_CONTEXT_PROFILE_MASK_ARB; attribs[7] = profileFlags; //WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB ou WGL_CONTEXT_CORE_PROFILE_BIT_ARB
		}

		HGLRC newcontext = wglCreateContextAttribsARB(device, 0, attribs);

		::wglMakeCurrent(NULL, NULL);
		::wglDeleteContext(context);
		context = newcontext;
		::wglMakeCurrent(device, context);
	}

	// show me the money !

	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::UpdateWindow(hWnd);
	::SetFocus(hWnd);

#else

	InitOGLExtensions();

#endif		

	return true;
}

///
//  ESgiWindowProc()
//
//      Main window procedure
//
LRESULT WINAPI EsgiWindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	LRESULT lRet = 1; 

	switch (uMsg) 
	{ 
		case WM_ACTIVATE:
			break;

		case WM_CREATE:
			break;
	/*
		case WM_SIZE:
			{
			EsgiContext *context = (EsgiContext*)(LONG_PTR) GetWindowLongPtr( hWnd, GWL_USERDATA );
			if ( context ) {
				context->width = LOWORD( lParam );
				context->height = HIWORD( lParam );
				InvalidateRect( esContext->hWnd, NULL, FALSE );
			}
			}
	*/
		case WM_PAINT:
			{
			EsgiContext *context = (EsgiContext*)(LONG_PTR) GetWindowLongPtr( hWnd, GWL_USERDATA );
            
			//if ( context && context->drawFunc )
			// if (context.drawFunc)
			//   context.drawFunc();
            
			if (context) {
				ValidateRect( context->hWnd, NULL );
			}
			}
			return 0;

		case WM_SYSCOMMAND:                     // Intercept System Commands
		{
			switch (wParam)                     // Check System Calls
			{
				case SC_SCREENSAVE:             // Screensaver Trying To Start?
				case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
				return 0;                   // Prevent From Happening
			}
			break;                          // Exit
		}
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0; 
		/*    
		case WM_CHAR:
			{
			POINT      point;
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
            
			GetCursorPos( &point );

			if ( esContext && esContext->keyFunc )
				esContext->keyFunc ( esContext, (unsigned char) wParam, 
										(int) point.x, (int) point.y );
			}
			break;
		*/ 

         case WM_KEYDOWN:
             if ((BYTE)wParam == VK_ESCAPE) {
				 PostQuitMessage(0); 
                 return 0;
			 }
             break;

		default: 			
			break; 
	} 

	lRet = DefWindowProc(hWnd, uMsg, wParam, lParam); 
	return lRet; 
}

bool EsgiContext::CreateWindow(const char *applicationName, int originx, int originy, int width, int height, unsigned int flags)
{
	renderflags = flags;

#ifdef ESGI_GLUT

	unsigned int glutDisplayModeBits = GLUT_RGBA|GLUT_DOUBLE;
	unsigned int flagBits = flags;
	unsigned int mask = 1;
	while (flagBits != 0)
	{			
		switch (flagBits & mask)
		{
			case 0: break;
			case ESGI_WINDOW_DEPTH: glutDisplayModeBits |= GLUT_DEPTH; break;
			case ESGI_WINDOW_STENCIL: glutDisplayModeBits |= GLUT_STENCIL; break;
			case ESGI_WINDOW_MULTISAMPLE: glutDisplayModeBits |= GLUT_MULTISAMPLE; break;
			case ESGI_WINDOW_DOUBLEBUFFER: glutDisplayModeBits |= GLUT_DOUBLE; break;
			default: break;
		}
		flagBits &= ~mask;
		mask <<= 1;
	}
	glutInitWindowPosition(originx, originy);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(glutDisplayModeBits);        
	glutCreateWindow(applicationName);

#else

	WNDCLASS wndclass = {0}; 
	DWORD    wStyle   = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);
   
	wndclass.style         = CS_OWNDC;
	wndclass.lpfnWndProc   = (WNDPROC)EsgiWindowProc; 
	wndclass.hInstance     = hInstance; 
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclass.lpszClassName = TEXT("opengl");

	if ( !RegisterClass(&wndclass) ) 
		return false; 

	// if (!fullscreen)
	wStyle = /*WS_VISIBLE*/ WS_POPUP | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CAPTION;// | WS_SIZEBOX;
   
	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = width;
	windowRect.bottom = height;

	AdjustWindowRect(&windowRect, wStyle, FALSE);

	int positionx = originx;
	int positiony = originy;
	int longueur = windowRect.right - windowRect.left;
	int hauteur = windowRect.bottom - windowRect.top;

	if (flags & 0x80000000)
	{
		int desktopWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int desktopHeight = ::GetSystemMetrics(SM_CYSCREEN);
		 
		positionx = (desktopWidth - longueur)/2;
		positiony = (desktopHeight - hauteur)/2;
	}

	//
	// Appel explicite a CreateWindowA car on a #undef la macro CreateWindow !!!
	//
	hWnd = CreateWindowA(
						TEXT("opengl"),
						applicationName,
						wStyle,
						positionx,
						positiony,
						longueur,
						hauteur,
						NULL,
						NULL,
						hInstance,
						NULL);

	if (hWnd == NULL) {
		return false;
	}	

	// Set the ESContext* to the GWL_USERDATA so that it is available to the 
	// EsgiWindowProc
	SetWindowLongPtr( hWnd, GWL_USERDATA, (LONG)(LONG_PTR)this );

	//ShowWindow(hWnd, FALSE);

#endif

	return true;
}

// --- Finalisation ----------------------------------------------------------

void EsgiContext::DestroyContext()
{
    if (context)
    {
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( context );
		context = NULL;
    }

    if (device)
    {
		ReleaseDC( hWnd, device );
		device = NULL;
    }
}

void EsgiContext::CloseWindow()
{
	if (hWnd)
    {
        DestroyWindow( hWnd );
        hWnd = NULL;

		UnregisterClass("opengl", GetModuleHandle(NULL));

		//if (fullscreen)
		//{
		//	// Restore original desktop resolution
		//	ChangeDisplaySettings( NULL, CDS_FULLSCREEN );
		//}
    }	
}

// ---

void EsgiContext::SwapBuffers()
{
#ifdef ESGI_GLUT

	glutSwapBuffers();
	glutPostRedisplay();

#else

	::SwapBuffers(device);

#endif
}

void EsgiContext::MainLoop()
{
   MSG msg = { 0 };
   bool done = false;

#ifdef ESGI_GLUT

	glutReshapeFunc(&esgiReshapeFunc);
	glutIdleFunc(&esgiIdleFunc);
	glutDisplayFunc(&esgiDisplayFunc);
	glutKeyboardFunc(&esgiKeyboardFunc);
	glutSpecialFunc(&esgiKeyboardSpecialFunc);
	glutPassiveMotionFunc(&esgiMouseFunc);
	glutMainLoop();

#else

   while (!done)
   {
      bool gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);

      if (gotMsg)
      {
         if (msg.message == WM_QUIT)
         {
             done = true; 
         }
         else
         {
             TranslateMessage(&msg); 
             DispatchMessage(&msg); 
         }      
	  }
#if 0 // voir ci-dessous
      else
         SendMessage( hWnd, WM_PAINT, 0, 0 );
#endif
      
      esgiIdleFunc();
	  // Il est preferable d'effectuer le rendu ici plutot
	  // que de passer par le message systeme WM_PAINT et subir
	  // une eventuelle latence 
	  esgiDisplayFunc();
   }

#endif

   esgiCleanFunc();
}

#endif