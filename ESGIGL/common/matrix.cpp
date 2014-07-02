// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------

#include "matrix.h"

#define ESGI_PI 3.14159265f

// ---
// La formule classique Cij = Aik * Bkj doit etre transposee du fait que 
// la matrice est stockee en memoire vecteur colonne apres vecteur colonne.
// Ce qui fait que, en realite, des qu'il est question de multiplication
// on doit tenir compte du stockage et adapter le code.
// 
mat4 esgiMultiplyMatrix(const mat4 &A, const mat4 &B)
{
	mat4 C;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			C.m[j][i] = 0;
			for (int k = 0; k < 4; k++)
			{
				C.m[j][i] += A.m[k][i] * B.m[j][k];
			}
			/*C.m[j][i] = A.m[0][i] * B.m[j][0];
			C.m[j][i] += A.m[1][i] * B.m[j][1];
			C.m[j][i] += A.m[2][i] * B.m[j][2];
			C.m[j][i] += A.m[3][i] * B.m[j][3];*/
		}
	}

	return C;
}

// Rotations ---
// Les fonctions suivantes creent des matrices de rotation autour d'un axe du monde
//
mat4 esgiRotateX(float angle)
{
	angle = angle * ESGI_PI / 180.f;
	mat4 m;
	m.Identity();
	float c = cosf(angle), s = sinf(angle);
	m.J.y = c; m.J.z = s;
	m.K.y = -s; m.K.z = c;
	return m;
}

mat4 esgiRotateY(float angle)
{
	angle = angle * ESGI_PI / 180.f;
	mat4 m;
	m.Identity();
	float c = cosf(angle), s = sinf(angle);
	m.I.x = c; m.I.z = -s;
	m.K.x = s; m.K.z = c;
	return m;
}

mat4 esgiRotateZ(float angle)
{
	angle = angle * ESGI_PI / 180.f;
	mat4 m;
	m.Identity();
	float c = cosf(angle), s = sinf(angle);
	m.I.x = c; m.I.y = s;
	m.J.x = -s; m.J.y = c;
	return m;
}

// repere de camera ---
//
mat4 esgiLookAt(const vec3 &eye, const vec3 &target, const vec3 &up)
{
	vec3 z = (target - eye).NormalizedSafe();
	if (z.Length() == 0.f)
		z.z = -1.f;
	vec3 y = up.Normalized();
	vec3 x = (z.Cross(y)).Normalized();
	y = x.Cross(z);
    vec4 eyePoint;
    eyePoint.set(-eye, 1.f);
	mat4 m;
    m.I.set(x, 0.f);
    m.J.set(y, 0.f);
    m.K.set(-z, 0.f);
	m.T.set(0.f, 0.f, 0.f, 1.f);
	m.Transpose();
    vec4 eyePrime = m*eyePoint;	
    m.T = eyePrime;
	return m;
}

// projection orthogonale ---
//
mat4 esgiOrtho(float left, float right, float bottom, float top, float near, float far)
{
	float a = 2.0f / (right - left);
	float b = 2.0f / (top - bottom);
	float c = -2.0f / (far - near);
	float tx = (right + left) / (right - left);
	float ty = (top + bottom) / (top - bottom);
	float tz = (far + near) / (far - near);

	mat4 m;
	m.I.x = a; m.I.y = 0; m.I.z = 0; m.I.w = 0;
	m.J.x = 0; m.J.y = b; m.J.z = 0; m.J.w = 0;
	m.K.x = 0; m.K.y = 0; m.K.z = c; m.K.w = 0;
	m.T.x = -tx; m.T.y = -ty; m.T.z = -tz; m.T.w = 1;
	return m;
}

// projection perspective ---
//
mat4 esgiFrustum(float left, float right, float bottom, float top, float near, float far)
{
	float a = 2.f * near / (right - left);
	float b = 2.f * near / (top - bottom);
	float c = (right + left) / (right - left);
	float d = (top + bottom) / (top - bottom);
	float e = -((far + near) / (far - near));
	float f = -((2.f * near * far) / (far - near));
	mat4 m;
	m.I.x = a; m.I.y = 0; m.I.z = 0; m.I.w = 0;
	m.J.x = 0; m.J.y = b; m.J.z = 0; m.J.w = 0;
	m.K.x = c; m.K.y = d; m.K.z = e; m.K.w = -1;
	m.T.x = 0; m.T.y = 0; m.T.z = f; m.T.w = 0;
	return m;
}

mat4 esgiPerspective(float fovy, float aspect, float near, float far)
{
	float frustumW, frustumH;
   
	frustumH = tanf( fovy / 360.0f * ESGI_PI ) * near;
	frustumW = frustumH * aspect;

	return esgiFrustum(-frustumW, frustumW, -frustumH, frustumH, near, far);
}

vec4 esgiPerspectiveFrustum(float fovy, float aspect, float near, float far)
{
	float frustumW, frustumH;
   
	frustumH = tanf( fovy / 360.0f * ESGI_PI ) * near;
	frustumW = frustumH * aspect;

	return vec4(-frustumW, frustumW, -frustumH, frustumH);
}
