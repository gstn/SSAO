// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_MATHS_MATRIX
#define ESGI_MATHS_MATRIX

// --- Includes --------------------------------------------------------------

#include "vector.h"

// --- Classes ---------------------------------------------------------------

// note: Toutes les matrices sont des matrices "colonne" (column matrix) stockees en memoire 
// dans l'ordre "colonne d'abord" (column-major storage).
//
// Ceci est volontairement amplifie dans les classes Matrix qui suivent par l'utilisation
// explicite des classes VectorN<T> pour bien indiquer qu'en memoire le stockage se fait 
// colonne apres colonne, et non, element apres element (si l'on considere la notation Mij)
// 
// exemple: pour une matrice 2x2, la notation conventionnelle Mij / i = ligne, j = colonne
// ce qui nous laisse penser que m12 (B) est l'element 'suivant' m11 (A) tout a la fois du fait
// du semblant d'incrementation et de l'habitude de lire de la gauche vers la droite et de haut en bas.
//
// | m11 m12 |		| A B |			
// | m21 m22 |		| C D |
//
// Or comme on l'a dit, mathematiquement une matrice est composee de vecteurs colonnes
// et c'est la convention historique suivie par OpenGL et que nous adopterons donc.
// De plus, le stockage s'effectuant colonne apres colonne la matrice precedente sera stockee ainsi:
//
// |m11 m21 m12 m22| -> | A C B D | -> | I.x I.y J.x J.y |
//

// --- Matrix2

template <typename Type>
struct Matrix2 
{
	Matrix2() {}
    Matrix2(const Type* m)
    {
        I.x = m[0]; I.y = m[1];
        J.x = m[2]; J.y = m[3];
    }

	void Identity()
    {
        I.x = 1; I.y = 0;
        J.x = 0; J.y = 1;
    }

	void Transpose()
	{
		Type temp = I.y;
		I.y = J.x;
		J.x = temp;
	}

    vec2 I;
    vec2 J;
};

// --- Matrix3

template <typename Type>
struct Matrix3 
{
	Matrix3() {}    
    Matrix3(const Type* m)
    {
        I.x = m[0]; I.y = m[1]; I.z = m[2];
        J.x = m[3]; J.y = m[4]; J.z = m[5];
        K.x = m[6]; K.y = m[7]; K.z = m[8];
    }
    Matrix3(vec3 _x, vec3 _y, vec3 _z) : I(_x), J(_y), K(_z)
    {
    }

	void Identity()
	{
        I.x = 1; I.y = 0; I.z = 0;
        J.x = 0; J.y = 1; J.z = 0;
        K.x = 0; K.y = 0; K.z = 1;
    }

	void Transpose()
	{
		Type temp0 = I.y; I.y = J.x; J.x = temp0;
		Type temp1 = I.z; I.z = K.x; K.x = temp1;
		Type temp2 = J.z; J.z = K.y; K.y = temp2;
	}

    Matrix3 Transposed() const
    {
        Matrix3 m;
        m.I.x = I.x; m.I.y = J.x; m.I.z = K.x;
        m.J.x = I.y; m.J.y = J.y; m.J.z = K.y;
        m.K.x = I.z; m.K.y = J.z; m.K.z = K.z;
        return m;
    }

    vec3 I;
    vec3 J;
    vec3 K;
};

// --- Matrix4

template <typename Type>
struct Matrix4 
{
	Matrix4() {}
    Matrix4(const Matrix3<Type>& m)
    {
        I.x = m.I.x; I.y = m.I.y; I.z = m.I.z; I.w = 0;
        J.x = m.J.x; J.y = m.J.y; J.z = m.J.z; J.w = 0;
        K.x = m.K.x; K.y = m.K.y; K.z = m.K.z; K.w = 0;
        T.x = 0; T.y = 0; T.z = 0; T.w = 1;
    }
    Matrix4(const Type* m)
    {
        I.x = m[0];  I.y = m[1];  I.z = m[2];  I.w = m[3];
        J.x = m[4];  J.y = m[5];  J.z = m[6];  J.w = m[7];
        K.x = m[8];  K.y = m[9];  K.z = m[10]; K.w = m[11];
        T.x = m[12]; T.y = m[13]; T.z = m[14]; T.w = m[15];
    }

