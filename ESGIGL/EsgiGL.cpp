// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------


#include "EsgiGL.h"
#include "Common/EsgiTimer.h"
#include "Common/EsgiUtils.h"

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

// --- Globals ---------------------------------------------------------------

EsgiGLApplication* EsgiGLApplication::g_ThisInstance = NULL;


static inline const char * GetGLErrorString(GLenum error)
{
    const char *str;
    switch( error )
    {
        case GL_NO_ERROR:
            str = "GL_NO_ERROR";
            break;
        case GL_INVALID_ENUM:
            str = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            str = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            str = "GL_INVALID_OPERATION";
            break;      
#if defined __gl_h_ || defined __gl3_h_
        case GL_OUT_OF_MEMORY:
            str = "GL_OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            str = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
#endif
#if defined __gl_h_
        case GL_STACK_OVERFLOW:
            str = "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            str = "GL_STACK_UNDERFLOW";
            break;
        case GL_TABLE_TOO_LARGE:
            str = "GL_TABLE_TOO_LARGE";
            break;
#endif
        default:
            str = "(ERROR: Unknown Error Enum)";
            break;
    }
    return str;
}

// --- Classes ---------------------------------------------------------------

EsgiGLApplication::EsgiGLApplication() : 
					m_FramebufferFlags(ESGI_WINDOW_RGBA)
					, m_ApplicationName(0), m_Width(800), m_Height(600), m_OriginX(0), m_OriginY(0)
					, m_PreviousFrameTime(0), m_ElapsedTime(0), m_AppTime(0)
{
	assert(g_ThisInstance == NULL);
	g_ThisInstance = this;

	memset(&m_Context, 0, sizeof(EsgiContext));
	m_Context.SetContextVersion(2, 0);
	m_Context.SetContextProfile(ESGI_PROFILE_COMPATIBLE);
	
	m_DebugTextColor.set(0.f, 0.f, 0.f, 1.f);
	m_DebugDrawFunc = NULL;
    
    m_InitFunc = NULL;
    m_CleanFunc = NULL;
    m_DrawFunc = NULL;
    m_UpdateFunc = NULL;
    m_KeyFunc = NULL;

}

EsgiGLApplication::~EsgiGLApplication()
{
	if (!m_Initialised) {
		Shutdown();
	}
}

void EsgiGLApplication::CreateWindow(const char *nom, bool centered)
{
	m_ApplicationName = nom;

	m_Initialised = false;
    
	if (centered) {
		// pas vraiment de rapport avec le framebuffer, mais plus simple comme ca
		m_FramebufferFlags |= 0x80000000;
	}
	
	if (m_Context.CreateWindow(m_ApplicationName, m_OriginX, m_OriginY, m_Width, m_Height, m_FramebufferFlags))
	{
		m_Initialised = Initialise();
	}
}

bool EsgiGLApplication::Initialise()
{
	bool ok = true;

	ok = m_Context.CreateContext();
	
	if (ok)
	{
		//
		// infos sur le GPU et le driver
		//
		int maxVertexAttribs, maxVertexUniforms, maxVarying, maxFragmentUniforms;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
#if defined(ESGI_GLES_20)
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniforms);
		glGetIntegerv(GL_MAX_VARYING_VECTORS, &maxVarying);
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragmentUniforms);
#else
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniforms);
		glGetIntegerv(GL_MAX_VARYING_FLOATS, &maxVarying);
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniforms);
#endif
		// affiche les informations du driver et de la carte graphique
		ESGI_PRINT("Pilotes OpenGL version %s - supporte GLSL version %s\n", (char *)glGetString(GL_VERSION), (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
		ESGI_PRINT("Chipset graphique \"%s\" fabrique par \"%s\"\n\n", (char *)glGetString(GL_RENDERER), (char *)glGetString(GL_VENDOR));
		ESGI_PRINT("max vertex attributes = %d, max vertex uniforms = %d\n", maxVertexAttribs, maxVertexUniforms);
		ESGI_PRINT("max varying = %d, max fragment uniforms = %d\n\n", maxVarying, maxFragmentUniforms);
	}
	else
	{
	}

	return ok;
}

void EsgiGLApplication::Shutdown()
{
	m_Context.DestroyContext();
	m_Context.CloseWindow();
}

