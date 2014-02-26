// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_EGL
#define ESGI_EGL

// --- Includes --------------------------------------------------------------

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#if defined(_WIN32)
// HACK Windows: CreateWindow est une macro definie par Windows.h
// Or comme nous avons quelques fonctions qui se nomment de la meme maniere
// il nous faut supprimer sa definition autrement le preprocesseur
// va remplacer toutes les occurences de CreateWindow par CreateWindowA !
#ifdef CreateWindow
#undef CreateWindow
#endif
#endif

// ---

struct EsgiContext
{	
	EGLNativeWindowType  hWnd;	/// Window handle
	EGLDisplay  eglDisplay;		/// EGL display   	
	EGLContext  eglContext;		/// EGL context	
	EGLSurface  eglSurface;		/// EGL surface
	unsigned int renderflags;

	void SetContextVersion(unsigned int major, unsigned int minor) {}
	void SetContextProfile(unsigned int profile) {}
	void SetContextFlags(unsigned int flags) {}

	bool CreateContext();
	void DestroyContext();

	void ChoosePixelFormat();
	void SetPixelFormat();	
	
	bool CreateWindow(const char *name, int originex, int originey, int longueur, int hauteur, unsigned int flags);
	void CloseWindow();

	void MainLoop();
	void SwapBuffers();
};

#endif // ESGI_EGL