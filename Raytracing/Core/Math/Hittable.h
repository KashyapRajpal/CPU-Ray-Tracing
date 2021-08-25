//
//  Hittable.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 3/2/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef Hittable_h
#define Hittable_h

#include "Utils.h"
#include "Ray.h"
#include "Material.h"
//class Material;

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

class Hittable
{
    public:
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};

#endif /* Hittable_h */
