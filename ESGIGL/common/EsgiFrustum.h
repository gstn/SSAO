// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_FRUSTUM
#define ESGI_FRUSTUM

// --- Includes --------------------------------------------------------------

#include "vector.h"
#include "matrix.h"

// --- Classes ---------------------------------------------------------------

class EsgiFrustum
{
public:
	EsgiFrustum() {}
	// note: pour rappel les elements de nos matrices sont stockees en memoire colonne apres colonne (column-major order)
	// ce qui signifie en pratique que les 4eme elements des vecteurs lignes (rows) sont stockees aux offset 3, 7, 11, 15
	void ExtractPlanes(const mat4& m)
	{
		// implementation standard basee sur l'excellent papier de Gribb & Hartmann
		// http://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf

		// plan gauche, -w < x -> v.(row4 + row1)
		m_Planes[0].x = m.I.w + m.I.x;
		m_Planes[0].y = m.J.w + m.J.x;
		m_Planes[0].z = m.K.w + m.K.x;
		m_Planes[0].w = m.T.w + m.T.x;

		// plan droit, x < w -> v.(row4 - row1)
		m_Planes[1].x = m.I.w - m.I.x;
		m_Planes[1].y = m.J.w - m.J.x;
		m_Planes[1].z = m.K.w - m.K.x;
		m_Planes[1].w = m.T.w - m.T.x;

		// plan bas, -w < y -> v.(row4 + row2)
		m_Planes[2].x = m.I.w + m.I.y;
		m_Planes[2].y = m.J.w + m.J.y;
		m_Planes[2].z = m.K.w + m.K.y;
		m_Planes[2].w = m.T.w + m.T.y;

		// plan haut, y < w -> v.(row4 - row2)
		m_Planes[3].x = m.I.w - m.I.y;
		m_Planes[3].y = m.J.w - m.J.y;
		m_Planes[3].z = m.K.w - m.K.y;
		m_Planes[3].w = m.T.w - m.T.y;

		// plan avant, -w < z -> v.(row4 + row3)
		m_Planes[4].x = m.I.w + m.I.z;
		m_Planes[4].y = m.J.w + m.J.z;
		m_Planes[4].z = m.K.w + m.K.z;
		m_Planes[4].w = m.T.w + m.T.z;

		// plan arriere, z < w -> v.(row4 - row3)
		m_Planes[5].x = m.I.w - m.I.z;
		m_Planes[5].y = m.J.w - m.J.z;
		m_Planes[5].z = m.K.w - m.K.z;
		m_Planes[5].w = m.T.w - m.T.z;

		// cette etape n'est techniquement pas necessaire si l'on ne souhaite effectuer 
		// que des tests logiques sans attendre une valeur precise en retour
		// ...mais autant le faire vu le faible cout de l'operation
		for (int plan=0; plan<6; plan++)
		{
			float x = m_Planes[plan].x, y = m_Planes[plan].y, z = m_Planes[plan].z;
			float inv_length = 1.f / std::sqrt(x * x + y * y + z * z);
			m_Planes[plan].x = x * inv_length;
			m_Planes[plan].y = y * inv_length;
			m_Planes[plan].z = z * inv_length;
			m_Planes[plan].w *= inv_length;
		}
	}

	inline int Classify(const vec4& plane, const vec3& point)
	{
		float d;
		d = plane.x*point.x + plane.y*point.y + plane.z*point.z + plane.w;
		if (d < 0.f) return -1.f; // derriere
		if (d > 0.f) return 1.f; // devant
		return 0.f; // coplanaire
	}

	bool TestAABB(const vec3* aabb)
	{
		for (int plan = 0; plan < 6; plan++)
		{
			bool inside = false;	

			for (int point = 0; point < 8; point++)
			{
				if (Classify(m_Planes[plan], aabb[point]) > 0.f)
				{
					inside = true;
					break;
				}
			}

			if (!inside)
				return false;
		}
		return true;
	}

private:
	vec4 m_Planes[6];
};

#endif // ESGI_FRUSTUM