	void Identity()
    {
        I.x = 1; I.y = 0; I.z = 0; I.w = 0;
        J.x = 0; J.y = 1; J.z = 0; J.w = 0;
        K.x = 0; K.y = 0; K.z = 1; K.w = 0;
        T.x = 0; T.y = 0; T.z = 0; T.w = 1;
    }

	void Transpose()
	{
		Type temp0 = I.y; I.y = J.x; J.x = temp0;
		Type temp1 = I.z; I.z = K.x; K.x = temp1;
		Type temp2 = J.z; J.z = K.y; K.y = temp2;
		Type temp3 = T.x; T.x = I.w; I.w = temp3;
		Type temp4 = J.w; J.w = T.y; T.y = temp4;
		Type temp5 = K.w; K.w = T.z; T.z = temp5;
	}

	Matrix4 Transposed() const
    {
        Matrix4 m;
        m.I.x = I.x; m.I.y = J.x; m.I.z = K.x; m.I.w = T.x;
        m.J.x = I.y; m.J.y = J.y; m.J.z = K.y; m.J.w = T.y;
        m.K.x = I.z; m.K.y = J.z; m.K.z = K.z; m.K.w = T.z;
        m.T.x = I.w; m.T.y = J.w; m.T.z = K.w; m.T.w = T.w;
        return m;
    }
/*
	bool invertMatrix(const double m[16], double invOut[16])
	{
		double inv[16], det;
		int i;

		inv[0] = m[5]  * m[10] * m[15] - 
				 m[5]  * m[11] * m[14] - 
				 m[9]  * m[6]  * m[15] + 
				 m[9]  * m[7]  * m[14] +
				 m[13] * m[6]  * m[11] - 
				 m[13] * m[7]  * m[10];

		inv[4] = -m[4]  * m[10] * m[15] + 
				  m[4]  * m[11] * m[14] + 
				  m[8]  * m[6]  * m[15] - 
				  m[8]  * m[7]  * m[14] - 
				  m[12] * m[6]  * m[11] + 
				  m[12] * m[7]  * m[10];

		inv[8] = m[4]  * m[9] * m[15] - 
				 m[4]  * m[11] * m[13] - 
				 m[8]  * m[5] * m[15] + 
				 m[8]  * m[7] * m[13] + 
				 m[12] * m[5] * m[11] - 
				 m[12] * m[7] * m[9];

		inv[12] = -m[4]  * m[9] * m[14] + 
				   m[4]  * m[10] * m[13] +
				   m[8]  * m[5] * m[14] - 
				   m[8]  * m[6] * m[13] - 
				   m[12] * m[5] * m[10] + 
				   m[12] * m[6] * m[9];

		inv[1] = -m[1]  * m[10] * m[15] + 
				  m[1]  * m[11] * m[14] + 
				  m[9]  * m[2] * m[15] - 
				  m[9]  * m[3] * m[14] - 
				  m[13] * m[2] * m[11] + 
				  m[13] * m[3] * m[10];

		inv[5] = m[0]  * m[10] * m[15] - 
				 m[0]  * m[11] * m[14] - 
				 m[8]  * m[2] * m[15] + 
				 m[8]  * m[3] * m[14] + 
				 m[12] * m[2] * m[11] - 
				 m[12] * m[3] * m[10];

		inv[9] = -m[0]  * m[9] * m[15] + 
				  m[0]  * m[11] * m[13] + 
				  m[8]  * m[1] * m[15] - 
				  m[8]  * m[3] * m[13] - 
				  m[12] * m[1] * m[11] + 
				  m[12] * m[3] * m[9];

		inv[13] = m[0]  * m[9] * m[14] - 
				  m[0]  * m[10] * m[13] - 
				  m[8]  * m[1] * m[14] + 
				  m[8]  * m[2] * m[13] + 
				  m[12] * m[1] * m[10] - 
				  m[12] * m[2] * m[9];

		inv[2] = m[1]  * m[6] * m[15] - 
				 m[1]  * m[7] * m[14] - 
				 m[5]  * m[2] * m[15] + 
				 m[5]  * m[3] * m[14] + 
				 m[13] * m[2] * m[7] - 
				 m[13] * m[3] * m[6];

		inv[6] = -m[0]  * m[6] * m[15] + 
				  m[0]  * m[7] * m[14] + 
				  m[4]  * m[2] * m[15] - 
				  m[4]  * m[3] * m[14] - 
				  m[12] * m[2] * m[7] + 
				  m[12] * m[3] * m[6];

		inv[10] = m[0]  * m[5] * m[15] - 
				  m[0]  * m[7] * m[13] - 
				  m[4]  * m[1] * m[15] + 
				  m[4]  * m[3] * m[13] + 
				  m[12] * m[1] * m[7] - 
				  m[12] * m[3] * m[5];

		inv[14] = -m[0]  * m[5] * m[14] + 
				   m[0]  * m[6] * m[13] + 
				   m[4]  * m[1] * m[14] - 
				   m[4]  * m[2] * m[13] - 
				   m[12] * m[1] * m[6] + 
				   m[12] * m[2] * m[5];

		inv[3] = -m[1] * m[6] * m[11] + 
				  m[1] * m[7] * m[10] + 
				  m[5] * m[2] * m[11] - 
				  m[5] * m[3] * m[10] - 
				  m[9] * m[2] * m[7] + 
				  m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] - 
				 m[0] * m[7] * m[10] - 
				 m[4] * m[2] * m[11] + 
				 m[4] * m[3] * m[10] + 
				 m[8] * m[2] * m[7] - 
				 m[8] * m[3] * m[6];

		inv[11] = -m[0] * m[5] * m[11] + 
				   m[0] * m[7] * m[9] + 
				   m[4] * m[1] * m[11] - 
				   m[4] * m[3] * m[9] - 
				   m[8] * m[1] * m[7] + 
				   m[8] * m[3] * m[5];

		inv[15] = m[0] * m[5] * m[10] - 
				  m[0] * m[6] * m[9] - 
				  m[4] * m[1] * m[10] + 
				  m[4] * m[2] * m[9] + 
				  m[8] * m[1] * m[6] - 
				  m[8] * m[2] * m[5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0)
			return false;

		det = 1.0 / det;

		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;

		return true;
	}
	*/
	// multiplie un vecteur colonne par une matrice
	// l'ordre implicite est la post-multiplication
    Vector4<Type> operator *(const Vector4<Type>& rhs) const
    {
        Vector4<Type> v;
        v.x = I.x * rhs.x + J.x * rhs.y + K.x * rhs.z + T.x * rhs.w;
        v.y = I.y * rhs.x + J.y * rhs.y + K.y * rhs.z + T.y * rhs.w;
        v.z = I.z * rhs.x + J.z * rhs.y + K.z * rhs.z + T.z * rhs.w;
        v.w = I.w * rhs.x + J.w * rhs.y + K.w * rhs.z + T.w * rhs.w;
        return v;
    }

	union
	{
		struct
		{
			vec4 I;
			vec4 J;
			vec4 K;
			vec4 T;
		};
		float m[4][4];
	};
};


typedef Matrix2<float> mat2;
typedef Matrix3<float> mat3;
typedef Matrix4<float> mat4;

// --- helpers
mat4 esgiMultiplyMatrix(const mat4 &A, const mat4 &B);

// rotations
mat4 esgiRotateX(float angle);
mat4 esgiRotateY(float angle);
mat4 esgiRotateZ(float angle);

// camera ---
mat4 esgiLookAt(const vec3 &eye, const vec3 &target, const vec3 &up);

// projection ---
mat4 esgiOrtho(float left, float right, float bottom, float top, float near, float far);
mat4 esgiFrustum(float left, float right, float bottom, float top, float near, float far);
mat4 esgiPerspective(float fovy, float aspect, float near, float far);
vec4 esgiPerspectiveFrustum(float fovy, float aspect, float near, float far);


#endif // ESGI_MATHS_MATRIX