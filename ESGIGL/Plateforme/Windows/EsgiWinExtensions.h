// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_WINDOWS_EXTENSIONS
#define ESGI_WINDOWS_EXTENSIONS

// --- Includes --------------------------------------------------------------

#define ESGI_GLUT

#ifdef ESGI_GLUT
#include "GLUT/glut.h"
#else
/* XXX This is from Win32's <windef.h> */
#ifndef APIENTRY
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif
#endif

/* XXX This is from Win32's <wingdi.h> and <winnt.h> */
#ifndef WINGDIAPI
#define WINGDIAPI __declspec(dllimport)
#endif

#include <GL/gl.h>

#undef APIENTRY
#undef WINGDIAPI
#endif

#include "GL/glext.h"
#include "GL/wglext.h"

// --- Globals ---------------------------------------------------------------

// Core
extern PFNGLBLENDEQUATIONPROC							glBlendEquation;
extern PFNGLACTIVETEXTUREPROC							glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC						glClientActiveTexture; // deprecie
extern PFNGLDRAWRANGEELEMENTSPROC						glDrawRangeElements;
// Framebuffer
extern PFNGLDRAWBUFFERSPROC								glDrawBuffers;
// Textures
extern PFNGLGENERATEMIPMAPPROC							glGenerateMipmap;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC					glCompressedTexImage2D;

// Shaders
extern PFNGLUSEPROGRAMPROC								glUseProgram;
extern PFNGLBINDATTRIBLOCATIONPROC						glBindAttribLocation;
extern PFNGLGETATTRIBLOCATIONPROC						glGetAttribLocation;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC					glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC				glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC                     glVertexAttribPointer;
extern PFNGLVERTEXATTRIBIPOINTERPROC                    glVertexAttribIPointer;
extern PFNGLCOMPILESHADERPROC                           glCompileShader;
extern PFNGLCREATEPROGRAMPROC                           glCreateProgram;
extern PFNGLCREATESHADERPROC                            glCreateShader;
extern PFNGLATTACHSHADERPROC                            glAttachShader;
extern PFNGLDETACHSHADERPROC                            glDetachShader;
extern PFNGLDELETESHADERPROC                            glDeleteShader;
extern PFNGLLINKPROGRAMPROC                             glLinkProgram;
extern PFNGLVALIDATEPROGRAMPROC                         glValidateProgram;
extern PFNGLDELETEPROGRAMPROC                           glDeleteProgram;
extern PFNGLGETPROGRAMIVPROC                            glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC                       glGetProgramInfoLog;
extern PFNGLSHADERSOURCEPROC                            glShaderSource;
extern PFNGLPROGRAMPARAMETERIPROC						glProgramParameteri;
extern PFNGLGETSHADERIVPROC                             glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC                        glGetShaderInfoLog;
extern PFNGLBINDATTRIBLOCATIONPROC                      glBindAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC                      glGetUniformLocation;
extern PFNGLUNIFORMMATRIX3FVPROC                        glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC                        glUniformMatrix4fv;
extern PFNGLVERTEXATTRIB1FPROC                          glVertexAttrib1f;
extern PFNGLVERTEXATTRIB3FPROC                          glVertexAttrib3f;
extern PFNGLVERTEXATTRIB4FPROC                          glVertexAttrib4f;
extern PFNGLUNIFORM1IPROC                               glUniform1i;
extern PFNGLUNIFORM1FPROC                               glUniform1f;
extern PFNGLUNIFORM3FPROC                               glUniform3f;
extern PFNGLUNIFORM4FPROC                               glUniform4f;
extern PFNGLUNIFORM3FVPROC                              glUniform3fv;
extern PFNGLUNIFORM4FVPROC                              glUniform4fv;

extern PFNGLDRAWELEMENTSINSTANCEDPROC					glDrawElementsInstanced;
extern PFNGLVERTEXATTRIBDIVISORARBPROC					glVertexAttribDivisor;

// Buffer Objects
extern PFNGLGENBUFFERSPROC								glGenBuffers;
extern PFNGLBINDBUFFERPROC                              glBindBuffer;
extern PFNGLMAPBUFFERPROC								glMapBuffer;
extern PFNGLUNMAPBUFFERPROC								glUnmapBuffer;
extern PFNGLDELETEBUFFERSPROC                           glDeleteBuffers;

// Vertex Buffer Objects (VBO)
extern PFNGLBUFFERDATAPROC                              glBufferData;
extern PFNGLBUFFERSUBDATAPROC							glBufferSubData;

// Framebuffer Objects (FBO)
extern PFNGLBINDRENDERBUFFERPROC                        glBindRenderbuffer;
extern PFNGLGENRENDERBUFFERSPROC                        glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC                     glRenderbufferStorage;
extern PFNGLDELETERENDERBUFFERSPROC                     glDeleteRenderbuffers;
extern PFNGLBINDFRAMEBUFFERPROC                         glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC                      glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC                         glGenFramebuffers;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC					glFramebufferRenderbuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC					glFramebufferTexture2D;
extern PFNGLBLITFRAMEBUFFERPROC                         glBlitFramebuffer;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC				glGetRenderbufferParameteriv;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC					glCheckFramebufferStatus;

// WGL
extern PFNWGLCREATECONTEXTATTRIBSARBPROC				wglCreateContextAttribsARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC					wglChoosePixelFormatARB;

// --- Functions -------------------------------------------------------------

void InitOGLExtensions();

#endif // ESGI_WINDOWS_EXTENSIONS