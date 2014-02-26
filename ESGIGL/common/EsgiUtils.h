// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_UTILS
#define ESGI_UTILS

// --- Includes --------------------------------------------------------------

#include "matrix.h"

// --- Functions --------------------------------------------------------------

void esgiUtilsDrawAxes(const mat4 &modelview, const mat4 &projection, float size = 1.f);
bool esgiUtilsSetupShaders();
void esgiUtilsCleanShaders();

#endif // ESGI_UTILS