//
//  Vector.hpp
//  Raytracing
//
//  Created by Kashyap Rajpal on 2/17/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef Vector_h
#define Vector_h

#include <stdio.h>
#include <iostream>
#include <math.h>
#include "Utils.h"

class Vector3
{
public:
    Vector3() = default;
    Vector3(const Vector3& other)
    {m_value[0] = other.X(); m_value[1] = other.Y(); m_value[2] = other.Z();}
    Vector3(const float value1, const float value2, const float value3)
    {m_value[0] = value1; m_value[1] = value2; m_value[2] = value3;}
    inline float X() const { return m_value[0]; }
    inline float Y() const { return m_value[1]; }
    inline float Z() const { return m_value[2]; }
    inline float R() const { return m_value[0]; }
    inline float G() const { return m_value[1]; }
    inline float B() const { return m_value[2]; }

    inline const Vector3& operator+() const { return *this; }
    inline Vector3 operator-() const { return Vector3(-X(), -Y(), -Z()); }

    inline float operator[](int index) { return m_value[index];}
    inline Vector3& operator+=(const Vector3 &other);
    inline Vector3& operator-=(const Vector3 &other);
    inline Vector3& operator*=(const Vector3 &other);
    inline Vector3& operator/=(const Vector3 &other);
    inline Vector3& operator*=(const float t);
    inline Vector3& operator/=(const float t);

    inline float Length() const { return sqrt(SquaredLength()); }
    inline float SquaredLength() const { return X() * X() + Y() * Y() + Z() * Z(); }
    inline void MakeUnitVector();
    inline void WriteColor(std::ostream& out, int samplesPerPixel);
    inline static Vector3 Random();
    inline static Vector3 Random(double min, double max);
    inline static Vector3 GetRandomUnitVector();
    inline static Vector3 Reflect(const Vector3& vector_in, const Vector3& normal);
    inline static Vector3 GetZero();
    inline static Vector3 RandomInUnitSphere();
    inline static Vector3 RandomInHemiSphere(Vector3& normal);
    inline static Vector3 Refract(const Vector3& uv, const Vector3& n, double etai_over_etat);
    inline static Vector3 RandomInUnitDisk();
    inline void WriteColor(std::string& out, int samplesPerPixel);
    
    float m_value[3];
};

inline std::istream& operator>>(std::istream &is, Vector3 &t)
{
    is >> t.m_value[0] >> t.m_value[1] >> t.m_value[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vector3 &t)
{
    os << t.X() << " " << t.Y() << " " << t.Z();
    return os;
}

inline void Vector3::MakeUnitVector()
{
    float k = 1.0 / sqrt(SquaredLength());
    m_value[0] *= k; m_value[1] *= k; m_value[2] *= k;
}

inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.X() + v2.X(), v1.Y() + v2.Y(), v1.Z() + v2.Z());
}

inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.X() - v2.X(), v1.Y() - v2.Y(), v1.Z() - v2.Z());
}

inline Vector3 operator*(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.X() * v2.X(), v1.Y() * v2.Y(), v1.Z() * v2.Z());
}

inline Vector3 operator*(float t, const Vector3 &v)
{
    return Vector3(t * v.X(), t * v.Y(), t * v.Z());
}

inline Vector3 operator*(const Vector3 &v, float t)
{
    return Vector3(t * v.X(), t * v.Y(), t * v.Z());
}

inline Vector3 operator/(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.X() / v2.X(), v1.Y() / v2.Y(), v1.Z() / v2.Z());
}

inline Vector3 operator/(Vector3 v, float t)
{
    return Vector3(v.X() / t, v.Y() / t, v.Z() / t);
}

inline float dot(const Vector3 &v1, const Vector3 &v2)
{
    return v1.X() * v2.X()
         + v1.Y() * v2.Y()
         + v1.Z() * v2.Z();
}

inline Vector3 cross(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.Y() * v2.Z() - v1.Z() * v2.Y(),
                v1.Z() * v2.X() - v1.X() * v2.Z(),
                v1.X() * v2.Y() - v1.Y() * v2.X());
}

