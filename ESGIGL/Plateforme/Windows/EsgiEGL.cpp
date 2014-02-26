// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#if defined(_WIN32) && defined(ESGI_GLES_20)

// --- Includes --------------------------------------------------------------

#include "../EsgiGL.h"
#include <cstdio>

// base sur ESUtil.c issu de OpenGL(R) ES 2.0 Programming Guide
// http://www.opengles-book.com

#if defined(_MSC_VER) && defined(ESGI_GLES_20)
#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#endif

// --- Functions -------------------------------------------------------------

// --- log -------------------------------------------------------------------

void ESGI_PRINT(const char *format, ...)
{
	char buffer[512];
	
	// Formatage du message dans une chaine de caractere
	va_list vaParams;
	va_start(vaParams, format);
	vsnprintf(buffer, 512, format, vaParams);
	va_end(vaParams);	

#if _MSC_VER
	::OutputDebugString(buffer);
#endif
	printf("%s", buffer);
}

void ESGI_PRINTSCREEN(int x, int y, const char *format, ...)
{
	char buffer[512];
	
	// Formatage du message dans une chaine de caractere
	va_list vaParams;
	va_start(vaParams, format);
	vsnprintf(buffer, 512, format, vaParams);
	va_end(vaParams);
	strcat(buffer, "\n");

#if _MSC_VER
	::OutputDebugString(buffer);
#endif
	//printf("%s", buffer);
}

// --- Initialisation --------------------------------------------------------

bool EsgiContext::CreateContext()
{
   EGLint numConfigs;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLDisplay display;
   EGLContext context;
   EGLSurface surface;
   EGLConfig config;
   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
   
   EGLint configAttribList[] =
   {
       EGL_RED_SIZE,       5,
       EGL_GREEN_SIZE,     6,
       EGL_BLUE_SIZE,      5,
       EGL_ALPHA_SIZE,     (renderflags & ESGI_WINDOW_RGBA) ? 8 : EGL_DONT_CARE,
       EGL_DEPTH_SIZE,     (renderflags & ESGI_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
       EGL_STENCIL_SIZE,   (renderflags & ESGI_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
       EGL_SAMPLE_BUFFERS, (renderflags & ESGI_WINDOW_MULTISAMPLE) ? 1 : 0,
       EGL_NONE
   };
   EGLint surfaceAttribList[] =
   {
       //EGL_POST_SUB_BUFFER_SUPPORTED_NV, flags & (ES_WINDOW_POST_SUB_BUFFER_SUPPORTED) ? EGL_TRUE : EGL_FALSE,
       EGL_NONE, EGL_NONE
   };

   // Get Display
   display = eglGetDisplay(GetDC(hWnd));
   if ( display == EGL_NO_DISPLAY )
   {
      return EGL_FALSE;
   }

   // Initialize EGL
   if ( !eglInitialize(display, &majorVersion, &minorVersion) )
   {
      return EGL_FALSE;
   }

   // Get configs
   if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
   {
      return EGL_FALSE;
   }

   // Choose config
   if ( !eglChooseConfig(display, configAttribList, &config, 1, &numConfigs) )
   {
      return EGL_FALSE;
   }

   // Create a surface
   surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, surfaceAttribList);
   if ( surface == EGL_NO_SURFACE )
   {
      return EGL_FALSE;
   }

   // Create a GL context
   context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
   if ( context == EGL_NO_CONTEXT )
   {
      return EGL_FALSE;
   }   
   
   // Make the context current
   if ( !eglMakeCurrent(display, surface, surface, context) )
   {
      return EGL_FALSE;
   }
   
   eglDisplay = display;
   eglSurface = surface;
   eglContext = context;

   // show me the money !
   ShowWindow(hWnd, TRUE);

   return EGL_TRUE;
}

///
//  ESWindowProc()
//
//      Main window procedure
//
LRESULT WINAPI EsgiWindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
   LRESULT  lRet = 1; 

   switch (uMsg) 
   { 
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
         break;

	  case WM_CLOSE:
      case WM_DESTROY:
         PostQuitMessage(0);             
         break; 
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
      default: 
         lRet = DefWindowProc(hWnd, uMsg, wParam, lParam); 
         break; 
   } 

   return lRet; 
}

bool EsgiContext::CreateWindow(const char *applicationName, int originx, int originy, int width, int height, unsigned int flags)
{
	renderflags = flags;

	WNDCLASS wndclass = {0}; 
	DWORD    wStyle   = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);
   
	wndclass.style         = CS_OWNDC;
	wndclass.lpfnWndProc   = (WNDPROC)EsgiWindowProc; 
	wndclass.hInstance     = hInstance; 
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclass.lpszClassName = TEXT("opengles20");

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
						TEXT("opengles20"),
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

	return true;
}

void EsgiContext::MainLoop()
{
   MSG msg = { 0 };
   bool done = false;

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
}


// --- Finalisation ----------------------------------------------------------

void EsgiContext::DestroyContext()
{
    if (eglContext)
    {
		//eglMakeCurrent( NULL, NULL );
		eglDestroyContext( eglDisplay, eglContext );
		eglContext = EGL_NO_CONTEXT;
    }

	if (eglSurface)
	{
		eglDestroySurface(eglDisplay, eglSurface);
		eglSurface = EGL_NO_SURFACE;
	}

    if (eglDisplay)
    {
		eglTerminate( eglDisplay);
		eglDisplay = EGL_NO_DISPLAY;
    }
}

void EsgiContext::CloseWindow()
{
	if (hWnd)
    {
        DestroyWindow( hWnd );
        hWnd = NULL;

		UnregisterClass("opengles20", GetModuleHandle(NULL));

		//if (fullscreen)
		//{
		//	// Restore original desktop resolution
		//	ChangeDisplaySettings( NULL, CDS_FULLSCREEN );
		//}
    }	
}


void EsgiContext::SwapBuffers()
{
	eglSwapBuffers(eglDisplay, eglSurface);
}



#endif // ESGI_GLES_20