// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#if defined(_WIN32) && !defined(ESGI_GLES_20)

#pragma comment (lib, "opengl32.lib")  /* link with Microsoft OpenGL lib */

// --- Includes --------------------------------------------------------------

#include "EsgiWinExtensions.h"

// --- Globals ---------------------------------------------------------------

// Core
PFNGLBLENDEQUATIONPROC							glBlendEquation					= NULL;
PFNGLACTIVETEXTUREPROC							glActiveTexture					= NULL;
PFNGLCLIENTACTIVETEXTUREPROC					glClientActiveTexture			= NULL;
PFNGLDRAWRANGEELEMENTSPROC						glDrawRangeElements				= NULL;
// Framebuffer
PFNGLDRAWBUFFERSPROC							glDrawBuffers		            = NULL;

// Textures
PFNGLGENERATEMIPMAPPROC                         glGenerateMipmap                = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC					glCompressedTexImage2D          = NULL;

// Shaders
PFNGLUSEPROGRAMPROC                             glUseProgram					= NULL;
PFNGLBINDATTRIBLOCATIONPROC                     glBindAttribLocation            = NULL;
PFNGLGETATTRIBLOCATIONPROC                      glGetAttribLocation             = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC				glEnableVertexAttribArray       = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC				glDisableVertexAttribArray      = NULL;
PFNGLVERTEXATTRIBPOINTERPROC					glVertexAttribPointer           = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC					glVertexAttribIPointer          = NULL;
PFNGLCOMPILESHADERPROC                          glCompileShader                 = NULL;
PFNGLCREATEPROGRAMPROC                          glCreateProgram                 = NULL;
PFNGLCREATESHADERPROC                           glCreateShader                  = NULL;
PFNGLATTACHSHADERPROC                           glAttachShader                  = NULL;
PFNGLDETACHSHADERPROC                           glDetachShader                  = NULL;
PFNGLDELETESHADERPROC							glDeleteShader					= NULL;
PFNGLLINKPROGRAMPROC                            glLinkProgram                   = NULL;
PFNGLVALIDATEPROGRAMPROC                        glValidateProgram               = NULL;
PFNGLDELETEPROGRAMPROC                          glDeleteProgram                 = NULL;
PFNGLGETPROGRAMIVPROC                           glGetProgramiv                  = NULL;
PFNGLGETPROGRAMINFOLOGPROC                      glGetProgramInfoLog             = NULL;
PFNGLPROGRAMPARAMETERIPROC						glProgramParameteri				= NULL;
PFNGLSHADERSOURCEPROC                           glShaderSource                  = NULL;
PFNGLGETSHADERIVPROC                            glGetShaderiv                   = NULL;
PFNGLGETSHADERINFOLOGPROC                       glGetShaderInfoLog              = NULL;
PFNGLGETUNIFORMLOCATIONPROC                     glGetUniformLocation            = NULL;
PFNGLUNIFORMMATRIX3FVPROC                       glUniformMatrix3fv              = NULL;
PFNGLUNIFORMMATRIX4FVPROC                       glUniformMatrix4fv              = NULL;
PFNGLVERTEXATTRIB1FPROC                         glVertexAttrib1f                = NULL;
PFNGLVERTEXATTRIB3FPROC                         glVertexAttrib3f                = NULL;
PFNGLVERTEXATTRIB4FPROC                         glVertexAttrib4f                = NULL;
PFNGLUNIFORM1FPROC                              glUniform1f                     = NULL;
PFNGLUNIFORM1IPROC                              glUniform1i                     = NULL;
PFNGLUNIFORM3FPROC                              glUniform3f                     = NULL;
PFNGLUNIFORM4FPROC                              glUniform4f                     = NULL;
PFNGLUNIFORM1FVPROC                             glUniform1fv                    = NULL;
PFNGLUNIFORM3FVPROC                             glUniform3fv                    = NULL;
PFNGLUNIFORM4FVPROC                             glUniform4fv                    = NULL;

PFNGLDRAWELEMENTSINSTANCEDPROC					glDrawElementsInstanced			= NULL;
PFNGLVERTEXATTRIBDIVISORARBPROC					glVertexAttribDivisor			= NULL;

