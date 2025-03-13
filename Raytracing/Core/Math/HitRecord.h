#ifndef HitRecord_h
#define HitRecord_h

#include "Ray.h"
#include "Vector.h"

class Material;

struct HitRecord
{
    bool     m_frontFace;
    float    m_t;
    Vector3  m_point;
    Vector3  m_normal;
    
    shared_ptr<Material> m_material;
    
    inline void SetFaceNormal(const Ray& r, const Vector3& outward_normal)
    {
        m_frontFace = dot(r.GetDirection(), outward_normal) < 0;
        m_normal = m_frontFace ? outward_normal : -outward_normal;
    }
};

#endif