inline Vector3& Vector3::operator+=(const Vector3 &v)
{
    m_value[0] += v.X();
    m_value[1] += v.Y();
    m_value[2] += v.Z();
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& v)
{
    m_value[0] -= v.X();
    m_value[1] -= v.Y();
    m_value[2] -= v.Z();
    return *this;
}

inline Vector3& Vector3::operator*=(const Vector3 &v)
{
    m_value[0] *= v.X();
    m_value[1] *= v.Y();
    m_value[2] *= v.Z();
    return *this;
}

inline Vector3& Vector3::operator*=(const float t)
{
    m_value[0] *= t;
    m_value[1] *= t;
    m_value[2] *= t;
    return *this;
}

inline Vector3& Vector3::operator/=(const Vector3 &v)
{
    m_value[0] /= v.X();
    m_value[1] /= v.Y();
    m_value[2] /= v.Z();
    return *this;
}

inline Vector3& Vector3::operator/=(const float t)
{
    
    if (t != 0.f)
    {
        float k = 1.0 / t;
        *this *= k;
    }
    
    return *this;
}

inline Vector3 unit_vector(Vector3 v)
{
    return v / v.Length();
}

inline void Vector3::WriteColor(std::ostream &out, int samplesPerPixel) {
    // Divide the color total by the number of samples.
    auto scale = 1.0 / samplesPerPixel;
    
    // Gamma correct color by factor of 2.0
    auto r = sqrt(scale * m_value[0]);
    auto g = sqrt(scale * m_value[1]);
    auto b = sqrt(scale * m_value[2]);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * Clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * Clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * Clamp(b, 0.0, 0.999)) << '\n';
}

inline void Vector3::WriteColor(std::string& out, int samplesPerPixel)
{
    // Divide the color total by the number of samples.
    auto scale = 1.0 / samplesPerPixel;
    
    // Gamma correct color by factor of 2.0
    auto r = sqrt(scale * m_value[0]);
    auto g = sqrt(scale * m_value[1]);
    auto b = sqrt(scale * m_value[2]);

    // Write the translated [0,255] value of each color component.
    out.append(std::to_string(static_cast<int>(256 * Clamp(r, 0.0, 0.999))));
    out.append(" ");
    out.append(std::to_string(static_cast<int>(256 * Clamp(g, 0.0, 0.999))));
    out.append(" ");
    out.append(std::to_string(static_cast<int>(256 * Clamp(b, 0.0, 0.999))));
    out.append("\n");
}

inline Vector3 Vector3::Random()
{
    return Vector3(RandomDouble(), RandomDouble(), RandomDouble());
}

inline Vector3 Vector3::Random(double min, double max)
{
    return Vector3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
}

inline Vector3 Vector3::Reflect(const Vector3& v, const Vector3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline Vector3 Vector3::GetZero()
{
    return Vector3(0.f, 0.f, 0.f);
}

inline Vector3 Vector3::GetRandomUnitVector()
{
    Vector3 newVector = Vector3::Random();
    newVector.MakeUnitVector();
    return newVector;
}

Vector3 Vector3::RandomInUnitSphere()
{
    double angle = RandomDouble(0, 2 * s_kPI);
    double z = RandomDouble(-1, 1);
    double r  = sqrt(1- z * z);
    Vector3 p = Vector3(r * cos(angle), r * sin(angle), z);
    return p;
}

Vector3 Vector3::RandomInHemiSphere(Vector3& normal)
{
    Vector3 randomInHemiSphere = RandomInUnitSphere();
    if (dot(randomInHemiSphere, normal) > 0)
    {
        return randomInHemiSphere;
    }
    
    return -randomInHemiSphere;
}

inline Vector3 Vector3::Refract(const Vector3& uv, const Vector3& n, double etai_over_etat)
{
    double cosTheta = dot(-uv, n);
    Vector3 r_out_parallel =  etai_over_etat * (uv + cosTheta * n);
    Vector3 r_out_perp = -sqrt(1.0 - r_out_parallel.SquaredLength()) * n;
    return r_out_parallel + r_out_perp;
}

inline Vector3 Vector3::RandomInUnitDisk()
{
    while (true)
    {
        Vector3 p = Vector3(RandomDouble(-1,1), RandomDouble(-1,1), 0);
        if (p.SquaredLength() >= 1)
            continue;
        
        return p;
    }
}
#endif /* Vector_h */
