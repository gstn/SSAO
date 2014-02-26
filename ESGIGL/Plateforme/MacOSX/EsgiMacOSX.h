// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_MACOSX
#define ESGI_MACOSX

// --- Includes --------------------------------------------------------------

// --- Globals ---------------------------------------------------------------

struct EsgiContext
{
private:
    void *self; // Objective-C wrapper

	unsigned int majorVersion;
	unsigned int minorVersion;
	unsigned int contextFlags;
	unsigned int profileFlags;

public:

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

#endif // ESGI_MACOSX