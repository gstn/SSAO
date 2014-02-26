// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_WINDOWS
#define ESGI_WINDOWS

// --- Includes --------------------------------------------------------------

#include "EsgiWinExtensions.h"

// HACK Windows: CreateWindow est une macro definie par Windows.h
// Or comme nous avons quelques fonctions qui se nomment de la meme maniere
// il nous faut supprimer sa definition autrement le preprocesseur
// va remplacer toutes les occurences de CreateWindow par CreateWindowA !
#ifdef CreateWindow
#undef CreateWindow
#endif

// --- Globals ---------------------------------------------------------------

struct EsgiContext
{
    HDC		device;			// Private GDI device context
    HGLRC	context;		// Permanent rendering context
	HWND	hWnd;			// HANDLE de la fenetre
	unsigned int majorVersion;
	unsigned int minorVersion;
	unsigned int contextFlags;
	unsigned int profileFlags;

	unsigned int renderflags;


	void SetContextVersion(unsigned int major, unsigned int minor);
	void SetContextProfile(unsigned int profile);
	void SetContextFlags(unsigned int flags);

	bool CreateContext();
	void DestroyContext();

	void ChoosePixelFormat();
	void SetPixelFormat();	
	
	bool CreateWindow(const char *name, int originex, int originey, int longueur, int hauteur, unsigned int flags);
	void CloseWindow();

	void MainLoop();
	void SwapBuffers();
};

#endif // ESGI_WINDOWS