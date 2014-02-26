// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_GL
#define ESGI_GL

// --- Defines ---------------------------------------------------------------

// OpenGL 1.5 - derniere revision sans shaders
//#define ESGI_GL_15				0 
// OpengGL 2.0 - premiere officialisation des shaders, suivie d'OpenGL 2.1
//#define ESGI_GL_20				1
// OpengGL ES 2.0 - rendu seulement avec shader sur plateformes embarquees
//#define ESGI_GLES_20				2

// Fenetre ---

// SetupWindow flag - RGB color buffer
#define ESGI_WINDOW_RGB				0
// SetupWindow flag - ALPHA color buffer
#define ESGI_WINDOW_RGBA			1 
// SetupWindow flag - depth buffer
#define ESGI_WINDOW_DEPTH			2 
// SetupWindow flag - stencil buffer
#define ESGI_WINDOW_STENCIL			4
// SetupWindow flag - multi-sample buffer
#define ESGI_WINDOW_MULTISAMPLE		8
// Double buffering
#define ESGI_WINDOW_DOUBLEBUFFER	16
// force la fenetre au centre du bureau
#define ESGI_WINDOW_CENTERED        1

// Profile OpenGL (hormis GLES)
#define ESGI_PROFILE_COMPATIBLE		1
#define ESGI_PROFILE_CORE			2
// Context OpenGL (hormis GLES)
#define ESGI_CONTEXT_DEBUG			1
#define ESGI_CONTEXT_FORWARD		2

/*
 * ESGI API macro definitions -- the special key codes:
 */
#define  ESGI_KEY_F1                        0x0001
#define  ESGI_KEY_F2                        0x0002
#define  ESGI_KEY_F3                        0x0003
#define  ESGI_KEY_F4                        0x0004
#define  ESGI_KEY_F5                        0x0005
#define  ESGI_KEY_F6                        0x0006
#define  ESGI_KEY_F7                        0x0007
#define  ESGI_KEY_F8                        0x0008
#define  ESGI_KEY_F9                        0x0009
#define  ESGI_KEY_F10                       0x000A
#define  ESGI_KEY_F11                       0x000B
#define  ESGI_KEY_F12                       0x000C
#define  ESGI_KEY_LEFT                      0x0064
#define  ESGI_KEY_UP                        0x0065
#define  ESGI_KEY_RIGHT                     0x0066
#define  ESGI_KEY_DOWN                      0x0067
#define  ESGI_KEY_PAGE_UP                   0x0068
#define  ESGI_KEY_PAGE_DOWN                 0x0069
#define  ESGI_KEY_HOME                      0x006A
#define  ESGI_KEY_END                       0x006B
#define  ESGI_KEY_INSERT                    0x006C

/*
 * ESGI API macro definitions -- mouse state definitions
 */
#define  ESGI_LEFT_BUTTON                   0x0000
#define  ESGI_MIDDLE_BUTTON                 0x0001
#define  ESGI_RIGHT_BUTTON                  0x0002
#define  ESGI_DOWN                          0x0000
#define  ESGI_UP                            0x0001
#define  ESGI_LEFT                          0x0000
#define  ESGI_ENTERED                       0x0001


// --- Includes --------------------------------------------------------------

#include "Plateforme/EsgiPlateforme.h"	
#include "Common/vector.h"

// --- Globales --------------------------------------------------------------

void ESGI_PRINT(const char *format, ...);
void ESGI_PRINTSCREEN(int x, int y, const char *format, ...);

void esgiKeyboardFunc(unsigned char key, int mx, int my);
void esgiKeyboardSpecialFunc(int key, int mx, int my);
void esgiMouseFunc(int mx, int my);
void esgiReshapeFunc(int , int );
void esgiIdleFunc();
void esgiDisplayFunc();
void esgiCleanFunc();

// --- Classes ---------------------------------------------------------------

class EsgiGLApplication
{
public:
	EsgiGLApplication();
	~EsgiGLApplication();

	inline bool IsInitialised() const							{ return m_Initialised; }

	// GLUT style ---

	void InitWindowPosition(int originx, int originy)			{ m_OriginX = originx; m_OriginY = originy; }
	void InitWindowSize(int longueur, int hauteur)				{ m_Width = longueur; m_Height = hauteur; }	
	void InitDisplayMode(unsigned int flags)					{ m_FramebufferFlags = flags; }
	void CreateWindow(const char *name, bool centered = false);
	
	inline void InitContextVersion(unsigned int major, unsigned int minor)	{ m_Context.SetContextVersion(major, minor); }
	inline void InitContextProfile(unsigned int profile)		{ m_Context.SetContextProfile(profile); }
	inline void InitContextFlags(unsigned int flags)			{ m_Context.SetContextFlags(flags); }

	void MainLoop();
	void MainLoopEvent();
	void SwapBuffers();

	void InitFunc(bool (*initFunc)())                           { m_InitFunc = initFunc; }
	void CleanFunc(void (*cleanFunc)())                         { m_CleanFunc = cleanFunc; }
	
    void DisplayFunc(void (*drawFunc)())						{ m_DrawFunc = drawFunc; }
	void DebugDisplayFunc(void (*drawFunc)())					{ m_DebugDrawFunc = drawFunc; }
	void IdleFunc(void (*updateFunc)(float))					{ m_UpdateFunc = updateFunc; }
	void KeyboardFunction(void (*keyFunc)(unsigned int))		{ m_KeyFunc = keyFunc; }

	// Utils ---

	void PrintExtensions();
	bool IsExtensionSupported(const char *extension);

	inline void SetDebugTextColor(vec4 color)					{ m_DebugTextColor = color; }

	void EnableWorldAxis(bool flag);

	// fonctions internes ---

	inline int GetMouseX() const								{ return m_DeltaMouseX; }
	inline int GetMouseY() const								{ return m_DeltaMouseY; }
	inline int GetDeltaMouseX() const							{ return m_DeltaMouseX; }
	inline int GetDeltaMouseY() const							{ return m_DeltaMouseY; }

	void HandleInputs(unsigned int key, int mousex, int mousey);
	
    bool Init();
	void Update();
	void Render();	
    void Clean();

	// Cette variable globale est requise afin de pouvoir utiliser glutIdleFunc()	
	// Attention: une seule instance de EsgiGL2Application est possible a la fois !
	static EsgiGLApplication *g_ThisInstance;

protected:
	bool Initialise();
	void Shutdown();
	void PostRender();	

	EsgiContext	m_Context;

	// callbacks
	bool (*m_InitFunc)();
    void (*m_CleanFunc)();
    void (*m_DrawFunc)();		
	void (*m_DebugDrawFunc)();
	void (*m_UpdateFunc)( float deltaTime );
	void (*m_KeyFunc)( unsigned int );
	//void (*m_KeySpecialFunc)( unsigned char );

private:
	// renderer
	unsigned int m_FramebufferFlags;
	bool m_Initialised;
	bool m_EnableWorldAxis;

	// window
	const char *m_ApplicationName;
	int m_Width;
	int m_Height;
	int m_OriginX;
	int m_OriginY;
	int m_MouseX;
	int m_MouseY;
	int m_DeltaMouseX;
	int m_DeltaMouseY;	

	// debug
	vec4 m_DebugTextColor;

	// timer
	double m_PreviousFrameTime;
	double m_ElapsedTime;
	double m_AppTime;
};

#endif // ESGI_GL
