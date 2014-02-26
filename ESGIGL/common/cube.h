// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_CUBE
#define ESGI_CUBE

// --- Includes --------------------------------------------------------------

#include "vector.h"

// --- Classes ---------------------------------------------------------------

enum eCubeMode
{
	eCube_Simple = 0,
	eCube_Complex,
	eCube_Max
};

class Cube
{
public:
	Cube() : m_Position(vec3(0.f, 0.f, 0.f)), m_Velocity(vec3(0.f, 0.f, 0.f))
		, m_Orientation(vec3(0.f, 0.f, 0.f)), m_AutoRotation(vec3(0.f, 0.f, 0.f))
		, m_Color(vec3(1.f, 1.f, 1.f)), m_Opacity(1.f)
	{
		m_Scale = 1.f;
	}
	~Cube()
	{
	}

	inline const vec3& GetPosition() const		{ return m_Position; }
	inline const vec3& GetOrientation() const	{ return m_Orientation; }
	inline float GetScale() const				{ return m_Scale; }
	inline const vec3& GetColor() const			{ return m_Color; }
	inline float GetOpacity() const				{ return m_Opacity; }

	inline void SetPosition(vec3 pos)			{ m_Position = pos; }
	inline void SetOrientation(vec3 rot)		{ m_Orientation = rot; }	
	inline void SetScale(float scale)			{ m_Scale = scale; }
	inline void SetColor(vec3 col, float a)		{ m_Color = col; m_Opacity = a; }
	inline void SetAutoRotation(vec3 rot)		{ m_AutoRotation = rot; }

	void Process(float elapsedTime);
	void Render(unsigned int programObject);

	// globales
	static void InitialiseGlobals(eCubeMode mode);
	static void Clean();

	static float *vertices;
	static float *normales;
	static short *indices;

	static unsigned int vertexCount;
	static unsigned int indexCount;
	static unsigned int verticesVBO;
	static unsigned int normalesVBO;
	static unsigned int indicesVBO;	

private:
	vec3	m_Position;
	float	m_Scale;
	vec3	m_Orientation;
	vec3	m_AutoRotation;
	vec3	m_Velocity;
	vec3	m_Color;
	float	m_Opacity;
};

#endif // ESGI_CUBE