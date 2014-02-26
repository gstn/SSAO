// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_PLATEFORME
#define ESGI_PLATEFORME

// --- Includes --------------------------------------------------------------

#if defined(_WIN64) || defined(_WIN32)

#if !defined(ESGI_GLES_20)
#include "Windows/EsgiWindows.h"
#else
#include "Windows/EsgiEGL.h"
#endif
// sous Windows les entetes OpenGL doivent etre ajoutees APRES windows.h
#include <GL/gl.h>

#elif defined(__APPLE__)

#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
// define something for simulator   
#elif TARGET_OS_IPHONE
// define something for iphone  
#else
#define TARGET_OS_OSX 1
#include <OpenGL/gl.h>
#include "MacOSX/EsgiMacOSX.h"
#endif

#elif defined(__linux__)

#elif defined(__unix__)

#elif defined(__ANDROID__)

// check version __ANDROID_API__

#else

#endif

#endif // ESGI_PLATEFORME