// Buffer Objects
PFNGLGENBUFFERSPROC                             glGenBuffers                    = NULL;
PFNGLBINDBUFFERPROC                             glBindBuffer                    = NULL;
PFNGLMAPBUFFERPROC								glMapBuffer	                    = NULL;
PFNGLUNMAPBUFFERPROC							glUnmapBuffer                   = NULL;
PFNGLDELETEBUFFERSPROC                          glDeleteBuffers                 = NULL;

// Vertex Buffer Objects (VBO)
PFNGLBUFFERDATAPROC                             glBufferData                    = NULL;
PFNGLBUFFERSUBDATAPROC                          glBufferSubData                 = NULL;

// Framebuffer Objects (FBO)
PFNGLBINDRENDERBUFFERPROC                       glBindRenderbuffer              = NULL;
PFNGLGENRENDERBUFFERSPROC                       glGenRenderbuffers              = NULL;
PFNGLRENDERBUFFERSTORAGEPROC					glRenderbufferStorage           = NULL;
PFNGLDELETERENDERBUFFERSPROC					glDeleteRenderbuffers           = NULL;
PFNGLBINDFRAMEBUFFERPROC                        glBindFramebuffer               = NULL;
PFNGLDELETEFRAMEBUFFERSPROC                     glDeleteFramebuffers            = NULL;
PFNGLGENFRAMEBUFFERSPROC                        glGenFramebuffers               = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC				glFramebufferRenderbuffer       = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC					glFramebufferTexture2D			= NULL;
PFNGLBLITFRAMEBUFFERPROC                        glBlitFramebuffer               = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVPROC				glGetRenderbufferParameteriv    = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC					glCheckFramebufferStatus        = NULL;

// WGL
PFNWGLCREATECONTEXTATTRIBSARBPROC				wglCreateContextAttribsARB		= NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC					wglChoosePixelFormatARB			= NULL;

// --- Initialisation --------------------------------------------------------

void InitOGLExtensions()
{
	//shaders
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");

	if (glCompileShader == NULL)
	{
		MessageBox(NULL, "OpenGL 2.0 is Not supported!", "Error!", MB_OK);
		//exit(1);
	}

	// Core

	glBlendEquation = ( PFNGLBLENDEQUATIONPROC )wglGetProcAddress("glBlendEquation");
    glClientActiveTexture = ( PFNGLCLIENTACTIVETEXTUREPROC )wglGetProcAddress("glClientActiveTexture");
	glActiveTexture = ( PFNGLACTIVETEXTUREPROC )wglGetProcAddress("glActiveTexture");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
	glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) wglGetProcAddress("glCompressedTexImage2D");	
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC) wglGetProcAddress("glDrawBuffers");
	glDrawRangeElements = ( PFNGLDRAWRANGEELEMENTSPROC )wglGetProcAddress("glDrawRangeElements");
	glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress("glDrawElementsInstanced");
	glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORARBPROC)wglGetProcAddress("glVertexAttribDivisor");

	// Shaders

	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)wglGetProcAddress("glValidateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)wglGetProcAddress("glProgramParameteri");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)wglGetProcAddress("glVertexAttrib3f");
	glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)wglGetProcAddress("glVertexAttrib4f");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
	glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");

	// framebuffer objects (FBO)

	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffers");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbuffer");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffers");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorage");
	
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebuffer");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffers");
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffers");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbuffer");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2D");
	glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) wglGetProcAddress("glBlitFramebuffer");	
	glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC     ) wglGetProcAddress("glGetRenderbufferParameteriv");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");

	// Buffer Objects
	glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer");
	glMapBuffer = (PFNGLMAPBUFFERPROC) wglGetProcAddress("glMapBuffer");
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) wglGetProcAddress("glUnmapBuffer");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");

	// VBO
	glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC) wglGetProcAddress("glBufferSubData");	

	// WGL
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)  wglGetProcAddress("wglChoosePixelFormatARB");
}

#endif