void EsgiGLApplication::HandleInputs(unsigned int key, int mousex, int mousey)
{
	m_DeltaMouseX = m_MouseX - mousex;
	m_DeltaMouseY = m_MouseY - mousey;
	m_MouseX = mousex;
	m_MouseY = mousey;

	if (key)
	{
		switch(key)
		{
			case 27: exit(0);
		}

		m_KeyFunc(key);
	}
	//m_MouseFunc(mousex, mousey);
}

bool EsgiGLApplication::Init()
{
	if (!esgiUtilsSetupShaders()) {
		return false;
	}
    
	if (m_InitFunc) {
        return m_InitFunc();
	}

    return true;
}


void EsgiGLApplication::Clean()
{
    if (m_CleanFunc) {
        m_CleanFunc();
        m_CleanFunc = NULL;
    }
	esgiUtilsCleanShaders();
    Shutdown();
}

void EsgiGLApplication::Update()
{
	m_ElapsedTime = EsgiTimer::GetElapsedTimeSince(m_PreviousFrameTime);
	m_PreviousFrameTime = EsgiTimer::GetTimerValue();
	m_AppTime += m_ElapsedTime;

	// mise a jour d'une frame de l'application
	m_UpdateFunc((float)m_ElapsedTime);
}
/*
void EsgiGL2Application::Resize(int longueur, int hauteur)
{
	m_Width = longueur;
	m_Height = hauteur;
}
*/
void EsgiGLApplication::Render()
{
	// callback utilisateur
	m_DrawFunc();

#if 0

	// post render
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

#ifndef ESGI_GLES_20
	// prepare le rendu du texte en projection orthonormale
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, m_Width, 0, m_Height);
	// inverse l'axe Y (pointe vers le bas)
	glScalef(1, -1, 1);
	// translate l'origine au coin haut gauche de l'écran
	glTranslatef(0, -m_Height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4fv(&m_DebugTextColor.x);
#endif

	ESGI_PRINTSCREEN(10, 16, "%4.2f fps (%3.6f ms) - Time : %6.2f sec", 1.0/m_ElapsedTime, m_ElapsedTime*1000.0, m_AppTime);

	if (m_DebugDrawFunc)
		m_DebugDrawFunc();

#ifndef ESGI_GLES_20
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4f(1.f, 1.f, 1.f, 1.f);
#endif

#endif

	SwapBuffers();
}

// --- Main Loop -------------------------------------------------------------

void EsgiGLApplication::MainLoop()
{
	if (m_Initialised)
	{	
		m_PreviousFrameTime = EsgiTimer::GetTimerValue();

        if (Init()) {
            m_Context.MainLoop();
        }
	}

	Shutdown();
}

void EsgiGLApplication::SwapBuffers()
{
	m_Context.SwapBuffers();
}

// divers ---

void EsgiGLApplication::PrintExtensions()
{
	// liste les extensions disponibles
	printf("Extensions supportees par le pilote:\n\n");
	char *extStr = (char *)glGetString(GL_EXTENSIONS);
	while (*extStr != 0)
	{
		if (*extStr != ' ')
			putchar(*extStr);
		else
			putchar('\n');
		extStr++;
	}
	putchar('\n');
	putchar('\n');
}

bool EsgiGLApplication::IsExtensionSupported(const char *extension)
{
	int extLen = strlen(extension);
	char *extStr = (char *)glGetString(GL_EXTENSIONS);
	char *ext = strstr(extStr, extension);
	while (ext != NULL)
	{
		// afin d'etre sur qu'il s'agit de la bonne extension et non d'une variante...
		int len = 0;
		while (*ext != ' ')
		{
			len++;
			ext++;
		}

		if (len == extLen) {
			return true;
		}

		ext = strstr(ext, extension);
	}

	return false;
}

// --- Glut callbacks --------------------------------------------------------

void esgiKeyboardFunc(unsigned char key, int mx, int my)
{	
	EsgiGLApplication::g_ThisInstance->HandleInputs(key, mx, my);
}

void esgiKeyboardSpecialFunc(int key, int mx, int my)
{	
	EsgiGLApplication::g_ThisInstance->HandleInputs(key, mx, my);
}

void esgiMouseFunc(int mx, int my)
{
	EsgiGLApplication::g_ThisInstance->HandleInputs(0, mx, my);
}

void esgiReshapeFunc(int , int )
{
}

void esgiIdleFunc()
{
	EsgiGLApplication::g_ThisInstance->Update();
}

void esgiDisplayFunc()
{
	EsgiGLApplication::g_ThisInstance->Render();
}

void esgiCleanFunc()
{
    EsgiGLApplication::g_ThisInstance->Clean();
}
