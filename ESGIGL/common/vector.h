// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_MATHS_VECTOR
#define ESGI_MATHS_VECTOR

// --- Includes --------------------------------------------------------------

#include <cmath>

// --- Classes ---------------------------------------------------------------

// --- Vector2

template <typename T>
struct Vector2 
{
    Vector2() {}
    Vector2(T x, T y) : x(x), y(y) {}

    T Dot(const Vector2& v) const
    {
        return x * v.x + y * v.y;
    }

    Vector2 operator+(const Vector2& v) const
    {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2 operator-(const Vector2& v) const
    {
        return Vector2(x - v.x, y - v.y);
    }

    void operator+=(const Vector2& v)
    {
        *this = Vector2(x + v.x, y + v.y);
	}

    void operator-=(const Vector2& v)
    {
        *this = Vector2(x - v.x, y - v.y);
    }

    Vector2 operator/(float s) const
    {
        return Vector2(x / s, y / s);
    }

    Vector2 operator*(float s) const
    {
        return Vector2(x * s, y * s);
    }

    void operator/=(float s)
    {
        *this = Vector2(x / s, y / s);
    }

    void operator*=(float s)
    {
        *this = Vector2(x * s, y * s);
    }

    void Normalize()
    {
        float s = 1.0f / Length();
        x *= s;
        y *= s;
    }

    Vector2 Normalized() const 
    {
        Vector2 v = *this;
        v.Normalize();
        return v;
    }

    T LengthSquared() const
    {
        return x * x + y * y;
    }

    T Length() const
    {
        return sqrt(LengthSquared());
    }

    operator Vector2<float>() const
    {
        return Vector2<float>(x, y);
    }

    bool operator==(const Vector2& v) const
    {
        return x == v.x && y == v.y;
    }

    T x, y;
};

// --- Vector3

template <typename T>
struct Vector3 
{
    Vector3() {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    T Length()
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    T LengthSquared()
    {
        return (x * x + y * y + z * z);
    }

    void Normalize()
    {
        float s = 1.0f / Length();
        x *= s;
        y *= s;
        z *= s;
    }

    Vector3 Normalized() const 
    {
        Vector3 v = *this;
        v.Normalize();
        return v;
    }

    Vector3 NormalizedSafe() const 
    {
        Vector3 v = *this;
		if (v.LengthSquared() != 0.f)
			v.Normalize();
		else
			v = Vector3(0.f, 0.f, 0.f);
        return v;
    }

    Vector3 Cross(const Vector3& v) const
    {
        return Vector3(y * v.z - z * v.y,
                       z * v.x - x * v.z,
                       x * v.y - y * v.x);
    }

    T Dot(const Vector3& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 operator+(const Vector3& v) const
    {
        return Vector3(x + v.x, y + v.y,  z + v.z);
    }

    void operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    void operator-=(const Vector3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    void operator/=(T s)
    {
        x /= s;
        y /= s;
        z /= s;
    }

    Vector3 operator-(const Vector3& v) const
    {
        return Vector3(x - v.x, y - v.y,  z - v.z);
    }

    Vector3 operator-() const
    {
        return Vector3(-x, -y, -z);
    }

    Vector3 operator*(T s) const
    {
        return Vector3(x * s, y * s, z * s);
    }

    Vector3 operator/(T s) const
    {
        return Vector3(x / s, y / s, z / s);
    }

    bool operator==(const Vector3& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    T x, y, z;
};


// --- Vector4

template <typename T>
struct Vector4 
{
    //Vector4() {}
    //Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    //Vector4(const Vector3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

    void set(T _x, T _y, T _z, T _w) { x = _x; y = _y; z = _z; w = _w; };
    void set(const Vector3<T>& v, T _w) { x = v.x; y = v.y; z = v.z; w = _w; };
    
 
    Vector4 operator+(const Vector4& v) const
    {
		return Vector4(x + v.x, y + v.y,  z + v.z, w + v.w);
    }

    T Dot(const Vector4& v) const
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    T x, y, z, w;
};


typedef Vector2<bool> bvec2;

typedef Vector2<int> ivec2;
typedef Vector3<int> ivec3;
typedef Vector4<int> ivec4;

typedef Vector2<float> vec2;
typedef Vector3<float> vec3;
typedef Vector4<float> vec4;

#endif // ESGI_MATHS_VECTOR