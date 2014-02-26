// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------

#include "EsgiGL.h"
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

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
}

// OpenGL profile ---

void EsgiContext::SetContextVersion(unsigned int major, unsigned int minor) { majorVersion = major; minorVersion = minor; }
void EsgiContext::SetContextProfile(unsigned int profile) 
{ 
	//profileFlags |= (profile & ESGI_PROFILE_CORE ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB); 
}
void EsgiContext::SetContextFlags(unsigned int flags) 
{ 
	//contextFlags |= (flags & ESGI_CONTEXT_FORWARD ? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB : 0);
	//contextFlags |= (flags & ESGI_CONTEXT_DEBUG ? WGL_CONTEXT_DEBUG_BIT_ARB : 0);
}



